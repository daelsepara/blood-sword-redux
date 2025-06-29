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

    void GenerateBattlepits(Rogue::Base &rogue, int width, int height, int max_rooms, int min_size, int max_size, bool inner_tunnel)
    {
        rogue.Battlepits = Map::Base(width, height);

        // generate battlepits
        Battlepits::Generate(rogue.Battlepits, rogue.Rooms, max_rooms, min_size, max_size, inner_tunnel);

        // place party at the center of the first room
        rogue.Battlepits[rogue.Rooms[0].Center()].Occupant = Map::Object::PARTY;

        rogue.Battlepits[rogue.Rooms[0].Center()].Id = Map::Party;
    }

    Rogue::Base GenerateBattlepits(int width, int height, int max_rooms, int min_size, int max_size, bool inner_tunnel)
    {
        auto rogue = Rogue::Base();

        // generate battlepits
        Rogue::GenerateBattlepits(rogue, width, height, max_rooms, min_size, max_size, inner_tunnel);

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

    void RenderBattlepits(Scene::Base &scene, Rogue::Base &rogue)
    {
        // get fog color
        auto fog = Color::O(Color::Inactive, 0x08);

        // get leading character's awareness
        auto first = Engine::First(rogue.Party);

        auto radius = rogue.Party[first].Value(Attribute::Type::AWARENESS) / 2;

        auto view = FieldOfView::Compute(rogue.Battlepits, Point(rogue.Party.X, rogue.Party.Y), radius, FieldOfView::Method::SHADOW_CAST);

        for (auto y = rogue.Battlepits.Y; y < rogue.Battlepits.Y + rogue.Battlepits.ViewY; y++)
        {
            for (auto x = rogue.Battlepits.X; x < rogue.Battlepits.X + rogue.Battlepits.ViewX; x++)
            {
                auto offset = Point(x - rogue.Battlepits.X, y - rogue.Battlepits.Y);

                auto &tile = rogue.Battlepits[Point(x, y)];

                auto screen = Point(rogue.Battlepits.DrawX, rogue.Battlepits.DrawY) + offset * rogue.Battlepits.TileSize;

                auto visible = BloodSword::In(view, x, y);

                if (tile.IsOccupied() && (visible || tile.Explored))
                {
                    switch (tile.Occupant)
                    {
                    case Map::Object::PARTY:

                        if (Engine::IsAlive(rogue.Party) && tile.Id == Map::Party)
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

                        if (tile.Lifetime > 0 && tile.TemporaryAsset != Asset::Type::NONE)
                        {
                            scene.VerifyAndAdd(Scene::Element(Asset::Get(tile.TemporaryAsset), screen));
                        }
                        else if (tile.Asset != Asset::Type::NONE)
                        {
                            scene.VerifyAndAdd(Scene::Element(Asset::Get(tile.Asset), screen));
                        }

                        break;

                    case Map::Object::ITEMS:

                        if (tile.Id >= 0 && tile.Id < rogue.Loot.size() && rogue.Loot.size() > 0)
                        {
                            if (rogue.Loot[tile.Id].Items.size() > 0)
                            {
                                auto first = Engine::FirstAsset(rogue.Loot[tile.Id].Items);

                                if (first != Item::NotFound)
                                {
                                    auto &item = rogue.Loot[tile.Id].Items[first];

                                    scene.VerifyAndAdd(Scene::Element(Asset::Get(item.Asset), screen));
                                }
                            }
                        }

                        break;

                    default:

                        break;
                    }
                }
                else if (!tile.IsOccupied() && tile.Asset != Asset::Type::NONE && (visible || tile.Explored))
                {
                    scene.VerifyAndAdd(Scene::Element(Asset::Get(tile.Asset), screen));
                }

                if (visible)
                {
                    tile.Explored = true;
                }
                else if (tile.Explored)
                {
                    scene.Add(Scene::Element(screen.X, screen.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Blur));
                }
                else
                {
                    scene.Add(Scene::Element(screen.X, screen.Y, BloodSword::TileSize, BloodSword::TileSize, fog));
                }
            }
        }
    }

    void Handle(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Point point)
    {
        if (rogue.Battlepits[point].IsOccupied())
        {
            Interface::MessageBox(graphics, background, "TILE OCCUPIED", Color::Inactive);
        }
        else
        {
            Sound::Play(Sound::Type::ERROR);
        }
    }

    bool Actions(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Point point)
    {
        auto regenerate_scene = false;

        if (Battlepits::Empty(rogue.Battlepits, point))
        {
            Sound::Play(Sound::Type::ERROR);
        }
        else if (Rogue::Blocked(rogue, point))
        {
            Rogue::Handle(graphics, background, rogue, point);
        }
        else if (Rogue::Move(rogue, point))
        {
            regenerate_scene = true;
        }

        return regenerate_scene;
    }

    void Game(Graphics::Base &graphics, Party::Base &party)
    {
        auto rogue = Rogue::GenerateBattlepits(50, 50, 100, 5, 7, false);

        rogue.Party = party;

        if (rogue.Rooms.size() > 0)
        {
            auto center = rogue.Rooms[0].Center();

            rogue.Party.X = center.X;

            rogue.Party.Y = center.Y;

            auto panel_w = (graphics.Width - BloodSword::TileSize * 3) / 2;

            auto panel_h = (graphics.Height - BloodSword::TileSize * 2);

            rogue.Battlepits.ViewX = panel_w / rogue.Battlepits.TileSize;

            rogue.Battlepits.ViewY = panel_h / rogue.Battlepits.TileSize;

            auto offset_x = (panel_w - rogue.Battlepits.ViewX * rogue.Battlepits.TileSize) / 2;

            auto offset_y = (panel_h - rogue.Battlepits.ViewY * rogue.Battlepits.TileSize) / 2;

            rogue.Battlepits.DrawX = (BloodSword::TileSize * 2 + panel_w + offset_x);

            rogue.Battlepits.DrawY = (BloodSword::TileSize + offset_y);

            auto image_location = Point(BloodSword::TileSize + BloodSword::Pad, BloodSword::TileSize + BloodSword::Pad);

            auto image = Interface::GeneratePartyStats(graphics, rogue.Party, panel_w - BloodSword::LargePad);

            if (image)
            {
                image_location = Point(BloodSword::TileSize + BloodSword::Pad, BloodSword::TileSize + BloodSword::Pad);
            }

            auto regenerate_scene = true;

            auto scene = Scene::Base();

            auto done = false;

            while (!done)
            {
                if (regenerate_scene)
                {
                    scene = Scene::Base();

                    // left panel border
                    scene.Add(Scene::Element(BloodSword::TileSize, BloodSword::TileSize, panel_w, panel_h, Color::Background, Color::Active, BloodSword::Border));

                    if (image)
                    {
                        // add left panel
                        scene.VerifyAndAdd(Scene::Element(image, image_location));
                    }

                    // map battlepits panel border
                    scene.Add(Scene::Element(BloodSword::TileSize + panel_w + BloodSword::TileSize, BloodSword::TileSize, panel_w, panel_h, Color::Background, Color::Active, BloodSword::Border));

                    // center battlepits to party's location
                    Rogue::Center(rogue, Map::Object::PARTY, Map::Party);

                    // add battlepits to scene
                    Rogue::RenderBattlepits(scene, rogue);

                    regenerate_scene = false;
                }

                auto input = Input::RogueInput(graphics, {scene});

                if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
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

                            regenerate_scene = Rogue::Actions(graphics, scene, rogue, point);
                        }
                    }
                    else if (input.Type == Controls::Type::DOWN)
                    {
                        auto point = Point(rogue.Party.X, rogue.Party.Y);

                        if (point.Y < rogue.Battlepits.Height - 1)
                        {
                            point.Y++;

                            regenerate_scene = Rogue::Actions(graphics, scene, rogue, point);
                        }
                    }
                    else if (input.Type == Controls::Type::LEFT)
                    {
                        auto point = Point(rogue.Party.X, rogue.Party.Y);

                        if (point.X > 0)
                        {
                            point.X--;

                            regenerate_scene = Rogue::Actions(graphics, scene, rogue, point);
                        }
                    }
                    else if (input.Type == Controls::Type::RIGHT)
                    {
                        auto point = Point(rogue.Party.X, rogue.Party.Y);

                        if (point.X < rogue.Battlepits.Width - 1)
                        {
                            point.X++;

                            regenerate_scene = Rogue::Actions(graphics, scene, rogue, point);
                        }
                    }

                    input.Selected = false;
                }
            }

            BloodSword::Free(&image);
        }
    }
}

#endif
