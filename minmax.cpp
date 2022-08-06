//
// Created by michel on 8/5/22.
//
#include "minmax.h"

MinMax::MinMax(GamePlay gamePlay) :
        gamePlay(gamePlay)
{
}

int MinMax::max(int alpha, int beta) {
    std::list<Card> next_moves = this->gamePlay.get_possible_next_moves();
    if(next_moves.size() == 0)
    {
        return this->gamePlay.eval();
    }
    int maxValue = -1;
    Card maxMove;
    for(auto const& move : next_moves)
    {
        this->gamePlay.make_move(move);
        int value = 0;
        if(this->gamePlay.get_current_player() == this->gamePlay.getMaxPlayer())
        {
            value = this->max(alpha + 1, 0);
        } else {
            value = this->min(alpha + 1, 0);
        }
        if(value > maxValue)
        {
            maxValue = value;
            maxMove = move;
        }
        this->gamePlay.revert_move();
    }
    return maxValue;
}

int MinMax::min(int alpha, int beta) {
    std::list<Card> next_moves = this->gamePlay.get_possible_next_moves();
    if(next_moves.size() == 0)
    {
        return this->gamePlay.eval();
    }
    int minValue = 121;
    Card minMove;
    for(auto const& move : next_moves)
    {
        this->gamePlay.make_move(move);
        int value = 0;
        if(this->gamePlay.get_current_player() == this->gamePlay.getMaxPlayer())
        {
            value = this->max(alpha + 1, 0);
        } else {
            value = this->min(alpha + 1, 0);
        }
        if(value < minValue)
        {
            minValue = value;
            minMove = move;
        }
        this->gamePlay.revert_move();
    }
    return minValue;
}
