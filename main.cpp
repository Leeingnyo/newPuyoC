#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>

#include "common.h"
#include "cross_socket.h"
#include "cross_library.h"
#include "WELLRNG512Gen.h"

#include "player_information.h"
#include "puyo.h"
#include "board.h"
#include "game.h"

void help();

int main() {
    srand(time(NULL));
    Console::ScreenClear();
    Console::HideCursor();
    if (Socket::StartUp()) {
        std::cout << "Winsock 초기화 실패" << std::endl;
        return -1;
    }
    
    bool app_alive = true;
    while (app_alive) {
        int choice;
        std::shared_ptr<Game> game;
        
        Console::ScreenClear();
        std::cout << Console::GotoXY(X(0), Y(0));
        std::cout << Console::white;
        std::cout << "선택해주세요" << std::endl;
        std::cout << "0. 게임설명" << std::endl;
        std::cout << "1. 혼자하기" << std::endl;
        std::cout << "2. 둘이하기(같은컴)" << std::endl;
        std::cout << "3. 둘이하기(멀티플레이)" << std::endl;
        std::cout << "4. 끝내기" << std::endl;
        
        choice = getch();
        switch(choice) {
            case '0': {
                help();
            }   continue;
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
        
        if (game->GameInit()) {
            game->GameLoop();
            game->GameResult();
        }
    }
    
    std::cout << Console::gray;
    Console::ShowCursor();
    Console::ScreenClear();
    Socket::CleanUp();
    return 0;
}

void help() {
    Console::ScreenClear();
    std::cout << Console::GotoXY(0, 0);
    std::cout << "# 메뉴 설명   _.- 지금 보고 계신 것" << std::endl;
    std::cout << "0. 게임설명 -` _.-- 혼자 연습하실 수 있습니다. 연쇄를 연습해보자구요!" << std::endl;
    std::cout << "1. 혼자하기 --`  ,- 둘이서 같은 컴퓨터로 대결하실 수 있습니다" << std::endl;
    std::cout << "2. 둘이하기(같은컴)   ,-원격의 상대와 대결하실 수 있습니다" << std::endl;
    std::cout << "3. 둘이하기(멀티플레이)   멀리 떨어진 친구와 ㄱㄱ!" << std::endl;
    std::cout << "4. 끝내기 --- 게임을 끝냅니다 (플레이 해주세요!)" << std::endl;
    std::cout << "(AI와 대결 - 언젠가 구현될 겁니다)" << std::endl;
    std::cout << "(1/3) 아무 키를 눌러주세요" << std::endl;
    getch();
    Console::ScreenClear();
    std::cout << Console::GotoXY(0, 0);
    std::cout << "# 게임 설명" << std::endl;
    std::cout << "'뿌요뿌요'는 일본의 게임 회사 컴파일에서 제작한 낙하형 퍼즐 게임 시리즈 입니다 (위키백과)." << std::endl;
    std::cout << "'뿌요'라는 슬라임 형태의 몬스터가 등장하는데," << std::endl;
    std::cout << "이 몬스터들은 4개 이상이 모이면 터지게 된답니다." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "# 게임 규칙" << std::endl;
    std::cout << "위에서 내려오는 뿌요를 이동하고 회전시켜서 같은 색 4개를 인접시키세요." << std::endl;
    std::cout << "터지게 되면 점수를 얻고 상대방에게 방해뿌요를 보냅니다." << std::endl;
    std::cout << "터지면서 위에 쌓인 뿌요가 내려와서 또 4개가 모여 티질 수 있는데, 이를 연쇄라고 합니다." << std::endl;
    std::cout << "연쇄 수가 많을 수록 점수를 더 얻게 되며 더 많은 방해뿌요를 상대방에게 보냅니다!" << std::endl;
    std::cout << "방해뿌요는 주변의 뿌요가 터지면 사라지게 됩니다." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "그럼 상대방을 보내버립시다!" << std::endl;
    std::cout << "(2/3) 아무 키를 눌러주세요" << std::endl;
    getch();
    Console::ScreenClear();
    std::cout << Console::GotoXY(0, 0);
    std::cout << "# 키 설명" << std::endl;
    std::cout << "## 혼자하기 / 둘이하기(멀티)" << std::endl;
    std::cout << "*-------------------------------*" << std::endl;
    std::cout << "| 왼쪽 | 아래로 | 오른쪽 | 회전 |" << std::endl;
    std::cout << "| h, j |   k    |   l    |  z   |" << std::endl;
    std::cout << "*-------------------------------*" << std::endl;
    std::cout << "(h for vim user)" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "## 둘이하기(같은컴)" << std::endl;
    std::cout << "### 1p" << std::endl;
    std::cout << "*-------------------------------*" << std::endl;
    std::cout << "| 왼쪽 | 아래로 | 오른쪽 | 회전 |" << std::endl;
    std::cout << "|  d   |   f    |   g    |  z   |" << std::endl;
    std::cout << "*-------------------------------*" << std::endl;
    std::cout << "### 2p" << std::endl;
    std::cout << "*-------------------------------*" << std::endl;
    std::cout << "| 왼쪽 | 아래로 | 오른쪽 | 회전 |" << std::endl;
    std::cout << "|  l   |   ;    |   '    |  ,   |" << std::endl;
    std::cout << "*-------------------------------*" << std::endl;
    std::cout << "(3/3) 아무 키를 눌러주세요" << std::endl;
    getch();
}
