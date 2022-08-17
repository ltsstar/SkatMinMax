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
        Card* card = new Card(card_type);
        add_card(card);
    }
}

bool Hand::has_color(int color)
{
    return colors[color].size() > 0;
}

bool Hand::has_joker()
{
    return jokers.size() > 0;
}

void Hand::remove_card(Card* card)
{
    if(card->is_joker())
        jokers.erase(std::find(jokers.begin(), jokers.end(), card));
    else
        colors[card->get_card_color()].erase(std::find(colors[card->get_card_color()].begin(),
                                                      colors[card->get_card_color()].end(), card));
}

void Hand::add_card(Card* card)
{
    if(card->is_joker())
        jokers.push_back(card);
    else
        colors[card->get_card_color()].push_back(card);
}

std::vector<Card*> Hand::get_cards()
{
    std::vector<Card*> res(jokers);
    for(int i=0; i<4; ++i)
    {
       res.insert(res.end(), colors[i].begin(), colors[i].end());
    }
    return res;
}

Card* Hand::get_last_card()
{
    if(jokers.size())
        return jokers.back();
    for(int i=0; i<4; ++i)
    {
        if(colors[i].size()>0)
            return colors[i].back();
    }
}

std::vector<Card*> Hand::get_jokers()
{
    return jokers;
}

std::vector<Card*> Hand::get_color(int color)
{
    return colors[color];
}

std::vector<Card*> Hand::get_color_and_jokers(int color)
{
    std::vector<Card*> colors_and_jokers(colors[color]);
    colors_and_jokers.insert(colors_and_jokers.end(), jokers.begin(), jokers.end());
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

    forehand = new Hand(forehandCards);
    midhand = new Hand(midhandCards);
    backhand = new Hand(backhandCards);
}

GamePlay::GamePlay(int max_player, Trump trump, std::set<CardType> forehandCards, std::set<CardType> midhandCards,
                   std::set<CardType> backhandCards) :
                   trump(trump),
                   max_player(max_player)
                   {
    if(!cards_sanity(forehandCards, midhandCards, backhandCards, std::vector<CardType>()))
        throw std::invalid_argument("cards exist twice");

   forehand = new Hand(forehandCards);
   midhand = new Hand(midhandCards);
   backhand = new Hand(backhandCards);
}

GamePlay::GamePlay(int max_player, Trump trump, std::set<CardType> forehandCards, std::set<CardType> midhandCards,
                   std::set<CardType> backhandCards, std::vector<CardType> played_card_types, std::vector<int> winners) :
                   trump(trump),
                   max_player(max_player),
                   winners(winners)
                   {
    if(!cards_sanity(forehandCards, midhandCards, backhandCards, played_card_types))
        throw std::invalid_argument("card set imbalanced : either cards exist twice or imbalanced");

    forehand = new Hand(forehandCards);
    midhand = new Hand(midhandCards);
    backhand = new Hand(backhandCards);
    std::transform(played_card_types.begin(), played_card_types.end(), played_cards.begin(),
                   [](CardType card_type) {return new Card(card_type);});
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

bool GamePlay::is_trump(Card* card)
{
    return card->is_joker() || card->is_color(static_cast<int>(trump));
}

int GamePlay::get_better_card(Card* first_card, Card* second_card)
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
        bool first_card_joker = first_card->is_joker();
        bool second_card_joker = second_card->is_joker();

        if(!first_card_joker && second_card_joker)
        {
            return 1;
        } else if(first_card_joker && !second_card_joker) {
            return 0;
        } else
        {
            return static_cast<int>(*second_card) > static_cast<int>(*first_card);
        }
    } else // else if(!this->is_trump(first_card) && !this->is_trump(second_card))
    {
        if(first_card->get_card_color() == second_card->get_card_color())
        {
            return static_cast<int>(*second_card) > static_cast<int>(*first_card);
        } else {
            return 0;
        }
    }
}

int GamePlay::get_winner(Card* cards[3])
{
    int first_winner = get_better_card(cards[0], cards[1]);
    int second_winner = get_better_card(cards[first_winner], cards[2]);
    return second_winner ? 2 : first_winner;
}

bool GamePlay::is_new_play()
{
    return !(played_cards.size() % 3);
}

bool GamePlay::is_last_play()
{
    return played_cards.size() % 3 == 2;
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

Card* GamePlay::get_first_card()
{
    return played_cards[(played_cards.size() - 1) / 3 * 3];
}

std::vector<Card*> GamePlay::get_viable_cards(Card* first_card, Hand* hand)
{
    int first_card_color = first_card->get_card_color();
    if(is_trump(first_card))
    {
        bool has_trump = hand->has_color(static_cast<int>(trump)) || hand->has_joker();
        if(has_trump)
        {
            return hand->get_color_and_jokers(static_cast<int>(trump));
        }
    } else if(hand->has_color(first_card_color)) {
        return hand->get_color(first_card_color);
    }
    return hand->get_cards();
}

std::vector<Card*> GamePlay::get_possible_next_moves()
{
    bool new_play = is_new_play();
    int current_player = get_current_player();
    Hand* current_hand;

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
        return current_hand->get_cards();
    } else
    {
        Card* first_card = get_first_card();
        return get_viable_cards(first_card, current_hand);
    }
}

std::vector<Card*> GamePlay::reduce_hand_equivalencies(Hand* player_hand)
{
    std::vector<Card*> result;
    for(int i=0; i<4; ++i)
    {
        if(player_hand->has_color(i))
        {
            std::vector<Card*> color_cards = player_hand->get_color(i);

            bool has7 = false;
            bool has8 = false;
            bool has9 = false;

            for(Card* card : color_cards)
            {
                if(static_cast<int>(*card) == i*8)
                {
                    // has D7, H7, S7 or C7
                    has7 = true;
                } else if(static_cast<int>(*card) == i*8 + 1)
                {
                    has8 = true;
                } else if(static_cast<int>(*card) == i*8 + 2)
                {
                    has9 = true;
                }
            }

            bool drop8 = false;
            bool drop9 = false;

            if(has7 && has8 && has9)
            {
                drop8 = true;
                drop9 = true;
            } else if(has7 && has9)
            {
                has8 = std::find_if(color_cards.begin(), color_cards.end(),
                             [](Card* card) {return static_cast<int>(*card) % 8 == 1; })
                                     != color_cards.end();

                if(has8)
                {
                    drop8 = true;
                    drop9 = true;
                }
            } else if(has7 && has8)
            {
                drop8 = true;
            } else if(has8 && has9) {
                drop9 = true;
            }
            for(Card* card : color_cards)
            {
                if(drop8 && static_cast<int>(*card) % 8 == 1)
                    continue;
                if(drop9 && static_cast<int>(*card) % 8 == 2)
                    continue;
                result.push_back(card);
            }
        }
    }

    int joker_int = 0;
    std::vector<Card*> jokers = player_hand->get_jokers();

    for(Card* joker : jokers)
    {
        int joker_color = pow(2, joker->get_card_color());
        joker_int += joker_color;
    }


    if(joker_int == 15      // DJ + DJ + SJ + CJ
        || joker_int == 7   // DJ + HJ + SJ
        || joker_int == 14  // HJ + SJ + CJ
        || joker_int == 6   // HJ + SJ
        || joker_int == 3   // DJ + HJ
        || joker_int == 12  // SJ + CJ
        )
    {
        result.push_back(jokers.front());
    } else if(joker_int == 13) // DJ + SJ + CJ
    {
        for(Card* joker : jokers)
        {
            if(static_cast<CardType>(*joker) == CardType::DJ
                || static_cast<CardType>(*joker) == CardType::CJ)
                result.push_back(joker);
        }
    } else if(joker_int == 11) // DJ + HJ + CJ
    {
        for(Card* joker : jokers)
        {
            if(static_cast<CardType>(*joker) == CardType::DJ
               || static_cast<CardType>(*joker) == CardType::CJ)
                result.push_back(joker);
        }
    } else {
        result.insert(result.end(), jokers.begin(), jokers.end());
    }
    return result;
}



std::vector<Card*> GamePlay::get_reduced_next_moves()
{
    bool new_play = is_new_play();
    bool last_play = is_last_play();

    int current_player = get_current_player();
    Hand* current_hand;

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
        return reduce_hand_equivalencies(current_hand);
    } else
    {
        Card* first_card = get_first_card();
        return get_viable_cards(first_card, current_hand);
    }
}

void GamePlay::make_move(Card *card)
{
    int player = get_current_player();
    played_cards.push_back(card);

    if(player == 0)
    {
        forehand->remove_card(card);
    } else if (player == 1)
    {
        midhand->remove_card(card);
    } else // if (player == 2)
    {
        backhand->remove_card(card);
    }

    // eval winner
    if(played_cards.size() % 3 == 0)
    {
        Card* cards[3] = {played_cards[played_cards.size() - 3],
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
    Card* card = played_cards.back();

    // revert winners
    if(played_cards.size() % 3 == 0)
    {
        winners.pop_back();
    }

    int last_player = get_previous_player();

    if(last_player == 0)
    {
        forehand->add_card(card);
    } else if(last_player == 1)
    {
        midhand->add_card(card);
    } else // if(last_player == 2)
    {
        backhand->add_card(card);
    }

    played_cards.pop_back();
}

int GamePlay::get_points_of_card(Card* card)
{
    int card_int = static_cast<int>(*card) % 8;
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

int GamePlay::get_points_of_hand(Card* play[3])
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
    Card* first_card = played_cards[depth - 2];
    Card* second_card = played_cards[depth - 1];
    Card* third_card = played_cards[depth];

    Card* play[3] = {first_card, second_card, third_card};
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
        Card* first_card = played_cards[i * 3];
        Card* second_card = played_cards[i * 3 + 1];
        Card* third_card = played_cards[i * 3 + 2];

        Card* play[3] = {first_card, second_card, third_card};
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