#ifndef __CHOICE_HPP__
#define __CHOICE_HPP__

#include <string>
#include <vector>

#include "Book.hpp"
#include "Attribute.hpp"
#include "CharacterClasses.hpp"
#include "Item.hpp"

// classes and functions for managing story choices
namespace BloodSword::Choice
{
    enum class Type
    {
        NORMAL,
        ATTRIBUTE,
        CHARACTER,
        SKILL,
        ITEM
    };

    Mapping<Choice::Type> TypeMapping = {
        {Type::NORMAL, "NORMAL"},
        {Type::ATTRIBUTE, "ATTRIBUTE"},
        {Type::CHARACTER, "CHARACTER"},
        {Type::SKILL, "SKILL"},
        {Type::ITEM, "ITEM"}};

    Choice::Type Map(const char *choice)
    {
        return BloodSword::Find(Choice::TypeMapping, choice);
    }

    Choice::Type Map(std::string choice)
    {
        return Choice::Map(choice.c_str());
    }

    class Base
    {
    public:
        Book::Destination Destination = {Book::Number::None, 0};

        Choice::Type Choice = Choice::Type::NORMAL;

        Attribute::Type Attribute = Attribute::Type::NONE;

        Character::Class Class = Character::Class::NONE;

        Item::Type Item = Item::Type::NONE;

        std::string Text;

        int Quantity = -1;
    };
}

#endif