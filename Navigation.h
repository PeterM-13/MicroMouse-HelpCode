#ifndef NAVIGATION
#define NAVIGATION

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

void setupNavigation();

void navForward();
void navRight();
void navLeft();

bool checkMiddle();

#endif // NAVIGATION