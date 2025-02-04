#include "Gyro.h"

const float COLLISION_THRESHOLD = 2.1; // smaller = more sensitive

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
  if(updateGyroData)
  {
    updateGyroDataValues();
  }

  if (collisionDetectionActive)
  {
    detectCollision();
  }
}

float maxValue = 0.0;
float minValue = 2.0;
float avgOffset = 0.0;
// During setup, calculate the bias
void calibrateGyro() 
{
  delay(100); // Allow time for user to move hand away from mouse
  print("INFO: Calibrating Gyro...");
  const float numSamples = 1000.0;
 // float maxValue = 0.0;
  float sumZ = 0.0;
  for (int i = 0; i < numSamples; i++) 
  {
    if (IMU.gyroscopeAvailable()) 
    {
      float x, y, z;
      IMU.readGyroscope(x, y, z);
      if(z > maxValue)
      {
        maxValue = z;
      }
      if(z < minValue)
      {
        minValue = z;
      }
      sumZ =+ z;
    }
  }
  avgOffset = sumZ / numSamples; 
  maxValue = maxValue * 1.1;
  minValue = minValue * 0.1;
  print("INFO: Gyro Calibrated, offset by: " + String(gyroBiasZ));
}

void updateGyroDataValues()
{
  if (IMU.gyroscopeAvailable()) 
  {
    float x, y, z;
    float dt = (micros() - lastGyroTime) / 1000000.0; // Time difference in seconds
    IMU.readGyroscope(x, y, z);
    lastGyroTime = micros();
    if(z > maxValue || z < minValue)
    {
      gyroAngle += ((z-avgOffset) * dt);
    }
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

