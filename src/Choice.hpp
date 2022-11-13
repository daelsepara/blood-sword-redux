#ifndef __CHOICE_HPP__
#define __CHOICE_HPP__

#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "Attribute.hpp"
#include "CharacterClasses.hpp"
#include "Item.hpp"

// classes and functions for managing story choices
namespace BloodSword::Choice
{
    enum class Type
    {
        NONE,
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
        Book::Destination Destination = {Book::Number::NONE, 0};

        Choice::Type Type = Choice::Type::NORMAL;

        Attribute::Type Attribute = Attribute::Type::NONE;

        Character::Class CharacterClass = Character::Class::NONE;

        Item::Type Item = Item::Type::NONE;

        std::string Text;

        int Quantity = -1;
    };

    // TODO: parse choice from json
    Choice::Base Parse(nlohmann::json json)
    {
        auto choice = Choice::Base();

        if (!json["choice"].is_null())
        {
            auto data = json["choice"];

            if (!data["destination"].is_null())
            {
                // set destination
            }

            if (!data["type"].is_null())
            {
                // set choice type
                choice.Type = Choice::Map(std::string(data["type"]));
            }

            if (!data["attribute"].is_null())
            {
                // set attribute
                choice.Attribute = Attribute::Map(std::string(data["attribute"]));
            }

            if (!data["characterClass"].is_null())
            {
                // set character class
                choice.CharacterClass = Character::Map(std::string(data["characterClass"]));
            }

            if (!data["item"].is_null())
            {
                // set item
            }

            if (!data["text"].is_null())
            {
                choice.Text = std::string(data["text"]);
            }

            if (!data["quantity"].is_null())
            {
                choice.Quantity = std::stoi(std::string(data["quantity"]));
            }
        }

        return choice;
    }
}

#endif