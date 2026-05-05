#include "servos.h"

Servo messervos[4];

void setupServos(){
    messervos[0].attach(PIN_SERVO_1, 500, 2400);
    messervos[1].attach(PIN_SERVO_2, 500, 2400);
    messervos[2].attach(PIN_SERVO_3, 500, 2400);
    messervos[3].attach(PIN_SERVO_4, 500, 2400);
}

void ecrireAngleServo(int numServo, int angle){
    if (numServo >=1 && numServo <=4){
        messervos[numServo - 1].write(angle);
    }
}