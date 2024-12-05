# Leonardo MicroMouse Help Code

Here you will find the help code for MicroMouse.
Plase also refer to the MicroMouse user guide for more infomation.

## Documentation

### Global

#### `print(String text, int priority)`
Logs the text `text` to the serial console if the global variable `DEBUG_MODE` is greater than `priority`.

### `ledOn()`
Turns the Arduino's built in LED on.

### `ledOff()`
Turns the Arduino's built in LED off.

### Action Buffer

#### `initBuffer(CircularBuffer &cb)`
Initializes a new circular buffer to store data/actions.

#### `isBufferFull(CircularBuffer &cb)`
Returns a Boolean indicating if the circular buffer `cb` is full. Returns `true` if full.

#### `isBufferEmpty(CircularBuffer &cb)`
Returns a Boolean indicating if the circular buffer `cb` is empty. Returns `true` if empty.

#### `addAction(CircularBuffer &cb, Action action)`
Adds the action `action` to the circular buffer `cb`.

#### `getAction(CircularBuffer &cb, Action &action)`
Sets `action` to the next action from the circular buffer `cb` and removes the action from the buffer.

#### `clearBuffer(CircularBuffer &cb)`
Empties the buffer `cb`.

### Actions

For each of these methods, `speed` is between 0-100.

#### `addBlindMoveForwardAction(float nCells, int speed)`
Moves the mouse forwards at `speed`. Stops after traveling the number of cells `nCells`. No IR detection and no lane centering.

#### `addMoveForwardAction(float nCells, int speed)`
Moves the mouse forwards at `speed`. Stops after traveling the number of cells `nCells`. With IR detection and lane centering.

#### `addRotateAction(int angle, int speed)`
Rotates the mouse by `angle` at `speed`. Positive `angle` rotates clockwise, negative `angle` rotates anticlockwise.

#### `addParkAction()`
Stops both motors and sets speed to zero.

#### `addBlindReverseAction(float nCells, int speed)`
Moves the mouse backwards at `speed`. Stops after traveling the number of cells `nCells`. No IR detection and no lane centering.

#### `addCheckWallsAction()`
Updates the global boolean variables `wallFront`, `wallBack`, `wallLeft`, `wallRight`.

#### `addDelayAction(float delay_ms)`
Causes a pause for `delay_ms` milliseconds.


## Contributors
* Peter Metcalfe
* Yash Kalidindi
* Abbas 

## Lisence
CC BY-NC-SA 4.0