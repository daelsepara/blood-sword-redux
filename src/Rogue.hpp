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

    Rogue::Base GenerateBattlepits(int width, int height, int max_rooms, int min_size, int max_size)
    {
        auto rogue = Rogue::Base();

        // generate battlepits
        Rogue::GenerateBattlepits(rogue, width, height, max_rooms, min_size, max_size);

        // place party at the center of the first room
        rogue.Battlepits[rogue.Rooms[0].Center()].Occupant = Map::Object::PARTY;

        rogue.Battlepits[rogue.Rooms[0].Center()].Id = Map::Party;

        return rogue;
    }

    bool Blocked(Rogue::Base &rogue, Point point)
    {
        auto &tile = rogue.Battlepits[point];

        return (tile.IsOccupied() || tile.IsBlocked() || !tile.IsPassable());
    }

    // center battlepits on entity (map object type, id)
    void Center(Rogue::Base &rogue, Map::Object entity, int id)
    {
        auto src = rogue.Battlepits.Find(entity, id);

        rogue.Battlepits.X = src.X - (rogue.Battlepits.ViewX) / 2 + 1;

        rogue.Battlepits.Y = src.Y - (rogue.Battlepits.ViewY) / 2 + 1;

        rogue.Battlepits.CheckBounds();
    }

    bool Move(Rogue::Base &rogue, Point point)
    {
        auto moved = !Rogue::Blocked(rogue, point);

        auto &party = rogue.Party;

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

            Rogue::Center(rogue, Map::Object::PARTY, Map::Party);
        }

        return moved;
    }

    Scene::Base RenderBattlepits(Rogue::Base &rogue)
    {
        auto scene = Scene::Base();

        // add background frame
        scene.Add(Scene::Element(rogue.Battlepits.DrawX, rogue.Battlepits.DrawY, rogue.Battlepits.ViewX * rogue.Battlepits.TileSize, rogue.Battlepits.ViewY * rogue.Battlepits.TileSize, Color::Background, Color::Active, 1));

        for (auto y = rogue.Battlepits.Y; y < rogue.Battlepits.Y + rogue.Battlepits.ViewY; y++)
        {
            auto tile_y = y - rogue.Battlepits.Y;

            for (auto x = rogue.Battlepits.X; x < rogue.Battlepits.X + rogue.Battlepits.ViewX; x++)
            {
                auto tile_x = x - rogue.Battlepits.X;

                auto location = Point(x, y);

                auto &tile = rogue.Battlepits[location];

                auto screen = Point(rogue.Battlepits.DrawX, rogue.Battlepits.DrawY) + Point(tile_x, tile_y) * rogue.Battlepits.TileSize;

                if (tile.IsOccupied())
                {
                    switch (tile.Occupant)
                    {
                    case Map::Object::PARTY:

                        if (tile.Id == Map::Party)
                        {
                            auto first = Engine::First(rogue.Party);

                            auto &player = rogue.Party[first];

                            if (Engine::IsAlive(player))
                            {
                                scene.VerifyAndAdd(Scene::Element(Asset::Get(player.Asset), screen));
                            }
                        }

                        break;

                    case Map::Object::ENEMY:

                        if (tile.Id >= 0 && tile.Id < rogue.Opponents.size() && rogue.Opponents.size() > 0)
                        {
                            auto first = Engine::First(rogue.Opponents[tile.Id]);

                            auto &enemy = rogue.Opponents[tile.Id][first];

                            if (Engine::IsAlive(enemy))
                            {
                                scene.VerifyAndAdd(Scene::Element(Asset::Get(enemy.Asset), screen));
                            }
                        }

                        break;

                    case Map::Object::TEMPORARY_OBSTACLE:

                        if (tile.Lifetime > 0)
                        {
                            if (tile.TemporaryAsset != Asset::Type::NONE)
                            {
                                scene.VerifyAndAdd(Scene::Element(Asset::Get(tile.TemporaryAsset), screen));
                            }
                        }
                        else if (tile.Asset != Asset::Type::NONE)
                        {
                            scene.VerifyAndAdd(Scene::Element(Asset::Get(tile.Asset), screen));
                        }

                        break;

                    default:

                        break;
                    }
                }
                else if (tile.Asset != Asset::Type::NONE)
                {
                    scene.VerifyAndAdd(Scene::Element(Asset::Get(tile.Asset), screen));
                }
            }
        }

        return scene;
    }

    void Handle(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Point point)
    {
        Interface::MessageBox(graphics, background, "TILE OCCUPIED", Color::Inactive);
    }

    void Test(Graphics::Base &graphics)
    {
        auto rogue = Rogue::GenerateBattlepits(100, 100, 100, 6, 10);

        rogue.Party = Party::Base({
            Generate::Character(Character::Class::WARRIOR, 8),
            Generate::Character(Character::Class::TRICKSTER, 8),
            Generate::Character(Character::Class::SAGE, 8),
            Generate::Character(Character::Class::ENCHANTER, 8)});

        if (rogue.Rooms.size() > 0)
        {
            auto center = rogue.Rooms[0].Center();

            rogue.Party.X = center.X;

            rogue.Party.Y = center.Y;

            rogue.Battlepits.DrawX = (graphics.Width - rogue.Battlepits.ViewX * rogue.Battlepits.TileSize) / 2;

            rogue.Battlepits.DrawY = (graphics.Height - rogue.Battlepits.ViewY * rogue.Battlepits.TileSize) / 2;

            auto regenerate_scene = true;

            auto scene = Scene::Base();

            auto done = false;

            while (!done)
            {
                if (regenerate_scene)
                {
                    Rogue::Center(rogue, Map::Object::PARTY, Map::Party);

                    scene = Rogue::RenderBattlepits(rogue);

                    regenerate_scene = false;
                }

                auto input = Input::WaitForInput(graphics, {scene});

                if (input.Selected && (input.Type != Controls::Type::NONE) && !input.Hold)
                {
                    if (input.Type == Controls::Type::MENU)
                    {
                        done = true;
                    }
                    else if (input.Type == Controls::Type::UP)
                    {
                        auto point = Point(rogue.Party.X, rogue.Party.Y);

                        if (point.Y > 0)
                        {
                            point.Y--;

                            if (Rogue::Blocked(rogue, point))
                            {
                                Rogue::Handle(graphics, scene, rogue, point);
                            }
                            else if (Rogue::Move(rogue, point))
                            {
                                regenerate_scene = true;
                            }
                        }
                    }
                    else if (input.Type == Controls::Type::DOWN)
                    {
                        auto point = Point(rogue.Party.X, rogue.Party.Y);

                        if (point.Y < rogue.Battlepits.Height - 1)
                        {
                            point.Y++;

                            if (Rogue::Blocked(rogue, point))
                            {
                                Rogue::Handle(graphics, scene, rogue, point);
                            }
                            else if (Rogue::Move(rogue, point))
                            {
                                regenerate_scene = true;
                            }
                        }
                    }
                    else if (input.Type == Controls::Type::LEFT)
                    {
                        auto point = Point(rogue.Party.X, rogue.Party.Y);

                        if (point.X > 0)
                        {
                            point.X--;

                            if (Rogue::Blocked(rogue, point))
                            {
                                Rogue::Handle(graphics, scene, rogue, point);
                            }
                            else if (Rogue::Move(rogue, point))
                            {
                                regenerate_scene = true;
                            }
                        }
                    }
                    else if (input.Type == Controls::Type::RIGHT)
                    {
                        auto point = Point(rogue.Party.X, rogue.Party.Y);

                        if (point.X < rogue.Battlepits.Width - 1)
                        {
                            point.X++;

                            if (Rogue::Blocked(rogue, point))
                            {
                                Rogue::Handle(graphics, scene, rogue, point);
                            }
                            else if (Rogue::Move(rogue, point))
                            {
                                regenerate_scene = true;
                            }
                        }
                    }

                    input.Selected = false;
                }
            }
        }
    }
}

#endif
