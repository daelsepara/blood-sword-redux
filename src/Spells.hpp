#ifndef __SPELLS_HPP__
#define __SPELLS_HPP__

#include <string>

#include "Templates.hpp"
#include "AssetTypes.hpp"

namespace BloodSword::Spells
{
    // spell type
    enum class Type
    {
        NONE = -1,
        VOLCANO_SPRAY,
        NIGHTHOWL,
        WHITE_FIRE,
        SWORDTHRUST,
        EYE_OF_THE_TIGER,
        IMMEDIATE_DELIVERANCE,
        MISTS_OF_DEATH,
        THE_VAMPIRE_SPELL,
        PILLAR_OF_SALT,
        SHEET_LIGHTNING,
        GHASTLY_TOUCH,
        NEMESIS_BOLT,
        SERVILE_ENTHRALMENT,
        SUMMON_FALTYN,
        PREDICTION,
        DETECT_ENCHANTMENT
    };

    template <typename T>
    using Mapped = std::unordered_map<Spells::Type, T>;

    // spell class (blasting or psychic)
    enum class Class
    {
        NONE,
        BLASTING,
        PSYCHIC
    };

    Mapping<Spells::Type> TypeMapping = {
        {Type::NONE, "NONE"},
        {Type::VOLCANO_SPRAY, "VOLCANO SPRAY"},
        {Type::NIGHTHOWL, "NIGHTHOWL"},
        {Type::WHITE_FIRE, "WHITE FIRE"},
        {Type::SWORDTHRUST, "SWORDTHRUST"},
        {Type::EYE_OF_THE_TIGER, "EYE OF THE TIGER"},
        {Type::IMMEDIATE_DELIVERANCE, "IMMEDIATE DELIVERANCE"},
        {Type::MISTS_OF_DEATH, "MISTS OF DEATH"},
        {Type::THE_VAMPIRE_SPELL, "THE VAMPIRE SPELL"},
        {Type::PILLAR_OF_SALT, "PILLAR OF SALT"},
        {Type::SHEET_LIGHTNING, "SHEET LIGHTNING"},
        {Type::GHASTLY_TOUCH, "GHASTLY TOUCH"},
        {Type::NEMESIS_BOLT, "NEMESIS BOLT"},
        {Type::SERVILE_ENTHRALMENT, "SERVILE ENTHRALMENT"},
        {Type::SUMMON_FALTYN, "SUMMON FALTYN"},
        {Type::PREDICTION, "PREDICTION"},
        {Type::DETECT_ENCHANTMENT, "DETECT ENCHANTMENT"}};

    std::unordered_map<Spells::Type, Asset::Type> Assets = {
        {Type::VOLCANO_SPRAY, Asset::Type::VOLCANO_SPRAY},
        {Type::NIGHTHOWL, Asset::Type::NIGHTHOWL},
        {Type::WHITE_FIRE, Asset::Type::WHITE_FIRE},
        {Type::SWORDTHRUST, Asset::Type::SWORDTHRUST},
        {Type::EYE_OF_THE_TIGER, Asset::Type::EYE_OF_THE_TIGER},
        {Type::IMMEDIATE_DELIVERANCE, Asset::Type::IMMEDIATE_DELIVERANCE},
        {Type::MISTS_OF_DEATH, Asset::Type::MISTS_OF_DEATH},
        {Type::THE_VAMPIRE_SPELL, Asset::Type::THE_VAMPIRE_SPELL},
        {Type::PILLAR_OF_SALT, Asset::Type::PILLAR_OF_SALT},
        {Type::SHEET_LIGHTNING, Asset::Type::SHEET_LIGHTNING},
        {Type::GHASTLY_TOUCH, Asset::Type::GHASTLY_TOUCH},
        {Type::NEMESIS_BOLT, Asset::Type::NEMESIS_BOLT},
        {Type::SERVILE_ENTHRALMENT, Asset::Type::SERVILE_ENTHRALMENT},
        {Type::SUMMON_FALTYN, Asset::Type::SUMMON_FALTYN},
        {Type::PREDICTION, Asset::Type::PREDICTION},
        {Type::DETECT_ENCHANTMENT, Asset::Type::DETECT_ENCHANTMENT}};

    Mapping<Spells::Class> ClassMapping = {
        {Class::NONE, "NONE"},
        {Class::BLASTING, "BLASTING"},
        {Class::PSYCHIC, "PSYCHIC"}};

    // spells that do not need to be called to mind
    std::vector<Spells::Type> Basic = {
        Type::SUMMON_FALTYN,
        Type::PREDICTION,
        Type::DETECT_ENCHANTMENT};

    std::vector<Spells::Type> Target = {
        Type::NIGHTHOWL,
        Type::WHITE_FIRE,
        Type::SWORDTHRUST,
        Type::THE_VAMPIRE_SPELL,
        Type::PILLAR_OF_SALT,
        Type::GHASTLY_TOUCH,
        Type::NEMESIS_BOLT,
        Type::SERVILE_ENTHRALMENT};

    Spells::Type Map(const char *spell)
    {
        return BloodSword::Find(Spells::TypeMapping, spell);
    }

    Spells::Type Map(std::string spell)
    {
        return Spells::Map(spell.c_str());
    }

    Spells::Class MapClass(const char *spellClass)
    {
        return BloodSword::Find(Spells::ClassMapping, spellClass);
    }

    Spells::Class MapClass(std::string spell)
    {
        return Spells::MapClass(spell.c_str());
    }

    bool In(std::vector<Spells::Type> &list, Spells::Type spell)
    {
        return list.size() > 0 && BloodSword::Find(list, spell) != list.end();
    }

    // spell base class
    class Base
    {
    public:
        // spell type
        Spells::Type Type = Type::NONE;

        // spell class
        Spells::Class Class = Class::NONE;

        // spell is only usable in battle
        bool IsBattle = true;

        // spell can be cast on range
        bool Ranged = false;

        // spell complexity level
        int Complexity = 0;

        // current complexity level
        int CurrentComplexity = 0;

        // spell overall duration, -1 if infinite
        int Duration = -1;

        Base(Spells::Type type,
             Spells::Class spellClass,
             bool isBattle,
             bool ranged,
             int complexity,
             int duration) : Type(type),
                             Class(spellClass),
                             IsBattle(isBattle),
                             Ranged(ranged),
                             Complexity(complexity),
                             CurrentComplexity(complexity),
                             Duration(duration) {}

        Base(Spells::Type type,
             Spells::Class spellClass,
             bool isBattle,
             bool ranged,
             int complexity) : Base(type, spellClass, isBattle, ranged, complexity, -1) {}

        bool IsBasic()
        {
            return Spells::In(Spells::Basic, this->Type);
        }

        bool RequiresTarget()
        {
            return Spells::In(Spells::Target, this->Type);
        }
    };
}

#endif
