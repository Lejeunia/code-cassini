#include <Arduino.h>
#include "config.h"
#include "US.h"

void setup() {
    Serial.begin(9600);
    setupUltrasons();
    Serial.println("--- DEMARRAGE TEST ULTRASON ---");
    
}

void loop() {
    float dG = getDistance(PIN_TRIG_g, PIN_ECHO_g);
    
    
    Serial.print("Gauche :");
    Serial.print(dG);
    Serial.println(" cm");

    

    delay(1000);
}