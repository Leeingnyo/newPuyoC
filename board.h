#pragma once

#define MAP_WIDTH 6
#define MAP_HEIGHT 13

class Puyo;
class BiPuyo;
class Game;

class Board {
    enum class State { IDLE, DROP, CHAINING, DROP_OBSTACLE, NEED_NEXT, HALT, GAMEOVER };
    Puyo map[MAP_HEIGHT][MAP_WIDTH];
    std::shared_ptr<BiPuyo> bipuyo;
    int bipuyo_x, bipuyo_y;
    
    State state;
    /*
      State Machine
      IDLE - (bipuyo meets floor or other puyo) -> DROP
      DROP - (no puyo in the air) -> CHAINING
      CHAINING - (puyo in air) -> DROP
      CHAINING - (no more chain, no more drop) -> DROP_OBSTACLE
      CHAINING - (2,1) -> GAMEOVER
      DROP_OBSTACLE - (no obstacles or drop all) -> NEED_NEXT
      DROP_OBSTACLE - (2,1) -> GAMEOVER
      NEED_NEXT - (take next bipuyo) -> IDLE
    */
    
    int gain_score;
    int chain_number;
    int obstacle_number_taken;
    int obstacle_number_to_send;
    
    const int GRAVITY_FRAME = 35;
    int gravity_t;
    
    const int ROTATE_FRAME = 5;
    int rotate_t;
    
    const int MOVE_FRAME = 3;
    int move_t;
    
    const int DOWN_FRAME = 1;
    int down_t;
    
    const int DROP_PREDELAY = 15;
    int drop_predelay_t;
    const int DROP_FRAME = 2;
    int drop_t;
    
    const int CHAINING_FRAME = 30;
    int chaining_t;
public:
    Board() {
        gain_score = 0;
        chain_number = 0;
        obstacle_number_taken = 0;
        obstacle_number_to_send = 0;
        
        gravity_t = 0;
        rotate_t = 0;
        move_t = 0;
        down_t = 0;
        drop_predelay_t = 0;
        drop_t = 0;
        chaining_t = 0;
        
        state = State::IDLE;
    }
    bool IsBusy() {
        return state != State::IDLE;
    }
    bool IsNeedNext() {
        return state == State::NEED_NEXT;
    }
    bool IsGameOver() {
        return state == State::GAMEOVER;
    }
    
    void MoveLeft() {
        int sub_x = 0, sub_y = 0;
        if (bipuyo) {
            bipuyo->GetDirection(sub_x, sub_y);
        }
        if (move_t == 0) {
            move_t = 1;
            if (bipuyo_x != 0 && bipuyo_x + sub_x != 0 && map[bipuyo_y][bipuyo_x - 1].IsBlank() && map[bipuyo_y + sub_y][bipuyo_x + sub_x - 1].IsBlank()) {
                bipuyo_x--;
            }
        }
    }
    void MoveRight() {
        int sub_x = 0, sub_y = 0;
        if (bipuyo) {
            bipuyo->GetDirection(sub_x, sub_y);
        }
        if (move_t == 0) {
            move_t = 1;
            if (bipuyo_x != (MAP_WIDTH - 1) && bipuyo_x + sub_x != (MAP_WIDTH - 1) && map[bipuyo_y][bipuyo_x + 1].IsBlank() && map[bipuyo_y + sub_y][bipuyo_x + sub_x + 1].IsBlank()) {
                bipuyo_x++;
            }
        }
    }
    void MoveDown() {
        int sub_x = 0, sub_y = 0;
        if (bipuyo) {
            bipuyo->GetDirection(sub_x, sub_y);
        }
        if (down_t == 0) {
            down_t = 1;
            gravity_t += 40;
        }
    }
    void RotateCCW() {
        if (rotate_t == 0) {
            rotate_t = 1;
            int sub_x = 0, sub_y = 0;
            if (bipuyo) {
                bipuyo->GetDirection(sub_x, sub_y);
            }
            if (sub_x == 1) {
                // MS 일때 
                // 아무 의미 없음 
            } 
            if (sub_y == -1) {
                // 위일 때 
                // |S
                // |M --> |SM 
                // 이면 밀어야 함 
                if (bipuyo_x == 0 || !map[bipuyo_y][bipuyo_x - 1].IsBlank()) {
                    // 근데 
                    // |S|     |M|
                    // |M| --> |S|
                    // 이면 뒤바꿔야 함 
                    if (bipuyo_x == (MAP_WIDTH - 1) || !map[bipuyo_y][bipuyo_x + 1].IsBlank()) {
                        bipuyo_y--;
                        bipuyo->RotateCCW();
                    }
                    else {
                        bipuyo_x++;
                    }
                }
            }
            if (sub_x == -1) {
                // SM 일떄 
                //       M
                // SM -> S
                // 바닥이 있으면 이렇게 
                if (bipuyo_y == (MAP_HEIGHT - 1) || !map[bipuyo_y + 1][bipuyo_x].IsBlank()) {
                    bipuyo_y--;
                }
            }
            if (sub_y == 1) {
                // 아래일 때 
                // M|
                // S| --> MS| 
                // 이면 밀어야 함 
                if (bipuyo_x == (MAP_WIDTH - 1) || !map[bipuyo_y][bipuyo_x + 1].IsBlank()) {
                    // 근데 
                    // |M|     |S|
                    // |S| --> |M|
                    // 이면 뒤바꿔야 함 
                    if (bipuyo_x == 0 || !map[bipuyo_y][bipuyo_x - 1].IsBlank()) {
                        bipuyo_y++;
                        bipuyo->RotateCCW();
                    }
                    else {
                        bipuyo_x--;
                    }
                }
            }
            bipuyo->RotateCCW();
        }
    }
    
    void SetNextBiPuyo(const std::shared_ptr<BiPuyo>& next) {
        bipuyo = next;
        bipuyo_x = 2;
        bipuyo_y = 1;
        state = State::IDLE;
    }
    void UpdatePlayerInformation(PlayerInformation &info) {
        info.score += gain_score;
        gain_score = 0;
        info.chain_number = chain_number;
        if (info.max_chain_number < chain_number)
            info.max_chain_number = chain_number;
        info.obstacle_number_taken = obstacle_number_taken;
    }
    void TakeObstacles(int obstacle_number_taken) {
        this->obstacle_number_taken += obstacle_number_taken;
    }
    int SendObstacles() {
        if (!IsBusy()) {
            int obstacle_number_to_send = this->obstacle_number_to_send;
            this->obstacle_number_to_send = 0;
            return obstacle_number_to_send;
        }
        return 0;
    }
    
    void Update() {
        switch (state) {
            case State::IDLE: IdleUpdate(); break;
            case State::DROP: DropUpdate(); break;
            case State::CHAINING: ChainingUpdate(); break;
            case State::DROP_OBSTACLE: DropObstacleUpdate(); break;
            case State::NEED_NEXT: break;
            default: { state = State::HALT; } break;
        }
    }
private:
    void IdleUpdate() {
        int sub_x = 0, sub_y = 0;
        if (bipuyo) {
            bipuyo->GetDirection(sub_x, sub_y);
        }
        if (gravity_t++ > GRAVITY_FRAME) {
            gravity_t = 0;
            if (bipuyo_y < (MAP_HEIGHT - 1) && map[bipuyo_y + 1][bipuyo_x].IsBlank() && bipuyo_y + sub_y < (MAP_HEIGHT - 1) && map[bipuyo_y + sub_y + 1][bipuyo_x + sub_x].IsBlank()) {
                bipuyo_y++;
            }
            else {
                map[bipuyo_y][bipuyo_x] = Puyo(bipuyo->GetMain());
                if (bipuyo_y + sub_y >= 0)
                    map[bipuyo_y + sub_y][bipuyo_x + sub_x] = Puyo(bipuyo->GetSub());
                bipuyo.reset();
                state = State::DROP;
            }
        }
        if (rotate_t > ROTATE_FRAME) {
            rotate_t = 0;
        }
        else if (rotate_t > 0) {
            rotate_t++;
        }
        if (move_t > MOVE_FRAME) {
            move_t = 0;
        }
        else if (move_t > 0) {
            move_t++;
        }
        if (down_t > DOWN_FRAME) {
            down_t = 0;
        }
        else if (down_t > 0) {
            down_t++;
        }
    }

    /*
     * return true when board status is changed
     */
    bool ShiftDown() {
        bool isChanged = false;
        for (int i = MAP_HEIGHT - 1; i > 0; i--) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                if (map[i][j].IsBlank() && !map[i - 1][j].IsBlank()) {
                    map[i][j] = map[i - 1][j];
                    map[i - 1][j] = Puyo();
                    isChanged = true;
                }
            }
        }
        return isChanged;
    }

    void DropUpdate() {
        if (drop_predelay_t++ < DROP_PREDELAY)
            return;
        if (drop_t++ < DROP_FRAME)
            return;
        drop_t = 0;
        bool isChanged = ShiftDown();
        if (!isChanged) {
            state = State::CHAINING;
            drop_predelay_t = 0;
        }
    }
    
    class Pair {
    public:
        int x; int y;
        Pair(int x, int y) {
            this->x = x;
            this->y = y;
        }
    };
    void Chaining(bool (&check)[MAP_HEIGHT][MAP_WIDTH], std::vector<Pair> &chain, std::vector<Pair> &nearby_obstacles, int x, int y) {
        if (!check[y][x]) {
            check[y][x] = true;
            if (map[y][x].IsObstacle()) {
                nearby_obstacles.push_back(Pair(x, y));
                return;
            }
            chain.push_back(Pair(x, y));
            if (y > 0 && (map[y - 1][x].IsSameColor(map[y][x]) || map[y - 1][x].IsObstacle()))
                Chaining(check, chain, nearby_obstacles, x, y - 1);
            if (x > 0 && (map[y][x - 1].IsSameColor(map[y][x]) || map[y][x - 1].IsObstacle()))
                Chaining(check, chain, nearby_obstacles, x - 1, y);
            if (y + 1 < MAP_HEIGHT && (map[y + 1][x].IsSameColor(map[y][x]) || map[y + 1][x].IsObstacle()))
                Chaining(check, chain, nearby_obstacles, x, y + 1);
            if (x + 1 < MAP_WIDTH && (map[y][x + 1].IsSameColor(map[y][x]) || map[y][x + 1].IsObstacle()))
                Chaining(check, chain, nearby_obstacles, x + 1, y);
        }
    }
    void ChainingUpdate() {
        bool chaining = false;
        static std::vector<std::vector<Pair>> chain_list;
        static std::vector<Pair> removed_obstacles;
        
        if (chaining_t > CHAINING_FRAME) {
            for (auto &chain : chain_list) {
                gain_score += chain.size() * 10 * pow(2, chain_number - 1); // 점수 계산 
                obstacle_number_to_send += pow(2, chain_number) - 1;
                while (obstacle_number_taken > 0 && obstacle_number_to_send > 0) {
                    obstacle_number_taken -= 1;
                    obstacle_number_to_send -= 1;
                }
                for (auto &pair : chain) {
                    map[pair.y][pair.x] = Puyo();
                }
            }
            for (auto &pair : removed_obstacles) {
                map[pair.y][pair.x] = Puyo();
            }
            chain_list.clear();
            removed_obstacles.clear();
            chaining_t = 0;
            state = State::DROP;
            return;
        }
        else if (chaining_t > CHAINING_FRAME * 2 / 3) {
            chaining_t++;
            return;
        }
        else if (chaining_t > 0) {
            Puyo effect = Puyo(chaining_t / 4 % 2 ? PuyoKind::EXPLOSION : PuyoKind::BLANK);
            for (auto &chain : chain_list) {
                for (auto &pair : chain) {
                    map[pair.y][pair.x] = effect;
                }
            }
            for (auto &pair : removed_obstacles) {
                map[pair.y][pair.x] = effect;
            }
            chaining_t++;
            return;
        }
        
        // 맵 끝에서 끝까지 
        bool check[MAP_HEIGHT][MAP_WIDTH] = { false, };
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                if (map[y][x].IsBlank()) continue;
                if (map[y][x].IsObstacle()) continue;
                
                std::vector<Pair> chain;
                std::vector<Pair> nearby_obstacles;
                Chaining(check, chain, nearby_obstacles, x, y);
                if (chain.size() >= 4) { // FIXME make 4 constant. extract it
                    chaining = true;
                    chain_list.push_back(chain);
                    removed_obstacles.insert(removed_obstacles.end(), nearby_obstacles.begin(), nearby_obstacles.end());
                }
            }
        }
        
        if (chaining) {
            chaining_t = 1;
            chain_number++;
        }
        else {
            chain_number = 0;
            if (map[1][2].IsBlank()) state = State::DROP_OBSTACLE;
            else state = State::GAMEOVER;
        }
    }
    void DropObstacleUpdate() {
        static bool isProgressed = false;
        if (!isProgressed) {
            if (obstacle_number_taken > 0) {
                isProgressed = true;
            }
            else {
                state = State::NEED_NEXT;
                return;
            }
        }
        if (drop_predelay_t++ < DROP_PREDELAY)
            return;
        if (drop_t++ < DROP_FRAME)
            return;
        drop_t = 0;
        // generate obstacles at empty top of map
        if (obstacle_number_taken > 0) {
            bool empty[MAP_WIDTH] = { false, };
            for (int i = 0; i < MAP_WIDTH; i++) {
                empty[i] = map[0][i].IsBlank();
            }
            std::vector<int> empty_position;
            for (int i = 0; i < MAP_WIDTH; i++) {
                if (empty[i]) {
                    empty_position.push_back(i);
                }
            }
            int empty_number = empty_position.size();
            if (obstacle_number_taken >= empty_number) {
                for (auto position : empty_position) {
                    map[0][position] = Puyo(PuyoKind::OBSTACLE);
                }
                obstacle_number_taken -= empty_number;
            } else {
                if (empty_number > 0) {
                    std::random_shuffle(empty_position.begin(), empty_position.end());
                    for (int i = 0; i < obstacle_number_taken; i++) {
                        map[0][empty_position[i]] = Puyo(PuyoKind::OBSTACLE);
                    }
                    obstacle_number_taken = 0;
                }
            }
        }
        // shift down
        bool isChanged = ShiftDown();
        // no more move
        if (!isChanged) {
            isProgressed = false;
            drop_predelay_t = 0;
            if (map[1][2].IsBlank()) state = State::NEED_NEXT;
            else state = State::GAMEOVER;
            return;
        }
    }
public:
    void Draw(int offset_x, int offset_y) {
        int sub_x = 0, sub_y = 0;
        if (bipuyo) {
            bipuyo->Draw(bipuyo_x, bipuyo_y, offset_x, offset_y - 1);
            bipuyo->GetDirection(sub_x, sub_y);
        }
        for (int i = 1; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                if (bipuyo) {
                    if (j == bipuyo_x + sub_x && i == bipuyo_y + sub_y) continue;
                    if (j == bipuyo_x && i == bipuyo_y) continue;
                }
                std::cout << Console::GotoXY(X(offset_x + j), Y(offset_y + i - 1));
                map[i][j].Draw();
            }
        }
    }
    
    std::string Serialize() {
        std::ostringstream data;
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                data << map[i][j].Serialize();
            }
        }
        data << static_cast<int>(state);
        if (bipuyo) {
            data << bipuyo->Serialize();
            data << bipuyo_x << " ";
            data << bipuyo_y;
        }
        return data.str();
    }
    void Deserialize(std::string data) {
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                map[i][j].Deserialize(data.at(i * MAP_WIDTH + j));
            }
        }
        int stateInteger = std::stoi(data.substr(MAP_HEIGHT * MAP_WIDTH, 1));
        state = static_cast<State>(stateInteger);
        if (data.length() > MAP_HEIGHT * MAP_WIDTH + 1) {
            if (bipuyo == nullptr) {
                bipuyo = BiPuyoGenerator::GenerateEmptyBipuyo();
            }
            bipuyo->Deserialize(data.substr(MAP_HEIGHT * MAP_WIDTH + 1, 3));

            std::string buf;
            std::stringstream ss(data.substr(MAP_HEIGHT * MAP_WIDTH + 1 + 3));
            std::vector<std::string> tokens;
            while (ss >> buf)
                tokens.push_back(buf);
            bipuyo_x = std::stoi(tokens[0]);
            bipuyo_y = std::stoi(tokens[1]);
        }
        else {
            bipuyo.reset();
        }
    }
};
