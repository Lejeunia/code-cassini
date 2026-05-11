#include <Arduino.h>
#include <unity.h>
#include "config.h"
#include "US.h" // <--- Important : pour que le test "voit" tes fonctions

void test_capteur_gauche() {
    float d = getDistance(PIN_TRIG_g, PIN_ECHO_g);
    // On vérifie que le capteur ne renvoie pas -1 (erreur/timeout)
    TEST_ASSERT_NOT_EQUAL(-1, d);
    // On vérifie que la distance est cohérente (ex: entre 2cm et 400cm)
    TEST_ASSERT_GREATER_THAN(0, d);
}

void setup() {
    delay(2000);
    UNITY_BEGIN();

    setupUltrasons(); // <--- Appel de TA fonction d'initialisation

    RUN_TEST(test_capteur_gauche);

    UNITY_END();
}

void loop() {}