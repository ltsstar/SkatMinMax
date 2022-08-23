//
// Created by michel on 8/5/22.
//

#ifndef SKATMINMAX_SKAT_H
#define SKATMINMAX_SKAT_H

#include <list>
#include <vector>
#include <cstdio>
#include <bitset>
#include <algorithm>
#include <set>
#include <stdexcept>
#include <cmath>

#include "hand.h"



class GamePlay
{
public:
    Hand* forehand;
    Hand* midhand;
    Hand* backhand;

    int points[10];

    std::vector<Card*> played_cards;

    Trump trump;
    int max_player;
    std::vector<int> winners;
    int getMaxPlayer() const;

public:
    GamePlay(Trump trump);
    GamePlay(Trump trump, std::set<CardType> forehandCards, std::set<CardType> midhandCards,
             std::set<CardType> backhandCards);
    GamePlay(int max_player, Trump trump, std::set<CardType> forehandCards, std::set<CardType> midhandCards,
             std::set<CardType> backhandCards);
    GamePlay(int max_player, Trump trump, std::set<CardType> forehandCards, std::set<CardType> midhandCards,
             std::set<CardType> backhandCards, std::vector<CardType> played_card_types, std::vector<int> winners);
    bool cards_sanity(std::set<CardType> forehandCards, std::set<CardType> midhandCards,
                      std::set<CardType> backhandCards, std::vector<CardType> played_card_types);

    bool is_trump(Card* card);
    bool is_new_play();
    bool is_last_play();
    int get_current_player();
    int get_winner(Card* cards[3]);
    int get_previous_player();
    int get_better_card(Card* first_card, Card* second_card);

    std::vector<Card*> get_viable_cards(Card* first_card, Hand* hand);

    Card* get_first_card();
    Card* get_second_card();
    std::vector<Card*> get_possible_next_moves();
    std::vector<Card*> reduce_hand_equivalencies(Hand* player_hand);
    Hand get_viable_hand(Card* first_card, Hand* hand);
    std::vector<Card*> get_reduced_next_moves();
    void make_move(Card *move);
    void revert_move();
    int get_points_of_card(Card* card);
    int get_points_of_hand(Card* play[3]);
    std::pair<int, int> eval_depth(int depth, int start_player);
    int eval();
};

#endif //SKATMINMAX_SKAT_H
