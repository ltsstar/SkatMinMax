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

CardContainer::CardContainer(const CardContainer& other_card_container)
{
    jokers = other_card_container.jokers;
    for(int i=0;i<4;++i)
        colors[i] = other_card_container.colors[i];
}

CardContainer::CardContainer(std::set<CardType> cards)
{
    for(const CardType &card_type : cards){
        Card* card = new Card(card_type);
        add_card(card);
    }
}

CardContainer::CardContainer(std::vector<Card*> cards)
{
    for(const Card *card : cards)
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

bool CardContainer::has_color(int color)
{
    return colors[color].size() > 0;
}

bool CardContainer::has_joker()
{
    return jokers.size() > 0;
}

void CardContainer::remove_card(const Card* card)
{
    if(card->is_joker())
        jokers.erase(std::find(jokers.begin(), jokers.end(), card));
    else
        colors[card->get_color()].erase(std::find(colors[card->get_color()].begin(),
                                                  colors[card->get_color()].end(), card));
}

void CardContainer::remove_color(int color)
{
    colors[color] = std::vector<Card*>();
}

void CardContainer::add_card(Card* card)
{
    if(card->is_joker())
        jokers.push_back(card);
    else
        colors[card->get_color()].push_back(card);
}

std::vector<Card*> CardContainer::get_jokers()
{
    return jokers;
}

std::vector<Card*> CardContainer::get_color(int color)
{
    return colors[color];
}

CardContainer CardContainer::get_color_and_jokers(int color)
{
    CardContainer new_card_container = CardContainer(*this);
    for(int i=0;i<4;++i)
    {
        if(i != color)
            new_card_container.remove_color(i);
    }
    return new_card_container;
}