#ifndef __ROGUE_HPP__
#define __ROGUE_HPP__

#include "RogueBase.hpp"
#include "RogueBattle.hpp"

// classes and functions to enable rogue-like game mode
namespace BloodSword::Rogue
{
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
        auto radius = Engine::IsAlive(party) ? party[first].Value(Attribute::Type::AWARENESS) / 2 : 0;

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

                auto loot_id = Map::NotFound;

                auto opponent_id = Map::NotFound;

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

                    case Map::Object::ENEMIES:

                        opponent_id = Rogue::FindOpponents(rogue, Point(x, y));

                        if (opponent_id >= 0 && opponent_id < rogue.Opponents.size() && rogue.Opponents.size() > 0)
                        {
                            if (Engine::IsAlive(rogue.Opponents[opponent_id]))
                            {
                                auto first = Engine::First(rogue.Opponents[opponent_id]);

                                auto &enemy = rogue.Opponents[opponent_id][first];

                                if (Engine::IsAlive(enemy))
                                {
                                    scene.VerifyAndAdd(Scene::Element(Asset::Get(enemy.Asset), screen));
                                }
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

    // setup movement animation for enemy parties
    bool Move(Rogue::Base &rogue, int enemy, Animation::Base &movement, Point start, Point end)
    {
        auto &map = rogue.Battlepits;

        auto moving = false;

        // find a direct path to the destination
        auto path = Move::FindPath(map, start, end);

        auto closer = false;

        if (path.Points.size() == 0)
        {
            // move closer to target
            path = Move::FindPath(map, start, path.Closest);

            closer = true;
        }

        // add extra move if enemy is trying to close distance
        auto valid = Move::Count(map, path) + (closer ? 1 : 0);

        if (valid > 0)
        {
            map.Put(start, Map::Object::NONE, -1);

            auto first = path.Points.begin();

            // add destination to the count
            auto moves = std::min(valid, 1000);

            if (enemy >= 0 && enemy < rogue.Opponents.size())
            {
                auto character = Engine::First(rogue.Opponents[enemy]);

                if (character >= 0 && character < rogue.Opponents[enemy].Count())
                {
                    // setup animation
                    movement = Interface::Movement(map, Points(first, first + moves), start, rogue.Opponents[enemy][character].Asset);

                    moving = true;
                }
            }
        }

        return moving;
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

        auto controls = Controls::List();

        auto captions = std::vector<std::string>();

        // take item
        assets.push_back(Asset::Type::USE);

        controls.push_back(Controls::Type::TAKE);

        captions.push_back("TAKE");

        // go back
        assets.push_back(Asset::Type::BACK);

        controls.push_back(Controls::Type::BACK);

        auto values = std::vector<int>(controls.size());

        std::iota(values.begin(), values.end(), 0);

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

                    // padding
                    auto pads = BloodSword::LargePad;

                    // default width
                    auto w = std::max(BloodSword::Width(menu) + pads, wrap);

                    // default height
                    auto h = std::max(BloodSword::Height(menu) + pads, BloodSword::TileSize);

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
                rogue.Battlepits[point].Id = Map::NotFound;

                rogue.Battlepits[point].Occupant = Map::Object::NONE;

                rogue.Loot.erase(rogue.Loot.begin() + loot);
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

    // check if point is a good spot
    bool GoodSpot(Rogue::Base &rogue, Room::Base &room, Point point)
    {
        auto &map = rogue.Battlepits;

        auto empty = (map[point].Type == Map::Object::PASSABLE && !map[point].IsOccupied());

        auto inside = room.Inside(point) && empty;

        if (!inside)
        {
            return false;
        }

        auto left = ((point.X == room.X1 + 1) && map[Point(room.X1, point.Y)].IsBlocked()) || (point.X > room.X1 + 1);

        auto right = ((point.X == room.X2 - 2) && map[Point(room.X2 - 1, point.Y)].IsBlocked()) || (point.X < room.X2 - 2);

        auto top = ((point.Y == room.Y1 + 1) && map[Point(point.X, room.Y1)].IsBlocked()) || (point.Y > room.Y1 + 1);

        auto bottom = ((point.Y == room.Y2 - 2) && map[Point(point.X, room.Y2 - 1)].IsBlocked()) || (point.Y < room.Y2 - 2);

        return inside && top && bottom && left && right && empty;
    }

    // manage item while in rogue mode
    void ManageItem(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Character::Class character_class, int id)
    {
        auto &party = rogue.Party;

        auto &character = party[character_class];

        auto &items = character.Items;

        auto exit = false;

        auto available = Point(-1, -1);

        Items::Inventory ether = {};

        auto destination = &ether;

        // find location for room where things can be dropped
        if (party.Room != Room::None)
        {
            auto &room = rogue.Rooms[party.Room];

            // check if there is a pre-existing loot bag
            for (auto direction : Map::Directions)
            {
                auto point = party.Origin() + direction;

                auto loot_id = FindLoot(rogue, point);

                if (loot_id >= 0 && loot_id < rogue.Loot.size())
                {
                    destination = &rogue.Loot[loot_id].Items;

                    available = point;

                    break;
                }
            }

            // select a new location
            if (available.IsNone())
            {
                for (auto direction : Map::Directions)
                {
                    auto point = party.Origin() + direction;

                    if (Rogue::GoodSpot(rogue, room, point))
                    {
                        available = point;

                        break;
                    }
                }
            }
        }

        while (!exit)
        {
            auto assets = Asset::List();

            auto controls = Controls::List();

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

            if (items[id].Has(Item::Property::CONTAINER) && items[id].Contains == Item::Type::GOLD && items[id].Quantity > 0 && Engine::Count(party) > 1)
            {
                // money
                assets.push_back(Asset::Type::MONEY);

                controls.push_back(Controls::Type::MONEY);

                std::string gold_string = std::string(Item::TypeMapping[items[id].Contains]) + ": " + std::to_string(character.Quantity(items[id].Contains));

                captions.push_back(gold_string);
            }

            if (items[id].Has(Item::Property::CONTAINER) && items[id].Contains == Item::Type::ARROW && items[id].Quantity > 0 && Engine::Count(party) > 1)
            {
                // arrows
                assets.push_back(Asset::Type::QUIVER);

                controls.push_back(Controls::Type::QUIVER);

                std::string arrow_string = std::string(Item::TypeMapping[items[id].Contains]) + ": " + std::to_string(character.Quantity(items[id].Contains));

                captions.push_back(arrow_string);
            }

            if (Engine::Count(party) > 1 && !items[id].Has(Item::Property::CANNOT_TRADE))
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

            auto values = std::vector<int>(controls.size());

            std::iota(values.begin(), values.end(), 0);

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
                                items[id].Has(Item::Property::EQUIPPED);

                                done = Interface::TransferItem(graphics, background, party, party[other_character], items, id);

                                if (done)
                                {
                                    exit = true;
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

        // create loot bag in map
        if (destination->size() > 0 && !available.IsNone())
        {
            rogue.Battlepits[available].Occupant = Map::Object::ITEMS;

            auto loot = Rogue::Loot(available);

            loot.Items = *destination;

            rogue.Loot.push_back(loot);
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

            // padding
            auto pads = BloodSword::LargePad;

            // default width
            auto w = std::max(wrap, BloodSword::Width(menu) + pads);

            // default height
            auto h = std::max(BloodSword::TileSize, BloodSword::Height(menu) + pads);

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

        Controls::List controls = {
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

    bool SetBattleOrder(Graphics::Base &graphics, Scene::Base &background, Party::Base &party)
    {
        auto update = false;

        if (Engine::IsAlive(party))
        {
            if (party.Count() > 1)
            {
                Interface::BattleOrder(graphics, background, party);

                update = true;
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

        return update;
    }

    Rogue::Update Menu(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue)
    {
        Rogue::Update update = {false, false, false};

        auto &party = rogue.Party;

        Asset::List assets = {
            Asset::Type::CHARACTER,
            Asset::Type::MAP,
            Asset::Type::BATTLE_ORDER,
            Asset::Type::LOAD,
            Asset::Type::SAVE,
            Asset::Type::ABOUT,
            Asset::Type::EXIT,
            Asset::Type::BACK};

        Controls::List controls = {
            Controls::Type::PARTY,
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

        auto values = std::vector<int>(controls.size());

        std::iota(values.begin(), values.end(), 0);

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
                else if (input == Controls::Type::PARTY)
                {
                    Rogue::PartyInformation(graphics, background, rogue, 0);

                    update.Party = true;
                }
                else if (input == Controls::Type::MAP)
                {
                    Rogue::ShowMap(graphics, background, rogue);
                }
                else if (input == Controls::Type::BATTLE_ORDER)
                {
                    done = Rogue::SetBattleOrder(graphics, background, party);

                    update.Party = true;
                }
                else if (input == Controls::Type::EXIT)
                {
                    if (Interface::Confirm(graphics, background, "ARE YOU SURE?", Color::Background, Color::Active, BloodSword::Border, Color::Active, true))
                    {
                        done = true;

                        update.Quit = true;
                    }
                }
                else
                {
                    Interface::NotImplemented(graphics, background);
                }
            }
        }

        return update;
    }

    Points GoodSpots(Rogue::Base &rogue, Room::Base &room)
    {
        auto available = Points();

        for (auto y = room.Y1 + 2; y <= room.Y2 - 2; y++)
        {
            for (auto x = room.X1 + 2; x <= room.X2 - 2; x++)
            {
                auto point = Point(x, y);

                if (Rogue::GoodSpot(rogue, room, point))
                {
                    available.push_back(point);
                }
            }
        }

        return available;
    }

    void PlaceItem(Rogue::Base &rogue, Item::Base item)
    {
        // radomize room location
        auto id = Engine::Percentile.NextInt(1, rogue.Rooms.size() - 1);

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
                location = available[Engine::Percentile.NextInt(0, available.size() - 1)];

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

    Party::Base GenerateMonsters(Character::Base enemy, int min_size, int max_size, int multiplier = 1)
    {
        auto monsters = Party::Base();

        for (auto monster = 0; monster < Engine::Percentile.NextInt(min_size, max_size); monster++)
        {
            auto adjust = (3 - Engine::Random.NextInt()) * multiplier;

            auto endurance = std::max(1, enemy.Maximum(Attribute::Type::ENDURANCE) + adjust);

            enemy.Maximum(Attribute::Type::ENDURANCE, endurance);

            enemy.Value(Attribute::Type::ENDURANCE, endurance);

            monsters.Add(enemy);
        }

        return monsters;
    }

    void PlaceMonsters(Rogue::Base &rogue, int number)
    {
        auto options = std::vector<int>(rogue.Rooms.size() - 2);

        std::iota(options.begin(), options.end(), 1);

        for (auto enemies = 0; enemies < number; enemies++)
        {
            auto center = Point(-1, -1);

            auto room = Room::None;

            auto target = 0;

            // look for un-occupied room
            while (center.IsNone() || room == Room::None || rogue.Battlepits[center].IsOccupied())
            {
                target = Engine::Percentile.NextInt(0, options.size() - 1);

                room = options[target];

                center = rogue.Rooms[room].Center();
            }

            // remove the room from contention
            options.erase(options.begin() + target);

            Character::Base enemy;

            auto min_size = 3;

            auto max_size = 5;

            auto multiplier = 1;

            auto enemy_type = Engine::Percentile.NextInt(0, 100);

            if (enemy_type <= 30)
            {
                enemy = Generate::NPC("ASSASSIN", Skills::Type::NONE, Skills::Type::SHURIKEN, {Skills::Type::SHURIKEN}, 7, 6, 7, 6, 0, 1, 0, 0, Asset::Type::ASSASSIN);

                enemy.Add(Item::Base("SHURIKEN POUCH", Item::Type::LIMITED_SHURIKEN, {Item::Property::CONTAINER, Item::Property::CANNOT_DROP, Item::Property::CANNOT_TRADE, Item::Property::EQUIPPED, Item::Property::RANGED}, Item::Type::SHURIKEN, 2));

                min_size = 3;

                max_size = 5;
            }
            else if (enemy_type <= 90)
            {
                enemy = Generate::NPC("BARBARIAN", {}, 8, 5, 7, 12, 1, 1, 2, BloodSword::MaximumMoves, Asset::Type::BARBARIAN);

                min_size = 2;

                max_size = 4;
            }
            else
            {
                enemy = Generate::NPC("ADVENTURER", {}, 8, 6, 6, 22, 3, 2, 0, BloodSword::MaximumMoves, Asset::Type::CHARACTER);

                min_size = 1;

                max_size = 2;

                multiplier = 2;
            }

            auto monsters = Rogue::GenerateMonsters(enemy, min_size, max_size, multiplier);

            // place monsters in battlepits
            monsters.X = center.X;

            monsters.Y = center.Y;

            monsters.Room = room;

            rogue.Opponents.push_back(monsters);

            rogue.Battlepits[center].Occupant = Map::Object::ENEMIES;
        }
    }

    void GenerateItems(Rogue::Base &rogue, std::string name, Item::Type type, Asset::Type asset, int number, int min_quantity, int max_quantity)
    {
        for (auto items = 0; items < number; items++)
        {
            auto item = Item::Base(name.c_str(), type, {}, Item::Type::NONE, Engine::Percentile.NextInt(min_quantity, max_quantity));

            item.Asset = asset;

            Rogue::PlaceItem(rogue, item);
        }
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

    bool BattleResults(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, int &enemy)
    {
        auto done = false;

        if (!Engine::IsAlive(rogue.Party))
        {
            Interface::MessageBox(graphics, background, "YOUR ADVENTURE HAS COME TO AN END", Color::Highlight);

            done = true;
        }
        else if (!Engine::IsAlive(rogue.Opponents[enemy]))
        {
            Interface::FlashMessage(graphics, background, "YOUR PARTY IS VICTORIUS", Color::Active);

            auto &tile = rogue.Battlepits[rogue.Opponents[enemy].Origin()];

            tile.Occupant = Map::Object::NONE;

            tile.Id = Map::NotFound;

            rogue.Opponents.erase(rogue.Opponents.begin() + enemy);

            enemy = Map::NotFound;
        }

        return done;
    }

    Rogue::Update Handle(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Point point)
    {
        Rogue::Update update = {false, false, false};

        auto &tile = rogue.Battlepits[point];

        if (tile.IsOccupied())
        {
            if (tile.Occupant == Map::Object::ITEMS)
            {
                Rogue::ShowLoot(graphics, background, rogue, point);

                update.Scene = true;

                update.Party = true;
            }
            else if (tile.Occupant == Map::Object::ENEMIES)
            {
                auto enemy = Rogue::FindOpponents(rogue, point);

                if (enemy >= 0 && enemy < rogue.Opponents.size())
                {
                    Rogue::Battle(graphics, background, rogue, enemy);

                    // check results
                    Rogue::BattleResults(graphics, background, rogue, enemy);

                    update.Scene = true;

                    update.Party = true;

                    Input::Flush();
                }
            }
            else
            {
                Interface::MessageBox(graphics, background, "TILE OCCUPIED", Color::Inactive);

                update.Scene = true;

                update.Party = true;
            }
        }
        else
        {
            Sound::Play(Sound::Type::ERROR);
        }

        return update;
    }

    Rogue::Update Actions(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Point point)
    {
        Rogue::Update result = {false, false};

        if (Battlepits::Empty(rogue.Battlepits, point))
        {
            Sound::Play(Sound::Type::ERROR);
        }
        else if (Rogue::Blocked(rogue, point))
        {
            result = Rogue::Handle(graphics, background, rogue, point);
        }
        else if (Rogue::Move(rogue, point))
        {
            result.Scene = true;
        }

        return result;
    }

    Scene::Base UpdateScene(Rogue::Base &rogue, SDL_Texture *image, Point image_location, int panel_w, int panel_h, FieldOfView::Method method, bool animating)
    {
        auto scene = Scene::Base();

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
        Rogue::RenderBattlepits(scene, rogue, method, !animating);

        return scene;
    }

    // main game loop
    void Main(Graphics::Base &graphics, Rogue::Base &rogue)
    {
        // set FOV algorithm
        auto method = FieldOfView::Map(Engine::ToUpper(Interface::Settings["fov"]));

        // determine panel dimensions and locations
        auto panel_w = (graphics.Width - BloodSword::TileSize * 3) / 2;

        auto panel_h = (graphics.Height - BloodSword::TileSize * 2);

        rogue.Battlepits.ViewX = panel_w / rogue.Battlepits.TileSize;

        rogue.Battlepits.ViewY = panel_h / rogue.Battlepits.TileSize;

        auto offset_x = (panel_w - rogue.Battlepits.ViewX * rogue.Battlepits.TileSize) / 2;

        auto offset_y = (panel_h - rogue.Battlepits.ViewY * rogue.Battlepits.TileSize) / 2;

        rogue.Battlepits.DrawX = (BloodSword::TileSize * 2 + panel_w + offset_x);

        rogue.Battlepits.DrawY = (BloodSword::TileSize + offset_y);

        // generate initial party stats image
        auto image = Interface::GeneratePartyStats(graphics, rogue.Party, panel_w - BloodSword::LargePad);

        auto image_location = Point(BloodSword::TileSize + BloodSword::Pad, BloodSword::TileSize + BloodSword::Pad);

        if (image)
        {
            image_location = Point(BloodSword::TileSize + BloodSword::Pad, BloodSword::TileSize + BloodSword::Pad);
        }

        // start with a blank scene
        Rogue::Update update = {true, false, false};

        auto scene = Scene::Base();

        // animation
        auto movement = Animation::Base();

        auto animating = false;

        auto done = false;

        auto events = true;

        auto enemy = Map::NotFound;

        while (!done)
        {
            if (update.Party)
            {
                BloodSword::Free(&image);

                image = Interface::GeneratePartyStats(graphics, rogue.Party, panel_w - BloodSword::LargePad);

                update.Scene = true;

                update.Party = false;
            }

            if (update.Scene || animating)
            {
                scene = Rogue::UpdateScene(rogue, image, image_location, panel_w, panel_h, method, animating);

                update.Scene = false;
            }

            if (animating)
            {
                animating = !Graphics::Animate(graphics, scene, movement, BloodSword::FrameDelay);

                if (!animating)
                {
                    // place enemy in final destination
                    rogue.Opponents[enemy].X = movement.Current.X;

                    rogue.Opponents[enemy].Y = movement.Current.Y;

                    rogue.Battlepits.Put(movement.Current, Map::Object::ENEMIES, -1);

                    // trigger event if distance closed with party
                    if (rogue.Battlepits.Distance(movement.Current, rogue.Party.Origin()) <= 1)
                    {
                        events = true;
                    }

                    // reset animation
                    movement = Animation::Base();

                    // reset enemy
                    enemy = Map::NotFound;

                    // update battlepits
                    update.Scene = true;

                    // skip input and events processing
                    Input::Flush();

                    continue;
                }
            }

            // enemy movement, ranged and magic attacks
            if (Engine::IsAlive(rogue.Party) && events && !animating && rogue.Party.Room != Room::None && rogue.Rooms[rogue.Party.Room].Inside(rogue.Party.Origin()))
            {
                enemy = Rogue::FindOpponents(rogue, rogue.Party.Room);

                if (enemy >= 0 && enemy < rogue.Opponents.size())
                {
                    auto distance = rogue.Battlepits.Distance(rogue.Party.Origin(), rogue.Opponents[enemy].Origin());

                    if (distance > 1)
                    {
                        // move or shoot at party
                        animating = Rogue::Move(rogue, enemy, movement, rogue.Opponents[enemy].Origin(), rogue.Party.Origin());
                    }
                    else
                    {
                        // update scene
                        Rogue::UpdateScene(rogue, image, image_location, panel_w, panel_h, method, true);

                        // flash a message
                        Interface::FlashMessage(graphics, scene, "PARTY ATTACKED!", Color::Background, Color::Highlight, BloodSword::Border, BloodSword::OneSecond);

                        // commence battle
                        Rogue::Battle(graphics, scene, rogue, enemy);

                        // check results
                        done = Rogue::BattleResults(graphics, scene, rogue, enemy);

                        update.Scene = true;

                        update.Party = true;

                        Input::Flush();
                    }
                }

                events = false;
            }

            if (!animating)
            {
                auto input = Input::RogueInput(graphics, {scene});

                auto prev = rogue.Party.Origin();

                if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
                {
                    auto point = rogue.Party.Origin();

                    if (input.Type == Controls::Type::MENU)
                    {
                        update = Rogue::Menu(graphics, scene, rogue);

                        done = update.Quit;
                    }
                    else if (input.Type == Controls::Type::UP)
                    {
                        if (point.Y > 0)
                        {
                            point.Y--;

                            update = Rogue::Actions(graphics, scene, rogue, point);
                        }
                    }
                    else if (input.Type == Controls::Type::DOWN)
                    {
                        if (point.Y < rogue.Battlepits.Height - 1)
                        {
                            point.Y++;

                            update = Rogue::Actions(graphics, scene, rogue, point);
                        }
                    }
                    else if (input.Type == Controls::Type::LEFT)
                    {
                        if (point.X > 0)
                        {
                            point.X--;

                            update = Rogue::Actions(graphics, scene, rogue, point);
                        }
                    }
                    else if (input.Type == Controls::Type::RIGHT)
                    {
                        if (point.X < rogue.Battlepits.Width - 1)
                        {
                            point.X++;

                            update = Rogue::Actions(graphics, scene, rogue, point);
                        }
                    }
                    else if (input.Type == Controls::Type::MAP)
                    {
                        Rogue::ShowMap(graphics, scene, rogue);
                    }
                    else if (input.Type == Controls::Type::PARTY)
                    {
                        auto first = std::max(0, Engine::First(rogue.Party));

                        Rogue::PartyInformation(graphics, scene, rogue, first);

                        update.Party = true;
                    }
                    else if (input.Type == Controls::Type::BATTLE_ORDER)
                    {
                        update.Party = Rogue::SetBattleOrder(graphics, scene, rogue.Party);
                    }
                    else if (input.Type == Controls::Type::EXIT)
                    {
                        done = Interface::Confirm(graphics, scene, "ARE YOU SURE?", Color::Background, Color::Active, BloodSword::Border, Color::Active, true);
                    }

                    // trigger event on movement
                    if (prev != rogue.Party.Origin())
                    {
                        events = true;
                    }

                    input.Selected = false;
                }
            }
        }

        BloodSword::Free(&image);
    }

    void Game(Graphics::Base &graphics)
    {
        // set default control to the first
        Controls::Default = 0;

        auto rogue = Rogue::GenerateBattlepits(50, 100, 100, 5, 7, false);

        // create party
        rogue.Party = Interface::CreateParty(graphics, {8, 4, 3, 2}, false);

        if (rogue.Rooms.size() > 0 && rogue.Party.Count() > 0)
        {
            // 50% rooms has monsters
            Rogue::PlaceMonsters(rogue, rogue.Rooms.size() / 2);

            // 25% rooms has gold loot
            Rogue::PlaceGold(rogue, rogue.Rooms.size() / 4, 10, 50);

            // 25% rooms has arrows loot if TRICKSTER or SAGE present in party
            if (rogue.Party.Has(Character::Class::TRICKSTER) || rogue.Party.Has(Character::Class::SAGE))
            {
                Rogue::PlaceArrows(rogue, rogue.Rooms.size() / 4, 4, 20);
            }

            // place party at the center of the starting room
            auto center = rogue.Rooms[0].Center();

            rogue.Party.X = center.X;

            rogue.Party.Y = center.Y;

            // run main game loop
            Rogue::Main(graphics, rogue);
        }
    }
}

#endif
