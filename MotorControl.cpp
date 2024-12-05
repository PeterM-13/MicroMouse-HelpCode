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
const int RIGHT_MOTOR_BIAS = 5; 
// Varying motor bias - used for lane centering
int leftMotorBias = LEFT_MOTOR_BIAS;
int rightMotorBias = RIGHT_MOTOR_BIAS;

// Number of encoder cycles to move one cell, found with trial and error currently
const int CELL_DISTANCE = 600; 

// Constansts used for lane centering (Adjust with trail and error)
const unsigned int OFF_AXES_THRESHOLD = 12; // How tight to the middle of the lane it stays
const signed int IR_SENSOR_LEFT_RIGHT_BIAS = 0;  // Corrects bias left or right of the lane - make negative for left bias

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
    if( (spinDirection == notSpinning) && (leftMotorSteps > leftMotorStepsEnd || rightMotorSteps > rightMotorStepsEnd) ) // Check if should stop motors
    {
      parkMotors();
    }

    if( (spinDirection == clockwise && gyroAngle < gyroAngleEnd) || (spinDirection == antiClockwise && gyroAngle > gyroAngleEnd) ) // Check if should stop spinning
    {
      parkMotors();
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


// ------------ Actions ---------------
void moveForward(float nCells)
{
  int distanceToTravel = CELL_DISTANCE * nCells;
  leftMotorStepsEnd = leftMotorSteps + distanceToTravel;
  rightMotorStepsEnd = rightMotorSteps + distanceToTravel;
  driveMotors();
  print("INFO: Motors Forward: " + String(nCells) + " squares");
}

void reverse(float nCells)
{
  int distanceToTravel = CELL_DISTANCE * nCells;
  leftMotorStepsEnd = leftMotorSteps + distanceToTravel;
  rightMotorStepsEnd = rightMotorSteps + distanceToTravel;
  reverseMotors();
  print("INFO: Motors Reverse: " + String(nCells) + " squares");
}

void parkMotors()
{
  leftMotorDirection = parked;
  analogWrite(LEFT_MOTOR_PIN_A, 0);
  analogWrite(LEFT_MOTOR_PIN_B, 0); 
  rightMotorDirection = parked;
  analogWrite(RIGHT_MOTOR_PIN_A, 0);
  analogWrite(RIGHT_MOTOR_PIN_B, 0); 
  spinDirection = notSpinning;
  laneCenteringActive = false;
  currentActionComplete = true;
  print("INFO: Motors Parked");
}

void rotate(float angle)
{
  const float currentSpeed = ((leftMotorSpeed + rightMotorSpeed) / 2.0); // Used to scale angleOffset, as the faster you go the more error there is.
  const float angleOffset = 0.0101 * currentSpeed;
  gyroAngleEnd = gyroAngle - (angle * angleOffset);
  if(angle > 0.0) // Clockwise
  {
    spinDirection = clockwise;
    analogWrite(LEFT_MOTOR_PIN_A, leftMotorSpeed - rightMotorBias); 
    analogWrite(LEFT_MOTOR_PIN_B, 0);
    leftMotorDirection = driving;
    analogWrite(RIGHT_MOTOR_PIN_A, rightMotorSpeed - leftMotorBias);
    analogWrite(RIGHT_MOTOR_PIN_B, 0); 
    rightMotorDirection = reversing;
  }
  else if(angle < 0.0) // Anti-Clockwise
  {
    spinDirection = antiClockwise;
    analogWrite(LEFT_MOTOR_PIN_A, 0);
    analogWrite(LEFT_MOTOR_PIN_B, leftMotorSpeed - rightMotorBias); 
    leftMotorDirection = reversing;
    analogWrite(RIGHT_MOTOR_PIN_A, 0); 
    analogWrite(RIGHT_MOTOR_PIN_B, rightMotorSpeed - leftMotorBias);
    rightMotorDirection = driving;
  }
  print("INFO: Motors Rotating: " + String(angle));
}

void startActionDelay(float delay_ms){
  actionDelayEnd = delay_ms + millis();
  actionDelayActive = true;
}


// ---------- Other methods ----------
void laneCenter()
{
  resetMotorBias();
  checkAllWalls();
  // Only if there is a wall either side try IR lane centering
  if(wallLeft && wallRight)
  {
    const float kp = (leftMotorSpeed + rightMotorSpeed) / 800.0; // Proportional gain, from average motor speed
    int left = getIRreading(LEFT_LED, 3) - IR_SENSOR_LEFT_RIGHT_BIAS;
    int right = getIRreading(RIGHT_LED, 3) + IR_SENSOR_LEFT_RIGHT_BIAS;
    int diff = left - right;
    int correction = round(kp * diff);
    if (abs(diff) > OFF_AXES_THRESHOLD) 
    {
      leftMotorBias -= correction;
      rightMotorBias += correction;
    }
    else
    {
      resetMotorBias();
    }
  }
  else
  {
    resetMotorBias();
  }
  driveMotors();
}

