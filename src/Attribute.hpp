#ifndef __ATTRIBUTE_HPP__
#define __ATTRIBUTE_HPP__

#include <string>
#include <vector>

#include "Templates.hpp"

namespace BloodSword::Attribute
{
    // attribute type
    enum class Type
    {
        NONE = -1,
        FIGHTING_PROWESS,
        AWARENESS,
        PSYCHIC_ABILITY,
        ENDURANCE,
        DAMAGE,
        ARMOUR
    };

    std::vector<Attribute::Type> All = {
        Attribute::Type::FIGHTING_PROWESS,
        Attribute::Type::AWARENESS,
        Attribute::Type::PSYCHIC_ABILITY,
        Attribute::Type::ENDURANCE,
        Attribute::Type::DAMAGE,
        Attribute::Type::ARMOUR};

    // attribute case class
    class Base
    {
    public:
        // attribute type
        Attribute::Type Type = Type::NONE;

        // base value
        int Value = 0;

        // modifier
        int Modifier = 0;

        // maximum value
        int Maximum = 0;

        Base(Attribute::Type type, int value, int modifier) : Type(type), Value(value), Modifier(modifier), Maximum(value) {}

        Base() {}
    };

    Mapping<Attribute::Type> TypeMapping = {
        {Attribute::Type::NONE, "NONE"},
        {Attribute::Type::FIGHTING_PROWESS, "FIGHTING PROWESS"},
        {Attribute::Type::AWARENESS, "AWARENESS"},
        {Attribute::Type::PSYCHIC_ABILITY, "PSYCHIC ABILITY"},
        {Attribute::Type::ENDURANCE, "ENDURANCE"},
        {Attribute::Type::DAMAGE, "DAMAGE"},
        {Attribute::Type::ARMOUR, "ARMOUR"}};

    Attribute::Type Map(const char *attribute)
    {
        return BloodSword::Find(Attribute::TypeMapping, attribute);
    }

    Attribute::Type Map(std::string attribute)
    {
        return Attribute::Map(attribute.c_str());
    }
}

#endif
