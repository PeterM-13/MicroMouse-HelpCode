#ifndef Motor_Control_H
#define Motor_Control_H

#include "Globals.h"
#include "IRcontrol.h"
#include "Gyro.h"

enum motorDirection
{
    reversing = -1,
    parked = 0,
    driving = 1,
};

enum spinDirection
{
  antiClockwise = -1,
  notSpinning = 0,
  clockwise = 1,
};

extern bool laneCenteringActive;

void setupMotors();
void loopMotors();

// Keep track of encoder cycles
void updateLeftEncoder();
void updateRightEncoder();
// Sets motor bias back to pre-defined value (found with trial and error)
void resetMotorBias();

// Turns motors OFF
void parkMotors(bool withBrake=false);
// Turns motors ON, rotating forwards
void driveMotors();
// Drives motros in opposite directions (for turning)
void driveMotorsOpposite(bool clockwise);
// Rotates both motors backwards
void reverseMotors();
// Sets both motor's speeds independenty
void setSpeed(int motor1, int motor2);

// Moves mouse forwards a certain number of cells
void moveForward(float nCells);
// Moves mouse backwards a certain number of cells
void reverse(float nCells);
// Turn on the spot
void rotate(float angle);
void rotateWithEncoders(float angle);
void rotateWithGyro(float angle);

void laneCenter();

void resetCollisionDetection();
void detectCollisionWithSteps();

void startActionDelay(float delay_ms);

// This is called if collision detected
void collisionSolution();



#endif // Motor_Control_H