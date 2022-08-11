//
// Created by michel on 8/5/22.
//
#include "skat.h"
#include "minmax.h"


GamePlay::GamePlay(Trump trump) {
    this->trump = trump;
}

GamePlay::GamePlay(Trump trump, std::set<Card> forehandCards, std::set<Card> midhandCards,
std::set<Card> backhandCards)
{
    this->trump = trump;
    this->forehandCards = forehandCards;
    this->midhandCards = midhandCards;
    this->backhandCards = backhandCards;
    if(!cards_sanity(forehandCards, midhandCards, backhandCards, playedCards))
        throw std::invalid_argument("cards exist twice");
}

GamePlay::GamePlay(int max_player, Trump trump, std::set<Card> forehandCards, std::set<Card> midhandCards,
                   std::set<Card> backhandCards) {
    this->maxPlayer = max_player;
    this->trump = trump;
    this->forehandCards = forehandCards;
    this->midhandCards = midhandCards;
    this->backhandCards = backhandCards;
    if(!cards_sanity(forehandCards, midhandCards, backhandCards, playedCards))
        throw std::invalid_argument("cards exist twice");
}

GamePlay::GamePlay(int max_player, Trump trump, std::set<Card> forehandCards, std::set<Card> midhandCards,
                   std::set<Card> backhandCards, std::vector<Card> playedCards, std::vector<int> winners) {
    this->maxPlayer = max_player;
    this->trump = trump;
    this->forehandCards = forehandCards;
    this->midhandCards = midhandCards;
    this->backhandCards = backhandCards;
    this->playedCards = playedCards;
    this->winners = winners;
    if(!cards_sanity(forehandCards, midhandCards, backhandCards, playedCards))
        throw std::invalid_argument("card set imbalanced : either cards exist twice or imbalanced");
}

bool GamePlay::cards_sanity(std::set<Card> forehandCards, std::set<Card> midhandCards, std::set<Card> backhandCards, std::vector<Card> playedCards)
{
    bool imbalanced = forehandCards.size() < midhandCards.size() ||
        midhandCards.size() < backhandCards.size();
    if(imbalanced)
        return false;

    for(auto const& card : forehandCards)
    {
        if(std::find(playedCards.begin(), playedCards.end(), card) != playedCards.end())
            return false;
        playedCards.push_back(card);
    }
    for(auto const& card : midhandCards)
    {
        if(std::find(playedCards.begin(), playedCards.end(), card) != playedCards.end())
            return false;
        playedCards.push_back(card);
    }
    for(auto const& card : backhandCards)
    {
        if(std::find(playedCards.begin(), playedCards.end(), card) != playedCards.end())
            return false;
        playedCards.push_back(card);
    }
    return true;
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
    return second_winner ? 2 : first_winner;
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

bool GamePlay::color_in_deck(int color, std::set<Card> *deck)
{
    // no jokers!
    for(Card card : *deck)
    {
        if(get_color_of_card(card) == color && !is_joker(card))
            return true;
    }
    return false;
}

bool GamePlay::joker_in_deck(std::set<Card> *deck)
{
    for(Card card : *deck)
    {
        if(is_joker(card))
            return true;
    }
    return false;
}

void GamePlay::get_color_and_jokers(int color, std::set<Card> *deck)
{
    std::set<Card> result;
    for(Card card : *deck)
    {
        if(get_color_of_card(card) == color)
            result.insert(card);
        else if(is_joker(card))
            result.insert(card);
    }
}

void GamePlay::get_color(int color, std::set<Card> *deck)
{
    // without jokers!
    std::set<Card> result;
    for(Card card : *deck)
    {
        if(get_color_of_card(card) == color && !is_joker(card))
            result.insert(card);
    }
}

void GamePlay::get_viable_cards(Card first_card, std::set<Card> *remaining_cards)
{
    int first_card_color = get_color_of_card(first_card);
    bool trump_played = is_trump(first_card);
    if(trump_played)
    {
        bool has_trump = color_in_deck(static_cast<int>(trump), remaining_cards) || joker_in_deck(remaining_cards);
        if(has_trump)
        {
            get_color_and_jokers(static_cast<int>(trump), remaining_cards);
        }
    } else {
        bool has_color = color_in_deck(first_card_color, remaining_cards);
        if(has_color)
        {
            get_color(first_card_color, remaining_cards);
        }
    }
}

std::set<Card> GamePlay::get_possible_next_moves()
{
    bool new_play = is_new_play();
    int current_player = get_current_player();
    std::set<Card> current_player_cards;

    if(current_player == 0) {
        current_player_cards = forehandCards;
    } else if(current_player == 1) {
        current_player_cards = midhandCards;
    } else //if(current_player == 2)
    {
        current_player_cards = backhandCards;
    }

    if(!new_play)
    {
        Card first_card = get_first_card();
        get_viable_cards(first_card, &current_player_cards);
    }

    return current_player_cards;
}

void GamePlay::make_move(Card move)
{
    int player = get_current_player();
    playedCards.push_back(move);

    if(player == 0)
    {
        forehandCards.erase(move);
    } else if (player == 1)
    {
        midhandCards.erase(move);
    } else // if (player == 2)
    {
        backhandCards.erase(move);
    }

    // eval winner
    if(playedCards.size() % 3 == 0)
    {
        Card cards[3] = {this->playedCards[this->playedCards.size() - 3],
                         this->playedCards[this->playedCards.size() - 2],
                         this->playedCards[this->playedCards.size() - 1]};
        int winner = get_winner(cards);
        if(winners.size())
        {
            winner = (winners.back() + winner) % 3;
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

void GamePlay::revert_move()
{
    Card move = playedCards.back();

    // revert winners
    if(playedCards.size() % 3 == 0)
    {
        winners.pop_back();
    }

    int last_player = get_previous_player();

    if(last_player == 0)
    {
        forehandCards.insert(move);
    } else if(last_player == 1)
    {
        midhandCards.insert(move);
    } else // if(last_player == 2)
    {
        backhandCards.insert(move);
    }

    playedCards.pop_back();
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
    for(int i=0;i < playedCards.size()/3;++i)
    {
        Card first_card = playedCards[i*3];
        Card second_card = playedCards[i*3+1];
        Card third_card = playedCards[i*3+2];

        Card play[3] = {first_card, second_card, third_card};
        int winner = this->get_winner(play);
        if((winner + start_player) % 3 == maxPlayer)
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
