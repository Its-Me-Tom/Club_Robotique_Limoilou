//
// Created by clovis on 25-02-03.
//

#include "pince.h"
#include "encoder.h"

#include <main.h>

#define VAL_ENCODEUR_PINCE_OUVERTE (-250)

int8_t etatPince = 0;

void initPince() {
    CrcLib::SetDigitalPinMode(PINCE_LIMIT_SWITCH, INPUT);
}

void updatePince() {
    //static int etatPince = 0;

    // 0: En train de tourner vers la limit switch
    // 1: Limit switch enfonçée pour la première fois
    // 2: Rouverture de la pince
    // 3: Initialisation faite, état nominal

    pinceLimitSwitch = CrcLib::GetDigitalInput(PINCE_LIMIT_SWITCH) == HIGH;


    switch (etatPince) {
        case 0:
            pinceDirectionCommandee = VITESSE_PINCE;
            if (pinceLimitSwitch) {
                etatPince = 1;
                CrcLib::PlayTune(CrcUtility::TUNE_LOW_TONE, false);
            }
        break;
        case 1:
            pinceDirectionCommandee = 0;
            initEncoder();
            etatPince = 2;
        break;
        case 2:
            pinceDirectionCommandee = -VITESSE_PINCE;
            if (readEncoder() <= VAL_ENCODEUR_PINCE_OUVERTE) {
                CrcLib::PlayTune(CrcUtility::TUNE_HIGH_TONE, false);
                etatPince = 3;
            }
        break;
        case 3:
            int32_t encoderPos = readEncoder();
            if (pinceDirectionDemandee > 0 && !pinceLimitSwitch) {
                // On ouvre la pince
                pinceDirectionCommandee = pinceDirectionDemandee;
            }
            else if (pinceDirectionCommandee > 0 && pinceLimitSwitch) {
                // Pince grande ouverte
                pinceDirectionCommandee = 0;
            }
            else if (pinceDirectionDemandee < 0 && encoderPos > VAL_ENCODEUR_PINCE_OUVERTE) {
                // On ferme la pince
                pinceDirectionCommandee = pinceDirectionDemandee / 1.22;
            }
            else if (pinceDirectionDemandee <= 0 && encoderPos < VAL_ENCODEUR_PINCE_OUVERTE) {
                pinceDirectionCommandee = 0;
            }

            //if (pinceDirectionDemandee == 0 && encoderPos < )
/*
            char buffer[100];
            sprintf(buffer, "pls: %d, pe: %ld, pdc: %d, pdd: %d, ep: %d\n",
                pinceLimitSwitch, encoderPos, pinceDirectionCommandee, pinceDirectionDemandee, etatPince);
            Serial.print(buffer);*/
        break;
        default:
            pinceDirectionCommandee = 0;
    }

}