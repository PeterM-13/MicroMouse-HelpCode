#include "YourAlgorithm.h"

bool startAlgorithm = false;
bool startAlgorithm2 = false;

// These vars are currently unused, but might help :)
int currentCell[2];
int relativeDirection = NORTH;


void setupAlgorithm()
{
  currentCell[X] = 0;
  currentCell[Y] = 0;
}

void loopAlgorithm()
{
  if(startAlgorithm) // Set True after button 1 pressed
  {
      // ********* Un-comment the algorithm you wish to run *******
      mainAlgorithm();
      // basicAlgo();
      // demoSqaureAlgo();
      // testAlgo();

      // --- Hardware test algorithms ---
      // testMotors();
      // testIrLeds();
      // testIrReadings();
  }
  else if(startAlgorithm2)  // Set True after button 2 pressed
  {
    testIrLeds();
  }
}
  
void mainAlgorithm()
{
  // *********   You can put your Maze-Solving Algorithm here   *********

 






  collisionDetectionActive = false;
  startAlgorithm = false; // This line stops this function looping
}


void basicAlgo()
{
  // Moves forward if possible; otherwise, tries turning right, then left. Stops if blocked.
  if(currentActionComplete && isBufferEmpty(actionBuffer))
  {
    if(!wallFront)
    {
      addMoveForwardAction(1, 70);
    }
    else if(!wallRight)
    {
      addBlindMoveForwardAction(0.55, 70);
      addRotateAction(90.0, 70);
      addBlindMoveForwardAction(0.25, 60);
      addCheckWallsAction();
    }
    else if(!wallLeft)
    {
      addBlindMoveForwardAction(0.55, 70);
      addRotateAction(-90.0, 80);
      addBlindMoveForwardAction(0.25, 60);
      addCheckWallsAction();
    }
    else
    {
      startAlgorithm = false;
      addBlindMoveForwardAction(0.5, 50);
    }
  }
  collisionDetectionActive = true;
}

void demoSqaureAlgo()
{
  // Goes in a square
  for( int i=0; i<4; i++){
    addBlindMoveForwardAction(1, 75);
    addRotateAction(90.0, 70);
  }
  startAlgorithm = false;
}

void testAlgo()
{
  // You can use this func to test things out

  startAlgorithm = false;
}







// ------------- Algorithms for hardware tests -------------
// Run these to identify issues with the hardware.

void testMotors()
{
  // Drives the mouse forwards at varying speed, slow to fast.
  addMoveForwardAction(1, 60);
  addMoveForwardAction(1, 75);
  addMoveForwardAction(1, 100);
  startAlgorithm = false;
}
void testIrLeds()
{
  // Turns each LED ON / OFF for 1 second
  // Use your phone camera to view if the LED turns on (invisible to human eye).
  // Should see a faint purple glow usning phone camera.
  testLed(FRONT_LEFT_LED);
  testLed(LEFT_LED);
  testLed(RIGHT_LED);
  testLed(FRONT_RIGHT_LED);
}
void testIrReadings()
{
  // Logs the readings from each phototransistor.
  // View logs in 'Serial Plotter'.
  // Watch values change as you put an object in front of each sensor.
  Serial.println(String(getIRreading(FRONT_LEFT_LED))+","+String(getIRreading(LEFT_LED))+","+String(getIRreading(RIGHT_LED))+","+String(getIRreading(FRONT_RIGHT_LED)));
}
