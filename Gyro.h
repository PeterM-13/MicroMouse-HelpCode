#ifndef Gyro
#define Gyro

#include <Arduino_LSM9DS1.h> // Library for IMU Arduino Nano BLE

#include "Globals.h"

extern float gyroAngle;

void setupGyro();
void loopGyro();

void calibrateGyro();

void updateGyroDataValues();

void detectCollision();

#endif // Gyro