//asteroid.h
#pragma once
#include <opencv2/opencv.hpp>
////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Asteroid Class (Asteroid.h and Asteroid.cpp):
 *
 * This class will represent a single asteroid.
 * It should have properties like position, size, color, and velocity.
 * You can use OpenCV's `cv::Point` for position and velocity, and `cv::Scalar` for color.
 * You'll also need methods to move the asteroid and detect collisions, that's to be handled within the AsteroidBelt.
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////

class asteroid
{

private:
    int radius;
    cv::Point postion;
    cv::Point velocity;
    cv::Scalar color;

public:
    asteroid(int rad, cv::Point pos, cv::Point vel, cv::Scalar col);

    //gets & sets
    int getRadius() const {return radius;}
    cv::Point getPosition() const {return postion;}
    cv::Point getVelocity() const {return velocity;}
    cv::Scalar getColor() const {return color;}
    //void setRadius(int newRadius) {radius = newRadius;} // not needed, cuz u never change the radius of an asteroid post-construction
    void setPosition(cv::Point newPosition) {postion = newPosition;}

    //methods... all other methods will be handled within the AsteroidBelt
};
