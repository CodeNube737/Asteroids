//CMissile.h
#pragma once
#include <opencv2/opencv.hpp>

class missile
{

private:
    int length;
    cv::Point postion;
    cv::Point velocity;
    const cv::Scalar color; // cv::Scalar(0, 0, 255)

public:
    missile(int len, cv::Point pos, cv::Point vel, cv::Scalar col) :
    lenth(len),
    postion(pos),
    velocity(vel),
    color(col)
{}

    //gets & sets
    int getLength() const {return length;}
    cv::Point getPosition() const {return postion;}
    cv::Point getVelocity() const {return velocity;}
    void setPosition(cv::Point newPosition) {postion = newPosition;}

    //methods... all other methods will be handled within the CAsteroidGame
};
