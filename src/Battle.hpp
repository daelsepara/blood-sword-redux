#ifndef __BATTLE_HPP__
#define __BATTLE_HPP__

#include <vector>

#include "Map.hpp"
#include "Party.hpp"

// classes for describing battle (combatants, map, conditions)
namespace BloodSword::Battle
{
    enum class Condition
    {
        AMBUSH_PLAYER,
        AMBUSH_NPC,
        CANNOT_FLEE,
        SURVIVORS
    };

    enum class Result
    {
        NONE = -1,
        DETERMINE,
        DEFEAT,
        VICTORY,
        FLEE
    };

    const int Unlimited = -1;

    class Base
    {
    public:
        // battle conditions (see Battle::Condition types)
        std::vector<Battle::Condition> Conditions = {};

        // battle map
        Map::Base Map;

        // enemy combatants
        Party::Base Opponents;

        // round limit (-1 if unlimited)
        int Duration = Battle::Unlimited;

        Base(std::vector<Battle::Condition> conditions, Map::Base &map, Party::Base &opponents, int duration) : Conditions(conditions), Map(map), Opponents(opponents), Duration(duration) {}

        Base() {}

        bool Is(Battle::Condition condition)
        {
            auto result = false;

            for (auto &check : this->Conditions)
            {
                if (check == condition)
                {
                    result = true;

                    break;
                }
            }

            return result;
        }

        bool IsDefined()
        {
            return this->Opponents.Count() > 0 && Map.IsValid();
        }
    };
}

#endif
