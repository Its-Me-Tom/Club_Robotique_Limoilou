//==============================================================================
// ROBOT_ACTIONS_CTRL.CPP - Robot Actions Control
//==============================================================================
#include "main.h"
#include "manette.h"
#include "robot_actions_ctrl.h"
#include "grabber_encoder.h"


//===================== BUTTON MAPPINGS =======================//
// controller button mappings
#define BTN_ELEVATOR_UP manette.r1      // L1 bumper - move elevator up
#define BTN_ELEVATOR_DOWN manette.l1    // R1 bumper - move elevator down
#define BTN_GRABBER_TOGGLE manette.b    // B button - toggle grabbers open/close
#define BTN_ARM_TOP manette.y           // Y button - arm to top position
#define BTN_ARM_MIDDLE manette.x        // X button - arm to middle position
#define BTN_ARM_BOTTOM manette.a        // A button - arm to bottom position

// Limit switch pins (LS = Limit Switch)
// elevator
#define LS_ELEVATOR_BOTTOM CRC_DIG_1    // Bottom elevator     (pin 1)
#define LS_ELEVATOR_TOP CRC_DIG_2       // Top elevator        (pin 2)
// arm pos
#define LS_ARM_TOP CRC_DIG_10            // Top arm position    (pin 3)
#define LS_ARM_MIDDLE CRC_DIG_11        // Middle arm position (pin 4)
#define LS_ARM_BOTTOM CRC_DIG_12         // Bottom arm position (pin 5)
#define TOP 0
#define HALFWAY 1
#define BOTTOM 2
// grabbers
//#define GRABBER_FULLY_OPEN_POSITION (-250)  // Encoder value when fully open
#define LS_GRABBER_OPEN CRC_DIG_6       // Grabbers open       (pin 6)
#define LS_GRABBER_CLOSED CRC_DIG_7     // Grabbers closed     (pin 7)
#define OPEN 1
#define CLOSED 0
#define CLOSING_TIME 1500  // ammound of time (in ms) to close befor it start forcing less

//=========================== VARIABLES ========================//
// elevator
int8_t elevateurSpeed = 0;
bool topLimit = false;
bool bottomLimit = false;
// grabbers - armSpeed and grabberSpeed are defined in main.cpp
//int32_t encoderPos = 0; encoder not used
bool Grabbering = false;
bool openLimit = false;
bool closedLimit = false;
int LastGrabberState = OPEN;
bool currentBtn = false;
bool prevGrabberBtn = false;
uint32_t closing_tmr = 0;  // now stores a millis() timestamp instead of a counter

// arm
bool upwards = false;
bool halfways = false;
bool downwards = false;
int TargetArmState = BOTTOM;
int LastArmState = BOTTOM;

//===================== FUNCTION POINTERS =======================//
void (*actionElevateur)() = initElevateur;
void (*actionGrabber)() = initGrabber;
void (*actionArm)() = controlArm;


//======================== ELEVATOR CTRL =======================//
void initElevateur() {
    topLimit = CrcLib::GetDigitalInput(LS_ELEVATOR_TOP);
    
    if (!topLimit) {
        // move down until top limit is reached
        elevateurSpeed = -50;
    } else {
        elevateurSpeed = 0;
        actionElevateur = controlElevateur;
    }
}

void controlElevateur() {
    topLimit = CrcLib::GetDigitalInput(LS_ELEVATOR_TOP);
    bottomLimit = CrcLib::GetDigitalInput(LS_ELEVATOR_BOTTOM);

//    if (BTN_ELEVATOR_UP && BTN_ELEVATOR_DOWN) {
//        elevateurSpeed = -10; // slight up to hold position againt gravity
//        // note we might remove it and also change the move up speed to 50 since the elevater 
//        // MIGHT have a counter weight in the future
//    }
    /*else */if (BTN_ELEVATOR_UP && !topLimit) {
        elevateurSpeed = -75; // move up
    }
    else if (BTN_ELEVATOR_DOWN && !bottomLimit) {
        elevateurSpeed = 50; // move down
    }
    else {
        elevateurSpeed = -10; // notihn
    }
}


//===================== GRABBERS ====================//
// open/closes with button B, uses 2 limit swtich
// idea: when toggle mode is switched to close, close it but the keep the motor engave at alow speed 
// unless closed limit switche is pressed (cause it need to make sure its holding the barrels)
// this is to prevent the motors from bruning and forcing too much
void initGrabber() {
    closedLimit = CrcLib::GetDigitalInput(LS_GRABBER_CLOSED);
    
    if (!closedLimit) {
        // close grabbers until closed limit is reached
        grabberSpeed = -50;
    } else {
        // at closed position - reset encoder
        LastGrabberState = CLOSED;
        grabberSpeed = 0;
        actionGrabber = controlGrabber;
    }
}

void controlGrabber() {
    openLimit   = CrcLib::GetDigitalInput(LS_GRABBER_OPEN);
    closedLimit = CrcLib::GetDigitalInput(LS_GRABBER_CLOSED);
    //encoderPos = readEncoder();  // Get current position (encoder not used)

    // Button debouncing
    currentBtn = BTN_GRABBER_TOGGLE;
    if (currentBtn && !prevGrabberBtn) {
        // Toggle state
        LastGrabberState = (LastGrabberState == OPEN) ? CLOSED : OPEN;
        Grabbering = true;
        closing_tmr = millis();
    }
    prevGrabberBtn = currentBtn;

    if (Grabbering) {

        // Closing
        if (LastGrabberState == CLOSED) {
            // and the holding function
            if (!closedLimit) {
                if (millis() - closing_tmr <= CLOSING_TIME) {
                    grabberSpeed = -75;
                } else {
                    grabberSpeed = -10;
                }
            } else {
                Grabbering = false;
                grabberSpeed = 0;
                closing_tmr = 0;
            }
        } 
        // Opening
        else {
            if (!openLimit) {
                grabberSpeed = 75;
            } 
            else {
                Grabbering = false;
                grabberSpeed = 0;
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
    
    // check for button presses to set target arm state
    if (BTN_ARM_TOP) {
        TargetArmState = TOP;
    }
    else if (BTN_ARM_MIDDLE) {
        TargetArmState = HALFWAY;
    }
    else if (BTN_ARM_BOTTOM) {
        TargetArmState = BOTTOM;
    }
    
    // determine current arm port and move accordingly
    armSpeed = 0;
    switch (TargetArmState) {
        // move to top position
        case TOP:
            if (!upwards) {
                armSpeed = 50;}
            else {
                LastArmState = TOP;}
        break;
        // move to middle position
        case HALFWAY:
            if (!halfways) {
                armSpeed = (LastArmState == TOP) ? -50 : 50;}
            else {
                LastArmState = HALFWAY;}
        break;
        // move to bottom position
        case BOTTOM:
            if (!downwards) {
                armSpeed = -50;}
            else {
                LastArmState = BOTTOM;}
        break;
    }
}