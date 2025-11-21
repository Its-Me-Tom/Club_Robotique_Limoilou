//
// Created by clovis on 3/19/24.
//

#include "main.h"
#include "manette.h"
#include "pince.h"

#define ANALOG_DEADZONE 10
#define DUREE_ACTIVATION_LANCEUR 10000

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
    manette.xbox = false;
    manette.leftJoystick.x = 0; // -128 à 127
    manette.leftJoystick.y = 0;
    manette.rightJoystick.x = 0;
    manette.rightJoystick.y = 0;
    manette.leftTrigger = 0;
    manette.rightTrigger = 0;
}

void readManette() {
    // Mettre en commentaire les boutons non utilisés pour économiser des ressources.
    manette.a = CrcLib::ReadDigitalChannel(BUTTON::COLORS_DOWN);
    manette.b = CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT);
    manette.x = CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT);
    manette.y = CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP);
    manette.up = CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP);
    manette.down = CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN);
    manette.left = CrcLib::ReadDigitalChannel(BUTTON::ARROW_LEFT);
    manette.right = CrcLib::ReadDigitalChannel(BUTTON::ARROW_RIGHT);
    //manette.select = CrcLib::ReadDigitalChannel(BUTTON::SELECT);
    //manette.start = CrcLib::ReadDigitalChannel(BUTTON::START);
    manette.l1 = CrcLib::ReadDigitalChannel(BUTTON::L1);
    manette.r1 = CrcLib::ReadDigitalChannel(BUTTON::R1);
    //manette.l2 = CrcLib::ReadDigitalChannel(BUTTON::HATL);
    //manette.r2 = CrcLib::ReadDigitalChannel(BUTTON::HATR);
    //manette.xbox = CrcLib::ReadDigitalChannel(BUTTON::LOGO);
    manette.leftJoystick.x = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X);
    manette.leftJoystick.y = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y);
    //manette.rightJoystick.x = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X);
    manette.rightJoystick.y = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y);
    manette.leftTrigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L);
    manette.rightTrigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R);
}

/*
 * Fonction originale avant inversion de l'axe horizontal du joystick, 0000
// Vérifier la position du joystick gauche et appliquer la valeur correspondante à driveSpeed
void verifieCommandeDriveJoy() {
    if ((abs(manette.leftJoystick.x) > 10 || abs(manette.leftJoystick.y) > 10)) {
        if (manette.a) { // Crawl mode
            driveSpeed.x = (int8_t)(-(manette.leftJoystick.x / 4)); // Ça prend plus de jus pour tourner
            driveSpeed.y = (int8_t)(-(manette.leftJoystick.y / 5));
        }
        else {
            if (manette.leftJoystick.x == -128) {
                driveSpeed.x = 127; // Pour éviter que ça overflow
            } else if (manette.leftJoystick.x == 127) {
                driveSpeed.x = -128;
            } else {
                driveSpeed.x = -manette.leftJoystick.x;
            }

            if (manette.leftJoystick.y == -128) {
                driveSpeed.y = 127; // Pour éviter que ça overflow
            } else if (manette.leftJoystick.y == 127) {
                driveSpeed.y = -128;
            } else {
                driveSpeed.y = -manette.leftJoystick.y;
            }
        }
    }
}
*/


// Vérifier la position du joystick gauche et appliquer la valeur correspondante à driveSpeed
void verifieCommandeDriveJoy() {
    if ((abs(manette.leftJoystick.x) > 10 || abs(manette.leftJoystick.y) > 10)) {
        if (manette.a) { // Crawl mode
            driveSpeed.x = (int8_t)((manette.leftJoystick.x / 4)); // Ça prend plus de jus pour tourner
            driveSpeed.y = (int8_t)(-(manette.leftJoystick.y / 5));
        }
        else {
            driveSpeed.x = manette.leftJoystick.x;
            /*
             * 0000
            if (manette.leftJoystick.x == -128) {
                driveSpeed.x = 127; // Pour éviter que ça overflow
            } else if (manette.leftJoystick.x == 127) {
                driveSpeed.x = -128;
            } else {
                driveSpeed.x = -manette.leftJoystick.x;
            }
*/
            if (manette.leftJoystick.y == -128) {
                driveSpeed.y = 127; // Pour éviter que ça overflow
            } else if (manette.leftJoystick.y == 127) {
                driveSpeed.y = -128;
            } else {
                driveSpeed.y = -manette.leftJoystick.y;
            }
        }
    }
}

// Même chose que verifieCommandeDrive, mais avec le DPad. Sert à rouler ben lentement.
void verifieCommandeDriveDPad() {
    if (manette.up) {
        driveSpeed.x = 0;
        driveSpeed.y = 25;

        // inversion 0000
        driveSpeed.x = -driveSpeed.x;
    }

    if (manette.up && manette.right) {
        driveSpeed.x = -25;
        driveSpeed.y = 12;

        // inversion 0000
        driveSpeed.x = -driveSpeed.x;
    }

    if (manette.right && !manette.up) {
        driveSpeed.x = -50;
        driveSpeed.y = 0;

        // inversion 0000
        driveSpeed.x = -driveSpeed.x;
    }

    if (manette.right && manette.down) {
        driveSpeed.x = -25;
        driveSpeed.y = -12;

        // inversion 0000
        driveSpeed.x = -driveSpeed.x;
    }

    if (manette.down && !manette.right) {
        driveSpeed.x = 0;
        driveSpeed.y = -25;

        // inversion 0000
        driveSpeed.x = -driveSpeed.x;
    }

    if (manette.down && manette.left) {
        driveSpeed.x = 25;
        driveSpeed.y = -12;

        // inversion 0000
        driveSpeed.x = -driveSpeed.x;
    }

    if (manette.left && !manette.down) {
        driveSpeed.x = 50;
        driveSpeed.y = 0;

        // inversion 0000
        driveSpeed.x = -driveSpeed.x;
    }

    if (manette.left && manette.up) {
        driveSpeed.x = 25;
        driveSpeed.y = 12;

        // inversion 0000
        driveSpeed.x = -driveSpeed.x;
    }

}

void verifieCommandeLanceur() {/*
    //static unsigned long lastMillis = 0;
    static unsigned long lastCommandMillis = 0;
    long long timeDiff = millis() - lastCommandMillis;

    if (manette.x && !manette.b) {
        //lanceurSpeed = 127;
        lanceurSpeed = 95;
        lastCommandMillis = millis();
    }
    else if (!manette.x && manette.b) {
        lanceurSpeed = 64;
        lastCommandMillis = millis();
    }
    else {
        lanceurSpeed = 0;
    }
    */

    //static unsigned long lastMillis = 0;
    static unsigned long lastCommandMillis = 0;
    long long timeDiff = millis() - lastCommandMillis;

    if (manette.x && !manette.b) {
        //lanceurSpeed = 127;
        lanceurSpeed = 112;
        lastCommandMillis = millis();
    }
    else if (!manette.x && manette.b) {
        lanceurSpeed = 95;
        lastCommandMillis = millis();
    }

    if (timeDiff > DUREE_ACTIVATION_LANCEUR) {
        lanceurSpeed = 0;
    }
}

void verifieCommandeFlippeux() {
    if (manette.leftTrigger > -128 + ANALOG_DEADZONE && manette.rightTrigger > -128 + ANALOG_DEADZONE) {
        flippeuxSpeed = 0;
    }
    else if (manette.leftTrigger > -128 + ANALOG_DEADZONE) {
        flippeuxSpeed = (int8_t)(-(((manette.leftTrigger + 128) / 2) / 3));
    }
    else if (manette.rightTrigger > -128 + ANALOG_DEADZONE) {
        flippeuxSpeed = (int8_t)(((manette.rightTrigger + 128) / 2) / 3);
    }
    else {
        flippeuxSpeed = 0;
    }

    if (manette.y && flippeuxSpeed + FLIPPEUX_CORRECTION_GRAVITE <= 127) {
        flippeuxSpeed += FLIPPEUX_CORRECTION_GRAVITE;
    }
}

void verifieCommandePince() {
    /*
    // inversé 20 fév 01h05 (r -> l, l -> r)
    if (manette.r1) {
        pinceDirectionDemandee = VITESSE_PINCE;
    }
    else if (manette.l1) {
        pinceDirectionDemandee = -VITESSE_PINCE;
    }
    */
    static bool lockPiton = false;

    if (manette.r1 && !lockPiton) {
        // Piton était pas enfoncé et est maintenant enfoncé, front montant
        lockPiton = true;
        if (pinceDirectionDemandee == 0) {
            pinceDirectionDemandee = VITESSE_PINCE;
        }
        pinceDirectionDemandee = -pinceDirectionDemandee;
    }

    if (!manette.r1 && lockPiton) {
        // Piton était enfoncé et a été lâché, front descendant
        lockPiton = false;
    }
}

void verifieCommandeElevateur() {
    if (manette.rightJoystick.y < -ANALOG_DEADZONE) {
        // monter
        elevateurDirectionDemandee = manette.rightJoystick.y;
    }
    else if (manette.rightJoystick.y > ANALOG_DEADZONE) {
        // descendre
        elevateurDirectionDemandee = manette.rightJoystick.y;
    }
    else {
        elevateurDirectionDemandee = 0;
    }
}