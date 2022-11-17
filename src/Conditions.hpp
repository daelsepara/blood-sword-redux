#ifndef __CONDITIONS_HPP__
#define __CONDITIONS_HPP__

#include "Book.hpp"
#include "Templates.hpp"

namespace BloodSword::Conditions
{
    enum class Type
    {
        NONE = -1,
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
}
#endif