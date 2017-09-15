#pragma once

class WELLRNG512Generator;

enum class PuyoKind : char { BLANK, AKARI, KYOKO, YUI, CHINATSU, AYANO, CHITOSE, SAKURAKO, HIMAWARI, OBSTACLE, EXPLOSION };
                            // NOTHING, RED, YELLOW, BLACK, PINK, PURPLE, WHITE, BRIGHT BROWN, BLUE, TRANSPARENT, GRAY
                            // a    b       c       d       e       f       g       h           i       j           k

class Puyo {
    PuyoKind puyo;
public:
    Puyo() {
        puyo = PuyoKind::BLANK;
    }
    Puyo(PuyoKind puyo) {
        this->puyo = puyo; 
    }
    Puyo(const Puyo& puyo) {
        this->puyo = puyo.puyo;
    }
    void Draw() {
        switch (puyo) {
            case PuyoKind::BLANK:
                std::cout << Console::black << "  ";
                break;
            case PuyoKind::AKARI:
                std::cout << Console::red << "●";
                break;
            case PuyoKind::KYOKO:
                std::cout << Console::yellow << "●";
                break;
            case PuyoKind::YUI:
                std::cout << Console::dark_gray << "●";
                break;
            case PuyoKind::CHINATSU:
                std::cout << Console::violet << "●";
                break;
            case PuyoKind::OBSTACLE:
                std::cout << Console::white << "○";
                break;
            case PuyoKind::EXPLOSION:
                std::cout << Console::gray << "※";
                break; 
            default: break;
        }
    }
    bool IsSameColor(const Puyo& other) {
        return puyo == other.puyo;
    }
    int ToInt() {
        return static_cast<int>(puyo);
    }
    char Serialize() {
        return 'a' + static_cast<char>(puyo);
    }
    void Deserialize(char c) {
        c -= 'a';
        puyo = static_cast<PuyoKind>(c);
    }
    bool IsBlank() {
        return puyo == PuyoKind::BLANK;
    }
};

class BiPuyo {
    enum class Direction { UP, RIGHT, DOWN, LEFT };
    Puyo main;
    Puyo sub;
    Direction direction = Direction::UP;
    BiPuyo() { }
public:
    BiPuyo(PuyoKind main, PuyoKind sub) : main(main), sub(sub) { }
    Puyo GetMain() {
        return main;
    }
    Puyo GetSub() {
        return sub;
    }
    void GetDirection(int &x, int &y) {
        switch (direction) {
            case Direction::UP:    { x = 0; y = -1; } break;
            case Direction::RIGHT: { x = 1; y = 0; } break;
            case Direction::DOWN:  { x = 0; y = 1; } break;
            case Direction::LEFT:  { x = -1; y = 0; } break;
            default: { x = 0; y = -1; } break;
        }
    }
    void Draw(int offset_x, int offset_y) {
        int x, y;
        GetDirection(x, y);
        std::cout << Console::GotoXY(X(offset_x), Y(offset_y));
        main.Draw();
        std::cout << Console::GotoXY(X(offset_x + x), Y(offset_y + y));
        sub.Draw();
    }
    void Draw(int x, int y, int offset_x, int offset_y) {
        int sub_x, sub_y;
        GetDirection(sub_x, sub_y);
        if (y > 0) {
            std::cout << Console::GotoXY(X(x + offset_x), Y(y + offset_y));
            main.Draw();
        }
        if (y + sub_y > 0) {
            std::cout << Console::GotoXY(X(x + offset_x + sub_x), Y(y + offset_y + sub_y));
            sub.Draw();
        }
    }
    void RotateCW() {
        // Rotate ClockWise
        switch (direction) {
            case Direction::UP:    { direction = Direction::RIGHT; } break;
            case Direction::RIGHT: { direction = Direction::DOWN; } break;
            case Direction::DOWN:  { direction = Direction::LEFT; } break;
            case Direction::LEFT:  { direction = Direction::UP; } break;
            default: direction = Direction::UP; break;
        }
    }
    void RotateCCW() {
        // Rotate CounterClockWise
        switch (direction) {
            case Direction::UP:    { direction = Direction::LEFT; } break;
            case Direction::RIGHT: { direction = Direction::UP; } break;
            case Direction::DOWN:  { direction = Direction::RIGHT; } break;
            case Direction::LEFT:  { direction = Direction::DOWN; } break;
            default: direction = Direction::UP; break;
        }
    }
};

class BiPuyoGenerator {
    std::shared_ptr<WELLRNG512Generator> puyo_color_random;
public:
    BiPuyoGenerator() {
        puyo_color_random = std::make_shared<WELLRNG512Generator>();
    }
    BiPuyoGenerator(const int &seed) {
        puyo_color_random = std::make_shared<WELLRNG512Generator>(seed);
    }
    std::shared_ptr<BiPuyo> GenerateBipuyo() {
        return std::make_shared<BiPuyo>(
            static_cast<PuyoKind>(puyo_color_random->Generate() % 4 + 1),
            static_cast<PuyoKind>(puyo_color_random->Generate() % 4 + 1)
        );
    }
};
