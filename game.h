#pragma once

class PlayerInformation;
class Board;

class Game {
public:
    virtual bool GameInit()=0;
    virtual void GameLoop()=0;
    virtual void Draw()=0;
private:
protected:
    void DrawFrame(int offset_x, int offset_y);
    
    std::shared_ptr<WELLRNG512Generator> puyo_color_random;
    std::shared_ptr<BiPuyo> GenerateBipuyo(){
        return std::make_shared<BiPuyo>(
            static_cast<PuyoKind>(puyo_color_random->Generate() % 4 + 1),
            static_cast<PuyoKind>(puyo_color_random->Generate() % 4 + 1)
        );
    }
};

class SingleGame : public Game {
    bool gameover = false;
    std::shared_ptr<Board> my_board;
    std::shared_ptr<BiPuyo> my_next_bipuyo;
    PlayerInformation my_info;
public:
    virtual bool GameInit();
    virtual void GameLoop();
    virtual void Draw();
};

class VSGame : public Game {
    bool gameover = false;
public:
    virtual bool GameInit();
    virtual void GameLoop();
    virtual void Draw();
private:
    void DrawFrame();
    void DrawBoard();
};

class VSRemoteGame : public VSGame {
    bool gameover = false;
public:
    virtual bool GameInit();
    virtual void GameLoop();
    virtual void Draw();
};

class AIGame : public VSGame {
    
public:
    virtual bool GameInit();
    virtual void GameLoop();
    virtual void Draw();
};
