#ifndef __CONDITIONS_HPP__
#define __CONDITIONS_HPP__

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "Party.hpp"
#include "Templates.hpp"

namespace BloodSword::Story::Conditions
{
    enum class Type
    {
        NONE = -1,
        NORMAL,
        HAS_CHARACTER,
        CHOOSE_CHARACTER,
        BATTLE_WIN,
        BATTLE_FLEE,
        BATTLE_ENTHRALLED,
        BATTLE_WIN_OR_ENTHRALLED,
        HAS_ITEM,
        USE_ITEM,
        DROP_ITEM,
        GAIN_ENDURANCE,
        LOSE_ENDURANCE
    };

    BloodSword::Mapping<Conditions::Type> TypeMapping = {
        {Conditions::Type::NONE, "NONE"},
        {Conditions::Type::NORMAL, "NORMAL"},
        {Conditions::Type::HAS_CHARACTER, "HAS_CHARACTER"}};

    Conditions::Type Map(const char *Conditions)
    {
        return BloodSword::Find(Conditions::TypeMapping, Conditions);
    }

    Conditions::Type Map(std::string Conditions)
    {
        return Conditions::Map(Conditions.c_str());
    }

    class Base
    {
    public:
        Story::Conditions::Type Type = Type::NONE;

        std::string Text;

        Book::Location Location = {Book::Number::NONE, -1};

        std::vector<std::string> Variables = {};

        Base() {}

        Base(Story::Conditions::Type type, std::string text, Book::Location location, std::vector<std::string> variables) : Type(type), Variables(variables), Text(text), Location(location)
        {
        }
    };

    Conditions::Base Parse(nlohmann::json json)
    {
        auto condition = Conditions::Base();

        if (!json["condition"].is_null())
        {
            auto data = json["condition"];

            if (!data["location"].is_null())
            {
                // set location
                auto book = !data["location"]["book"].is_null() ? Book::MapBook(std::string(data["location"]["book"])) : Book::Number::NONE;

                auto number = !data["location"]["number"].is_null() ? std::stoi(std::string(data["location"]["number"])) : -1;

                condition.Location = {book, number};
            }

            if (!data["type"].is_null())
            {
                // set condition type
                condition.Type = Conditions::Map(std::string(data["type"]));
            }

            if (!data["text"].is_null())
            {
                // set text
                condition.Text = std::string(data["text"]);
            }

            if (!data["variables"].is_null() && data["variables"].is_array() && data["variables"].size() > 0)
            {
                std::vector<std::string> variables = {};

                // set variables
                for (auto i = 0; i < data["variables"].size(); i++)
                {
                    variables.push_back(std::string(data["variables"][i]));
                }

                condition.Variables = variables;
            }
        }

        return condition;
    }

    bool Process(Party::Base &party, Story::Conditions::Base condition)
    {
        auto result = false;

        if (condition.Type == Story::Conditions::Type::NONE)
        {
            result = true;
        }
        else if (condition.Type == Story::Conditions::Type::NORMAL)
        {
            result = true;
        }
        else if (condition.Type == Story::Conditions::Type::HAS_CHARACTER)
        {
            auto character = Character::Map(condition.Variables[0]);

            result = party.Has(character);
        }

        return result;
    }
}
#endif
