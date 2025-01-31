#ifndef Your_Algorithm
#define Your_Algorithm

#include "Globals.h"
#include "IRcontrol.h"
#include "MotorControl.h"

extern bool startAlgorithm;
extern bool startAlgorithm2;

void setupAlgorithm();
void loopAlgorithm();

void mainAlgorithm();
void basicAlgo();
void demoSqaureAlgo();
void testAlgo();
void oneSqrTest(int speed);

void testMotors();
void testIrLeds();
void testIrReadings();

// Define Types
enum coordinates
{
    X = 0,
    Y = 1,
};
enum compassDirection
{
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3,
};


#endif // Your_Algorithm