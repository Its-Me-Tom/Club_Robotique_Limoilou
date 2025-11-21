//
// Created by clovis on 25-02-03.
//

#ifndef PINCE_H
#define PINCE_H

#include <CrcLib.h>

#define PINCE_MOTEUR CRC_PWM_1
#define PINCE_LIMIT_SWITCH CRC_DIG_1
#define VITESSE_PINCE 50

void initPince();
void updatePince();

extern int8_t pinceDirectionDemandee;
extern int8_t pinceDirectionCommandee;

#endif //PINCE_H
