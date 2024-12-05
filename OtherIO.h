#ifndef Other_IO
#define Other_IO
// (Other Input / Output)

#include "Globals.h"
#include "YourAlgorithm.h"


extern unsigned int pulseLedDelay_ms;


void setupOtherIO();
void loopOtherIO();

void pushButton1Pressed();
void pushButton2Pressed();
bool checkButtonsForPress();

void pulseLedLoop();


#endif // Other_IO