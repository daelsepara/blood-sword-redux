#ifndef __ROGUE_ITEMS_HPP__
#define __ROGUE_ITEMS_HPP__

#include "RogueMethods.hpp"

namespace BloodSword::Rogue
{
    // take item found in location
    void TakeItem(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Items::Inventory &items, int id)
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
                    auto wrap = BloodSword::TripleTile;

                    auto text_list = Graphics::TextList();

                    for (auto &item : items)
                    {
                        text_list.push_back(Graphics::RichText(item.String(true), Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0));
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
                                    Rogue::TakeItem(graphics, background, rogue, items, choice);
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

    // use item while in battle or defer to normal use
    Rogue::Update UseItem(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Character::Class character_class, int id)
    {
        Rogue::Update update = {false, false, false};

        auto &party = rogue.Party;

        auto &character = party[character_class];

        auto in_battle = character.Is(Character::Status::IN_BATTLE);

        auto &item = character.Items[id];

        auto usable = (in_battle && item.Has(Item::Property::COMBAT)) || !in_battle;

        party.ChosenCharacter = character_class;

        // while in battle some items might require a target, otherwise defer to normal processing of effects
        if (usable)
        {
            if (item.Type == Item::Type::STEEL_SCEPTRE)
            {
                auto charged = item.Contains == Item::Type::CHARGE;

                auto has_charges = item.IsCharged(Item::Type::CHARGE, 1);

                if (charged && has_charges)
                {
                    auto target = Rogue::SelectTarget(graphics, rogue, true, false, rogue.Party.Index(character_class));

                    if (target >= 0 && target < rogue.Opponents[rogue.Enemy].Count())
                    {
                        auto &defender = rogue.Opponents[rogue.Enemy][target];

                        Interface::DamagePlayer(graphics, background, defender, 5, 0, false, true, true);

                        Interface::MessageBox(graphics, background, defender.Name + " KILLED!", defender.IsPlayer() ? Color::Highlight : Color::Active);

                        item.Remove(Item::Type::CHARGE, 1);

                        update.Scene = true;

                        update.Party = true;
                    }
                }
                else if (!has_charges)
                {
                    Interface::MessageBox(graphics, background, "NO CHARGES LEFT!", Color::Highlight);
                }
            }
            else if (item.Type == Item::Type::VELLUM_SCROLL)
            {
                // remove item
                Interface::ConsumeItem(character, id);

                auto previous_enemy = rogue.Enemy;

                auto cannot_flee = rogue.CannotFlee;

                // create enemy party (SMEABORG)
                auto enemy_id = rogue.Opponents.size();

                auto enemies = Party::Base();

                auto smeaborg = Generate::NPC("SMEABORG", Skills::Type::NONE, Skills::Type::NONE, {Skills::Type::SLOW_MURDER}, 9, 9, 8, 45, 2, 5, 0, 1000, Asset::Type::SMEABORG);

                enemies.Add(smeaborg);

                rogue.Opponents.push_back(enemies);

                // cannot flee this battle
                rogue.CannotFlee = true;

                // display Smeaborg battle text message
                std::string text = "You unfurl the scroll. It is covered with glittering scratch-like runes that seem to scuttle to and fro across the page. Wisps of green-black vapour curl out like tentacles to enfold you, cutting off your view of your surroundings. You seem to stand in an endless sea of fog. Then a strong breeze whips up around you. Strangely it blows your hair and you can feel its chill on your skin, but your clothes do not billow up at all. The fog disperses, leaving you with a grim vista. You are on a plain of sticky olive-brown mire, broken at intervals by bones and skulls that protrue from the mud.\n\nA being clad in azure armour stands four metres tall, manlike in form except for his head, which is the huge horned skull of a stag. Green eyes roll in the hollow sockets. He opens his flessless mouth, sending a wave of foetid air rolling towards you. \"Welcome to the realm of Smeaborg the Fleshless,\" he says. \"Our relationship will be a brief and bloody one.\" He hefts his giant halberd and stalks to the attack.\n\nYou cannot flee.";

                auto width = BloodSword::TileSize * 10;

                auto height = BloodSword::TileSize * 6;

                Interface::ScrollableTextBox(graphics, background, Fonts::Normal, text, width, height, Color::S(Color::Active), TTF_STYLE_NORMAL, Color::Background, Color::Active, BloodSword::Border, Color::Active, Asset::Type::SWORDTHRUST, true);

                Rogue::Battle(graphics, background, rogue, enemy_id);

                // restore previous state (if party was fighting an enemy)
                rogue.Enemy = previous_enemy;

                // restore flee status
                rogue.CannotFlee = cannot_flee;

                update.Scene = true;

                update.Party = true;
            }
            else
            {
                // add manual overrides here, otherwise default to processing
                Interface::ProcessEffects(graphics, background, party, character, id);

                update.Scene = true;

                update.Party = true;
            }
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

            if (items[id].Has(Item::Property::CONTAINER))
            {
                if (items[id].Contains == Item::Type::GOLD && items[id].Quantity > 0 && Engine::Count(party) > 1 && !in_battle)
                {
                    // money
                    assets.push_back(Asset::Type::MONEY);

                    controls.push_back(Controls::Type::MONEY);

                    std::string gold_string = std::string(Item::TypeMapping[items[id].Contains]) + ": " + std::to_string(character.Quantity(items[id].Contains));

                    captions.push_back(gold_string);
                }
                else if (items[id].Contains == Item::Type::ARROW && items[id].Quantity > 0 && Engine::Count(party) > 1 && !in_battle)
                {
                    // arrows
                    assets.push_back(Asset::Type::QUIVER);

                    controls.push_back(Controls::Type::QUIVER);

                    std::string arrow_string = std::string(Item::TypeMapping[items[id].Contains]) + ": " + std::to_string(character.Quantity(items[id].Contains));

                    captions.push_back(arrow_string);
                }
                else if (items[id].Contains == Item::Type::CHARGE && items[id].Quantity > 0 && in_battle)
                {
                    // arrows
                    assets.push_back(Asset::Type::STEEL_SCEPTRE);

                    controls.push_back(Controls::Type::USE);

                    std::string charges = std::string(Item::TypeMapping[items[id].Contains]) + ": " + std::to_string(character.Quantity(items[id].Contains));

                    captions.push_back(charges);
                }
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
                    else if (input == Controls::Type::DRINK || input == Controls::Type::EAT || input == Controls::Type::READ || input == Controls::Type::USE)
                    {
                        update = Rogue::UseItem(graphics, background, rogue, character_class, id);

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
            auto wrap = BloodSword::TripleTile;

            auto text_list = Graphics::TextList();

            for (auto &item : character.Items)
            {
                text_list.push_back(Graphics::RichText(item.String(true), Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0));
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

                            if (!Engine::IsAlive(character) || update.Scene || update.Party)
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
