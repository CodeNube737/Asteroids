/*

// CAsteroidsGame.cpp
#include "CAsteroidGame.h"

CAsteroidsGame::CAsteroidsGame(cv::Point initial) :
     spaceship(initial), leave(false), key('0')
{
    img = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3, cv::Scalar(0, 0, 0));
}

void CAsteroidsGame::run() {
    while (!leave)
    {
        update();
        draw(img);
    }
}

void CAsteroidsGame::update() {
    key = cv::waitKey(DELAY);
    userInput(key);
    moveShip(WINDOW_WIDTH, WINDOW_HEIGHT);
    detectCollisions(WINDOW_HEIGHT); // should be done before moveLasers, so there's less lasers to move
    moveLasers();
}

void CAsteroidsGame::draw(cv::Mat& img) {
    img = cv::Scalar::all(0);

    drawText();
    drawShip();
    drawMissiles();
//    drawDebug();

    cv::imshow(WINDOW_NAME, img); // after all drawings
}

void CAsteroidsGame::drawText() {
    cv::putText(img, "Press 'l' to leave", cv::Point(WINDOW_WIDTH / 2 - 135, WINDOW_HEIGHT / 2 - 25), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
}

void CAsteroidsGame::drawShip() {
    cv::circle(img, spaceship.getPosition(), SHIP_RADIUS, SHIP_COLOR, -1);
}

void CAsteroidsGame::drawMissiles() {
    cv::Point laser_position;
    for (uint16_t i = 0; i < laser.size(); i++) {
        laser_position = laser[i].getPosition();
        cv::line(img, laser_position, cv::Point(laser_position.x, laser_position.y - LENGTH_MISSILE), COLOR_LASER, THICK_LASER );
    }
}

void CAsteroidsGame::drawDebug() {
    // the next 2 lines are to troubleshoot: earlier, this showed lasers generated, but not drawn, or moved. They can be reused to troubleshoot missing graphics.
    std::string lasers = std::to_string(laser.size());
    cv::putText(img, lasers, cv::Point(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 1);
}

void CAsteroidsGame::userInput(char direction) {
    cv::Point velocity = spaceship.getVelocity();
    switch (direction)
    {
    case 'w': velocity.y -= ACCELERATION; break; // (0,0) is top, left
    case 's': velocity.y += ACCELERATION; break;
    case 'a': velocity.x -= ACCELERATION; break;
    case 'd': velocity.x += ACCELERATION; break;
    case ' ': generateLaser(); break;
    case 'l': leave = true; break;
    }

    // Limit velocity to max_velocity
    velocity.x = std::min(std::max(velocity.x, -max_velocity), max_velocity);
    velocity.y = std::min(std::max(velocity.y, -max_velocity), max_velocity);

    spaceship.setVelocity(velocity);
}

void CAsteroidsGame::moveShip(int window_width, int window_height) {
    cv::Point position = spaceship.getPosition();
    position += spaceship.getVelocity();

    // Wrap around if the spaceship goes off-screen
    if (position.x < 0) position.x = window_width;
    if (position.x > window_width) position.x = 0;
    if (position.y < 0) position.y = window_height;
    if (position.y > window_height) position.y = 0;

    spaceship.setPosition(position);
}

void CAsteroidsGame::generateLaser() {
    cv::Point shipPosition = spaceship.getPosition();
    CAsteroidsGame::laser.push_back({LENGTH_MISSILE, shipPosition, cv::Point(0,SPEED_MISSILE)}); // missile(int len, cv::Point pos, cv::Point vel)
}

void CAsteroidsGame::detectCollisions(int window_height) {
    cv::Point laser_position;
    for (uint16_t i = 0; i < laser.size(); i++) {
        laser_position = laser[i].getPosition();
        if( laser_position.y <= 0 )
            laser.erase(laser.begin() + i); // cherno says this is how to erase
    }
}

void CAsteroidsGame::moveLasers() {
    // note: no need to wrap-around, cuz detectColision() will deal with the only possiblr border condition
    cv::Point laser_position;
    for (uint16_t i = 0; i < laser.size(); i++) {
        laser_position = laser[i].getPosition() - laser[i].getVelocity();
        laser[i].setPosition(laser_position);
    }
}

*/

// CAsteroidGame.cpp
#include "CAsteroidGame.h"

CAsteroidGame::CAsteroidGame(cv::Point shipPos, int numAsteroids)
    : spaceship(shipPos), leave(false), key('0')
{
    img = cv::Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3, BKGRD_COLOR);

    asteroids.reserve(numAsteroids);
    for (int i = 0; i < numAsteroids; ++i) {
        int rad = rand() % 30 + 10;
        cv::Point pos(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
        cv::Point vel(rand() % 7 - 3, rand() % 7 - 3);
        cv::Scalar col(rand() % 255, rand() % 255, rand() % 255);
        asteroids.emplace_back(rad, pos, vel, col);
    }
}

void CAsteroidGame::run() {
    while (!leave)
    {
        update();
        draw(img);
    }
}

void CAsteroidGame::update() {
    key = cv::waitKey(DELAY);

    // Ship/missiles system
    userInput(key);
    moveShip(WINDOW_WIDTH, WINDOW_HEIGHT);
    detectLaserCollisions(WINDOW_HEIGHT);
    moveLasers();

    // Asteroid system
    generateAsteroid();
    moveAsteroids();
    detectAsteroidCollisions(); // <-- Uses correct out-of-bounds logic
}

void CAsteroidGame::draw(cv::Mat& img) {
    img = BKGRD_COLOR;

    // Draw asteroids
    for (const asteroid& ast : asteroids)
        cv::circle(img, ast.getPosition(), ast.getRadius(), ast.getColor(), 5);

    // Draw ship
    cv::circle(img, spaceship.getPosition(), SHIP_RADIUS, SHIP_COLOR, -1);

    // Draw missiles
    for (const auto& m : laser) {
        cv::Point p = m.getPosition();
        cv::line(img, p, cv::Point(p.x, p.y - LENGTH_MISSILE), COLOR_LASER, THICK_LASER);
    }

    cv::putText(img, "Press 'l' to leave", cv::Point(WINDOW_WIDTH / 2 - 135, WINDOW_HEIGHT / 2 - 25),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);

    cv::imshow(WINDOW_NAME, img);
}

// --- Asteroid System ---
void CAsteroidGame::generateAsteroid() {
    if (asteroids.size() < MAX_ASTEROIDS) {
        int rad = rand() % 30 + 10;
        cv::Point pos(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
        cv::Point vel(rand() % 7 - 3, rand() % 7 - 3);
        cv::Scalar col(rand() % 255, rand() % 255, rand() % 255);
        asteroids.push_back(asteroid(rad, pos, vel, col));
    }
}

void CAsteroidGame::moveAsteroids() {
    for (asteroid& ast : asteroids) {
        cv::Point pos = ast.getPosition() + ast.getVelocity();

        // Wrap around edges (optional, you can remove this if you want out-of-bounds removal only)
        if (pos.x < 0) pos.x = WINDOW_WIDTH;
        if (pos.x > WINDOW_WIDTH) pos.x = 0;
        if (pos.y < 0) pos.y = WINDOW_HEIGHT;
        if (pos.y > WINDOW_HEIGHT) pos.y = 0;

        ast.setPosition(pos);
    }
}

void CAsteroidGame::detectAsteroidCollisions() {
    // Remove asteroids if out of bounds
    for (size_t i = 0; i < asteroids.size(); ) {
        cv::Point pos = asteroids[i].getPosition();
        if (pos.x < 0 || pos.x > WINDOW_WIDTH || pos.y < 0 || pos.y > WINDOW_HEIGHT) {
            asteroids.erase(asteroids.begin() + i);
        } else {
            ++i;
        }
    }
}

// --- Ship & Missile System ---
void CAsteroidGame::userInput(char direction) {
    cv::Point velocity = spaceship.getVelocity();
    switch (direction)
    {
        case 'w': velocity.y -= ACCELERATION; break;
        case 's': velocity.y += ACCELERATION; break;
        case 'a': velocity.x -= ACCELERATION; break;
        case 'd': velocity.x += ACCELERATION; break;
        case ' ': generateLaser(); break;
        case 'l': leave = true; break;
    }
    velocity.x = std::min(std::max(velocity.x, -max_velocity), max_velocity);
    velocity.y = std::min(std::max(velocity.y, -max_velocity), max_velocity);

    spaceship.setVelocity(velocity);
}

void CAsteroidGame::moveShip(int window_width, int window_height) {
    cv::Point position = spaceship.getPosition();
    position += spaceship.getVelocity();

    // Wrap around if the spaceship goes off-screen
    if (position.x < 0) position.x = window_width;
    if (position.x > window_width) position.x = 0;
    if (position.y < 0) position.y = window_height;
    if (position.y > window_height) position.y = 0;

    spaceship.setPosition(position);
}

void CAsteroidGame::generateLaser() {
    cv::Point shipPosition = spaceship.getPosition();
    laser.push_back({LENGTH_MISSILE, shipPosition, cv::Point(0,SPEED_MISSILE)});
}

void CAsteroidGame::detectLaserCollisions(int window_height) {
    for (size_t i = 0; i < laser.size(); ) {
        if (laser[i].getPosition().y <= 0)
            laser.erase(laser.begin() + i);
        else
            ++i;
    }
}

void CAsteroidGame::moveLasers() {
    for (size_t i = 0; i < laser.size(); ++i) {
        cv::Point laser_position = laser[i].getPosition() - laser[i].getVelocity();
        laser[i].setPosition(laser_position);
    }
}
