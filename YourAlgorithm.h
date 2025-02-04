#ifndef Your_Algorithm
#define Your_Algorithm

#include "Globals.h"
#include "IRcontrol.h"
#include "MotorControl.h"
#include "Navigation.h"

extern bool startAlgorithm;
extern bool startAlgorithm2;

void setupAlgorithm();
void loopAlgorithm();

void mainAlgorithm();
void basicAlgo();
// void demoSqaureAlgo();
// void testAlgo();
// void oneSqrTest(int speed);

void startOffsetAction();
void moveForwardAction();
void turnLeftAction();
void turnRightAction();
void turnAroundAction();

void testMotors();
void testIrLeds();
void testIrReadings();


#endif // Your_Algorithm