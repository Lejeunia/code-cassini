#ifndef SERVOS_H
#define SERVOS_H

#include <Arduino.h>
#include <Servo.h>
#include "config.h"

extern Servo messervos[4];

void setupServos();

void ecrireAngleServo(int numServo, int angle);
//cette fonction a comme parapmètre le numéro du servo, et l'angle auquel il doit se positioner


#endif 