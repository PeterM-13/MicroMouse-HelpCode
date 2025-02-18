#ifndef NAVIGATION
#define NAVIGATION

#include <Arduino.h>
#include <math.h>

// Define Types
struct Cell {
    int X;
    int Y;
};
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

extern Cell currentCell;
extern int currentDirection;
extern float targetCell;

void setupNavigation();

void navForward();
void navRight();
void navLeft();

Cell getCell(int direction);
float distToMiddle(Cell coord);

bool checkMiddle();

#endif // NAVIGATION