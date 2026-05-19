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




void setup() {
    
    Serial.begin(9600);

    /*
    //------------------------------------------------------------
    
    Serial.println("=== DEMARRAGE ===");
    if (!nrf.begin()) {
        Serial.println("Arrêt — NRF non initialisé.");
        while (true); // bloque si le module ne répond pas
    }
    //nrf.printDetails();
    Serial.println("=== EN ATTENTE DE PAQUETS ===");
    //-------------------------------------------------------------*/

    /*
    // 1. Initialisation matérielle (configure les pins et les vitesses)
    setupMotors(); //ampèrage alim = 2,38 A pour 4 steppers

    Serial.println("Commandes disponibles :");
    Serial.println("  1+500  → m1 avance de 500 pas");
    Serial.println("  1-500  → m1 recule de 500 pas");
    Serial.println("  2+500  → m2 avance de 500 pas");
    Serial.println("  2-500  → m2 recule de 500 pas");*/

    
    // setupMotors();
    
    // m1.moveTo(10000);
    // m2.moveTo(10000);
    // m3.moveTo(10000);
    // m4.moveTo(10000);

    
    // setupUltrasons();
    // Serial.println("--- DEMARRAGE TEST ULTRASON ---");

    
    // setupServos();
    // Serial.println("-----DEMARRAGE TEST SERVOS------");
    // Serial.println("Commande : numéro servo + angle (ex: 1+90)"); 
    //servo 1 : 95°   2 : 93°   3 : 99°   4 : 95°

    Serial.println();
    Serial.println("=== TEST VL53L4CD ===");

    Wire.begin();

    if (!tof.begin())
    {
        Serial.println("Echec initialisation TOF.");

        while (1)
        {
            delay(100);
        }
    }

    Serial.println("TOF pret.");

}

void loop() {

    
    /*
    //--────────────────────────────────────────────
    nrf.update();
    while (nrf.hasCommand()) {
        String cmd = nrf.readCommand();
        Serial.println("CMD: " + cmd);
    }
   //--────────────────────────────────────────────*/

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


    /*V1
    //-────────────────────────────────────────────
    // 1. Moteur tourne en continu 
    m1.run();
    m2.run();
    m3.run();
    m4.run();

    Serial.print("isRunning: ");
    Serial.print(m1.isRunning());
    Serial.print(" | distanceToGo: ");
    Serial.print(m1.distanceToGo());
    Serial.print(" | obstacleDetecte: ");
    Serial.println(obstacleDetecte);

    // Bloquer les servos à 90° quand les moteurs tournent
    if (m1.isRunning() || m2.isRunning() || m3.isRunning() || m4.isRunning()) {
        ecrireAngleServo(1, 95);
        ecrireAngleServo(2, 93);
        ecrireAngleServo(3, 99);
        ecrireAngleServo(4, 95);
    }

    // Lecture TOF
    if (tof.mesurer()) {
        uint16_t distTOF = tof.getDistance();
        Serial.print("Distance TOF : ");
        Serial.print(distTOF);
        Serial.println(" mm");
        tof.reagir();

        // Obstacle détecté à moins de 500mm (50cm)
        if (distTOF > 0 && distTOF < 500) {
            m1.stop();
            m2.stop();
            m3.stop();
            m4.stop();
            Serial.println("Obstacle TOF !");
        }else {
            obstacleDetecte = false; // voie libre côté TOF
        }
    }

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
            m2.stop();
            m3.stop();
            m4.stop();
            Serial.println("Obstacle !");
        } else {
            // Redonner la cible si le moteur est arrêté
            if ((m4.distanceToGo() == 0)||(m3.distanceToGo() == 0)||(m1.distanceToGo() == 0)) {
                m1.moveTo(m1.currentPosition() + 10000);
                m2.moveTo(m2.currentPosition() + 10000);    
                m3.moveTo(m3.currentPosition() + 10000);
                m4.moveTo(m4.currentPosition() + 10000);
            }
        }
    }
   //-────────────────────────────────────────────*/

    /*
    // 1. Moteurs tournent en continu
    m1.run();
    m2.run();
    m3.run();
    m4.run();

    // Bloquer les servos à 90° quand les moteurs tournent
    if (m1.isRunning() || m2.isRunning() || m3.isRunning() || m4.isRunning()) {
        ecrireAngleServo(1, 95);
        ecrireAngleServo(2, 93);
        ecrireAngleServo(3, 99);
        ecrireAngleServo(4, 95);
    }

    // 2. Lecture TOF
    bool obstacleTOF = false;
    if (tof.mesurer()) {
        uint16_t distTOF = tof.getDistance();
        if (distTOF > 0 && distTOF < 200) { // 200mm = 20cm
            obstacleTOF = true;
            Serial.println("Obstacle TOF !");
        }
    }

    // 3. Lecture alternée des capteurs ultrason (un seul par cycle)
    if (millis() - dernierChronoCapteurs >= INTERVALLE_LECTURE) {
        dernierChronoCapteurs = millis();

        if (capteurActif == 0) {
            distG = getDistance(PIN_TRIG_g, PIN_ECHO_g);
            capteurActif = 1;
        } else {
            distD = getDistance(PIN_TRIG_d, PIN_ECHO_d);
            capteurActif = 0;
        }
    }

    // 4. Décision : arrêt ou relance
    bool obstacleUS = (distG > 0 && distG < 20) || (distD > 0 && distD < 20);

    if (obstacleTOF || obstacleUS) {
        m1.stop();
        m2.stop();
        m3.stop();
        m4.stop();
        Serial.println("Obstacle detecte !");
    } else {
        // Redonner une cible si les moteurs ont atteint leur destination
        if (m1.distanceToGo() == 0 || m2.distanceToGo() == 0 ||
            m3.distanceToGo() == 0 || m4.distanceToGo() == 0) {
            m1.moveTo(m1.currentPosition() + 10000);
            m2.moveTo(m2.currentPosition() + 10000);
            m3.moveTo(m3.currentPosition() + 10000);
            m4.moveTo(m4.currentPosition() + 10000);
        }
    }*/

    /*
    float dG = getDistance(PIN_TRIG_g, PIN_ECHO_g);
    float dD = getDistance(PIN_TRIG_d, PIN_ECHO_d);
    
    
    Serial.print("Gauche :");
    Serial.print(dG);
    Serial.println(" cm");
    
    
    Serial.print("Droite :");
    Serial.print(dD);
    Serial.println(" cm");
    delay(1000);*/


    /* 
    // -────────────────────────────────────────────
    //commandes servos

    if (Serial.available()) { // vérifie si des données sont arrivées sur le port série
        String cmd = Serial.readStringUntil('\n'); // lit jusqu'au retour à la ligne
        cmd.trim(); // supprime les espaces et \r

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
        else if (cmd.startsWith("avant+") || cmd.startsWith("arriere+")) {
            int angle = cmd.substring(cmd.indexOf('+') + 1).toInt();
            if (angle >= 0 && angle <= 180) {
                if (cmd.startsWith("avant+")) {
                    ecrireAngleServo(1, angle);
                    ecrireAngleServo(2, angle);
                    Serial.print("Servos avant -> ");
                } else {
                    ecrireAngleServo(3, angle);
                    ecrireAngleServo(4, angle);
                    Serial.print("Servos arriere -> ");
                }
                Serial.print(angle);
                Serial.println(" deg");
            } else {
                Serial.println("Angle invalide (0-180)");
            }
        }
        else {
            // Commande individuelle : "1+90"
            if (cmd.length() >= 3 && cmd.charAt(1) == '+') {
                int numServo = cmd.charAt(0) - '0';
                int angle = cmd.substring(2).toInt();
                if (numServo >= 1 && numServo <= 4 && angle >= 0 && angle <= 180) {
                    ecrireAngleServo(numServo, angle);
                    Serial.print("Servo ");
                    Serial.print(numServo);
                    Serial.print(" -> ");
                    Serial.print(angle);
                    Serial.println(" deg");
                } else {
                    Serial.println("Angle invalide (0-180) ou servo invalide (1-4)");
                }
            } else {
                Serial.println("Commande non reconnue");
            }
        }
    }
    //-────────────────────────────────────────────*/

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

    tof.update();

    delay(50);

}

    
    

