#include "Gyro.h"

const float COLLISION_THRESHOLD = 1.8; // smaller = more sensitive

float gyroAngle = 0.0;
float gyroBiasZ = 0.0;
unsigned long lastGyroTime; // last gyro measurement

void setupGyro()
{
  if (!IMU.begin()) 
  {
    print("ERROR: Failed to initialize IMU!");
    fatalError = true;
    return;
  }
  calibrateGyro();
} 


void loopGyro()
{
  //updateGyroData();

  if (collisionDetectionActive)
  {
    detectCollision();
  }
}


// During setup, calculate the bias
void calibrateGyro() 
{
  delay(100); // Allow time for user to move hand away from mouse
  print("INFO: Calibrating Gyro...");
  const int numSamples = 100;
  float sumZ = 0.0;
  for (int i = 0; i < numSamples; i++) 
  {
    if (IMU.gyroscopeAvailable()) 
    {
      float x, y, z;
      IMU.readGyroscope(x, y, z);
      sumZ += z;
      delay(5);  // Small delay for sampling
    }
  }
  gyroBiasZ = sumZ / numSamples; 
  print("INFO: Gyro Calibrated, offset by: " + String(gyroBiasZ));
}

void updateGyroData()
{
  if (IMU.gyroscopeAvailable()) 
  {
    float x, y, z;
    long currentGyroTime = millis();
    float dt = (currentGyroTime - lastGyroTime) / 1000.0; // Time difference in seconds
    lastGyroTime = currentGyroTime;
    IMU.readGyroscope(x, y, z);
    z -= gyroBiasZ;  // Apply bias correction
    gyroAngle += z * dt;
    print("GyroAngle: "+ String(gyroAngle), 2);
  }
}


void detectCollision()
{
  if (IMU.accelerationAvailable()) 
  {
    float accelX, accelY, accelZ;
    IMU.readAcceleration(accelX, accelY, accelZ);
    // Calculate the magnitude of acceleration and compare to threshold
    float magnitude = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);
    if (magnitude > COLLISION_THRESHOLD) 
    {
      print("ERROR: Collision detected!");
      fatalError = true;    // ******** This does not have to be an error - You could try and reverse for example ***********
    }
  }
}

