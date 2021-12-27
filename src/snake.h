#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <chrono>
#include "SDL.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  // CHANGED TO EXTEND THE GAME
  Snake(int grid_width, int grid_height, int max_time_between_meals);

  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);

  // ADDED TO MEET RUBRIC ITEM "Classes encapsulate behavior" -- Snake's data moved from public to private
  Direction getDirection() const { return direction; }
  int getSize() const { return size; }
  bool getAlive() const { return alive; }
  float getSpeed() const { return speed; }
  float getHeadX() const { return head_x; }
  float getHeadY() const { return head_y; }
  std::vector<SDL_Point> getBody () const { return body; }
  void setDirection(Direction dir) { direction = dir; }

 private:

  // Methods
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  // Attributes

  // CHANGED TO MEET RUBRIC ITEM "Classes encapsulate behavior" -- Snake's data moved from public to private
  Direction direction = Direction::kUp;
  int size{1};
  bool alive{true};
  float speed{0.1f};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

  bool growing{false};

  // ADDED TO EXTEND THE GAME
  std::chrono::time_point<std::chrono::system_clock> lastTimeSnakeAte;
  int _maxTimeBetweenMeals;  // in ms
  
  int grid_width;
  int grid_height;
};

#endif