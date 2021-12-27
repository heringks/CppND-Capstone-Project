#ifndef FOOD_H
#define FOOD_H

#include <chrono>
#include "SDL.h"
#include "snake.h"

class Food {
 public:

  // Methods
  Food(int grid_width, int grid_height, Snake* snakePtr);
  ~Food();
  void Update();
  float getSpeed() const { return speed; }
  SDL_Point getLocation() const { return location; }
  void setLocation(SDL_Point loc);

 private:

  // Methods
  Snake::Direction SetDirection();
  void RetainPreviousDirection(float &X, float &Y);

  // Attributes
  SDL_Point location;
  float speed{0.0};
  int grid_width;
  int grid_height;
  float floatLocationX;
  float floatLocationY;
  Snake::Direction direction;
  Snake::Direction previousDirection;
  bool retainPreviousDirection;
  std::chrono::time_point<std::chrono::system_clock> lastUpdate;
  Snake* _snake;
};

#endif
