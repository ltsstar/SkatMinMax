//
// Created by michel on 8/5/22.
//
#include "skat.h"
#include "minmax.h"


GamePlay::GamePlay(Trump trump) {
    this->trump = trump;
}

GamePlay::GamePlay(Trump trump, std::list<Card> forehandCards, std::list<Card> midhandCards,
std::list<Card> backhandCards)
{
    this->trump = trump;
    this->forehandCards = card_list_to_bitset(forehandCards);
    this->midhandCards = card_list_to_bitset(midhandCards);
    this->backhandCards = card_list_to_bitset(backhandCards);
}

GamePlay::GamePlay(int max_player, Trump trump, std::list<Card> forehandCards, std::list<Card> midhandCards,
                   std::list<Card> backhandCards) {
    this->maxPlayer = max_player;
    this->trump = trump;
    this->forehandCards = card_list_to_bitset(forehandCards);
    this->midhandCards = card_list_to_bitset(midhandCards);
    this->backhandCards = card_list_to_bitset(backhandCards);
}

std::bitset<32> GamePlay::card_list_to_bitset(std::list<Card> cards)
{
    std::bitset<32> result;
    for(auto const& card : cards)
    {
        result |= std::bitset<32>(1) << static_cast<int>(card);
    }
    return result;
}

bool GamePlay::is_trump(Card card)
{
    int int_card = static_cast<int>(card);
    return int_card / 8 == static_cast<int>(trump) || is_joker(card);
}

bool GamePlay::is_joker(Card card)
{
    return !((static_cast<int>(card) + 1) % 8);
}

int GamePlay::get_better_card(Card first_card, Card second_card)
{
    bool first_card_trump = is_trump(first_card);
    bool second_card_trump = is_trump(second_card);

    if(first_card_trump && !second_card_trump)
    {
        return 0;
    } else if(!first_card_trump && second_card_trump)
    {
        return 1;
    } else if(first_card_trump && second_card_trump)
    {
        bool first_card_joker = is_joker(first_card);
        bool second_card_joker = is_joker(second_card);

        if(!first_card_joker && second_card_joker)
        {
            return 1;
        } else if(first_card_joker && !second_card_joker) {
            return 0;
        } else
        {
            return static_cast<int>(second_card) > static_cast<int>(first_card);
        }
    } else // else if(!this->is_trump(first_card) && !this->is_trump(second_card))
    {
        if(get_color_of_card(first_card) == get_color_of_card(second_card))
        {
            return static_cast<int>(second_card) > static_cast<int>(first_card);
        } else {
            return 0;
        }
    }
}

int GamePlay::get_winner(Card cards[3])
{
    int first_winner = get_better_card(cards[0], cards[1]);
    int second_winner = get_better_card(cards[first_winner], cards[2]);
    return first_winner + 2 * second_winner;
}

bool GamePlay::is_new_play()
{
    return !(playedCards.size() % 3);
}

int GamePlay::get_current_player() {
    if(playedCards.size() > 2)
    {
        int last_winner = winners[playedCards.size() / 3 - 1];
        return (last_winner + playedCards.size() % 3) % 3;
    } else {
        return playedCards.size();
    }
}

Card GamePlay::get_first_card()
{
    return playedCards[(playedCards.size() - 1) / 3 * 3];
}

int GamePlay::get_color_of_card(Card card)
{
    return static_cast<int>(card) / 8;
}

bool GamePlay::color_in_deck(int color, std::bitset<32> deck)
{
    return get_color(color, deck).any();
}

bool GamePlay::joker_in_deck(std::bitset<32> deck)
{
    const std::bitset<32> jokers = 0b10000000100000001000000010000000;
    return (deck & jokers).any();
}

std::bitset<32> GamePlay::get_color_and_jokers(int color, std::bitset<32> cards)
{
    const std::bitset<32> jokers = 0b10000000100000001000000010000000;
    return get_color(color, cards) | (jokers & cards);
}

/**
 * Gets colors in deck WITHOUT JOKERS
 * @param color
 * @param cards
 * @return
 */
std::bitset<32> GamePlay::get_color(int color, std::bitset<32> cards)
{
    const std::bitset<32> diamonds =0b00000000000000000000000001111111;
    std::bitset<32> color_shift = diamonds << 8*color;
    return cards & color_shift;
}

std::bitset<32> GamePlay::get_viable_cards(Card first_card, std::bitset<32> remaining_cards)
{
    int first_card_color = get_color_of_card(first_card);
    bool trump = is_trump(first_card);
    if(trump)
    {
        bool has_trump = color_in_deck(static_cast<int>(trump), remaining_cards) || joker_in_deck(remaining_cards);
        if(has_trump)
        {
            return get_color_and_jokers(static_cast<int>(trump), remaining_cards);
        } else {
            return remaining_cards;
        }
    } else {
        bool has_color = color_in_deck(first_card_color, remaining_cards);
        if(has_color)
        {
            return get_color(first_card_color, remaining_cards);
        } else {
            return remaining_cards;
        }
    }
}

std::bitset<32> GamePlay::get_possible_next_moves()
{
    bool new_play = is_new_play();
    int current_player = get_current_player();
    std::bitset<32> current_player_cards;

    if(current_player == 0) {
        current_player_cards = forehandCards;
    } else if(current_player == 1) {
        current_player_cards = midhandCards;
    } else if(current_player == 2) {
        current_player_cards = backhandCards;
    }
    if(new_play) {
        return current_player_cards;
    } else
    {
        return get_viable_cards(get_first_card(),current_player_cards);
    }
}

void GamePlay::make_move(Card move) {
    int player = get_current_player();


    playedCards.push_back(move);
    if(player == 0)
    {
        forehandCards  ^= std::bitset<32>(1) << static_cast<int>(move);
    } else if (player == 1)
    {
        midhandCards  ^= std::bitset<32>(1) << static_cast<int>(move);
    } else // if (player == 2)
    {
        backhandCards ^= std::bitset<32>(1) << static_cast<int>(move);
    }

    // eval winner
    if(playedCards.size() % 3 == 0)
    {
        Card cards[3] = {this->playedCards[this->playedCards.size() - 3],
                         this->playedCards[this->playedCards.size() - 2],
                         this->playedCards[this->playedCards.size() - 1]};
        int winner;
        if(winners.size())
        {
            winner = get_winner(cards);
            winner = (winners.back() + winner) % 3;
        } else {
            winner = get_winner(cards);
        }
        winners.push_back(winner);
    }
}

int GamePlay::get_previous_player()
{
    if(playedCards.size() > 3)
    {
        // get last winner
        int winner_last_round = winners[winners.size()-1];
        int offset = (playedCards.size() - 1) % 3;
        return (winner_last_round + offset) % 3;
    } else {
        return playedCards.size() - 1;
    }
}

void GamePlay::revert_move() {
    Card move = playedCards.back();
    int last_player = get_previous_player();

    if(last_player == 0)
    {
        forehandCards ^= std::bitset<32>(1) << static_cast<int>(move);
    } else if(last_player == 1)
    {
        midhandCards ^= std::bitset<32>(1) << static_cast<int>(move);
    } else // if(last_player == 2)
    {
        backhandCards ^= std::bitset<32>(1) << static_cast<int>(move);
    }


    // revert winners
    if(playedCards.size() % 3 == 0)
    {
        winners.pop_back();
    }

    this->playedCards.pop_back();
}

int GamePlay::get_points_of_card(Card card)
{
    int card_int = static_cast<int>(card) % 8;
    if(card_int <= 2)
        return 0;
    else if(card_int == 5) //10
        return 10;
    else if(card_int == 6) //Ace
        return 11;
    else if(card_int == 7)
        return 2;
    else
        return card_int;
}

int GamePlay::get_points_of_hand(Card play[3])
{
    int res = 0;
    for(int i=0;i<3;++i)
    {
        res += get_points_of_card(play[i]);
    }
    return res;
}

int GamePlay::eval() {
    int result = 0;
    int start_player = 0;
    for(int i=0;i < this->playedCards.size()/3;++i)
    {
        Card first_card = this->playedCards[i*3];
        Card second_card = this->playedCards[i*3+1];
        Card third_card = this->playedCards[i*3+2];

        Card play[3] = {first_card, second_card, third_card};
        int winner = this->get_winner(play);
        if((winner + start_player) % 3 == this->maxPlayer)
        {
            result += this->get_points_of_hand(play);
        }
        start_player = (winner + start_player) % 3;
    }
    return result;
}

int GamePlay::getMaxPlayer() const {
    return maxPlayer;
}
