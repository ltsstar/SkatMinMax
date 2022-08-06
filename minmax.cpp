//
// Created by michel on 8/5/22.
//
#include "minmax.h"

MinMax::MinMax(GamePlay gamePlay) :
        gamePlay(gamePlay)
{
}

int MinMax::max(int alpha, int beta) {
    std::bitset<32> next_moves = gamePlay.get_possible_next_moves();
    if(next_moves.none())
    {
        return this->gamePlay.eval();
    }
    int maxValue = -1;
    Card maxMove;
    for(int i=0;i<32;++i)
    {
        if((next_moves & (std::bitset<32>(1) << i)).none())
            continue;

        this->gamePlay.make_move(static_cast<Card>(i));
        int value = 0;
        if(gamePlay.get_current_player() == gamePlay.getMaxPlayer())
        {
            value = max(alpha + 1, 0);
        } else {
            value = min(alpha + 1, 0);
        }
        if(value > maxValue)
        {
            maxValue = value;
            //maxMove = move;
        }
        this->gamePlay.revert_move();
    }
    return maxValue;
}

int MinMax::min(int alpha, int beta) {
    std::bitset<32> next_moves = gamePlay.get_possible_next_moves();
    if(next_moves.none())
    {
        return this->gamePlay.eval();
    }
    int minValue = 121;
    Card minMove;
    for(int i=0;i<32;++i)
    {
        if((next_moves & (std::bitset<32>(1) << i)).none())
            continue;

        gamePlay.make_move(static_cast<Card>(i));
        int value = 0;
        if(gamePlay.get_current_player() == gamePlay.getMaxPlayer())
        {
            value = max(alpha + 1, 0);
        } else {
            value = min(alpha + 1, 0);
        }
        if(value < minValue)
        {
            minValue = value;
            //minMove = move;
        }
        this->gamePlay.revert_move();
    }
    return minValue;
}
