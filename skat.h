//
// Created by michel on 8/5/22.
//

#include <list>
#include <vector>
#include <cstdio>
#include <bitset>

#ifndef SKATMINMAX_SKAT_H
#define SKATMINMAX_SKAT_H

enum class Card {
    D7 = 0, D8 = 1, D9 = 2, DQ = 3, DK = 4, D10 = 5, DA = 6, DJ = 7,
    H7 = 8, H8 = 9, H9 = 10, HQ = 11, HK = 12, H10 = 13, HA = 14, HJ = 15,
    S7 = 16, S8 = 17, S9 = 18, SQ = 19, SK = 20, S10 = 21, SA = 22, SJ = 23,
    C7 = 24, C8 = 25, C9 = 26, CQ = 27, CK = 28, C10 = 29, CA = 30, CJ = 31
};

enum class Trump {
    Diamonds = 0,
    Hearts = 1,
    Spades = 2,
    Clubs = 3
};

class GamePlay
{
    std::bitset<32> forehandCards;
    std::bitset<32> midhandCards;
    std::bitset<32> backhandCards;

    std::vector<Card> playedCards;

    Trump trump;
    int maxPlayer;
    std::vector<int> winners;

public:
    int getMaxPlayer() const;

public:
    GamePlay(Trump trump);
    GamePlay(Trump trump, std::list<Card> forehandCards, std::list<Card> midhandCards,
             std::list<Card> backhandCards);
    GamePlay(int max_player, Trump trump, std::list<Card> forehandCards, std::list<Card> midhandCards,
             std::list<Card> backhandCards);
    static std::bitset<32> card_list_to_bitset(std::list<Card> cards);
    static std::vector<Card> bitset_to_card_vector(std::bitset<32> deck);
    static bool is_joker(Card card);
    static int get_color_of_card(Card card);
    static bool joker_in_deck(std::bitset<32> deck);
    static bool color_in_deck(int color, std::bitset<32> deck);
    static std::bitset<32> get_color_and_jokers(int color, std::bitset<32> cards);
    static std::bitset<32> get_color(int color, std::bitset<32> cards);

    bool is_trump(Card card);
    bool is_new_play();
    int get_current_player();
    int get_winner(Card cards[3]);
    int get_previous_player();
    int get_better_card(Card first_card, Card second_card);
    std::bitset<32> get_viable_cards(Card first_card, std::bitset<32> remaining_cards);

    Card get_first_card();
    std::bitset<32> get_possible_next_moves();
    void make_move(Card move);
    void revert_move();
    int get_points_of_card(Card card);
    int get_points_of_hand(Card play[3]);
    int eval();
};

#endif //SKATMINMAX_SKAT_H
