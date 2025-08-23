// CAsteroidsGame.cpp
#include "CAsteroidGame.h"

CAsteroidsGame::CAsteroidsGame(cv::Point start_position, int numAsteroids) :
    spaceship(start_position), leave(false), key('0'), collisionCount(0)
{
    img = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3, BKGRD_COLOR);

    if (asteroids.size() < MAX_ASTEROIDS)
        numAsteroids = MAX_ASTEROIDS;
    asteroids.reserve(numAsteroids);
    for (int i = 0; i < numAsteroids; ++i)
    {
        int rad = rand() % 30 + 10;
        cv::Point pos(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
        cv::Point vel(rand() % 7 - 3, rand() % 7 - 3);
        cv::Scalar col(rand() % 255, rand() % 255, rand() % 255);
        asteroids.emplace_back(rad, pos, vel, col);
    }
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
    detectCollisions(); // should be done before moveLasers&asteroids, so there's less lasers to move
    moveLasers();
    moveAsteroids();
}

void CAsteroidsGame::draw(cv::Mat& img)
{
    img = BKGRD_COLOR;
    drawText();
    drawShip();
    drawMissiles();
    drawAsteroids();
//    drawDebug();
    cv::imshow(WINDOW_NAME, img); // after all drawings
}

void CAsteroidsGame::drawText()
{
    cv::putText(img, "Press 'l' to leave", cv::Point(WINDOW_WIDTH / 2 - 135, WINDOW_HEIGHT / 2 - 25), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
}

void CAsteroidsGame::drawAsteroids()
{
    for (const asteroid& ast : asteroids)
        cv::circle(img, ast.getPosition(), ast.getRadius(), ast.getColor(), 5);
}

void CAsteroidsGame::drawShip()
{
    cv::circle(img, spaceship.getPosition(), SHIP_RADIUS, SHIP_COLOR, -1);
}

void CAsteroidsGame::drawMissiles()
{
    cv::Point laser_position;
    for (uint16_t i = 0; i < laser.size(); i++)
    {
        laser_position = laser[i].getPosition();
        cv::line(img, laser_position, cv::Point(laser_position.x, laser_position.y - LENGTH_MISSILE), COLOR_LASER, THICK_LASER );
    }
}

void CAsteroidsGame::drawDebug()
{
    // the next 2 lines are to troubleshoot: earlier, this showed lasers generated, but not drawn, or moved. They can be reused to troubleshoot missing graphics.
    std::string lasers = std::to_string(laser.size());
    cv::putText(img, lasers, cv::Point(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 1);
}

void CAsteroidsGame::userInput(char direction)
{
    cv::Point velocity = spaceship.getVelocity();
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
        leave = true;
        break;
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

void CAsteroidsGame::generateLaser()
{
    cv::Point shipPosition = spaceship.getPosition();
    CAsteroidsGame::laser.push_back({LENGTH_MISSILE, shipPosition, cv::Point(0,SPEED_MISSILE)}); // missile(int len, cv::Point pos, cv::Point vel)
}

void CAsteroidsGame::moveLasers()
{
    // note: no need to wrap-around, cuz detectColision() will deal with the only possiblr border condition
    cv::Point laser_position;
    for (uint16_t i = 0; i < laser.size(); i++)
    {
        laser_position = laser[i].getPosition() - laser[i].getVelocity();
        laser[i].setPosition(laser_position);
    }
}

void CAsteroidsGame::generateAsteroid()
{
    if (asteroids.size() < MAX_ASTEROIDS)
    {
        int rad = rand() % 30 + 10;
        cv::Point pos(rand() % WINDOW_WIDTH, rand() % (WINDOW_HEIGHT/2));
        cv::Point vel( (rand() % 7 - 3)*ASTEROID_SPEED/100, (rand() % 7 - 3)*ASTEROID_SPEED/100 );
        cv::Scalar col(rand() % 255, rand() % 255, rand() % 255);
        asteroids.push_back(asteroid(rad, pos, vel, col));
    }
}

void CAsteroidsGame::moveAsteroids()
{
    for (asteroid& ast : asteroids)
    {
        cv::Point pos = ast.getPosition() + (ast.getVelocity()*ASTEROID_SPEED/100);

        // Wrap around edges (optional, you can remove this if you want out-of-bounds removal only)
        if (pos.x < 0) pos.x = WINDOW_WIDTH;
        if (pos.x > WINDOW_WIDTH) pos.x = 0;
        if (pos.y < 0) pos.y = WINDOW_HEIGHT;
        if (pos.y > WINDOW_HEIGHT) pos.y = 0;

        ast.setPosition(pos);
    }
}

void CAsteroidsGame::detectCollisions() // future work: add flags so that u don't check this every cycle
{
    int collisionCount = 0;

    missileBoundary(); // collision

    for (size_t i = 0; i < asteroids.size(); i++)   // look at one asteroid collide w/ any object
    {
        cv::Point posI = asteroids[i].getPosition();
        int radI = asteroids[i].getRadius();

        asteroidAsteroid(i, posI, radI, collisionCount); // collision

        asteroidMissile(i, posI, radI, collisionCount); // collision

        ///else if ...copied incorrect...( (pos.x < 0 || pos.x > WINDOW_WIDTH || pos.y < 0 || pos.y > WINDOW_HEIGHT) ) {
        // collisions between ship and the asteroids INCOMPLETE
        //asteroids
        //loseGame(); // not made yet, but forshadowed in header-file

    }
    for (int i = 0; i < (collisionCount); ++i) // generate new asteroids
        generateAsteroid();
    //if(collisionCount) {cv::waitKey(DELAY);} // optional delay if CPU resources get clogged

}

void CAsteroidsGame::missileBoundary()
{
    cv::Point laser_position;
    for (uint16_t i = 0; i < laser.size(); i++)
    {
        laser_position = laser[i].getPosition();
        if( laser_position.y <= 0 )
            laser.erase(laser.begin() + i); // cherno says this is how to erase
    }
}

void CAsteroidsGame::asteroidAsteroid(size_t asteroidNo, cv::Point asteroidPos, int asteroidRad, int& collisionCount)
{
    int i = asteroidNo;
    cv::Point posI = asteroidPos;
    int radI = asteroidRad;
            for (size_t j = i+1; j < asteroids.size(); j++)
        {
            cv::Point posJ = asteroids[j].getPosition();
            int radJ = asteroids[j].getRadius();
            double distance = cv::norm(posI-posJ); // compare asteroid "I" to each other asteroid
            if (distance <= (radI + radJ))
            {
                asteroids.erase(asteroids.begin() + i);
                asteroids.erase(asteroids.begin() + j - 1);
                j = asteroids.size(); // exit for-loop
                collisionCount += 2;
            }
        }
}

void CAsteroidsGame::asteroidMissile(size_t asteroidNo, cv::Point asteroidPos, int asteroidRad, int& collisionCount)
{
    cv::Point laser_position;
    for (uint16_t i = 0; i < laser.size(); i++)
    {
        laser_position = laser[i].getPosition() + cv::Point(0,LENGTH_MISSILE);
        double distance = cv::norm(asteroidPos-laser_position);
        if( distance <= asteroidRad )
        {
            laser.erase(laser.begin() + i);
            asteroids.erase(asteroids.begin() + asteroidNo);
            collisionCount++;
        }
    }
}

// below is a copy of the original detect collisions for Asteroids:
/*

void AsteroidBelt::detectCollisions() // thanks again gpt!
{
    int collisionCount = 0;

    // Check for collisions between asteroids
    for (size_t i = 0; i < _Asteroids.size(); ++i)
    {
        for (size_t j = i + 1; j < _Asteroids.size(); ++j)
        {
            cv::Point pos1 = _Asteroids[i].getPosition();
            cv::Point pos2 = _Asteroids[j].getPosition();
            int rad1 = _Asteroids[i].getRadius();
            int rad2 = _Asteroids[j].getRadius();

            // Calculate the distance between the two asteroids
            double distance = cv::norm(pos1 - pos2);

            // Check if the distance is less than the sum of their radii (collision detected)
            if (distance < rad1 + rad2)
            {
                collisionCount++;
                _Asteroids.erase(_Asteroids.begin() + j);
                _Asteroids.erase(_Asteroids.begin() + i);
            }
        }
    }

    // Generate new asteroids (you can adjust the number of new asteroids as needed)
    for (int i = 0; i < (collisionCount * 2); ++i) // "* 2", assuming 2 asteroids collided with eachother
        generateAsteroid();

    // Optionally, you can add a delay for a few cycles if needed
    if(collisionCount) {cv::waitKey(DELAY_TIME);}
}
*/
