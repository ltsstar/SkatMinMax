//
// Created by michel on 8/5/22.
//
#include "minmax.h"

MinMax::MinMax(GamePlay gamePlay) :
        gamePlay(gamePlay),
        node_count(0),
        depth_count{}
        {
            start_time = std::chrono::system_clock::now();
        }

void MinMax::print_progress()
{
    auto bench_time = std::chrono::system_clock::now();

    char buffer[4096] = {};
    std::sprintf(buffer + strlen(buffer), "Total: %lu\n", node_count);
    if(node_count % progress_print == 0)
    {
        for(int i=0;i<30;++i)
        {
            std::sprintf(buffer + strlen(buffer), "(Depth, Count): (%d, %lu)\n", i, depth_count[i]);
        }
        std::sprintf(buffer + strlen(buffer), "Time per %lu : %lld", progress_print,
                     std::chrono::duration_cast<std::chrono::milliseconds>(bench_time - start_time).count());
        start_time = std::chrono::system_clock::now();
        WARN(buffer);
    }
}

std::pair<int, std::vector<Card>> MinMax::max(int depth, int alpha, int beta) {
    node_count++;
    depth_count[depth]++;
    print_progress();

    std::set<Card> next_moves = gamePlay.get_possible_next_moves();

    int maxValue = -1;
    std::vector<Card> previous_moves;
    Card maxMove;
    for(auto const& card : next_moves)
    {
        this->gamePlay.make_move(card);
        std::pair<int, std::vector<Card>> value;
        if(depth == 26)
        {
            value = last();
        }
        else if(gamePlay.get_current_player() == gamePlay.getMaxPlayer())
        {
            value = max(depth + 1, 0, 0);
        } else {
            value = min(depth + 1, 0, 0);
        }

        if(value.first > maxValue)
        {
            maxValue = value.first;
            maxMove = card;
            previous_moves = value.second;
        }

        gamePlay.revert_move();
    }
    previous_moves.push_back(maxMove);
    return std::pair<int, std::vector<Card>>(maxValue, previous_moves);
}

std::pair<int, std::vector<Card>> MinMax::min(int depth, int alpha, int beta) {
    node_count++;
    depth_count[depth]++;
    print_progress();

    std::set<Card> next_moves = gamePlay.get_possible_next_moves();

    int minValue = 121;
    std::vector<Card> previous_moves;
    Card minMove;
    for(auto const& card : next_moves)
    {
        gamePlay.make_move(card);
        std::pair<int, std::vector<Card>> value;
        if(depth == 26)
        {
            value = last();
        }
        else if(gamePlay.get_current_player() == gamePlay.getMaxPlayer())
        {
            value = max(depth + 1, 0, 0);
        } else {
            value = min(depth + 1, 0, 0);
        }

        if(value.first < minValue)
        {
            minValue = value.first;
            minMove = card;
            previous_moves = value.second;
        }

        gamePlay.revert_move();
    }
    previous_moves.push_back(minMove);
    return std::pair<int, std::vector<Card>>(minValue, previous_moves);
}

std::pair<int, std::vector<Card>> MinMax::last()
{
    node_count++;print_progress();    node_count++;print_progress();    node_count++;print_progress();
    depth_count[27]++;depth_count[28]++;depth_count[29]++;

    Card cards[3];
    int current_player = gamePlay.get_current_player();

    for(int i=0; i<3; ++i) {
        if (current_player == 0)
            cards[i] = *gamePlay.forehandCards.end();
        else if (current_player == 1)
            cards[i] = *gamePlay.midhandCards.end();
        else
            cards[i] = *gamePlay.backhandCards.end();
        gamePlay.playedCards.push_back(cards[i]);
        current_player = (current_player + 1) % 3;
    }
    std::pair<int, std::vector<Card>> result = std::pair<int, std::vector<Card>>(gamePlay.eval(),
            {cards[0], cards[1], cards[2]});

    for(int i=0; i<3; ++i) {
        gamePlay.playedCards.pop_back();
    }
    return result;
}