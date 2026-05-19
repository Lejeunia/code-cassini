#include "TOF.h"
#include "config.h"
#include <Wire.h>
#include <vl53l4cd_class.h>

// Création de l'instance du capteur en passant le bus I2C (Wire) et la broche XSHUT
VL53L4CD sensor_vl53l4cd(&Wire, TOF_XSHUT_PIN);

bool initTOF() {
    // Initialisation du bus I2C (si ce n'est pas déjà fait ailleurs)
    Wire.begin();
    
    // Configuration de la broche XSHUT si elle est définie
    if (TOF_XSHUT_PIN >= 0) {
        pinMode(TOF_XSHUT_PIN, OUTPUT);
        digitalWrite(TOF_XSHUT_PIN, LOW);
        delay(10);
        digitalWrite(TOF_XSHUT_PIN, HIGH);
        delay(10);
    }

    // Initialisation du composant
    if (sensor_vl53l4cd.begin() != 0) {
        Serial.println(F("[TOF] Erreur: Impossible de communiquer avec le VL53L4CD"));
        return false;
    }

    // Arrêt d'une éventuelle mesure précédente pour appliquer la config
    sensor_vl53l4cd.VL53L4CD_StopRanging();

    // Configuration du Timing Budget (Plus il est élevé, plus c'est précis, mais plus c'est lent)
    if (sensor_vl53l4cd.VL53L4CD_SetRangeTiming(TOF_TIMING_BUDGET, 0) != 0) {
        Serial.println(F("[TOF] Erreur: Impossible de configurer le Timing Budget"));
        return false;
    }

    // Démarrage de la mesure continue
    if (sensor_vl53l4cd.VL53L4CD_StartRanging() != 0) {
        Serial.println(F("[TOF] Erreur: Impossible de démarrer les mesures"));
        return false;
    }

    Serial.println(F("[TOF] Initialisation réussie !"));
    return true;
}

int16_t readDistanceTOF() {
    uint8_t isDataReady = 0;
    VL53L4CD_Result_t results;
    int16_t distance = -1;

    // Vérifie si une nouvelle donnée est disponible
    sensor_vl53l4cd.VL53L4CD_CheckForDataReady(&isDataReady);

    if (isDataReady == 0) {
        // Le capteur a configuré la mesure continue de manière asynchrone.
        // Si aucune donnée n'est prête à cet instant T, on sort proprement (non-bloquant).
        return -2; 
    }

    // Récupération des résultats
    if (sensor_vl53l4cd.VL53L4CD_GetResult(&results) == 0) {
        // On vérifie que le statut de la mesure est valide (0 = Valeur OK)
        if (results.range_status == 0) {
            distance = results.distance_mm;
        } else {
            // Statut d'erreur (ex: cible trop loin, signal trop faible...)
            #ifdef DEBUG_TOF
            Serial.print(F("[TOF] Statut mesure invalide : "));
            Serial.println(results.range_status);
            #endif
        }
    }

    // Efface le flag pour autoriser le capteur à prendre la mesure suivante
    sensor_vl53l4cd.VL53L4CD_ClearInterrupt();

    return distance;
}