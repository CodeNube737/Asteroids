//CAsteroidBelt.h

#pragma once
#include "asteroid.h"
#include <vector>

#define WINDOW_NAME "Asteroid(s)"
#define BKGRD_COLOR cv::Scalar(49,52,49)
#define INPUT_DELAY 1
#define HORIZON_Y 30
#define MIN_RADIUS 20
#define MAX_RADIUS 40
#define MIN_VELOCITY -5
#define MAX_VELOCITY 5
#define DELAY_TIME 10

/////////////////////////////////////////////////////////////////////////////////////////////////////
/* 2. AsteroidBelt Class (AsteroidBelt.h and AsteroidBelt.cpp):
 *
 * This class will manage all the asteroids.
 * Some might call this class a "container"
 * It should have a `std::vector` of Asteroid objects.
 * You'll need methods to add new asteroids, update the position of all asteroids,
 * detect and handle collisions, and draw all asteroids on the screen.
 *
 * You'll also need to handle user input and display the score.
 */
/////////////////////////////////////////////////////////////////////////////////////////////////////

class AsteroidBelt
{

private:
    //asteroid Asteroid;//(int rad, cv::Point pos, cv::Point vel, cv::Scalar col); // may not need this line at all...
    bool _leave;
    char _userInput;
    std::vector<asteroid> _Asteroids;
    int _killCounter;
    const int _maxQttyAsteroids;
    cv::Mat _canvas;
    cv::Size _guiSize;

public:
    AsteroidBelt(int maximum);

    //methods
    void run(); // calls draw and update in an infinite loop... can be done with threads in a later iteration
    void draw(); // checks for key input, then erases the previous screen, then draws the updated objects screen, based on updated values
    void update(); // updates values based on key input and then by on-screen interactions. Contains lots of sub-functions
    void generateAsteroid(); // handles construction of new asteroids. this is intially, and post-collision
    void moveAsteroids(); // updates the posiion of every asteroid based on velocity
    void detectCollisions(); // handles destruction & reconstruction of asteroids if there are collision(s)
};
