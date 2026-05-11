#include "gyro.h"

Adafruit_MPU6050 mpu;

bool setupGyro() {
    //initialisation de la communication I2C
    if (!mpu.begin()) {
        return false; //retourne faux si le capteur n'est pas trouvé
    }

    //Configuration des plages de mesure
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    return true
}

GyroData getGyroData(){
    sensors_event_t a ,g, temp;
    mpu.getEVent(&a, &g, &temp);

    GyroData data;
    data.accX = a.acceleration.x;
    data.accY = a.acceleration.y;
    data.accZ = a.acceleration.z;

    data.gyroX = g.gyro.x; //[rad/s]
    data.gyroY = g.gyro.y;
    data.gyroZ = g.gyro.z;

    data.temp = temp.temperature;

    return data;
}