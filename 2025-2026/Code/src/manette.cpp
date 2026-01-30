//==============================================================================
// MANETTE.CPP - Controller Input Reading
//==============================================================================
// Handles reading all controller inputs (buttons, joysticks, triggers)
// Created by clovis on 3/19/24.
//==============================================================================

#include "main.h"
#include "manette.h"

// Initialize all controller inputs to default values (do not touch)
void initManette() {
    manette.a = false;
    manette.b = false;
    manette.x = false;
    manette.y = false;
    manette.up = false;
    manette.down = false;
    manette.left = false;
    manette.right = false;
    manette.select = false;
    manette.start = false;
    manette.l1 = false;
    manette.r1 = false;
    manette.l2 = false;
    manette.r2 = false;
    manette.l3 = false;
    manette.xbox = false;
    manette.leftJoystick.x = 0;
    manette.leftJoystick.y = 0;
    manette.rightJoystick.x = 0;
    manette.rightJoystick.y = 0;
    manette.leftTrigger = 0;
    manette.rightTrigger = 0;
}

// Mettre en commentaire les boutons non utilisés pour économiser des ressources.
void readManette() {
    manette.a = CrcLib::ReadDigitalChannel(BUTTON::COLORS_DOWN);
    //manette.b = CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT);
    //manette.x = CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT);
    //manette.y = CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP);
    manette.up = CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP);
    manette.down = CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN);
    manette.left = CrcLib::ReadDigitalChannel(BUTTON::ARROW_LEFT);
    manette.right = CrcLib::ReadDigitalChannel(BUTTON::ARROW_RIGHT);
    //manette.select = CrcLib::ReadDigitalChannel(BUTTON::SELECT);
    //manette.start = CrcLib::ReadDigitalChannel(BUTTON::START);
    manette.l1 = CrcLib::ReadDigitalChannel(BUTTON::L1);     // descend elevateur 
    manette.r1 = CrcLib::ReadDigitalChannel(BUTTON::R1);     // monte elevateur
    //manette.l2 = CrcLib::ReadDigitalChannel(BUTTON::HATL); // limit SWITCH du bas 
    //manette.r2 = CrcLib::ReadDigitalChannel(BUTTON::HATR); // limit SWITCH du haut 
    manette.l3 = CrcLib::ReadDigitalChannel(BUTTON::HATL);    // HATL = left joystick click
    //manette.xbox = CrcLib::ReadDigitalChannel(BUTTON::LOGO);
    manette.leftJoystick.x = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X);
    manette.leftJoystick.y = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y);
    manette.rightJoystick.x = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X);
    manette.rightJoystick.y = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y);
    manette.leftTrigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L);      // Recule le robot  
    manette.rightTrigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R);     // Avance le robot
}