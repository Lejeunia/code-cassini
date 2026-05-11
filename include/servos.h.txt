#ifndef SERVO_H
#define SERVO_H

#include <Arduino.h>
#include <servos.h>
#include "config.h"

void setupServos();

void ecrireAngleServo(int numServo, int angle);
//cette fonction a comme parapmètre le numéro du servo, et l'angle auquel il doit se positioner


#endif 