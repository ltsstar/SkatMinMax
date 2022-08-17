//
// Created by michel on 8/5/22.
//

#include <list>
#include <vector>
#include <cstdio>
#include <bitset>
#include <algorithm>
#include <set>
#include <stdexcept>
#include <cmath>

#ifndef SKATMINMAX_SKAT_H
#define SKATMINMAX_SKAT_H



enum class Trump {
    Diamonds = 0,
    Hearts = 1,
    Spades = 2,
    Clubs = 3
};

enum class CardType {
    D7 = 0, D8 = 1, D9 = 2, DQ = 3, DK = 4, D10 = 5, DA = 6, DJ = 7,
    H7 = 8, H8 = 9, H9 = 10, HQ = 11, HK = 12, H10 = 13, HA = 14, HJ = 15,
    S7 = 16, S8 = 17, S9 = 18, SQ = 19, SK = 20, S10 = 21, SA = 22, SJ = 23,
    C7 = 24, C8 = 25, C9 = 26, CQ = 27, CK = 28, C10 = 29, CA = 30, CJ = 31
};

class Card
{
    CardType card_type;
    bool card_joker;
    int card_color;
public:
    Card() {};
    explicit Card(CardType card_type);
    int get_card_color();
    bool is_joker();
    bool is_color(int color);
    explicit operator CardType() const {
        return card_type;
    };
    explicit operator int() const {
        return static_cast<int>(card_type);
    };
    bool operator <(const Card& other_card) const
    {
        return card_type < other_card.card_type;
    };
    bool operator ==(const Card& other_card) const
    {
        return card_type == other_card.card_type;
    }
};

class Hand
{
    std::vector<Card*> jokers;
    std::vector<Card*> colors[4];

public:
    Hand();
    Hand(std::set<CardType> cards);
    bool has_card(Card* card);
    bool has_color(int color);
    bool has_joker();
    void remove_card(Card* card);
    void add_card(Card* card);
    std::vector<Card*> get_cards();
    Card* get_last_card();
    std::vector<Card*> get_jokers();
    std::vector<Card*> get_color(int color);
    std::vector<Card*> get_color_and_jokers(int color);
};

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
    std::vector<Card*> get_possible_next_moves();
    std::vector<Card*> reduce_hand_equivalencies(Hand* player_hand);
    std::vector<Card*> get_reduced_next_moves();
    void make_move(Card *move);
    void revert_move();
    int get_points_of_card(Card* card);
    int get_points_of_hand(Card* play[3]);
    std::pair<int, int> eval_depth(int depth, int start_player);
    int eval();
};

#endif //SKATMINMAX_SKAT_H
