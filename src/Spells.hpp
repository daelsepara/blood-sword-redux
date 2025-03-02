#ifndef __SPELLS_HPP__
#define __SPELLS_HPP__

#include <string>

#include "nlohmann/json.hpp"
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

    typedef std::vector<Spells::Type> List;

    // spell class (blasting or psychic)
    enum class Class
    {
        NONE = -1,
        BLASTING,
        PSYCHIC
    };

    BloodSword::Mapping<Spells::Type> TypeMapping = {
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

    BloodSword::UnorderedMap<Spells::Type, Asset::Type> Assets = {
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

    BloodSword::Mapping<Spells::Class> ClassMapping = {
        {Spells::Class::NONE, "NONE"},
        {Spells::Class::BLASTING, "BLASTING"},
        {Spells::Class::PSYCHIC, "PSYCHIC"}};

    // spells that do not need to be called to mind
    Spells::List Basic = {
        Spells::Type::SUMMON_FALTYN,
        Spells::Type::PREDICTION,
        Spells::Type::DETECT_ENCHANTMENT};

    // spells that target a single opponent
    Spells::List Target = {
        Spells::Type::NIGHTHOWL,
        Spells::Type::WHITE_FIRE,
        Spells::Type::SWORDTHRUST,
        Spells::Type::THE_VAMPIRE_SPELL,
        Spells::Type::PILLAR_OF_SALT,
        Spells::Type::GHASTLY_TOUCH,
        Spells::Type::NEMESIS_BOLT,
        Spells::Type::SERVILE_ENTHRALMENT};

    // spells that target entire party
    Spells::List TargetsParty = {
        Spells::Type::VOLCANO_SPRAY,
        Spells::Type::MISTS_OF_DEATH,
        Spells::Type::SHEET_LIGHTNING};

    // spell difficulty (number of die rolls)
    BloodSword::IntMapping<Spells::Type> Difficulty = {
        {Spells::Type::NONE, 2},
        {Spells::Type::VOLCANO_SPRAY, 2},
        {Spells::Type::NIGHTHOWL, 2},
        {Spells::Type::WHITE_FIRE, 2},
        {Spells::Type::SWORDTHRUST, 2},
        {Spells::Type::EYE_OF_THE_TIGER, 2},
        {Spells::Type::IMMEDIATE_DELIVERANCE, 2},
        {Spells::Type::MISTS_OF_DEATH, 2},
        {Spells::Type::THE_VAMPIRE_SPELL, 2},
        {Spells::Type::PILLAR_OF_SALT, 2},
        {Spells::Type::SHEET_LIGHTNING, 2},
        {Spells::Type::GHASTLY_TOUCH, 2},
        {Spells::Type::NEMESIS_BOLT, 2},
        {Spells::Type::SERVILE_ENTHRALMENT, 2},
        {Spells::Type::SUMMON_FALTYN, 2},
        {Spells::Type::PREDICTION, 2},
        {Spells::Type::DETECT_ENCHANTMENT, 2}};

    // default spell difficulty modifier
    BloodSword::IntMapping<Spells::Type> DifficultyModifier = {
        {Spells::Type::NONE, 0},
        {Spells::Type::VOLCANO_SPRAY, 0},
        {Spells::Type::NIGHTHOWL, 0},
        {Spells::Type::WHITE_FIRE, 0},
        {Spells::Type::SWORDTHRUST, 0},
        {Spells::Type::EYE_OF_THE_TIGER, 0},
        {Spells::Type::IMMEDIATE_DELIVERANCE, 0},
        {Spells::Type::MISTS_OF_DEATH, 0},
        {Spells::Type::THE_VAMPIRE_SPELL, 0},
        {Spells::Type::PILLAR_OF_SALT, 0},
        {Spells::Type::SHEET_LIGHTNING, 0},
        {Spells::Type::GHASTLY_TOUCH, 0},
        {Spells::Type::NEMESIS_BOLT, 0},
        {Spells::Type::SERVILE_ENTHRALMENT, 0},
        {Spells::Type::SUMMON_FALTYN, 0},
        {Spells::Type::PREDICTION, 0},
        {Spells::Type::DETECT_ENCHANTMENT, 0}};

    // damage (number of die rolls)
    BloodSword::IntMapping<Spells::Type> Damage = {
        {Spells::Type::NONE, 0},
        {Spells::Type::VOLCANO_SPRAY, 1},
        {Spells::Type::NIGHTHOWL, 0},
        {Spells::Type::WHITE_FIRE, 2},
        {Spells::Type::SWORDTHRUST, 3},
        {Spells::Type::EYE_OF_THE_TIGER, 0},
        {Spells::Type::IMMEDIATE_DELIVERANCE, 0},
        {Spells::Type::MISTS_OF_DEATH, 2},
        {Spells::Type::THE_VAMPIRE_SPELL, 4},
        {Spells::Type::PILLAR_OF_SALT, 0},
        {Spells::Type::SHEET_LIGHTNING, 2},
        {Spells::Type::GHASTLY_TOUCH, 7},
        {Spells::Type::NEMESIS_BOLT, 7},
        {Spells::Type::SERVILE_ENTHRALMENT, 0},
        {Spells::Type::SUMMON_FALTYN, 0},
        {Spells::Type::PREDICTION, 0},
        {Spells::Type::DETECT_ENCHANTMENT, 0}};

    // alternate damage rolls
    BloodSword::IntMapping<Spells::Type> AlternateDamage = {
        {Spells::Type::NONE, 0},
        {Spells::Type::VOLCANO_SPRAY, 1},
        {Spells::Type::NIGHTHOWL, 0},
        {Spells::Type::WHITE_FIRE, 2},
        {Spells::Type::SWORDTHRUST, 3},
        {Spells::Type::EYE_OF_THE_TIGER, 0},
        {Spells::Type::IMMEDIATE_DELIVERANCE, 0},
        {Spells::Type::MISTS_OF_DEATH, 2},
        {Spells::Type::THE_VAMPIRE_SPELL, 4},
        {Spells::Type::PILLAR_OF_SALT, 0},
        {Spells::Type::SHEET_LIGHTNING, 2},
        {Spells::Type::GHASTLY_TOUCH, 2},
        {Spells::Type::NEMESIS_BOLT, 7},
        {Spells::Type::SERVILE_ENTHRALMENT, 0},
        {Spells::Type::SUMMON_FALTYN, 0},
        {Spells::Type::PREDICTION, 0},
        {Spells::Type::DETECT_ENCHANTMENT, 0}};

    // default damage roll modifier
    BloodSword::IntMapping<Spells::Type> DamageModifier = {
        {Spells::Type::NONE, 0},
        {Spells::Type::VOLCANO_SPRAY, 0},
        {Spells::Type::NIGHTHOWL, 0},
        {Spells::Type::WHITE_FIRE, 2},
        {Spells::Type::SWORDTHRUST, 3},
        {Spells::Type::EYE_OF_THE_TIGER, 0},
        {Spells::Type::IMMEDIATE_DELIVERANCE, 0},
        {Spells::Type::MISTS_OF_DEATH, 0},
        {Spells::Type::THE_VAMPIRE_SPELL, 0},
        {Spells::Type::PILLAR_OF_SALT, 0},
        {Spells::Type::SHEET_LIGHTNING, 2},
        {Spells::Type::GHASTLY_TOUCH, 0},
        {Spells::Type::NEMESIS_BOLT, 7},
        {Spells::Type::SERVILE_ENTHRALMENT, 0},
        {Spells::Type::SUMMON_FALTYN, 0},
        {Spells::Type::PREDICTION, 0},
        {Spells::Type::DETECT_ENCHANTMENT, 0}};

    Spells::Type Map(const char *spell)
    {
        return BloodSword::Find(Spells::TypeMapping, spell);
    }

    Spells::Type Map(std::string spell)
    {
        return Spells::Map(spell.c_str());
    }

    Spells::Class MapClass(const char *spell_class)
    {
        return BloodSword::Find(Spells::ClassMapping, spell_class);
    }

    Spells::Class MapClass(std::string spell)
    {
        return Spells::MapClass(spell.c_str());
    }

    bool In(Spells::List &list, Spells::Type spell)
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
             Spells::Class spell_class,
             bool is_battle,
             bool ranged,
             int complexity,
             int duration) : Type(type),
                             Class(spell_class),
                             IsBattle(is_battle),
                             Ranged(ranged),
                             Complexity(complexity),
                             CurrentComplexity(complexity),
                             Duration(duration) {}

        Base(Spells::Type type,
             Spells::Class spell_class,
             bool is_battle,
             bool ranged,
             int complexity) : Base(type, spell_class, is_battle, ranged, complexity, -1) {}

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

    // class to determine how opponent casts spell
    class Usage
    {
    public:
        Spells::Type Spell = Spells::Type::NONE;

        int Uses = 0;

        // min number of opponents before it is used
        int Min = 0;

        // max number of opponents before it is used
        int Max = 4;

        Usage(Spells::Type spell, int uses, int min, int max) : Spell(spell), Uses(uses), Max(max) {}

        Usage(Spells::Type spell, int min, int max) : Spell(spell), Uses(1), Min(min), Max(max) {}

        Usage() {}
    };

    typedef std::vector<Spells::Usage> Strategy;

    typedef std::vector<Spells::Base> Grimoire;

    Spells::Grimoire Load(nlohmann::json &data)
    {
        auto spells = Spells::Grimoire();

        for (auto i = 0; i < data.size(); i++)
        {
            auto type = !data[i]["type"].is_null() ? Spells::Map(std::string(data[i]["type"])) : Spells::Type::NONE;

            auto spell_class = !data[i]["class"].is_null() ? Spells::MapClass(std::string(data[i]["class"])) : Spells::Class::NONE;

            auto battle = !data[i]["battle"].is_null() ? data[i]["battle"].get<bool>() : false;

            auto ranged = !data[i]["ranged"].is_null() ? data[i]["ranged"].get<bool>() : false;

            auto complexity = !data[i]["complexity"].is_null() ? int(data[i]["complexity"]) : 0;

            auto duration = !data[i]["duration"].is_null() ? int(data[i]["duration"]) : 0;

            if (type != Spells::Type::NONE && spell_class != Spells::Class::NONE)
            {
                spells.push_back(Spells::Base(type, spell_class, battle, ranged, complexity, duration));
            }
        }

        return spells;
    }

    Spells::List LoadList(nlohmann::json &data)
    {
        auto spells = Spells::List();

        for (auto i = 0; i < data.size(); i++)
        {
            auto spell = !data[i].is_null() ? Spells::Map(std::string(data[i])) : Spells::Type::NONE;

            if (spell != Spells::Type::NONE)
            {
                spells.push_back(spell);
            }
        }

        return spells;
    }

    Spells::Strategy LoadStrategy(nlohmann::json &data)
    {
        auto strategies = Spells::Strategy();

        for (auto i = 0; i < data.size(); i++)
        {
            auto spell = !data[i]["spell"].is_null() ? Spells::Map(std::string(data[i]["spell"])) : Spells::Type::NONE;

            auto uses = !data[i]["uses"].is_null() ? int(data[i]["uses"]) : 0;

            auto min = !data[i]["min"].is_null() ? int(data[i]["min"]) : 0;

            auto max = !data[i]["max"].is_null() ? int(data[i]["max"]) : 0;

            if (spell != Spells::Type::NONE)
            {
                strategies.push_back(Spells::Usage(spell, uses, min, max));
            }
        }

        return strategies;
    }

    nlohmann::json Data(Spells::Grimoire &spells)
    {
        nlohmann::json data;

        for (auto &spell : spells)
        {
            nlohmann::json row;

            row.emplace("type", Spells::TypeMapping[spell.Type]);

            row.emplace("class", Spells::ClassMapping[spell.Class]);

            row.emplace("battle", spell.IsBattle);

            row.emplace("ranged", spell.Ranged);

            row.emplace("complexity", spell.Complexity);

            row.emplace("duration", spell.Duration);

            data.push_back(row);
        }

        return data;
    }

    nlohmann::json Data(Spells::List &memorized)
    {
        nlohmann::json data;

        for (auto &spell : memorized)
        {
            data.push_back(std::string(Spells::TypeMapping[spell]));
        }

        return data;
    }

    nlohmann::json Data(Spells::Strategy &strategies)
    {
        nlohmann::json data;

        for (auto &strategy : strategies)
        {
            nlohmann::json spell;

            spell["spell"] = std::string(Spells::TypeMapping[strategy.Spell]);

            spell["uses"] = strategy.Uses;

            spell["min"] = strategy.Min;

            spell["max"] = strategy.Max;

            data.push_back(spell);
        }

        return data;
    }

    // NPC will try to cast spell
    bool CanCastSpells(Spells::Strategy &strategies, int opponents)
    {
        bool cast = false;

        for (auto &strategy : strategies)
        {
            if (strategy.Uses > 0 && opponents >= strategy.Min && opponents <= strategy.Max)
            {
                cast = true;

                break;
            }
        }

        return cast;
    }

    // NPC casts spell (updates strategy)
    void CastSpell(Spells::Strategy &strategies, Spells::Type spell)
    {
        for (auto &strategy : strategies)
        {
            if (strategy.Uses > 0 && strategy.Spell == spell)
            {
                strategy.Uses--;

                break;
            }
        }
    }
}

#endif
