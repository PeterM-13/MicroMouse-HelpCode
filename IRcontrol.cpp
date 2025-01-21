#include "IRcontrol.h"

// IR Phototransistor Pins
const int IR_SENSOR_1_PIN = A0;
const int IR_SENSOR_2_PIN = A1;
const int IR_SENSOR_3_PIN = A2;
const int IR_SENSOR_4_PIN = A3;

// IR LED Pins
const int IR_LED_1_PIN = A7;
const int IR_LED_2_PIN = A6;
const int IR_LED_3_PIN = A5;
const int IR_LED_4_PIN = A4;

// Max IR reading limits to detect a wall
const int IR_SENSOR_1_WALL_THRESHOLD = 500;
const int IR_SENSOR_2_WALL_THRESHOLD = 500;
const int IR_SENSOR_3_WALL_THRESHOLD = 500;
const int IR_SENSOR_4_WALL_THRESHOLD = 500;
const int IR_SENSOR_1_4_SIDE_WALL_THRESHOLD = 500;

const int IR_MAX_READING = 1024;
const int IR_LIMIT = IR_MAX_READING * 0.5;
const int IR_LED_ON_TIME_ms = 3;  // The time the LEDs are on to take a reading


void setupLEDs()
{
    // ------------------- Pin Setup  -------------------
    // LED Steup
    pinMode(IR_LED_1_PIN, OUTPUT);
    pinMode(IR_LED_2_PIN, OUTPUT);
    pinMode(IR_LED_3_PIN, OUTPUT);
    pinMode(IR_LED_4_PIN, OUTPUT);

    // IR Phototransistor Setup
    pinMode(IR_SENSOR_1_PIN, INPUT);
    pinMode(IR_SENSOR_2_PIN, INPUT);
    pinMode(IR_SENSOR_3_PIN, INPUT);
    pinMode(IR_SENSOR_4_PIN, INPUT);

    // ------------------- Set OUTPUT Initial States -------------------
    digitalWrite(IR_LED_1_PIN, LOW);
    digitalWrite(IR_LED_2_PIN, LOW);
    digitalWrite(IR_LED_3_PIN, LOW);
    digitalWrite(IR_LED_4_PIN, LOW);
}

void loopLEDs()
{
  ;
}


void isWallFront()
{
  // Check the 2 Forward facing IR sensors
  wallFront = (getIRreading(FRONT_LEFT_LED) < IR_SENSOR_1_WALL_THRESHOLD && getIRreading(FRONT_RIGHT_LED) < IR_SENSOR_4_WALL_THRESHOLD);
}
void isWallLeft()
{
  // Check the left facing sensor OR check the left forward sensor incase mouse askew
  wallLeft = (getIRreading(LEFT_LED) < IR_SENSOR_2_WALL_THRESHOLD) || (getIRreading(FRONT_LEFT_LED) < IR_SENSOR_1_4_SIDE_WALL_THRESHOLD && getIRreading(FRONT_RIGHT_LED) > IR_SENSOR_1_4_SIDE_WALL_THRESHOLD);
}
void isWallRight()
{
  // Check the right facing sensor OR check the right forward sensor incase mouse askew
  wallRight = (getIRreading(RIGHT_LED) < IR_SENSOR_3_WALL_THRESHOLD) || (getIRreading(FRONT_LEFT_LED) > IR_SENSOR_1_4_SIDE_WALL_THRESHOLD && getIRreading(FRONT_RIGHT_LED) < IR_SENSOR_1_4_SIDE_WALL_THRESHOLD);
}
void isWallFrontClose()
{
  const int frontIRLimit = 300;
  const int sideIRLimit = 700;
  wallFrontClose = (getIRreading(RIGHT_LED) < sideIRLimit && getIRreading(LEFT_LED) < sideIRLimit && getIRreading(FRONT_LEFT_LED) < frontIRLimit && getIRreading(FRONT_LEFT_LED) < frontIRLimit);
}
void checkAllWalls()
{
  // isWallFront();
  // isWallRight();
  // isWallLeft();
  // isWallFrontClose();
  const int FL = getIRreading(FRONT_LEFT_LED);
  const int FR = getIRreading(FRONT_RIGHT_LED);
  const int L = getIRreading(LEFT_LED);
  const int R = getIRreading(RIGHT_LED);
  wallFront = (FL < IR_SENSOR_1_WALL_THRESHOLD && FR < IR_SENSOR_4_WALL_THRESHOLD);
  wallLeft = (L < IR_SENSOR_2_WALL_THRESHOLD); //|| (FL < IR_SENSOR_1_4_SIDE_WALL_THRESHOLD && FR > IR_SENSOR_1_4_SIDE_WALL_THRESHOLD);
  wallRight = (R < IR_SENSOR_3_WALL_THRESHOLD);// || (FL > IR_SENSOR_1_4_SIDE_WALL_THRESHOLD && FR < IR_SENSOR_1_4_SIDE_WALL_THRESHOLD);
  const int frontIRLimit = 300;
  const int sideIRLimit = 550;
  wallFrontClose = (L < sideIRLimit && R < sideIRLimit && FL < frontIRLimit && FR < frontIRLimit);
}

int getIRreading(const int led, const int nReadings)
{
  int readingTotal = 1;
  int avgReading;
  int preReading;
  int reading;
  for(int i=0; i < nReadings; i++)
  {
    switch(led)
    {
      case FRONT_LEFT_LED:
        preReading = analogRead(IR_SENSOR_1_PIN);
        digitalWrite(IR_LED_1_PIN, HIGH);
        delay(IR_LED_ON_TIME_ms);
        reading = analogRead(IR_SENSOR_1_PIN);
        digitalWrite(IR_LED_1_PIN, LOW);
        break;
      case LEFT_LED:
        preReading = analogRead(IR_SENSOR_2_PIN);
        digitalWrite(IR_LED_2_PIN, HIGH);
        delay(IR_LED_ON_TIME_ms);
        reading = analogRead(IR_SENSOR_2_PIN);
        digitalWrite(IR_LED_2_PIN, LOW);
        break;
      case RIGHT_LED:
        preReading = analogRead(IR_SENSOR_3_PIN);
        digitalWrite(IR_LED_3_PIN, HIGH);
        delay(IR_LED_ON_TIME_ms);
        reading = analogRead(IR_SENSOR_3_PIN);
        digitalWrite(IR_LED_3_PIN, LOW);
        break;
      case FRONT_RIGHT_LED:
        preReading = analogRead(IR_SENSOR_4_PIN);
        digitalWrite(IR_LED_4_PIN, HIGH);
        delay(IR_LED_ON_TIME_ms);
        reading = analogRead(IR_SENSOR_4_PIN);
        digitalWrite(IR_LED_4_PIN, LOW);
        break;
    }
    // If too much IR when inactive, can't trust reading when active.
    // if(preReading < IR_LIMIT)
    // {
    //   return -1;
    // }
    int ambientLight = IR_MAX_READING - preReading;

    readingTotal += (reading + ambientLight);
  }
  avgReading = round(readingTotal / nReadings);
  return avgReading;
}


// Turns the selected LED ON / OFF for 1 second
void testLed(const int led)
{
  switch(led)
  {
    case FRONT_LEFT_LED:
      digitalWrite(IR_LED_1_PIN, HIGH);
      delay(1000);
      digitalWrite(IR_LED_1_PIN, LOW);
      break;
    case LEFT_LED:
      digitalWrite(IR_LED_2_PIN, HIGH);
      delay(1000);
      digitalWrite(IR_LED_2_PIN, LOW);
      break;
    case RIGHT_LED:
      digitalWrite(IR_LED_3_PIN, HIGH);
      delay(1000);
      digitalWrite(IR_LED_3_PIN, LOW);
      break;
    case FRONT_RIGHT_LED:
      digitalWrite(IR_LED_4_PIN, HIGH);
      delay(1000);
      digitalWrite(IR_LED_4_PIN, LOW);
      break;
  }
  delay(1000);
}
