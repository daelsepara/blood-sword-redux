#ifndef __ROGUE_GENERATOR_HPP__
#define __ROGUE_GENERATOR_HPP__

#include "RogueBase.hpp"

namespace BloodSword::Rogue
{
    void GenerateBattlepits(Rogue::Base &rogue, int width, int height, int max_rooms, int min_size, int max_size, Battlepits::Connection connection, bool inner_tunnel, int gap_size)
    {
        rogue.Battlepits = Map::Base(width, height);

        // clear population
        rogue.Rooms.clear();

        rogue.Opponents.clear();

        rogue.Loot.clear();

        rogue.Enemies = 0;

        rogue.Enemy = Map::NotFound;

        rogue.StatsWidth = 0;

        rogue.Party = Party::Base();

        // generate battlepits
        Battlepits::Generate(rogue.Battlepits, rogue.Rooms, max_rooms, min_size, max_size, connection, inner_tunnel, gap_size);

        // place party at the center of the first room
        rogue.Battlepits[rogue.Rooms[0].Center()].Occupant = Map::Object::PARTY;

        rogue.Battlepits[rogue.Rooms[0].Center()].Id = Map::Party;
    }

    Rogue::Base GenerateBattlepits(int width, int height, int max_rooms, int min_size, int max_size)
    {
        auto connection = Battlepits::Map(Engine::ToUpper(Interface::Settings["room_connections"]));

        auto inner_tunnel = Engine::ToUpper(Interface::Settings["tunnels"]) == "INNER";

        auto gap_size = int(Interface::Settings["gap_size"]);

        auto rogue = Rogue::Base();

        // generate battlepits
        Rogue::GenerateBattlepits(rogue, width, height, max_rooms, min_size, max_size, connection, inner_tunnel, gap_size);

        return rogue;
    }

    Character::Base NPC(const char *name, Skills::Type fight, Skills::Type shoot, Skills::List skills, std::vector<int> fpr, std::vector<int> psy, std::vector<int> awr, std::vector<int> end, std::vector<int> arm, std::vector<int> dmg_v, std::vector<int> dmg_m, Asset::Type asset)
    {
        auto fpr_value = fpr.size() > 1 ? fpr[Engine::Percentile.NextInt() % fpr.size()] : fpr[0];

        auto psy_value = psy.size() > 1 ? psy[Engine::Percentile.NextInt() % psy.size()] : psy[0];

        auto awr_value = awr.size() > 1 ? awr[Engine::Percentile.NextInt() % awr.size()] : awr[0];

        auto end_value = end.size() > 1 ? end[Engine::Percentile.NextInt() % end.size()] : end[0];

        auto arm_value = arm.size() > 1 ? arm[Engine::Percentile.NextInt() % arm.size()] : arm[0];

        auto dmg_v_value = dmg_v.size() > 1 ? dmg_v[Engine::Percentile.NextInt() % dmg_v.size()] : dmg_v[0];

        auto dmg_m_value = dmg_m.size() > 1 ? dmg_m[Engine::Percentile.NextInt() % dmg_m.size()] : dmg_m[0];

        auto npc = Generate::NPC(name, fight, shoot, skills, fpr_value, psy_value, awr_value, end_value, arm_value, dmg_v_value, dmg_m_value, 1000, asset);

        npc.Target = Target::Type::ENEMY;

        npc.Targets = {Target::Type::PLAYER};

        return npc;
    }

    // give each member the same item
    void AddItems(Party::Base &party, Item::Base item)
    {
        for (auto character = 0; character < party.Count(); character++)
        {
            party[character].Add(item);
        }
    }

    Party::Base GenerateMonsters(const char *name, Skills::Type fight, Skills::Type shoot, Skills::List skills, std::vector<int> fpr, std::vector<int> psy, std::vector<int> awr, std::vector<int> end, std::vector<int> arm, std::vector<int> dmg_v, std::vector<int> dmg_m, Asset::Type asset, int min_size, int max_size)
    {
        auto monsters = Party::Base();

        for (auto monster = 0; monster < Engine::Percentile.NextInt(min_size, max_size); monster++)
        {
            auto enemy = Rogue::NPC(name, fight, shoot, skills, fpr, psy, awr, end, arm, dmg_v, dmg_m, asset);

            monsters.Add(enemy);
        }

        return monsters;
    }

    // place ICON THE UNGODLY in last room
    void PlaceBoss(Rogue::Base &rogue)
    {
        auto id = int(rogue.Rooms.size() - 1);

        auto &room = rogue.Rooms[id];

        auto center = room.Center();

        auto monsters = Rogue::GenerateMonsters("ICON THE UNGODLY", Skills::Type::NONE, Skills::Type::NONE, {Skills::Type::RETRIBUTIVE_FIRE}, {8}, {8}, {7}, {28}, {2}, {2}, {2}, Asset::Type::ICON_THE_UNGODLY, 1, 1);

        monsters.X = center.X;

        monsters.Y = center.Y;

        monsters.Room = id;

        rogue.Opponents.push_back(monsters);

        rogue.Battlepits[center].Occupant = Map::Object::ENEMIES;
    }

    void PlaceMonsters(Rogue::Base &rogue, int number)
    {
        auto options = std::vector<int>(rogue.Rooms.size() - 2);

        std::iota(options.begin(), options.end(), 1);

        std::shuffle(options.begin(), options.end(), Engine::Random.Generator());

        for (auto enemies = 0; enemies < number; enemies++)
        {
            auto center = Point(-1, -1);

            auto room = Room::None;

            auto target = 0;

            // look for un-occupied room
            while (center.IsNone() || room == Room::None || rogue.Battlepits[center].IsOccupied())
            {
                target = Engine::Percentile.NextInt() % options.size();

                room = options[target];

                center = rogue.Rooms[room].Center();
            }

            // remove the room from contention
            options.erase(options.begin() + target);

            auto monsters = Party::Base();

            auto enemy_type = Engine::Percentile.NextInt(0, 100);

            if (enemy_type <= 30)
            {
                monsters = Rogue::GenerateMonsters("ASSASSIN", Skills::Type::NONE, Skills::Type::SHURIKEN, {Skills::Type::SHURIKEN}, {5, 6, 7}, {4, 5, 6}, {5, 6, 7}, {4, 5, 6}, {0}, {1}, {0}, Asset::Type::ASSASSIN, 3, 4);

                Rogue::AddItems(monsters, Item::Base("SHURIKEN POUCH", Item::Type::LIMITED_SHURIKEN, {Item::Property::CONTAINER, Item::Property::CANNOT_DROP, Item::Property::CANNOT_TRADE, Item::Property::EQUIPPED, Item::Property::RANGED}, Item::Type::SHURIKEN, 2));
            }
            else if (enemy_type <= 90)
            {
                monsters = Rogue::GenerateMonsters("BARBARIAN", Skills::Type::NONE, Skills::Type::NONE, {}, {6, 7, 8}, {3, 4, 5}, {5, 6, 7}, {8, 10, 12}, {1}, {1}, {0, 1, 2}, Asset::Type::BARBARIAN, 2, 3);
            }
            else
            {
                monsters = Rogue::GenerateMonsters("ADVENTURER", Skills::Type::NONE, Skills::Type::NONE, {}, {6, 7, 8}, {4, 5, 6}, {4, 5, 6}, {14, 18, 22}, {1, 2, 3}, {1, 2}, {0, 1, 2}, Asset::Type::CHARACTER, 1, 2);
            }

            // place monsters in battlepits
            monsters.X = center.X;

            monsters.Y = center.Y;

            monsters.Room = room;

            rogue.Opponents.push_back(monsters);

            rogue.Battlepits[center].Occupant = Map::Object::ENEMIES;
        }

        rogue.Enemies = rogue.Opponents.size();
    }

    // place item in specific room
    void PlaceItem(Rogue::Base &rogue, Item::Base item, int id)
    {
        if (id >= 0 && id < rogue.Rooms.size())
        {
            auto &room = rogue.Rooms[id];

            auto available = Rogue::GoodSpots(rogue, room);

            if (available.size() > 0)
            {
                // find a good location
                auto good = false;

                auto loot = Map::NotFound;

                Point location;

                while (!good)
                {
                    location = available[Engine::Percentile.NextInt() % available.size()];

                    loot = Rogue::FindLoot(rogue, location);

                    // add to existing loot bag if found
                    if (loot >= 0 && loot < rogue.Loot.size())
                    {
                        break;
                    }

                    good = Rogue::GoodSpot(rogue, room, location);
                }

                if (loot != Map::NotFound)
                {
                    rogue.Loot[loot].Items.push_back(item);
                }
                else
                {
                    rogue.Battlepits[location].Occupant = Map::Object::ITEMS;

                    auto loot = Rogue::Loot(location.X, location.Y);

                    loot.Items = {item};

                    rogue.Loot.push_back(loot);
                }
            }
        }
    }

    // place item in a random room
    void PlaceItem(Rogue::Base &rogue, Item::Base item)
    {
        // radomize room location
        auto id = Engine::Percentile.NextInt(1, rogue.Rooms.size() - 1);

        Rogue::PlaceItem(rogue, item, id);
    }

    // place items in a random room, ensuring only a single instance of the item is present
    void PlaceItems(Rogue::Base &rogue, Item::Base item, int number)
    {
        auto rooms = std::vector<int>(rogue.Rooms.size() - 2);

        std::iota(rooms.begin(), rooms.end(), 1);

        std::shuffle(rooms.begin(), rooms.end(), Engine::Random.Generator());

        // radomize room location
        for (auto i = 0; i < number; i++)
        {
            auto random = Engine::Percentile.NextInt() % rooms.size();

            auto id = rooms[random];

            Rogue::PlaceItem(rogue, item, id);

            rooms.erase(rooms.begin() + random);
        }
    }

    // distribute items randomly across several rooms
    void PlaceItems(Rogue::Base &rogue, Items::Inventory items)
    {
        auto rooms = std::vector<int>(rogue.Rooms.size() - 2);

        std::iota(rooms.begin(), rooms.end(), 1);

        std::shuffle(rooms.begin(), rooms.end(), Engine::Random.Generator());

        for (auto item : items)
        {
            auto random = Engine::Percentile.NextInt() % rooms.size();

            auto id = rooms[random];

            Rogue::PlaceItem(rogue, item, id);

            rooms.erase(rooms.begin() + random);
        }
    }

    // generate items from a selection
    void PlaceItems(Rogue::Base &rogue, Items::Inventory items, int number)
    {
        auto rooms = std::vector<int>(rogue.Rooms.size() - 2);

        std::iota(rooms.begin(), rooms.end(), 1);

        std::shuffle(rooms.begin(), rooms.end(), Engine::Random.Generator());

        auto options = items.size();

        for (auto i = 0; i < number; i++)
        {
            auto item = items[Engine::Percentile.NextInt() % options];

            auto random = Engine::Percentile.NextInt() % rooms.size();

            auto id = rooms[random];

            Rogue::PlaceItem(rogue, item, id);

            rooms.erase(rooms.begin() + random);
        }
    }

    // generate quantities of an item (e.g. GOLD, ARROWS)
    void GenerateItems(Rogue::Base &rogue, std::string name, Item::Type type, Asset::Type asset, int number, int min_quantity, int max_quantity, Item::Properties properties = {})
    {
        auto item = Item::Base(name.c_str(), type, properties, Item::Type::NONE, min_quantity == max_quantity ? min_quantity : Engine::Percentile.NextInt(min_quantity, max_quantity));

        item.Asset = asset;

        Rogue::PlaceItems(rogue, item, number);
    }

    // generate gold loot
    void PlaceGold(Rogue::Base &rogue, int number, int min_gold, int max_gold)
    {
        Rogue::GenerateItems(rogue, "GOLD", Item::Type::GOLD, Asset::Type::MONEY, number, min_gold, max_gold);
    }

    // generate arrows loot
    void PlaceArrows(Rogue::Base &rogue, int number, int min_arrows, int max_arrows)
    {
        Rogue::GenerateItems(rogue, "ARROWS", Item::Type::ARROW, Asset::Type::QUIVER, number, min_arrows, max_arrows);
    }

    // generate food loot
    void PlaceFood(Rogue::Base &rogue, int number)
    {
        Rogue::GenerateItems(rogue, "FOOD", Item::Type::FOOD, Asset::Type::FOOD, number, 1, 1, {Item::Property::EDIBLE});
    }

    // generate potion/scroll of healing loot
    void PlacePotions(Rogue::Base &rogue, int number)
    {
        auto potion = Item::Base("POTION", Item::Type::POTION_OF_HEALING, {Item::Property::LIQUID, Item::Property::COMBAT}, Item::Type::NONE, 1, Asset::Type::DRINK);

        auto scroll = Item::Base("SCROLL", Item::Type::SCROLL_HEALING, {Item::Property::READABLE}, Item::Type::NONE, 1, Asset::Type::READ);

        // potions 67% more likely than scrolls
        Rogue::PlaceItems(rogue, {potion, potion, scroll}, number);
    }

    void PlaceUniqueItems(Rogue::Base &rogue)
    {
        // steel sceptre
        auto sceptre = Item::Base("STEEL SCEPTRE", Item::Type::STEEL_SCEPTRE, {Item::Property::CONTAINER, Item::Property::COMBAT, Item::Property::REQUIRES_TARGET}, Item::Type::CHARGE, 5, Asset::Type::STEEL_SCEPTRE);

        // vellum scroll
        auto vellum = Item::Base("VELLUM SCROLL", Item::Type::VELLUM_SCROLL, {Item::Property::READABLE}, Item::Type::NONE, 1, Asset::Type::READ);

        Rogue::PlaceItems(rogue, {sceptre, vellum});
    }

    void PlaceLoot(BloodSword::Rogue::Base &rogue, int number, int min_gold, int max_gold)
    {
        // 25% rooms has gold loot
        Rogue::PlaceGold(rogue, number, min_gold, max_gold);

        // 25% rooms has food loot
        Rogue::PlaceFood(rogue, number);

        // 25% rooms has potion loot
        Rogue::PlacePotions(rogue, number);

        // place unique items
        Rogue::PlaceUniqueItems(rogue);

        // 25% rooms has arrows loot if TRICKSTER or SAGE present in party
        if (rogue.Has(Character::Class::TRICKSTER) || rogue.Has(Character::Class::SAGE))
        {
            Rogue::PlaceArrows(rogue, rogue.Rooms.size() / 4, 4, 20);
        }
    }
}

#endif