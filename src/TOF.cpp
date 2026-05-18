#include "TOF.h"

TOFSensor::TOFSensor()
    : _sensor(&Wire, TOF_XSHUT_PIN),
      _distance_mm(0),
      _status(255)
{}

bool TOFSensor::begin() {
    Wire.begin();
    _sensor.begin();
    _sensor.VL53L4CD_Off();
    _sensor.InitSensor();
    _sensor.VL53L4CD_SetRangeTiming(TOF_TIMING_BUDGET, 0);
    _sensor.VL53L4CD_StartRanging();
    Serial.println("[TOF] Capteur initialisé OK");
    return true;
}

bool TOFSensor::mesurer() {
    uint8_t dataReady = 0;
    _sensor.VL53L4CD_CheckForDataReady(&dataReady);
    if (!dataReady) return false;

    VL53L4CD_Result_t results;
    _sensor.VL53L4CD_ClearInterrupt();
    _sensor.VL53L4CD_GetResult(&results);

    _distance_mm = results.distance_mm;
    _status      = results.range_status;

    return (_status == 0);
}

void TOFSensor::reagir() {
    if (_distance_mm < DIST_SEUIL_PROCHE) {
        Serial.println("  → PROCHE !");
    } else if (_distance_mm < DIST_SEUIL_LOIN) {
        Serial.println("  → Zone intermédiaire");
    } else {
        Serial.println("  → Loin ou rien");
    }
}

uint16_t TOFSensor::getDistance() { return _distance_mm; }
uint8_t  TOFSensor::getStatus()   { return _status; }