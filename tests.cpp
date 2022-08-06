//
// Created by michel on 8/5/22.
//
#include <catch2/catch_test_macros.hpp>
#include "skat.h"
#include "minmax.h"

TEST_CASE( "joker_in_deck", "[skat]")
{
    std::list<Card> deck = {Card::D7, Card::SQ, Card::SJ, Card::CJ, Card::C7};
    std::bitset<32> b_deck = GamePlay::card_list_to_bitset(deck);
    bool joker_in_deck = GamePlay::joker_in_deck(b_deck);

    REQUIRE(joker_in_deck == true);
}

TEST_CASE( "joker_in_deck 2", "[skat]")
{
    std::list<Card> deck = {Card::DJ};
    std::bitset<32> b_deck = GamePlay::card_list_to_bitset(deck);
    bool joker_in_deck = GamePlay::joker_in_deck(b_deck);

    REQUIRE(joker_in_deck == true);
}

TEST_CASE( "joker_in_deck 3", "[skat]")
{
    std::list<Card> deck = {Card::C7};
    std::bitset<32> b_deck = GamePlay::card_list_to_bitset(deck);
    bool joker_in_deck = GamePlay::joker_in_deck(b_deck);

    REQUIRE(joker_in_deck == false);
}

TEST_CASE( "color_in_deck normal", "[skat]")
{
    std::list<Card> deck = {Card::C7};
    std::bitset<32> b_deck = GamePlay::card_list_to_bitset(deck);
    bool clubs_in_deck = GamePlay::color_in_deck(3, b_deck);
    bool diamonds_in_deck = GamePlay::color_in_deck(0, b_deck);

    REQUIRE(clubs_in_deck == true);
    REQUIRE(diamonds_in_deck == false);
}

TEST_CASE( "color_in_deck joker", "[skat]")
{
    std::list<Card> deck = {Card::CJ};
    std::bitset<32> b_deck = GamePlay::card_list_to_bitset(deck);
    bool clubs_in_deck = GamePlay::color_in_deck(3, b_deck);
    bool diamonds_in_deck = GamePlay::color_in_deck(0, b_deck);

    REQUIRE(clubs_in_deck == false);
    REQUIRE(diamonds_in_deck == false);
}

TEST_CASE( "get_winner", "[skat]")
{
    GamePlay gamePlay = GamePlay(Trump::Hearts);
    Card cards[] = {Card::HJ, Card::SJ, Card::D10};
    int winner = gamePlay.get_winner(cards);
    REQUIRE(winner == 1);
}

TEST_CASE( "get_winner 2", "[skat]")
{
    GamePlay gamePlay = GamePlay(Trump::Diamonds);
    Card cards[] = {Card::DA, Card::C9, Card::CJ};
    int winner = gamePlay.get_winner(cards);
    REQUIRE(winner == 2);
}

TEST_CASE( "minmax no counterplay possible", "[minmax]")
{
    GamePlay gamePlay = GamePlay(0,
             Trump::Diamonds,
             {Card::CJ, Card::HJ},
             {Card::DK, Card::SA},
             {Card::D10, Card::C9});
    MinMax minMax = MinMax(gamePlay);
    int max = minMax.max(0,0);
    REQUIRE(max == 29);
}

TEST_CASE( "minmax counterplay", "[minmax]")
{
    GamePlay gamePlay = GamePlay(0,
                                 Trump::Diamonds,
                                 {Card::CJ, Card::HJ},
                                 {Card::SJ, Card::DA},
                                 {Card::D10, Card::C9});
    MinMax minMax = MinMax(gamePlay);
    int max = minMax.max(0,0);
    REQUIRE(max == 23);
}

TEST_CASE( "minmax counterplay 2", "[minmax]")
{
    GamePlay gamePlay = GamePlay(0,
                                 Trump::Diamonds,
                                 {Card::CJ, Card::HJ},
                                 {Card::SJ, Card::DJ},
                                 {Card::DK, Card::DQ});
    MinMax minMax = MinMax(gamePlay);
    int max = minMax.max(0,0);
    REQUIRE(max == 7);
}

TEST_CASE( "minmax big game", "[minmax]")
{
    GamePlay gamePlay = GamePlay(0,
                                 Trump::Diamonds,
                                 {Card::CJ, Card::HJ, Card::D7, Card::CA, Card::C9, Card::D7, Card::SQ, Card::S9, Card::H9, Card::H8},
                                 {Card::SJ, Card::DJ, Card::DA, Card::D10, Card::C10, Card::CQ, Card::S10, Card::S8, Card::HQ, Card::H7},
                                 {Card::DK, Card::DQ, Card::D8, Card::CK, Card::C8, Card::SK, Card::S7, Card::HA, Card::H10, Card::HK});
    MinMax minMax = MinMax(gamePlay);
    int max = minMax.max(0,0);
    REQUIRE(max == 2);
}

TEST_CASE( "minmax safe total win", "[minmax]")
{
    GamePlay gamePlay = GamePlay(0,
                                 Trump::Diamonds,
                                 {Card::CJ, Card::SJ, Card::HJ, Card::DJ, Card::DA, Card::D10, Card::DK, Card::DQ, Card::D9, Card::D8},
                                 {Card::H7, Card::H8, Card::H9, Card::HQ, Card::HK, Card::H10, Card::HA, Card::S7, Card::S8, Card::S9},
                                 {Card::SQ, Card::SK, Card::S10, Card::SA, Card::C7, Card::S8, Card::S9, Card::CQ, Card::CK, Card::C10});
    // hand: CA, D7
    MinMax minMax = MinMax(gamePlay);
    int max = minMax.max(0,0);
    REQUIRE(max == 2);
}