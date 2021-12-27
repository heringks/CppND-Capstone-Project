#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "food.h"

// ADDED TO EXTEND THE GAME
enum difficultyLevel {basic, intermediate, advanced};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);

  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);

  // CHANGED TO EXTEND THE GAME
  float GetScore() const;

  int GetSize() const;

  // ADDED TO EXTEND THE GAME
  static void GetUsersChoice();
  static difficultyLevel _difficultyLevel;
  static int _maxTimeBetweenMeals; // in seconds

 private:
  
  // ADDED TO EXTEND THE GAME
  std::size_t _grid_width;
  std::size_t _grid_height;

  Snake snake;
  Food food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  float score{0.0};

  void PlaceFood();
  void Update();
};

#endif