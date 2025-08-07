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
