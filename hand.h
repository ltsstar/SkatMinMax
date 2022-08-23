//
// Created by michel on 8/23/22.
//

#ifndef SKATMINMAX_HAND_H
#define SKATMINMAX_HAND_H

#include <vector>
#include <set>
#include <algorithm>

enum class Trump {
    Diamonds = 0,
    Hearts = 1,
    Spades = 2,
    Clubs = 3
};

enum class CardType {
    D7 = 0, D8 = 1, D9 = 2, DQ = 3, DK = 4, D10 = 5, DA = 6, DJ = 7,
    H7 = 8, H8 = 9, H9 = 10, HQ = 11, HK = 12, H10 = 13, HA = 14, HJ = 15,
    S7 = 16, S8 = 17, S9 = 18, SQ = 19, SK = 20, S10 = 21, SA = 22, SJ = 23,
    C7 = 24, C8 = 25, C9 = 26, CQ = 27, CK = 28, C10 = 29, CA = 30, CJ = 31
};

class Card
{
    CardType card_type;
    bool card_joker;
    int card_color;
public:
    Card() {};
    explicit Card(CardType card_type);
    int get_color() const;
    int get_points();
    bool is_joker() const;
    bool is_color(int color) const;
    explicit operator CardType() const {
        return card_type;
    };
    explicit operator int() const {
        return static_cast<int>(card_type);
    };
    bool operator <(const Card& other_card) const
    {
        return card_type < other_card.card_type;
    };
    bool operator ==(const Card& other_card) const
    {
        return card_type == other_card.card_type;
    }
};

class CardContainer
{
    std::vector<Card*> jokers;
    std::vector<Card*> colors[4];
public:
    CardContainer() = default;
    CardContainer(const CardContainer& other_card_container);
    explicit CardContainer(std::set<CardType> cards);
    explicit CardContainer(std::vector<Card*> cards);

    bool has_card(const Card* card);
    bool has_card(const Card card);
    bool has_joker();
    bool has_color(int color);

    void remove_card(const Card* card);
    void remove_color(int color);
    void add_card(Card* card);

    std::vector<Card*> get_jokers();
    std::vector<Card*> get_color(int color);
    CardContainer get_color_and_jokers(int color);

    explicit operator std::vector<Card*>() {
        return std::vector<Card*>(begin(), end());
    }

    // member typedefs provided through inheriting from std::iterator
    class iterator: public std::iterator<
            std::input_iterator_tag,   // iterator_category
            long,                      // value_type
            long,                      // difference_type
            const long*,               // pointer
            long                       // reference
    >{
    public:
        int group;
        std::vector<Card*>* jokers;
        std::vector<Card*> (*colors)[4];
        std::vector<Card*>::iterator current_iterator;

        explicit iterator(std::vector<Card*>* _jokers, std::vector<Card*> (*_colors)[4],
                          std::vector<Card*>::iterator _current_iterator, int _group) :
                jokers(_jokers), colors{_colors}, current_iterator(_current_iterator), group(_group)
        {
        }
        iterator& operator++() {
            if(group < 4)
            {
                current_iterator++;
                if(current_iterator == colors[group]->end())
                {
                    if(group == 3)
                    {
                        current_iterator = jokers->begin();
                    } else {
                        group++;
                        current_iterator = colors[group]->begin();
                    }
                } else {
                    current_iterator++;
                }
            } else {
                current_iterator++;
                if(current_iterator == jokers->end())
                    group++;
            }
            return *this;
        }
        bool operator==(const iterator other) const {
            return group == other.group && current_iterator == other.current_iterator;
        }
        bool operator!=(const iterator other) const {
            return !(*this == other);
        }
        const Card& operator*() const {
            return **current_iterator;
        }
    };
    iterator begin() {return iterator(&jokers, &colors, colors[0].begin(), 0);}
    iterator end() {return iterator(&jokers, &colors, jokers.end(), 5);}
};

class Hand : CardContainer
{
public:

    Card* get_last_card();
    std::vector<Card*> get_color_and_jokers(int color);
};

#endif //SKATMINMAX_HAND_H
