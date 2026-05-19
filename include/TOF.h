#ifndef TOF_H
#define TOF_H

#include <Arduino.h>
#include <Wire.h>
#include <vl53l4cd_class.h>

class TOF
{
public:
    TOF(TwoWire *wire);

    bool begin();
    void update();

private:
    TwoWire *m_wire;
    VL53L4CD *m_sensor;

    VL53L4CD_Result_t m_results;
};

#endif