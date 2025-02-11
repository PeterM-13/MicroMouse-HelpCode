#include "Globals.h"

// Constants

// Maze Variables
bool wallLeft = true;
bool wallRight = true;
bool wallBack = true;
bool wallFront = false;

// Progrm Variables
bool fatalError = false; // If True causes mouse to stop
bool collisionDetectionActive = true;
bool laneCenteringActive = false;
bool updateGyroData = false;

void setupGlobals()
{  
  // Debug Setup
  if(DEBUG_MODE > 0)
  {
    Serial.begin(9600);
    while (!Serial);  // Wait for Serial Monitor to open before continuing
  }

  // Intialise a circular buffer for actions
  initBuffer(actionBuffer);
}


// Function to replace 'Serial.println()' based on global var 'debugMode'
void print(String text, int priority)
{
  if(DEBUG_MODE >= priority)
  {
    // Serial.print(millis()); // Prints timestamps - useful for debugging
    Serial.println(text);
  }
}

void ledOn()
{
  digitalWrite(LED_BUILTIN, HIGH);
}
void ledOff()
{
  digitalWrite(LED_BUILTIN, LOW);
}


long prevLoopTime_us = 0;
void printLoopTime()
{
  const long currentLoopTime_us = micros();
  print(String(currentLoopTime_us - prevLoopTime_us));
  prevLoopTime_us = currentLoopTime_us;
}
