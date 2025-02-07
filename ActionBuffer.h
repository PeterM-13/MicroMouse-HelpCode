#ifndef ACTION_BUFFER_H
#define ACTION_BUFFER_H

#include <Arduino.h>


// ---------- Vars -------------
const int BUFFER_SIZE = 60; // Define the size of the buffer
extern bool currentActionComplete;

// ----------- Types ---------------
// Enumeration for different action/move types
enum ActionType {
  ACTION_TYPE_MOVE_BASIC_FORWARDS, // move forwards without IR sensors
  ACTION_TYPE_MOVE_FORWARDS,       // Move forwards with IR sensors active
  ACTION_TYPE_TURN_RIGHT,         // Turns 90 clockwise
  ACTION_TYPE_TURN_LEFT,          // Turns 90 anti-clockwise
  ACTION_TYPE_TURN_AROUND,        // Turns 180 (which way is further from wall)
  ACTION_TYPE_PARK,               // Stop motors
  ACTION_TYPE_REVERSE,            // reverse
  ACTION_TYPE_CHECK_WALLS,        // Updates the globals wall variables with the latest IR readings
  ACTION_TYPE_START_WALLS_CHECK,  // Starts taking IR readings
  ACTION_TYPE_IR_MONITOR,         // Starts driving until IR readings are met
  ACTION_TYPE_DELAY,              // Used to simply pause movements
};

// Structure to represent an action/move
struct Action {
  int actionID;
  ActionType type;
  unsigned long timestamp;
  float nCells = 0;
  int motor1Speed = -1;
  int motor2Speed = -1;
  float angle = 0;
};

// Circular buffer structure
struct CircularBuffer {
  Action buffer[BUFFER_SIZE];
  int head;
  int tail;
  int count;
};
extern CircularBuffer actionBuffer;


// ------------ Functions --------------

// Initialize the circular buffer
void initBuffer(CircularBuffer &cb);

// Check if the buffer is full
bool isBufferFull(CircularBuffer &cb);

// Check if the buffer is empty
bool isBufferEmpty(CircularBuffer &cb);

// Empties the buffer
void clearBuffer(CircularBuffer &cb);

// Add an action to the buffer
bool addAction(CircularBuffer &cb, Action action);

// Get an action from the buffer
bool getAction(CircularBuffer &cb, Action &action);

Action* seeNextAction(CircularBuffer &cb);

// Replaces the current actoin with a new one
void replaceCurrentAction(CircularBuffer &cb, Action action);

// Inserts new action at begining of buffer
void insertAction(CircularBuffer &cb, Action action);

// Functions to add actions to buffer
void addBlindMoveForwardAction(float nCells, int speed);
void addMoveForwardAction(float nCells, int speed);
void addTurnLeftAction(int speed);
void addTurnRightAction(int speed);
void addTurnAroundAction(int seed);
void addParkAction();
void addBlindReverseAction(float nCells, int speed);
void addCheckWallsAction();
void addStartCheckingWallsAction();
void addIrMonitoringAction(int speed);
void addDelayAction(float delay_ms);


#endif // ACTION_BUFFER_H