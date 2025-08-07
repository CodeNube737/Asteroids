#include "CAsteroidGame.h"

int main()
{
    CAsteroidGame Asteroids(cv::Point(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), 7); // 7 is the best amount of asteroids

    Asteroids.run();

    return 0;
}
