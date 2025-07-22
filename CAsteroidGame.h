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
    void run();
    void update();
    void draw(cv::Mat& img);
    void userInput(char direction);
    void moveShip(int window_width, int window_height);
    void generateLaser(); // handles construction of new laser beams.
    void moveLasers(); // updates the posiion of every asteroid based on velocity
    void detectCollisions(); // handles destruction of missiles if there are collision(s)

};
