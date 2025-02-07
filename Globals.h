#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <array>
#include <cmath>

#include "ActionBuffer.h"


// ------------- Define Global Vars ----------
// Constants
extern const int DEBUG_MODE;

// Variables
extern bool wallLeft;
extern bool wallRight;
extern bool wallBack;
extern bool wallFront;
extern bool fatalError;
extern bool collisionDetectionActive;
extern bool laneCenteringActive;
extern bool readAllSensorsOnce;
extern bool readAllSensorsCont; // continuously
extern bool updateGyroData;

// ------------- Define Global Functions ----------
void setupGlobals();

void print(String text, int priority = 1);
void ledOn();
void ledOff();
void printLoopTime();

// ------------- Define Global Type(s) -------------
enum LEDs
{
  FRONT_LEFT_LED = 0,
  LEFT_LED = 1,
  RIGHT_LED = 2,
  FRONT_RIGHT_LED = 3,
};
enum irReadingsIndexes
{
  IR_VALUE = 0, // Most recent IR value
  AMBIENT_VALUE = 1,  // Most recent recoring of ambient light
  TIME = 2, // Time to turn LED off and reord results.
};

#endif // GLOBALS_H