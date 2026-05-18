#include <Arduino.h>
#include "config.h"
#include "US.h"
#include "servos.h"
#include "stepper.h"
#include <AccelStepper.h>
#include "radio_manager.h"
#include "TOF.h"

extern AccelStepper m1;
extern AccelStepper m2;
extern AccelStepper m3;
extern AccelStepper m4;

unsigned long dernierArret = 0;
TOFSensor tof;




void setup() {
    
    Serial.begin(9600);

    /*
    setupRadio();*/

    /*
    // 1. Initialisation matérielle (configure les pins et les vitesses)
    setupMotors(); //ampèrage alim = 2,38 A pour 4 steppers

    Serial.println("Commandes disponibles :");
    Serial.println("  1+500  → m1 avance de 500 pas");
    Serial.println("  1-500  → m1 recule de 500 pas");
    Serial.println("  2+500  → m2 avance de 500 pas");
    Serial.println("  2-500  → m2 recule de 500 pas");*/


    
    setupUltrasons();
    Serial.println("--- DEMARRAGE TEST ULTRASON ---");

    
    setupServos();
    Serial.println("-----DEMARRAGE TEST SERVOS------");
    Serial.println("Commande : numéro servo + angle (ex: 1+90)");

    /*
    if (!tof.begin()) {
        Serial.println("Capteur non détecté.");
        while (true);
    }*/

}

void loop() {
    
    /*
    checkRadio();*/

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

    
    float dG = getDistance(PIN_TRIG_g, PIN_ECHO_g);
    /*float dD = getDistance(PIN_TRIG_d, PIN_ECHO_d);*/
    
    
    Serial.print("Gauche :");
    Serial.print(dG);
    Serial.println(" cm");
    
    /*
    Serial.print("Droite :");
    Serial.print(dD);
    Serial.println(" cm");*/

    delay(1000);

    
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
    }

    /*
    if (tof.mesurer()) {
        Serial.print("Distance : ");
        Serial.print(tof.getDistance());
        Serial.println(" mm");
        tof.reagir();
    }*/

    
}

    
    
    
