#include "YourAlgorithm.h"

bool startAlgorithm = false;
bool startAlgorithm2 = false;

/* *******

  These variables are available to read from:
  > currentDirection = NORTH, EAST, SOUTH or WEST.
  > currentCell[X] / currentCell[Y] = Current X, Y position in maze. 

  * [0,0] = bottom left.
  * Default starting position is [0,0] facing EAST -> Defined in Navigation.cpp.
  * Update this if want to test your mouse in a different starting position/orientation.

 ********/

void setupAlgorithm()
{
  delay(1000); // Allow button press
  setupNavigation(); // Resets starting position
  startOffsetAction(); // Move forward slightly to see walls ahead
}


void loopAlgorithm()
{ 
  if(startAlgorithm) // Set True after button 1 pressed
  {
      mainAlgorithm();
  }
  else if(startAlgorithm2)  // Set True after button 2 pressed
  {
    // // Restart main algorithm, remembering last position
    // startAlgorithm2 = false;
    // startAlgorithm = true;
    // startOffsetAction();

    testIrReadings();
  }
}


// Algorithm that makes decisions based on proximity to center.
void mainAlgorithm()
{
  if(currentActionComplete && isBufferEmpty(actionBuffer))
  {

    // Check if reached the middle. Stop if reached.
    if (checkMiddle())
    {
      startAlgorithm = false;
      parkMotors();
      ledOn();
      return;
    }

    if(!wallFront)
    {
      if(!wallRight && distToMiddle(getCell(EAST)) < distToMiddle(getCell(NORTH)))
      {
        // No wall front or right. Turn right as it's close to middle.
        turnRightAction();
      }
      else if(!wallLeft && distToMiddle(getCell(WEST)) < distToMiddle(getCell(NORTH)))
      {
        // No wall front or left. Turn left as it's close to middle.
        turnLeftAction();
      }
      else
      {
        // No wall forwards, so go forwards.
        moveForwardAction();
      }
     // Not considered: (!wallLeft && !wallRight), or decisions of equel distance to middle.
    }
    else if(!wallRight && !wallLeft)
    {
      if(distToMiddle(getCell(EAST)) > distToMiddle(getCell(WEST)))
      {
        turnLeftAction();
      }
      else
      {
        turnRightAction();
      }
      // Not considered: right and left == same dist to middle.
    }
    else if(!wallRight)
    {
      // If wall in front and no wall right, turn right.
      turnRightAction();
    }
    else if(!wallLeft)
    {
      // If wall front and no wall left, turn left.
      turnLeftAction();
    }
    else 
    {
      // Reached dead end, so turn around.
      turnAroundAction();
    }
  }
}




// ------------- Algorithms for hardware tests -------------
// Run these to identify issues with the hardware.
void testMotors()
{
  // Drives the mouse forwards at varying speed, slow to fast.
  addBlindMoveForwardAction(1, 60);
  addBlindMoveForwardAction(1, 75);
  addBlindMoveForwardAction(1, 100);
  addParkAction();
  startAlgorithm = false;
  startAlgorithm2 = false;
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
  Serial.print(String(irReadings[FRONT_LEFT_LED][IR_VALUE]));
  Serial.print(",");
  Serial.print(String(irReadings[LEFT_LED][IR_VALUE]));
  Serial.print(",");
  Serial.print(String(irReadings[RIGHT_LED][IR_VALUE]));
  Serial.print(",");
  Serial.println(String(irReadings[FRONT_RIGHT_LED][IR_VALUE]));
}







// ------------ Simplified navigation functions -----------
// Modify at your own risk!

void startOffsetAction()
{
  navForward();
  addBlindMoveForwardAction(0.75, 70);
  addCheckWallsAction();
}
void moveForwardAction()
{ 
  navForward();
  addMoveForwardAction(1.0, 100);
  addCheckWallsAction();
}
void turnLeftAction()
{
  if(wallFront)
  {
    addIrMonitoringAction(70); 
  }
  else
  {
    const float DIST_OFFSET = 0.8 - (stepsSincelastWallGap/(float)CELL_DISTANCE);
    if(stepsSincelastWallGap != 0 && DIST_OFFSET >= 0.0)
    {
      addBlindMoveForwardAction(DIST_OFFSET, 70);
    }
    else
    {
      wallLeft = true;
      return;
    }
  }
  navLeft();
  navForward();
  addTurnLeftAction(80);
  addMoveForwardAction(0.85, 70);
  addCheckWallsAction();
}
void turnRightAction()
{
  if(wallFront)
  {
    addIrMonitoringAction(70); 
  }
  else
  {
    const float DIST_OFFSET = 0.8 - (stepsSincelastWallGap/(float)CELL_DISTANCE);
    if(stepsSincelastWallGap != 0 && DIST_OFFSET >= 0.0)
    {
      addBlindMoveForwardAction(DIST_OFFSET, 70);
    }
    else
    {
      wallRight = true;
      return;
    }
  }
  navRight();
  navForward();
  addTurnRightAction(80);
  addMoveForwardAction(0.85, 70);
  addCheckWallsAction();
}
void turnAroundAction()
{
  navRight();
  navRight();
  addIrMonitoringAction(70);
  addTurnAroundAction(80);
  addBlindReverseAction(0.5, 68);
  startOffsetAction();
}