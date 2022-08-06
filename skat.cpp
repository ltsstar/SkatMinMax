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
    this->forehandCards = forehandCards;
    this->midhandCards = midhandCards;
    this->backhandCards = backhandCards;
}

GamePlay::GamePlay(int max_player, Trump trump, std::list<Card> forehandCards, std::list<Card> midhandCards,
                   std::list<Card> backhandCards) {
    this->maxPlayer = max_player;
    this->trump = trump;
    this->forehandCards = forehandCards;
    this->midhandCards = midhandCards;
    this->backhandCards = backhandCards;
}

void GamePlay::add_played_card(Card played_card) {
    this->playedCards.push_back(played_card);
}

void GamePlay::set_played_cards(std::list<Card> played_cards) {
    std::vector<Card> played_cards_vector(played_cards.size());
    std::copy(played_cards.begin(), played_cards.end(), played_cards_vector.begin());
    this->playedCards = played_cards_vector;
}

bool GamePlay::is_trump(Card card)
{
    int int_card = static_cast<int>(card);
    return int_card / 8 == static_cast<int>(trump) || this->is_joker(card);
}

bool GamePlay::is_joker(Card card)
{
    return !((static_cast<int>(card) + 1) % 8);
}

int GamePlay::get_better_card(Card first_card, Card second_card)
{
    if(first_card == Card::CJ || second_card == Card::CJ)
        int a = 5;
    if(this->is_trump(first_card) && !this->is_trump(second_card))
    {
        return 0;
    } else if(!this->is_trump(first_card) && this->is_trump(second_card))
    {
        return 1;
    } else if(this->is_trump(first_card) && this->is_trump(second_card))
    {
        if(this->is_joker(first_card) && this->is_joker(second_card))
        {
            return static_cast<int>(second_card) > static_cast<int>(first_card);
        } else if(!this->is_joker(first_card) && this->is_joker(second_card))
        {
            return 1;
        } else if(this->is_joker(first_card) && !this->is_joker(second_card)) {
            return 0;
        } else if(!this->is_joker(first_card) && !this->is_joker(second_card))
        {
            return static_cast<int>(second_card) > static_cast<int>(first_card);
        }
    } else if(!this->is_trump(first_card) && !this->is_trump(second_card))
    {
        if(this->get_color_of_card(first_card) == this->get_color_of_card(second_card))
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
    return first_winner + second_winner;
}

bool GamePlay::is_new_play()
{
    return !(this->playedCards.size() % 3);
}

int GamePlay::get_current_player() {
    if(this->playedCards.size() > 2)
    {
        int last_winner = winners[this->playedCards.size() / 3 - 1];
        return (last_winner + this->playedCards.size() % 3) % 3;
    } else {
        return playedCards.size();
    }
}

Card GamePlay::get_first_card()
{
    return this->playedCards[(this->playedCards.size() - 1) / 3 * 3];
}

int GamePlay::get_color_of_card(Card card)
{
    return static_cast<int>(card) / 8;
}

bool GamePlay::color_in_deck(int color, std::list<Card> deck)
{
    for(auto const& card : deck)
    {
        int remainder = static_cast<int>(card) - (8*color);
        // excludes jokers!
        if(0 <= remainder && remainder < 7)
            return true;
    }
    return false;
}

bool GamePlay::joker_in_deck(std::list<Card> deck)
{
    for(auto const& card : deck)
    {
        if((static_cast<int>(card) + 1) % 8 == 0)
            return true;
    }
    return false;
}

std::list<Card> GamePlay::get_color_and_jokers(int color, std::list<Card> cards)
{
    std::list<Card> results;
    for(auto const& card : cards)
    {
        if(this->get_color_of_card(card) == color)
            results.push_back(card);
        else if(this->is_joker(card))
            results.push_back(card);
    }
    return results;
}

/**
 * Gets colors in deck WITHOUT JOKERS
 * @param color
 * @param cards
 * @return
 */
std::list<Card> GamePlay::get_color(int color, std::list<Card> cards)
{
    std::list<Card> results;
    for(auto const& card : cards)
    {
        if(this->get_color_of_card(card) == color && !this->is_joker(card))
            results.push_back(card);
    }
    return results;
}

std::list<Card> GamePlay::get_viable_cards(Card first_card, std::list<Card> remaining_cards)
{
    int first_card_color = this->get_color_of_card(first_card);
    bool is_trump = this->is_trump(first_card);
    if(is_trump)
    {
        bool has_trump = this->color_in_deck(static_cast<int>(this->trump), remaining_cards) || this->joker_in_deck(remaining_cards);
        if(has_trump)
        {
            return this->get_color_and_jokers(static_cast<int>(this->trump), remaining_cards);
        } else {
            return remaining_cards;
        }
    } else {
        bool has_color = this->color_in_deck(first_card_color, remaining_cards);
        if(has_color)
        {
            return this->get_color(first_card_color, remaining_cards);
        } else {
            return remaining_cards;
        }
    }
}

std::list<Card> GamePlay::get_possible_next_moves()
{
    bool new_play = this->is_new_play();
    int current_player = this->get_current_player();
    std::list<Card> current_player_cards;

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
        return this->get_viable_cards(this->get_first_card(),current_player_cards);
    }
}

void GamePlay::make_move(Card move) {
    int player = get_current_player();


    playedCards.push_back(move);
    if(player == 0)
    {
        forehandCards.remove(move);
    } else if (player == 1)
    {
        midhandCards.remove(move);
    } else if (player == 2)
    {
        backhandCards.remove(move);
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
    Card move = this->playedCards.back();
    int last_player = get_previous_player();

    if(last_player == 0)
    {
        this->forehandCards.push_back(move);
    } else if(last_player == 1)
    {
        this->midhandCards.push_back(move);
    } else if(last_player == 2)
    {
        this->backhandCards.push_back(move);
    }


    // revert winners
    if(playedCards.size() % 3 == 0)
    {
        winners.pop_back();
    }

    this->playedCards.pop_back();
}

std::list<std::list<Card>> GamePlay::get_next_plays()
{
    std::list<std::list<Card>> result;
    std::list<Card> next_first_moves = this->get_possible_next_moves();
    for(auto const& next_first_move : next_first_moves)
    {
        this->make_move(next_first_move);
        std::list<Card> next_second_moves = this->get_possible_next_moves();
        for(auto const& next_second_move : next_second_moves)
        {
            this->make_move(next_second_move);
            std::list<Card> next_third_moves = this->get_possible_next_moves();
            for(auto const& next_third_move : next_third_moves)
            {
                result.push_back({next_first_move, next_second_move, next_third_move});
            }
            this->revert_move();
        }
        this->revert_move();
    }
    return result;
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
        res += this->get_points_of_card(play[i]);
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
