//==============================================================================
// MANETTE.CPP - Controller Input Reading
//==============================================================================

#include "main.h"
#include "manette.h"

//======================== INITIALIZE CONTROLLER ===============================
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

//======================== READ CONTROLLER INPUTS ==============================
// Mettre en commentaire les boutons non utilisés pour économiser des ressources.
void readManette() {
    // Face buttons
    manette.a = CrcLib::ReadDigitalChannel(BUTTON::COLORS_DOWN);   // launcher
    manette.b = CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT);  // grabber toggle
    manette.x = CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT);   // arm middle
    manette.y = CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP);     // arm top
    
    // DPad
    manette.up = CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP);
    manette.down = CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN);
    manette.left = CrcLib::ReadDigitalChannel(BUTTON::ARROW_LEFT);
    manette.right = CrcLib::ReadDigitalChannel(BUTTON::ARROW_RIGHT);
    
    // Bumpers
    manette.l1 = CrcLib::ReadDigitalChannel(BUTTON::L1);  // elevator up
    manette.r1 = CrcLib::ReadDigitalChannel(BUTTON::R1);  // elevator down
    
    // Joystick clicks
    manette.l3 = CrcLib::ReadDigitalChannel(BUTTON::HATL);  // crawl mode
    //manette.r3 = CrcLib::ReadDigitalChannel(BUTTON::HATR);
    
    // Menu buttons (mettre en commentaire si non utilisés)
    //manette.select = CrcLib::ReadDigitalChannel(BUTTON::SELECT);
    //manette.start = CrcLib::ReadDigitalChannel(BUTTON::START);
    //manette.xbox = CrcLib::ReadDigitalChannel(BUTTON::LOGO);
    
    // Joysticks (pour le contrôle de la base)
    manette.leftJoystick.x = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X);
    manette.leftJoystick.y = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y);
    manette.rightJoystick.x = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X);
    manette.rightJoystick.y = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y);
    
    // Triggers (pour avancer/reculer en mode Rocket League)
    manette.leftTrigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L);   // recule
    manette.rightTrigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R);  // avance
}