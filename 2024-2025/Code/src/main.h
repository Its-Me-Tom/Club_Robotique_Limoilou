//
// Created by clovis on 25-01-27.
//

#ifndef MAIN_H
#define MAIN_H

#include <CrcBuzz.h>
#include <CrcLib.h>
#include <CrcNeo.h>
#include <CrcRemoteState.h>
#include <CrcXbee.h>

#define ELEVATEUR_CORRECTION_GRAVITE (-7)
#define FLIPPEUX_CORRECTION_GRAVITE 16

#define ELEVATEUR_LIMIT_SWITCH_HAUT CRC_DIG_2
#define ELEVATEUR_LIMIT_SWITCH_BAS CRC_DIG_12

struct coords {
    int8_t x;
    int8_t y;
};

extern bool pinceLimitSwitch;
extern bool elevateurLimitSwitchBas;
extern bool elevateurLimitSwitchHaut;
extern coords driveSpeed;
extern int8_t lanceurSpeed;
extern int8_t etatPince;
extern int8_t flippeuxSpeed;
extern int8_t elevateurDirectionDemandee;

//////
/*
const Note TUNE_BIZZ[] = {
    { 20, NOTE_G4 }, { 20, NOTE_A4 }, { 20, NOTE_B4 }, { 20, NOTE_C5 },
    { 20, NOTE_D5 }, { 20, NOTE_E5 }, { 20, NOTE_F5 }, { 20, NOTE_G5 },
    { 20, NOTE_F5 }, { 20, NOTE_E5 }, { 20, NOTE_D5 }, { 20, NOTE_C5 },
    { 20, NOTE_B4 }, { 20, NOTE_A4 }, { 20, NOTE_G4 }, { 20, NOTE_SILENCE },
    Note::END
};

const Note TUNE_MARIO_MUSH[] = {
    {30, 523}, {30, 392}, {30, 523}, {30, 659}, {30, 784},
    {30, 1047}, {30, 784}, {30, 415}, {}
};
*/
//GRUB_INIT_TUNE="1750 523 1 392 1 523 1 659 1 784 1 1047 1 784 1 415 1 523 1 622 1 831 1 622 1 831 1 1046 1 1244 1
//1661 1 1244 1 466 1 587 1 698 1 932 1 1195 1 1397 1 1865 1 1397 1"

//////

#endif //MAIN_H
