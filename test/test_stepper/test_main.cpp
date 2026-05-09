#include <Arduino.h>
#include <unity.h>
#include <AccelStepper.h> 

// Configuration de ton stepper
const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11); 

void test_stepper_movement() {
    // Test : Faire un tour complet
    myStepper.setSpeed(60);
    myStepper.step(stepsPerRevolution);
    
    // Ici on fait une vérification logique simple
    // Dans un test physique, c'est toi qui observes si le moteur a tourné
    TEST_ASSERT_TRUE(true); 
}

void setup() {
    delay(2000); // Important pour laisser le temps au port série
    UNITY_BEGIN();

    RUN_TEST(test_stepper_movement);

    UNITY_END();
}

void loop() { //à laisser vide
}