//==============================================================================
// MAIN.CPP - TitanBot 2024 Main Control Loop
//==============================================================================
// ░▀█▀░░░▀█▀░░░▀█▀░░░█▀█░░░█▀█░░░█▀▄░░░█▀█░░░▀█▀
// ░░█░░░░░█░░░░░█░░░░█▀█░░░█░█░░░█▀▄░░░█░█░░░░█░
// ░░▀░░░░▀▀▀░░░░▀░░░░▀░▀░░░▀░▀░░░▀▀░░░░▀▀▀░░░░▀░
//
// Main control loop for robot operations
//==============================================================================

#include "main.h"
#include "manette.h"
#include "base_ctrl.h"
#include "robot_actions_ctrl.h"

#define ELEVATEUR_MOTOR CRC_PWM_1
#define DRIVE_MOTOR_R CRC_PWM_2
#define LANCEUR_MOTOR CRC_PWM_3
#define DRIVE_MOTOR_L CRC_PWM_4
#define GRABBER_MOTOR CRC_PWM_5
#define ARM_MOTOR CRC_PWM_6

#ifdef DEBUG_LOG
void debugLog();
#endif

void actionManetteConnectee();
void actionManettePasConnectee();
void updateMoteurs();

coords driveSpeed;
manette_s manette;
int8_t lanceurSpeed = 0;
int8_t grabberSpeed = 0;
int8_t armSpeed = 0;

void setup() {
    Serial.begin(9600);
    initManette();
    CrcLib::Initialize();
    CrcLib::InitializePwmOutput(ELEVATEUR_MOTOR);
    CrcLib::InitializePwmOutput(DRIVE_MOTOR_L);
    CrcLib::InitializePwmOutput(DRIVE_MOTOR_R);
    CrcLib::InitializePwmOutput(LANCEUR_MOTOR);
    CrcLib::InitializePwmOutput(GRABBER_MOTOR);
    CrcLib::InitializePwmOutput(ARM_MOTOR);
    
    // Initialize limit switches
    CrcLib::SetDigitalPinMode(CRC_DIG_1, INPUT);
    CrcLib::SetDigitalPinMode(CRC_DIG_2, INPUT);
    CrcLib::SetDigitalPinMode(CRC_DIG_3, INPUT);
    CrcLib::SetDigitalPinMode(CRC_DIG_4, INPUT);
    CrcLib::SetDigitalPinMode(CRC_DIG_7, INPUT);  // *** ADDED - Grabber closed limit ***
}

void loop() {
    CrcLib::Update();
    readManette();

    if (CrcLib::IsCommValid()) {
        actionManetteConnectee();
    } else {
        actionManettePasConnectee(); // Très important!!! Ne pas enlever!
    }

    updateMoteurs();

#ifdef DEBUG_LOG
    debugLog();
#endif
}

// Cette fonction est appelée à chaque itération de la boucle principale tant que la manette est connectée
void actionManetteConnectee() {
    actionElevateur();
    actionGrabber();  // *** ADDED ***

    driveSpeed.x = 0;
    driveSpeed.y = 0;

    verifieCommandeDriveJoy();      // Le DPad a priorité sur le joystick gauche, donc on s'occupe du DPad après le joystick
    verifieCommandeDriveDPad();     // pour overwrite le joystick si c'est nécessaire.

    //verifieCommandeLanceur();
    //verifieCommandeGrabber();
    //verifieCommandeArm();
}

// Cette fonction est appelée à chaque itération de la boucle principale tant que la manette n'est pas connectée.
// Ne surtout pas changer ce comportement! Sans ça, lorsque la manette est déconnectée, les moteurs pourraient tous
// partir à puissance maximale, à la grande surprise de la prochaine personne qui allumera le robot.
void actionManettePasConnectee() {
    driveSpeed.x = 0;
    driveSpeed.y = 0;
    lanceurSpeed = 0;
    elevateurSpeed = 0;
    grabberSpeed = 0;
    armSpeed = 0;
}

void updateMoteurs() {
    CrcLib::MoveArcade(driveSpeed.x, driveSpeed.y, DRIVE_MOTOR_L, DRIVE_MOTOR_R);
    CrcLib::SetPwmOutput(ELEVATEUR_MOTOR, elevateurSpeed);
    CrcLib::SetPwmOutput(LANCEUR_MOTOR, lanceurSpeed);
    CrcLib::SetPwmOutput(GRABBER_MOTOR, grabberSpeed);
    CrcLib::SetPwmOutput(ARM_MOTOR, armSpeed);
}

#ifdef DEBUG_LOG
void debugLog() {
    char buffer[100];
    sprintf(buffer, "elevateur: %d, lanceur: %d, grabber: %d, arm: %d\n", 
            elevateurSpeed, lanceurSpeed, grabberSpeed, armSpeed);
    Serial.print(buffer);
}
#endif