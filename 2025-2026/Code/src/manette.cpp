//
// Created by clovis on 3/19/24.
//

#include "main.h"
#include "manette.h"
#include "elevateur.h"

// ===== MODE DE CONTRÔLE =====
// true = Mode Rocket League (triggers pour avant/arrière, joystick X pour steering)
// false = Mode Joystick classique (joystick contrôle tout)
const bool ROCKET_LEAGUE_MODE = true;
// ============================

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

void readManette() {
    // Mettre en commentaire les boutons non utilisés pour économiser des ressources.
    manette.a = CrcLib::ReadDigitalChannel(BUTTON::COLORS_DOWN);
    //manette.b = CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT);
    manette.x = CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT);
    //manette.y = CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP);
    manette.up = CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP);
    manette.down = CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN);
    manette.left = CrcLib::ReadDigitalChannel(BUTTON::ARROW_LEFT);
    manette.right = CrcLib::ReadDigitalChannel(BUTTON::ARROW_RIGHT);
    //manette.select = CrcLib::ReadDigitalChannel(BUTTON::SELECT);
    //manette.start = CrcLib::ReadDigitalChannel(BUTTON::START);
    manette.l1 = CrcLib::ReadDigitalChannel(BUTTON::L1);
    manette.r1 = CrcLib::ReadDigitalChannel(BUTTON::R1);
    manette.l3 = CrcLib::ReadDigitalChannel(BUTTON::HATL); // HATL = left joystick click
    //manette.l2 = CrcLib::ReadDigitalChannel(BUTTON::HATL);
    //manette.r2 = CrcLib::ReadDigitalChannel(BUTTON::HATR);
    //manette.xbox = CrcLib::ReadDigitalChannel(BUTTON::LOGO);
    manette.leftJoystick.x = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X);
    manette.leftJoystick.y = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y);
    manette.rightJoystick.x = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X);
    manette.rightJoystick.y = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y);
    manette.leftTrigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L);
    manette.rightTrigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R);
}

// La variable elevateurDirectionCommandee va gentiment demander à la machine à états dans elevateur.cpp de monter,
// descendre ou ne pas bouger. C'est la machine à états qui décidera quoi faire de cette demande.
void verifieCommandeElevateurAuto() {
    if (manette.l1 && manette.l2) {
        elevateurDirectionCommandee = 0;
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

// Vérifier la position du joystick gauche et appliquer la valeur correspondante à driveSpeed
void verifieCommandeDriveJoy() {
    // Calcul du steering X (identique dans les deux modes)
    if (abs(manette.leftJoystick.x) > 10) {
        int8_t steering;
        if (manette.leftJoystick.x == -128) {
            steering = -96; // 75% de la vitesse
        } else if (manette.leftJoystick.x == 127) {
            steering = 95; // 75% de la vitesse
        } else {
            steering = (manette.leftJoystick.x * 7) / 8; // 87.5% of turning speed
        }
        
        // Crawl mode: 25% du steering
        if (manette.l3) {
            driveSpeed.x = steering / 3; // 75% / 3 = 25%
        } else {
            driveSpeed.x = steering;
        }
    } else {
        driveSpeed.x = 0;
    }
    
    // Calcul de l'avant/arrière Y (DIFFÉRENT selon le mode)
    int8_t forward_back = 0;
    
    if (ROCKET_LEAGUE_MODE) {
        // MODE ROCKET LEAGUE: Utiliser les triggers pour Y
        if (manette.rightTrigger > -118) {
            forward_back = (int8_t)map(manette.rightTrigger, -128, 127, 0, 127);
        } else if (manette.leftTrigger > -118) {
            forward_back = -(int8_t)map(manette.leftTrigger, -128, 127, 0, 127);
        }
    } else {
        // MODE NORMAL: Utiliser le joystick Y
        if (abs(manette.leftJoystick.y) > 10) {
            if (manette.leftJoystick.y == -128) {
                forward_back = 127;
            } else if (manette.leftJoystick.y == 127) {
                forward_back = -128;
            } else {
                forward_back = -manette.leftJoystick.y;
            }
        }
    }
    
    // Crawl mode pour Y (identique dans les deux modes)
    if (manette.l3) {
        driveSpeed.y = forward_back / 5; // Ça prend plus de jus pour tourner
    } else {
        driveSpeed.y = forward_back;
    }
}

// Contrôle numérique du DPad - fonctionne comme un joystick analogique
void verifieCommandeDriveDPad() {
    // Vitesse de base pour chaque direction du DPad
    const int8_t DPAD_BASE_SPEED = 50;
    
    int8_t dpad_x = 0;
    int8_t dpad_y = 0;
    
    // Calculer la composante X
    if (manette.right) {
        dpad_x += DPAD_BASE_SPEED;
    }
    if (manette.left) {
        dpad_x -= DPAD_BASE_SPEED;
    }
    
    // Calculer la composante Y
    if (manette.up) {
        dpad_y += DPAD_BASE_SPEED;
    }
    if (manette.down) {
        dpad_y -= DPAD_BASE_SPEED;
    }
    
    // Appliquer seulement si au moins un bouton est pressé
    if (manette.up || manette.down || manette.left || manette.right) {
        driveSpeed.x = dpad_x;
        driveSpeed.y = dpad_y;
    }
}

void verifieCommandeLanceur() {
    if (manette.x) {
        lanceurSpeed = 127;
    } else {
        lanceurSpeed = 0;
    }
}