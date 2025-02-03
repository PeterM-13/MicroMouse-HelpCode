#include "YourAlgorithm.h"

bool startAlgorithm = false;
bool startAlgorithm2 = false;

// These vars are currently unused, but might help :)
int currentCell[2];
int relativeDirection = NORTH;


void setupAlgorithm()
{
  delay(500); // Allow button press

  currentCell[X] = 0;
  currentCell[Y] = 0;


  addBlindMoveForwardAction(0.3, 70);
  addStartCheckingWallsAction();
  addBlindMoveForwardAction(0.2, 70);
  addCheckWallsAction();
}



void loopAlgorithm()
{ 
  if(startAlgorithm) // Set True after button 1 pressed
  {
      mainAlgorithm();
      //testAlgo();
  }
  else if(startAlgorithm2)  // Set True after button 2 pressed
  {
     addStartCheckingWallsAction();
     addDelayAction(100);
     addIrMonitoringAction(80);
    //testIrLeds();
    //testIrReadings();
    //testMotors();
    //testAlgo();
    //oneSqrTest(100);
    startAlgorithm = false;
    startAlgorithm2 = false;
  }
}
 

void mainAlgorithm()
{
  
  if(currentActionComplete && isBufferEmpty(actionBuffer))
  {
    if(!wallFront)
    {
      addMoveForwardAction(0.5, 100);
      addBlindMoveForwardAction(0.3, 100);
      addStartCheckingWallsAction();
      addBlindMoveForwardAction(0.2, 100);
      addCheckWallsAction();
    }
    else if(!wallRight)
    {
      addIrMonitoringAction(70); // Moves until IR reading of 70 on Front sensors (so close to wall)
      addTurnRightAction(80);
      addStartCheckingWallsAction();
      addBlindMoveForwardAction(0.85, 70);
      addCheckWallsAction();
    }
    else if(!wallLeft)
    {
      addIrMonitoringAction(70); // Moves until IR reading of 70 on Front sensors (so close to wall)
      addTurnLeftAction(80);
      addStartCheckingWallsAction();
      addBlindMoveForwardAction(0.85, 70);
      addCheckWallsAction();
    }
    else // Reached dead end, turn around
    {
      addIrMonitoringAction(70);
      addTurnAroundAction(80);
      addBlindReverseAction(0.5, 68);
      // Restart
      addBlindMoveForwardAction(0.3, 70);
      addStartCheckingWallsAction();
      addBlindMoveForwardAction(0.2, 70);
      addCheckWallsAction();
    }
  }
}


void oneSqrTest(int speed)
{
  addBlindMoveForwardAction(1, speed);
  startAlgorithm = false;
  startAlgorithm2 = false;
}


void testAlgo()
{
  // For testing

  startAlgorithm2 = false;
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
