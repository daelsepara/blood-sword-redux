#ifndef __BATTLE_LOGGER_HPP__
#define __BATTLE_LOGGER_HPP__

#include "../InterfaceInventory.hpp"

namespace BloodSword::BattleLogger
{
    void LogSpellStrategy(Spells::Type spell, int spells_cast, bool cast)
    {
#if defined(DEBUG)
        std::cerr << "[SPELLS CAST] ";

        if (spell != Spells::Type::NONE)
        {
            std::cerr << "[" << Spells::TypeMapping[spell] << "] ";
        }

        std::cerr << std::to_string(spells_cast) << " [CAST] " << (cast ? "TRUE" : "FALSE") << std::endl;
#endif
    }

    void LogCaster(Character::Base &caster, int caster_id)
    {
#if defined(DEBUG)
        std::cerr << "["
                  << Target::Mapping[caster.Target]
                  << " " << std::to_string(caster_id)
                  << "] ";

        if (!Engine::IsAlive(caster))
        {
            std::cerr << "IS DEAD" << std::endl;
        }
        else if (!caster.Has(Skills::Type::SPELLS))
        {
            std::cerr << "CANNOT CAST SPELLS" << std::endl;
        }
        else
        {
            std::cerr << "CANNOT CAST SPELLS THIS ROUND" << std::endl;
        }
#endif
    }

    void LogCasters(Battle::Base &battle)
    {
#if defined(DEBUG)
        if (battle.Casters.size() > 0)
        {
            std::cerr << "[SPELL CASTERS] (";

            for (auto i = 0; i < battle.Casters.size(); i++)
            {
                if (i > 0)
                {
                    std::cerr << ", ";
                }

                std::cerr << "[" << Target::Mapping[battle.Opponents[battle.Casters[i]].Target] << " " << std::to_string(battle.Casters[i]) << "]";
            }

            std::cerr << ")" << std::endl;
        }
#endif
    }

    void LogTargets(const char *target_type, Target::Type attacker, int id, int count)
    {
#if defined(DEBUG)
        std::cerr << "["
                  << Target::Mapping[attacker]
                  << " "
                  << id
                  << "] ["
                  << target_type
                  << " TARGETS] "
                  << count
                  << std::endl;
#endif
    }

    void LogNoSpellTargets(Spells::Type spell)
    {
#if defined(DEBUG)
        std::cerr << "[SPELL] [" << Spells::TypeMapping[spell] << "] NO VALID TARGETS" << std::endl;
#endif
    }

    void LogSpellMissing(Spells::Type spell)
    {
#if defined(DEBUG)
        std::cerr << "[SPELL] [" << Spells::TypeMapping[spell] << "] NOT IN GRIMOIRE!" << std::endl;
#endif
    }

    void LogSpellUnusable(Spells::Type spell)
    {
#if defined(DEBUG)
        std::cerr << "[SPELL] [" << Spells::TypeMapping[spell] << "] CANNOT BE USED IN BATTLE!" << std::endl;
#endif
    }

    void LogAction(const char *action, Target::Type attacker, int id, Target::Type target, int target_id)
    {
#if defined(DEBUG)
        std::cerr << "["
                  << Target::Mapping[attacker]
                  << " "
                  << id
                  << "] ["
                  << action
                  << "] ["
                  << Target::Mapping[target]
                  << " "
                  << target_id
                  << "]"
                  << std::endl;
#endif
    }

    void LogSpellCasting(Target::Type caster, int caster_id, Spells::Type spell)
    {
#if defined(DEBUG)
        std::cerr << "["
                  << Target::Mapping[caster]
                  << " "
                  << std::to_string(caster_id)
                  << "] [CASTS "
                  << Spells::TypeMapping[spell]
                  << "]"
                  << std::endl;
#endif
    }

    void LogCombatants(Party::Base &party, const char *group)
    {
#if defined(DEBUG)
        std::cerr << "["
                  << group
                  << "] "
                  << std::to_string(party.Count())
                  << " [LIVE] "
                  << std::to_string(Engine::Count(party))
                  << std::endl;
#endif
    }

    void LogGroup(Book::Location location, const char *first, const char *second, int count_first, int count_second)
    {
#if defined(DEBUG)
        std::cerr << Book::String(location)
                  << " ["
                  << first
                  << "] "
                  << std::to_string(count_first)
                  << " ["
                  << second
                  << "] "
                  << std::to_string(count_second)
                  << std::endl;
#endif
    }

    void LogSurvivors(Book::Location location, const char *first, const char *second, int count_first, int count_second)
    {
#if defined(DEBUG)
        auto group_first = std::string(first) + " SURVIVORS";

        auto group_second = std::string(second) + " SURVIVORS";

        BattleLogger::LogGroup(location, group_first.c_str(), group_second.c_str(), count_first, count_second);
#endif
    }

    void LogGroupAction(const char *group, const char *action, Target::Type actor, int id)
    {
#if defined(DEBUG)
        std::cerr << "["
                  << group
                  << "] ["
                  << action
                  << "] ["
                  << Target::Mapping[actor]
                  << " "
                  << std::to_string(id)
                  << "]"
                  << std::endl;
#endif
    }

    void LogBattleResults(Battle::Base &battle, Party::Base &party, Battle::Result initial, Battle::Result result)
    {
#if defined(DEBUG)
        std::cerr << "[BATTLE RESULTS] " << Battle::ResultMapping[initial] << std::endl
                  << "[PARTY] " << (Engine::IsAlive(party) ? "ALIVE" : "INCAPACITATED") << std::endl
                  << "[OPPONENTS] " << (Engine::IsAlive(battle.Opponents, Character::ControlType::NPC) ? "ALIVE" : "INCAPACITATED") << std::endl
                  << "[FLEEING] " << (Engine::IsFleeing(party) ? "YES" : "NO") << std::endl
                  << "[FINAL RESULTS] " << Battle::ResultMapping[result] << std::endl;
#endif
    }
}

#endif
