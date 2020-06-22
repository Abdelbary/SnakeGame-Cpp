#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <fstream>
#include <sstream>
int main() {
 

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  
  //add current score to score file
  std::ofstream out("scores.txt",std::ios_base::app);
  if(out.is_open())
  {
  out << game.GetScore();
  out << "\n\r";
  out.close();
  }
  else
  {
    std::cout<<"error openning score file\n";
  }
  
  
  //get the higest score
  std::ifstream stream("scores.txt");
  std::istringstream linestream;
  int highestScore = 0;
  std::string line="";
  if(stream.is_open())
  {
    int score{0};

    while(std::getline(stream,line))
    {
       linestream.str(line);
       linestream>>score;
       if(score > highestScore)
        highestScore = score;
    };

    stream.close();
  }
  else 
  {
        std::cout<<"error openning score file\n";
  }
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  std::cout << "Highest Score: " << highestScore << "\n";

  return 0;
}