#ifndef __SPELLS_HPP__
#define __SPELLS_HPP__

#include <map>
#include <string>

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

    std::map<Spells::Type, const char *> TypeMapping = {
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

    std::map<Spells::Class, const char *> ClassMapping = {
        {Class::NONE, "NONE"},
        {Class::BLASTING, "BLASTING"},
        {Class::PSYCHIC, "PSYCHIC"}};

    Spells::Type GetType(const char *spell)
    {
        auto result = Spells::Type::NONE;

        auto spells = Spells::TypeMapping.begin();

        while (spells != Spells::TypeMapping.end())
        {
            if (std::strcmp(spells->second, spell) == 0)
            {
                result = spells->first;

                break;
            }

            spells++;
        }

        return result;
    }

    Spells::Type GetType(std::string spell)
    {
        return Spells::GetType(spell.c_str());
    }

    Spells::Class GetClass(const char *spellClass)
    {
        auto result = Spells::Class::NONE;

        auto spellClasses = Spells::ClassMapping.begin();

        while (spellClasses != Spells::ClassMapping.end())
        {
            if (std::strcmp(spellClasses->second, spellClass) == 0)
            {
                result = spellClasses->first;

                break;
            }

            spellClasses++;
        }

        return result;
    }

    Spells::Class GetClass(std::string spell)
    {
        return Spells::GetClass(spell.c_str());
    }

    class Base
    {
        Spells::Type Type = Type::NONE;

        Spells::Class Class = Class::NONE;

        bool IsCombat = true;

        bool Ranged = false;

        int Complexity = 0;

        int CurrentComplexity = 0;

        int Duration = -1;

        void Initialize(Spells::Type type, Spells::Class spellClass, bool isCombat, bool ranged, int complexity, int duration)
        {
            this->Type = type;

            this->Class = spellClass;

            this->IsCombat = isCombat;

            this->Ranged = ranged;

            this->Complexity = complexity;

            this->CurrentComplexity = complexity;

            this->Duration = duration;
        }

        Base(Spells::Type type, Spells::Class spellClass, bool isCombat, bool ranged, int complexity, int duration)
        {
            this->Initialize(type, spellClass, isCombat, ranged, complexity, duration);
        }

        Base(Spells::Type type, Spells::Class spellClass, bool isCombat, bool ranged, int complexity)
        {
            this->Initialize(type, spellClass, isCombat, ranged, complexity, -1);
        }
    };
}

#endif