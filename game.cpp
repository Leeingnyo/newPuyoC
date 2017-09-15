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

void Game::DrawFrame(int offset_x, int offset_y) {
    std::cout << Console::green;
    std::cout << Console::GotoXY(X(0 + offset_x), Y(0 + offset_y));
    std::cout << "┌──  ───┐┌───┐";
    for (int i = 1; i < 13; i++) {
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
void Game::DrawBoard(const std::shared_ptr<Board> &board, int offset_x, int offset_y) {
    board->Draw(1 + offset_x, 1 + offset_y);
}
void Game::DrawNextBiPuyo(const std::shared_ptr<BiPuyo> &bipuyo, int offset_x, int offset_y) {
    bipuyo->Draw(10 + offset_x, 3 + offset_y);
}
void Game::DrawPlayerInformation(const PlayerInformation &info, int offset_x, int offset_y) {
    std::cout << Console::white;
    std::cout << Console::GotoXY(X(9 + offset_x), Y(5 + offset_y)) << std::right << std::setw(6) << info.chain_number;
    std::cout << Console::GotoXY(X(9 + offset_x), Y(7 + offset_y)) << std::right << std::setw(6) << info.max_chain_number;
    std::cout << Console::GotoXY(X(9 + offset_x), Y(9 + offset_y)) << std::right << std::setw(6) << info.score;
    std::cout << Console::GotoXY(X(9 + offset_x), Y(12 + offset_y)) << std::right << std::setw(6) << info.obstacle_number_taken;
}
void Game::Draw(const std::shared_ptr<Board> &board, const std::shared_ptr<BiPuyo> &bipuyo, const PlayerInformation &info, int offset_x, int offset_y) {
    Game::DrawFrame(offset_x, offset_y);
    Game::DrawBoard(board, offset_x, offset_y);
    Game::DrawNextBiPuyo(bipuyo, offset_x, offset_y);
    Game::DrawPlayerInformation(info, offset_x, offset_y);
}

bool SingleGame::GameInit() {
    Console::ScreenClear();
    std::cout << Console::GotoXY(X(0), Y(0));
    
    // prepare game
    my_bipuyo_generator = std::make_shared<BiPuyoGenerator>(); // single game doesn't need sync seed
    my_board = std::make_shared<Board>();
    my_board->SetNextBiPuyo(my_bipuyo_generator->GenerateBipuyo());
    my_next_bipuyo = my_bipuyo_generator->GenerateBipuyo();
    
    std::cout << "Loading..." << std::endl;
    Console::Sleep(500);
    return true;
}
void SingleGame::GameLoop() {
    while (!gameover) {
        if (!my_board->IsBusy()) {
            if (kbhit()) {
                int input = getch();
                bool special = false;
                if (input == 224) {
                    input = getch();
                    special = true;
                }

                if (special) {
                    if (input == 75) {
                        my_board->MoveLeft();
                    }
                    if (input == 77) {
                        my_board->MoveRight();
                    }
                    if (input == 80) {
                        my_board->MoveDown();
                    }
                } else {
                    if (input == 'j' || input == 'h') {
                        my_board->MoveLeft();
                    }
                    if (input == 'l') {
                        my_board->MoveRight();
                    }
                    if (input == 'k') {
                        my_board->MoveDown();
                    }
                    if (input == '-') {
                        break;
                    }
                    if (input == 'z') { // rotate
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
        if (my_board->IsNeedNext()) {
            // need next bipuyo
            my_board->SetNextBiPuyo(my_next_bipuyo);
            // set next bipuyo
            my_next_bipuyo = my_bipuyo_generator->GenerateBipuyo();
        }
        gameover = my_board->IsGameOver();
        // process
        
        Draw();
        // draw
        
        Console::Sleep(16); // ?
    }
}
void SingleGame::Draw() {
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
    Game::Draw(my_board, my_next_bipuyo, my_info, offset_x, offset_y);
}

bool VSGame::GameInit() {
    Console::ScreenClear();
    std::cout << Console::GotoXY(X(0), Y(0));

    // prepare game
    int seed = time(NULL);
    my_bipuyo_generator = std::make_shared<BiPuyoGenerator>(seed);
    my_board = std::make_shared<Board>();
    my_board->SetNextBiPuyo(my_bipuyo_generator->GenerateBipuyo());
    my_next_bipuyo = my_bipuyo_generator->GenerateBipuyo();
    other_bipuyo_generator = std::make_shared<BiPuyoGenerator>(seed);
    other_board = std::make_shared<Board>();
    other_board->SetNextBiPuyo(other_bipuyo_generator->GenerateBipuyo());
    other_next_bipuyo = other_bipuyo_generator->GenerateBipuyo();

    std::cout << "Loading..." << std::endl;
    Console::Sleep(500);
    return true;
}
void VSGame::GameLoop() {
    while (!gameover) {
        
        if (kbhit()) {
            int input = getch();
            bool special = false;
            if (input == 224) {
                input = getch();
                special = true;
            }

            if (!my_board->IsBusy()) {
                if (special) { }
                else {
                    if (input == 'd') {
                        my_board->MoveLeft();
                    }
                    if (input == 'g') {
                        my_board->MoveRight();
                    }
                    if (input == 'f') {
                        my_board->MoveDown();
                    }
                    if (input == 'z') { // rotate
                        my_board->RotateCCW();
                    }
                }
            }
            if (!other_board->IsBusy()) {
                if (special) { }
                else {
                    if (input == 'l') {
                        other_board->MoveLeft();
                    }
                    if (input == '\'') {
                        other_board->MoveRight();
                    }
                    if (input == ';') {
                        other_board->MoveDown();
                    }
                    if (input == 'm') {
                        other_board->RotateCCW();
                    }
                }
            }
        }
        // input
        
        my_board->Update();
        my_board->UpdatePlayerInformation(my_info);
        if (my_board->IsNeedNext()) {
            my_board->SetNextBiPuyo(my_next_bipuyo);
            my_next_bipuyo = my_bipuyo_generator->GenerateBipuyo();
        }
        other_board->Update();
        other_board->UpdatePlayerInformation(other_info);
        if (other_board->IsNeedNext()) {
            other_board->SetNextBiPuyo(other_next_bipuyo);
            other_next_bipuyo = other_bipuyo_generator->GenerateBipuyo();
        }
        gameover = my_board->IsGameOver() || other_board->IsGameOver();
        // process
        
        Draw();
        Console::Sleep(16);
    }
}
void VSGame::Draw() {
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
    Game::Draw(my_board, my_next_bipuyo, my_info);
    for (int i = 0; i <= 14; i++) {
        std::cout << Console::GotoXY(X(13), Y(i)) << "  ";
    }
    Game::Draw(other_board, other_next_bipuyo, other_info, 14);
}

bool VSRemoteGame::GameInit() {
    int input;
    
    Console::ScreenClear();
    std::cout << Console::GotoXY(X(0), X(0));
    std::cout << "하실 것을 골라주세요" << std::endl;
    std::cout << "0. 돌아가기" << std::endl;
    std::cout << "1. 서버" << std::endl;
    std::cout << "2. 클라" << std::endl;
    input = getch();
    
    switch (input) {
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
void VSRemoteGame::GameLoop() {
    while (true) {
        // recv
        
        if (kbhit()) {
            int input = getch();
            bool special = false;
            if (input == 224) {
                input = getch();
                special = true;
            }
            std::cout << Console::GotoXY(X(0), Y(15))
                << (char)input;
                
            if (input == 'x') {
                break;
            }
        }
        
        Draw();
        Console::Sleep(16); // ?
        
        // send
    }
}
void VSRemoteGame::Draw() {
    VSGame::Draw();
}
