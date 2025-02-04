#include "Navigation.h"

int currentCell[2];
int relativeDirection = EAST;

void setupNavigation()
{
  currentCell[X] = 0;
  currentCell[Y] = 0;
}

void navForward()
{
  switch (relativeDirection)
  {
    case NORTH:
      currentCell[Y] ++;
      break;
    case EAST:
      currentCell[X] ++;
      break;
    case SOUTH:
      currentCell[Y] --;
      break;
    case WEST:
      currentCell[X] --;
      break;
  }
}

void navRight()
{
  switch (relativeDirection)
  {
    case NORTH:
      relativeDirection = EAST;
      break;
    case EAST:
      relativeDirection = SOUTH;
      break;
    case SOUTH:
      relativeDirection = WEST;
      break;
    case WEST:
      relativeDirection = NORTH;
      break;
  }
}
void navLeft()
{
  switch (relativeDirection)
  {
    case NORTH:
      relativeDirection = WEST;
      break;
    case EAST:
      relativeDirection = NORTH;
      break;
    case SOUTH:
      relativeDirection = EAST;
      break;
    case WEST:
      relativeDirection = SOUTH;
      break;
  }
}

bool checkMiddle()
{
  return ((currentCell[X]==7 && currentCell[Y]==7) ||
  (currentCell[X]==7 && currentCell[Y]==8) ||
  (currentCell[X]==8 && currentCell[Y]==7) ||
  (currentCell[X]==8 && currentCell[Y]==8));
}
