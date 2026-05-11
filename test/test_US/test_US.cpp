#include "Arduino.h"
#include <unity.h>
#include "config.h"

// Fonction utilitaire pour mesurer la distance
float readDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout de 30ms
    if (duration == 0) return -1;
    return duration * 0.034 / 2;
}

// TEST 1 : Vérifier que le capteur renvoie bien une valeur (pas de timeout)
void test_sensor_is_responding() {
    float distance = readDistance();
    TEST_ASSERT_GREATER_THAN(-1, distance);
}

// TEST 2 : Vérifier que la distance est dans une plage réaliste (ex: > 2cm)
void test_distance_is_realistic() {
    float distance = readDistance();
    // On part du principe que tu ne colles pas le capteur à l'objet
    TEST_ASSERT_GREATER_THAN(2.0, distance); 
    TEST_ASSERT_LESS_THAN(400.0, distance);
}

void setup() {
    delay(2000); // Temps pour ouvrir le monitor
    UNITY_BEGIN();
    
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    RUN_TEST(test_sensor_is_responding);
    RUN_TEST(test_distance_is_realistic);

    UNITY_END();
}

void loop() {
    // Pas besoin de loop pour un test unitaire
}