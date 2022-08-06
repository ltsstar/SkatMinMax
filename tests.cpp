//
// Created by michel on 8/5/22.
//
#include <catch2/catch_test_macros.hpp>
#include "skat.h"
#include "minmax.h"

TEST_CASE(" asdf", "[asdf]")
{
    REQUIRE(1 == 1);
}

TEST_CASE( "is_joker", "[skat]")
{
    GamePlay gameplay = GamePlay(Trump::Hearts);
    REQUIRE(gameplay.is_joker(Card::DJ) == true);
    REQUIRE(gameplay.is_joker(Card::HJ) == true);
    REQUIRE(gameplay.is_joker(Card::SJ) == true);
    REQUIRE(gameplay.is_joker(Card::CJ) == true);

    REQUIRE(gameplay.is_joker(Card::H8) == false);
}

TEST_CASE( "get_color_and_jokers", "[skat]")
{
    GamePlay gameplay = GamePlay(Trump::Hearts);
    std::list<Card> cards = {Card::H9, Card::D7, Card::CQ, Card::HJ, Card::SJ};
    std::list<Card> colors_and_jokers = gameplay.get_color_and_jokers(1, cards);

    std::list<Card> true_colors_and_jokers = {Card::H9, Card::HJ, Card::SJ};
    REQUIRE(colors_and_jokers == true_colors_and_jokers);
}

TEST_CASE( "get_viable_cards trump", "[skat]")
{

    GamePlay gameplay = GamePlay(Trump::Hearts);
    std::list<Card> remaining_cards = {Card::H9, Card::D7, Card::DJ, Card::HJ, Card::CQ};
    std::list<Card> viable_cards = gameplay.get_viable_cards(Card::H8, remaining_cards);

    std::list<Card> true_viable_cards = {Card::H9, Card::DJ, Card::HJ};
    REQUIRE(viable_cards == true_viable_cards);
}

TEST_CASE( "get_viable_cards not trump", "[skat]")
{

    GamePlay gameplay = GamePlay(Trump::Hearts);
    std::list<Card> remaining_cards = {Card::H9, Card::D7, Card::DJ, Card::HJ, Card::CQ};
    std::list<Card> viable_cards = gameplay.get_viable_cards(Card::D8, remaining_cards);

    std::list<Card> true_viable_cards = {Card::D7};
    REQUIRE(viable_cards == true_viable_cards);
}

TEST_CASE( "get_winner", "[skat]")
{
    GamePlay gamePlay = GamePlay(Trump::Hearts);
    Card cards[] = {Card::HJ, Card::SJ, Card::D10};
    int winner = gamePlay.get_winner(cards);
    REQUIRE(winner == 1);
}

TEST_CASE( "get_next_plays", "[skat]")
{
    GamePlay gameplay = GamePlay(Trump::Diamonds,
                                 {Card::D8, Card::C8},
                                 {Card::S10, Card::SA},
                                 {Card::D10, Card::C9});

    std::list<std::list<Card>> next_plays = gameplay.get_next_plays();
    std::list<std::list<Card>> true_next_plays = {
            {Card::D8, Card::S10, Card::D10},
            {Card::D8, Card::SA, Card::D10},
            {Card::C8, Card::S10, Card::C9},
            {Card::C8, Card::SA, Card::C9}
    };

    REQUIRE(next_plays == true_next_plays);
}

TEST_CASE( "get_next_plays empty", "[skat]")
{
    GamePlay gameplay = GamePlay(Trump::Diamonds,
                                 {},
                                 {},
                                 {});

    std::list<std::list<Card>> next_plays = gameplay.get_next_plays();
    std::list<std::list<Card>> true_next_plays = {
    };

    REQUIRE(next_plays == true_next_plays);
}

TEST_CASE( "minmax no counterplay", "[minmax]")
{
    GamePlay gamePlay = GamePlay(0,
             Trump::Diamonds,
             {Card::CJ, Card::HJ},
             {Card::SJ, Card::SA},
             {Card::D10, Card::C9});
    MinMax minMax = MinMax(gamePlay);
    int max = minMax.max(0,0);
    REQUIRE(max == 27);
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