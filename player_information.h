#pragma once

struct PlayerInformation{
    int score;
    int chain_number;
    int max_chain_number;
    int obstacle_number_taken;
    int obstacle_number_to_send;

    PlayerInformation() {
        this->score = 0;
        this->chain_number = 0;
        this->max_chain_number = 0;
        this->obstacle_number_taken = 0;
        this->obstacle_number_to_send = 0;
    }
};
