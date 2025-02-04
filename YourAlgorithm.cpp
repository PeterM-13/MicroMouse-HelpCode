#include "YourAlgorithm.h"

/* TODO

 Use only on sensor on lane centering

 Compare front values before a turn?

 steps!? 
 Constantly monitor IR -> record last wall gap. Reset step.

*/

bool startAlgorithm = false;
bool startAlgorithm2 = false;

void setupAlgorithm()
{
  delay(500); // Allow button press

  startOffsetAction();
}



void loopAlgorithm()
{ 
  if(startAlgorithm) // Set True after button 1 pressed
  {
      mainAlgorithm();
  }
  else if(startAlgorithm2)  // Set True after button 2 pressed
  {
     //addStartCheckingWallsAction();
     //addDelayAction(100);
     //addIrMonitoringAction(80);
    startAlgorithm = false;
    startAlgorithm2 = false;
  }
}
 

void mainAlgorithm()
{
  if(currentActionComplete && isBufferEmpty(actionBuffer))
  {
    if (checkMiddle())
    {
      startAlgorithm = false;
      ledOn();
      return;
    }

    if(!wallFront)
    {
      if(!wallRight && (bool)random(0,2))
      {
        turnRightAction();
      }
      else if(!wallLeft && (bool)random(0,2))
      {
        turnLeftAction();
      }
      else
      {
        moveForwardAction();
      }
    }
    else if(!wallRight)
    {
      turnRightAction();
    }
    else if(!wallLeft)
    {
      turnLeftAction();
    }
    else // Reached dead end, turn around
    {
      turnAroundAction();
    }
  }
}



void startOffsetAction()
{
  addBlindMoveForwardAction(0.3, 70);
  addStartCheckingWallsAction();
  addBlindMoveForwardAction(0.2, 70);
  addCheckWallsAction();
}
void moveForwardAction()
{ 
  addMoveForwardAction(0.5, 100);
  addBlindMoveForwardAction(0.3, 100);
  addStartCheckingWallsAction();
  addBlindMoveForwardAction(0.2, 100);
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
    addBlindMoveForwardAction(0.6, 70);
  }
  addTurnLeftAction(80);
  addStartCheckingWallsAction();
  addBlindMoveForwardAction(0.85, 70);
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
    addBlindMoveForwardAction(0.6, 70);
  }
  addTurnRightAction(80);
  addStartCheckingWallsAction();
  addBlindMoveForwardAction(0.85, 70);
  addCheckWallsAction();
}
void turnAroundAction()
{
  addIrMonitoringAction(70);
  addTurnAroundAction(80);
  addBlindReverseAction(0.5, 68);
  startOffsetAction();
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
  //Serial.println(String(getIRreading(FRONT_LEFT_LED))+","+String(getIRreading(LEFT_LED))+","+String(getIRreading(RIGHT_LED))+","+String(getIRreading(FRONT_RIGHT_LED)));
  laneCenteringActive = true; // Makes the IR sensor values update constantly   
  Serial.print(String(irReadings[FRONT_LEFT_LED][IR_VALUE]));
  Serial.print(",");
  Serial.print(String(irReadings[LEFT_LED][IR_VALUE]));
  Serial.print(",");
  Serial.print(String(irReadings[RIGHT_LED][IR_VALUE]));
  Serial.print(",");
  Serial.println(String(irReadings[FRONT_RIGHT_LED][IR_VALUE]));
}
