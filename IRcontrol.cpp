#include "IRcontrol.h"

// IR Phototransistor Pins
const int IR_SENSOR_1_PIN = A0;
const int IR_SENSOR_2_PIN = A1;
const int IR_SENSOR_3_PIN = A2;
const int IR_SENSOR_4_PIN = A3;
const int IR_SENSORS[N_SENSORS] = {IR_SENSOR_1_PIN, IR_SENSOR_2_PIN, IR_SENSOR_3_PIN, IR_SENSOR_4_PIN};

// IR LED Pins
const int IR_LED_1_PIN = A7;
const int IR_LED_2_PIN = A6;
const int IR_LED_3_PIN = A5;
const int IR_LED_4_PIN = A4;
const int IR_LED_PINS[4] = {IR_LED_1_PIN, IR_LED_2_PIN, IR_LED_3_PIN, IR_LED_4_PIN};

int irReadings[N_SENSORS][3]; // 4 sensors // 0 = last reading, 1 = last ambient reading, 2 = time end
int currentSensor = 3; // The sensor currently being read from
bool prevIrReadEnded = true;
//bool readAllSensors = false;
bool checkedIrReadings = false;

// Max IR reading limits to detect a wall
const int IR_SENSOR_1_WALL_THRESHOLD = 400;
const int IR_SENSOR_2_WALL_THRESHOLD = 120;
const int IR_SENSOR_3_WALL_THRESHOLD = 120;
const int IR_SENSOR_4_WALL_THRESHOLD = 400;
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
  // if(readAllSensorsOnce || readAllSensorsCont)
  // {
    // Read sensors ALL the time.
    if(prevIrReadEnded) // If finished reading prev sensor
    {
      currentSensor ++;
      if(currentSensor >= N_SENSORS) // Reset count if above 3
      {
        currentSensor = 0;
      }
      prevIrReadEnded = false;
      startIrReading(currentSensor);
    }
    else  // Prev reading timer not ended yet
    {
      if(micros() >= irReadings[currentSensor][TIME])  // Check if timer should be up
      {
        stopIrReading(currentSensor); // Store value
        prevIrReadEnded = true;
        if(currentSensor == 3)
        {
          readAllSensorsOnce = false;
        }
      }
    }
  // }
  // else if(laneCenteringActive)
  // {
  //   // Read all sensors constantly if laneCentering, or next action is blindMoveForward.
  //   readAllSensorsOnce = true;
  // }
}

void checkAllWalls()
{
  wallFront = (irReadings[FRONT_LEFT_LED][0] < IR_SENSOR_1_WALL_THRESHOLD && irReadings[FRONT_RIGHT_LED][0] < IR_SENSOR_4_WALL_THRESHOLD);
  wallLeft = (irReadings[LEFT_LED][0] < IR_SENSOR_2_WALL_THRESHOLD); //|| (FL < IR_SENSOR_1_4_SIDE_WALL_THRESHOLD && FR > IR_SENSOR_1_4_SIDE_WALL_THRESHOLD);
  wallRight = (irReadings[RIGHT_LED][0] < IR_SENSOR_3_WALL_THRESHOLD);// || (FL > IR_SENSOR_1_4_SIDE_WALL_THRESHOLD && FR < IR_SENSOR_1_4_SIDE_WALL_THRESHOLD);
  //const int frontIRLimit = 300;
  //const int sideIRLimit = 550;
  //wallFrontClose = (irReadings[LEFT_LED][0] < sideIRLimit && irReadings[RIGHT_LED][0] < sideIRLimit && irReadings[FRONT_LEFT_LED][0] < frontIRLimit && irReadings[FRONT_RIGHT_LED][0] < frontIRLimit);
  //checkedIrReadings = true;
}



void startIrReading(const int led)
{
  const int ambientLight = analogRead(IR_SENSORS[led]); // Pre-read, for ambiant light
  irReadings[led][AMBIENT_VALUE] = IR_MAX_READING - ambientLight;
  digitalWrite(IR_LED_PINS[led], HIGH); // LED on
  irReadings[led][TIME] = micros() + IR_LED_ON_TIME_ms*1000;  // Time the LED should turn off
}

void stopIrReading(const int led)
{ 
  const int reading = analogRead(IR_SENSORS[led]); 
  irReadings[led][IR_VALUE] = reading + irReadings[led][AMBIENT_VALUE]; // Store value with ambient light added
  digitalWrite(IR_LED_PINS[led], LOW); // LED off
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
