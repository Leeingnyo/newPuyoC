#ifdef _WIN32

#include <windows.h>
#include <conio.h>

namespace Console {
    
    void CursorView(bool show)
    {
        HANDLE hConsole;
        CONSOLE_CURSOR_INFO ConsoleCursor;
    
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
        ConsoleCursor.bVisible = show;
        ConsoleCursor.dwSize = 1;
    
        SetConsoleCursorInfo(hConsole , &ConsoleCursor);
    }
    
    enum class Code {
        BLACK         = 0,
        DARK_BLUE     = 1,
        DARK_GREEN    = 2,
        DARK_SKY_BLUE = 3,
        DARK_RED      = 4,
        DARK_VIOLET   = 5,
        DARK_YELLOW   = 6,
        GRAY          = 7,
        DARK_GRAY     = 8,
        BLUE          = 9,
        GREEN         = 10,
        SKY_BLUE      = 11,
        RED           = 12,
        VIOLET        = 13,
        YELLOW        = 14,
        WHITE         = 15,
    };
    
    class TextColor {
        Code code = Code::GRAY;
    public:
        TextColor(Code code) : code(code) {}
        
        friend std::ostream& operator << (std::ostream& os, const TextColor& cc) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)cc.code);
            return os;
        }
    };
    
    TextColor black(Code::BLACK);
    TextColor dark_blue(Code::DARK_BLUE);
    TextColor dark_green(Code::DARK_GREEN);
    TextColor dark_sky_blue(Code::DARK_SKY_BLUE);
    TextColor dark_red(Code::DARK_RED);
    TextColor dark_violet(Code::DARK_VIOLET);
    TextColor dark_yellow(Code::DARK_YELLOW);
    TextColor gray(Code::GRAY);
    TextColor dark_gray(Code::DARK_GRAY);
    TextColor blue(Code::BLUE);
    TextColor green(Code::GREEN);
    TextColor sky_blue(Code::SKY_BLUE);
    TextColor red(Code::RED);
    TextColor violet(Code::VIOLET);
    TextColor yellow(Code::YELLOW);
    TextColor white(Code::WHITE);
    
    void ScreenClear(){
        system("cls");
    }

    void HideCursor(){
        CursorView(false);
    }
    void ShowCursor(){
        CursorView(true);
    }
    
    class GotoXY {
        int x, y;
    public:
        GotoXY(){ x = y = 0; }
        GotoXY(int x, int y){ this->x = x; this->y = y; }
        
        friend std::ostream& operator<<(std::ostream& os, const GotoXY& gotoxy);
    };
    std::ostream& operator<<(std::ostream& os, const GotoXY& gotoxy){
        COORD pos = {gotoxy.x, gotoxy.y};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
        return os;
    }
}

#else

#include <ostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
 
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

namespace Console {
    enum class Code {
        BLACK         = 30,
        DARK_BLUE     = 34,
        DARK_GREEN    = 32,
        DARK_SKY_BLUE = 36,
        DARK_RED      = 31,
        DARK_VIOLET   = 35,
        DARK_YELLOW   = 33,
        GRAY          = 37,
        DARK_GRAY     = 90,
        BLUE          = 94,
        GREEN         = 92,
        SKY_BLUE      = 96,
        RED           = 91,
        VIOLET        = 95,
        YELLOW        = 93,
        WHITE         = 97,
    };
    
    class TextColor {
        Code code;
    public:
        TextColor(Code code) : code(code) {}
        
        friend std::ostream& operator<<(std::ostream& os, const TextColor& cc) {
            return os << "\e[" << (int)cc.code << "m";
        }
    };
    
    TextColor black(Code::BLACK);
    TextColor dark_blue(Code::DARK_BLUE);
    TextColor dark_green(Code::DARK_GREEN);
    TextColor dark_sky_blue(Code::DARK_SKY_BLUE);
    TextColor dark_red(Code::DARK_RED);
    TextColor dark_violet(Code::DARK_VIOLET);
    TextColor dark_yellow(Code::DARK_YELLOW);
    TextColor gray(Code::GRAY);
    TextColor dark_gray(Code::DARK_GRAY);
    TextColor blue(Code::BLUE);
    TextColor green(Code::GREEN);
    TextColor sky_blue(Code::SKY_BLUE);
    TextColor red(Code::RED);
    TextColor violet(Code::VIOLET);
    TextColor yellow(Code::YELLOW);
    TextColor white(Code::WHITE);
    
    void ScreenClear(){
        std::cout << "\e[2J\e[H";
    }

    void HideCursor(){
        std::cout << "\e[?25l";
    }
    void ShowCursor(){
        std::cout << "\e[?25h";
    }
    
    class GotoXY {
        int x, y;
    public:
        GotoXY(){ x = y = 0; }
        GotoXY(int x, int y){ this->x = x; this->y = y; }
        
        friend std::ostream& operator<<(std::ostream& os, const GotoXY& gotoxy);
    };
    std::ostream& operator<<(std::ostream& os, const GotoXY& gotoxy){
        return os << "\e[" << gotoxy.y + 1 << ";" << gotoxy.x + 1 << "f";
    }
}

#endif
