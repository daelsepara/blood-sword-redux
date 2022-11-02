#ifndef __CHOICE_HPP__
#define __CHOICE_HPP__

#include <string>
#include <vector>

#include "Book.hpp"
#include "Attribute.hpp"
#include "CharacterClasses.hpp"

// classes and functions for managing story choices
namespace BloodSword::Choice
{
    enum class Type
    {
        NORMAL,
        ATTRIBUTE,
        CHARACTER,
        SKILL,
    };

    class Base
    {
    public:
        Book::Destination Destination = {Book::Number::None, 0};

        Choice::Type Choice = Choice::Type::NORMAL;

        Attribute::Type Attribute = Attribute::Type::NONE;

        Character::Class Class = Character::Class::NONE;

        std::string Text;

        int Quantity = -1;
    };
}

#endif