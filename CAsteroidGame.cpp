// CAsteroidsGame.cpp
#include "CAsteroidGame.h"

CAsteroidsGame::CAsteroidsGame(cv::Point initial) :
     spaceship(initial), leave(false), key('0')
{
    img = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3, cv::Scalar(0, 0, 0));
}

void CAsteroidsGame::run()
{
    while (!leave)
    {
        update();
        draw(img);
    }
}

void CAsteroidsGame::update()
{
    key = cv::waitKey(DELAY);
    userInput(key);
    moveShip(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void CAsteroidsGame::draw(cv::Mat& img)
{
    img = cv::Scalar::all(0);


    cv::circle(img, spaceship.getPosition(), SHIP_RADIUS, SHIP_COLOR, -1);
    cv::putText(img, "Press 'l' to leave", cv::Point(WINDOW_WIDTH / 2 - 135, WINDOW_HEIGHT / 2 - 25), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    cv::imshow(WINDOW_NAME, img);
}

void CAsteroidsGame::userInput(char direction)
{
    cv::Point velocity = spaceship.getVelocity();
    switch (direction)
    {
    case 'w': velocity.y -= ACCELERATION; break;
    case 's': velocity.y += ACCELERATION; break;
    case 'a': velocity.x -= ACCELERATION; break;
    case 'd': velocity.x += ACCELERATION; break;
    case 'l': leave = true; break;
    }

    // Limit velocity to max_velocity
    velocity.x = std::min(std::max(velocity.x, -max_velocity), max_velocity);
    velocity.y = std::min(std::max(velocity.y, -max_velocity), max_velocity);

    spaceship.setVelocity(velocity);
}

void CAsteroidsGame::moveShip(int window_width, int window_height)
{
    cv::Point position = spaceship.getPosition();
    position += spaceship.getVelocity();

    // Wrap around if the spaceship goes off-screen
    if (position.x < 0) position.x = window_width;
    if (position.x > window_width) position.x = 0;
    if (position.y < 0) position.y = window_height;
    if (position.y > window_height) position.y = 0;

    spaceship.setPosition(position);
}




