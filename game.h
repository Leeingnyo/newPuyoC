#pragma once

class PlayerInformation;
class BiPuyo;
class BiPuyoGenerator;
class Board;

class Game {
public:
    virtual bool GameInit()=0;
    virtual void GameLoop()=0;
    virtual void GameResult()=0;
    virtual void Draw()=0;
private:
protected:
    static void DrawFrame(int offset_x = 0, int offset_y = 0);
    static void DrawBoard(const std::shared_ptr<Board> &board, int offset_x = 0, int offset_y = 0);
    static void DrawNextBiPuyo(const std::shared_ptr<BiPuyo> &bipuyo, int offset_x = 0, int offset_y = 0);
    static void DrawPlayerInformation(const PlayerInformation &info, int offset_x = 0, int offset_y = 0);
    static void Draw(const std::shared_ptr<Board> &board, const std::shared_ptr<BiPuyo> &bipuyo, const PlayerInformation &info, int offset_x = 0, int offset_y = 0);
};

class SingleGame : public Game {
    bool gameover = false;
    std::shared_ptr<BiPuyoGenerator> my_bipuyo_generator;
    std::shared_ptr<Board> my_board;
    std::shared_ptr<BiPuyo> my_next_bipuyo;
    PlayerInformation my_info;
public:
    virtual bool GameInit();
    virtual void GameLoop();
    virtual void GameResult();
    virtual void Draw();
};

class VSGame : public Game {
protected:
    bool gameover = false;
    std::shared_ptr<BiPuyoGenerator> my_bipuyo_generator;
    std::shared_ptr<Board> my_board;
    std::shared_ptr<BiPuyo> my_next_bipuyo;
    PlayerInformation my_info;

    std::shared_ptr<BiPuyoGenerator> other_bipuyo_generator;
    std::shared_ptr<Board> other_board;
    std::shared_ptr<BiPuyo> other_next_bipuyo;
    PlayerInformation other_info;
public:
    virtual bool GameInit();
    virtual void GameLoop();
    virtual void GameResult();
    virtual void Draw();
};

class VSRemoteGame : public VSGame {
    std::shared_ptr<Socket> socket;
    int obstacle_to_send = 0;
    int obstacle_to_take = 0;
public:
    virtual bool GameInit();
    virtual void GameLoop();
    virtual void Draw();

    std::string Serialize();
    void Deserialize(std::string data);
};

class AIGame : public VSGame {
    
public:
    virtual bool GameInit();
    virtual void GameLoop();
    virtual void Draw();
};
