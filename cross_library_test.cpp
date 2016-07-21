#include <iostream>
#include <cstdio>

#include "cross_library.h"

// CrossLibraryTest
int main(){
    Console::ScreenClear();
    Console::HideCursor();
    bool a = true;
    while (a){
        if (kbhit()){
            char d = getch();
            std::cout << d;
            if (d == 'x') a = false;
            fflush(stdin);
        }
    }
    a = true;
    while (a){
        std::cout << Console::GotoXY(5, 5) << Console::red << "아아우아" << Console::GotoXY(0, 0) << kbhit();
        if (kbhit()){
            char c = getch();
            std::cout << Console::GotoXY(0, 1) << c << Console::GotoXY(0, 0);
            a = false;
        }
    }
    std::cout << Console::gray;
    Console::ShowCursor();
    Console::ScreenClear();
    return 0;
}
