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
        NORMAL,
        AMBUSH_PLAYER,
        AMBUSH_NPC,
        CANNOT_FLEE
    };

    class Base
    {
    public:
        std::vector<Battle::Condition> Conditions = {};

        Map::Base Map;

        Party::Base NPC;

        Base(std::vector<Battle::Condition> conditions, Map::Base &map, Party::Base &npc) : Conditions(conditions), Map(map), NPC(npc) {}

        Base() {}
    };
}

#endif
