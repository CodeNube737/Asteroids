// CAsteroidsGame.h
#pragma once
#include "CShip.h"
#include "CMissile.h"
#include "CAsteroid.h"
#include <vector>
// game parameters
#define WINDOW_NAME "Asteroids.mik"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define DELAY 30
#define BKGRD_COLOR cv::Scalar(0,0,0)

class CAsteroidsGame
{
private:
    enum state_t { PLAYING, PAUSED, GAME_OVER, MENU }; // future work
    state_t State;

    Spaceship spaceship; // initialized in constructor
    std::vector<missile> laser; // non-initial, after constructor
    std::vector<asteroid> asteroids; // post-initial, (in constructor, and after)
    cv::Mat img;
    bool leave;
    char key;

public:
    CAsteroidsGame(int numAsteroids);

    // Methods
    void run(); // loops update and draw
    void update(); // gets user input & updates all positions & detects colission
    void draw(cv::Mat& img); // draws everything that was updated
    void drawText();
    void drawShip();
    void drawMissiles();
    void drawAsteroids();
    void drawDebug(); // optional, if you notice an object isn't updating properly, you can show its members
    void userInput(char direction); // gets a single user input every loop
    void moveShip(int window_width, int window_height); // updates the position of the ship based on velocity
    void generateLaser(); // handles construction of new laser beams.
    void moveLasers(); // updates the position of every missile/laser based on velocity
    void generateAsteroid(); // used everytime 1 or more asteroids are destroyed
    void moveAsteroids(); // updates the position of the asteroids based on velocity and ASTEROID_SPEED
    void detectCollisions(); // most complex, but is ultimately just a tree of what-if's (possible collisions)
    void missileBoundary(); // collisions between lasers and top of screen
    void asteroidAsteroid(size_t asteroidNo, cv::Point asteroidPos, int asteroidRad, int& collisionCount); // collisions between Asteroids and themselves
    void asteroidMissile(size_t asteroidNo, cv::Point asteroidPos, int asteroidRad, int& collisionCount); // collisions between Asteroids and missiles (lasers)
    void asteroidShip(cv::Point asteroidPos, int asteroidRad); // collisions between ship and the asteroids
    void loseGame(); // should pause the screen, erase all but the "press l to leave", and rewrite in below, "You lose"... may also display your score

};
