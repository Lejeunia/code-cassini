#include <Arduino.h>
#include "config.h"
#include "stepper.h"
#include <AccelStepper.h>

//états stepper
AccelStepper m1(AccelStepper::DRIVER, M1_STEP_PIN, M1_DIR_PIN);
AccelStepper m2(AccelStepper::DRIVER, M2_STEP_PIN, M2_DIR_PIN);
AccelStepper m3(AccelStepper::DRIVER, M3_STEP_PIN, M3_DIR_PIN);
AccelStepper m4(AccelStepper::DRIVER, M4_STEP_PIN, M4_DIR_PIN);

void setupMotors(){
    //Configuration des pins enable en sortie
    //on gère l'alimentation électrique via le driver (interrupteur)
    pinMode(M1_ENABLE_PIN, OUTPUT);
    pinMode(M2_ENABLE_PIN, OUTPUT);
    pinMode(M3_ENABLE_PIN, OUTPUT);
    pinMode(M4_ENABLE_PIN, OUTPUT);

    //Active les moteurs par défaut
    digitalWrite(M1_ENABLE_PIN, LOW);
    digitalWrite(M2_ENABLE_PIN, LOW);
    digitalWrite(M3_ENABLE_PIN, LOW);
    digitalWrite(M4_ENABLE_PIN, LOW);

    //Réglage de la vitesse
    m1.setMaxSpeed(200);
    m1.setAcceleration(500);
    
    m2.setMaxSpeed(200);
    m2.setAcceleration(500);

    m3.setMaxSpeed(200);
    m3.setAcceleration(500);

    m4.setMaxSpeed(200);
    m4.setAcceleration(500);

}

void setMotorsEnabled(bool enabled){
    uint8_t state = enabled ? LOW : HIGH;
    digitalWrite(M1_ENABLE_PIN, state);
    digitalWrite(M2_ENABLE_PIN, state);
    digitalWrite(M3_ENABLE_PIN, state);
    digitalWrite(M4_ENABLE_PIN, state);

}

void avancerMoteurs(long distance){
    m1.moveTo(m1.currentPosition() + distance);
    m2.moveTo(m2.currentPosition() + distance);
    m3.moveTo(m3.currentPosition() + distance);
    m4.moveTo(m4.currentPosition() + distance);

    while (
        m1.distanceToGo() != 0 || 
        m2.distanceToGo() != 0 || 
        m3.distanceToGo() != 0 || 
        m4.distanceToGo() != 0
    ) {
        m1.run();
        m2.run();
        m3.run();
        m4.run();
    }
    
}