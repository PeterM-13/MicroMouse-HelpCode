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
extern bool wallFrontClose;
extern bool fatalError;
extern bool collisionDetectionActive;


// ------------- Define Global Functions ----------
void setupGlobals();

void print(String text, int priority = 1);
void ledOn();
void ledOff();


// ------------- Define Global Type(s) -------------
enum LEDs
{
  FRONT_LEFT_LED = 1,
  LEFT_LED = 2,
  RIGHT_LED = 3,
  FRONT_RIGHT_LED = 4,
};


#endif // GLOBALS_H