#ifdef _WIN32

#include <windows.h>
#include <conio.h>
#include <ostream>

#include "cross_library.h"

void Console::Sleep(int t){
    ::Sleep(t);
}

void Console::CursorView(bool show){
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;

    SetConsoleCursorInfo(hConsole , &ConsoleCursor);
}

void Console::ScreenClear(){
    system("cls");
}

void Console::HideCursor(){
    Console::CursorView(false);
}
void Console::ShowCursor(){
    Console::CursorView(true);
}

std::ostream& Console::operator << (std::ostream& os, const TextColor& cc) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)cc.code);
    return os;
}

Console::TextColor Console::black(Code::BLACK);
Console::TextColor Console::dark_blue(Code::DARK_BLUE);
Console::TextColor Console::dark_green(Code::DARK_GREEN);
Console::TextColor Console::dark_sky_blue(Code::DARK_SKY_BLUE);
Console::TextColor Console::dark_red(Code::DARK_RED);
Console::TextColor Console::dark_violet(Code::DARK_VIOLET);
Console::TextColor Console::dark_yellow(Code::DARK_YELLOW);
Console::TextColor Console::gray(Code::GRAY);
Console::TextColor Console::dark_gray(Code::DARK_GRAY);
Console::TextColor Console::blue(Code::BLUE);
Console::TextColor Console::green(Code::GREEN);
Console::TextColor Console::sky_blue(Code::SKY_BLUE);
Console::TextColor Console::red(Code::RED);
Console::TextColor Console::violet(Code::VIOLET);
Console::TextColor Console::yellow(Code::YELLOW);
Console::TextColor Console::white(Code::WHITE);

std::ostream& Console::operator<<(std::ostream& os, const GotoXY& gotoxy){
    COORD pos = {gotoxy.x, gotoxy.y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
    return os;
}

#else

#include <iostream>
#include <ostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "cross_library.h"

bool kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }
    return false;
}

int getch() {
    struct termios oldattr, newattr;
    int ch;

    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );

    ch = getchar();

    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );

    return ch;
}

void Console::Sleep(int t){
    usleep(t);
}

void Console::ScreenClear(){
    std::cout << "\e[2J\e[H";
}

void Console::HideCursor(){
    std::cout << "\e[?25l";
}
void Console::ShowCursor(){
    std::cout << "\e[?25h";
}

std::ostream& Console::operator<<(std::ostream& os, const TextColor& cc) {
    return os << "\e[" << (int)cc.code << "m";
}

Console::TextColor Console::black(Code::BLACK);
Console::TextColor Console::dark_blue(Code::DARK_BLUE);
Console::TextColor Console::dark_green(Code::DARK_GREEN);
Console::TextColor Console::dark_sky_blue(Code::DARK_SKY_BLUE);
Console::TextColor Console::dark_red(Code::DARK_RED);
Console::TextColor Console::dark_violet(Code::DARK_VIOLET);
Console::TextColor Console::dark_yellow(Code::DARK_YELLOW);
Console::TextColor Console::gray(Code::GRAY);
Console::TextColor Console::dark_gray(Code::DARK_GRAY);
Console::TextColor Console::blue(Code::BLUE);
Console::TextColor Console::green(Code::GREEN);
Console::TextColor Console::sky_blue(Code::SKY_BLUE);
Console::TextColor Console::red(Code::RED);
Console::TextColor Console::violet(Code::VIOLET);
Console::TextColor Console::yellow(Code::YELLOW);
Console::TextColor Console::white(Code::WHITE);

std::ostream& Console::operator<<(std::ostream& os, const GotoXY& gotoxy){
    return os << "\e[" << gotoxy.y + 1 << ";" << gotoxy.x + 1 << "f";
}

#endif

