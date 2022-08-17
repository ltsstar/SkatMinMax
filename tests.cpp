//
// Created by michel on 8/5/22.
//
#include <catch2/catch_test_macros.hpp>
#include "skat.h"
#include "minmax.h"



TEST_CASE( "get_previous_player", "[skat]")
{
    /*
     * ****
    GamePlay gamePlay = GamePlay(0,
                                 Trump::Diamonds,
                                 {Card::CJ, Card::HJ, Card::D7, Card::CA, Card::C9, Card::D7, Card::SQ, Card::S9, Card::H9, Card::H8},
                                 {Card::SJ, Card::DJ, Card::DA, Card::D10, Card::C10, Card::CQ, Card::S10, Card::S8, Card::HQ, Card::H7},
                                 {Card::DK, Card::DQ, Card::D8, Card::CK, Card::C8, Card::SK, Card::S7, Card::HA, Card::H10, Card::HK});
    MinMax minMax = MinMax(gamePlay);
    std::pair<int, std::vector<Card>> res = minMax.max(0,0);
     * ****
     <-- with this as start
     */
    GamePlay gamePlay = GamePlay(0,
                                 Trump::Diamonds,
                                 {},
                                 {CardType::H7},
                                 {CardType::HK},
                                 {CardType::CJ, CardType::SJ, CardType::DK, CardType::HJ, CardType::DJ, CardType::DQ,
                                  CardType::D7, CardType::D8, CardType::D10, CardType::CK, CardType::CA, CardType::C10,
                                  CardType::C8, CardType::C9, CardType::CQ, CardType::SK, CardType::SQ, CardType::S10,
                                  CardType::S7, CardType::S9, CardType::S8, CardType::HA, CardType::H9, CardType::HQ,
                                  CardType::H10, CardType::H8},
                                 {0, 0, 1, 1, 1, 1, 1, 1, 2});
    gamePlay.get_previous_player();

}

TEST_CASE( "cards exist twice", "[smallgame]")
{
    GamePlay gamePlay = GamePlay(0,
                                 Trump::Diamonds,
                                 {CardType::CJ, CardType::CJ},
                                 {CardType::DK, CardType::SA},
                                 {CardType::D10, CardType::C9});
    MinMax minMax = MinMax(gamePlay);
    std::pair<int, std::vector<Card*>> res = minMax.max(0, -1,121);
    REQUIRE(res.first == 29);
}

TEST_CASE( "minmax no counterplay possible", "[smallgame]")
{
    GamePlay gamePlay = GamePlay(0,
             Trump::Diamonds,
             {CardType::CJ, CardType::HJ},
             {CardType::DK, CardType::SA},
             {CardType::D10, CardType::C9});
    MinMax minMax = MinMax(gamePlay);
    std::pair<int, std::vector<Card*>> res = minMax.max(0,-1,121);
    REQUIRE(res.first == 29);
}

TEST_CASE( "minmax counterplay", "[smallgame]")
{
    GamePlay gamePlay = GamePlay(0,
                                 Trump::Diamonds,
                                 {CardType::CJ, CardType::HJ},
                                 {CardType::SJ, CardType::DA},
                                 {CardType::D10, CardType::C9});
    MinMax minMax = MinMax(gamePlay);
    std::pair<int, std::vector<Card*>> res = minMax.max(0,-1,121);
    REQUIRE(res.first == 23);
}

TEST_CASE( "minmax counterplay 2", "[smallgame]")
{
    GamePlay gamePlay = GamePlay(0,
                                 Trump::Diamonds,
                                 {CardType::CJ, CardType::HJ},
                                 {CardType::SJ, CardType::DJ},
                                 {CardType::DK, CardType::DQ});
    MinMax minMax = MinMax(gamePlay);
    std::pair<int, std::vector<Card*>> res = minMax.max(0,-1,121);
    REQUIRE(res.first == 7);
}

TEST_CASE( "minmax big game", "[minmax]")
{
    GamePlay gamePlay = GamePlay(0,
                                 Trump::Spades,
                                 {CardType::CJ, CardType::SJ, CardType::DJ, CardType::SA, CardType::S10, CardType::SQ,
                                  CardType::S8, CardType::CA, CardType::H8, CardType::H7},
                                 {CardType::HJ, CardType::SK, CardType::S9, CardType::C10, CardType::C8, CardType::C7,
                                  CardType::HQ, CardType::D9, CardType::D8, CardType::D7},
                                 {CardType::S7, CardType::CK, CardType::C9, CardType::HA, CardType::H10, CardType::HK,
                                  CardType::DA, CardType::D10, CardType::DK, CardType::DQ});
    MinMax minMax = MinMax(gamePlay);
    std::pair<int, std::vector<Card*>> res = minMax.max(0,-1,121);
    REQUIRE(res.first == 2);
}

TEST_CASE( "minmax big game 2", "[minmax]")
{
    GamePlay gamePlay = GamePlay(0,
                                 Trump::Diamonds,
                                 {CardType::CJ, CardType::SJ, CardType::DJ, CardType::C10, CardType::C9, CardType::S10, CardType::SQ, CardType::DA, CardType::D10, CardType::DQ},
                                 {CardType::HJ, CardType::CQ, CardType::C8, CardType::C7, CardType::SA, CardType::S7, CardType::HA, CardType::H8, CardType::DK, CardType::D8},
                                 {CardType::CA, CardType::CK, CardType::SK, CardType::S9, CardType::S8, CardType::H10, CardType::H9, CardType::H7, CardType::D9, CardType::D7});
    MinMax minMax = MinMax(gamePlay);
    std::pair<int, std::vector<Card*>> res = minMax.max(0,-1,121);
    REQUIRE(res.first == 2);
}


TEST_CASE( "minmax safe total win", "[minmax]")
{
    GamePlay gamePlay = GamePlay(0,
                                 Trump::Diamonds,
                                 {CardType::CJ, CardType::SJ, CardType::HJ, CardType::DJ, CardType::DA, CardType::D10, CardType::DK, CardType::DQ, CardType::D9, CardType::D8},
                                 {CardType::H7, CardType::H8, CardType::H9, CardType::HQ, CardType::HK, CardType::H10, CardType::HA, CardType::S7, CardType::S8, CardType::S9},
                                 {CardType::SQ, CardType::SK, CardType::S10, CardType::SA, CardType::C7, CardType::S8, CardType::S9, CardType::CQ, CardType::CK, CardType::C10});
    // hand: CA, D7
    MinMax minMax = MinMax(gamePlay);
    std::pair<int, std::vector<Card*>> res = minMax.max(0,-1,121);
    REQUIRE(res.first == 2);
}