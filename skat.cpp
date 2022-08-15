//
// Created by michel on 8/5/22.
//
#include "skat.h"
#include "minmax.h"

Card::Card(CardType card_type) :
    card_type(card_type)
{
    card_joker = (static_cast<int>(card_type) + 1) % 8 == 0;
    card_color = static_cast<int>(card_type) / 8;
}

int Card::get_card_color()
{
    return card_color;
}

bool Card::is_joker()
{
    return card_joker;
}

bool Card::is_color(int color)
{
    return card_color == color;
}

Hand::Hand()
{
}

Hand::Hand(std::set<CardType> deck)
{
    for(const CardType &card_type : deck){
        add_card(Card(card_type));
    }
}

bool Hand::has_card(Card card)
{
    return cards.find(card) != cards.end();
}

bool Hand::has_color(int color)
{
    return colors[color].size() > 0;
}

bool Hand::has_joker()
{
    return jokers.size() > 0;
}

void Hand::remove_card(Card card)
{
    cards.erase(card);
    if(card.is_joker())
        jokers.erase(card);
    else
        colors[card.get_card_color()].erase(card);
}

void Hand::add_card(Card card)
{
    cards.insert(card);

    if(card.is_joker())
        jokers.insert(card);
    else
        colors[card.get_card_color()].insert(card);
}

std::set<Card> Hand::get_cards()
{
    return cards;
}

std::set<Card> Hand::get_jokers()
{
    return jokers;
}

std::set<Card> Hand::get_color(int color)
{
    return colors[color];
}

std::set<Card> Hand::get_color_and_jokers(int color)
{
    std::set<Card> colors_and_jokers(colors[color]);
    colors_and_jokers.insert(jokers.begin(), jokers.end());
    return colors_and_jokers;
}

GamePlay::GamePlay(Trump trump) :
    trump(trump)
{
}

GamePlay::GamePlay(Trump trump, std::set<CardType> forehandCards, std::set<CardType> midhandCards,
std::set<CardType> backhandCards) :
    trump(trump)
{
    if(!cards_sanity(forehandCards, midhandCards, backhandCards, std::vector<CardType>()))
        throw std::invalid_argument("cards exist twice");

    forehand = Hand(forehandCards);
    midhand = Hand(midhandCards);
    backhand = Hand(backhandCards);
}

GamePlay::GamePlay(int max_player, Trump trump, std::set<CardType> forehandCards, std::set<CardType> midhandCards,
                   std::set<CardType> backhandCards) :
                   trump(trump),
                   max_player(max_player)
                   {
    if(!cards_sanity(forehandCards, midhandCards, backhandCards, std::vector<CardType>()))
        throw std::invalid_argument("cards exist twice");

   forehand = Hand(forehandCards);
   midhand = Hand(midhandCards);
   backhand = Hand(backhandCards);
}

GamePlay::GamePlay(int max_player, Trump trump, std::set<CardType> forehandCards, std::set<CardType> midhandCards,
                   std::set<CardType> backhandCards, std::vector<CardType> played_card_types, std::vector<int> winners) :
                   trump(trump),
                   max_player(max_player),
                   winners(winners)
                   {
    if(!cards_sanity(forehandCards, midhandCards, backhandCards, played_card_types))
        throw std::invalid_argument("card set imbalanced : either cards exist twice or imbalanced");

    forehand = Hand(forehandCards);
    midhand = Hand(midhandCards);
    backhand = Hand(backhandCards);
    std::transform(played_card_types.begin(), played_card_types.end(), played_cards.begin(),
                   [](CardType card_type) {return Card(card_type);});
}

bool GamePlay::cards_sanity(std::set<CardType> forehandCards, std::set<CardType> midhandCards,
                            std::set<CardType> backhandCards, std::vector<CardType> played_card_types)
{
    bool imbalanced = forehandCards.size() < midhandCards.size() ||
        midhandCards.size() < backhandCards.size();
    if(imbalanced)
        return false;

    for(auto const& card : forehandCards)
    {
        if(std::find(played_card_types.begin(), played_card_types.end(), card) != played_card_types.end())
            return false;
        played_card_types.push_back(card);
    }
    for(auto const& card : midhandCards)
    {
        if(std::find(played_card_types.begin(), played_card_types.end(), card) != played_card_types.end())
            return false;
        played_card_types.push_back(card);
    }
    for(auto const& card : backhandCards)
    {
        if(std::find(played_card_types.begin(), played_card_types.end(), card) != played_card_types.end())
            return false;
        played_card_types.push_back(card);
    }
    return true;
}

bool GamePlay::is_trump(Card card)
{
    return card.is_joker() || card.is_color(static_cast<int>(trump));
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
        bool first_card_joker = first_card.is_joker();
        bool second_card_joker = second_card.is_joker();

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
        if(first_card.get_card_color() == second_card.get_card_color())
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
    return !(played_cards.size() % 3);
}

int GamePlay::get_current_player() {
    if(played_cards.size() > 2)
    {
        int last_winner = winners[played_cards.size() / 3 - 1];
        return (last_winner + played_cards.size() % 3) % 3;
    } else {
        return played_cards.size();
    }
}

Card GamePlay::get_first_card()
{
    return played_cards[(played_cards.size() - 1) / 3 * 3];
}

std::set<Card> GamePlay::get_viable_cards(Card first_card, Hand hand)
{
    int first_card_color = first_card.get_card_color();
    if(is_trump(first_card))
    {
        bool has_trump = hand.has_color(static_cast<int>(trump)) || hand.has_joker();
        if(has_trump)
        {
            return hand.get_color_and_jokers(static_cast<int>(trump));
        }
    } else if(hand.has_color(first_card_color)) {
        return hand.get_color(first_card_color);
    }
    return hand.get_cards();
}

std::set<Card> GamePlay::get_possible_next_moves()
{
    bool new_play = is_new_play();
    int current_player = get_current_player();
    Hand current_hand;

    if (current_player == 0) {
        current_hand = forehand;
    } else if (current_player == 1) {
        current_hand = midhand;
    } else //if(current_player == 2)
    {
        current_hand = backhand;
    }
    if(new_play)
    {
        return current_hand.get_cards();
    } else
    {
        Card first_card = get_first_card();
        return get_viable_cards(first_card, current_hand);
    }
}

void GamePlay::make_move(Card card)
{
    int player = get_current_player();
    played_cards.push_back(card);

    if(player == 0)
    {
        forehand.remove_card(card);
    } else if (player == 1)
    {
        midhand.remove_card(card);
    } else // if (player == 2)
    {
        backhand.remove_card(card);
    }

    // eval winner
    if(played_cards.size() % 3 == 0)
    {
        Card cards[3] = {played_cards[played_cards.size() - 3],
                         played_cards[played_cards.size() - 2],
                         played_cards[played_cards.size() - 1]};
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
    if(played_cards.size() > 3)
    {
        // get last winner
        int winner_last_round = winners[winners.size()-1];
        int offset = (played_cards.size() - 1) % 3;
        return (winner_last_round + offset) % 3;
    } else {
        return played_cards.size() - 1;
    }
}

void GamePlay::revert_move()
{
    Card card = played_cards.back();

    // revert winners
    if(played_cards.size() % 3 == 0)
    {
        winners.pop_back();
    }

    int last_player = get_previous_player();

    if(last_player == 0)
    {
        forehand.add_card(card);
    } else if(last_player == 1)
    {
        midhand.add_card(card);
    } else // if(last_player == 2)
    {
        backhand.add_card(card);
    }

    played_cards.pop_back();
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

std::pair<int, int> GamePlay::eval_depth(int depth, int start_player)
{
    int result = 0;
    Card first_card = played_cards[depth - 2];
    Card second_card = played_cards[depth - 1];
    Card third_card = played_cards[depth];

    Card play[3] = {first_card, second_card, third_card};
    int winner = this->get_winner(play);
    if((winner + start_player) % 3 == max_player)
    {
        return std::pair<int, int>((winner + start_player) % 3, this->get_points_of_hand(play));
    } else {
        return std::pair<int, int>((winner + start_player) % 3, 0);
    }
}

int GamePlay::eval() {
    int result = 0;
    int start_player = 0;
    for(int i=0; i < played_cards.size() / 3; ++i)
    {
        Card first_card = played_cards[i * 3];
        Card second_card = played_cards[i * 3 + 1];
        Card third_card = played_cards[i * 3 + 2];

        Card play[3] = {first_card, second_card, third_card};
        int winner = this->get_winner(play);
        if((winner + start_player) % 3 == max_player)
        {
            result += this->get_points_of_hand(play);
        }
        start_player = (winner + start_player) % 3;
    }
    return result;
}

int GamePlay::getMaxPlayer() const {
    return max_player;
}