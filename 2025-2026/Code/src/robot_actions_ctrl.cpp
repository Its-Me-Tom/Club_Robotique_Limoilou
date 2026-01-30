//==============================================================================
// ROBOT_ACTIONS_CTRL.CPP - Robot Actions Control
//==============================================================================
// Handles robot-specific actions (elevator, launcher, etc.)
// Includes elevator state machine and manual controls
//==============================================================================

#include "main.h"
#include "manette.h"
#include "robot_actions_ctrl.h"

#define AIMANT_PIN CRC_DIG_1

// Forward declarations for all elevator functions
void initElevateur();
void initMonteElevateur();
void monteElevateur();
void initDescendElevateur();
void descendElevateur();
void bougePasElevateur();

// External variables from main.cpp
extern int8_t elevateurDirectionCommandee;
extern int8_t lanceurSpeed;

// Elevator state machine function pointer
void (*actionElevateur)() = initElevateur;

// Elevator variables
int8_t elevateurSpeed = 0;
bool aimantAligne = false;

//==============================================================================
// ELEVATOR STATE MACHINE
//==============================================================================

void initElevateur() {
    aimantAligne = !CrcLib::GetDigitalInput(AIMANT_PIN);
    if (!aimantAligne) {
        elevateurSpeed = 50;
    } else {
        elevateurSpeed = 0;
        //elevateurDirectionCommandee = 0;
        actionElevateur = bougePasElevateur;
    }
}

void initMonteElevateur() {
    aimantAligne = !CrcLib::GetDigitalInput(AIMANT_PIN);
    elevateurSpeed = 50;
    if (!aimantAligne) {
        actionElevateur = monteElevateur;
    }
}

void monteElevateur() {
    aimantAligne = !CrcLib::GetDigitalInput(AIMANT_PIN);
    if (aimantAligne) {
        elevateurSpeed = 0;
        elevateurDirectionCommandee = 0;
        actionElevateur = bougePasElevateur;
    } else {
        elevateurSpeed = 50;
    }
}


void initDescendElevateur() {
    aimantAligne = !CrcLib::GetDigitalInput(AIMANT_PIN);
    elevateurSpeed = -50;
    if (!aimantAligne) {
        actionElevateur = descendElevateur;
    }
}

void descendElevateur() {
    aimantAligne = !CrcLib::GetDigitalInput(AIMANT_PIN);
    if (aimantAligne) {
        elevateurSpeed = 0;
        elevateurDirectionCommandee = 0;
        actionElevateur = bougePasElevateur;
    } else {
        elevateurSpeed = -50;
    }
}


// quand on pese le boutton, le moteur vas donner une petite 
// puissance pour contrer la gravité pour pas quelle redescend.
void bougePasElevateur() {
    // Debug print removed to avoid blocking the main loop and
    // introducing latency when updating motor outputs.
    // Serial.write("b\n");
    aimantAligne = !CrcLib::GetDigitalInput(AIMANT_PIN);
    elevateurSpeed = 0;
    if (elevateurDirectionCommandee == 1) {
        actionElevateur = initMonteElevateur;
    } else if (elevateurDirectionCommandee == -1) {
        actionElevateur = initDescendElevateur;
    }
}

//==============================================================================
// ELEVATOR COMMAND FUNCTIONS
//==============================================================================

// La variable elevateurDirectionCommandee va gentiment demander à la machine à états de monter,
// descendre ou ne pas bouger. C'est la machine à états qui décidera quoi faire de cette demande.
void verifieCommandeElevateurAuto() {
    if (manette.l1 && manette.l2) {         // l2 C La limit SWITCH du bas 
        elevateurDirectionCommandee = 0;    // (si l1 et limit switch en bas est persé fais rien)
    } else if (manette.l1) {
        elevateurDirectionCommandee = 1;
    } else if (manette.r1) {
        elevateurDirectionCommandee = -1;
    }
}

// Contrôle manuel de l'élévateur. Peut être utile pour faire des tests ou en cas de problème.
void verifieCommandeElevateurManuel() {
    // Les gâchettes sont = -128 lorsqu'elles ne sont pas appuyés et 127 lorsqu'elles sont 100% enfonçées
    if (manette.leftTrigger > -118 && manette.rightTrigger < -118) {
        elevateurSpeed = (int8_t)map(manette.leftTrigger, -128, 127, 0, -30);
    } else if (manette.rightTrigger > -118 && manette.leftTrigger < -118) {
        elevateurSpeed = (int8_t)map(manette.rightTrigger, -128, 127, 0, 30);
    }
}

//==============================================================================
// LAUNCHER COMMAND FUNCTIONS
//==============================================================================

void verifieCommandeLanceur() {
    if (manette.x) {
        lanceurSpeed = 127;
    } else {
        lanceurSpeed = 0;
    }
}