# CppND-Capstone-Project

My C++ Capstone Project is extending the Snake Game provided in the Capstone Project starter code with additional features. Summary of the additional features provided:

1) Change the game to allow 3 levels of diffuculty: 
  a) Basic: the Snake Game as it is in the starter code; 
  b) Intermediate: 
    i) The food moves at half the speed of the snake. Movement will always be in the same direction.
    ii) Each time the snake eats, its speed will be increased by +0.025 (in the "Basic" level, speed is increased by +0.02.
  c) Advanced: 
    i) The food moves at 3/4 the speed of the snake, generally away from the snake. The direction of the food is determined by the direction of the snake and the food's location relative to the snake's location.
    ii) Each time the snake eats, its speed will be increased by +0.03
3) The number of points earned by the snake eating the food will be a relative value based on the difficulty level chosen by the user and the speed of the food.
4) Each time the snake has not eaten within a maximum amount of time determined by the user, its body will be decreased by 1 square. If it gets to the point in which there is no body, the snake will die.
5) User input will be added, prompting the user to select the diffculty level the user desires to play the game at and the maximum amount of time the snake can go between meals. In addition, a "welcome file" is provided, which explains the "jist" of the game to the user and defines the three difficulty levels. The program reads the "welcome file" and displays it to the screen. This is done prior to prompting the use to select the diffiulty level and the max time between meals.

The original README.md file for the startup code was renamed to README_SC.md. The basic setup, including the dependency on the SDL library, was unchanged. 

## Overview of the Code Structure

See the Power-Point presentation Snake_Game_Code_Walkthrough.pptx in the project repo.

food.h/cpp are new files that were added to the starter code. food.cpp was added to CMakeLists.txt to bring it into the build. Other than food.h/cpp, all the changes or additions to the other code files are denoted by one of the following prefacing comments:

// ADDED TO EXTEND THE GAME

// CHANGED TO EXTEND THE GAME

// CHANGED TO MEET RUBRIC ITEM ...

## Explanation of how project submission satisfies the necessary rubric: 

See Rubric_traceability.xlsx in the project repo.

## Basic Build Instructions (copied from README_SC.md):

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.
