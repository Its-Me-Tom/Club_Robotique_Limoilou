//==============================================================================
// BASE_CTRL.H - Robot Base/Drive Control
//==============================================================================
// Handles robot base movement control (joystick, DPad, Rocket League mode)
// Includes steering inversion when reversing in Rocket League mode
//==============================================================================

#ifndef ROBOT_2024_BASE_CTRL_H
#define ROBOT_2024_BASE_CTRL_H

void verifieCommandeDriveJoy();
void verifieCommandeDriveDPad();

#endif //ROBOT_2024_BASE_CTRL_H