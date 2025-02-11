#include "ActionBuffer.h"

CircularBuffer actionBuffer;
bool currentActionComplete = true;

// Initialize the circular buffer
void initBuffer(CircularBuffer &cb) 
{
  cb.head = 0;
  cb.tail = 0;
  cb.count = 0;
}

// Check if the buffer is full
bool isBufferFull(CircularBuffer &cb) 
{
  return cb.count == BUFFER_SIZE;
}

// Check if the buffer is empty
bool isBufferEmpty(CircularBuffer &cb) 
{
  return cb.count == 0;
}

// Add an action to the buffer
bool addAction(CircularBuffer &cb, Action action) 
{
  if (isBufferFull(cb)) 
  {
    // Buffer is full, cannot add new action
    return false;
  }
  cb.buffer[cb.head] = action;
  cb.head = (cb.head + 1) % BUFFER_SIZE;
  cb.count++;

  return true;
}

// Get an action from the buffer
bool getAction(CircularBuffer &cb, Action &action) 
{
  if (isBufferEmpty(cb)) 
  {
    // Buffer is empty, no action to get
    return false;
  }
  action = cb.buffer[cb.tail];
  cb.tail = (cb.tail + 1) % BUFFER_SIZE;
  cb.count--;

  return true;
}

// Clear all actions in the buffer
void clearBuffer(CircularBuffer &cb) 
{
  cb.head = 0;
  cb.tail = 0;
  cb.count = 0;
}

Action* seeNextAction(CircularBuffer &cb)
{
  if (!isBufferEmpty(cb) && !currentActionComplete)
  {
    return &cb.buffer[cb.tail + 1];
  } 
  else
  {
    return NULL;
  }
}

void replaceCurrentAction(CircularBuffer &cb, Action action)
{
  if (isBufferEmpty(cb)) 
  {
    addAction(cb, action);
  } 
  else if (!currentActionComplete) 
  {
    cb.buffer[cb.tail] = action;
  } 
  else 
  {
    addAction(cb, action);
  }
}

void insertAction(CircularBuffer &cb, Action action)
{
  if (isBufferFull(cb)) 
  {
    return;
  }

  // Move tail backward (wrapping around if needed)
  cb.tail = (cb.tail - 1 + BUFFER_SIZE) % BUFFER_SIZE;

  // Insert the new action at the new tail position
  cb.buffer[cb.tail] = action;
  
  // Increase count
  cb.count++;
}




// ------------ Add Action Functions --------------
void addBlindMoveForwardAction(float nCells, int speed)
{
  Action newAction;
  newAction.timestamp = millis();
  newAction.type = ACTION_TYPE_MOVE_BASIC_FORWARDS;
  newAction.nCells = nCells;
  newAction.motor1Speed = speed;
  newAction.motor2Speed = speed;
  addAction(actionBuffer, newAction);
}
void addMoveForwardAction(float nCells, int speed)
{
  Action newAction;
  newAction.timestamp = millis();
  newAction.type = ACTION_TYPE_MOVE_FORWARDS;
  newAction.nCells = nCells;
  newAction.motor1Speed = speed;
  newAction.motor2Speed = speed;
  addAction(actionBuffer, newAction);
}
void addTurnLeftAction(int speed)
{
  Action newAction;
  newAction.timestamp = millis();
  newAction.type = ACTION_TYPE_TURN_LEFT;
  newAction.motor1Speed = speed;
  newAction.motor2Speed = speed;
  addAction(actionBuffer, newAction);
}
void addTurnRightAction(int speed)
{
  Action newAction;
  newAction.timestamp = millis();
  newAction.type = ACTION_TYPE_TURN_RIGHT;
  newAction.motor1Speed = speed;
  newAction.motor2Speed = speed;
  addAction(actionBuffer, newAction);
}
void addTurnAroundAction(int speed)
{
  Action newAction;
  newAction.timestamp = millis();
  newAction.type = ACTION_TYPE_TURN_AROUND;
  newAction.motor1Speed = speed;
  newAction.motor2Speed = speed;
  addAction(actionBuffer, newAction);
}
void addParkAction()
{
  Action newAction;
  newAction.timestamp = millis();
  newAction.type = ACTION_TYPE_PARK;
  newAction.motor1Speed = 0;
  newAction.motor2Speed = 0;
  addAction(actionBuffer, newAction);
}
void addBlindReverseAction(float nCells, int speed)
{
  Action newAction;
  newAction.timestamp = millis();
  newAction.type = ACTION_TYPE_REVERSE;
  newAction.nCells = nCells;
  newAction.motor1Speed = speed;
  newAction.motor2Speed = speed;
  addAction(actionBuffer, newAction);
}
void addIrMonitoringAction(int speed)
{
  Action newAction;
  newAction.timestamp = millis();
  newAction.type = ACTION_TYPE_IR_MONITOR;
  newAction.motor1Speed = speed;
  newAction.motor2Speed = speed;
  addAction(actionBuffer, newAction);
}
void addCheckWallsAction()
{
  Action newAction;
  newAction.timestamp = millis();
  newAction.type = ACTION_TYPE_CHECK_WALLS;
  addAction(actionBuffer, newAction);
}
void addDelayAction(float delay_ms){
  Action newAction;
  newAction.timestamp = delay_ms;
  newAction.type = ACTION_TYPE_DELAY;
  newAction.motor1Speed = -1;
  newAction.motor2Speed = -1;
  addAction(actionBuffer, newAction);
}