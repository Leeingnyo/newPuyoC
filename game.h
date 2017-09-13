#pragma once

class PlayerInformation;
class BiPuyo;
class BiPuyoGenerator;
class Board;

class Game {
public:
    virtual bool GameInit()=0;
    virtual void GameLoop()=0;
    virtual void Draw()=0;
private:
protected:
    static void DrawFrame(int offset_x, int offset_y);
};

class SingleGame : public Game {
    bool gameover = false;
    std::shared_ptr<BiPuyoGenerator> bipuyo_generator;
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
