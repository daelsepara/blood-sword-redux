#ifndef __ROGUE_HPP__
#define __ROGUE_HPP__

#include "Battlepits.hpp"
#include "Interface.hpp"

// classes and functions to enable rogue-like game mode
namespace BloodSword::Rogue
{
    class Loot
    {
    public:
        // location in battlepits
        int X = -1;

        int Y = -1;

        // items in this location
        Items::Inventory Items = {};

        Loot() {}
    };

    class Base
    {
    public:
        // battlepits
        Map::Base Battlepits;

        // rooms generated in battlepits map
        std::vector<Room::Base> Rooms = {};

        // groups of enemies in battlepits
        std::vector<Party::Base> Opponents = {};

        // groups of items in battlepits
        std::vector<Loot> Loot = {};

        // player party adventuring in battlepits
        Party::Base Party;

        Base() {}
    };
}

#endif
