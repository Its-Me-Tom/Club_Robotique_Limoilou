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
#define LS_ELEVATOR_BOTTOM CRC_DIG_1    // Bottom elevator    (pin 22)
#define LS_ELEVATOR_TOP CRC_DIG_2       // Top elevator       (pin 23)
// arm pos
#define LS_ARM_TOP CRC_DIG_2            // Top arm position    (pin 23)
#define LS_ARM_MIDDLE CRC_DIG_3         // Middle arm position (pin 24)
#define LS_ARM_BOTTOM CRC_DIG_1         // Bottom arm position (pin 22)
// grabbers
#define LS_GRABBER_OPEN CRC_DIG_4       // Grabbers fully open (pin 25)

//=================== ELEVATOR STATE MACHINE ==================//
void (*actionElevateur)() = initElevateur;
int8_t elevateurSpeed = 0;
bool aimantAligne = false;

void initElevateur() {
    bool bottomLimit = CrcLib::GetDigitalInput(LS_ELEVATOR_BOTTOM);  // true = pressed
    
    if (bottomLimit) {
        elevateurSpeed = 0;
        actionElevateur = controlElevateur;
    } else {
        elevateurSpeed = -50;
        actionElevateur = controlElevateur;
    }
}

void controlElevateur() {
    bool topLimit = CrcLib::GetDigitalInput(LS_ELEVATOR_TOP);        // true = pressed
    bool bottomLimit = CrcLib::GetDigitalInput(LS_ELEVATOR_BOTTOM);  // true = pressed
    
    // Both buttons pressed = hold position
    if (BTN_ELEVATOR_UP && BTN_ELEVATOR_DOWN) {
        elevateurSpeed = 5;  // counter gravity
    }
    // Up button only - can go up if NOT at top limit
    else if (BTN_ELEVATOR_UP && !topLimit) {
        elevateurSpeed = 75;  // go up
    }
    // Down button only - can go down if NOT at bottom limit
    else if (BTN_ELEVATOR_DOWN && !bottomLimit) {
        elevateurSpeed = -50;  // go down
    }
    // Nothing pressed or at limit
    else {
        elevateurSpeed = 0;  // motor off
    }
}

//===================== GRABBERS ====================//
// literrylly open a closes with one or 2 limit switch
// will be toggled with B button
extern int8_t grabberSpeed;

void controlGrabber() {
    // TODO: implement grabber control
}


//===================== ARM ROTATION ====================//
// there is 3 modes mapped on 3 buttons:
// Y for the arm to go to the top position
// X for the arm to go to the middle position
// A for the arm to go to the bottom position

// note: for now only 2 limit switches are used (top and bottom)
// and a magnet sencor thigy for the middle position
extern int8_t armSpeed;

void controlArm() {
    // TODO: implement arm control
}