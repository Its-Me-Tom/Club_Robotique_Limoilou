//
// Created by clovis on 25-01-22.
//

//
// ░▀█▀░░░▀█▀░░░▀█▀░░░█▀█░░░█▀█░░░█▀▄░░░█▀█░░░▀█▀
// ░░█░░░░░█░░░░░█░░░░█▀█░░░█░█░░░█▀▄░░░█░█░░░░█░
// ░░▀░░░░▀▀▀░░░░▀░░░░▀░▀░░░▀░▀░░░▀▀░░░░▀▀▀░░░░▀░
//
// TitanBot 2025

// Ce code est certes fonctionnel, relativement fiable et robuste, mais aussi vite fait, mal fait,
// et assemblé comme une vieille tercel qui tient miraculeusement avec de la broche et des prières.
// Ne pas utiliser comme exemple. Éventuellement refactoriser pour servir de référence pour l'an
// prochain.

#include "main.h"

#include <encoder.h>

#include "manette.h"
#include "pince.h"

#define DRIVE_MOTOR_L CRC_PWM_4
#define DRIVE_MOTOR_R CRC_PWM_2
#define LANCEUR_MOTEUR CRC_PWM_6
#define FLIPPEUX_MOTEUR CRC_PWM_3
#define ELEVATEUR_MOTEUR CRC_PWM_5

#ifdef DEBUG_LOG

void debugLog();

#endif

void actionManetteConnectee();
void actionManettePasConnectee();
void updateMoteurs();
void updateElevateur();

coords driveSpeed;

manette_s manette;

int8_t elevateurDirectionDemandee = 0;
int8_t elevateurDirectionCommandee = 0;
int8_t lanceurSpeed = 0;
int8_t flippeuxSpeed = 0;
bool pinceLimitSwitch = false;
bool elevateurLimitSwitchHaut = false;
bool elevateurLimitSwitchBas = false;
int8_t pinceDirectionDemandee = 0;
int8_t pinceDirectionCommandee = 0;
int8_t etatElevateur = 0;

void setup() {
    Serial.begin(9600);

    CrcLib::Initialize();

    initManette();
    readManette();
    initPince();

    CrcLib::SetDigitalPinMode(ELEVATEUR_LIMIT_SWITCH_HAUT, INPUT);       // pas une erreur, notre electronique est
    CrcLib::SetDigitalPinMode(ELEVATEUR_LIMIT_SWITCH_BAS, INPUT_PULLUP); // juste un ptit peu bizz

    CrcLib::InitializePwmOutput(DRIVE_MOTOR_L);
    CrcLib::InitializePwmOutput(DRIVE_MOTOR_R);
    CrcLib::InitializePwmOutput(PINCE_MOTEUR);
    CrcLib::InitializePwmOutput(ELEVATEUR_MOTEUR);
    CrcLib::InitializePwmOutput(FLIPPEUX_MOTEUR);
    CrcLib::InitializePwmOutput(LANCEUR_MOTEUR);

    //Serial.println(CrcLib::GetDigitalInput(PINCE_LIMIT_SWITCH));
}

void loop() {
    CrcLib::Update();
    readManette();

    if (CrcLib::IsCommValid()) {
        actionManetteConnectee();
    }
    else {
        actionManettePasConnectee(); // Très important!!! Ne pas enlever!
    }

    updateMoteurs();
    updatePince();
    updateElevateur();

#ifdef DEBUG_LOG

    debugLog();

#endif
}

// Cette fonction est appelée à chaque itération de la boucle principale tant que la manette est connectée
void actionManetteConnectee() {
    driveSpeed.x = 0;
    driveSpeed.y = 0;

    verifieCommandeDriveJoy();  // Le DPad a priorité sur le joystick gauche, donc on s'occupe du DPad après le joystick
    verifieCommandeDriveDPad(); // pour overwrite le joystick si c'est nécessaire.
    verifieCommandePince();
    verifieCommandeElevateur();
    verifieCommandeFlippeux();
    verifieCommandeLanceur();
}

// Cette fonction est appelée à chaque itération de la boucle principale tant que la manette n'est pas connectée.
// Ne surtout pas changer ce comportement! Sans ça, lorsque la manette est déconnectée, les moteurs pourraient tous
// partir à puissance maximale, à la grande surprise de la prochaine personne qui allumera le robot.
void actionManettePasConnectee() {
    driveSpeed.x = 0;
    driveSpeed.y = 0;

    CrcLib::SetPwmOutput(PINCE_MOTEUR, pinceDirectionCommandee);
}

void updateElevateur() {
    elevateurLimitSwitchBas = !CrcLib::GetDigitalInput(ELEVATEUR_LIMIT_SWITCH_BAS);
    elevateurLimitSwitchHaut = CrcLib::GetDigitalInput(ELEVATEUR_LIMIT_SWITCH_HAUT);
    // elevateur direction negative qd elevateur monde
    // joy y negatif lorsque joy y levé vers le haut

    if (elevateurDirectionDemandee < 0 && elevateurLimitSwitchHaut) {
        // Si on veut monter l'élévateur et que la limit switch est enfoncée
        elevateurDirectionCommandee = -7;
        etatElevateur = 1;
    }
    else if (elevateurDirectionDemandee > 0 && elevateurLimitSwitchBas) {
        // Si on veut descendre l'élévateur et que la limit switch est enfoncée
        elevateurDirectionCommandee = 0;
        etatElevateur = 2;
    }
    else if (elevateurDirectionDemandee < 0 && !elevateurLimitSwitchHaut) {
        // Si on veut monter l'élévateur et que la limit switch n'est pas enfoncée
        elevateurDirectionCommandee = elevateurDirectionDemandee;
        etatElevateur = 3;
    }
    else if (elevateurDirectionDemandee > 0 && !elevateurLimitSwitchBas) {
        // Si on veut descendre l'élévateur et que la limit switch n'est pas enfoncée
        elevateurDirectionCommandee = elevateurDirectionDemandee;
        etatElevateur = 4;
    }
    else {
        // Sinon, ça veut dire que l'élévateur ne bouge pas ou une des conditions redondantes commentées ci-dessus
        elevateurDirectionCommandee = elevateurDirectionDemandee;
        etatElevateur = 5;
    }

    if (elevateurDirectionCommandee + ELEVATEUR_CORRECTION_GRAVITE >= -128 && !elevateurLimitSwitchBas) {
        elevateurDirectionCommandee += ELEVATEUR_CORRECTION_GRAVITE;
    }
}

void updateMoteurs() {
    CrcLib::MoveArcade(driveSpeed.x, driveSpeed.y, DRIVE_MOTOR_L, DRIVE_MOTOR_R);
    CrcLib::SetPwmOutput(PINCE_MOTEUR, pinceDirectionCommandee);
    CrcLib::SetPwmOutput(LANCEUR_MOTEUR, lanceurSpeed);
    CrcLib::SetPwmOutput(FLIPPEUX_MOTEUR, flippeuxSpeed);
    CrcLib::SetPwmOutput(ELEVATEUR_MOTEUR, elevateurDirectionCommandee);
}

#ifdef DEBUG_LOG

void debugLog() {
    char buffer[100];
    sprintf(buffer, "pls: %d, pe: %ld, pdc: %d, pdd: %d, ml: %d, mr: %d, ep: %d\n",
        pinceLimitSwitch, readEncoder(), pinceDirectionCommandee, pinceDirectionDemandee, manette.l1, manette.r1, etatPince);
    Serial.print(buffer);
}

#endif
