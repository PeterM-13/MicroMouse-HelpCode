#include "MotorControl.h"

// Motor Control Pins
const int LEFT_MOTOR_PIN_A = 2;
const int LEFT_MOTOR_PIN_B = 3;
const int RIGHT_MOTOR_PIN_A = 5;
const int RIGHT_MOTOR_PIN_B = 4;

// Encoder Phase-A Pins
const int LEFT_ENCODER_PIN = 6;
const int RIGHT_ENCODER_PIN = 7;

// Default motor Bias (found with trial and error currenly)
const int LEFT_MOTOR_BIAS = 0;
const int RIGHT_MOTOR_BIAS = 3; 
// Varying motor bias - used for lane centering
int leftMotorBias = LEFT_MOTOR_BIAS;
int rightMotorBias = RIGHT_MOTOR_BIAS;

// Number of encoder cycles to move one cell, found with trial and error currently
const int CELL_DISTANCE = 565; 

// Constansts used for lane centering (LC) (Adjust with trail and error)
const int LC_OFF_AXES_THRESHOLD = 2; // How tight to the middle of the lane it stays. Lower = more sensitive.
//const float LC_KP = 2.0; // Proportional Gain - how quickly it adjusts. Lower = more sesnsitive.
const signed int LC_LEFT_RIGHT_BIAS = -4;  // Corrects bias left or right of the lane - make negative for left movement

// Varriables keeping track of live positional data
int leftMotorDirection = parked;
int rightMotorDirection = parked;
int spinDirection = notSpinning;
int leftMotorSteps = 0;
int rightMotorSteps = 0;
int leftMotorSpeed = 0;
int rightMotorSpeed = 0;

// Variables to store values that dictate when certiain actions should stop
int leftMotorStepsEnd = 0;
int rightMotorStepsEnd = 0;
float gyroAngleEnd = 0.0;
long actionDelayEnd = 0.0;
bool actionDelayActive = false;
bool irMonitoringActive = false;
int irMonitoringEnd = 48;

// Variables used for collision detection
long colDetcPrevTime_ms = 0; // Last time collisions were detected
int colDetcPrevStepCount = 0; // Last time step count was checked
const int COL_DETC_TIME_GAP_ms = 500;   // Check for collision every 500ms
const int COL_DETC_STEP_THRESHOLD= 50; // Check for min 50 steps every 500ms


void setupMotors()
{
  // Motor pin setup
  pinMode(LEFT_MOTOR_PIN_A, OUTPUT);
  pinMode(LEFT_MOTOR_PIN_B, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN_A, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN_B, OUTPUT);

  // Encoder pin setup
  pinMode(LEFT_ENCODER_PIN, INPUT);
  pinMode(RIGHT_ENCODER_PIN, INPUT);

  // Interrupts
  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN), updateLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN), updateRightEncoder, RISING);

  // Set default state
  parkMotors();
}


void loopMotors()
{
  if(leftMotorDirection != parked || rightMotorDirection != parked) // If moving
  { 
    if (spinDirection == notSpinning)
    {
      detectCollisionWithSteps();
    }
    

    
    if(irMonitoringActive)
    {
      if(irMonitoringEnd > irReadings[FRONT_LEFT_LED][IR_VALUE] || irMonitoringEnd > irReadings[FRONT_RIGHT_LED][IR_VALUE])
      {
        print("INFO: IR monitoring end reached");
        parkMotors(true);
      }
    }
    else
    {
      if( (leftMotorSteps > leftMotorStepsEnd || rightMotorSteps > rightMotorStepsEnd) ) // Check if should stop motors
      {
        print("INFO: Steps end reached");
        parkMotors(true);
      }
    }

    // No longer using the gyro - too inaccurate, had +/- 10 deg
    // if( (spinDirection == clockwise && gyroAngle < gyroAngleEnd) || (spinDirection == antiClockwise && gyroAngle > gyroAngleEnd) ) // Check if should stop spinning
    // {
    //   parkMotors();
    // }
    //isWallFrontClose();
    // if(wallFrontClose)
    // {
    //   collisionSolution();
    // }

    if(laneCenteringActive)
    {
      laneCenter();
      //laneCenterPID();
    }
  }

  if(actionDelayActive && actionDelayEnd < millis()) // Check if timer/delay is complete
  {
    currentActionComplete = true;
    actionDelayActive = false;
  }
}


// ----------- Update Functions --------------
void updateLeftEncoder() 
{
  leftMotorSteps ++;
}
void updateRightEncoder() 
{
  rightMotorSteps ++;
}
void resetMotorBias()
{
  leftMotorBias = LEFT_MOTOR_BIAS;
  rightMotorBias = RIGHT_MOTOR_BIAS;
}
void setSpeed(int motor1, int motor2)
{
  leftMotorSpeed = motor1 - rightMotorBias;
  rightMotorSpeed = motor2 - leftMotorBias;
}
void driveMotors()
{
  analogWrite(LEFT_MOTOR_PIN_A, leftMotorSpeed - rightMotorBias); 
  analogWrite(LEFT_MOTOR_PIN_B, 0);
  leftMotorDirection = driving;
  analogWrite(RIGHT_MOTOR_PIN_A, 0); 
  analogWrite(RIGHT_MOTOR_PIN_B, rightMotorSpeed - leftMotorBias);
  rightMotorDirection = driving;
}
void reverseMotors()
{
  analogWrite(LEFT_MOTOR_PIN_A, 0);
  analogWrite(LEFT_MOTOR_PIN_B, leftMotorSpeed - rightMotorBias); 
  leftMotorDirection = reversing;
  analogWrite(RIGHT_MOTOR_PIN_A, rightMotorSpeed - leftMotorBias);
  analogWrite(RIGHT_MOTOR_PIN_B, 0); 
  rightMotorDirection = reversing;
}
void driveMotorsOpposite(bool clockwise)
{
  if(clockwise == true) // Clockwise
  {
    spinDirection = clockwise;
    analogWrite(LEFT_MOTOR_PIN_A, leftMotorSpeed - rightMotorBias); 
    analogWrite(LEFT_MOTOR_PIN_B, 0);
    leftMotorDirection = driving;
    analogWrite(RIGHT_MOTOR_PIN_A, rightMotorSpeed - leftMotorBias);
    analogWrite(RIGHT_MOTOR_PIN_B, 0); 
    rightMotorDirection = reversing;
  }
  else // Anti-Clockwise
  {
    spinDirection = antiClockwise;
    analogWrite(LEFT_MOTOR_PIN_A, 0);
    analogWrite(LEFT_MOTOR_PIN_B, leftMotorSpeed - rightMotorBias); 
    leftMotorDirection = reversing;
    analogWrite(RIGHT_MOTOR_PIN_A, 0); 
    analogWrite(RIGHT_MOTOR_PIN_B, rightMotorSpeed - leftMotorBias);
    rightMotorDirection = driving;
  }
}


// ------------ Actions ---------------
void moveForward(float nCells)
{
  int distanceToTravel = CELL_DISTANCE * nCells;
  leftMotorStepsEnd = leftMotorSteps + distanceToTravel;
  rightMotorStepsEnd = rightMotorSteps + distanceToTravel;
  resetCollisionDetection();
  driveMotors();
  print("INFO: Motors Forward: " + String(nCells) + " squares");
}
void reverse(float nCells)
{
  int distanceToTravel = CELL_DISTANCE * nCells;
  leftMotorStepsEnd = leftMotorSteps + distanceToTravel;
  rightMotorStepsEnd = rightMotorSteps + distanceToTravel;
  resetCollisionDetection();
  reverseMotors();
  print("INFO: Motors Reverse: " + String(nCells) + " squares");
}

void parkMotors(bool withBrake)
{
  leftMotorDirection = parked;
  analogWrite(LEFT_MOTOR_PIN_A, (int)withBrake);
  analogWrite(LEFT_MOTOR_PIN_B, (int)withBrake); 
  rightMotorDirection = parked;
  analogWrite(RIGHT_MOTOR_PIN_A, (int)withBrake);
  analogWrite(RIGHT_MOTOR_PIN_B, (int)withBrake); 
  spinDirection = notSpinning;
  laneCenteringActive = false;
  currentActionComplete = true;
  irMonitoringActive = false;
  readAllSensorsCont = false;
  updateGyroData = false;
  collisionDetectionActive = true;
  leftMotorSteps = 0; // Reset to 0 incase of overflow
  rightMotorSteps = 0;
  resetMotorBias();
  print("INFO: Motors Parked");
}

void rotate(float angle)
{
  rotateWithEncoders(angle);
  //rotateWithGyro(angle);
}
void turnRight()
{
  int distanceToTravel = 90 * 2.42;  // Convert angle to steps
  leftMotorStepsEnd = leftMotorSteps + distanceToTravel;
  rightMotorStepsEnd = rightMotorSteps + distanceToTravel;
  driveMotorsOpposite(true);
}
void turnLeft()
{
  int distanceToTravel = 90 * 2.52;  // Convert angle to steps
  leftMotorStepsEnd = leftMotorSteps + distanceToTravel;
  rightMotorStepsEnd = rightMotorSteps + distanceToTravel;
  driveMotorsOpposite(false);
}
void turnAround()
{
  if(irReadings[LEFT_LED][IR_VALUE] < irReadings[RIGHT_LED][IR_VALUE]) // Closer to left wall, so turn right (clockwise)
  {
    int distanceToTravel = 180 * 2.42;  // Convert angle to steps
    leftMotorStepsEnd = leftMotorSteps + distanceToTravel;
    rightMotorStepsEnd = rightMotorSteps + distanceToTravel;
    driveMotorsOpposite(true);
  }
  else
  {
    int distanceToTravel = 180 * 2.42;  // Convert angle to steps
    leftMotorStepsEnd = leftMotorSteps + distanceToTravel;
    rightMotorStepsEnd = rightMotorSteps + distanceToTravel;
    driveMotorsOpposite(false);
  }
}

void rotateWithEncoders(float angle)
{
  int distanceToTravel = abs(angle) * 2.52;  // Convert angle to steps
  leftMotorStepsEnd = leftMotorSteps + distanceToTravel;
  rightMotorStepsEnd = rightMotorSteps + distanceToTravel;
  driveMotorsOpposite(angle > 0.0);
}

void rotateWithGyro(float angle)
{
  updateGyroData = true;
  const float currentSpeed = ((leftMotorSpeed + rightMotorSpeed) / 2.0); // Used to scale angleOffset, as the faster you go the more error there is.
  const float angleOffset = 0.009 * currentSpeed; //0.0101
  gyroAngleEnd = gyroAngle - (angle * angleOffset);
  driveMotorsOpposite(angle > 0.0);
  print("INFO: Motors Rotating: " + String(angle));
}

void startIrMonitoring()
{
  irMonitoringActive = true;
  driveMotors();
  print("INFO: IR monitoring started.");
}

void startActionDelay(float delay_ms){
  actionDelayEnd = delay_ms + millis();
  actionDelayActive = true;
}


// ---------- Other methods ----------
void collisionSolution()
{
    // Reverse a small amount and skip current action in buffer
    Action newAction;
    newAction.timestamp = millis();
    newAction.type = ACTION_TYPE_REVERSE;
    newAction.nCells = 0.15;
    newAction.motor1Speed = 60;
    newAction.motor2Speed = 60;
    replaceCurrentAction(actionBuffer, newAction);
}

void resetCollisionDetection()
{
  colDetcPrevStepCount = abs(leftMotorSteps + rightMotorSteps) / 2.0; 
  colDetcPrevTime_ms = millis();
}

// Collision Detection using step count
void detectCollisionWithSteps()
{
  if(millis() > (colDetcPrevTime_ms + COL_DETC_TIME_GAP_ms))
  {
    if(abs((leftMotorSteps + rightMotorSteps) / 2.0) < (colDetcPrevStepCount + COL_DETC_STEP_THRESHOLD))
    {
      // Collision Detected!
      if(collisionDetectionActive)
      {
        print("ERROR: Collision detected from steps!");
        fatalError = true;
        //collisionSolution(); // TODO: This is not working as exected yet
      }
      else // Reversing into wall to re-allign
      {
        delay(400);
        parkMotors();
      }
    }
    colDetcPrevStepCount = abs((leftMotorSteps + rightMotorSteps) / 2.0);
    colDetcPrevTime_ms = millis();
  }
}


void laneCenter()
{
  resetMotorBias();
  const int leftWall = (irReadings[LEFT_LED][0] < 280); 
  const int rightWall = (irReadings[RIGHT_LED][0] < 280);
  // Only if there is a wall either side try IR lane centering
  if (leftWall && rightWall)
  {
    const int left = irReadings[LEFT_LED][IR_VALUE] - LC_LEFT_RIGHT_BIAS;
    const int right = irReadings[RIGHT_LED][IR_VALUE] + LC_LEFT_RIGHT_BIAS;
    const signed int diff = left - right;  // Positive when needs to move left
    const signed int correction = abs(round(diff));  // LC_KP

    if(diff > LC_OFF_AXES_THRESHOLD) // Too far right, move left
    {
      leftMotorBias -= correction;
      rightMotorBias += correction;
    }
    else if(diff < (-1)*LC_OFF_AXES_THRESHOLD)  // Too far left, move right
    {
      leftMotorBias += correction;
      rightMotorBias -= correction;
    }
    else
    {
      resetMotorBias();
    }
  }
  driveMotors();
}
