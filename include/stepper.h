#ifndef stepper_H
#define stepper_H

#include    <AccelStepper.h>

//annonce la fonction au projet
//fonction pour configurer toutes les pins des moteurs
void setupMotors();

void setMotorsEnabled(bool enabled);

void avancerMoteurs(long distance);

#endif
