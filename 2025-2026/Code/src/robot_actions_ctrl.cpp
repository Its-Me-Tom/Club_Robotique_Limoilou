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
#define UP 0
#define HALFWAY 1
#define DOWN 2
// grabbers
#define LS_GRABBER_OPEN CRC_DIG_4       // Grabbers fully open (pin 25)
#define OPEN 1
#define CLOSED 0


//=========================== VARIABLES ========================//
// elevator
int8_t elevateurSpeed = 0;
bool topLimit = false;
bool bottomLimit = false;
// grabbers
int8_t grabberSpeed;
bool openLimit = false;
bool closedLimit = false;
int LastGrabberState = OPEN;
// arm rotation
int8_t armSpeed = 0;
bool upwards = false;
bool halfways = false;
bool downwards = false;
int LastarmtState = DOWN;


//===================== FUNCTION POINTERS =======================//
void (*actionElevateur)() = initElevateur;
void (*actionGrabber)() = controlGrabber;
void (*actionArm)() = controlArm;


//======================== ELEVATOR CTRL =======================//
void initElevateur() {
    bottomLimit = CrcLib::GetDigitalInput(LS_ELEVATOR_BOTTOM);
    
    if (!bottomLimit) {
        // move down until bottom limit is reached
        elevateurSpeed = -50;
    } else {
        elevateurSpeed = 0;
        actionElevateur = controlElevateur;
    }
}

void controlElevateur() {
    topLimit = CrcLib::GetDigitalInput(LS_ELEVATOR_TOP);
    bottomLimit = CrcLib::GetDigitalInput(LS_ELEVATOR_BOTTOM);

    if (BTN_ELEVATOR_UP && BTN_ELEVATOR_DOWN) {
        elevateurSpeed = 5; // slight up to hold position againt gravity
    }
    else if (BTN_ELEVATOR_UP && !topLimit) {
        elevateurSpeed = 75; // move up
    }
    else if (BTN_ELEVATOR_DOWN && !bottomLimit) {
        elevateurSpeed = -50; // move down
    }
    else {
        elevateurSpeed = 0; // notihn
    }
}


//===================== GRABBERS ====================//
// literrylly open/closes with button B and one or 2 limit switch 
void initGrabber() {
    openLimit = CrcLib::GetDigitalInput(LS_GRABBER_OPEN);
    //closedLimit = CrcLib::GetDigitalInput(LS_GRABBER_CLOSED);
    
    if (!openLimit) {
        // open grabbers until open limit is reached
        grabberSpeed = 50;
    } else {
        grabberSpeed = 0;
        actionGrabber = controlGrabber;
    }
}

void controlGrabber() {
    openLimit = CrcLib::GetDigitalInput(LS_GRABBER_OPEN);
    //closedLimit = CrcLib::GetDigitalInput(LS_GRABBER_CLOSED);
    grabberSpeed = 50; 

    if (BTN_GRABBER_TOGGLE) {
        if (LastState == OPEN) {
            // close grabbers
            grabberSpeed = -50;
            // fix for knowing when to stop the grabbers
            // we don't have a closed limit switch
            
        }
        else {
            // open grabbers
            if (!openLimit) {
                grabberSpeed = 50;
            } else {
                grabberSpeed = 0;
                LastGrabberState = OPEN;
            }
        }
    }
}


//===================== ARM ROTATION ====================//
// there is 3 modes mapped on 3 buttons:
// Y for the arm to go to the top position
// X for the arm to go to the middle position
// A for the arm to go to the bottom position

void initArm() {
    downwards = CrcLib::GetDigitalInput(LS_ARM_BOTTOM);
    if (!downwards) {
        // move down until bottom limit is reached
        armSpeed = -50;
    } else {
        armSpeed = 0;
        actionArm = controlArm;
    }
}

void controlArm() {
    upwards = CrcLib::GetDigitalInput(LS_ARM_TOP);
    halfways = CrcLib::GetDigitalInput(LS_ARM_MIDDLE);
    downwards = CrcLib::GetDigitalInput(LS_ARM_BOTTOM);
    
    if (BTN_ARM_TOP) {
        // move arm down until bottom limit is reached
        armSpeed = -50;
    } else if (BTN_ARM_MIDDLE) {
        // move arm up until middle limit is reached
        armSpeed = 50;
    } else if (BTN_ARM_BOTTOM) {
        if (!downwards) {
            armSpeed = -50;
        } else {
            armSpeed = 0;
        }
    } else {
    }
}