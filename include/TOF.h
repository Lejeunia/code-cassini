#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <vl53l4cd_class.h>
#include "config.h"

class TOFSensor {
public:
    TOFSensor();
    bool     begin();
    bool     mesurer();
    void     reagir();
    uint16_t getDistance();
    uint8_t  getStatus();

private:
    VL53L4CD _sensor;
    uint16_t _distance_mm;
    uint8_t  _status;
};