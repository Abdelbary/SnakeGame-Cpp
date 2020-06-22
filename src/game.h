#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
constexpr std::size_t kFramesPerSecond{60};
constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
constexpr std::size_t kScreenWidth{700};
constexpr std::size_t kScreenHeight{500};
constexpr std::size_t kGridWidth{50};
constexpr std::size_t kGridHeight{40};



class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  bool FoodCell(int x , int y)const;
  int GetScore() const;
  int GetSize() const;
  void PlaceKiller();
 private:
  Snake snake;
  std::vector<std::pair<SDL_Point,FoodType> > food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int foodCapacity{5};
  int currentFoodCount;
  int score;
  double shrinkFoddProbability;
  void PlaceFood();
  void Update();
};

#endif
