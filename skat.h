//
// Created by michel on 8/5/22.
//

#include <list>
#include <vector>
#include <cstdio>

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
    std::list<Card> forehandCards;
    std::list<Card> midhandCards;
    std::list<Card> backhandCards;

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
    void set_played_cards(std::list<Card> played_cards);
    void set_current_starter(int starter);
    void add_played_card(Card played_card);
    bool is_trump(Card card);
    bool is_joker(Card card);
    bool is_new_play();
    int get_current_player();
    int get_winner(Card cards[3]);
    int get_previous_player();
    int get_better_card(Card first_card, Card second_card);
    bool joker_in_deck(std::list<Card> deck);
    bool color_in_deck(int color, std::list<Card> deck);
    Card get_first_card();
    int get_color_of_card(Card card);
    std::list<Card> get_color_and_jokers(int color, std::list<Card> cards);
    std::list<Card> get_color(int color, std::list<Card> cards);
    std::list<Card> get_viable_cards(Card first_card, std::list<Card> remaining_cards);
    std::list<Card> get_possible_next_moves();
    void make_move(Card move);
    void revert_move();
    std::list<std::list<Card>> get_next_plays();
    void make_play(std::list<Card> play);
    void revert_play();
    int get_points_of_card(Card card);
    int get_points_of_hand(Card play[3]);
    int eval();
};

#endif //SKATMINMAX_SKAT_H
