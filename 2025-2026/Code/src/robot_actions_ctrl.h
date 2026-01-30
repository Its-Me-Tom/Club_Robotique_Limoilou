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
void initMonteElevateur();
void monteElevateur();
void initDescendElevateur();
void descendElevateur();
void bougePasElevateur();

// Elevator state machine function pointer
extern void (*actionElevateur)();

// Elevator variables
extern int8_t elevateurSpeed;
extern bool aimantAligne;
extern int8_t elevateurDirectionCommandee;  // ADDED THIS
extern int8_t lanceurSpeed;  // ADDED THIS

// Command functions
void verifieCommandeElevateurAuto();
void verifieCommandeElevateurManuel();
void verifieCommandeLanceur();

#endif //ROBOT_2024_ROBOT_ACTIONS_CTRL_H