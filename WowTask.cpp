#include <iostream>
#include <thread>
#include <chrono>

#include "memory.h"
#include "game.h"


int main()
{
  CGame game(L"Wow.exe");
  game.InitOffsets();

  while (true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    system("cls");
    std::cout << "IsPlayerInGame: " << game.IsPlayerInGame() << std::endl;
    std::cout << "IsPlayerLoading: " << game.IsPlayerLoading() << std::endl;
  }

  system("pause");
}