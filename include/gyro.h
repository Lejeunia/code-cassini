#ifndef GYRO_H
#define GYRO_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

struct GyroData {
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;
    float temp;

}

//initialisation
bool setupGyro();

//lecture des données
GyroData getGyroData();



#endif