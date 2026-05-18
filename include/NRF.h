
#ifndef NRF_H
#define NRF_H
#include "config.h"
#include <Arduino.h>

#include <RF24.h>   // bibliothèque du module NRF24L01
#include <SPI.h>    // communication SPI (Arduino Mega : MOSI=51, MISO=50, SCK=52)



// ─────────────────────────────────────────────
// CONFIGURATION RADIO — Groupe 2 : CASSINI-TX
// ─────────────────────────────────────────────
#define NRF_CHANNEL 104                              
static const uint64_t NRF_PIPE_ADDRESS = 0xE8E8F0F0A2; 

#define NRF_DATA_RATE     RF24_250KBPS  // débit imposé par le Hub
#define NRF_PA_LEVEL      RF24_PA_MAX   // puissance maximale
#define NRF_PAYLOAD_SIZE  32            // taille fixe des paquets (32 octets)

#define NRF_CMD_BUFFER_SIZE 256         // buffer pour reconstruire les messages




// ─────────────────────────────────────────────
// CLASSE DE COMMUNICATION NRF
// ─────────────────────────────────────────────
class NRF_Comm {

public:
    NRF_Comm(uint8_t cePin, uint8_t csnPin); // constructeur (CE et CSN)

    bool begin();         // initialise le module NRF24L01
    bool update();        // lit les données reçues et met à jour le buffer

    bool hasCommand() const; // vérifie si une commande est disponible
    String readCommand();    // récupère la prochaine commande reçue
    
    void printDetails() { _radio.printDetails(); }
    bool radioAvailable() { return _radio.available(); }

private:

    RF24 _radio;              // objet radio NRF24L01

    char _rawBuf[NRF_CMD_BUFFER_SIZE]; // buffer brut des données reçues
    int  _rawLen;                      // longueur actuelle du buffer

    String  _cmdQueue[16];  // file FIFO de commandes (16 slots max)
    uint8_t _cmdHead;       // index de lecture (tête FIFO)
    uint8_t _cmdTail;       // index d'écriture (queue FIFO)
    uint8_t _cmdCount;      // nombre de commandes en attente

    void _parseBuffer();    // analyse le buffer et extrait les commandes
    void _enqueueCommand(const char* start, int len); // ajoute une commande dans la file
};

extern NRF_Comm nrf;
#endif