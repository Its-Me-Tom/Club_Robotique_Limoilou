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
void initGrabber();
void initArm();

// Elevator state machine function pointer
extern void (*actionElevateur)();
extern void (*actionGrabber)();

// Elevator variables
extern int8_t elevateurSpeed;

// Command functions
void controlElevateur();
void controlGrabber();
void controlArm();

#endif //ROBOT_2024_ROBOT_ACTIONS_CTRL_H