#include "TOF.h"

TOF::TOF(TwoWire *wire)
{
    m_wire = wire;
    m_sensor = nullptr;
}

bool TOF::begin()
{
    m_sensor = new VL53L4CD(m_wire, -1);

    Serial.println("Initialisation VL53L4CD...");

    if (m_sensor->begin() != VL53L4CD_ERROR_NONE)
    {
        Serial.println("Erreur init VL53L4CD !");
        return false;
    }

    Serial.println("VL53L4CD detecte.");

    // Timing budget
    m_sensor->VL53L4CD_SetRangeTiming(200, 0);

    if (m_sensor->VL53L4CD_StartRanging() != VL53L4CD_ERROR_NONE)
    {
        Serial.println("Erreur start ranging !");
        return false;
    }

    Serial.println("Ranging demarre.");

    return true;
}

void TOF::update()
{
    uint8_t newDataReady = 0;

    if (m_sensor->VL53L4CD_CheckForDataReady(&newDataReady) != VL53L4CD_ERROR_NONE)
    {
        Serial.println("Erreur data ready");
        return;
    }

    if (newDataReady)
    {
        if (m_sensor->VL53L4CD_GetResult(&m_results) == VL53L4CD_ERROR_NONE)
        {
            Serial.print("Distance: ");
            Serial.print(m_results.distance_mm);
            Serial.println(" mm");
        }

        m_sensor->VL53L4CD_ClearInterrupt();
    }
}

