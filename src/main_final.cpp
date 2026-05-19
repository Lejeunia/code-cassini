
#include <Arduino.h>
#include "config.h"
#include "US.h"
#include "servos.h"
#include "stepper.h"
#include <AccelStepper.h>
#include "NRF.h"
#include "TOF.h"
#include <Wire.h>

extern AccelStepper m1;
extern AccelStepper m2;
extern AccelStepper m3;
extern AccelStepper m4;

TOF tof(&Wire);

unsigned long dernierChronoCapteurs = 0;
const unsigned long INTERVALLE_LECTURE = 100; // On vérifie les capteurs toutes les 100 ms
static uint8_t capteurActif = 0;  // 0 = gauche, 1 = droite
static float distG = 999, distD = 999;
NRF_Comm nrf(NRF_cePin, NRF_csnPin);  
bool obstacleDetecte = false; 


// CONFIGURATION DES DIRECTIONS : 0 = Tout droit, 1 = Droite, 2 = Gauche
uint8_t directionActuelle = 0; 

void cmdAvancer();
void cmdReculer();
void cmdGauche(int angle = 90);
void cmdDroite(int angle = 90);
void cmdStop();
void traiterCommande(const String& cmd);

void setup() {
    Serial.begin(9600);
    while (!Serial) { delay(10); }
 
    Serial.println(F("=== DÉMARRAGE DU SYSTÈME ==="));
 
    setupMotors();
    setupServos();
    setupUltrasons();
 
    if (!nrf.begin()) {
        Serial.println(F("Arrêt — NRF non initialisé."));
        while (true);
    }
 
    Serial.println(F("=== EN ATTENTE DE COMMANDES NRF ==="));
    Serial.println(F("Commandes : start | avancer | reculer | gauche | droite | stop"));
}

void loop() {
    // 1. Faire tourner les moteurs (non-bloquant)
    m1.run();
    m2.run();
    m3.run();
    m4.run();

    /*
    // DEBUG TEMPORAIRE — à supprimer après
    static unsigned long dernierDebug = 0;
    if (millis() - dernierDebug >= 500) {
        dernierDebug = millis();
        Serial.print(F("[DEBUG] Servo1="));
        Serial.print(messervos[0].read());
        Serial.print(F(" Servo2="));
        Serial.print(messervos[1].read());
        Serial.print(F(" Servo3="));
        Serial.print(messervos[2].read());
        Serial.print(F(" Servo4="));
        Serial.println(messervos[3].read());
    }*/
 
    // 2. Lecture alternée des ultrasons
    if (!cmdReculer){
        if (millis() - dernierChronoCapteurs >= INTERVALLE_LECTURE) {
            dernierChronoCapteurs = millis();
    
            if (capteurActif == 0) {
                distG = getDistance(PIN_TRIG_g, PIN_ECHO_g);
                capteurActif = 1;
            } else {
                distD = getDistance(PIN_TRIG_d, PIN_ECHO_d);
                capteurActif = 0;
            }
    
            // Obstacle détecté à moins de 20 cm → arrêt immédiat
            if ((distG > 0 && distG < 20) || (distD > 0 && distD < 20)) {
                cmdStop(); // Utiliser cmdStop remet aussi les servos droits proprement
                Serial.println(F("[US] Obstacle détecté — moteurs arrêtés"));
            }
        }
    }

 
    // 3. Réception et traitement des commandes NRF
    nrf.update();
    while (nrf.hasCommand()) {
        String cmd = nrf.readCommand();
        cmd.trim();
        cmd.toLowerCase();
        Serial.print(F("[NRF] Reçu : "));
        Serial.println(cmd);
        traiterCommande(cmd);
    }
}

// ============================================================
//  Fonctions de mouvement
// ============================================================
 
void cmdAvancer() {
    Serial.println(F("[CMD] Avancer"));
    directionActuelle = 0; // Mode Tout Droit
    
    ecrireAngleServo(1, 95);
    ecrireAngleServo(2, 93);
    ecrireAngleServo(3, 99);
    ecrireAngleServo(4, 95);
 
    m1.moveTo(m1.currentPosition() + 10000);
    m2.moveTo(m2.currentPosition() + 10000);
    m3.moveTo(m3.currentPosition() + 10000);
    m4.moveTo(m4.currentPosition() + 10000);
}
 
void cmdReculer() {
    Serial.println(F("[CMD] Reculer"));
    directionActuelle = 0; // Mode Tout Droit
    
    ecrireAngleServo(1, 95);
    ecrireAngleServo(2, 93);
    ecrireAngleServo(3, 99);
    ecrireAngleServo(4, 95);
 
    m1.moveTo(m1.currentPosition() - 10000);
    m2.moveTo(m2.currentPosition() - 10000);
    m3.moveTo(m3.currentPosition() - 10000);
    m4.moveTo(m4.currentPosition() - 10000);
}
 
void cmdDroite(int angle = 90) {
    Serial.print(F("[CMD] Rotation droite "));
    Serial.print(angle);
    Serial.println(F("°"));

    long pas = ((long)PAS_ROTATION_360 * angle )/ 360.0;
    directionActuelle = 2;

    ecrireAngleServo(1, 45);
    ecrireAngleServo(2, 135);
    ecrireAngleServo(3, 135);
    ecrireAngleServo(4, 45);
    delay(400);

    m1.moveTo(m1.currentPosition() - pas);
    m2.moveTo(m2.currentPosition() + pas);
    m3.moveTo(m3.currentPosition() - pas);
    m4.moveTo(m4.currentPosition() + pas);

    while (m1.isRunning() || m2.isRunning() || m3.isRunning() || m4.isRunning()) {
        m1.run(); m2.run(); m3.run(); m4.run();
        /*
        if (millis() - dernierChronoCapteurs >= INTERVALLE_LECTURE) {
            dernierChronoCapteurs = millis();
            distG = getDistance(PIN_TRIG_g, PIN_ECHO_g);
            distD = getDistance(PIN_TRIG_d, PIN_ECHO_d);
            if ((distG > 0 && distG < 20) || (distD > 0 && distD < 20)) {
                cmdStop();
                Serial.println(F("[US] Obstacle — arrêt"));
                return;
            }
        }*/
    }

    ecrireAngleServo(1, 95);
    ecrireAngleServo(2, 93);
    ecrireAngleServo(3, 99);
    ecrireAngleServo(4, 95);
    directionActuelle = 0;

    distD = getDistance(PIN_TRIG_d, PIN_ECHO_d);
    distG = getDistance(PIN_TRIG_g, PIN_ECHO_g);

    if (distD > 20 && distG > 20) {
        cmdAvancer();
        return;   
    }
}

void cmdGauche(int angle = 90) {
    Serial.print(F("[CMD] Rotation gauche "));
    Serial.print(angle);
    Serial.println(F("°"));

    long pas = ((long)PAS_ROTATION_360 * angle )/ 360.0;
    directionActuelle = 1;

    ecrireAngleServo(1, 45);
    ecrireAngleServo(2, 135);
    ecrireAngleServo(3, 135);
    ecrireAngleServo(4, 45);
    delay(400);

    m1.moveTo(m1.currentPosition() + pas);
    m2.moveTo(m2.currentPosition() - pas);
    m3.moveTo(m3.currentPosition() + pas);
    m4.moveTo(m4.currentPosition() - pas);

    while (m1.isRunning() || m2.isRunning() || m3.isRunning() || m4.isRunning()) {
        m1.run(); m2.run(); m3.run(); m4.run();
        /*
        if (millis() - dernierChronoCapteurs >= INTERVALLE_LECTURE) {
            dernierChronoCapteurs = millis();
            distG = getDistance(PIN_TRIG_g, PIN_ECHO_g);
            distD = getDistance(PIN_TRIG_d, PIN_ECHO_d);
            if ((distG > 0 && distG < 20) || (distD > 0 && distD < 20)) {
                cmdStop();
                Serial.println(F("[US] Obstacle — arrêt"));
                return;
            }
        }*/
    }

    ecrireAngleServo(1, 95);
    ecrireAngleServo(2, 93);
    ecrireAngleServo(3, 99);
    ecrireAngleServo(4, 95);
    directionActuelle = 0;

    distD = getDistance(PIN_TRIG_d, PIN_ECHO_d);
    distG = getDistance(PIN_TRIG_g, PIN_ECHO_g);


    if (distD > 20 && distG > 20) {
        cmdAvancer();
        return;   
    }

   
}
 
void cmdStop() {
    Serial.println(F("[CMD] Stop"));
    directionActuelle = 0;

    m1.stop();
    m2.stop();
    m3.stop();
    m4.stop();
 
    ecrireAngleServo(1, 95);
    ecrireAngleServo(2, 93);
    ecrireAngleServo(3, 99);
    ecrireAngleServo(4, 95);
}
 
// ============================================================
//  Interpréteur de commandes NRF
// ============================================================
 
void traiterCommande(const String& cmd) {
    if (cmd == "start" || cmd == "avancer") {
        cmdAvancer();
    }
    else if (cmd == "reculer" || cmd == "recule") {
        cmdReculer();
    }
    else if (cmd.startsWith("gauche")) {
        int angle = 90; // valeur par défaut
        if (cmd.length() > 6) {
            String valeur = cmd.substring(6);
            valeur.trim();
            angle = valeur.toInt();
            if (angle <= 0 || angle > 360) angle = 90; // sécurité
        }
        cmdGauche(angle);
    }
    else if (cmd.startsWith("droite")) {
        int angle = 90;
        if (cmd.length() > 6) {
            String valeur = cmd.substring(6);
            valeur.trim();
            angle = valeur.toInt();
            if (angle <= 0 || angle > 360) angle = 90;
        }
        cmdDroite(angle);
    }

    else if (cmd == "stop" || cmd == "arret") {
        cmdStop();
    }
    else {
        Serial.print(F("[NRF] Commande inconnue : "));
        Serial.println(cmd);
    }
}    
