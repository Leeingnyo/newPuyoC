#pragma once

struct PlayerInformation{
    int score;
    int chain_number;
    int max_chain_number;
    int obstacle_number_taken;

    PlayerInformation() {
        this->score = 0;
        this->chain_number = 0;
        this->max_chain_number = 0;
        this->obstacle_number_taken = 0;
    }

    std::string Serialize() {
        std::ostringstream data;
        data << score << ' ';
        data << chain_number << ' ';
        data << max_chain_number << ' ';
        data << obstacle_number_taken;
        return data.str();
    }
    void Deserialize(std::string data) {
        std::string buf;
        std::stringstream ss(data);
        std::vector<std::string> tokens;
        while (ss >> buf)
            tokens.push_back(buf);
        score = std::stoi(tokens[0]);
        chain_number = std::stoi(tokens[1]);
        max_chain_number = std::stoi(tokens[2]);
        obstacle_number_taken = std::stoi(tokens[3]);
    }
};
