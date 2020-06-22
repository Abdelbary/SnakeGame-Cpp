#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) 
      ,score(0)
      ,currentFoodCount(0)
      ,foodCapacity(5)
      ,shrinkFoddProbability(0.1){
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
bool Game::FoodCell(int x, int y) const
{
  for (auto const &f : food) {
    if (x == f.first.x && y == f.first.y) {
      return true;
    }
  }
  return false;
}

void Game::PlaceFood() {
  int x, y;
  for(; currentFoodCount < foodCapacity;currentFoodCount++)
  {
    while (true) {
      x = (random_w(engine)%kScreenWidth)-1;
      y = (random_h(engine)%kScreenHeight)-1;
      // Check that the location is not occupied by a snake item before placing
      // food.
      if (!snake.SnakeCell(x, y) && !this->FoodCell(x,y) ) {
        FoodType ft = (rand()%101 <= (shrinkFoddProbability*100))? FoodType::Shrink : FoodType::Normal;
        food.push_back({{x,y},ft});
        
        break;
    }
  }
  
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  for(auto f = food.begin() ; f != food.end();f++)
    if (f->first.x == new_x && f->first.y == new_y) {
      score++;
      currentFoodCount--;
       //check for what kind of food
      if(f->second == FoodType::Normal)
      {
        // Grow snake and increase speed.
        snake.GrowBody();
        snake.speed += 0.02;
        std::cout<<"GROW\n";
      }
      else if(f->second == FoodType::Shrink)
      {
        snake.ShrinkBody();
        snake.speed = 0.1f;
        std::cout<<"SHRINK\n";
        PlaceKiller(); //as the player choose to eat the shrink bill the price is to add a killer
        //premenamte bill
      }
      else if(f->second == FoodType::Killer)
          snake.alive  = false; //kill the snake

      food.erase(f);
      PlaceFood();
      break;
    }
}
void Game::PlaceKiller() {
  //for each shrink bill add a killer bill
  int x,y;
  while(true)
  {
    x = (random_w(engine)%kScreenWidth)-1;
    y = (random_h(engine)%kScreenHeight)-1;
    if (!snake.SnakeCell(x, y) && !this->FoodCell(x,y) ) 
    {
      food.push_back({{x,y},FoodType::Killer});
      break;
    } 
  }
}
int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }
