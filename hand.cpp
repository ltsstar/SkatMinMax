//
// Created by michel on 8/23/22.
//

#include "hand.h"

Card::Card(CardType card_type) :
        card_type(card_type)
{
    card_joker = (static_cast<int>(card_type) + 1) % 8 == 0;
    card_color = static_cast<int>(card_type) / 8;
}

int Card::get_color() const
{
    return card_color;
}

bool Card::is_joker() const
{
    return card_joker;
}

bool Card::is_color(int color) const
{
    return card_color == color;
}

CardContainer::CardContainer() :
    lowest_colors{nullptr},
    highest_colors{nullptr},
    lowest_joker(nullptr),
    highest_joker(nullptr),
    colors{std::vector<Card*>()}
{}

CardContainer::CardContainer(const CardContainer& other_card_container) :
        lowest_joker(other_card_container.lowest_joker),
        highest_joker(other_card_container.highest_joker),
        jokers(other_card_container.jokers)
{
    for(int i=0;i<4;++i)
    {
        colors[i] = other_card_container.colors[i];
        lowest_colors[i] = other_card_container.lowest_colors[i];
        highest_colors[i] = other_card_container.highest_colors[i];
    }

}

CardContainer::CardContainer(std::set<CardType> cards) :
        lowest_colors{nullptr},
        highest_colors{nullptr},
        lowest_joker(nullptr),
        highest_joker(nullptr),
        colors{std::vector<Card*>()}
{
    for(const CardType &card_type : cards){
        Card* card = new Card(card_type);
        add_card(card);
    }
}

CardContainer::CardContainer(std::vector<Card*> cards) :
        lowest_colors{nullptr},
        highest_colors{nullptr},
        lowest_joker(nullptr),
        highest_joker(nullptr),
        colors{std::vector<Card*>()}
{
    for(Card *card : cards)
    {
        add_card(card);
    }
}

bool CardContainer::has_card(const Card* card)
{
    if(card->is_joker())
    {
        for(auto const& joker: jokers)
        {
            if(card == joker)
                return true;
        }
    } else {
        int color = card->get_color();
        for(auto const& color_card : colors[color])
        {
            if(card == color_card)
                return true;
        }
    }
    return false;
}

bool CardContainer::has_card(const Card card)
{
    if(card.is_joker())
    {
        for(auto const& joker: jokers)
        {
            if(card == *joker)
                return true;
        }
    } else {
        int color = card.get_color();
        for(auto const& color_card : colors[color])
        {
            if(card == *color_card)
                return true;
        }
    }
    return false;
}

bool CardContainer::has_higher_color(Card *card) {
    for(auto const own_card : colors[card->get_color()])
    {
        if(static_cast<int>(*card) < static_cast<int>(*own_card))
        {
            return true;
        }
    }
    return false;
}

bool CardContainer::has_color(int color)
{
    return colors[color].size() > 0;
}

bool CardContainer::has_joker()
{
    return jokers.size() > 0;
}

bool CardContainer::has_higher_joker(Card* card)
{
    if(jokers.size())
    {
        if(static_cast<int>(*highest_joker) > static_cast<int>(*card))
            return true;
    }
    return false;
}

Card* CardContainer::find_highest_joker()
{
    if(jokers.empty())
        return nullptr;

    int value = -1;
    Card* res_joker;
    for(auto joker : jokers)
    {
        if(static_cast<int>(*joker) > value) {
            value = static_cast<int>(*joker);
            res_joker = joker;
        }
    }
    return res_joker;
}

Card* CardContainer::find_lowest_joker()
{
    if(jokers.empty())
        return nullptr;

    int value = 33;
    Card* res_joker;
    for(auto joker : jokers)
    {
        if(static_cast<int>(*joker) < value) {
            value = static_cast<int>(*joker);
            res_joker = joker;
        }
    }
    return res_joker;
}

Card* CardContainer::find_highest_color(int color)
{
    if(colors[color].empty())
        return nullptr;

    int value = -1;
    Card* res_color;
    for(auto card : colors[color])
    {
        if(static_cast<int>(*card) > value) {
            value = static_cast<int>(*card);
            res_color = card;
        }
    }
    return res_color;
}

Card* CardContainer::find_lowest_color(int color)
{
    if(colors[color].empty())
        return nullptr;

    int value = 33;
    Card* res_color;
    for(auto card : colors[color])
    {
        if(static_cast<int>(*card) < value) {
            value = static_cast<int>(*card);
            res_color = card;
        }
    }
    return res_color;
}

void CardContainer::remove_card(const Card* card)
{
    if(card->is_joker())
    {
        jokers.erase(std::find(jokers.begin(), jokers.end(), card));
        if(card == lowest_joker) {
            lowest_joker = find_lowest_joker();
        }
        if(card == highest_joker) {
            highest_joker = find_highest_joker();
        }
    } else {
        int color = card->get_color();
        colors[color].erase(std::find(colors[card->get_color()].begin(),
                                              colors[card->get_color()].end(),
                         card));
        if(card == highest_colors[color])
        {
            highest_colors[color] = find_highest_color(color);
        }
        if(card == lowest_colors[color])
        {
            lowest_colors[color] = find_lowest_color(color);
        }
    }
}

void CardContainer::remove_cards()
{
    jokers.clear();
    for(auto &color : colors)
    {
        color.clear();
    }
    std::fill_n(lowest_colors, 4, nullptr);
    std::fill_n(highest_colors, 4, nullptr);
    highest_joker = nullptr;
    lowest_joker = nullptr;
}

void CardContainer::remove_color(int color)
{
    colors[color].clear();
    lowest_colors[color] = nullptr;
    highest_colors[color] = nullptr;
}

void CardContainer::remove_jokers()
{
    jokers.clear();
    lowest_joker = nullptr;
    highest_joker = nullptr;
}

void CardContainer::add_joker(Card* card)
{
    if(jokers.size() == 0)
    {
        lowest_joker = card;
        highest_joker = card;
    } else {
        if(static_cast<int>(*card) < static_cast<int>(*lowest_joker))
            lowest_joker = card;
        else if(static_cast<int>(*card) > static_cast<int>(*highest_joker))
            highest_joker = card;
    }
    jokers.push_back(card);
}

void CardContainer::add_color(Card* card)
{
    int i = card->get_color();
    if(colors[i].size() == 0)
    {
        lowest_colors[i] = card;
        highest_colors[i] = card;
    } else {
        if(static_cast<int>(*card) < static_cast<int>(*lowest_colors[i]))
            lowest_colors[i] = card;
        else if(static_cast<int>(*card) > static_cast<int>(*highest_colors[i]))
            highest_colors[i] = card;
    }
    colors[i].push_back(card);
}

void CardContainer::add_card(Card* card)
{
    if(card->is_joker()) {
        add_joker(card);
    } else {
        add_color(card);
    }
}

void CardContainer::add_cards(std::vector<Card*> cards)
{
    for(auto const card : cards)
    {
        add_card(card);
    }
}

Card* CardContainer::get_lowest_joker()
{
    return lowest_joker;
}

Card* CardContainer::get_highest_joker()
{
    return highest_joker;
}

Card* CardContainer::get_lowest_card_of_color(int color)
{
    return lowest_colors[color];
}

Card* CardContainer::get_highest_card_of_color(int color)
{
    return highest_colors[color];
}

std::vector<Card*> CardContainer::get_jokers()
{
    return jokers;
}

std::vector<Card*> CardContainer::get_color(int color)
{
    return colors[color];
}

std::vector<Card*> CardContainer::get_lowest_card_of_each_color()
{
    std::vector<Card*> result;
    for(Card* card : lowest_colors)
    {
        if(card != nullptr)
            result.push_back(card);
    }
    return result;
}

std::vector<Card*> CardContainer::get_highest_card_of_each_color()
{
    std::vector<Card*> result;
    for(Card* card : highest_colors)
    {
        result.push_back(card);
    }
    return result;
}

CardContainer CardContainer::get_color_and_jokers(int color)
{
    CardContainer new_card_container(*this);
    for(int i=0;i<4;++i)
    {
        if(i != color)
            new_card_container.remove_color(i);
    }
    return new_card_container;
}

int CardContainer::size()
{
    int res = jokers.size();
    for(auto const& color : colors)
    {
        res += color.size();
    }
    return res;
}