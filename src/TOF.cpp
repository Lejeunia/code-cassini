#include "TOF.h"

TOFSensor::TOFSensor()
    : _sensor(Wire, TOF_XSHUT_PIN, -1),
      _distance_mm(0),
      _status(255)
{}

bool TOFSensor::begin() {
    Wire.begin();

    if (_sensor.begin() != 0) {
        Serial.println("[TOF] Erreur : capteur non détecté !");
        return false;
    }

    _sensor.setTimingBudgetInMs(TOF_TIMING_BUDGET);
    _sensor.setIntermeasurementPeriod(TOF_INTER_MEAS);
    _sensor.startRanging();

    Serial.println("[TOF] Capteur initialisé OK");
    return true;
}

bool TOFSensor::mesurer() {
    if (!_sensor.checkForDataReady()) return false;

    _distance_mm = _sensor.getDistance();
    _status      = _sensor.getRangeStatus();
    _sensor.clearInterrupt();

    return (_status == 0);
}

void TOFSensor::reagir() {
    if (_distance_mm < DIST_SEUIL_PROCHE) {
        Serial.println("  → PROCHE !");
        // ex: digitalWrite(LED_ROUGE, HIGH);

    } else if (_distance_mm < DIST_SEUIL_LOIN) {
        Serial.println("  → Zone intermédiaire");

    } else {
        Serial.println("  → Loin ou rien");
    }
}

uint16_t TOFSensor::getDistance() { return _distance_mm; }
uint8_t  TOFSensor::getStatus()   { return _status; }