#ifndef __SPELLS_HPP__
#define __SPELLS_HPP__

#include <string>

#include "Templates.hpp"

namespace BloodSword::Spells
{
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

    Mapping<Spells::Class> ClassMapping = {
        {Class::NONE, "NONE"},
        {Class::BLASTING, "BLASTING"},
        {Class::PSYCHIC, "PSYCHIC"}};

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

    class Base
    {
    public:
        Spells::Type Type = Type::NONE;

        Spells::Class Class = Class::NONE;

        bool IsCombat = true;

        bool Ranged = false;

        int Complexity = 0;

        int CurrentComplexity = 0;

        int Duration = -1;

        Base(Spells::Type type,
             Spells::Class spellClass,
             bool isCombat,
             bool ranged,
             int complexity,
             int duration) : Type(type),
                             Class(spellClass),
                             IsCombat(isCombat),
                             Ranged(ranged),
                             Complexity(complexity),
                             Duration(duration) { this->CurrentComplexity = complexity; }

        Base(Spells::Type type,
             Spells::Class spellClass,
             bool isCombat,
             bool ranged,
             int complexity) : Base(type, spellClass, isCombat, ranged, complexity, -1) {}
    };
}

#endif
