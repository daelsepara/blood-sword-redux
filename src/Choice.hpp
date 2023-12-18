#ifndef __CHOICE_HPP__
#define __CHOICE_HPP__

#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "Attribute.hpp"
#include "CharacterClasses.hpp"
#include "Item.hpp"
#include "Conditions.hpp"

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

    BloodSword::Mapping<Choice::Type> TypeMapping = {
        {Choice::Type::NORMAL, "NORMAL"},
        {Choice::Type::ATTRIBUTE, "ATTRIBUTE"},
        {Choice::Type::CHARACTER, "CHARACTER"},
        {Choice::Type::SKILL, "SKILL"},
        {Choice::Type::ITEM, "ITEM"}};

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
        BloodSword::Story::Conditions::Base Condition;

        Base() {}
    };

    // TODO: parse choice from json
    Choice::Base Parse(nlohmann::json json)
    {
        auto choice = Choice::Base();

        return choice;
    }

    bool Process(Choice::Base &choice)
    {
        auto valid = false;

        return valid;
    }
}

#endif
