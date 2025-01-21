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
const int CELL_DISTANCE = 560; 

// Constansts used for lane centering (LC) (Adjust with trail and error)
const int LC_OFF_AXES_THRESHOLD = 3; // How tight to the middle of the lane it stays. Lower = more sensitive.
const float LC_KP = 5.0; // Proportional Gain - how quickly it adjusts. Lower = more sesnsitive.
const signed int LC_LEFT_RIGHT_BIAS = -45;  // Corrects bias left or right of the lane - make negative for right movement

// Varriables keeping track of live positional data
int leftMotorDirection = parked;
int rightMotorDirection = parked;
int spinDirection = notSpinning;
long leftMotorSteps = 0;
long rightMotorSteps = 0;
int leftMotorSpeed = 0;
int rightMotorSpeed = 0;

// Variables to store values that dictate when certiain actions should stop
long leftMotorStepsEnd = 0;
long rightMotorStepsEnd = 0;
float gyroAngleEnd = 0.0;
bool laneCenteringActive = false;
long actionDelayEnd = 0.0;
bool actionDelayActive = false;

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
    if (spinDirection == notSpinning && collisionDetectionActive)
    {
      detectCollisionWithSteps();
    }
    

    if( (leftMotorSteps > leftMotorStepsEnd || rightMotorSteps > rightMotorStepsEnd) ) // Check if should stop motors
    {
      parkMotors(true);
    }
    // if( (spinDirection == clockwise && gyroAngle < gyroAngleEnd) || (spinDirection == antiClockwise && gyroAngle > gyroAngleEnd) ) // Check if should stop spinning
    // {
    //   parkMotors();
    // }
    isWallFrontClose();
    if(wallFrontClose)
    {
      collisionSolution();
    }

    if(laneCenteringActive)
    {
      laneCenter();
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

void rotateWithEncoders(float angle)
{
  int distanceToTravel = abs(angle) * 2.52;  // Convert angle to steps
  leftMotorStepsEnd = leftMotorSteps + distanceToTravel;
  rightMotorStepsEnd = rightMotorSteps + distanceToTravel;
  driveMotorsOpposite(angle > 0.0);
}

void rotateWithGyro(float angle)
{
  const float currentSpeed = ((leftMotorSpeed + rightMotorSpeed) / 2.0); // Used to scale angleOffset, as the faster you go the more error there is.
  const float angleOffset = 0.009 * currentSpeed; //0.0101
  gyroAngleEnd = gyroAngle - (angle * angleOffset);
  driveMotorsOpposite(angle > 0.0);
  print("INFO: Motors Rotating: " + String(angle));
}

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

void startActionDelay(float delay_ms){
  actionDelayEnd = delay_ms + millis();
  actionDelayActive = true;
}


// ---------- Other methods ----------
void laneCenter()
{
  resetMotorBias();
  // Only if there is a wall either side try IR lane centering
  if(wallLeft && wallRight)
  {
    int left = getIRreading(LEFT_LED,2) - LC_LEFT_RIGHT_BIAS;
    int right = getIRreading(RIGHT_LED,2) + LC_LEFT_RIGHT_BIAS;
    signed int diff = left - right;  // Positive when needs to move left
    unsigned int correction = abs(round(diff / LC_KP));

    if(diff > LC_OFF_AXES_THRESHOLD) // Too far right, move left
    {
      leftMotorBias += correction;
    }
    else if(diff < (-1)*LC_OFF_AXES_THRESHOLD)  // Too far left, move right
    {
      rightMotorBias += correction;
    }
    else
    {
      resetMotorBias();
    }
  }
  driveMotors();
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
      print("ERROR: Collision detected from steps!");
      fatalError = true;
      //collisionSolution(); // TODO: This is not working as exected yet
    }
    colDetcPrevStepCount = abs((leftMotorSteps + rightMotorSteps) / 2.0);
    colDetcPrevTime_ms = millis();
  }
}

