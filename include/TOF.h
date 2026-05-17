#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_VL53L1X.h>
#include "config.h"

class TOFSensor {
public:
    TOFSensor();

    bool begin();
    bool mesurer();
    void reagir();
    uint16_t getDistance();
    uint8_t  getStatus();

private:
    SFEVL53L1X _sensor;
    uint16_t   _distance_mm;
    uint8_t    _status;
};