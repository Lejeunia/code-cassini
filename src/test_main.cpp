#include <Arduino.h>
#include "config.h"
#include "US.h"
#include "servos.h"
#include "stepper.h"
#include <AccelStepper.h>

extern AccelStepper m1;

void setup() {
    
    Serial.begin(9600);

    // 1. Initialisation matérielle (configure les pins et les vitesses)
    setupMotors();


    Serial.println("--- Test Moteur 3 Uniquement ---");
    Serial.println("Le moteur doit faire 1 tour (200 pas), pause, puis retour.");

    // On prépare le premier mouvement
    m1.moveTo(1000); 

    /*
    setupUltrasons();
    Serial.println("--- DEMARRAGE TEST ULTRASON ---");

    setupServos();
    Serial.println("-----DEMARRAGE TEST SERVOS------");
    
    ecrireAngleServo(1, 0);
    Serial.println("servo 1 : 0°");*/



}

void loop() {

    // Cette fonction doit être appelée en boucle pour générer les pas
    m1.run();

    // Quand le moteur a fini son mouvement
    if (m1.distanceToGo() == 0) {
        Serial.println("Cible atteinte. Pause...");
        delay(1000); // Pause d'une seconde
        
        // Détermination de la nouvelle cible (Aller-Retour)
        long nouvelleCible = (m1.currentPosition() == 0) ? 250 : 0;
        
        Serial.print("Nouvelle destination : ");
        Serial.println(nouvelleCible);
    }

    /*
    float dG = getDistance(PIN_TRIG_g, PIN_ECHO_g);
    
    
    Serial.print("Gauche :");
    Serial.print(dG);
    Serial.println(" cm");

    

    delay(1000);*/
    
    
}