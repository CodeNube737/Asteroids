//CMissile.h
#pragma once
#include <opencv2/opencv.hpp>

#define LENGTH_MISSILE 6 // not too long!
#define SPEED_MISSILE 20 // 20 is good
#define COLOR_LASER cv::Scalar(0, 0, 255)
#define THICK_LASER 2

class missile
{

private:
    int _length;
    cv::Point _postion;
    cv::Point _velocity;

public:
    missile(int len, cv::Point pos, cv::Point vel) :
        _length(len), _postion(pos), _velocity(vel) {}

    //gets & sets
    int getLength() const {return _length;}
    cv::Point getPosition() const {return _postion;}
    cv::Point getVelocity() const {return _velocity;}
    void setPosition(cv::Point newPosition) {_postion = newPosition;}
};
