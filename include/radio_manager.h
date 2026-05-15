#ifndef RADIO_MANAGER_H
#define RADIO_MANAGER_H

#include <Arduino.h>

//initialsie le module de communication
void setupRadio();

//vérifie si des données sont reçues et les traites
void checkRadio();

void interpreterCommandes(const char* message);

void interpreterMultipleCommandes(const char* message);


#endif