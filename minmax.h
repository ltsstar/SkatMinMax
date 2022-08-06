//
// Created by michel on 8/5/22.
//

#include "skat.h"


#ifndef SKATMINMAX_MINMAX_H
#define SKATMINMAX_MINMAX_H

class MinMax
{
    GamePlay gamePlay;
public:
    MinMax(GamePlay gamePlay);
    int max(int alpha, int beta);
    int min(int alpha, int beta);
};

#endif //SKATMINMAX_MINMAX_H
