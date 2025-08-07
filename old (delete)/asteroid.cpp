//asteroid.cpp
#include "asteroid.h"

asteroid::asteroid(int rad, cv::Point pos, cv::Point vel, cv::Scalar col) :
    radius(rad),
    postion(pos),
    velocity(vel),
    color(col)
{}
