#include "MotorControl.h"

// Motor Control Pins
const int LEFT_MOTOR_PIN_A = 2;
const int LEFT_MOTOR_PIN_B = 3;
const int RIGHT_MOTOR_PIN_A = 5;
const int RIGHT_MOTOR_PIN_B = 4;

// Encoder Phase-A Pins
const int LEFT_ENCODER_PIN = 6;
const int RIGHT_ENCODER_PIN = 7;

// Varying motor bias - used for lane centering
int leftMotorBias = LEFT_MOTOR_BIAS;
int rightMotorBias = RIGHT_MOTOR_BIAS;

// Varriables keeping track of live positional data
int leftMotorDirection = parked;
int rightMotorDirection = parked;
int spinDirection = notSpinning;
int leftMotorSteps = 0;
int rightMotorSteps = 0;
int prevAvgMotorSteps = 0;
int leftMotorSpeed = 0;
int rightMotorSpeed = 0;
int stepsSincelastWallGap = 0;

// Variables to store values that dictate when certiain actions should stop
int leftMotorStepsEnd = 0;
int rightMotorStepsEnd = 0;
float gyroAngleEnd = 0.0;
long actionDelayEnd = 0.0;
bool actionDelayActive = false;
bool irMonitoringActive = false;
//int irMonitoringEnd = 47;

// Variables used for collision detection
long colDetcPrevTime_ms = 0; // Last time collisions were detected
int colDetcPrevStepCount = 0; // Last time step count was checked


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
    // If moving check IR sensors
    irSensorsActive = true;

    if (spinDirection == notSpinning)
    {
      detectCollisionWithSteps();
    }
    
    if(irMonitoringActive)
    {
      if(IR_MONITORING_THRESHOLD > irReadings[FRONT_LEFT_LED][IR_VALUE] || IR_MONITORING_THRESHOLD > irReadings[FRONT_RIGHT_LED][IR_VALUE])
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
      else if(spinDirection == notSpinning)
      {
        // Monitor last wall opening
        if(irReadings[LEFT_LED][IR_VALUE] > IR_SENSOR_2_WALL_THRESHOLD/2.0 ||
          irReadings[RIGHT_LED][IR_VALUE] > IR_SENSOR_3_WALL_THRESHOLD/2.0) // No wall left or no wall right
        {
          int avgMotorSteps = round((leftMotorSteps + rightMotorSteps) / 2.0);
          int diffAvgMotorSteps = avgMotorSteps - prevAvgMotorSteps;
          if(abs(diffAvgMotorSteps) < 10)
          {
            stepsSincelastWallGap += diffAvgMotorSteps;
          }
          prevAvgMotorSteps = avgMotorSteps;
        }
        else if(stepsSincelastWallGap > 0) // There is wall left & right again
        {
          stepsSincelastWallGap = 0;
          prevAvgMotorSteps = 0;
        }
      }
    }

    // No longer using the gyro - too inaccurate, had +/- 10 deg
    // if( (spinDirection == clockwise && gyroAngle < gyroAngleEnd) || (spinDirection == antiClockwise && gyroAngle > gyroAngleEnd) ) // Check if should stop spinning
    // {
    //   parkMotors();
    // }

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
  resetCollisionDetection();
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
void resetVariables()
{
  spinDirection = notSpinning;
  laneCenteringActive = false;
  currentActionComplete = true;
  irMonitoringActive = false;
  updateGyroData = false;
  collisionDetectionActive = true;
  prevAvgMotorSteps = 0;
  leftMotorSteps = 0; // Reset to 0 incase of overflow
  rightMotorSteps = 0;
  irSensorsActive = false; // If not moving, no need to check IR sensors
  resetMotorBias();
  print("INFO: Variables reset");
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
  print("INFO: Motors parked");
  resetVariables();
}
void turnRight()
{
  // If diff positive, left side greater, so left side further, so turn more to go right
  int diff = 0;
  if(wallFront)
  {
    diff = ((irReadings[FRONT_LEFT_LED][IR_VALUE]+FRONT_IR_VALUE_DIFF) - (irReadings[FRONT_RIGHT_LED][IR_VALUE]-FRONT_IR_VALUE_DIFF)) * TURNING_DIFF_SCALE;
  }
  const int distanceToTravel = (90+diff) * TURN_RIGHT_DEG_TO_STEPS_MULTIPLIER;  // Convert angle to steps
  leftMotorStepsEnd = leftMotorSteps + distanceToTravel;
  rightMotorStepsEnd = rightMotorSteps + distanceToTravel;
  driveMotorsOpposite(true);
}
void turnLeft()
{
  // If diff positive, right side greater, so right side further, so turn more to go left
  int diff = 0;
  if(wallFront)
  {
    diff = ((irReadings[FRONT_RIGHT_LED][IR_VALUE]-FRONT_IR_VALUE_DIFF) - (irReadings[FRONT_LEFT_LED][IR_VALUE]+FRONT_IR_VALUE_DIFF)) * TURNING_DIFF_SCALE;
  }
  const int distanceToTravel = (90+diff) * TURN_LEFT_DEG_TO_STEPS_MULTIPLIER;  // Convert angle to steps
  leftMotorStepsEnd = leftMotorSteps + distanceToTravel;
  rightMotorStepsEnd = rightMotorSteps + distanceToTravel;
  driveMotorsOpposite(false);
}
void turnAround()
{
  if(irReadings[LEFT_LED][IR_VALUE] < irReadings[RIGHT_LED][IR_VALUE]) // Closer to left wall, so turn right (clockwise)
  {
    const int distanceToTravel = 180 * 2.42;  // Convert angle to steps
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

// -- Unused funcs for turning to any angle --
// void rotateWithEncoders(float angle)
// {
//   int distanceToTravel = abs(angle) * 2.52;  // Convert angle to steps
//   leftMotorStepsEnd = leftMotorSteps + distanceToTravel;
//   rightMotorStepsEnd = rightMotorSteps + distanceToTravel;
//   driveMotorsOpposite(angle > 0.0);
// }
// void rotateWithGyro(float angle)
// {
//   updateGyroData = true;
//   const float currentSpeed = ((leftMotorSpeed + rightMotorSpeed) / 2.0); // Used to scale angleOffset, as the faster you go the more error there is.
//   const float angleOffset = 0.009 * currentSpeed; //0.0101
//   gyroAngleEnd = gyroAngle - (angle * angleOffset);
//   driveMotorsOpposite(angle > 0.0);
//   print("INFO: Motors Rotating: " + String(angle));
// }

void startIrMonitoring()
{
  irMonitoringActive = true;
  resetCollisionDetection();
  driveMotors();
  print("INFO: IR monitoring started.");
}

void startActionDelay(float delay_ms){
  actionDelayEnd = delay_ms + millis();
  actionDelayActive = true;
}


// ---------- Other methods ----------
void resetCollisionDetection()
{
  colDetcPrevStepCount = round(leftMotorSteps + rightMotorSteps) / 2.0; 
  colDetcPrevTime_ms = millis();
}

// Collision Detection using step count
void detectCollisionWithSteps()
{
  if(millis() > (colDetcPrevTime_ms + COL_DETC_TIME_GAP_ms))
  {
    if(round((leftMotorSteps + rightMotorSteps) / 2.0) < (colDetcPrevStepCount + COL_DETC_STEP_THRESHOLD))
    {
      // Collision Detected!
      if(collisionDetectionActive)
      {
        print("ERROR: Collision detected from steps!");
        fatalError = true;
      }
      else // Reversing into wall to re-allign
      {
        delay(200);
        parkMotors();
      }
    }
    colDetcPrevStepCount = round((leftMotorSteps + rightMotorSteps) / 2.0);
    colDetcPrevTime_ms = millis();
  }
}

void laneCenter()
{
  resetMotorBias();
  const int leftWall = (irReadings[LEFT_LED][0] < 250); 
  const int rightWall = (irReadings[RIGHT_LED][0] < 250);
  const int left = irReadings[LEFT_LED][IR_VALUE] - LC_LEFT_RIGHT_BIAS;
  const int right = irReadings[RIGHT_LED][IR_VALUE] + LC_LEFT_RIGHT_BIAS;
  signed int diff;
  // Only if there is a wall either side try standard IR lane centering
  if (leftWall && rightWall)
  {
    diff = left - right;  // Positive when needs to move left  
  }
  else if(leftWall)
  {
    diff = left - LC_LEFT_IR_VALUE;
  }
  else if(rightWall)
  {
    diff = LC_RIGHT_IR_VALUE - right;
  }
  else
  {
    resetMotorBias();
    driveMotors();
    return;
  }
  signed int correction = abs(round(diff));
  correction = min(abs(correction), LC_MAX_CORRECTION) * (correction/correction);
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
  driveMotors();
}
