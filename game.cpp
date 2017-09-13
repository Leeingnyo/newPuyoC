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

void Game::DrawFrame(int offset_x, int offset_y){
    std::cout << Console::green;
    std::cout << Console::GotoXY(X(0 + offset_x), Y(0 + offset_y));
    std::cout << "┌──  ───┐┌───┐";
    for (int i = 1; i < 13; i++){
        std::cout << Console::GotoXY(X( 0 + offset_x), Y(i + offset_y)) << "│";
        std::cout << Console::GotoXY(X( 7 + offset_x), Y(i + offset_y)) << "││";
        std::cout << Console::GotoXY(X(12 + offset_x), Y(i + offset_y)) << "│";
    }
    std::cout << Console::GotoXY(X(0 + offset_x), Y(13 + offset_y));
    std::cout << "└──────┘└───┘";
    std::cout << Console::GotoXY(X( 9 + offset_x), Y( 1 + offset_y)) << " NEXT ";
    std::cout << Console::GotoXY(X( 9 + offset_x), Y( 2 + offset_y)) << "  " << Console::GotoXY(X(11 + offset_x), Y( 2 + offset_y)) << "  ";
    std::cout << Console::GotoXY(X( 9 + offset_x), Y( 3 + offset_y)) << "  " << Console::GotoXY(X(11 + offset_x), Y( 3 + offset_y)) << "  ";
    std::cout << Console::GotoXY(X( 9 + offset_x), Y( 4 + offset_y)) << " Chain";
    std::cout << Console::GotoXY(X( 9 + offset_x), Y( 6 + offset_y)) << "MaxChn";
    std::cout << Console::GotoXY(X( 9 + offset_x), Y( 8 + offset_y)) << " Score";
    std::cout << Console::GotoXY(X( 9 + offset_x), Y(10 + offset_y)) << " BGM  ";
    std::cout << Console::GotoXY(X( 9 + offset_x), Y(11 + offset_y)) << "ObsNum";
    
    /*
        0                   1
        0 1 2 3 4 5 6 7 8 9 0 1 2 
    00  ┌──  ───┐┌───┐ 
     1  │            ││ NEXT │ 
     2  │            ││      │ 
     3  │            ││      │ 
     4  │            ││ Chain│ 
     5  │            ││      │ 
     6  │            ││MaxChn│ 
     7  │            ││      │ 
     8  │            ││ Score│ 
     9  │            ││      │ 
    10  │            ││BGM   │ 
     1  │            ││ObsNum│ 
     2  │            ││      │ 
     3  └──────┘└───┘ 
    */
}

bool SingleGame::GameInit(){
    Console::ScreenClear();
    std::cout << Console::GotoXY(X(0), Y(0));
    
    // 무슨 색? 
    puyo_color_random = std::make_shared<WELLRNG512Generator>(rand());
    
    my_board = std::make_shared<Board>();
    my_board->SetNextBiPuyo(GenerateBipuyo());
    
    my_next_bipuyo = GenerateBipuyo();
    
    std::cout << "Loading..." << std::endl;
    Console::Sleep(500);
    return true;
}
void SingleGame::GameLoop(){
    while (!gameover) {
        if (!my_board->IsBusy()){
            if (kbhit()){
                int input = getch();
                bool special = false;
                if (input == 224) {
                    input = getch();
                    special = true;
                }
                    
                if (special){
                    if (input == 75){
                        my_board->MoveLeft();
                    }
                    if (input == 77){
                        my_board->MoveRight();
                    }
                    if (input == 80){
                        my_board->MoveDown();
                    }
                } else {
                    if (input == 'j' || input == 'h'){
                        my_board->MoveLeft();
                    }
                    if (input == 'l'){
                        my_board->MoveRight();
                    }
                    if (input == 'k'){
                        my_board->MoveDown();
                    }
                    if (input == '-'){
                        break;
                    }
                    if (input == 'z'){ // rotate
                        my_board->RotateCCW();
                    }
                }
            }
        }
        else {
            if (kbhit()) getch();
        }
        // input
        
        my_board->Update();
        my_board->UpdatePlayerInformation(my_info);
        if (my_board->IsNeedNext()){
            // need next bipuyo
            my_board->SetNextBiPuyo(my_next_bipuyo);
            // set next bipuyo
            my_next_bipuyo = GenerateBipuyo();
        }
        gameover = my_board->IsGameOver();
        // process
        
        Draw();
        // draw
        
        Console::Sleep(16); // ?
    }
}
void SingleGame::Draw(){
    int offset_x = 0;
    int offset_y = 0;
    /*
        0                   1
        0 1 2 3 4 5 6 7 8 9 0 1 2 
    00  ┌──  ───┐┌───┐ 
     1  │            ││ NEXT │ 
     2  │            ││      │ 
     3  │            ││      │ 
     4  │            ││ Chain│ 
     5  │            ││      │ 
     6  │            ││MaxChn│ 
     7  │            ││      │ 
     8  │            ││ Score│ 
     9  │            ││      │ 
    10  │            ││BGM   │  
     1  │            ││ObsNum│ 
     2  │            ││      │ 
     3  └──────┘└───┘ 
    */
    Game::DrawFrame(0 + offset_x, 0 + offset_y);
    my_board->Draw(1 + offset_x, 1 + offset_y);
    my_next_bipuyo->Draw(10 + offset_x, 3 + offset_y);
    std::cout << Console::white;
    std::cout << Console::GotoXY(X(9), Y(5)) << std::right << std::setw(6) << my_info.chain_number;
    std::cout << Console::GotoXY(X(9), Y(7)) << std::right << std::setw(6) << my_info.max_chain_number;
    std::cout << Console::GotoXY(X(9), Y(9)) << std::right << std::setw(6) << my_info.score;
    std::cout << Console::GotoXY(X(9), Y(12)) << std::right << std::setw(6) << my_info.obstacle_number_taken;
}

bool VSGame::GameInit(){
    Console::ScreenClear();
    std::cout << Console::GotoXY(X(0), Y(0));
    std::cout << "Loading..." << std::endl;
    Console::Sleep(500);
    return true;
}
void VSGame::GameLoop(){
    while (true) {
        
        if (kbhit()){
            int input = getch();
            bool special = false;
            if (input == 224) {
                input = getch();
                special = true;
            }
            std::cout << Console::GotoXY(X(0), Y(15))
                << (char)input;
                
            if (input == 'x'){
                break;
            }
        }
        
        Draw();
        Console::Sleep(16);
    }
}
void VSGame::Draw(){
    DrawFrame();
}
void VSGame::DrawFrame(){
    Game::DrawFrame(0, 0);
    for (int i = 0; i <= 14; i++){
        std::cout << Console::GotoXY(X(13), Y(i)) << "  ";
    }
    Game::DrawFrame(14, 0);
    /*
        0                   1                   2 
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6
    00  ┌──  ───┐┌───┐  ┌──  ───┐┌───┐ 
     1  │            ││ NEXT │  │            ││ NEXT │ 
     2  │            ││      │  │            ││      │ 
     3  │            ││      │  │            ││      │ 
     4  │            ││      │  │            ││      │ 
     5  │            ││ Chain│  │            ││ Chain│ 
     6  │            ││      │  │            ││      │ 
     7  │            ││MaxChn│  │            ││MaxChn│ 
     8  │            ││      │  │            ││      │ 
     9  │            ││ Score│  │            ││ Score│ 
    10  │            ││      │  │            ││      │ 
     1  │            ││BGM   │  │            ││BGM   │ 
     2  │            ││ObsNum│  │            ││ObsNum│ 
     3  │            ││      │  │            ││      │ 
     4  └──────┘└───┘  └──────┘└───┘ 
    */
}
void VSGame::DrawBoard(){
    
}

bool VSRemoteGame::GameInit(){
    int input;
    
    Console::ScreenClear();
    std::cout << Console::GotoXY(X(0), X(0));
    std::cout << "하실 것을 골라주세요" << std::endl;
    std::cout << "0. 돌아가기" << std::endl;
    std::cout << "1. 서버" << std::endl;
    std::cout << "2. 클라" << std::endl;
    input = getch();
    
    switch (input){
        case '0': return false;
        case '1': break;
        case '2': break;
        default: return false;
    }
    // 서버 인지 클라 인지 
    // 서버 일 경우 연결을 만들고 자기 아이피 주소를 보여줌 
    // 클라일 경우 IP 주소 입력받게 함 
    
    // 연결되면 씨드 주고 받음 
    // 여러 가지 이니셜라이징 
    
    Console::ScreenClear();
    std::cout << Console::GotoXY(X(0), X(0));
    std::cout << "Loading..." << std::endl;
    Console::Sleep(500);
    return true;
}
void VSRemoteGame::GameLoop(){
    while (true) {
        // recv
        
        if (kbhit()){
            int input = getch();
            bool special = false;
            if (input == 224) {
                input = getch();
                special = true;
            }
            std::cout << Console::GotoXY(X(0), Y(15))
                << (char)input;
                
            if (input == 'x'){
                break;
            }
        }
        
        Draw();
        Console::Sleep(16); // ?
        
        // send
    }
}
void VSRemoteGame::Draw(){
    VSGame::Draw();
}
