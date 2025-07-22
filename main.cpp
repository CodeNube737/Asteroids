#include "CAsteroidGame.h"

int main()
{
    CAsteroidsGame Asteroids(cv::Point(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));

    Asteroids.run();

    return 0;
}
