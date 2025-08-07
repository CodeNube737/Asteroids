/*
// CAsteroidsGame.h
#pragma once
#include "CShip.h"
#include "CMissile.h"
#include <vector>

#define WINDOW_NAME "Asteroids.mik"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define DELAY 30

class CAsteroidsGame
{
private:
    Spaceship spaceship;
    std::vector<missile> laser;
    cv::Mat img;
    bool leave;
    char key;

public:
    CAsteroidsGame(cv::Point position);

    // Methods
    void run(); // loops update and draw
    void update(); // gets user input & updates all positions & detects colission
    void draw(cv::Mat& img); // draws everything that was updated
    void drawText();
    void drawShip();
    void drawMissiles();
    void drawDebug();
    void userInput(char direction); // gets a single user input every loop
    void moveShip(int window_width, int window_height); // updates the position of the ship based on velocity
    void generateLaser(); // handles construction of new laser beams.
    void moveLasers(); // updates the position of every missile/laser based on velocity
    void detectCollisions(int window_height); // handles destruction of missiles if there are collision(s)... for this iteration, just terminate at the top of the screen

};
*/

#pragma once
#include <opencv2/opencv.hpp>
#include "CShip.h"
#include "CMissile.h"
#include "CAsteroid.h"
#include <vector>

#define WINDOW_NAME "Asteroids.mik"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define DELAY 30
#define INPUT_DELAY 30
#define BKGRD_COLOR cv::Scalar(0,0,0)
#define MAX_ASTEROIDS 10

class CAsteroidGame
{
private:
    // Ship & Missiles
    Spaceship spaceship;
    std::vector<missile> laser;

    // Asteroids
    std::vector<asteroid> asteroids;

    // Canvas and control
    cv::Mat img;
    bool leave;
    char key;

    // Asteroid system helpers
    void generateAsteroid();
    void moveAsteroids();
    void detectAsteroidCollisions();

    // Ship/missile system helpers
    void userInput(char direction);
    void moveShip(int window_width, int window_height);
    void generateLaser();
    void moveLasers();
    void detectLaserCollisions(int window_height);

public:
    CAsteroidGame(cv::Point shipPos, int numAsteroids);

    void run();                // Main loop
    void update();             // Update both systems
    void draw(cv::Mat& img);   // Draw both systems
};
