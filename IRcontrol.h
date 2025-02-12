#ifndef IR_Control_H
#define IR_Control_H

#include "Globals.h"

const int N_SENSORS = 4; // Number of sensors

extern int irReadings[N_SENSORS][3];
extern bool irSensorsActive;

void setupLEDs();
void loopLEDs();

//int getIRreading(const int led, const int nReadings=1);
void startIrReading(const int led);
void stopIrReading(const int led);

void checkAllWalls();

void testLed(const int led);

#endif // IR_Control_H