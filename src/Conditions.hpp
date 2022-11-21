#ifndef __CONDITIONS_HPP__
#define __CONDITIONS_HPP__

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "Templates.hpp"

namespace BloodSword::Conditions
{
    enum class Type
    {
        NONE = -1,
        NORMAL,
        CHARACTER,
        ITEM,
        BATTLE,
        SHOP
    };

    Mapping<Conditions::Type> TypeMapping = {
        {Type::NONE, "NONE"},
        {Type::CHARACTER, "CHARACTER"},
        {Type::ITEM, "ITEM"},
        {Type::BATTLE, "BATTLE"},
        {Type::SHOP, "SHOP"}};

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

        Book::Destination Destination = {Book::Number::NONE, 0};

        Base() {}

        Base(Conditions::Type type, std::string variable, std::string text, Book::Destination destination) : Type(type), Variable(variable), Text(text), Destination(destination)
        {
        }
    };

    Conditions::Base Parse(nlohmann::json json)
    {
        auto condition = Conditions::Base();

        if (!json["condition"].is_null())
        {
            auto data = json["condition"];

            if (!data["destination"].is_null())
            {
                // set destination
                auto book = !data["destination"]["book"].is_null() ? Book::MapBook(std::string(data["destination"]["book"])) : Book::Number::NONE;

                auto number = !data["destination"]["number"].is_null() ? std::stoi(std::string(data["destination"]["number"])) : 0;

                condition.Destination = {book, number};
            }

            if (!data["type"].is_null())
            {
                // set choice type
                condition.Type = Conditions::Map(std::string(data["type"]));
            }

            if (!data["text"].is_null())
            {
                // set attribute
                condition.Text = std::string(data["text"]);
            }

            if (!data["variable"].is_null())
            {
                // set character class
                condition.Variable = std::string(data["variable"]);
            }
        }

        return condition;
    }

    bool Process(Conditions::Base condition)
    {
        auto result = false;

        switch (condition.Type)
        {
        case Conditions::Type::NONE:
            result = true;
        case Conditions::Type::NORMAL:
            result = true;
            break;
        default:
            result = true;
        }

        return result;
    }
}
#endif