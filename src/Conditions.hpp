#ifndef __CONDITIONS_HPP__
#define __CONDITIONS_HPP__

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "Graphics.hpp"
#include "Item.hpp"
#include "Party.hpp"
#include "Scene.hpp"
#include "Templates.hpp"

namespace BloodSword::Story::Conditions
{
    enum class Type
    {
        NONE = -1,
        NORMAL,
        IN_PARTY,
        CHOOSE_CHARACTER,
        CHOSEN_CHARACTER,
        BATTLE_VICTORY,
        BATTLE_FLEE,
        BATTLE_ENTHRALMENT,
        BATTLE_VICTORY_OR_ENTHRALMENT,
        HAS_ITEM,
        USE_ITEM,
        DROP_ITEM,
        TAKE_ITEM,
        TAKE_ITEMS,
        GAIN_ENDURANCE,
        LOSE_ENDURANCE,
        CHOOSE_NUMBER,
        CHOSEN_NUMBER,
        PARTY_WOUNDED,
        PREVIOUS_LOCATION
    };

    BloodSword::Mapping<Conditions::Type> TypeMapping = {
        {Conditions::Type::NONE, "NONE"},
        {Conditions::Type::NORMAL, "NORMAL"},
        {Conditions::Type::IN_PARTY, "IN PARTY"},
        {Conditions::Type::CHOOSE_CHARACTER, "CHOOSE CHARACTER"},
        {Conditions::Type::CHOSEN_CHARACTER, "CHOSEN CHARACTER"},
        {Conditions::Type::BATTLE_VICTORY, "BATTLE VICTORY"},
        {Conditions::Type::BATTLE_FLEE, "BATTLE FLEE"},
        {Conditions::Type::BATTLE_ENTHRALMENT, "BATTLE ENTHRALMENT"},
        {Conditions::Type::BATTLE_VICTORY_OR_ENTHRALMENT, "BATTLE VICTORY OR ENTHRALMENT"},
        {Conditions::Type::HAS_ITEM, "HAS ITEM"},
        {Conditions::Type::USE_ITEM, "USE ITEM"},
        {Conditions::Type::DROP_ITEM, "DROP ITEM"},
        {Conditions::Type::TAKE_ITEM, "TAKE ITEM"},
        {Conditions::Type::TAKE_ITEMS, "TAKE ITEMS"},
        {Conditions::Type::GAIN_ENDURANCE, "GAIN ENDURANCE"},
        {Conditions::Type::LOSE_ENDURANCE, "LOSE ENDURANCE"},
        {Conditions::Type::CHOOSE_NUMBER, "CHOOSE NUMBER"},
        {Conditions::Type::CHOSEN_NUMBER, "CHOSEN NUMBER"},
        {Conditions::Type::PARTY_WOUNDED, "PARTY WOUNDED"},
        {Conditions::Type::PREVIOUS_LOCATION, "PREVIOUS LOCATION"}};

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
        // condition type
        Conditions::Type Type = Type::NONE;

        // text to display
        std::string Text;

        // destination (book, section)
        Book::Location Location = {Book::Number::NONE, -1};

        // internal variables used to validate "condition"
        std::vector<std::string> Variables = {};

        // flag to indicate that the result (or condition) is inverted
        bool Invert = false;

        Base() {}
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

            if (!data["invert"].is_null() && data["invert"].is_boolean())
            {
                condition.Invert = bool(!data["invert"]);
            }
        }

        return condition;
    }

    // routine to validate "condition"
    bool Process(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Conditions::Base &condition)
    {
        auto result = false;

        if (condition.Type == Conditions::Type::NORMAL)
        {
            result = true;
        }
        else if (condition.Type == Conditions::Type::IN_PARTY)
        {
            auto character = Character::Map(condition.Variables[0]);

            result = party.Has(character);
        }
        else if (condition.Type == Conditions::Type::CHOSEN_CHARACTER)
        {
            auto character = Character::Map(condition.Variables[0]);

            result = party.ChosenCharacter == character;
        }
        else if (condition.Type == Conditions::Type::CHOSEN_NUMBER)
        {
            auto number = std::stoi(condition.Variables[0], nullptr, 10);

            result = party.ChosenNumber == number;
        }
        else if (condition.Type == Conditions::Type::HAS_ITEM)
        {
            auto item = Item::Map(condition.Variables[0]);

            result = party.Has(item);
        }

        return condition.Invert ? !result : result;
    }
}
#endif
