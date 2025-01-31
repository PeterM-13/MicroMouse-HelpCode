<h1 style="text-decoration: none;">Leonardo MicroMouse Help Code</h1>
<img height=80px align='right' src='https://upload.wikimedia.org/wikipedia/commons/thumb/8/87/Arduino_Logo.svg/720px-Arduino_Logo.svg.png'/>
Here you will find the help code for MicroMouse.
<br>Please also refer to the MicroMouse user guide for more infomation.

## Key Changes & Updates
Acheived 'Maze-Routing'.
- Made IR reading ansyc with 3ms delay. Rest of program continue in that time.
This made a huge difference and almost trippled the granuality of the readings.
- Improved lane centering.
- Added collision detection using the step count.
Where if I detect the wheels are not spinning when they should be, I stop.
- Attempted a `collisionSolution()`, but not fully implemented this yet.
This involved adding a new method to the action buffer to replace the current action with a new one.
So if I detect a collision I could replace the current action with a 'reverse' action.
- Added `irMonitoring` action, to drive forwards until an IR reading is detected.
This is used just before turning to ensure right distance from wall infront.
- Added ability to turn around when reached a dead-end. It reverses against the wall behind to re-aligh.
I added a `turnAround` action to enable this. It turns around in the direction away from the closest wall to avoid a collision.
- Added designated `turnLeft` and `turnRight` actions. 
Removed the `rotate` action, as I found a different number of steps required for each.
- Improved gyro functionality, but still seems unusable. Has a +/- 10deg error.

## Contents
<img src="https://github.com/user-attachments/assets/4a2ef373-2c49-4930-8519-a5ad0b24f1ef" alt="MicroMouse Title Page" align="right" height=250px/>

- [Documentation](#documentation)
    - [Global Variables](#global-variables)
    - [Global Methods](#global-methods)
    - [Actions](#actions)
    - [Action Buffer](#action-buffer)
- [Getting Started](#getting-started)
- [Dependency](#dependency)
- [Contributors](#contributors)
- [License](#license)

## Documentation

### Global variables
```c++
const int DEBUG_MODE = 0; 
// 0 = No logs, 1 = some logs, 2 = all logs
// MUST BE SET TO `0` TO OPERATE UNCONNECTED FORM SERIAL MONITOR!
// When set to >=1 it waits forever until serial monitor connected, so no logs are missed.

bool wallLeft = true;
bool wallRight = true;
bool wallBack = true;
bool wallFront = false;
// Variables to store current surrounding walls.

bool fatalError = false; 
// If `true`, causes mouse to stop. 
// Updates this in `Main.ino` line 82.

bool collisionDetectionActive = false;
// When `true`, monitors the IMU and sets `fatalError` to `true` if IMU data goes above threshold, indicating a collision. 
// Threshold set in `Gyro.cpp` line 3.
```

### Global Methods

```c++
void print(String text, int priority);
// Logs the text `text` to the serial console if the global variable `DEBUG_MODE` is greater than `priority`.

void ledOn();
// Turns the Arduino's built in LED on.

void ledOff();
// Turns the Arduino's built in LED off.
```

### Actions
For each of these methods, `speed` is between 50-100.
These methods are defined in `ActionBuffer.cpp`. 
It's recommended to call these methods from `YourAlgorithm.cpp`.
```c++

void addBlindMoveForwardAction(float nCells, int speed);
// Moves the mouse forwards at `speed`. 
// Stops after traveling the number of cells `nCells`. 
// No IR detection and no lane centering.

void addMoveForwardAction(float nCells, int speed);
// Moves the mouse forwards at `speed`. 
// Stops after traveling the number of cells `nCells`. 
// With IR detection and lane centering.

void addRotateAction(int angle, int speed);
// Rotates the mouse by `angle` at `speed`. 
// Positive `angle` rotates clockwise, negative `angle` rotates anticlockwise.

void addParkAction();
// Stops both motors and sets speed to zero.

void addBlindReverseAction(float nCells, int speed);
// Moves the mouse backwards at `speed`. 
// Stops after traveling the number of cells `nCells`. 
// No IR detection and no lane centering.

void addCheckWallsAction();
// Updates the global boolean variables `wallFront`, `wallBack`, `wallLeft`, `wallRight`.

void addDelayAction(float delay_ms);
// Causes a pause for `delay_ms` milliseconds.
```

### Action Buffer
A circular buffer class used to keep track of actions the mouse needs to perform.
```c++
void initBuffer(CircularBuffer &cb);
// Initialises a new circular buffer to store data/actions.

bool isBufferFull(CircularBuffer &cb);
// Returns a Boolean indicating if the circular buffer `cb` is full. 
// Returns `true` if full.

bool isBufferEmpty(CircularBuffer &cb);
// Returns a Boolean indicating if the circular buffer `cb` is empty. 
// Returns `true` if empty.

bool addAction(CircularBuffer &cb, Action action);
// Adds the action `action` to the circular buffer `cb`. 
// Returns `false` if buffer is full.

bool getAction(CircularBuffer &cb, Action &action);
// Sets `action` to the next action from the circular buffer `cb` and removes the action from the buffer. 
// Returns `false` if buffer is empty.

void clearBuffer(CircularBuffer &cb);
// Empties the buffer `cb`.
```

<br>

## Getting Started
Create your maze-solving algorthim in `YourAlgorithm.cpp`. <br>
Make use of the mathods described above, to add actins to the `actionBuffer`.

<br>

## Dependency
`<Arduino_LSM9DS1.h>` Used to control the IMU & Gyro.

<br>

## Contributors
* Peter Metcalfe
* Yash Kalidindi
* Abbas Ashfaq

<br>
<br>

## License
CC BY-NC-SA 4.0
