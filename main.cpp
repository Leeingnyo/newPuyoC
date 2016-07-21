#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <memory>
#include <vector>

#include "common.h"
#include "cross_library.h"
#include "WELLRNG512Gen.h"

#include "player_information.h"
#include "puyo.h"
#include "board.h"
#include "game.h"

int main(){
    srand(time(NULL));
    Console::ScreenClear();
    Console::HideCursor();
    
    bool app_alive = true;
    while (app_alive){
        int choice;
        std::shared_ptr<Game> game;
        
        Console::ScreenClear();
        std::cout << Console::GotoXY(X(0), Y(0));
        std::cout << Console::white;
        std::cout << "�������ּ���" << std::endl;
        std::cout << "1. ȥ���ϱ�" << std::endl;
        std::cout << "2. �����ϱ�(������)" << std::endl;
        std::cout << "3. �����ϱ�(��Ƽ�÷���)" << std::endl;
        std::cout << "4. ������" << std::endl;
        
        choice = getch();
        switch(choice){
            case '1': {
                game = std::make_shared<SingleGame>();
            }   break;
            case '2': {
                game = std::make_shared<VSGame>();
            }   break;
            case '3': {
                game = std::make_shared<VSRemoteGame>();
            }   break;
            case '4': {
                app_alive = false;
            }   continue;
            default: continue;
        }
        
        if (game->GameInit())
            game->GameLoop();
    }
    
    std::cout << Console::gray;
    Console::ShowCursor();
    Console::ScreenClear();
    return 0;
}
