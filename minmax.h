//
// Created by michel on 8/5/22.
//

#include "skat.h"

#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <cstdio>
#include <chrono>


#ifndef SKATMINMAX_MINMAX_H
#define SKATMINMAX_MINMAX_H

class MinMax
{
    unsigned long node_count;
    unsigned long depth_count[30];
    GamePlay gamePlay;
    unsigned long progress_print = 10000000;

    int points[10];
    int start_player[10];

    std::chrono::time_point<std::chrono::system_clock> start_time;

public:
    MinMax(GamePlay gamePlay);
    void print_progress();
    std::pair<int, std::vector<Card>> max(int depth, int alpha, int beta);
    std::pair<int, std::vector<Card>> min(int depth, int alpha, int beta);
    std::pair<int, std::vector<Card>> last();
};

#endif //SKATMINMAX_MINMAX_H
