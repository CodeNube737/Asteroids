//main.cpp
#include "CAsteroidGame.h"

int main()
{
    CAsteroidsGame Asteroids(cv::Point(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), 7);

    Asteroids.run();

    return 0;
}
