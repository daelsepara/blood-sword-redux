#ifndef __ATTRIBUTE_HPP__
#define __ATTRIBUTE_HPP__

#include "nlohmann/json.hpp"
#include "AssetTypes.hpp"
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

    typedef std::vector<Attribute::Type> Types;

    Attribute::Types All = {
        Attribute::Type::FIGHTING_PROWESS,
        Attribute::Type::AWARENESS,
        Attribute::Type::PSYCHIC_ABILITY,
        Attribute::Type::ENDURANCE,
        Attribute::Type::DAMAGE,
        Attribute::Type::ARMOUR};

    BloodSword::UnorderedMap<Attribute::Type, Asset::Type> Assets = {
        {Attribute::Type::NONE, Asset::Type::NONE},
        {Attribute::Type::FIGHTING_PROWESS, Asset::Type::FIGHT},
        {Attribute::Type::AWARENESS, Asset::Type::AWARENESS},
        {Attribute::Type::PSYCHIC_ABILITY, Asset::Type::PSYCHIC_ABILITY},
        {Attribute::Type::ENDURANCE, Asset::Type::ENDURANCE},
        {Attribute::Type::DAMAGE, Asset::Type::DAMAGE},
        {Attribute::Type::ARMOUR, Asset::Type::ARMOUR}};

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

        Base(Attribute::Type type, int value, int modifier, int maximum) : Type(type), Value(value), Modifier(modifier), Maximum(maximum) {}

        Base(Attribute::Type type, int value, int modifier) : Type(type), Value(value), Modifier(modifier), Maximum(value) {}

        Base() {}
    };

    BloodSword::Mapping<Attribute::Type> TypeMapping = {
        {Attribute::Type::NONE, "NONE"},
        {Attribute::Type::FIGHTING_PROWESS, "FIGHTING PROWESS"},
        {Attribute::Type::AWARENESS, "AWARENESS"},
        {Attribute::Type::PSYCHIC_ABILITY, "PSYCHIC ABILITY"},
        {Attribute::Type::ENDURANCE, "ENDURANCE"},
        {Attribute::Type::DAMAGE, "DAMAGE"},
        {Attribute::Type::ARMOUR, "ARMOUR"}};

    BloodSword::Mapping<Attribute::Type> Abbreviations = {
        {Attribute::Type::NONE, "---"},
        {Attribute::Type::FIGHTING_PROWESS, "FPR"},
        {Attribute::Type::AWARENESS, "AWR"},
        {Attribute::Type::PSYCHIC_ABILITY, "PSY"},
        {Attribute::Type::ENDURANCE, "END"},
        {Attribute::Type::DAMAGE, "DMG"},
        {Attribute::Type::ARMOUR, "ARM"}};

    Attribute::Type Map(const char *attribute)
    {
        return BloodSword::Find(Attribute::TypeMapping, attribute);
    }

    Attribute::Type Map(std::string attribute)
    {
        return Attribute::Map(attribute.c_str());
    }
}

namespace BloodSword::Attributes
{
    typedef std::vector<Attribute::Base> List;

    Attributes::List Load(nlohmann::json &data)
    {
        auto characteristics = Attributes::List();

        for (auto i = 0; i < data.size(); i++)
        {
            auto attribute = Attribute::Base();

            attribute.Type = !data[i]["type"].is_null() ? Attribute::Map(std::string(data[i]["type"])) : Attribute::Type::NONE;

            attribute.Value = !data[i]["value"].is_null() ? int(data[i]["value"]) : 0;

            attribute.Modifier = !data[i]["modifier"].is_null() ? int(data[i]["modifier"]) : 0;

            attribute.Maximum = !data[i]["maximum"].is_null() ? int(data[i]["maximum"]) : 0;

            if (attribute.Type != Attribute::Type::NONE)
            {
                characteristics.push_back(attribute);
            }
        }

        return characteristics;
    }

    nlohmann::json Data(Attributes::List &attributes)
    {
        nlohmann::json data;

        for (auto &attribute : attributes)
        {
            nlohmann::json row;

            row.emplace("type", Attribute::TypeMapping[attribute.Type]);

            row.emplace("value", attribute.Value);

            row.emplace("modifier", attribute.Modifier);

            row.emplace("maximum", attribute.Maximum);

            data.push_back(row);
        }

        return data;
    }
}

#endif
