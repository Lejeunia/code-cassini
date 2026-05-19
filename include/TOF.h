#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <vl53l4cd_class.h>
#include "config.h"

#ifndef TOF_H
#define TOF_H

#include <Arduino.h>

/**
 * @brief Initialise le capteur TOF VL53L4CD.
 * @return true si l'initialisation a réussi, false sinon.
 */
bool initTOF();

/**
 * @brief Lit la distance actuelle mesurée par le capteur.
 * @return La distance en millimètres (mm). Retourne -1 en cas d'erreur de lecture.
 */
int16_t readDistanceTOF();

#endif 