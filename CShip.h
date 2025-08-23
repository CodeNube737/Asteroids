// CShip.h
#pragma once
#include <opencv2/opencv.hpp>

#define max_velocity 10 // 10 is good
#define ACCELERATION 1
#define SHIP_RADIUS 10
#define ORANGE_METR cv::Scalar(0, 165, 255)
#define SHIP_COLOR ORANGE_METR


class Spaceship
{
private:
    cv::Point position;
    cv::Point velocity;

public:
    Spaceship(cv::Point initial) :
        position(initial), velocity(cv::Point(0, 0)) {}

    //gets & sets
    cv::Point getPosition() {return position;}
    void setPosition(cv::Point newPosition) {position = newPosition;}
    cv::Point getVelocity() {return velocity;}
    void setVelocity(cv::Point newVelocity) {velocity = newVelocity;}
};
