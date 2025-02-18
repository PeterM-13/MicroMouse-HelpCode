#include "Navigation.h"

Cell currentCell;
int currentDirection = EAST;
float targetCell = 7.5;

// [0,0] = Bottom Left
void setupNavigation()
{
  // Define starting square and direction
  currentDirection = EAST;
  currentCell.X = 0;
  currentCell.Y = 0;
}

void navForward()
{
  switch (currentDirection)
  {
    case NORTH:
      currentCell.Y ++;
      break;
    case EAST:
      currentCell.X ++;
      break;
    case SOUTH:
      currentCell.Y --;
      break;
    case WEST:
      currentCell.X --;
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

const int offsets[4][4][2] = {
    // NORTH  | EAST   | SOUTH  | WEST
    {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}, // Facing NORTH
    {{1, 0}, {0, -1}, {-1, 0}, {0, 1}}, // Facing EAST
    {{0, -1}, {-1, 0}, {0, 1}, {1, 0}}, // Facing SOUTH
    {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}  // Facing WEST
};
Cell getCell(int relativeDir) 
{
  return {
    currentCell.X + offsets[currentDirection][relativeDir][X],
    currentCell.Y + offsets[currentDirection][relativeDir][Y]
  };
}

float distToMiddle(Cell coord)
{
  return sqrt(pow(coord.X-targetCell, 2) + pow(coord.Y-targetCell, 2));
}

bool checkMiddle()
{
  return ((currentCell.X==7 && currentCell.Y==7) ||
  (currentCell.X==7 && currentCell.Y==8) ||
  (currentCell.X==8 && currentCell.Y==7) ||
  (currentCell.X==8 && currentCell.Y==8));
}
