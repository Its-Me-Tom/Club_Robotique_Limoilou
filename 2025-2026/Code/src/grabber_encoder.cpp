//
// Created by clovis on 25-02-05.
// non utiliser mais ustile su un moteur a besoin detre encoder
//

#include "grabber_encoder.h"

Encoder motor1Enco(CRC_ENCO_A, CRC_ENCO_B);

// Appeler cette fonction une fois que la limit switch a été pesée
void initEncoder() {
    // Tourner jusqu'à la limit switch ici, avant write(0)
    motor1Enco.write(0);
}

int32_t readEncoder() {
    return motor1Enco.read();
}