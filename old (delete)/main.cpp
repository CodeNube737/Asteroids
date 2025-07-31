#include "CAsteroidBelt.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * 3. Main Function (main.cpp):
 * This function will create an instance of the AsteroidBelt class and call its methods in a loop.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    AsteroidBelt CAsteroids(7); // argument is the maximum amount of asteroids allowed on screen... 7 is the sweetspot
    CAsteroids.run();

    return 0;
}
