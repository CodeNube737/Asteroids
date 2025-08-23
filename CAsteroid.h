//CAsteroid.h
#pragma once
#include <opencv2/opencv.hpp>

#define AVG_RADIUS 30 // +/- 10%
#define MAX_ASTEROIDS 10 // to prevent a fringe case over-generation
#define ASTEROID_SPEED 100 // percentage

class asteroid
{

private:
    int radius;
    cv::Point postion;
    cv::Point velocity;
    cv::Scalar color;

public:
    asteroid(int rad, cv::Point pos, cv::Point vel, cv::Scalar col) :
        radius(rad), postion(pos), velocity(vel), color(col) {}

    //gets & sets
    int getRadius() const {return radius;}
    cv::Point getPosition() const {return postion;}
    cv::Point getVelocity() const {return velocity;}
    cv::Scalar getColor() const {return color;}
    void setPosition(cv::Point newPosition) {postion = newPosition;}
};
