#ifndef __ROGUE_CLASSES_HPP__
#define __ROGUE_CLASSES_HPP__

#include "Battlepits.hpp"
#include "InterfaceInventory.hpp"

// classes and functions to define rogue-like game mode entities
namespace BloodSword::Rogue
{
    // generic "not found" constant
    const int None = -1;

    // trigger types
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
        // trigger type
        Rogue::TriggerType Type = Rogue::TriggerType::NONE;

        // location in battlepits
        int X = Rogue::None;

        int Y = Rogue::None;

        // message to display (upon encountering)
        std::string EncounterMessage = std::string();

        // message to display (when successfully triggered)
        std::string TriggerMessage = std::string();

        // message to display (when trigger fails)
        std::string FailMessage = std::string();

        // message to display (trigger has been activated but not completed)
        std::string ActiveMessage = std::string();

        // trigger variables
        std::vector<std::string> Variables = {};

        // trigger has been activated (encountered for the first time)
        bool Activated = false;

        // trigger cannot be completed
        bool Failed = false;

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

        // number of characters in party
        int Count()
        {
            return this->Party.Count();
        }

        // current room number
        int Room()
        {
            return this->Party.Room;
        }

        // current location as point
        Point Origin()
        {
            return this->Party.Origin();
        }

        // check if party is inside a room
        bool InsideRoom()
        {
            return this->Room() != Room::None && this->Rooms[this->Room()].Inside(this->Origin());
        }

        // check if party is alive
        bool IsAlive()
        {
            return Engine::IsAlive(this->Party);
        }

        // check if party has specific character class
        bool Has(Character::Class character)
        {
            return this->Party.Has(character);
        }
    };

    // update flags struct
    struct Update
    {
        bool Scene = false;

        bool Party = false;

        bool Quit = false;
    };
}

#endif
