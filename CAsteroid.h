//CAsteroid.h
#pragma once
#include <opencv2/opencv.hpp>

#define AVG_RADIUS 30 // +/- 10%
#define MAX_ASTEROIDS 10 // to prevent a fringe case over-generation
#define ASTEROID_SPEED 100 // percentage
#define MIN_ASTEROID_SPEED 2.0f

class asteroid
{

private:
    int _radius;
    cv::Point _postion;
    cv::Point _velocity;
    cv::Scalar _color;
    cv::Point2f _calculatedPosition;

public:
    asteroid(int rad, cv::Point pos, cv::Point vel, cv::Scalar col, cv::Point2f cPos) :
        _radius(rad), _postion(pos), _velocity(vel), _color(col), _calculatedPosition(cPos) {}

    //gets & sets
    int getRadius() const {return _radius;}
    cv::Point getPosition() const {return _postion;}
    void setPosition(cv::Point newPosition) {_postion = newPosition;}
    cv::Point getCalcPosition() {return _calculatedPosition;}
    void setCalcPosition(cv::Point newPosition) {_calculatedPosition = newPosition;}
    cv::Point getVelocity() const {return _velocity;}
    cv::Scalar getColor() const {return _color;}
};


// the moveAsteroid could be in this file, so the CGame doesn't need to think the best
// inheritance saves lines
