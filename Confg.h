
// DEBUG must be 0 if not connected to Serial
const int DEBUG_MODE = 0; // 1 = some logs, 2 = all logs


// ------------- Constants for Motors ----------------
// Number of encoder cycles to move one cell.
const int CELL_DISTANCE = 600; // Increase to go further
// Motor Bias.
const int LEFT_MOTOR_BIAS = 0; // Increase to make go right, decrease to go left
const int RIGHT_MOTOR_BIAS = 3;  // Increase to make go left, decrease to go right
// How close to get to the wall infront before turning.
const int IR_MONITORING_THRESHOLD = 47; // Direct IR value. Larger value = further from wall

// ------------ Constants for turning ---------------
// Number of steps to count when making left turn
const float TURN_RIGHT_DEG_TO_STEPS_MULTIPLIER = 2.4; // Increase to to turn right for longer.
const float TURN_LEFT_DEG_TO_STEPS_MULTIPLIER = 2.5; // Increase to turn left for longer
// IR values used to detect a gap (no wall). Motor steps counted every time gap opens to know when to turn.
const int IR_SENSOR_2_WALL_GAP_THRESHOLD = 200; // Increase for more sensitivity
const int IR_SENSOR_3_WALL_GAP_THRESHOLD = 200; // Good range: 120-320;
// When approaching a wall in front, whats the differnece in the 2 IR front values? AND then half it.
const int FRONT_IR_VALUE_DIFF = 2; // Positive for large right sensor value. E.g. Left=52, Right=56, 56-52=4, 4/2 = 2
// Scale for how much/less extra to turn based on front sensor readings difference.
// Set to 0 now so not active, had better results when not active.
const float TURNING_DIFF_SCALE = 0.0; // Higher = more turning based on readings

// --------  Constansts used for lane centering (LC) --------------
// How tight to the middle of the lane it stays. Lower value = narrower band to stay in, so more sensitive.
const int LC_OFF_AXES_THRESHOLD = 3; 
// Corrects bias left or right of the lane.
const signed int LC_LEFT_RIGHT_BIAS = -2; //make negative for left movement 
// The maximum the lane centering can adjust the motor bias.
const int LC_MAX_CORRECTION = 20; // Decrease for small adjustments
// -- Following vars used for 1 walled laned centering - so only a wall on the left, or only a wall on the right --
// Value used to intiate one-sided lane centering. Smaller than main wall threshold to detect singular posts.
const int IR_SENSOR_2_WALL_THRESHOLD_LC = 250; // Increase for more sensitivty. Don't make too big or will detect wall from 2 cells away.
const int IR_SENSOR_3_WALL_THRESHOLD_LC = 250; // Good range: 120-320;
// Left IR value to aim for when lane centering, with only one side wall.
const int LC_LEFT_IR_VALUE = 56; 
// Right IR value to aim for when lane centering, with only one side wall.
const int LC_RIGHT_IR_VALUE = 59; 

// --------- Constants for crash detection
// Crash detection with encoder steps, how often it checks for missed steps.
const int COL_DETC_TIME_GAP_ms = 500;   // Check for collision every 500ms
// Crash detection with encoder steps, the number of steps to be off by to define a collision.
const int COL_DETC_STEP_THRESHOLD = 50; // Check for min 50 steps every 500ms
// Gyro collision detection threshold. If IMU magnitude greater than this value, collision detected.
const float COLLISION_THRESHOLD = 2.8; // smaller = more sensitive

// ------------ Constants for IR sensors -------------
// Max IR reading limits to detect a wall.
const int IR_SENSOR_1_WALL_THRESHOLD = 400;
const int IR_SENSOR_2_WALL_THRESHOLD = 400;
const int IR_SENSOR_3_WALL_THRESHOLD = 400;
const int IR_SENSOR_4_WALL_THRESHOLD = 400;

