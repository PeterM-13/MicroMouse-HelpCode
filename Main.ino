#include "Main.h"


// Master Setup
void setup()
{
  // Light LED to show setup started
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // External File Setup
  setupGlobals();
  setupMotors();
  setupLEDs();
  setupGyro();
  setupOtherIO();

  // Show Setup is complete 
  digitalWrite(LED_BUILTIN, LOW);
}


// Master Loop
void loop()
{
  // External file loops
  loopOtherIO();
  loopGyro();
  loopMotors();
  loopLEDs();
  loopAlgorithm();

  // Main event handler
  // Gets next action from buffer to perform
  if(currentActionComplete == true)
  {
    Action action;
    if(getAction(actionBuffer, action)) 
    {
      currentActionComplete = false;
      if(action.motor1Speed >= 0 && action.motor2Speed >= 0)
      {
        setSpeed(action.motor1Speed, action.motor2Speed);
      }
      switch (action.type) 
      {
        case ACTION_TYPE_MOVE_BASIC_FORWARDS:
          print("ACTION: Blind Move Forwards");
          moveForward(action.nCells);
          break;
        case ACTION_TYPE_MOVE_FORWARDS:
          print("ACTION: Move Forwards");
          laneCenteringActive = true;
          moveForward(action.nCells);
          break;
        case ACTION_TYPE_TURN_LEFT:
          print("ACTION: Turn left");
          turnLeft();
          break;
        case ACTION_TYPE_TURN_RIGHT:
          print("ACTION: Turn right");
          turnRight();
          break;
        case ACTION_TYPE_TURN_AROUND:
          print("ACTION: Turn around");
          turnAround();
          break;
        case ACTION_TYPE_IR_MONITOR:
          print("ACTION: IR monitoring");
          readAllSensorsCont = true;
          startIrMonitoring();
          break;
        case ACTION_TYPE_PARK:
          print("ACTION: Park");
          parkMotors();
          break;
        case ACTION_TYPE_REVERSE:
          print("ACTION: Reverse");
          collisionDetectionActive = false;
          reverse(action.nCells);
          break;
        case ACTION_TYPE_CHECK_WALLS:
          print("ACTION: Check All Walls");
          checkAllWalls();
          currentActionComplete = true;
          break;
        case ACTION_TYPE_START_WALLS_CHECK:
          print("ACTION: Start Check All Walls");
          readAllSensorsCont = true;
          currentActionComplete = true;
          break;
        case ACTION_TYPE_DELAY:
          print("ACTION: Delay");
          startActionDelay(action.timestamp);
          break;
        default:
          print("ERROR: Unsupported action type!");
          break;
      }
    }
  }

  // Hold program if error occurs
  if(fatalError)
  {
    parkMotors();
    print("INFO: Program Stopped");
    pulseLedDelay_ms = 100; // Pulse Arduino LED to show error
    while(1)
    {
      pulseLedLoop();
      if(checkButtonsForPress())
      {
        pulseLedDelay_ms = 0; // Stop pulsing LED
        digitalWrite(LED_BUILTIN, LOW);
        break;
      }
    }
  }
}

