#include "OtherIO.h"

// Push Button vars
const int PUSH_BUTTON_1_PIN = 8;
bool pushButton1State = false;
bool pushButton1PrevState = false;
const int PUSH_BUTTON_2_PIN = 9;
bool pushButton2State = false;
bool pushButton2PrevState = false;

// Pulsing Arduino LED vars
bool pulseLedState = false;
unsigned int pulseLedDelay_ms = 0;
long prevPulseLedTime = 0;

// Spare pins
const int SPARE_PIN_1 = 10;
const int SPARE_PIN_2 = 12;


void setupOtherIO()
{
  // Pin Setup
  pinMode(PUSH_BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(PUSH_BUTTON_2_PIN, INPUT_PULLUP);

  //  Set Interrupt(s) 
  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_1_PIN), pushButton1Pressed, RISING);
  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_2_PIN), pushButton2Pressed, RISING);
}

void loopOtherIO()
{
  checkButtonsForPress();

  pulseLedLoop();
}


void pushButton1Pressed()
{
  if(pushButton1State == pushButton1PrevState)
  {
    pushButton1State = !pushButton1State;
  }
}
void pushButton2Pressed()
{
  if(pushButton2State == pushButton2PrevState)
  {
    pushButton2State = !pushButton2State;
  }
}


bool checkButtonsForPress()
{
  if(pushButton1State != pushButton1PrevState)
  {
    print("EVENT: Button 1 Pressed");
    ledOn();
    delay(150); // Allow button bounce
    ledOff();
    pushButton1PrevState = pushButton1State;
    
    // Start Algorithm
    fatalError = false;
    collisionDetectionActive = false;
    startAlgorithm = true;
    startAlgorithm2 = false;
    clearBuffer(actionBuffer);
    setupAlgorithm();
    return true;
  }

  if(pushButton2State != pushButton2PrevState)
  {
    print("EVENT: Button 2 Pressed");
    ledOn();
    delay(150); // Allow button bounce
    ledOff();
    pushButton2PrevState = pushButton2State;
    
    // TODO: Perform some action
    fatalError = false;
    collisionDetectionActive = false;
    startAlgorithm2 = true;
    startAlgorithm = false;
    clearBuffer(actionBuffer);
    setupAlgorithm();
    return true;
  }
  return false;
}


void pulseLedLoop()
{
  long currentPulseLedTime = millis();
  if (pulseLedDelay_ms > 0 && currentPulseLedTime > prevPulseLedTime + pulseLedDelay_ms) 
  {
    digitalWrite(LED_BUILTIN, pulseLedState);
    pulseLedState = !pulseLedState;
    prevPulseLedTime = currentPulseLedTime;
  }
}
