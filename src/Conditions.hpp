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
        CHARACTER
    };

    BloodSword::Mapping<Conditions::Type> TypeMapping = {
        {Conditions::Type::NONE, "NONE"},
        {Conditions::Type::NORMAL, "NORMAL"},
        {Conditions::Type::CHARACTER, "CHARACTER"}};

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
        Conditions::Type Type = Type::NONE;

        std::string Variable;

        std::string Text;

        Book::Location Location = {Book::Number::NONE, -1};

        Base() {}

        Base(Conditions::Type type, std::string variable, std::string text, Book::Location location) : Type(type), Variable(variable), Text(text), Location(location)
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

            if (!data["variable"].is_null())
            {
                // set variable
                condition.Variable = std::string(data["variable"]);
            }
        }

        return condition;
    }

    bool Process(Party::Base &party, Conditions::Base condition)
    {
        auto result = false;

        if (condition.Type == Conditions::Type::NONE)
        {
            result = true;
        }
        else if (condition.Type == Conditions::Type::NORMAL)
        {
            result = true;
        }
        else if (condition.Type == Conditions::Type::CHARACTER)
        {
            auto character = Character::Map(condition.Variable);

            result = party.Has(character);
        }

        return result;
    }
}
#endif