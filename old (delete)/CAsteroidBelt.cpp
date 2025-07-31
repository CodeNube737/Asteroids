//CAsteroidBelt.cpp
#include "CAsteroidBelt.h"

AsteroidBelt::AsteroidBelt(int maximum) :
    _leave(false), _userInput('0'), _maxQttyAsteroids(maximum), _guiSize(700,640) // should eventually be "_qtty(maximum)"
{
    //cvui::init(WINDOW_NAME, 20); // doesn't seem to be necessary
    // Generate the maximum number of asteroids
    for (int i = 0; i < _maxQttyAsteroids; ++i)
        generateAsteroid();

    _canvas = cv::Mat(_guiSize, CV_8UC3, BKGRD_COLOR); // (size(width, height), type)
}


void AsteroidBelt::run()
{
    while(!_leave)
    {
        draw();
        update();
    }
    //std::cout << "Hello world!" << std::endl; // this is more just a keepsake
}

void AsteroidBelt::draw()
{
    //check for key input
    if( (_userInput = cv::waitKey(INPUT_DELAY)) ) {} // wwaitkey() likes to chill in an if-statement

    // clear the _canvas
    _canvas = BKGRD_COLOR;

    //draws the updated objects screen, based on updated values... how do i draw the asteroid?
    for (const asteroid& ast : _Asteroids) // const so asteroid doesn't get changed (get, no sets)
        cv::circle(_canvas, ast.getPosition(), ast.getRadius(), ast.getColor(), 5); // Draw each asteroid

    // Display the canvas
    cv::imshow(WINDOW_NAME, _canvas);
}

void AsteroidBelt::update()
{
    if(_userInput == 'l')
        _leave = true;

    moveAsteroids();
    detectCollisions();
}

void AsteroidBelt::generateAsteroid()
{
    // Create a random number generator
    cv::RNG rng(cv::getTickCount());

    // Generate random radius between 20 and 40
    int rad = rng.uniform(MIN_RADIUS, MAX_RADIUS + 1);

    // Generate random position.x between 0 and _guiSize.width
    int posX = rng.uniform(0, _guiSize.width);
    int posY = HORIZON_Y;
    cv::Point pos = cv::Point(posX, posY);

    // Generate random velocity.x and velocity.y between -5 and 5
    int velX = rng.uniform(MIN_VELOCITY, MAX_VELOCITY + 1);
    int velY = rng.uniform(MIN_VELOCITY, MAX_VELOCITY + 1);
    cv::Point vel = cv::Point(velX, velY);

    // Generate random color
    cv::Scalar col = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));

    // Create the asteroid object
    asteroid newAsteroid(rad, pos, vel, col);

    // Add the new asteroid to the vector
    _Asteroids.push_back(newAsteroid);
}

void AsteroidBelt::moveAsteroids() // gpt totally rocked this one
{
    // Update the position of each asteroid
    for (asteroid& ast : _Asteroids) {
        cv::Point pos = ast.getPosition();
        cv::Point vel = ast.getVelocity();

        // Update the position based on the velocity
        pos.x += vel.x;
        pos.y += vel.y;

        // If the asteroid goes off the edge of the screen, make it appear on the opposite side
        if (pos.x < 0) pos.x = _guiSize.width;
        if (pos.y < 0) pos.y = _guiSize.height;
        if (pos.x > _guiSize.width) pos.x = 0;
        if (pos.y > _guiSize.height) pos.y = 0;

        // Set the new position
        ast.setPosition(pos);
    }
}

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



