//==============================================================================
// MANETTE.H - Controller Input Reading
//==============================================================================
// Handles reading all controller inputs (buttons, joysticks, triggers)
// Created by clovis on 3/19/24.
//==============================================================================

#ifndef ROBOT_2024_MANETTE_H
#define ROBOT_2024_MANETTE_H

void initManette();
void readManette();

struct manette_s {
    bool a, b, x, y, up, down, left, right, select, start, l1, r1, l2, r2, l3, xbox;
    coords leftJoystick, rightJoystick;
    int8_t leftTrigger, rightTrigger;
};

extern manette_s manette;

#endif //ROBOT_2024_MANETTE_H