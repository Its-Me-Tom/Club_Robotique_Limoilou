//==============================================================================
// ROBOT_ACTIONS_CTRL.H - Robot Actions Control
//==============================================================================
// Handles robot-specific actions (elevator, launcher, etc.)
// Includes elevator state machine and manual controls
//==============================================================================

#ifndef ROBOT_2024_ROBOT_ACTIONS_CTRL_H
#define ROBOT_2024_ROBOT_ACTIONS_CTRL_H

// Forward declarations for elevator state machine functions
void initElevateur();
void bougePasElevateur();
void monteElevateur();
void descendElevateur();

// Elevator state machine function pointer
extern void (*actionElevateur)();

// Elevator variables
extern int8_t elevateurSpeed;
extern bool aimantAligne;

// Command functions
void verifieCommandeLanceur();
void verifieCommandeGrabber();
void verifieCommandeArm();

#endif //ROBOT_2024_ROBOT_ACTIONS_CTRL_H