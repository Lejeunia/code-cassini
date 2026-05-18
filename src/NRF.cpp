#include "NRF.h"
#include "config.h"

NRF_Comm nrf(32, 53);

// ─────────────────────────────────────────────
// CONSTRUCTEUR
// ─────────────────────────────────────────────
NRF_Comm::NRF_Comm(uint8_t cePin, uint8_t csnPin) // constructeur avec pins CE et CSN
    : _radio(cePin, csnPin),   // initialise le module radio NRF24L01 avec les pins
      _rawLen(0),              // initialise la taille du buffer brut à 0
      _cmdHead(0),             // index lecture FIFO à 0
      _cmdTail(0),             // index d'écriture FIFO à 0
      _cmdCount(0)             // nombre de commandes stockées à 0
{
    memset(_rawBuf, 0, sizeof(_rawBuf)); // met tout le buffer à zéro (nettoyage mémoire)
}

// ─────────────────────────────────────────────
// INITIALISATION RADIO
// ─────────────────────────────────────────────
bool NRF_Comm::begin() {

    if (!_radio.begin()) {      // vérifie si le module répond
        Serial.println("[NRF] ERREUR : module non détecté. Vérifie le câblage SPI.");   // erreur si module non détecté
        return false;       // échec initialisation
    }

    _radio.setChannel(NRF_CHANNEL);          // canal 100 (Atlas V)
    _radio.setDataRate(NRF_DATA_RATE);        // 250 KBPS (imposé par le Hub)
    _radio.setPALevel(NRF_PA_LEVEL);          // puissance max
    _radio.setPayloadSize(NRF_PAYLOAD_SIZE);  // paquets fixes 32 octets
    _radio.setAutoAck(false);                 // pas d'ACK (Hub n'attend pas de réponse)
    _radio.disableDynamicPayloads();          // payload fixe

    _radio.openReadingPipe(1, NRF_PIPE_ADDRESS); // écoute sur l'adresse Atlas V
    _radio.startListening();                     // mode réception
    
    _radio.flush_rx(); // ← part d'un état propre   //ajout claude
    Serial.println("[NRF] OK — écoute sur canal 100, adresse 0xE8E8F0F0A1");
    return true;
}

// ─────────────────────────────────────────────
// LECTURE RADIO — à appeler dans loop()
// ─────────────────────────────────────────────
bool NRF_Comm::update() {

    if (!_radio.available()) return false;

    uint8_t packet[32];
    bool got = false;
    int maxPackets = 5;

    while (_radio.available() && maxPackets-- > 0) {

        _radio.read(packet, 32);
        /*
        // debug — affiche seulement si non-vide
        bool hasData = false;
        for (int i = 0; i < 32; i++) {
            if (packet[i] != 0) { hasData = true; break; }
        }
        
        if (hasData) {
            Serial.print("[RAW] ");
            for (int i = 0; i < 32; i++) {
                if (packet[i] >= 32 && packet[i] < 127)
                    Serial.print((char)packet[i]);
                else {
                    Serial.print("\\x");
                    Serial.print(packet[i], HEX);
                }
            }
            Serial.println();
        }
        */

        // ignore les paquets entièrement à zéro
        bool empty = true;
        for (int i = 0; i < 32; i++) {
            if (packet[i] != 0) { empty = false; break; }
        }
        if (empty) continue;
        
        got = true;

        int space = (NRF_CMD_BUFFER_SIZE - 1) - _rawLen;
        if (space <= 0) {
            _rawLen = 0;
            space = NRF_CMD_BUFFER_SIZE - 1;
        }
        int toCopy = min(32, space);
        memcpy(_rawBuf + _rawLen, packet, toCopy);
        _rawLen += toCopy;
    }

    _radio.flush_rx();
    if (got) {
        int prevLen;
        do {
            prevLen = _rawLen;
            _parseBuffer();
        } while (_rawLen > 0 && _rawLen < prevLen);
    }

    return got;
}

// ─────────────────────────────────────────────
// PARSE BUFFER — découpe les commandes sur \n
// ─────────────────────────────────────────────
void NRF_Comm::_parseBuffer() {

    for (int i = 0; i < _rawLen; i++) {

        if (_rawBuf[i] == '\n' || _rawBuf[i] == '\r') {

            _enqueueCommand(_rawBuf, i);

            int remaining = _rawLen - (i + 1);
            memmove(_rawBuf, _rawBuf + i + 1, remaining);
            _rawLen = remaining;

            return; // une ligne par appel, l'appelant reboucle via update()
        }
        // stoppe au premier \0 — tout ce qui suit est padding
        if (_rawBuf[i] == '\0') {
            if (i > 0) _enqueueCommand(_rawBuf, i); // enqueue ce qu'il y a avant
            _rawLen = 0; // jette le reste (que des \0)
            return;
        }
    }
    
    if (_rawLen >= 32) {
        _enqueueCommand(_rawBuf, _rawLen);
        _rawLen = 0;
    }
    
}

// ─────────────────────────────────────────────
// AJOUT FIFO
// ─────────────────────────────────────────────
void NRF_Comm::_enqueueCommand(const char* start, int len) {

    if (_cmdCount >= 16) {
        Serial.println("[NRF] AVERTISSEMENT : file FIFO pleine, commande ignorée !");
        return;
    }

    String cmd = "";
    for (int i = 0; i < len; i++) {
        if (start[i] >= 32 && start[i] < 127) { // garde uniquement l'ASCII imprimable
            cmd += start[i];
        }
    }
    if (cmd.length() < 2) return;   // ignore les commandes trop courtes (ex: juste un \n)
    if (cmd.length() > 0) {
        _cmdQueue[_cmdTail] = cmd;
        _cmdTail = (_cmdTail + 1) % 16;
        _cmdCount++;
        Serial.println("[NRF] Reçu : " + cmd);
    }
}

// ─────────────────────────────────────────────
// INTERFACE PUBLIQUE
// ─────────────────────────────────────────────
bool NRF_Comm::hasCommand() const {
    return _cmdCount > 0;
}

String NRF_Comm::readCommand() {
    if (_cmdCount == 0) return "";
    String cmd = _cmdQueue[_cmdHead];
    _cmdHead = (_cmdHead + 1) % 16;
    _cmdCount--;
    return cmd;
}

