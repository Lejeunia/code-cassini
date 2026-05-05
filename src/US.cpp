#include <arduino.h>
#include "config.h"
#include "US.h"

void setupUltrasons(){
    //Configuration des entrées/sorties
    pinMode(PIN_TRIG_g, OUTPUT);
    pinMode(PIN_ECHO_g, INPUT);
    pinMode(PIN_TRIG_d, OUTPUT);
    pinMode(PIN_ECHO_d, INPUT);

    //on met les triggers à l'état bas
    digitalWrite(PIN_TRIG_g, LOW);
    digitalWrite(PIN_TRIG_d, LOW);

}

float getDistance(int trigPin, int echoPin){
    //1. Envoi du signal de 10 µs
    digitalWrite(trigPin, HIGH); //envoie sur la trigpin
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    //2. Mesure du temps de l'écho
    long duration = pulseIn(echoPin, HIGH, US_TIMEOUT); //echoPin = numéro de pin echo
                                                    // high = commence à compter qd signal monte à 5V
                                                    //TIMEOUT = sécurité, arrête de compter si h-d

    //3. Calcul de la distance
    float distance = duration * SOUND_SPEED/2;

    if (duration == 0) return -1; //retourne -1 si hors de portée
    return distance; 
}
