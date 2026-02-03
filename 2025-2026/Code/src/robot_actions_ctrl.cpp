//==============================================================================
// ROBOT_ACTIONS_CTRL.CPP - Robot Actions Control
//==============================================================================

#include "main.h"
#include "manette.h"
#include "robot_actions_ctrl.h"


//===================== BUTTON MAPPINGS =======================//
// controller button mappings
#define BTN_ELEVATOR_UP manette.l1      // L1 bumper - move elevator up
#define BTN_ELEVATOR_DOWN manette.r1    // R1 bumper - move elevator down
#define BTN_LAUNCHER manette.a          // A button - launcher on/off
#define BTN_GRABBER_TOGGLE manette.b    // B button - toggle grabbers open/close
#define BTN_ARM_TOP manette.y           // Y button - arm to top position
#define BTN_ARM_MIDDLE manette.x        // X button - arm to middle position

// Limit switch pins (LS = Limit Switch)
// elevator
#define LS_BOTTOM CRC_DIG_1   // Bottom elevaor      (pin 1)
#define LS_TOP    CRC_DIG_2   // Top elevator        (pin 2)
// arm pos
#define LS_TOP    CRC_DIG_2   // Top arm position    (pin 3)
#define LS_MIDDLE CRC_DIG_3   // Middle arm position (pin 4)
#define LS_BOTTOM CRC_DIG_1   // Bottom arm position (pin 5)
// grabbers
#define LS_OPEN   CRC_DIG_4   // Grabbers fully open (pin 6)


//=================== ELEVATOR STATE MACHINE ==================//
void (*actionElevateur)() = initElevateur;  // current state
int8_t elevateurSpeed = 0;                  // motor speed -127 to 127
bool aimantAligne = false;                  // limit switch status

void initElevateur() {
    aimantAligne = !CrcLib::GetDigitalInput(LS_BOTTOM);
    
    if (aimantAligne) {
        elevateurSpeed = 0;
        actionElevateur = bougePasElevateur;
    } else {
        elevateurSpeed = -50;  // go down to find bottom limit
        actionElevateur = descendElevateur;
    }
}

void bougePasElevateur() {
    elevateurSpeed = 0;  // motor off
    
    if (BTN_ELEVATOR_UP) {
        actionElevateur = monteElevateur;
    } else if (BTN_ELEVATOR_DOWN) {
        actionElevateur = descendElevateur;
    }
}

void monteElevateur() {
    if (!BTN_ELEVATOR_UP) {  // button released
        elevateurSpeed = 0;
        actionElevateur = bougePasElevateur;
    } else {
        elevateurSpeed = 50;  // move up
    }
}

void descendElevateur() {
    aimantAligne = !CrcLib::GetDigitalInput(LS_BOTTOM);
    
    if (aimantAligne) {  // hit bottom limit
        elevateurSpeed = 0;
        actionElevateur = bougePasElevateur;
    } else if (!BTN_ELEVATOR_DOWN) {  // button released
        elevateurSpeed = 0;
        actionElevateur = bougePasElevateur;
    } else {
        elevateurSpeed = -50;  // move down
    }
}


//===================== GRABBERS ====================//
// literrylly open a closes with one or 2 limit switch
// will be toggled with B button


//===================== ARM ROTATION ====================//
// there is 3 modes mapped on 3 buttons:
// Y for the arm to go to the top position
// X for the arm to go to the middle position
// A for the arm to go to the bottom position

// note: for now only 2 limit switches are used (top and bottom)
// and a magnet sencor thigy for the middle position