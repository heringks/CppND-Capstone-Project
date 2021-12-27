#include <iostream>
#include <math.h>
#include <string>
#include "game.h"
#include "snake.h"
#include "food.h"

Food::Food(int grid_width, int grid_height, Snake* snakePtr) 
    : grid_width(grid_width), 
      grid_height(grid_height),
      retainPreviousDirection(false)
{
    try
    {
        if (snakePtr != nullptr)
            _snake = snakePtr;
        else
            throw (std::runtime_error("Either pointer to Snake object is invalid or there is no Snake object\n"));
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what();
        EXIT_FAILURE;
    }

}

Food::~Food()
{
}

void Food::Update()
{
    switch (Game::_difficultyLevel)
    {
        case (basic):
            // Do nothing. Food is stationary in the "basic" level
            break;
        case (intermediate):
            // Set the speed of the food to 1/2 that of the snake
            speed = _snake->getSpeed()/2.0;

            // Set direction even though not used in the "intermediate" level
            direction = Snake::Direction::kRight;

            // Update the location of the food. Food moves in a constant direction
            floatLocationX += speed;

            // Wrap the food around to the beginning if going off of the screen.
            floatLocationX = fmod(floatLocationX + grid_width, grid_width);
            location.x = static_cast<int>(floatLocationX);

            break;
        case (advanced):
            // Set the speed of the food to 3/4 that of the snake
            speed = _snake->getSpeed()*0.75;

            // If the previous direction should not be retained 
            /* NOTE: It is necessary to retain the previous direction of the food for a brief period of time in cases 
            ** when the locaton of the food wraps around the screen. When the location wraps around to the other side, 
            ** the location relative to the snake is reversed, which causes the direction determined for the food to go 
            ** in to be reversed. On the display, this causes the food to appear stuck infinitely alternating between 
            ** the edges of the screen (either top and bottom, or left and right).
            */
            if (!retainPreviousDirection) 
            {
                // Determine the direction the food should go in based on the snake's direction and the
                // food's location relative to the snake's location
                direction = SetDirection();
            }

            // Update the location of the food. The food generally moves away from the snake.
            switch (direction) 
            {
                case Snake::Direction::kUp:
                    floatLocationY -= speed;
                    break;

                case Snake::Direction::kDown:
                    floatLocationY += speed;
                    break;

                case Snake::Direction::kLeft:
                    floatLocationX -= speed;
                    break;

                case Snake::Direction::kRight:
                    floatLocationX += speed;
                    break;
            }

            // Determine if the previous direction of the food should be retained
            RetainPreviousDirection(floatLocationX, floatLocationY);

            // Wrap the food around to the beginning if going off of the screen.
            floatLocationX = fmod(floatLocationX + grid_width, grid_width);
            floatLocationY = fmod(floatLocationY + grid_height, grid_height);

            location.x = static_cast<int>(floatLocationX);
            location.y = static_cast<int>(floatLocationY);
            break;
    }
}

Snake::Direction Food::SetDirection()
{
    enum FoodToSnakeRelationship
    {
        kLeft_Above,    // Food to the left and above the snake
        kEqual_Above,   // Food above the snake, equal in x-coordinate
        kRight_Above,   // Food to the right and above the snake
        kLeft_Equal,    // Food to the left of the snake, equal in y-coordinate
        kRight_Equal,   // Food to the right of the snake, equal in y-coordinate
        kLeft_Below,    // Food to the left and below the snake
        kEqual_Below,   // Food below the snake, equal in x-coordinate
        kRight_Below    // Food to the right and below the snake
    };

    FoodToSnakeRelationship foodToSnakeRelationship;
    Snake::Direction directionLoc;
 
    Snake::Direction foodDirection[sizeof(Snake::Direction)][8] =
    {
        // 1st row: _snake->getDirection() = kUP
        {   Snake::Direction::kUp,      // Food to the left and above the snake
            Snake::Direction::kUp,      // Food above the snake and equal in x-coordinate
            Snake::Direction::kUp,      // Food to the right and above the snake
            Snake::Direction::kDown,    // Food to the left of the snake and equal in y-coordinate
            Snake::Direction::kDown,    // Food to the right of the snake and equal in y-coordinate
            Snake::Direction::kDown,    // Food to the left and below the snake
            Snake::Direction::kUp,      // Food below the snake and equal in x-coordinate
            Snake::Direction::kDown,    // Food to the right and below the snake
        },
        // 2nd row: _snake->getDirection() = kDown
        {   Snake::Direction::kUp,      // Food to the left and above the snake
            Snake::Direction::kDown,    // Food above the snake and equal in x-coordinate
            Snake::Direction::kUp,      // Food to the right and above the snake
            Snake::Direction::kUp,      // Food to the left of the snake and equal in y-coordinate
            Snake::Direction::kUp,      // Food to the right of the snake and equal in y-coordinate
            Snake::Direction::kDown,    // Food to the left and below the snake
            Snake::Direction::kDown,    // Food below the snake and equal in x-coordinate
            Snake::Direction::kDown,    // Food to the right and below the snake
        },
        // 3rd row: _snake->getDirection() = kLeft
        {   Snake::Direction::kLeft,    // Food to the left and above the snake
            Snake::Direction::kRight,   // Food above the snake and equal in x-coordinate
            Snake::Direction::kRight,   // Food to the right and above the snake
            Snake::Direction::kLeft,    // Food to the left of the snake and equal in y-coordinate
            Snake::Direction::kLeft,    // Food to the right of the snake and equal in y-coordinate
            Snake::Direction::kLeft,    // Food to the left and below the snake
            Snake::Direction::kRight,   // Food below the snake and equal in x-coordinate
            Snake::Direction::kRight,   // Food to the right and below the snake
        },
        // 4th row: _snake->getDirection() = kRight
        {   Snake::Direction::kLeft,    // Food to the left and above the snake
            Snake::Direction::kLeft,    // Food above the snake and equal in x-coordinate
            Snake::Direction::kRight,   // Food to the right and above the snake
            Snake::Direction::kRight,   // Food to the left of the snake and equal in y-coordinate
            Snake::Direction::kRight,   // Food to the right of the snake and equal in y-coordinate
            Snake::Direction::kLeft,    // Food to the left and below the snake
            Snake::Direction::kLeft,    // Food below the snake and equal in x-coordinate
            Snake::Direction::kRight,   // Food to the right and below the snake
        }
   };

    // Calculate the x and y coordinates distance between the snake and the food
    int diffX = static_cast<int>(floatLocationX - _snake->getHeadX());
    int diffY = static_cast<int>(floatLocationY - _snake->getHeadY());

    // Determine the relationship between the food's location and the snake's location
    if (diffX < 0)
    {
        if (diffY < 0)
        {
            foodToSnakeRelationship = kLeft_Above;
        }
        else if (diffY = 0)
        {
            foodToSnakeRelationship = kLeft_Equal;
        }
        else
        {
            foodToSnakeRelationship = kLeft_Below;
        }
    }
    else if (diffX = 0)
    {
        if (diffY < 0)
        {
            foodToSnakeRelationship = kEqual_Above;
        }
        else    // If both diffX and diffY are 0, then the snake has eaten the food
        {
            foodToSnakeRelationship = kEqual_Below;
        }        
    }
    else
    {
        if (diffY < 0)
        {
            foodToSnakeRelationship = kRight_Above;
        }
        else if (diffY = 0)
        {
            foodToSnakeRelationship = kRight_Equal;
        }
        else
        {
            foodToSnakeRelationship = kRight_Below;
        }
    }

    // Use the lookup matrix based on the direction of the snake and the relationship between the food's
    // location and the snake's location to determine the direction of the food
    directionLoc = foodDirection[static_cast<int>(_snake->getDirection())][static_cast<int>(foodToSnakeRelationship)];
    return directionLoc;
}

void Food::RetainPreviousDirection(float &X, float &Y)
{
    // If the new food location just computed is off the screen
    if ((X < 0.0) || (X > grid_width) || (Y < 0.0) || (Y > grid_height))
    {
        // Indicate the previously determined direction should be retained
        retainPreviousDirection = true;

        // Change the direction of the food to the previous direction
        direction = previousDirection;

        // Initialize stop watch
        lastUpdate = std::chrono::system_clock::now();
    }
    else
    {
        // Update the previous direction of the food
        previousDirection = direction;
    }

    // If the previous food location should be retained
    if (retainPreviousDirection)
    {
        // Amount of time the previous direction of the food should be retained
        long kTimeToRetainFoodDirection = 100 / speed; // in ms

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now() - lastUpdate).count();

        // If the amount of time to retain the previous food direction has expired
        if (timeSinceLastUpdate >= kTimeToRetainFoodDirection)
        {
            // Reset the flag to retain the previous food direction
            retainPreviousDirection = false;
        }
    }
}

void Food::setLocation(SDL_Point loc) 
{
    location = loc; // NOTE: random_w and random_h, used in Game::PlaceFood to set the starting location of the food, 
                    // are already limited to the size of the screen. Therefore, using an invariant to limit the location
                    // here is not needed.
    floatLocationX = static_cast<float>(location.x);
    floatLocationY = static_cast<float>(location.y);
}
