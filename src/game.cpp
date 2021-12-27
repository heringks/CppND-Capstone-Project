#include "game.h"
#include <iostream>
#include <fstream>
#include "SDL.h"

// CHANGED TO EXTEND THE GAME
Game::Game(std::size_t grid_width, std::size_t grid_height)
    : _grid_width(grid_width),
      _grid_height(grid_height),
      snake(grid_width, grid_height, _maxTimeBetweenMeals),
      food(grid_width, grid_height, &snake),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1))

{
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;

  // ADDED TO EXTEND THE TAME
  SDL_Point foodLocation;

  while (true) {
    x = random_w(engine);
    y = random_h(engine);

    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) 
    {
      // CHANGED TO EXTEND THE GAME
      foodLocation.x = x;
      foodLocation.y = y;
      food.setLocation(foodLocation);
      return;
    }
  }
}

void Game::Update() {
  if (!snake.getAlive()) return;

  snake.Update();

  // ADDED TO EXTEND THE GAME
  food.Update();

  int new_x = static_cast<int>(snake.getHeadX());
  int new_y = static_cast<int>(snake.getHeadY());

  // Check if there's food over here
  // CHANGED TO EXTEND THE GAME
  if (food.getLocation().x == new_x && food.getLocation().y == new_y) 
  {
    // CHANGED TO EXTEND THE GAME
    score = score + 1.0 + static_cast<float>(Game::_difficultyLevel) * 10.0*food.getSpeed();
    std::cout << "score = " << score << "\n";

    PlaceFood();

    // CHANGED TO EXTEND THE GAME -- Determination of the speed of the snake moved into Snake::UpdateBody()
    // Grow snake and increase speed.
    snake.GrowBody();
  }
}

// ADDED TO EXTEND THE GAME
difficultyLevel Game::_difficultyLevel;
int Game::_maxTimeBetweenMeals;
void Game::GetUsersChoice()
{
  int userInputDiffLevel;
  int userInputTimeBetweenMeals;
  std::string line;
  const std::string welcomeFile = "/home/workspace/CppND-Capstone-Snake-Game/Welcome.txt";

  std::ifstream filestream(welcomeFile);
  if (filestream.is_open()) 
  {

    // Read a line of data while there are lines to read
    while (std::getline(filestream, line))
    {
      std::cout << line << "\n";
    }

    // Close the stream
    filestream.close();
    std::cout << "\n";
  }
  else
  {
    std::cout << "ERROR! Could not open the welcome file.\n";
  }
  

  // Prompt user to select the level of difficulty
  std::cout << "\nEnter the level of difficulty you wish to play: \n";
  std::cout << "   1 = Basic\n";
  std::cout << "   2 = Intermediate\n";
  std::cout << "   3 = Advanced\n";
  std::cin >> userInputDiffLevel;
  std::cin.sync();
  std::cin.clear();

  // Prompt user to enter the maximum amount of time the snake can go between meals, in seconds
  std::cout << "Enter the maximum amount of time the snake can go between meals, in seconds.\n"; 
  std::cout << "Please enter an integer no less than 5 seconds and no greater than 30 seconds.\n";
  std::cin >> userInputTimeBetweenMeals;
  std::cin.sync();
  std::cin.clear();

  _maxTimeBetweenMeals = (userInputTimeBetweenMeals < 5) ? 5 : 
    ((userInputTimeBetweenMeals > 30) ? 30 : userInputTimeBetweenMeals);

  switch (userInputDiffLevel) 
  {
    case 1:
      std::cout << "Game starting at Basic Level\n";
      _difficultyLevel =  basic;
      break;

    case 2:
      std::cout << "Game starting at Intermediate Level\n";
      _difficultyLevel =  intermediate;
      break;

    case 3:
      std::cout << "Game starting at Advanced Level\n";
      _difficultyLevel =  advanced;
      break;

    default:
      // User entered an invalid input
      std::cout << "Invalid input. Game starting at Basic Level\n";
      _difficultyLevel =  basic;
      break;
  }
}

// CHANGED TO EXTEND THE GAME
float Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.getSize(); }