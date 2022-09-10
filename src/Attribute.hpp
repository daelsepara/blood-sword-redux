#ifndef __ATTRIBUTE_HPP__
#define __ATTRIBUTE_HPP__

#include <string>

#include "Templates.hpp"

namespace BloodSword::Attribute
{
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

    class Base
    {
    public:
        Attribute::Type Type = Type::NONE;

        int Value = 0;

        int Modifier = 0;

        int Maximum = 0;

        Base(Attribute::Type type, int value, int modifier)
        {
            this->Type = type;

            this->Value = value;

            this->Modifier = modifier;

            this->Maximum = value;
        }
    };

    Mapping<Attribute::Type> TypeMapping = {
        {Type::NONE, "NONE"},
        {Type::FIGHTING_PROWESS, "FIGHTING PROWESS"},
        {Type::AWARENESS, "AWARENESS"},
        {Type::PSYCHIC_ABILITY, "PSYCHIC ABILITY"},
        {Type::ENDURANCE, "ENDURANCE"},
        {Type::DAMAGE, "DAMAGE"},
        {Type::ARMOUR, "ARMOUR"}};

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
