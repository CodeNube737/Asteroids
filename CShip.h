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
    cv::Point _position;
    cv::Point _velocity;
    cv::Point2f _calculatedPosition;

public:
    Spaceship(cv::Point initial) :
        _position(initial), _velocity(cv::Point(0, 0)), _calculatedPosition(cv::Point2f(0.0f, 0.0f)) {}

    //gets & sets
    cv::Point getPosition() {return _position;}
    void setPosition(cv::Point newPosition) {_position = newPosition;}
    cv::Point getCalcPosition() {return _calculatedPosition;}
    void setCalcPosition(cv::Point newPosition) {_calculatedPosition = newPosition;}
    cv::Point getVelocity() {return _velocity;}
    void setVelocity(cv::Point newVelocity) {_velocity = newVelocity;}
};
