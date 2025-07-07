#ifndef __ROGUE_BASE_HPP__
#define __ROGUE_BASE_HPP__

#include "Battlepits.hpp"
#include "InterfaceInventory.hpp"

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

    int FindOpponents(Rogue::Base &rogue, int room)
    {
        auto found = -1;

        for (auto opponent = 0; opponent < rogue.Opponents.size(); opponent++)
        {
            if (rogue.Opponents[opponent].Room == room)
            {
                found = opponent;

                break;
            }
        }

        return found;
    }

    struct Update
    {
        bool Scene = false;

        bool Party = false;

        bool Quit = false;
    };

    // attribute labels
    std::string StatsLabels()
    {
        // attribute labels
        auto label_1 = std::string("FPR      END      ");

        auto label_2 = std::string("AWR      DMG      ");

        auto label_3 = std::string("PSY      ARM      ");

        auto labels = label_1 + '\n' + label_2 + '\n' + label_3;

        return labels;
    }

    SDL_Texture *Stats(Graphics::Base &graphics, Character::Base &character, int w)
    {
        auto surface_labels = TTF_RenderUTF8_Blended_Wrapped(Fonts::Normal, Rogue::StatsLabels().c_str(), Color::S(Color::Active), 0);

        // texture
        SDL_Texture *texture = nullptr;

        if (surface_labels)
        {
            auto labels_h = std::max(BloodSword::TileSize, surface_labels->h + BloodSword::Pad);

            auto surface = Graphics::CreateSurface(w, BloodSword::TileSize + BloodSword::Pad + labels_h);

            if (surface)
            {
                // retrieve attribute values
                auto fpr = Interface::ScoreString(character, Attribute::Type::FIGHTING_PROWESS);

                auto awr = Interface::ScoreString(character, Attribute::Type::AWARENESS);

                auto psy = Interface::ScoreString(character, Attribute::Type::PSYCHIC_ABILITY);

                auto end = Interface::ScoreString(character, Attribute::Type::ENDURANCE);

                auto dmg = Interface::ScoreString(character, Attribute::Type::DAMAGE);

                auto arm = Interface::ScoreString(character, Attribute::Type::ARMOUR);

                auto four = std::string(4, ' ');

                // format attribute values
                auto stats_1 = four + fpr + std::string(5 - fpr.size(), ' ') + four + end + '\n';

                auto stats_2 = four + awr + std::string(5 - awr.size(), ' ') + four + dmg + '\n';

                auto stats_3 = four + psy + std::string(5 - psy.size(), ' ') + four + arm;

                auto stats = stats_1 + stats_2 + stats_3;

                // create and convert SDL surface to appropriate format
                auto surface_stats = TTF_RenderUTF8_Blended_Wrapped(Fonts::Normal, stats.c_str(), Color::S(Color::Highlight), 0);

                SDL_Rect stats_rect;

                stats_rect.w = surface->w;

                stats_rect.h = surface->h;

                stats_rect.x = BloodSword::Pad;

                stats_rect.y = BloodSword::TileSize + BloodSword::Pad;

                Graphics::RenderAsset(surface, surface_labels, stats_rect);

                Graphics::RenderAsset(surface, surface_stats, stats_rect);

                // add icon (blur if dead)
                auto surface_asset = Engine::Score(character, Attribute::Type::ENDURANCE) > 0 ? Asset::Surface(character.Asset) : Asset::Surface(Asset::Type::DEAD);

                stats_rect.x = (surface->w - BloodSword::TileSize) / 2;

                stats_rect.y = 0;

                Graphics::RenderAsset(surface, surface_asset, stats_rect);

                stats_rect.x = BloodSword::Pad;

                stats_rect.y = BloodSword::Pad;

                if (character.Has(Character::Status::IN_BATTLE) && character.Has(Character::Status::MELEE) && Engine::IsAlive(character))
                {
                    auto melee = Asset::Surface(Asset::Type::FIGHT);

                    if (melee)
                    {
                        Graphics::RenderAsset(surface, melee, stats_rect);

                        BloodSword::Free(&melee);
                    }
                }
                else if (character.Has(Character::Status::IN_BATTLE) && character.Has(Character::Status::RANGED) && Engine::IsAlive(character))
                {
                    SDL_Surface *ranged = nullptr;

                    if (character.Has(Skills::Type::ARCHERY))
                    {
                        ranged = Asset::Surface(Asset::Type::ARCHERY);
                    }
                    else if (character.Has(Skills::Type::SHURIKEN))
                    {
                        ranged = Asset::Surface(Asset::Type::SHURIKEN);
                    }
                    else
                    {
                        ranged = Asset::Surface(Asset::Type::SHOOT);
                    }

                    if (ranged)
                    {
                        Graphics::RenderAsset(surface, ranged, stats_rect);

                        BloodSword::Free(&ranged);
                    }
                }

                if (character.Has(Character::Status::DEFENDING) && Engine::IsAlive(character))
                {
                    stats_rect.x = surface->w - (BloodSword::TileSize + BloodSword::Pad);

                    auto defend = Asset::Surface(Asset::Type::DEFEND);

                    if (defend)
                    {
                        Graphics::RenderAsset(surface, defend, stats_rect);

                        BloodSword::Free(&defend);
                    }
                }

                // cleanup
                BloodSword::Free(&surface_labels);

                BloodSword::Free(&surface_stats);

                BloodSword::Free(&surface_asset);

                texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

                BloodSword::Free(&surface);
            }

            // cleanup
            BloodSword::Free(&surface_labels);
        }

        return texture;
    }

    // generate textures of party stats
    BloodSword::Textures Stats(Graphics::Base &graphics, Party::Base &party, int w)
    {
        auto textures = std::vector<SDL_Texture *>();

        for (auto character = 0; character < party.Count(); character++)
        {
            auto texture = Rogue::Stats(graphics, party[character], w);

            textures.push_back(texture);
        }

        return textures;
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

    // manage item found in location
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
            auto selection = Interface::SelectIcons(graphics, background, items[id].Name.c_str(), assets, values, captions, 1, 1, Asset::Type::NONE, false, true);

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

                        Interface::ProcessEffects(graphics, background, party, character, id);

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
}

#endif
