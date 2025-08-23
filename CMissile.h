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
    int length;
    cv::Point postion;
    cv::Point velocity;

public:
    missile(int len, cv::Point pos, cv::Point vel) :
        length(len), postion(pos), velocity(vel) {}

    //gets & sets
    int getLength() const {return length;}
    cv::Point getPosition() const {return postion;}
    cv::Point getVelocity() const {return velocity;}
    void setPosition(cv::Point newPosition) {postion = newPosition;}
};
