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

    void GenerateBattlepits(Rogue::Base &rogue, int width, int height, int max_rooms, int min_size, int max_size)
    {
        rogue.Battlepits = Map::Base(width, height);

        // generate battlepits
        Battlepits::Generate(rogue.Battlepits, rogue.Rooms, max_rooms, min_size, max_size);

        // place party at the center of the first room
        rogue.Battlepits[rogue.Rooms[0].Center()].Occupant = Map::Object::PARTY;

        rogue.Battlepits[rogue.Rooms[0].Center()].Id = Map::Party;
    }

    bool Blocked(Rogue::Base &rogue, Point point)
    {
        auto &tile = rogue.Battlepits[point];

        return (tile.IsOccupied() || tile.IsBlocked() || !tile.IsPassable());
    }

    bool Move(Rogue::Base &rogue, Party::Base &party, Point point)
    {
        auto moved = !Rogue::Blocked(rogue, point);

        if (moved)
        {
            auto from = Point(party.X, party.Y);

            auto &origin = rogue.Battlepits[from];

            auto &destination = rogue.Battlepits[point];

            origin.Occupant = Map::Object::NONE;

            origin.Id = Map::NotFound;

            destination.Occupant = Map::Object::PARTY;

            destination.Id = Map::Party;

            party.X = point.X;

            party.Y = point.Y;
        }

        return moved;
    }
}

#endif
