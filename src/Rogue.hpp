#ifndef __ROGUE_HPP__
#define __ROGUE_HPP__

#include "Battlepits.hpp"
#include "InterfaceInventory.hpp"

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

        Point Location()
        {
            return Point(X, Y);
        }

        Loot() {}

        Loot(int x, int y) : X(x), Y(y) {}

        Loot(Point point) : X(point.X), Y(point.Y) {}
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
        std::vector<Rogue::Loot> Loot = {};

        // player party adventuring in battlepits
        Party::Base Party;

        Base() {}
    };

    int FindLoot(Rogue::Base &rogue, Point point)
    {
        auto &Loot = rogue.Loot;

        auto found = -1;

        for (auto loot = 0; loot < Loot.size(); loot++)
        {
            if (Loot[loot].Location() == point)
            {
                found = loot;

                break;
            }
        }

        return found;
    }

    int FindOpponents(Rogue::Base &rogue, Point point)
    {
        auto found = -1;

        for (auto opponent = 0; opponent < rogue.Opponents.size(); opponent++)
        {
            if (rogue.Opponents[opponent].Origin() == point)
            {
                found = opponent;

                break;
            }
        }

        return found;
    }

    void GenerateBattlepits(Rogue::Base &rogue, int width, int height, int max_rooms, int min_size, int max_size, Battlepits::Connection connection, bool inner_tunnel)
    {
        rogue.Battlepits = Map::Base(width, height);

        // generate battlepits
        Battlepits::Generate(rogue.Battlepits, rogue.Rooms, max_rooms, min_size, max_size, connection, inner_tunnel);

        // place party at the center of the first room
        rogue.Battlepits[rogue.Rooms[0].Center()].Occupant = Map::Object::PARTY;

        rogue.Battlepits[rogue.Rooms[0].Center()].Id = Map::Party;
    }

    Rogue::Base GenerateBattlepits(int width, int height, int max_rooms, int min_size, int max_size, bool inner_tunnel)
    {
        auto connection = Battlepits::Map(Engine::ToUpper(Interface::Settings["tunnels"]));

        auto rogue = Rogue::Base();

        // generate battlepits
        Rogue::GenerateBattlepits(rogue, width, height, max_rooms, min_size, max_size, connection, inner_tunnel);

        // place party at the center of the first room
        rogue.Battlepits[rogue.Rooms[0].Center()].Occupant = Map::Object::PARTY;

        rogue.Battlepits[rogue.Rooms[0].Center()].Id = Map::Party;

        return rogue;
    }

    // render battlepits
    void RenderBattlepits(Scene::Base &scene, Rogue::Base &rogue, FieldOfView::Method method, bool sight = true)
    {
        auto &map = rogue.Battlepits;

        auto &party = rogue.Party;

        // set fog color
        auto fog = Color::O(Color::Active, 0x14);

        // get leading character's awareness
        auto first = Engine::First(party);

        // set field of view radius
        auto radius = party[first].Value(Attribute::Type::AWARENESS) / 2;

        // calculate field of view
        auto view = FieldOfView::Compute(map, party.Origin(), radius, method);

        // offset for FoV illumination
        auto fov_offset = BloodSword::Pad;

        // size of FoV illumination
        auto fov_size = BloodSword::TileSize - fov_offset * 2;

        for (auto y = map.Y; y < map.Y + map.ViewY; y++)
        {
            for (auto x = map.X; x < map.X + map.ViewX; x++)
            {
                auto offset = Point(x - map.X, y - map.Y);

                auto &tile = map[Point(x, y)];

                auto screen = Point(map.DrawX, map.DrawY) + offset * map.TileSize;

                auto visible = BloodSword::In(view, x, y);

                auto loot_id = -1;

                auto opponent_id = -1;

                if (tile.IsOccupied() && (visible || tile.Explored))
                {
                    switch (tile.Occupant)
                    {
                    case Map::Object::PARTY:

                        if (Engine::IsAlive(party) && tile.Id == Map::Party)
                        {
                            auto first = Engine::First(party);

                            auto &player = party[first];

                            if (Engine::IsAlive(player))
                            {
                                scene.VerifyAndAdd(Scene::Element(Asset::Get(player.Asset), screen));
                            }
                        }

                        break;

                    case Map::Object::ENEMY:

                        opponent_id = Rogue::FindOpponents(rogue, Point(x, y));

                        if (opponent_id >= 0 && opponent_id < rogue.Opponents.size() && rogue.Opponents.size() > 0)
                        {
                            auto first = Engine::First(rogue.Opponents[opponent_id]);

                            auto &enemy = rogue.Opponents[opponent_id][first];

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

                        loot_id = Rogue::FindLoot(rogue, Point(x, y));

                        if (loot_id >= 0 && loot_id < rogue.Loot.size() && rogue.Loot.size() > 0)
                        {
                            auto &loot = rogue.Loot[loot_id];

                            if (loot.Items.size() > 0)
                            {
                                auto first = Engine::FirstAsset(loot.Items);

                                if (first != Item::NotFound)
                                {
                                    auto &item = loot.Items[first];

                                    scene.VerifyAndAdd(Scene::Element(Asset::Get(item.Asset), screen));
                                }
                                else
                                {
                                    scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::ITEMS), screen));
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
                else if (tile.Asset == Asset::Type::NONE && visible && sight)
                {
                    scene.Add(Scene::Element(screen.X + fov_offset, screen.Y + fov_offset, fov_size, fov_size, Color::O(Color::Highlight, 0x20)));
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

    bool Blocked(Rogue::Base &rogue, Point point)
    {
        auto &tile = rogue.Battlepits[point];

        auto items = (tile.IsOccupied() && tile.Type == Map::Object::ITEMS);

        auto blockers = (tile.IsOccupied() && tile.Type != Map::Object::ITEMS);

        if (items)
        {
            auto loot = Rogue::FindLoot(rogue, point);

            if (loot >= 0 && loot < rogue.Loot.size())
            {
                items &= (rogue.Loot[loot].Items.size() > 0);
            }
        }

        if (items)
        {
            return true;
        }

        return (blockers || tile.IsBlocked() || !tile.IsPassable());
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
            auto from = party.Origin();

            auto &origin = rogue.Battlepits[from];

            auto &destination = rogue.Battlepits[point];

            origin.Occupant = Map::Object::NONE;

            origin.Id = Map::NotFound;

            destination.Occupant = Map::Object::PARTY;

            destination.Id = Map::Party;

            party.Room = destination.Room;

            party.X = point.X;

            party.Y = point.Y;
        }

        return moved;
    }

    // manage item found in room
    void ManageItem(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Items::Inventory &items, int id)
    {
        auto &party = rogue.Party;

        auto assets = Asset::List();

        auto controls = Controls::Collection();

        auto captions = std::vector<std::string>();

        // take item
        assets.push_back(Asset::Type::USE);

        controls.push_back(Controls::Type::TAKE);

        captions.push_back("TAKE");

        // go back
        assets.push_back(Asset::Type::BACK);

        controls.push_back(Controls::Type::BACK);

        auto values = std::vector<int>();

        for (auto i = 0; i < controls.size(); i++)
        {
            values.push_back(i);
        }

        auto done = false;

        while (!done)
        {
            auto selection = Interface::SelectIcons(graphics, background, items[id].Name.c_str(), assets, values, captions, 1, 1, Asset::Type::NONE, false);

            if (selection.size() == 1)
            {
                auto input = controls[selection[0]];

                if (input == Controls::Type::BACK)
                {
                    done = true;
                }
                else if (input == Controls::Type::TAKE)
                {
                    auto character = Engine::FirstClass(party);

                    if (Engine::Count(party) > 1)
                    {
                        std::string message = "WHO TAKES THE " + items[id].Name + "?";

                        character = Interface::SelectCharacter(graphics, background, party, message.c_str(), true, true, false, false, true);
                    }

                    if (character != Character::Class::NONE)
                    {
                        done = Interface::TransferItem(graphics, background, party, party[character], items, id);
                    }
                }
            }
        }
    }

    // show inventory of a location
    void ShowLoot(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Point point)
    {
        auto loot = Rogue::FindLoot(rogue, point);

        if (loot >= 0 && loot < rogue.Loot.size())
        {
            auto &items = rogue.Loot[loot].Items;

            auto exit = false;

            if (items.size() > 0)
            {
                while (!exit)
                {
                    auto limit = std::min(4, int(items.size()));

                    auto start = 0;

                    auto last = start + limit;

                    auto options = int(items.size());

                    // wrap length
                    auto wrap = BloodSword::Wrap;

                    auto text_list = Graphics::TextList();

                    for (auto &item : items)
                    {
                        text_list.push_back(Graphics::RichText(item.String(true), Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, wrap));
                    }

                    auto menu = Graphics::CreateText(graphics, text_list);

                    // default width
                    auto w = wrap;

                    // default height
                    auto h = BloodSword::TileSize;

                    // padding
                    auto pads = BloodSword::LargePad;

                    for (auto &item : menu)
                    {
                        w = std::max(BloodSword::Width(item) + pads, wrap);

                        h = std::max(BloodSword::Height(item) + pads, h);
                    }

                    auto x = (graphics.Width - w) / 2 - (items.size() > limit ? (BloodSword::HalfTile + 1) : 0);

                    auto y = (graphics.Height - h * (limit + 1)) / 2 - BloodSword::HalfTile + BloodSword::Pad;

                    auto input = Controls::User();

                    auto done = false;

                    auto frame_x = x - BloodSword::HalfTile;

                    auto frame_y = y - BloodSword::HalfTile + BloodSword::Pad;

                    auto frame_w = w + BloodSword::HalfTile * (options > limit ? 4 : 2);

                    auto frame_h = (limit * h) + (BloodSword::HalfTile * 5) + BloodSword::OddPad;

                    while (!done)
                    {
                        auto overlay = Interface::Menu(menu, x, y, w, h, start, last, limit, Color::Background, Color::Background, Color::Active, true);

                        // add frame at the back
                        overlay.Elements.insert(overlay.Elements.begin(), Scene::Element(frame_x, frame_y, frame_w, frame_h, Color::Background, Color::Active, BloodSword::Border));

                        auto &lastControl = overlay.Controls.back();

                        auto id = lastControl.Id + 1;

                        auto first = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                        auto bottom = overlay.Controls[first + limit - 1].Y + h + BloodSword::LargePad;

                        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), x - BloodSword::SmallPad, bottom));

                        overlay.Add(Controls::Base(Controls::Type::BACK, id, id, id, first + limit - 1, id, x - BloodSword::SmallPad, bottom, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

                        Interface::ClearScrolling(overlay, input, Controls::Type::SCROLL_UP, Controls::Type::SCROLL_DOWN);

                        input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

                        if ((input.Selected && input.Type != Controls::Type::NONE && !input.Hold) || input.Up || input.Down)
                        {
                            if (input.Type == Controls::Type::BACK)
                            {
                                done = true;

                                exit = true;
                            }
                            else if (input.Type == Controls::Type::SCROLL_UP || input.Up)
                            {
                                Interface::ScrollUp(overlay, input, Controls::Type::SCROLL_UP, options, limit, start, last);
                            }
                            else if (input.Type == Controls::Type::SCROLL_DOWN || input.Down)
                            {
                                Interface::ScrollDown(overlay, input, Controls::Type::SCROLL_DOWN, options, limit, start, last);
                            }
                            else if (input.Type == Controls::Type::CHOICE)
                            {
                                auto list = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                                auto choice = start + (input.Current - list);

                                if (choice >= 0 && choice < items.size())
                                {
                                    Rogue::ManageItem(graphics, background, rogue, items, choice);
                                }

                                // check if item list is unchanged
                                if (items.size() == 0)
                                {
                                    done = true;

                                    exit = true;
                                }
                                else if (items.size() != options)
                                {
                                    done = true;
                                }
                            }
                        }
                    }

                    BloodSword::Free(menu);
                }
            }

            if (items.size() == 0)
            {
                rogue.Battlepits[point].Id = -1;

                rogue.Battlepits[point].Occupant = Map::Object::NONE;
            }
        }
    }

    void ShowMap(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue)
    {
        auto &map = rogue.Battlepits;

        auto &party = rogue.Party;

        auto scale = Point(8, 8);

        auto surface = Graphics::CreateSurface(map.Width * scale.X, map.Height * scale.Y);

        if (surface)
        {
            SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 0, 0, 0, 255));

            SDL_Rect rect;

            rect.w = scale.X;

            rect.h = scale.Y;

            for (auto y = 0; y < map.Height; y++)
            {
                for (auto x = 0; x < map.Width; x++)
                {
                    SDL_Surface *surface_asset = nullptr;

                    auto &tile = map[Point(x, y)];

                    rect.x = x * scale.X;

                    rect.y = y * scale.Y;

                    if (party.Origin() == Point(x, y))
                    {
                        surface_asset = Asset::Surface(Asset::Type::WHITE_SPACE, Color::Highlight);
                    }
                    else if (tile.Explored && tile.Asset != Asset::Type::NONE)
                    {
                        surface_asset = Asset::Surface(tile.Asset);
                    }
                    else if (!tile.Explored)
                    {
                        surface_asset = Graphics::CreateSurface(scale.X, scale.Y);

                        SDL_FillRect(surface_asset, nullptr, SDL_MapRGBA(surface_asset->format, Color::R(Color::Inactive), Color::G(Color::Inactive), Color::B(Color::Inactive), 255));
                    }

                    if (surface_asset)
                    {
                        Graphics::RenderAssetScaled(surface, surface_asset, rect);

                        BloodSword::Free(&surface_asset);
                    }
                }
            }

            auto texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

            if (texture)
            {
                auto width = BloodSword::Width(texture) + BloodSword::LargePad;

                auto height = BloodSword::TileSize * 6 + BloodSword::LargePad;

                auto x = (graphics.Width - width) / 2;

                auto y = (graphics.Height - height) / 2;

                // calculate offset to center current location
                auto text_h = height - (BloodSword::TileSize + BloodSword::Pad * 3);

                auto loc = party.Y * scale.Y + scale.Y / 2;

                auto offset = 0;

                if (loc > text_h / 2)
                {
                    offset = (loc - text_h / 2);
                }

                offset = std::min(std::max(0, offset), BloodSword::Height(texture) - text_h);

                Interface::ScrollableImageBox(graphics, background, texture, width, height, x, y, Color::Background, Color::Active, BloodSword::Border, Color::Active, Asset::Type::SWORDTHRUST, Asset::Type::UP, Asset::Type::DOWN, true, offset);

                BloodSword::Free(&texture);
            }

            BloodSword::Free(&surface);
        }
    }

    // manage item while in rogue mode
    void ManageItem(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Character::Class character_class, int id)
    {
        auto &party = rogue.Party;

        auto &character = party[character_class];

        auto &map = rogue.Battlepits;

        auto &items = character.Items;

        auto exit = false;

        Items::Inventory *destination = nullptr;

        Items::Inventory ether = {};

        // find location for room where things can be dropped
        if (party.Room != Room::None)
        {
            auto &room = rogue.Rooms[party.Room];

            auto available = Point(-1, -1);

            for (auto direction : Map::Directions)
            {
                auto point = party.Origin() + direction;

                for (auto &loot : rogue.Loot)
                {
                    if (point.X == loot.X && point.Y == loot.Y)
                    {
                        available = Point(loot.X, loot.Y);

                        destination = &loot.Items;

                        break;
                    }
                }
            }

            if (available.IsNone())
            {
                for (auto direction : Map::Directions)
                {
                    auto point = party.Origin() + direction;

                    auto inside = (point.X > room.X1 + 1 && point.X < room.X2 - 2 && point.Y > room.Y1 + 1 && point.Y < room.Y2 - 2);

                    auto empty = map[point].IsPassable() && !map[point].IsOccupied();

                    if (inside && empty)
                    {
                        rogue.Loot.push_back(Rogue::Loot(point.X, point.Y));

                        destination = &(rogue.Loot.back().Items);

                        map[point].Occupant = Map::Object::ITEMS;

                        map[point].Id = int(rogue.Loot.size() - 1);

                        available = point;

                        break;
                    }
                }
            }

            if (available.IsNone())
            {
                destination = &ether;
            }
        }

        while (!exit)
        {
            auto assets = Asset::List();

            auto controls = Controls::Collection();

            auto captions = std::vector<std::string>();

            auto is_equipment = (items[id].Has(Item::Property::WEAPON) || items[id].Has(Item::Property::ARMOUR) || items[id].Has(Item::Property::ACCESSORY));

            if (is_equipment)
            {
                if (items[id].Has(Item::Property::EQUIPPED))
                {
                    // unequip weapon / armour
                    assets.push_back(Asset::Type::CANCEL);

                    controls.push_back(Controls::Type::UNEQUIP);

                    captions.push_back("UNEQUIP");
                }
                else
                {
                    // equip weapon / armour
                    assets.push_back(Asset::Type::CONFIRM);

                    controls.push_back(Controls::Type::EQUIP);

                    captions.push_back("EQUIP");
                }
            }

            if (!items[id].Has(Item::Property::CONTAINER) && !is_equipment)
            {
                if (items[id].Has(Item::Property::LIQUID))
                {
                    // drink
                    assets.push_back(Asset::Type::DRINK);

                    controls.push_back(Controls::Type::DRINK);

                    captions.push_back("DRINK");
                }
                else if (items[id].Has(Item::Property::READABLE))
                {
                    // read / open scroll
                    assets.push_back(Asset::Type::READ);

                    controls.push_back(Controls::Type::READ);

                    captions.push_back("READ");
                }
                else if (items[id].Has(Item::Property::EDIBLE))
                {
                    // read / open scroll
                    assets.push_back(Asset::Type::FOOD);

                    controls.push_back(Controls::Type::EAT);

                    captions.push_back("EAT");
                }
                else
                {
                    // default: use
                    assets.push_back(Asset::Type::USE);

                    controls.push_back(Controls::Type::USE);

                    captions.push_back("USE");
                }
            }

            // item description / identify
            if (Items::FoundDescription(items[id].Type))
            {
                if (!items[id].Revealed)
                {
                    if (character.Class == Character::Class::SAGE)
                    {
                        assets.push_back(Asset::Type::IDENTIFY);

                        controls.push_back(Controls::Type::IDENTIFY);

                        captions.push_back("IDENTIFY");
                    }
                }
                else
                {
                    // add info button
                    assets.push_back(Asset::Type::ABOUT);

                    controls.push_back(Controls::Type::ABOUT);

                    captions.push_back("ABOUT");
                }
            }

            if (items[id].Has(Item::Property::CONTAINER) && items[id].Contains == Item::Type::GOLD && items[id].Quantity > 0 && Engine::Count(party) > 1 && !character.Has(Character::Status::TASK))
            {
                // money
                assets.push_back(Asset::Type::MONEY);

                controls.push_back(Controls::Type::MONEY);

                std::string gold_string = std::string(Item::TypeMapping[items[id].Contains]) + ": " + std::to_string(character.Quantity(items[id].Contains));

                captions.push_back(gold_string);
            }

            if (items[id].Has(Item::Property::CONTAINER) && items[id].Contains == Item::Type::ARROW && items[id].Quantity > 0 && Engine::Count(party) > 1 && !character.Has(Character::Status::TASK))
            {
                // money
                assets.push_back(Asset::Type::QUIVER);

                controls.push_back(Controls::Type::QUIVER);

                std::string arrow_string = std::string(Item::TypeMapping[items[id].Contains]) + ": " + std::to_string(character.Quantity(items[id].Contains));

                captions.push_back(arrow_string);
            }

            if (Engine::Count(party) > 1 && !items[id].Has(Item::Property::CANNOT_TRADE) && !character.Has(Character::Status::TASK))
            {
                // trade
                assets.push_back(Asset::Type::TRADE);

                controls.push_back(Controls::Type::TRADE);

                captions.push_back("TRADE");
            }

            if (!items[id].Has(Item::Property::CANNOT_DROP) && party.Room != Room::None)
            {
                assets.push_back(Asset::Type::DROP);

                controls.push_back(Controls::Type::DROP);

                captions.push_back("DROP");
            }

            assets.push_back(Asset::Type::BACK);

            controls.push_back(Controls::Type::BACK);

            auto values = std::vector<int>();

            for (auto i = 0; i < controls.size(); i++)
            {
                values.push_back(i);
            }

            auto done = false;

            while (!done)
            {
                auto selection = Interface::SelectIcons(graphics, background, items[id].Name.c_str(), assets, values, captions, 1, 1, Asset::Type::NONE, false);

                if (selection.size() == 1)
                {
                    auto input = controls[selection[0]];

                    if (input == Controls::Type::BACK)
                    {
                        if (Interface::CheckEncumbranceLimit(character))
                        {
                            done = true;

                            exit = true;
                        }
                        else
                        {
                            Interface::ErrorMessage(graphics, background, Interface::MSG_ITEMS);
                        }
                    }
                    else if (input == Controls::Type::EQUIP)
                    {
                        auto is_weapon = items[id].Is(Item::Property::WEAPON);

                        auto is_armour = items[id].Is(Item::Property::ARMOUR);

                        auto is_accessory = items[id].Is(Item::Property::ACCESSORY);

                        auto is_melee = is_weapon && items[id].Is(Item::Property::PRIMARY);

                        auto is_secondary = is_weapon && items[id].Is(Item::Property::SECONDARY);

                        auto weapon_type = is_weapon ? ((is_melee || is_secondary) ? (is_secondary ? Item::Property::SECONDARY : Item::Property::PRIMARY) : Item::Property::RANGED) : Item::Property::NONE;

                        auto equipped = is_weapon ? character.EquippedWeapon(weapon_type) : (is_armour ? character.EquippedArmour() : -1);

                        if ((equipped != 1 && equipped >= 0 && equipped < items.size()) && (is_weapon || is_armour) && !is_accessory)
                        {
                            // un-equip
                            items[equipped].Remove(Item::Property::EQUIPPED);
                        }

                        if (!items[id].Is(Item::Property::EQUIPPED))
                        {
                            // equip
                            items[id].Add(Item::Property::EQUIPPED);

                            done = true;
                        }
                    }
                    else if (input == Controls::Type::UNEQUIP)
                    {
                        if (items[id].Is(Item::Property::EQUIPPED))
                        {
                            items[id].Remove(Item::Property::EQUIPPED);

                            done = true;
                        }
                    }
                    else if (input == Controls::Type::DROP)
                    {
                        if (Interface::Confirm(graphics, background, "ARE YOU SURE?", Color::Background, Color::Highlight, BloodSword::Border, Color::Active, true))
                        {
                            if (destination)
                            {
                                std::string action = character.Name + " DROPPED";

                                Interface::DropItem(graphics, background, action, *destination, items, id);

                                done = true;

                                exit = true;
                            }
                            else
                            {
                                Interface::InternalError(graphics, background, std::string("Internal Error: DropItem"));
                            }
                        }
                    }
                    else if ((input == Controls::Type::IDENTIFY) || (input == Controls::Type::ABOUT))
                    {
                        auto is_sage = (character.Class == Character::Class::SAGE);

                        auto is_revealed = items[id].Revealed;

                        if ((is_sage || is_revealed) || (party.Has(Character::Class::SAGE) && Engine::IsAlive(party[Character::Class::SAGE])))
                        {
                            if (Items::FoundDescription(items[id].Type) && input == Controls::Type::ABOUT)
                            {
                                Interface::ShowBookDescription(graphics, background, items[id].Type);
                            }
                        }
                        else
                        {
                            Interface::ErrorMessage(graphics, background, Interface::MSG_IDENTIFY);
                        }
                    }
                    else if (input == Controls::Type::MONEY)
                    {
                        if (Engine::IsAlive(party) && Engine::Count(party) > 1)
                        {
                            std::string message = "SELECT THE PLAYER TO RECEIVE GOLD";

                            auto other_character = Interface::SelectCharacter(graphics, background, party, message.c_str(), true, true, false, false, true);

                            if (character.Class != other_character && other_character != Character::Class::NONE && party.Has(other_character) && Engine::IsAlive(party[other_character]))
                            {
                                if (!party[other_character].Has(Character::Status::TASK))
                                {
                                    std::string transfer_money = "SELECT HOW MUCH GOLD WILL BE TRANSFERRED";

                                    auto transfer_item = Item::Type::GOLD;

                                    auto transfer = Interface::GetNumber(graphics, background, transfer_money.c_str(), 0, character.Quantity(transfer_item), Asset::Type::MONEY, Asset::Type::UP, Asset::Type::DOWN);

                                    if (transfer > 0)
                                    {
                                        if (other_character != Character::Class::NONE && party.Has(other_character) && Engine::IsAlive(party[other_character]))
                                        {
                                            party[other_character].Add(transfer_item, transfer);

                                            character.Remove(transfer_item, transfer);

                                            done = true;

                                            exit = true;
                                        }
                                        else
                                        {
                                            Interface::InternalError(graphics, background, std::string("Internal Error: MONEY"));
                                        }
                                    }
                                }
                                else
                                {
                                    message = party[other_character].Name + " IS AWAY";

                                    Interface::MessageBox(graphics, background, message, Color::Highlight);
                                }
                            }
                        }
                        else
                        {
                            Interface::InternalError(graphics, background, std::string("Internal Error: MONEY"));
                        }
                    }
                    else if (input == Controls::Type::QUIVER)
                    {
                        if (Engine::IsAlive(party) && Engine::Count(party) > 1)
                        {
                            std::string message = "SELECT THE PLAYER TO RECEIVE ARROWS";

                            auto other_character = Interface::SelectCharacter(graphics, background, party, message.c_str(), true, true, false, false, true);

                            if (character.Class != other_character && other_character != Character::Class::NONE && party.Has(other_character) && Engine::IsAlive(party[other_character]))
                            {
                                if (!party[other_character].Has(Character::Status::TASK))
                                {
                                    if (party[other_character].Has(Item::Container(Item::Type::ARROW)))
                                    {
                                        std::string transfer_arrows = "HOW MANY ARROWS TO TRANSFER?";

                                        auto transfer_item = Item::Type::ARROW;

                                        auto transfer = Interface::GetNumber(graphics, background, transfer_arrows.c_str(), 0, character.Quantity(transfer_item), Asset::Type::QUIVER, Asset::Type::UP, Asset::Type::DOWN);

                                        if (transfer > 0)
                                        {
                                            if (other_character != Character::Class::NONE && party.Has(other_character) && Engine::IsAlive(party[other_character]))
                                            {
                                                party[other_character].Add(transfer_item, transfer);

                                                character.Remove(transfer_item, transfer);

                                                done = true;

                                                exit = true;
                                            }
                                            else
                                            {
                                                Interface::InternalError(graphics, background, std::string("Internal Error: ARROWS"));
                                            }
                                        }
                                    }
                                    else
                                    {
                                        std::string quiver = party[other_character].Name + " DOES NOT HAVE A QUIVER";

                                        Interface::MessageBox(graphics, background, quiver, Color::Highlight);
                                    }
                                }
                                else
                                {
                                    message = party[other_character].Name + " IS AWAY";

                                    Interface::MessageBox(graphics, background, message, Color::Highlight);
                                }
                            }
                        }
                        else
                        {
                            Interface::InternalError(graphics, background, std::string("Internal Error: ARROWS"));
                        }
                    }
                    else if (input == Controls::Type::TRADE)
                    {
                        if (Engine::IsAlive(party) && Engine::Count(party) > 1)
                        {
                            std::string message = "WHO SHALL RECEIVE THE " + items[id].Name + "?";

                            auto other_character = Interface::SelectCharacter(graphics, background, party, message.c_str(), true, true, false, false, true);

                            if (character.Class != other_character && other_character != Character::Class::NONE && party.Has(other_character) && Engine::IsAlive(party[other_character]))
                            {
                                if (!party[other_character].Has(Character::Status::TASK))
                                {
                                    items[id].Has(Item::Property::EQUIPPED);

                                    done = Interface::TransferItem(graphics, background, party, party[other_character], items, id);

                                    if (done)
                                    {
                                        exit = true;
                                    }
                                }
                                else
                                {
                                    message = party[other_character].Name + " IS AWAY";

                                    Interface::MessageBox(graphics, background, message, Color::Highlight);
                                }
                            }
                        }
                        else
                        {
                            Interface::InternalError(graphics, background, std::string("Internal Error: TRADE"));
                        }
                    }
                    else if (input == Controls::Type::DRINK || input == Controls::Type::EAT || input == Controls::Type::READ)
                    {
                        party.ChosenCharacter = character.Class;

                        if (Items::FoundDescription(items[id].Type))
                        {
                            Interface::ShowBookDescription(graphics, background, items[id].Type);
                        }

                        Interface::ItemEffects(graphics, background, party, character, items[id].Type);

                        done = true;

                        exit = true;
                    }
                    else
                    {
                        Interface::NotImplemented(graphics, background);
                    }
                }
            }
        }
    }

    // show player inventory
    void ShowInventory(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Character::Base &character)
    {
        auto exit = false;

        while (!exit)
        {
            auto limit = std::min(4, int(character.Items.size()));

            auto start = 0;

            auto last = start + limit;

            auto options = int(character.Items.size());

            // wrap length
            auto wrap = BloodSword::Wrap;

            auto text_list = Graphics::TextList();

            for (auto &item : character.Items)
            {
                text_list.push_back(Graphics::RichText(item.String(true), Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, wrap));
            }

            auto menu = Graphics::CreateText(graphics, text_list);

            // default width
            auto w = wrap;

            // default height
            auto h = BloodSword::TileSize;

            // padding
            auto pads = BloodSword::LargePad;

            for (auto &item : menu)
            {
                w = std::max(BloodSword::Width(item) + pads, wrap);

                h = std::max(BloodSword::Height(item) + pads, h);
            }

            auto x = (graphics.Width - w) / 2 - (character.Items.size() > limit ? (BloodSword::HalfTile + 1) : 0);

            auto y = (graphics.Height - h * (limit + 1)) / 2 - BloodSword::HalfTile + BloodSword::Pad;

            auto input = Controls::User();

            auto done = false;

            auto frame_x = x - BloodSword::HalfTile;

            auto frame_y = y - BloodSword::HalfTile + BloodSword::Pad;

            auto frame_w = w + BloodSword::HalfTile * (options > limit ? 4 : 2);

            auto frame_h = (limit * h) + (BloodSword::HalfTile * 5) + BloodSword::OddPad;

            while (!done)
            {
                auto overlay = Interface::Menu(menu, x, y, w, h, start, last, limit, Color::Background, Color::Background, Color::Active, true);

                // add frame at the back
                overlay.Elements.insert(overlay.Elements.begin(), Scene::Element(frame_x, frame_y, frame_w, frame_h, Color::Background, Color::Active, BloodSword::Border));

                auto &lastControl = overlay.Controls.back();

                auto id = lastControl.Id + 1;

                auto first = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                auto bottom = overlay.Controls[first + limit - 1].Y + h + BloodSword::LargePad;

                overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), x - BloodSword::SmallPad, bottom));

                overlay.Add(Controls::Base(Controls::Type::BACK, id, id, id, first + limit - 1, id, x - BloodSword::SmallPad, bottom, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

                Interface::ClearScrolling(overlay, input, Controls::Type::SCROLL_UP, Controls::Type::SCROLL_DOWN);

                input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

                if ((input.Selected && input.Type != Controls::Type::NONE && !input.Hold) || input.Up || input.Down)
                {
                    if (input.Type == Controls::Type::BACK)
                    {
                        done = true;

                        exit = true;
                    }
                    else if (input.Type == Controls::Type::SCROLL_UP || input.Up)
                    {
                        Interface::ScrollUp(overlay, input, Controls::Type::SCROLL_UP, options, limit, start, last);
                    }
                    else if (input.Type == Controls::Type::SCROLL_DOWN || input.Down)
                    {
                        Interface::ScrollDown(overlay, input, Controls::Type::SCROLL_DOWN, options, limit, start, last);
                    }
                    else if (input.Type == Controls::Type::CHOICE)
                    {
                        auto list = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                        auto choice = start + (input.Current - list);

                        if (choice >= 0 && choice < character.Items.size())
                        {
                            Rogue::ManageItem(graphics, background, rogue, character.Class, choice);

                            if (!Engine::IsAlive(character))
                            {
                                done = true;

                                exit = true;
                            }
                        }

                        if (character.Items.size() == 0)
                        {
                            done = true;

                            exit = true;
                        }
                        else if (character.Items.size() != options)
                        {
                            done = true;
                        }
                    }
                }
            }

            BloodSword::Free(menu);
        }
    }

    void ManageInventory(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Character::Base &character, bool blur = true)
    {
        if (!Engine::IsAlive(character))
        {
            Interface::MessageBox(graphics, background, Engine::IsDead(character), Color::Highlight);
        }
        else if (character.Items.size() > 0)
        {
            Rogue::ShowInventory(graphics, background, rogue, character);
        }
        else
        {
            Interface::MessageBox(graphics, background, "YOU DO NOT HAVE ANY ITEMS", Color::Highlight);
        }
    }

    void PartyInformation(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, int character = 0)
    {
        auto &party = rogue.Party;

        auto infow = BloodSword::OctaTile;

        auto panelw = BloodSword::OctaTile;

        auto panelh = BloodSword::PanelSize;

        auto names = Interface::GenerateNameCaptions(graphics, party);

        auto stats = Interface::PartyStats(graphics, party, infow);

        auto items = Interface::Items(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, infow);

        auto skills = Interface::Skills(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, infow);

        auto status = Interface::BasicStatus(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL);

        auto backgrounds = Interface::PartyBackgrounds(graphics, party, infow);

        auto spacing = BloodSword::TileSize + BloodSword::Pad;

        auto controlsy = (graphics.Height + panelh) / 2 - spacing - BloodSword::HugePad;

        auto controlsx = (graphics.Width - panelw) / 2 + BloodSword::Pad;

        auto panelx = (graphics.Width - panelw) / 2;

        auto panely = (graphics.Height - panelh) / 2;

        Asset::List assets = {
            Asset::Type::LEFT,
            Asset::Type::RIGHT,
            Asset::Type::CHARACTER,
            Asset::Type::INVENTORY,
            Asset::Type::SKILLS,
            Asset::Type::STATUS,
            Asset::Type::BACK};

        Controls::Collection controls = {
            Controls::Type::LEFT,
            Controls::Type::RIGHT,
            Controls::Type::ABOUT,
            Controls::Type::ITEMS,
            Controls::Type::SKILLS,
            Controls::Type::STATUS,
            Controls::Type::BACK,
        };

        std::vector<std::string> labels = {
            "PREVIOUS CHARACTER",
            "NEXT CHARACTER",
            "BACKGROUND",
            "INVENTORY",
            "SKILLS",
            "STATUS"};

        if (party.Count() == 1)
        {
            assets.erase(assets.begin(), assets.begin() + 2);

            controls.erase(controls.begin(), controls.begin() + 2);

            labels.erase(labels.begin(), labels.begin() + 2);
        }

        auto captions = Graphics::CreateText(graphics, Graphics::GenerateTextList(labels, Fonts::Caption, Color::Active, 0));

        auto display = Controls::Type::ABOUT;

        auto display_y = panely + BloodSword::TileSize + BloodSword::HalfTile + BloodSword::OddPad * 4;

        auto boxh = controlsy - display_y - BloodSword::LargePad;

        auto input = Controls::User();

        auto done = false;

        while (!done)
        {
            auto overlay = Scene::Base();

            // render panel
            overlay.Add(Scene::Element(panelx, panely, panelw, panelh, Color::Background, Color::Active, BloodSword::Border));

            // render subpanel
            overlay.Add(Scene::Element(panelx, display_y - BloodSword::Pad, panelw, boxh, Color::Background, Color::Active, BloodSword::Border));

            // render names
            overlay.VerifyAndAdd(Scene::Element(names[character], Point(panelx + BloodSword::Pad, panely + BloodSword::Pad)));

            // render stats
            overlay.VerifyAndAdd(Scene::Element(stats[character], Point(panelx + BloodSword::Pad, panely + BloodSword::Pad * 5)));

            // render information
            if (character >= 0 && character < party.Count())
            {
                switch (display)
                {
                case Controls::Type::ABOUT:

                    if (backgrounds[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(backgrounds[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;

                case Controls::Type::ITEMS:

                    if (items[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(items[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;

                case Controls::Type::SKILLS:

                    if (skills[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(skills[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;

                case Controls::Type::STATUS:

                    if (status[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(status[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;

                default:

                    if (items[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(items[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;
                }
            }

            // generate panel controls
            auto id = 0;

            for (auto control = 0; control < controls.size(); control++)
            {
                auto lt = id > 0 ? id - 1 : id;

                auto rt = controls[control] != Controls::Type::BACK ? id + 1 : id;

                overlay.VerifyAndAdd(Scene::Element(Asset::Get(assets[control]), Point(controlsx + control * spacing, controlsy)));

                overlay.Add(Controls::Base(controls[control], id, lt, rt, id, id, controlsx + control * spacing, controlsy, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

                if ((controls[control] == Controls::Type::LEFT && character == 0) || (controls[control] == Controls::Type::RIGHT && character == party.Count() - 1))
                {
                    // blur button
                    overlay.Add(Scene::Element(controlsx + control * spacing, controlsy, BloodSword::TileSize, BloodSword::TileSize, Color::Blur));
                }

                id++;
            }

            // render button captions
            if (input.Type != Controls::Type::BACK && input.Current >= 0 && input.Current < overlay.Controls.size())
            {
                auto &control = overlay.Controls[input.Current];

                // center caption
                auto center = (control.W - BloodSword::Width(captions[input.Current])) / 2;

                if ((control.X + center < (panelx + BloodSword::QuarterTile)) && input.Current == 0)
                {
                    center = 0;
                }

                overlay.VerifyAndAdd(Scene::Element(captions[input.Current], control.X + center, control.Y + control.H + BloodSword::Pad));
            }

            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

            if ((input.Selected && input.Type != Controls::Type::NONE && !input.Hold) || input.Up || input.Down)
            {
                if (input.Type == Controls::Type::BACK)
                {
                    done = true;
                }
                else if (input.Type == Controls::Type::LEFT)
                {
                    if (character > 0)
                    {
                        character--;
                    }
                    else
                    {
                        Sound::Play(Sound::Type::ERROR);
                    }
                }
                else if (input.Type == Controls::Type::RIGHT)
                {
                    if (character < party.Count() - 1)
                    {
                        character++;
                    }
                    else
                    {
                        Sound::Play(Sound::Type::ERROR);
                    }
                }
                else if (input.Type == Controls::Type::SKILLS)
                {
                    // trigger default character-specific skills
                    if (display == Controls::Type::SKILLS)
                    {
                        if (Engine::IsAlive(party[character]))
                        {
                            if (party[character].Class == Character::Class::SAGE)
                            {
                                Interface::Heal(graphics, background, party, party[character], true);
                            }
                            else if (party[character].Class == Character::Class::ENCHANTER)
                            {
                                Interface::RenderGrimoire(graphics, background, party[character]);
                            }

                            done = true;
                        }
                    }
                    else
                    {
                        display = Controls::Type::SKILLS;
                    }
                }
                else if (input.Type == Controls::Type::ITEMS)
                {
                    if (display == Controls::Type::ITEMS)
                    {
                        Rogue::ManageInventory(graphics, background, rogue, party[character], true);

                        done = true;
                    }
                    else
                    {
                        display = Controls::Type::ITEMS;
                    }
                }
                else if (input.Type == Controls::Type::ABOUT)
                {
                    display = Controls::Type::ABOUT;
                }
                else if (input.Type == Controls::Type::STATUS)
                {
                    display = Controls::Type::STATUS;
                }

                input.Selected = false;
            }
        }

        BloodSword::Free(backgrounds);

        BloodSword::Free(captions);

        BloodSword::Free(status);

        BloodSword::Free(items);

        BloodSword::Free(skills);

        BloodSword::Free(stats);

        BloodSword::Free(names);
    }

    bool Menu(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue)
    {
        auto &party = rogue.Party;

        auto quit = false;

        Asset::List assets = {
            Asset::Type::CHARACTER,
            Asset::Type::MAP,
            Asset::Type::BATTLE_ORDER,
            Asset::Type::LOAD,
            Asset::Type::SAVE,
            Asset::Type::ABOUT,
            Asset::Type::EXIT,
            Asset::Type::BACK};

        Controls::Collection controls = {
            Controls::Type::INFO,
            Controls::Type::MAP,
            Controls::Type::BATTLE_ORDER,
            Controls::Type::LOAD,
            Controls::Type::SAVE,
            Controls::Type::ABOUT,
            Controls::Type::EXIT,
            Controls::Type::BACK};

        std::vector<std::string> captions = {
            "PARTY",
            "MAP",
            "BATTLE ORDER",
            "LOAD GAME",
            "SAVE GAME",
            "HELP",
            "QUIT",
            "BACK"};

        auto values = std::vector<int>();

        for (auto i = 0; i < controls.size(); i++)
        {
            values.push_back(i);
        }

        auto message = "BloodSword: Rogue";

        auto done = false;

        while (!done)
        {
            auto selection = Interface::SelectIcons(graphics, background, message, assets, values, captions, 1, 1, Asset::Type::NONE, false, true);

            if (selection.size() == 1)
            {
                auto input = controls[selection[0]];

                if (input == Controls::Type::BACK)
                {
                    done = true;
                }
                else if (input == Controls::Type::INFO)
                {
                    Rogue::PartyInformation(graphics, background, rogue, 0);
                }
                else if (input == Controls::Type::MAP)
                {
                    Rogue::ShowMap(graphics, background, rogue);
                }
                else if (input == Controls::Type::BATTLE_ORDER)
                {
                    if (Engine::IsAlive(party))
                    {
                        if (party.Count() > 1)
                        {
                            Interface::BattleOrder(graphics, background, party);

                            done = true;
                        }
                        else
                        {
                            Interface::MessageBox(graphics, background, "YOU DO NOT HAVE ANY COMPANIONS", Color::Inactive);
                        }
                    }
                    else
                    {
                        Interface::ErrorMessage(graphics, background, Interface::MSG_OVER);
                    }
                }
                else if (input == Controls::Type::EXIT)
                {
                    if (Interface::Confirm(graphics, background, "ARE YOU SURE?", Color::Background, Color::Active, BloodSword::Border, Color::Active, true))
                    {
                        done = true;

                        quit = true;
                    }
                }
                else
                {
                    Interface::NotImplemented(graphics, background);
                }
            }
        }

        return quit;
    }

    void PlaceLoot(Rogue::Base &rogue)
    {
        auto gold = Item::Base("GOLD", Item::Type::GOLD, {}, Item::Type::NONE, Engine::Percentile.NextInt(10, 50));

        gold.Asset = Asset::Type::MONEY;

        auto &room = rogue.Rooms[1];

        auto item = Point(room.X1 + 2, room.Y1 + 2);

        auto loot = Rogue::Loot(item.X, item.Y);

        loot.Items = {gold};

        rogue.Battlepits[item].Occupant = Map::Object::ITEMS;

        rogue.Loot.push_back(loot);
    }

    void Handle(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Point point)
    {
        auto &tile = rogue.Battlepits[point];

        if (tile.IsOccupied())
        {
            if (tile.Occupant == Map::Object::ITEMS)
            {
                Rogue::ShowLoot(graphics, background, rogue, point);
            }
            else
            {
                Interface::MessageBox(graphics, background, "TILE OCCUPIED", Color::Inactive);
            }
        }
        else
        {
            Sound::Play(Sound::Type::ERROR);
        }
    }

    struct ActionResult
    {
        bool Update = false;

        bool Refresh = false;
    };

    Rogue::ActionResult Actions(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Point point)
    {
        Rogue::ActionResult result = {false, false};

        if (Battlepits::Empty(rogue.Battlepits, point))
        {
            Sound::Play(Sound::Type::ERROR);
        }
        else if (Rogue::Blocked(rogue, point))
        {
            Rogue::Handle(graphics, background, rogue, point);

            result.Update = true;

            result.Refresh = true;
        }
        else if (Rogue::Move(rogue, point))
        {
            result.Update = true;
        }

        return result;
    }

    void Game(Graphics::Base &graphics, Party::Base &party)
    {
        auto rogue = Rogue::GenerateBattlepits(50, 50, 100, 5, 7, false);

        auto method = FieldOfView::Map(Engine::ToUpper(Interface::Settings["fov"]));

        rogue.Party = party;

        if (rogue.Rooms.size() > 0)
        {
            Rogue::PlaceLoot(rogue);

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

            auto refresh_image = false;

            auto scene = Scene::Base();

            auto done = false;

            while (!done)
            {
                if (refresh_image)
                {
                    BloodSword::Free(&image);

                    image = Interface::GeneratePartyStats(graphics, rogue.Party, panel_w - BloodSword::LargePad);

                    refresh_image = false;
                }

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
                    Rogue::RenderBattlepits(scene, rogue, method);

                    regenerate_scene = false;
                }

                auto input = Input::RogueInput(graphics, {scene});

                if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
                {
                    if (input.Type == Controls::Type::MENU)
                    {
                        done = Rogue::Menu(graphics, scene, rogue);

                        regenerate_scene = true;

                        refresh_image = true;
                    }
                    else if (input.Type == Controls::Type::UP)
                    {
                        auto point = rogue.Party.Origin();

                        if (point.Y > 0)
                        {
                            point.Y--;

                            auto result = Rogue::Actions(graphics, scene, rogue, point);

                            regenerate_scene = result.Update;

                            refresh_image = result.Refresh;
                        }
                    }
                    else if (input.Type == Controls::Type::DOWN)
                    {
                        auto point = rogue.Party.Origin();

                        if (point.Y < rogue.Battlepits.Height - 1)
                        {
                            point.Y++;

                            auto result = Rogue::Actions(graphics, scene, rogue, point);

                            regenerate_scene = result.Update;

                            refresh_image = result.Refresh;
                        }
                    }
                    else if (input.Type == Controls::Type::LEFT)
                    {
                        auto point = rogue.Party.Origin();

                        if (point.X > 0)
                        {
                            point.X--;

                            auto result = Rogue::Actions(graphics, scene, rogue, point);

                            regenerate_scene = result.Update;

                            refresh_image = result.Refresh;
                        }
                    }
                    else if (input.Type == Controls::Type::RIGHT)
                    {
                        auto point = rogue.Party.Origin();

                        if (point.X < rogue.Battlepits.Width - 1)
                        {
                            point.X++;

                            auto result = Rogue::Actions(graphics, scene, rogue, point);

                            regenerate_scene = result.Update;

                            refresh_image = result.Refresh;
                        }
                    }
                    else if (input.Type == Controls::Type::MAP)
                    {
                        Rogue::ShowMap(graphics, scene, rogue);
                    }
                    else if (input.Type == Controls::Type::EXIT)
                    {
                        done = Interface::Confirm(graphics, scene, "ARE YOU SURE?", Color::Background, Color::Active, BloodSword::Border, Color::Active, true);
                    }

                    input.Selected = false;
                }
            }

            BloodSword::Free(&image);
        }
    }
}

#endif
