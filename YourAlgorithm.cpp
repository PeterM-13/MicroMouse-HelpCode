#include "YourAlgorithm.h"

bool startAlgorithm = false;
bool startAlgorithm2 = false;

// These vars are currently unused, but might help :)
int currentCell[2];
int relativeDirection = NORTH;


void setupAlgorithm()
{
  delay(500); 

  currentCell[X] = 0;
  currentCell[Y] = 0;

  addBlindMoveForwardAction(0.5, 70);
  addCheckWallsAction();
}

void loopAlgorithm()
{
  if(startAlgorithm) // Set True after button 1 pressed
  {
      // ********* Un-comment the algorithm you wish to run *******
      mainAlgorithm();
      //basicAlgo();
      //testAlgo();
      //oneSqrTest(100);

      // --- Hardware test algorithms ---
      //testMotors();
      //testIrLeds();
       //testIrReadings();
  }
  else if(startAlgorithm2)  // Set True after button 2 pressed
  {
    //testIrLeds();
    testIrReadings();
    //testAlgo();
    //oneSqrTest(50);
    startAlgorithm = false;
    //startAlgorithm2 = false;
  }
  
  // if(wallFrontClose){
  //   ledOn();
  // }else{
  //   ledOff();
  // }
}
 

void mainAlgorithm()
{
  // *********   You can put your Maze-Solving Algorithm here   *********
  if(currentActionComplete && isBufferEmpty(actionBuffer))
  {
    if(!wallFront)
    {
      addMoveForwardAction(0.5, 100);
      addBlindMoveForwardAction(0.5, 100);
      addCheckWallsAction();
    }
    else if(!wallRight)
    {
      addBlindMoveForwardAction(0.85, 70);
      addRotateAction(90.0, 70);
      addBlindMoveForwardAction(0.15, 70);
      addCheckWallsAction();
    }
    else if(!wallLeft)
    {
      addBlindMoveForwardAction(0.85, 70);
      addRotateAction(-90.0, 80);
      addBlindMoveForwardAction(0.15, 70);
      addCheckWallsAction();
    }
    else
    {
      startAlgorithm = false;
      ledOn();
    }
  }

  collisionDetectionActive = true;
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

void oneSqrTest(int speed)
{
  addBlindMoveForwardAction(1, speed);
  startAlgorithm = false;
  startAlgorithm2 = false;
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
  addDelayAction(500.0); 
  addRotateAction(-90.0, 70);
  addDelayAction(500.0); 
  addRotateAction(-90.0, 70);
  //addDelayAction(1000.0); 
  //addRotateAction(-90.0, 70);

  startAlgorithm2 = false;
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
