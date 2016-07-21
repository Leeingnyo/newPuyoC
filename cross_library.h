#ifdef _WIN32

#include <windows.h>
#include <conio.h>

#else

#include <ostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

bool kbhit();
int getch();

#endif

namespace Console {
    
    void Sleep(int t);
    
    void CursorView(bool show);
    
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
        
        friend std::ostream& operator<<(std::ostream& os, const TextColor& cc);
    };
    std::ostream& operator<<(std::ostream& os, const TextColor& cc);
    
    extern TextColor black;
    extern TextColor dark_blue;
    extern TextColor dark_green;
    extern TextColor dark_sky_blue;
    extern TextColor dark_red;
    extern TextColor dark_violet;
    extern TextColor dark_yellow;
    extern TextColor gray;
    extern TextColor dark_gray;
    extern TextColor blue;
    extern TextColor green;
    extern TextColor sky_blue;
    extern TextColor red;
    extern TextColor violet;
    extern TextColor yellow;
    extern TextColor white;
    
    void ScreenClear();
    
    void HideCursor();
    void ShowCursor();
    
    class GotoXY {
        int x, y;
    public:
        GotoXY(){ x = y = 0; }
        GotoXY(int x, int y){ this->x = x; this->y = y; }
        
        friend std::ostream& operator<<(std::ostream& os, const GotoXY& gotoxy);
    };
    std::ostream& operator<<(std::ostream& os, const GotoXY& gotoxy);
}
