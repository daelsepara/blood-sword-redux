#ifndef __ROGUE_COMBAT_HPP__
#define __ROGUE_COMBAT_HPP__

#include "RogueBase.hpp"

namespace BloodSword::Rogue
{
    void RenderCombatant(Scene::Base &scene, SDL_Texture *texture, int x, int y, Uint32 color)
    {
        scene.Add(Scene::Element(x, y, BloodSword::Width(texture), BloodSword::Height(texture), Color::Background, color, BloodSword::Border));

        scene.VerifyAndAdd(Scene::Element(texture, Point(x, y)));
    }

    void RenderCombatants(Graphics::Base &graphics, Scene::Base &scene, Party::Base &party, Textures &party_stats, Party::Base &enemies, BloodSword::Textures &enemy_stats, int stats_w, bool is_player, bool is_enemy, int id, Uint32 color)
    {
        // estimate positions from window
        auto window_h = BloodSword::QuarterTile * 18 - BloodSword::Pad;

        auto window_y = (graphics.Height - window_h) / 2;

        auto pad = BloodSword::SmallPad;

        auto enemy_offset = (graphics.Width - (enemies.Count() * stats_w + (enemies.Count() - 1) * pad)) / 2;

        auto enemy_h = BloodSword::Height(enemy_stats);

        auto enemy_y = window_y - (BloodSword::TileSize + enemy_h + BloodSword::Pad * 2);

        for (auto i = 0; i < enemies.Count(); i++)
        {
            if (!is_enemy || id != i)
            {
                auto enemy_x = enemy_offset + i * (stats_w + pad);

                Rogue::RenderCombatant(scene, enemy_stats[i], enemy_x, enemy_y, Color::Inactive);
            }
        }

        if (is_enemy)
        {
            auto enemy_x = enemy_offset + id * (stats_w + pad);

            Rogue::RenderCombatant(scene, enemy_stats[id], enemy_x, enemy_y, color);
        }

        auto party_offset = (graphics.Width - (party.Count() * stats_w + (party.Count() - 1) * pad)) / 2;

        auto party_y = window_y + window_h + BloodSword::TileSize + BloodSword::Pad;

        for (auto i = 0; i < party.Count(); i++)
        {
            if (!is_player || id != i)
            {
                auto party_x = party_offset + i * (stats_w + pad);

                Rogue::RenderCombatant(scene, party_stats[i], party_x, party_y, Color::Inactive);
            }
        }

        if (is_player)
        {
            auto party_x = party_offset + id * (stats_w + pad);

            Rogue::RenderCombatant(scene, party_stats[id], party_x, party_y, color);
        }
    }

    int SelectTarget(Graphics::Base &graphics, Scene::Base &scene, Party::Base &party, Textures &party_stats, Party::Base &enemies, BloodSword::Textures &enemy_stats, int stats_w, bool is_player, bool is_enemy, int id)
    {
        // estimate positions from window
        auto window_h = BloodSword::QuarterTile * 18 - BloodSword::Pad;

        auto window_y = (graphics.Height - window_h) / 2;

        int target = -1;

        auto texture = Graphics::CreateText(graphics, "SELECT TARGET", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto pad = BloodSword::SmallPad;

        auto max_selection = is_player ? enemies.Count() : party.Count();

        auto stats_h = is_player ? BloodSword::Height(enemy_stats) : BloodSword::Height(party_stats);

        auto offset = (graphics.Width - (max_selection * stats_w + (max_selection - 1) * pad)) / 2;

        auto target_y = is_player ? (window_y - (BloodSword::TileSize + stats_h + BloodSword::Pad * 2)) : (window_y + window_h + BloodSword::TileSize + BloodSword::Pad);

        auto selection = is_player ? Engine::First(enemies) : Engine::First(party);

        auto done = false;

        while (!done)
        {
            auto scene = Scene::Base();

            // render all combatants
            Rogue::RenderCombatants(graphics, scene, party, party_stats, enemies, enemy_stats, stats_w, is_player, is_enemy, id, Color::Active);

            // render label
            Interface::Boxed(graphics, scene, texture, Color::Background, Color::Active, BloodSword::Border);

            // calculate target texture x-coordinate
            auto target_x = offset + selection * (stats_w + pad);

            // render target
            Rogue::RenderCombatant(scene, is_player ? enemy_stats[selection] : party_stats[selection], target_x, target_y, Color::Highlight);

            // calculate pointer x-coordinate
            auto pointer_x = offset + selection * (stats_w + pad) + (stats_w - BloodSword::TileSize) / 2;

            // // calculate pointer y-coordinate, render pointer
            if (is_player)
            {
                auto pointer_y = window_y - (BloodSword::TileSize + BloodSword::Pad);

                scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::UP), pointer_x, pointer_y));
            }
            else
            {
                auto pointer_y = window_y + window_h + BloodSword::Pad;

                scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::DOWN), pointer_x, pointer_y));
            }

            auto input = Input::RogueInput(graphics, {scene});

            if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
            {
                if (input.Type == Controls::Type::LEFT)
                {
                    if (selection > 0)
                    {
                        selection--;
                    }
                }
                else if (input.Type == Controls::Type::RIGHT)
                {
                    if (selection < max_selection - 1)
                    {
                        selection++;
                    }
                }
                else if (input.Type == Controls::Type::UP || input.Type == Controls::Type::DOWN)
                {
                    Sound::Play(Sound::Type::ERROR);
                }
                else if (input.Type == Controls::Type::ACTION)
                {
                    if (selection >= 0 && selection < max_selection)
                    {
                        target = selection;

                        done = true;
                    }
                    else
                    {
                        Sound::Play(Sound::Type::ERROR);
                    }
                }
                else if (input.Type == Controls::Type::BACK)
                {
                    done = true;
                }

                input.Selected = false;
            }
        }

        BloodSword::Free(&texture);

        return target;
    }

    // use item while in battle or defer to normal use
    Rogue::Update UseItem(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Character::Class character_class, int id)
    {
        Rogue::Update update = {false, false, false};

        auto &party = rogue.Party;

        auto &character = party[character_class];

        auto in_battle = character.Is(Character::Status::IN_BATTLE);

        party.ChosenCharacter = character.Class;

        // while in battle some items might require a target, otherwise defer to normal processing of effects
        if (!in_battle)
        {
            Interface::ProcessEffects(graphics, background, party, character, id);
        }

        return update;
    }

    // manage item while in rogue mode
    Rogue::Update ManageItem(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Character::Class character_class, int id)
    {
        Rogue::Update update = {false, false, false};

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

            auto in_battle = character.Is(Character::Status::IN_BATTLE);

            auto usable = (in_battle && items[id].Has(Item::Property::COMBAT)) || !in_battle;

            if (is_equipment && !in_battle)
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

            if (!items[id].Has(Item::Property::CONTAINER) && !is_equipment && usable)
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

            if (items[id].Has(Item::Property::CONTAINER) && items[id].Contains == Item::Type::GOLD && items[id].Quantity > 0 && Engine::Count(party) > 1 && !in_battle)
            {
                // money
                assets.push_back(Asset::Type::MONEY);

                controls.push_back(Controls::Type::MONEY);

                std::string gold_string = std::string(Item::TypeMapping[items[id].Contains]) + ": " + std::to_string(character.Quantity(items[id].Contains));

                captions.push_back(gold_string);
            }

            if (items[id].Has(Item::Property::CONTAINER) && items[id].Contains == Item::Type::ARROW && items[id].Quantity > 0 && Engine::Count(party) > 1 && !in_battle)
            {
                // arrows
                assets.push_back(Asset::Type::QUIVER);

                controls.push_back(Controls::Type::QUIVER);

                std::string arrow_string = std::string(Item::TypeMapping[items[id].Contains]) + ": " + std::to_string(character.Quantity(items[id].Contains));

                captions.push_back(arrow_string);
            }

            if (Engine::Count(party) > 1 && !items[id].Has(Item::Property::CANNOT_TRADE) && !in_battle)
            {
                // trade
                assets.push_back(Asset::Type::TRADE);

                controls.push_back(Controls::Type::TRADE);

                captions.push_back("TRADE");
            }

            if (!items[id].Has(Item::Property::CANNOT_DROP) && party.Room != Room::None && !in_battle)
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
                        update = Rogue::UseItem(graphics, background, rogue, character_class, id);

                        if (in_battle)
                        {
                            update.Scene = true;

                            update.Party = true;
                        }

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

        return update;
    }

    // show player inventory
    Rogue::Update ShowInventory(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Character::Base &character)
    {
        Rogue::Update update = {false, false, false};

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
                            update = Rogue::ManageItem(graphics, background, rogue, character.Class, choice);

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

        return update;
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
