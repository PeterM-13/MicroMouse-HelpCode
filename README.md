<h1 style="text-decoration: none;">Leonardo MicroMouse Help Code</h1>
<img height=80px align='right' src='https://upload.wikimedia.org/wikipedia/commons/thumb/8/87/Arduino_Logo.svg/720px-Arduino_Logo.svg.png'/>
Here you will find the help code for MicroMouse.
<br>Please also refer to the MicroMouse user guide for more infomation.

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

### Config.h
This file contains ALL the variables to control every aspect of your mouse, all in one place.
```c++
// *** Recommended order to configure: ***
CELL_DISTANCE
LEFT_MOTOR_BIAS
RIGHT_MOTOR_BIAS
// Use `testMotors()` for these.

LC_LEFT_RIGHT_BIAS
LC_LEFT_IR_VALUE
LC_RIGHT_IR_VALUE
FRONT_IR_VALUE_DIFF
// Use `testIrReadings()` for these, looking at Serial Plotter.

TURN_LEFT_DEG_TO_STEPS_MULTIPLIER
TURN_RIGHT_DEG_TO_STEPS_MULTIPLIER
// Watch it turning and adjust accordilngly.
```

### How to move your mouse
It's recommended to call these methods from 'YourAlgorithm'.
```c++
void startOffsetAction();
// Moves the mouse forwards 80% of a cell. Offsetting it enough to check the walls in the next cell.
// Called at the begining of the algorithm and after `turnAroundAction`.

void moveForwardAction();
// Moves the mouse 1 cell forwards.
// Lane centering is active.

void turnRightAction();
// Turns the mouse 90 degrees right.

void turnLeftAction();
// Turns the mouse 90 degrees left.

void turnAroundAction();
// Turns the mouse 180 degress, in the direction furthest from a wall to avoid collision.

int* getCell(int direction);
// Returns the cell in the `direction` relative to mouse.
// For example, if mouse at [10, 5] facing EAST, getCell(NORTH) returns [9, 5].
// For example, if mouse at [10, 5] facing SOUTH, getCell(NORTH) returns [10, 4].

float distToMiddle(int coord[2]);
// Returns the distance from `coord` to middle.
// Use this in combination with `getCell`, like: distToMiddle(getCell(NORTH));

```
<br>

### Global variables
```c++
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

bool laneCenteringActive = false;
// When `true`, enables lane centering. Also enables contunuous IR sensor readings.

bool updateGyroData = false;
// When `true`, continuously gets data from gyro, and updates Z angle.
// This is quite intensive, and slows down the program loop. So only enables when required, like when turning.
// But gyro seems inacurate anyway. So not being used.
```

### Global Methods

```c++
void print(String text, int priority);
// Logs the text `text` to the serial console if the global variable `DEBUG_MODE` is greater than `priority`.

void ledOn();
// Turns the Arduino's built in LED on.

void ledOff();
// Turns the Arduino's built in LED off.

void printLoopTime();
// If called from a `loop`ing function, it prints the time to complete 1 cycle.
```

<br>

## Getting Started
Create your maze-solving algorthim in `YourAlgorithm.cpp`. <br>
Make use of the mathods described above, to add actions to the `actionBuffer`.

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
