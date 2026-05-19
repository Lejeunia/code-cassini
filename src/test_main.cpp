#include <Arduino.h>
#include "config.h"
#include "US.h"
#include "servos.h"
#include "stepper.h"
#include <AccelStepper.h>
#include "NRF.h"
#include "TOF.h"

extern AccelStepper m1;
extern AccelStepper m2;
extern AccelStepper m3;
extern AccelStepper m4;


TOFSensor tof;

unsigned long dernierChronoCapteurs = 0;
const unsigned long INTERVALLE_LECTURE = 100; // On vérifie les capteurs toutes les 100 ms
static uint8_t capteurActif = 0;  // 0 = gauche, 1 = droite
static float distG = 999, distD = 999;
NRF_Comm nrf(NRF_cePin, NRF_csnPin);   




void setup() {
    
    Serial.begin(9600);

    //------------------------------------------------------------
    
    Serial.println("=== DEMARRAGE ===");
    if (!nrf.begin()) {
        Serial.println("Arrêt — NRF non initialisé.");
        while (true); // bloque si le module ne répond pas
    }
    //nrf.printDetails();
    Serial.println("=== EN ATTENTE DE PAQUETS ===");
    //-------------------------------------------------------------

    /*
    // 1. Initialisation matérielle (configure les pins et les vitesses)
    setupMotors(); //ampèrage alim = 2,38 A pour 4 steppers

    Serial.println("Commandes disponibles :");
    Serial.println("  1+500  → m1 avance de 500 pas");
    Serial.println("  1-500  → m1 recule de 500 pas");
    Serial.println("  2+500  → m2 avance de 500 pas");
    Serial.println("  2-500  → m2 recule de 500 pas");*/

    /*
    setupMotors();
    
    m1.moveTo(10000);
    m2.moveTo(10000);
    m3.moveTo(10000);
    m4.moveTo(10000);*/
    
    /*
    setupUltrasons();
    Serial.println("--- DEMARRAGE TEST ULTRASON ---");*/

    /*
    setupServos();
    Serial.println("-----DEMARRAGE TEST SERVOS------");
    Serial.println("Commande : numéro servo + angle (ex: 1+90)"); */
    //servo 1 : 95°   2 : 93°   3 : 99°   4 : 95°

    /*
    if (!tof.begin()) {
        Serial.println("Capteur non détecté.");
        while (true);
    }*/

}

void loop() {

    
    nrf.update();
    while (nrf.hasCommand()) {
        String cmd = nrf.readCommand();
        Serial.println("CMD: " + cmd);
    }


    /*
    m1.run();   //jaune
    m2.run();   //noir
    m3.run();   //blanc
    m4.run();   //vert

    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        int moteur = cmd.charAt(0) - '0'; // 1 ou 2
        long pas = cmd.substring(1).toInt(); // ex: +500 ou -500

        // Coupe le courant dans les bobines une fois arrivé
        m1.disableOutputs();
        m2.disableOutputs();
        m3.disableOutputs();
        m4.disableOutputs();

        if (moteur == 1) { 
            m1.move(pas);
            Serial.print("M1 → ");
            Serial.println(pas);
        } else if (moteur == 2) {
            m2.move(pas);
            Serial.print("M2 → ");
            Serial.println(pas);
        } else if (moteur == 3) {
            m3.move(pas);
            Serial.print("M3 → ");
            Serial.println(pas);
        } else if (moteur == 4) {
            m4.move(pas);
            Serial.print("M4 → ");
            Serial.println(pas);
        } else {
            Serial.println("Commande invalide");
        }
    }*/

    /*
    //-────────────────────────────────────────────
    // 1. Moteur tourne en continu 
    m1.run();
    m2.run();
    m3.run();
    m4.run();

    // 2. Lecture alternée des capteurs (un seul par cycle)
    if (millis() - dernierChronoCapteurs >= INTERVALLE_LECTURE) {
        dernierChronoCapteurs = millis();

        if (capteurActif == 0) {
            distG = getDistance(PIN_TRIG_g, PIN_ECHO_g);
            capteurActif = 1;
        } else {
            distD = getDistance(PIN_TRIG_d, PIN_ECHO_d);
            capteurActif = 0;
        }

        // 3. Décision basée sur les deux dernières lectures
        if ((distG > 0 && distG < 20) || (distD > 0 && distD < 20)) {
            m1.stop();
            m3.stop();
            m4.stop();
            Serial.println("Obstacle !");
        } else {
            // Redonner la cible si le moteur est arrêté
            if ((m4.distanceToGo() == 0)||(m3.distanceToGo() == 0)||(m1.distanceToGo() == 0)) {
                m1.moveTo(m1.currentPosition() + 10000);
                m3.moveTo(m3.currentPosition() + 10000);
                m4.moveTo(m4.currentPosition() + 10000);
            }
        }
    }*/
   //-────────────────────────────────────────────

    /*
    float dG = getDistance(PIN_TRIG_g, PIN_ECHO_g);
    float dD = getDistance(PIN_TRIG_d, PIN_ECHO_d);
    
    
    Serial.print("Gauche :");
    Serial.print(dG);
    Serial.println(" cm");
    
    
    Serial.print("Droite :");
    Serial.print(dD);
    Serial.println(" cm");*/

    /*
    if (Serial.available()) { //vérifie si les données sont arrivées sur le port série
        String cmd = Serial.readStringUntil('\n'); //commence la lecture du moniteur série j-> retour ligne 
        cmd.trim(); //supprime espaces 

        // Commande direction : "droite", "gauche" ou "toutdroit"
        if (cmd == "droite") {
            ecrireAngleServo(1, 135);
            ecrireAngleServo(2, 135);
            ecrireAngleServo(3, 45);
            ecrireAngleServo(4, 45);
            Serial.println("Virage droite");
        }
        else if (cmd == "gauche") {
            ecrireAngleServo(1, 45);
            ecrireAngleServo(2, 45);
            ecrireAngleServo(3, 135);
            ecrireAngleServo(4, 135);
            Serial.println("Virage gauche");
        }
        else if (cmd == "toutdroit") {
            ecrireAngleServo(1, 95);
            ecrireAngleServo(2, 93);
            ecrireAngleServo(3, 99);
            ecrireAngleServo(4, 95);
            Serial.println("Tout droit");
        }

        // Commande groupée : "avant+angle" ou "arriere+angle"
        if (cmd.startsWith("avant+") || cmd.startsWith("arriere+")) {
            int angle = cmd.substring(cmd.indexOf('+') + 1).toInt();

            if (angle >= 0 && angle <= 180) {
                if (cmd.startsWith("avant+")) {
                    ecrireAngleServo(1, angle);
                    ecrireAngleServo(2, angle);
                    Serial.print("Servos avant → ");
                } else {
                    ecrireAngleServo(3, angle);
                    ecrireAngleServo(4, angle);
                    Serial.print("Servos arrière → ");
                }
                Serial.print(angle);
                Serial.println("°");
            } else {
                Serial.println("Angle invalide (0-180)");
            }
        }
        // Commande individuelle : "1+90"
        else {
            int numServo = cmd.charAt(0) - '0';
            int angle = cmd.substring(2).toInt();

            if (numServo >= 1 && numServo <= 4 && angle >= 0 && angle <= 180) {
                ecrireAngleServo(numServo, angle);
                Serial.print("Servo ");
                Serial.print(numServo);
                Serial.print(" → ");
                Serial.print(angle);
                Serial.println("°");
            } else {
                Serial.println("Angle invalide (0-180) ou servo invalide (1-4)");
            }
        }
    }*/


    /*
    //contrôle servos
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        int numServo = cmd.charAt(0) - '0';
        int angle = cmd.substring(2).toInt(); // saute le '+'

        if (numServo >= 1 && numServo <= 4 && angle >= 0 && angle <= 180) {
            ecrireAngleServo(numServo, angle);
            Serial.print("Servo ");
            Serial.print(numServo);
            Serial.print(" → ");
            Serial.print(angle);
            Serial.println("°");
        } 
        else {
            Serial.println("Angle invalide (0-180) ou servo invalide (1-4)");
        }
    }*/


    /*
    if (tof.mesurer()) {
        Serial.print("Distance : ");
        Serial.print(tof.getDistance());
        Serial.println(" mm");
        tof.reagir();
    }*/

}

    
    
    
