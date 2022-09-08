#ifndef __ATTRIBUTE_HPP__
#define __ATTRIBUTE_HPP__

#include <map>
#include <string>

namespace BloodSword::Attribute
{
    enum class Type
    {
        None = -1,
        FIGHTING_PROWESS,
        AWARENESS,
        PSYCHIC_ABILITY,
        ENDURANCE,
        DAMAGE
    };

    class Base
    {
    public:
        Attribute::Type Type = Type::None;

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

    std::map<Attribute::Type, const char *> AttributeMapping = {
        {Type::None, "NONE"},
        {Type::FIGHTING_PROWESS, "FIGHTING_PROWESS"},
        {Type::AWARENESS, "AWARENESS"},
        {Type::PSYCHIC_ABILITY, "PSYCHIC_ABILITY"},
        {Type::ENDURANCE, "ENDURANCE"},
        {Type::DAMAGE, "DAMAGE"}};

    Attribute::Type GetType(const char *attribute)
    {
        auto result = Type::None;

        auto attributes = Attribute::AttributeMapping.begin();

        while (attributes != Attribute::AttributeMapping.end())
        {
            if (std::strcmp(attributes->second, attribute) == 0)
            {
                result = attributes->first;

                break;
            }

            attributes++;
        }

        return result;
    }

    Attribute::Type GetType(std::string attribute)
    {
        return Attribute::GetType(attribute.c_str());
    }
}

#endif
