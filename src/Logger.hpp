#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include "BattleResults.hpp"
#include "Conditions.hpp"
#include "Party.hpp"

namespace BloodSword::Logger
{
    // disable logger
    void Disable()
    {
        SDL_SetHint(SDL_HINT_LOGGING, "app=quiet,error=quiet,*=quiet");

        SDL_SetHint(SDL_HINT_EVENT_LOGGING, "0");
    }
}

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
    // log opponent spell strategy
    void LogSpellStrategy(Spells::Type spell, int spells_cast, bool cast)
    {
        std::string log_string = "[SPELLS CAST] ";

        if (spell != Spells::Type::NONE)
        {
            log_string += "[" + Spells::TypeMapping[spell] + "]";
        }

        SDL_Log("%s [CAST] %s", log_string.c_str(), (cast ? "TRUE" : "FALSE"));
    }

    // log spell caster
    void LogCaster(Character::Base &caster, int caster_id)
    {
        std::string log_string = "[" + Target::Mapping[caster.Target] + " " + std::to_string(caster_id) + "] ";

        if (!Engine::IsAlive(caster))
        {
            log_string += "IS DEAD";
        }
        else if (!caster.Has(Skills::Type::SPELLS))
        {
            log_string += "CANNOT CAST SPELLS";
        }
        else
        {
            log_string += "CANNOT CAST SPELLS THIS ROUND";
        }

        SDL_Log("%s", log_string.c_str());
    }

    // log spell casters
    void LogCasters(Party::Base &party, std::vector<int> &casters)
    {
        if (SafeCast(casters.size()) > 0)
        {
            std::string casters = "[SPELL CASTERS] (";

            for (auto i = 0; i < SafeCast(casters.size()); i++)
            {
                if (i > 0)
                {
                    casters += ", ";
                }

                casters += "[" + Target::Mapping[party[casters[i]].Target] + " " + std::to_string(casters[i]) + "]";
            }

            SDL_Log("%s", casters.c_str());
        }
    }

    // log attacker targets
    void LogTargets(const char *target_type, Target::Type attacker, int id, int count)
    {
        SDL_Log("[%s %d] [%s TARGETS] %d", Target::Mapping[attacker].c_str(), id, target_type, count);
    }

    // log if there are no valid spell targets
    void LogNoSpellTargets(Spells::Type spell)
    {
        SDL_Log("[SPELL] [%s] NO VALID TARGETS", Spells::TypeMapping[spell].c_str());
    }

    // log if spell is not in grimoire
    void LogSpellMissing(Spells::Type spell)
    {
        SDL_Log("[SPELL] [%s] NOT IN GRIMOIRE!", Spells::TypeMapping[spell].c_str());
    }

    // log if spell is not usable
    void LogSpellUnusable(Spells::Type spell)
    {
        SDL_Log("[SPELL] [%s] CANNOT BE USED IN BATTLE!", Spells::TypeMapping[spell].c_str());
    }

    // log action, attacker and target
    void LogAction(const char *action, Target::Type attacker, int id, Target::Type target, int target_id)
    {
        SDL_Log("[%s %d] [%s] [%s %d]", Target::Mapping[attacker].c_str(), id, action, Target::Mapping[target].c_str(), target_id);
    }

    // log spell casting
    void LogSpellCasting(Target::Type caster, int caster_id, Spells::Type spell)
    {
        SDL_Log("[%s %d] [CASTS %s]", Target::Mapping[caster].c_str(), caster_id, Spells::TypeMapping[spell].c_str());
    }

    // log combatants
    void LogCombatants(Party::Base &party, const char *group)
    {
        SDL_Log("[%s] %d [LIVE] %d", group, party.Count(), Engine::Count(party));
    }

    // log party groups
    void LogGroup(Book::Location location, const char *first, const char *second, int count_first, int count_second)
    {
        SDL_Log("%s [%s] %d [%s] %d", Book::String(location).c_str(), first, count_first, second, count_second);
    }

    // log survivors
    void LogSurvivors(Book::Location location, const char *first, const char *second, int count_first, int count_second)
    {
        auto group_first = std::string(first) + " SURVIVORS";

        auto group_second = std::string(second) + " SURVIVORS";

        BattleLogger::LogGroup(location, group_first.c_str(), group_second.c_str(), count_first, count_second);
    }

    // log group action
    void LogGroupAction(const char *group, const char *action, Target::Type actor, int id)
    {
        SDL_Log("[%s] [%s] [%s %d]", group, action, Target::Mapping[actor].c_str(), id);
    }

    // log battle results
    void LogBattleResults(Party::Base &opponents, Party::Base &party, Battle::Result initial, Battle::Result result)
    {
        SDL_Log("[BATTLE RESULTS] %s [PARTY] %s [OPPONENTS] %s [FLEEING] %s [FINAL RESULTS] %s", Battle::ResultMapping[initial].c_str(), (Engine::IsAlive(party) ? "ALIVE" : "INCAPACITATED"), (Engine::IsAlive(opponents, Character::ControlType::NPC) ? "ALIVE" : "INCAPACITATED"), (Engine::IsFleeing(party) ? "YES" : "NO"), Battle::ResultMapping[result].c_str());
    }

    // log if character is in combat
    void LogInCombat(Character::Class character)
    {
        if (character != Character::Class::NONE)
        {
            SDL_Log("[IN COMBAT] [%s]", Character::ClassMapping[character].c_str());
        }
    }
}

namespace BloodSword::EngineLogger
{
    // "Simple Intelligence Logger"
    void Log(const char *action, const char *attacker, const char *target, int dist, int path = -1, int vuln = -1, int prob = -1, int threshold = -1)
    {
        std::string log_string = "[" + std::string(attacker) + "] [" + std::string(action) + "] [" + std::string(target) + "]";

        if (path != -1)
        {
            log_string += " [PATH] " + std::to_string(path);
        }

        log_string += " [DIST] " + std::to_string(dist);

        if (vuln != -1)
        {
            log_string += " [VULN] " + std::to_string(vuln);
        }

        if (prob != -1 && threshold != -1)
        {
            log_string += " [PROB] " + std::to_string(prob) + " " + (prob <= threshold ? "<=" : ">") + " " + std::to_string(threshold);
        }

        SDL_Log("%s", log_string.c_str());
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

    // log killed / paralyzed
    void Log(Party::Base &party, int i)
    {
        SDL_Log("[KILLED/PARALYZED %s]", Character::ClassMapping[party[i].Class].c_str());
    }
}

namespace BloodSword::EvalLogger
{
    // log story / choice conditions
    void Log(Conditions::Base &condition, bool result, bool failed, std::string text)
    {
        std::string log_string = "[CONDITION] " + std::string(Conditions::TypeMapping[condition.Type]) + "([RESULT] " + (result ? "TRUE" : "FALSE") + "[FAILED] " + (failed ? "TRUE" : "FALSE");

        if (SafeCast(text.size()) > 0)
        {
            log_string += ", [TEXT] " + text;
        }

        log_string += ")";

        if (condition.Invert)
        {
            log_string += " [INVERTED]";
        }

        SDL_Log("%s", log_string.c_str());
    }
}

namespace BloodSword::InterfaceLogger
{
    // log failure to cast / call to mind
    void LogSpellFailure(Character::Base &caster, Spells::Type spell)
    {
        if (!caster.HasCalledToMind(spell))
        {
            SDL_Log("[%s] NOT CALLED TO MIND", Spells::TypeMapping[spell].c_str());
        }
        else
        {
            SDL_Log("[%s] NOT IN GRIMOIRE", Spells::TypeMapping[spell].c_str());
        }
    }

    // log choice / menu options
    void LogOptions(Asset::List &assets, std::vector<int> &selection, std::string selected)
    {
        if (SafeCast(selection.size()) > 0)
        {
            selected += " (";

            for (auto i = 0; i < SafeCast(selection.size()); i++)
            {
                if (i > 0)
                {
                    selected += ", ";
                }

                selected += std::string(Asset::TypeMapping[assets[selection[i]]]);
            }

            selected += ")";

            SDL_Log("%s", selected.c_str());
        }
    }

    // log choices
    void LogChoice(const char *message, Asset::Type asset, int selected, int size)
    {
        SDL_Log("[%s %d] [%s] [SIZE] %d", message, selected, Asset::TypeMapping[asset].c_str(), size);
    }

    // log path to target
    void LogPathToTarget(Point target, int path, int distance)
    {
        SDL_Log("[TARGET (%d, %d)] [PATH] %d [DIST] %d", target.X, target.Y, path, distance);
    }

    // log targets for move action
    void LogMoveTargets(const char *type, Target::Type character, int src_id, int dst_id, int path, int valid, int avail)
    {
        SDL_Log("[%s %d] [MOVE] [TARGET %s %d] [PATH] %d [DIST] %d [VULN] %d", Target::Mapping[character].c_str(), src_id, type, dst_id, path, valid, avail);
    }
}

namespace BloodSword::StoryLogger
{
    // log major story events
    void LogSectionHeader(const char *header, Book::Location location)
    {
        SDL_Log("[%s] %s", header, Book::String(location).c_str());
    }

    // log story search results
    void LogSearch(Book::Location location, int current)
    {
        SDL_Log("[FIND] %s == %s", Book::String(location).c_str(), (current != -1 ? "FOUND" : "NOT FOUND"));
    }
}

#endif
