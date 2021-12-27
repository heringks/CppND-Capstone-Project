#include "game.h"
#include "snake.h"
#include <cmath>
#include <iostream>
#include <chrono>

// ADDED TO EXTEND THE GAME
Snake::Snake(int grid_width, int grid_height, int max_time_between_meals)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2),
        _maxTimeBetweenMeals(max_time_between_meals*1000)
{
  // Initialize the last time the snake ate
  lastTimeSnakeAte = std::chrono::system_clock::now();
}

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

// CHANGED TO EXTEND THE GAME
void Snake::UpdateHead()
{
  if (size > 0)
  {
    switch (direction) 
    {
      case Direction::kUp:
        head_y -= speed;
        break;

      case Direction::kDown:
        head_y += speed;
        break;

      case Direction::kLeft:
        head_x -= speed;
        break;

      case Direction::kRight:
        head_x += speed;
        break;
    }
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) 
{
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) 
  {
    // Remove the tail from the vector.
    body.erase(body.begin());

    // ADDED TO EXTEND THE GAME
    // If the snake has not eaten within the maximum time between meals, decrease its size by 
    // NOTE: The timing logic was modeled after the code in the Concurrent Traffic Simulation project
    // in functions TrafficLight.cpp and Vehicle.cpp
    long timeSinceSnakeAte = std::chrono::duration_cast<std::chrono::milliseconds>
      (std::chrono::system_clock::now() - lastTimeSnakeAte).count();
    if (timeSinceSnakeAte > _maxTimeBetweenMeals)
    {
      size--;
      if (body.size() > 0)
      {
        body.erase(body.begin());
      }
      lastTimeSnakeAte = std::chrono::system_clock::now();
    }

  } 
  else 
  {
    growing = false;
    size++;

    // ADDED TO EXTEND THE GAME

    // Increase speed according to the game difficulty
    speed += 0.02 + 0.005 * (static_cast<int>(Game::_difficultyLevel));

    // Update the last time the snake ate
    lastTimeSnakeAte = std::chrono::system_clock::now();
  }

  // CHANGED TO EXTEND THE GAME

  // Check if the snake has died.
  if (size == 0)
  {
    alive = false;
  }
  else
  {
    for (auto const &item : body) 
    {
      if (current_head_cell.x == item.x && current_head_cell.y == item.y)
      {
        alive = false;
      }
    }
  }

  if (!alive)
  {
    std::cout << "Snake has died\n";
  }  
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}