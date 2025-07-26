#ifndef __ROGUE_CLASSES_HPP__
#define __ROGUE_CLASSES_HPP__

#include "Battlepits.hpp"
#include "InterfaceInventory.hpp"

namespace BloodSword::Rogue
{
    const int None = -1;

    enum class TriggerType
    {
        NONE = -1,
        CHARACTER,
        ITEM,
        ANY_ITEM,
        ALL_ITEMS,
        VICTORY
    };

    // area triggers
    class Trigger
    {
    public:
        Rogue::TriggerType Type = Rogue::TriggerType::NONE;

        // location in battlepits
        int X = Rogue::None;

        int Y = Rogue::None;

        // message to display
        std::string Message = std::string();

        // trigger values
        std::vector<std::string> Values = {};

        Point Location()
        {
            return Point(X, Y);
        }

        Trigger() {}
    };

    // items in location
    class Loot
    {
    public:
        // location in battlepits
        int X = Rogue::None;

        int Y = Rogue::None;

        // items in this location
        Items::Inventory Items = {};

        Point Location()
        {
            return Point(X, Y);
        }

        Loot() {}

        Loot(Point point) : X(point.X), Y(point.Y) {}
    };

    // roguelike game base class
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
        std::vector<Rogue::Loot> Loot = {};

        // groups of triggers in battlepits
        std::vector<Rogue::Trigger> Triggers = {};

        // player party adventuring in battlepits
        Party::Base Party;

        // number of enemies
        int Enemies = 0;

        // current enemy party
        int Enemy = Rogue::None;

        // cannot flee battle
        bool CannotFlee = false;

        // width (in pixels) of character stats card
        int StatsWidth = 0;

        Base() {}

        int Count()
        {
            return this->Party.Count();
        }

        int Room()
        {
            return this->Party.Room;
        }

        Point Origin()
        {
            return this->Party.Origin();
        }

        bool InsideRoom()
        {
            return this->Room() != Room::None && this->Rooms[this->Room()].Inside(this->Origin());
        }

        bool IsAlive()
        {
            return Engine::IsAlive(this->Party);
        }

        bool Has(Character::Class character)
        {
            return this->Party.Has(character);
        }
    };

    struct Update
    {
        bool Scene = false;

        bool Party = false;

        bool Quit = false;
    };
}

#endif
