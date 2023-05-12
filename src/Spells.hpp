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
        {Spells::Type::NONE, "NONE"},
        {Spells::Type::VOLCANO_SPRAY, "VOLCANO SPRAY"},
        {Spells::Type::NIGHTHOWL, "NIGHTHOWL"},
        {Spells::Type::WHITE_FIRE, "WHITE FIRE"},
        {Spells::Type::SWORDTHRUST, "SWORDTHRUST"},
        {Spells::Type::EYE_OF_THE_TIGER, "EYE OF THE TIGER"},
        {Spells::Type::IMMEDIATE_DELIVERANCE, "IMMEDIATE DELIVERANCE"},
        {Spells::Type::MISTS_OF_DEATH, "MISTS OF DEATH"},
        {Spells::Type::THE_VAMPIRE_SPELL, "THE VAMPIRE SPELL"},
        {Spells::Type::PILLAR_OF_SALT, "PILLAR OF SALT"},
        {Spells::Type::SHEET_LIGHTNING, "SHEET LIGHTNING"},
        {Spells::Type::GHASTLY_TOUCH, "GHASTLY TOUCH"},
        {Spells::Type::NEMESIS_BOLT, "NEMESIS BOLT"},
        {Spells::Type::SERVILE_ENTHRALMENT, "SERVILE ENTHRALMENT"},
        {Spells::Type::SUMMON_FALTYN, "SUMMON FALTYN"},
        {Spells::Type::PREDICTION, "PREDICTION"},
        {Spells::Type::DETECT_ENCHANTMENT, "DETECT ENCHANTMENT"}};

    std::unordered_map<Spells::Type, Asset::Type> Assets = {
        {Spells::Type::VOLCANO_SPRAY, Asset::Type::VOLCANO_SPRAY},
        {Spells::Type::NIGHTHOWL, Asset::Type::NIGHTHOWL},
        {Spells::Type::WHITE_FIRE, Asset::Type::WHITE_FIRE},
        {Spells::Type::SWORDTHRUST, Asset::Type::SWORDTHRUST},
        {Spells::Type::EYE_OF_THE_TIGER, Asset::Type::EYE_OF_THE_TIGER},
        {Spells::Type::IMMEDIATE_DELIVERANCE, Asset::Type::IMMEDIATE_DELIVERANCE},
        {Spells::Type::MISTS_OF_DEATH, Asset::Type::MISTS_OF_DEATH},
        {Spells::Type::THE_VAMPIRE_SPELL, Asset::Type::THE_VAMPIRE_SPELL},
        {Spells::Type::PILLAR_OF_SALT, Asset::Type::PILLAR_OF_SALT},
        {Spells::Type::SHEET_LIGHTNING, Asset::Type::SHEET_LIGHTNING},
        {Spells::Type::GHASTLY_TOUCH, Asset::Type::GHASTLY_TOUCH},
        {Spells::Type::NEMESIS_BOLT, Asset::Type::NEMESIS_BOLT},
        {Spells::Type::SERVILE_ENTHRALMENT, Asset::Type::SERVILE_ENTHRALMENT},
        {Spells::Type::SUMMON_FALTYN, Asset::Type::SUMMON_FALTYN},
        {Spells::Type::PREDICTION, Asset::Type::PREDICTION},
        {Spells::Type::DETECT_ENCHANTMENT, Asset::Type::DETECT_ENCHANTMENT}};

    Mapping<Spells::Class> ClassMapping = {
        {Spells::Class::NONE, "NONE"},
        {Spells::Class::BLASTING, "BLASTING"},
        {Spells::Class::PSYCHIC, "PSYCHIC"}};

    // spells that do not need to be called to mind
    std::vector<Spells::Type> Basic = {
        Spells::Type::SUMMON_FALTYN,
        Spells::Type::PREDICTION,
        Spells::Type::DETECT_ENCHANTMENT};

    std::vector<Spells::Type> Target = {
        Spells::Type::NIGHTHOWL,
        Spells::Type::WHITE_FIRE,
        Spells::Type::SWORDTHRUST,
        Spells::Type::THE_VAMPIRE_SPELL,
        Spells::Type::PILLAR_OF_SALT,
        Spells::Type::GHASTLY_TOUCH,
        Spells::Type::NEMESIS_BOLT,
        Spells::Type::SERVILE_ENTHRALMENT};

    std::vector<Spells::Type> TargetsParty = {
        Spells::Type::VOLCANO_SPRAY,
        Spells::Type::MISTS_OF_DEATH,
        Spells::Type::SHEET_LIGHTNING};

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

        bool MultipleTargets()
        {
            return Spells::In(Spells::TargetsParty, this->Type);
        }
    };
}

#endif
