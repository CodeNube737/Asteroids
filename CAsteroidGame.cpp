// CAsteroidsGame.cpp
#include "CAsteroidGame.h"

CAsteroidsGame::CAsteroidsGame(int numAsteroids) :
    _state(PLAYING), _spaceship(cv::Point(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2)), _leave(false), _key('0')
{
    _img = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3, BKGRD_COLOR);

    if (_asteroids.size() > MAX_ASTEROIDS)
        numAsteroids = MAX_ASTEROIDS;
    _asteroids.reserve(numAsteroids);
    for (int i = 0; i < numAsteroids; ++i)
    {
        int rad = rand() % AVG_RADIUS + 10;
        cv::Point pos(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT/4);
        cv::Point vel(rand() % 7 - 3, rand() % 7 - 3);
        cv::Scalar col(rand() % 255, rand() % 255, rand() % 255);
        _asteroids.emplace_back(rad, pos, vel, col, pixel2float(pos));
    }
}

void CAsteroidsGame::run()
{
    while (!_leave)
    {
        update();
        draw(_img);
    }
}

void CAsteroidsGame::update()
{
    _key = cv::waitKey(DELAY);
    userInput(_key);
    moveShip(WINDOW_WIDTH, WINDOW_HEIGHT);
    detectCollisions(); // should be done before moveLasers&asteroids, so there's less lasers to move
    moveLasers();
    moveAsteroids();
}

void CAsteroidsGame::draw(cv::Mat& _img)
{
    _img = BKGRD_COLOR;
    drawText();
    if (_state == PLAYING)
    {
        drawShip();
        drawMissiles();
        drawAsteroids();
    }
    //drawDebug();
    cv::imshow(WINDOW_NAME, _img); // after all drawings
}

void CAsteroidsGame::drawText()
{
    cv::putText(_img, "Press 'l' to leave", cv::Point(WINDOW_WIDTH / 2 - 135, WINDOW_HEIGHT / 2 - 25), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    if (_state == GAME_OVER)
        cv::putText(_img, "Game Over", cv::Point(WINDOW_WIDTH / 2 - 180, WINDOW_HEIGHT / 2 - 100), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 2);
}

void CAsteroidsGame::drawAsteroids()
{
    for (const asteroid& ast : _asteroids)
        cv::circle(_img, ast.getPosition(), ast.getRadius(), ast.getColor(), 5);
}

void CAsteroidsGame::drawShip()
{
    cv::circle(_img, _spaceship.getPosition(), SHIP_RADIUS, SHIP_COLOR, -1);
}

void CAsteroidsGame::drawMissiles()
{
    cv::Point laser_position;
    for (uint16_t i = 0; i < _laser.size(); i++)
    {
        laser_position = _laser[i].getPosition();
        cv::line(_img, laser_position, cv::Point(laser_position.x, laser_position.y - LENGTH_MISSILE), COLOR_LASER, THICK_LASER );
    }
}

void CAsteroidsGame::drawDebug()
{
    // the next 2 lines are to troubleshoot: earlier, this showed lasers generated, but not drawn, or moved. They can be reused to troubleshoot missing graphics.
    std::string lasers = std::to_string(_laser.size());
    cv::putText(_img, lasers, cv::Point(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 1);
}

void CAsteroidsGame::userInput(char direction)
{
    cv::Point velocity = _spaceship.getVelocity();
    switch (direction)
    {
    case 'w':
        velocity.y -= ACCELERATION;
        break; // (0,0) is top, left
    case 's':
        velocity.y += ACCELERATION;
        break;
    case 'a':
        velocity.x -= ACCELERATION;
        break;
    case 'd':
        velocity.x += ACCELERATION;
        break;
    case ' ':
        generateLaser();
        break;
    case 'l':
        _leave = true;
        break;
    }

    // Limit velocity to max_velocity
    velocity.x = std::min(std::max(velocity.x, -max_velocity), max_velocity);
    velocity.y = std::min(std::max(velocity.y, -max_velocity), max_velocity);

    _spaceship.setVelocity(velocity);
}

cv::Point2f CAsteroidsGame::pixel2float(const cv::Point& p) const
{
    return cv::Point2f(
               static_cast<float>(p.x) * kVelocityScale,
               static_cast<float>(p.y) * kVelocityScale
           );

}

cv::Point CAsteroidsGame::float2pixel(const cv::Point2f& pf) const
{
    return cv::Point(
               static_cast<int>(pf.x / kVelocityScale),
               static_cast<int>(pf.y / kVelocityScale)
           );
}

void CAsteroidsGame::moveShip(int window_width, int window_height) // update with frame logic?
{
    cv::Point position = _spaceship.getPosition();
    position += _spaceship.getVelocity();

    // Wrap around if the spaceship goes off-screen
    if (position.x < 0) position.x = window_width;
    if (position.x > window_width) position.x = 0;
    if (position.y < 0) position.y = window_height;
    if (position.y > window_height) position.y = 0;

    _spaceship.setPosition(position);
}

void CAsteroidsGame::generateLaser()
{
    cv::Point shipPosition = _spaceship.getPosition();
    CAsteroidsGame::_laser.push_back({LENGTH_MISSILE, shipPosition, cv::Point(0,SPEED_MISSILE), pixel2float(shipPosition)}); // missile(int len, cv::Point pos, cv::Point vel)
}

void CAsteroidsGame::moveLasers()
{
    // Note: no need to wrap-around; border handled in detectColision()
    for (uint16_t i = 0; i < _laser.size(); i++)
    {
        // Get current float position and integer velocity
        cv::Point2f calculatedPos = _laser[i].getCalcPosition();
        cv::Point velocity = _laser[i].getVelocity();

        // Move by scaled velocity
        calculatedPos -= pixel2float(velocity);

        // Update accumulated float position
        _laser[i].setCalcPosition(calculatedPos);

        // For rendering/interface, convert back to pixel position
        cv::Point laser_position = float2pixel(calculatedPos);
        _laser[i].setPosition(laser_position);
    }
}

void CAsteroidsGame::generateAsteroid()
{
    if (_asteroids.size() < MAX_ASTEROIDS)
    {
        int rad = rand() % 30 + 10;
        cv::Point pos(rand() % WINDOW_WIDTH, rand() % (WINDOW_HEIGHT/2));
        cv::Point vel( (rand() % 7 - 3)*ASTEROID_SPEED/100, (rand() % 7 - 3)*ASTEROID_SPEED/100 );
        cv::Scalar col(rand() % 255, rand() % 255, rand() % 255);
        _asteroids.push_back(asteroid(rad, pos, vel, col, pixel2float(pos)));
    }
}

void CAsteroidsGame::moveAsteroids()
{
    for (asteroid& ast : _asteroids)
    {
        cv::Point2f calculatedPos = ast.getCalcPosition();
        cv::Point velocity = ast.getVelocity();

        calculatedPos += pixel2float(velocity); // Move by scaled velocity
        cv::Point pos = float2pixel(calculatedPos); // Convert to pixel position for rendering/interface

        // Wrap around edges (optional)
        if (pos.x < 0) pos.x = WINDOW_WIDTH;
        if (pos.x > WINDOW_WIDTH) pos.x = 0;
        if (pos.y < 0) pos.y = WINDOW_HEIGHT;
        if (pos.y > WINDOW_HEIGHT) pos.y = 0;
        calculatedPos = pixel2float(pos);

        // Update accumulated & rendered float position
        ast.setCalcPosition(calculatedPos);
        ast.setPosition(pos);
    }
}

void CAsteroidsGame::detectCollisions() // future work: add flags so that u don't check this every cycle
{
    int collisionCount = 0;

    missileBoundary(); // collision

    for (size_t i = 0; i < _asteroids.size(); i++)   // look at one asteroid collide w/ any object
    {
        cv::Point posI = _asteroids[i].getPosition();
        int radI = _asteroids[i].getRadius();

        asteroidAsteroid(i, posI, radI, collisionCount); // collision

        asteroidMissile(i, posI, radI, collisionCount); // collision

        asteroidShip(posI, radI); // collision
    }
    for (int i = 0; i < (collisionCount); ++i) // generate new asteroids
        generateAsteroid();
    //if(collisionCount) {cv::waitKey(DELAY);} // optional delay if CPU resources get clogged

}

void CAsteroidsGame::missileBoundary()
{
    cv::Point laser_position;
    for (uint16_t i = 0; i < _laser.size(); i++)
    {
        laser_position = _laser[i].getPosition();
        if( laser_position.y <= 0 )
            _laser.erase(_laser.begin() + i); // cherno says this is how to erase
    }
}

void CAsteroidsGame::asteroidAsteroid(size_t asteroidNo, cv::Point asteroidPos, int asteroidRad, int& collisionCount)
{
    int i = asteroidNo;
    cv::Point posI = asteroidPos;
    int radI = asteroidRad;
    for (size_t j = i+1; j < _asteroids.size(); j++)
    {
        cv::Point posJ = _asteroids[j].getPosition();
        int radJ = _asteroids[j].getRadius();
        double distance = cv::norm(posI-posJ); // compare asteroid "I" to each other asteroid
        if (distance <= (radI + radJ))
        {
            _asteroids.erase(_asteroids.begin() + i);
            _asteroids.erase(_asteroids.begin() + j - 1);
            j = _asteroids.size(); // exit for-loop
            collisionCount += 2;
        }
    }
}

void CAsteroidsGame::asteroidMissile(size_t asteroidNo, cv::Point asteroidPos, int asteroidRad, int& collisionCount)
{
    cv::Point laser_position;
    for (uint16_t i = 0; i < _laser.size(); i++)
    {
        laser_position = _laser[i].getPosition() + cv::Point(0,LENGTH_MISSILE);
        double distance = cv::norm(asteroidPos-laser_position);
        if( distance <= asteroidRad )
        {
            _laser.erase(_laser.begin() + i);
            _asteroids.erase(_asteroids.begin() + asteroidNo);
            collisionCount++;
        }
    }
}

void CAsteroidsGame::asteroidShip(cv::Point asteroidPos, int asteroidRad)
{
    cv::Point posI = asteroidPos;
    int radI = asteroidRad;
    cv::Point posJ = _spaceship.getPosition();
    int radJ = SHIP_RADIUS;

    double distance = cv::norm(posI-posJ); // compare asteroid "I" to each other asteroid
    if (distance <= (radI + radJ))
        loseGame();
}

void CAsteroidsGame::loseGame()
{
    _asteroids.clear();
    _laser.clear();
    _state = GAME_OVER;
}



