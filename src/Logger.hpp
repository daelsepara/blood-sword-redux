#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include "BattleResults.hpp"
#include "Conditions.hpp"
#include "Party.hpp"

namespace BloodSword::Engine
{
    // prototypes
    int Count(Party::Base &party, Character::ControlType control, Character::Status status);

    int Count(Party::Base &party);

    bool IsAlive(Party::Base &party, Character::ControlType control_type = Character::ControlType::PLAYER);

    bool IsAlive(Character::Base &character);

    bool IsFleeing(Party::Base &party);
}

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

    void LogCasters(Party::Base &party, std::vector<int> &casters)
    {
#if defined(DEBUG)
        if (casters.size() > 0)
        {
            std::cerr << "[SPELL CASTERS] (";

            for (auto i = 0; i < casters.size(); i++)
            {
                if (i > 0)
                {
                    std::cerr << ", ";
                }

                std::cerr << "[" << Target::Mapping[party[casters[i]].Target] << " " << std::to_string(casters[i]) << "]";
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

    void LogBattleResults(Party::Base &opponents, Party::Base &party, Battle::Result initial, Battle::Result result)
    {
#if defined(DEBUG)
        std::cerr << "[BATTLE RESULTS] " << Battle::ResultMapping[initial] << std::endl
                  << "[PARTY] " << (Engine::IsAlive(party) ? "ALIVE" : "INCAPACITATED") << std::endl
                  << "[OPPONENTS] " << (Engine::IsAlive(opponents, Character::ControlType::NPC) ? "ALIVE" : "INCAPACITATED") << std::endl
                  << "[FLEEING] " << (Engine::IsFleeing(party) ? "YES" : "NO") << std::endl
                  << "[FINAL RESULTS] " << Battle::ResultMapping[result] << std::endl;
#endif
    }
}

namespace BloodSword::EngineLogger
{
    // BloodSword Engine "Simple Intelligence Logger"
    void Log(const char *action, const char *attacker, const char *target, int dist, int path = -1, int vuln = -1, int prob = -1, int threshold = -1)
    {
#if defined(DEBUG)
        std::cerr << "[" << attacker << "]"
                  << " [" << action << "]"
                  << " [" << target << "]";

        if (path != -1)
        {
            std::cerr << " [PATH] " << std::to_string(path);
        }

        std::cerr << " [DIST] " << std::to_string(dist);

        if (vuln != -1)
        {
            std::cerr << " [VULN] " << std::to_string(vuln);
        }

        if (prob != -1 && threshold != -1)
        {
            std::cerr << " [PROB] "
                      << std::to_string(prob)
                      << " "
                      << (prob <= threshold ? "<=" : ">")
                      << " "
                      << std::to_string(threshold);
        }

        std::cerr << std::endl;
#endif
    }

    // log enemy action and target
    void Log(const char *action, Target::Type attacker, int id, Target::Type target, int target_id, int dist, int path = -1, int vuln = -1, int prob = -1, int threshold = -1)
    {
        auto action_string = std::string(action) + " TARGET " + std::to_string(target_id);

        auto attacker_string = Target::Mapping[attacker] + " " + std::to_string(id);

        auto target_string = Target::Mapping[target] + " " + std::to_string(target_id);

        EngineLogger::Log(action_string.c_str(), attacker_string.c_str(), target_string.c_str(), dist, path, vuln, prob, threshold);
    }

    // log enemy action and target
    void Log(const char *action, Target::Type target, int id, int target_id, int dist, int path = -1, int vuln = -1)
    {
        auto action_string = std::string(action) + " TARGET " + std::to_string(target_id);

        auto attacker_string = std::string("ENEMY ") + std::to_string(id);

        auto target_string = Target::Mapping[target] + " " + std::to_string(target_id);

        EngineLogger::Log(action_string.c_str(), attacker_string.c_str(), target_string.c_str(), dist, path, vuln);
    }
}

namespace BloodSword::EvalLogger
{
    void Log(Conditions::Base &condition, bool result, bool failed, std::string text)
    {
#if defined(DEBUG)
        // debug info
        std::cerr << "[CONDITION] "
                  << std::string(Conditions::TypeMapping[condition.Type])
                  << " ([RESULT] "
                  << (result ? "TRUE" : "FALSE")
                  << ", [FAILED] "
                  << (failed ? "TRUE" : "FALSE");

        if (text.size() > 0)
        {
            std::cerr << ", [TEXT] " << text;
        }

        std::cerr << ")";

        if (condition.Invert)
        {
            std::cerr << " [INVERTED]";
        }

        std::cerr << std::endl;
#endif
    }
}

namespace BloodSword::InterfaceLogger
{
    void LogSpellFailure(Character::Base &caster, Spells::Type spell)
    {
#if defined(DEBUG)
        if (!caster.HasCalledToMind(spell))
        {
            std::cerr << "[" << Spells::TypeMapping[spell] << "] NOT CALLED TO MIND" << std::endl;
        }
        else
        {
            std::cerr << "[" << Spells::TypeMapping[spell] << "] NOT IN GRIMOIRE" << std::endl;
        }
#endif
    }

    void LogOptions(Asset::List &assets, std::vector<int> &selection, std::string selected)
    {
#if defined(DEBUG)
        if (selection.size() > 0)
        {
            selected += " (";

            for (auto i = 0; i < selection.size(); i++)
            {
                if (i > 0)
                {
                    selected += ", ";
                }

                selected += std::string(Asset::TypeMapping[assets[selection[i]]]);
            }

            selected += ")";

            std::cerr << selected << std::endl;
        }
#endif
    }

    void LogChoice(const char *message, Asset::Type asset, int selected, int size)
    {
#if defined(DEBUG)
        std::cerr << "["
                  << message
                  << " "
                  << selected
                  << "] ["
                  << Asset::TypeMapping[asset]
                  << "] [SIZE] "
                  << size
                  << std::endl;
#endif
    }

    void LogPathToTarget(Point target, int path, int distance)
    {
#if defined(DEBUG)
        std::cerr << "[TARGET ("
                  << target.X
                  << ", "
                  << target.Y
                  << ")] [PATH] "
                  << path
                  << " [DIST] "
                  << distance
                  << std::endl;
#endif
    }

    void LogMoveTargets(const char *type, Target::Type character, int src_id, int dst_id, int path, int valid, int avail)
    {
#if defined(DEBUG)
        std::cerr << "["
                  << Target::Mapping[character]
                  << " "
                  << src_id
                  << "] [MOVE] "
                  << "[TARGET "
                  << type
                  << " "
                  << dst_id
                  << "] [PATH] "
                  << path
                  << " [DIST] "
                  << valid
                  << " [VULN] "
                  << avail
                  << std::endl;
#endif
    }
}

namespace BloodSword::StoryLogger
{
    // log major story events
    void LogSectionHeader(const char *header, Book::Location location, bool newline = true)
    {
#if defined(DEBUG)
        std::cerr << "["
                  << header
                  << "] "
                  << Book::String(location);

        if (newline)
        {
            std::cerr << std::endl;
        }
#endif
    }

    // log story search results
    void LogSearch(Book::Location location, int current)
    {
#if defined(DEBUG)
        StoryLogger::LogSectionHeader("FIND", location, false);

        std::cerr << " == " << (current != -1 ? "FOUND" : "NOT FOUND") << std::endl;
#endif
    }
}

#endif
