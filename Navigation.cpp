#include "Navigation.h"

int currentCell[2];
int currentDirection = EAST;

// [0,0] = Bottom Left
void setupNavigation()
{
  currentDirection = EAST;
  currentCell[X] = 3;
  currentCell[Y] = 15;
}

void navForward()
{
  switch (currentDirection)
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
  switch (currentDirection)
  {
    case NORTH:
      currentDirection = EAST;
      break;
    case EAST:
      currentDirection = SOUTH;
      break;
    case SOUTH:
      currentDirection = WEST;
      break;
    case WEST:
      currentDirection = NORTH;
      break;
  }
}
void navLeft()
{
  switch (currentDirection)
  {
    case NORTH:
      currentDirection = WEST;
      break;
    case EAST:
      currentDirection = NORTH;
      break;
    case SOUTH:
      currentDirection = EAST;
      break;
    case WEST:
      currentDirection = SOUTH;
      break;
  }
}

int* getCell(int relativeDir) 
{
  static int result[2];
  result[X] = currentCell[X];
  result[Y] = currentCell[Y];
  int offsets[4][4][2] = {
      // NORTH  | EAST   | SOUTH  | WEST
      {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}, // Facing NORTH
      {{1, 0}, {0, -1}, {-1, 0}, {0, 1}}, // Facing EAST
      {{0, -1}, {-1, 0}, {0, 1}, {1, 0}}, // Facing SOUTH
      {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}  // Facing WEST
  };

  // Adjust coordinates based on the relative direction and the requested direction
  result[X] += offsets[currentDirection][relativeDir][X];
  result[Y] += offsets[currentDirection][relativeDir][Y];

  return result;
}

float distToMiddle(int coord[2])
{
  return sqrt(pow(coord[X] - 7.5, 2) + pow(coord[Y] - 7.5, 2));
}

bool checkMiddle()
{
  return ((currentCell[X]==7 && currentCell[Y]==7) ||
  (currentCell[X]==7 && currentCell[Y]==8) ||
  (currentCell[X]==8 && currentCell[Y]==7) ||
  (currentCell[X]==8 && currentCell[Y]==8));
}
