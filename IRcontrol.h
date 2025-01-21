#ifndef IR_Control_H
#define IR_Control_H

#include "Globals.h"


void setupLEDs();
void loopLEDs();

int getIRreading(const int led, const int nReadings=1);
void isWallFront();
void isWallLeft();
void isWallRight();
void isWallFrontClose();
void checkAllWalls();

void testLed(const int led);

#endif // IR_Control_H