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
void initGrabber();  // *** ADDED ***

// Elevator state machine function pointer
extern void (*actionElevateur)();
extern void (*actionGrabber)();  // *** ADDED ***

// Elevator variables
extern int8_t elevateurSpeed;
extern bool aimantAligne;

// Command functions
void controlElevateur();
void controlGrabber();  // *** ADDED ***
void controlArm();

#endif //ROBOT_2024_ROBOT_ACTIONS_CTRL_H