#ifndef US_H
#define US_H

#include <Arduino.h>
#include "config.h"

//Initialiser les broches des deux capteurs
void setupUltrasons();

//Lire la distance d'un capteur spécifique (valeur en cm)
float getDistance(int trigPin, int echoPin);

#endif