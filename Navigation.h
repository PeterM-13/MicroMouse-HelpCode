#ifndef NAVIGATION
#define NAVIGATION

#include <Arduino.h>
#include <math.h>

// Define Types
enum coordinates
{
    X = 0,
    Y = 1,
};
enum compassDirection
{
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3,
};

extern int currentCell[2];
extern int currentDirection;
extern float targetCell;

void setupNavigation();

void navForward();
void navRight();
void navLeft();

int* getCell(int direction);
float distToMiddle(int coord[2]);

bool checkMiddle();

#endif // NAVIGATION