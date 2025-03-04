#ifndef Motor_Control_H
#define Motor_Control_H

#include "Globals.h"
#include "IRcontrol.h"
#include "Gyro.h"
#include "Navigation.h"

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
extern int stepsSincelastWallGap;

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

void resetVariables();

// Moves mouse forwards a certain number of cells
void moveForward(float nCells);
// Moves mouse backwards a certain number of cells
void reverse(float nCells);

// void rotateWithEncoders(float angle);
// void rotateWithGyro(float angle);
void turnRight();
void turnLeft();
void turnAround();

void startIrMonitoring();

void laneCenter();

void resetCollisionDetection();
void detectCollisionWithSteps();

void startActionDelay(float delay_ms);

// This is called if collision detected
void collisionSolution();

void laneCenterPID();

#endif // Motor_Control_H