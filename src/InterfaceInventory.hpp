#ifndef __INTERFACE_INVENTORY_HPP__
#define __INTERFACE_INVENTORY_HPP__

#include "InterfaceItems.hpp"

// inventory management
namespace BloodSword::Interface
{
    // manage item while in story mode
    Interface::ItemResult ManageItem(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Base &character, Items::Inventory &items, int id)
    {
        Interface::ItemResult update;

        auto exit = false;

        while (!exit)
        {
            // create black hole
            auto ether = Items::Inventory();

            // check if it can be dropped at current location
            auto current = Story::CurrentBook.Find(party.Location);

            // select destination for dropped items
            auto &destination = (current != -1 && current >= 0 && current < Story::CurrentBook.Sections.size()) ? Story::CurrentBook.Sections[current].Items : ether;

            auto assets = Asset::List();

            auto controls = Controls::Collection();

            auto captions = std::vector<std::string>();

            auto is_equipment = (items[id].Has(Item::Property::WEAPON) || items[id].Has(Item::Property::ARMOUR));

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
            if (Book::IsDefined(items[id].Description))
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
                    assets.push_back(Asset::Type::INFO);

                    controls.push_back(Controls::Type::INFO);

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
                // money
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

            if (!items[id].Has(Item::Property::CANNOT_DROP))
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
                        if (Interface::CheckItemLimit(character))
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

                        auto is_melee = is_weapon && items[id].Is(Item::Property::PRIMARY);

                        auto weapon_type = is_weapon ? (is_melee ? Item::Property::PRIMARY : Item::Property::RANGED) : Item::Property::NONE;

                        auto equipped = is_weapon ? character.EquippedWeapon(weapon_type) : (is_armour ? character.EquippedArmour() : -1);

                        if (equipped != 1 && equipped >= 0 && equipped < items.size())
                        {
                            // un-equip
                            items[equipped].Remove(Item::Property::EQUIPPED);
                        }

                        if (!items[id].Is(Item::Property::EQUIPPED))
                        {
                            // equip
                            update.Update = items[id].Add(Item::Property::EQUIPPED);

                            done = true;
                        }
                    }
                    else if (input == Controls::Type::UNEQUIP)
                    {
                        if (items[id].Is(Item::Property::EQUIPPED))
                        {
                            update.Update = items[id].Remove(Item::Property::EQUIPPED);

                            done = true;
                        }
                    }
                    else if (input == Controls::Type::DROP)
                    {
                        if (Interface::Confirm(graphics, background, "ARE YOU SURE?", Color::Background, Color::Highlight, BloodSword::Border, Color::Active, true))
                        {
                            std::string action = character.Name + " DROPPED";

                            Interface::DropItem(graphics, background, action, destination, items, id);

                            done = true;

                            exit = true;
                        }
                    }
                    else if ((input == Controls::Type::IDENTIFY) || (input == Controls::Type::INFO))
                    {
                        auto is_sage = (character.Class == Character::Class::SAGE);

                        auto is_revealed = items[id].Revealed;

                        if ((is_sage || is_revealed) || (party.Has(Character::Class::SAGE) && Engine::IsAlive(party[Character::Class::SAGE])))
                        {
                            if (Book::IsDefined(items[id].Description))
                            {
                                if (items[id].Revealed)
                                {
                                    auto description = Story::CurrentBook.Find(items[id].Description);

                                    if (description >= 0 && description < Story::CurrentBook.Sections.size() && !Story::CurrentBook.Sections[description].Text.empty())
                                    {
                                        auto item_description = Story::CurrentBook.Sections[description].Text;

                                        auto wrap = graphics.Width - BloodSword::TileSize * 8;

                                        Interface::TextBox(graphics, background, item_description, wrap);

                                        done = true;
                                    }
                                }
                                else
                                {
                                    items[id].Revealed = true;

                                    party.ChosenCharacter = character.Class;

                                    update.Next = items[id].Description;

                                    update.Update = true;

                                    done = true;

                                    exit = true;
                                }
                            }
                            else
                            {
                                std::string message = "INTERNAL ERROR: " + std::string(input == Controls::Type::IDENTIFY ? "IDENTIFY" : "INFO");

                                Interface::InternalError(graphics, background, message);
                            }
                        }
                        else
                        {
                            Interface::ErrorMessage(graphics, background, MSG_IDENTIFY);
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

                                        update.Update = true;
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
                                if (party[other_character].Has(Item::Type::QUIVER))
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

                                            update.Update = true;
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
                                update.Update = items[id].Has(Item::Property::EQUIPPED);
                                
                                done = Interface::TransferItem(graphics, background, party[other_character], items, id);

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
                    else if (input == Controls::Type::EAT)
                    {
                        if (Book::IsDefined(items[id].Effects))
                        {
                            party.ChosenCharacter = character.Class;

                            update.Next = items[id].Effects;
                        }
                        else
                        {
                            // handles effects such as potion of healing, etc.
                            Interface::ItemEffects(graphics, background, character, items[id].Type);
                        }

                        update.Update = true;

                        done = true;

                        exit = true;
                    }
                    else if (input == Controls::Type::DRINK)
                    {
                        if (Book::IsDefined(items[id].Effects))
                        {
                            party.ChosenCharacter = character.Class;

                            update.Next = items[id].Effects;
                        }
                        else
                        {
                            // handles effects such as potion of healing, etc.
                            Interface::ItemEffects(graphics, background, character, items[id].Type);
                        }

                        update.Update = true;

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

        return update;
    }

    // (character) manage item while in battle
    bool ManageItem(Graphics::Base &graphics, Scene::Base &background, Items::Inventory &loot, Character::Base &character, int id)
    {
        auto update = false;

        auto assets = Asset::List();

        auto controls = Controls::Collection();

        auto captions = std::vector<std::string>();

        // take item
        assets.push_back(Asset::Type::USE);

        controls.push_back(Controls::Type::USE);

        captions.push_back("USE");

        // go back
        assets.push_back(Asset::Type::BACK);

        controls.push_back(Controls::Type::BACK);

        auto values = std::vector<int>();

        for (auto i = 0; i < controls.size(); i++)
        {
            values.push_back(i);
        }

        auto done = false;

        auto &item = character.Items[id];

        while (!done)
        {
            auto selection = Interface::SelectIcons(graphics, background, character.Items[id].Name.c_str(), assets, values, captions, 1, 1, Asset::Type::NONE, false);

            if (selection.size() == 1)
            {
                auto input = controls[selection[0]];

                if (input == Controls::Type::BACK)
                {
                    done = true;
                }
                else if (input == Controls::Type::USE)
                {
                    if (item.Is(Item::Property::WEAPON) || item.Is(Item::Property::ARMOUR))
                    {
                        if (item.Is(Item::Property::EQUIPPED))
                        {
                            Interface::MessageBox(graphics, background, "EQUIPMENT IS ALREADY IN USE", Color::Active);
                        }
                        else
                        {
                            Interface::MessageBox(graphics, background, "YOU CANNOT CHANGE EQUIPMENT WHILE IN BATTLE!", Color::Highlight);
                        }
                    }
                    else if (!item.Has(Item::Property::COMBAT))
                    {
                        Interface::MessageBox(graphics, background, "YOU CANNOT USE THIS IN BATTLE!", Color::Highlight);
                    }
                    else
                    {
                        Interface::NotImplemented(graphics, background);
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

    // (party) manage item found in current section
    bool ManageItem(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Items::Inventory &items, int id)
    {
        auto update = false;

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
                        done = Interface::TransferItem(graphics, background, party[character], items, id);

                        update = true;
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

    // (character) show inventory while in story mode
    Interface::ItemResult ShowInventory(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Base &character)
    {
        Interface::ItemResult update = {false, Book::Undefined};

        auto exit = false;

        while (!exit)
        {
            auto limit = std::min(4, int(character.Items.size()));

            auto start = 0;

            auto last = start + limit;

            auto options = int(character.Items.size());

            // wrap length
            auto wrap = BloodSword::TileSize * 6;

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
            auto pads = BloodSword::Pad * 2;

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

                if (input.Up)
                {
                    input.Current = Controls::Find(overlay.Controls, Controls::Type::SCROLL_UP);

                    input.Up = false;
                }
                else if (input.Down)
                {
                    input.Current = Controls::Find(overlay.Controls, Controls::Type::SCROLL_DOWN);

                    input.Down = false;
                }

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
                        if (start > 0)
                        {
                            start -= 1;

                            if (start < 0)
                            {
                                start = 0;
                            }

                            last = start + limit;

                            if (last > options)
                            {
                                last = options;
                            }

                            input.Up = true;
                        }
                    }
                    else if (input.Type == Controls::Type::SCROLL_DOWN || input.Down)
                    {
                        if (options - last > 0)
                        {
                            if (start < options - limit)
                            {
                                start += 1;
                            }

                            if (start > options - limit)
                            {
                                start = options - limit;
                            }

                            last = start + limit;

                            if (last > options)
                            {
                                last = options;
                            }

                            input.Down = true;
                        }
                    }
                    else if (input.Type == Controls::Type::CHOICE)
                    {
                        auto list = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                        auto choice = start + (input.Current - list);

                        if (choice >= 0 && choice < character.Items.size())
                        {
                            update = Interface::ManageItem(graphics, background, party, character, character.Items, choice);

                            if (!Engine::IsAlive(character) || update.Update || Book::IsDefined(update.Next))
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

    // (character) show inventory while in battle
    bool ShowInventory(Graphics::Base &graphics, Scene::Base &background, Character::Base &character, Items::Inventory &loot)
    {
        auto update = false;

        auto exit = false;

        while (!exit)
        {
            auto limit = std::min(4, int(character.Items.size()));

            auto start = 0;

            auto last = start + limit;

            auto options = int(character.Items.size());

            // wrap length
            auto wrap = BloodSword::TileSize * 6;

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
            auto pads = BloodSword::Pad * 2;

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

                if (input.Up)
                {
                    input.Current = Controls::Find(overlay.Controls, Controls::Type::SCROLL_UP);

                    input.Up = false;
                }
                else if (input.Down)
                {
                    input.Current = Controls::Find(overlay.Controls, Controls::Type::SCROLL_DOWN);

                    input.Down = false;
                }

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
                        if (start > 0)
                        {
                            start -= 1;

                            if (start < 0)
                            {
                                start = 0;
                            }

                            last = start + limit;

                            if (last > options)
                            {
                                last = options;
                            }

                            input.Up = true;
                        }
                    }
                    else if (input.Type == Controls::Type::SCROLL_DOWN || input.Down)
                    {
                        if (options - last > 0)
                        {
                            if (start < options - limit)
                            {
                                start += 1;
                            }

                            if (start > options - limit)
                            {
                                start = options - limit;
                            }

                            last = start + limit;

                            if (last > options)
                            {
                                last = options;
                            }

                            input.Down = true;
                        }
                    }
                    else if (input.Type == Controls::Type::CHOICE)
                    {
                        auto list = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                        auto choice = start + (input.Current - list);

                        if (choice >= 0 && choice < character.Items.size())
                        {
                            update = Interface::ManageItem(graphics, background, loot, character, choice);
                        }

                        // check if item list is unchanged
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

    // show inventory of a location
    bool ShowInventory(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Items::Inventory &items)
    {
        auto update = false;

        auto exit = false;

        while (!exit)
        {
            auto limit = std::min(4, int(items.size()));

            auto start = 0;

            auto last = start + limit;

            auto options = int(items.size());

            // wrap length
            auto wrap = BloodSword::TileSize * 6;

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
            auto pads = BloodSword::Pad * 2;

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

                if (input.Up)
                {
                    input.Current = Controls::Find(overlay.Controls, Controls::Type::SCROLL_UP);

                    input.Up = false;
                }
                else if (input.Down)
                {
                    input.Current = Controls::Find(overlay.Controls, Controls::Type::SCROLL_DOWN);

                    input.Down = false;
                }

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
                        if (start > 0)
                        {
                            start -= 1;

                            if (start < 0)
                            {
                                start = 0;
                            }

                            last = start + limit;

                            if (last > options)
                            {
                                last = options;
                            }

                            input.Up = true;
                        }
                    }
                    else if (input.Type == Controls::Type::SCROLL_DOWN || input.Down)
                    {
                        if (options - last > 0)
                        {
                            if (start < options - limit)
                            {
                                start += 1;
                            }

                            if (start > options - limit)
                            {
                                start = options - limit;
                            }

                            last = start + limit;

                            if (last > options)
                            {
                                last = options;
                            }

                            input.Down = true;
                        }
                    }
                    else if (input.Type == Controls::Type::CHOICE)
                    {
                        auto list = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                        auto choice = start + (input.Current - list);

                        if (choice >= 0 && choice < items.size())
                        {
                            update = Interface::ManageItem(graphics, background, party, items, choice);
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

        return update;
    }

    // (character) manage inventory while in story mode
    Interface::ItemResult ManageInventory(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Base &character, bool blur = true)
    {
        Interface::ItemResult update = {false, Book::Undefined};

        if (!Engine::IsAlive(character))
        {
            Interface::MessageBox(graphics, background, Engine::IsDead(character), Color::Highlight);
        }
        else if (character.Items.size() > 0)
        {
            update = Interface::ShowInventory(graphics, background, party, character);
        }
        else
        {
            Interface::MessageBox(graphics, background, "YOU DO NOT HAVE ANY ITEMS", Color::Highlight);
        }

        return update;
    }

    // (party) manage inventory while in story mode
    Interface::ItemResult ManageInventory(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, bool blur = true)
    {
        Interface::ItemResult update;

        if (!Engine::IsAlive(party))
        {
            Interface::ErrorMessage(graphics, background, Interface::MSG_DEAD);
        }
        else
        {
            auto character = Engine::FirstClass(party);

            while (character != Character::Class::NONE)
            {
                if (Engine::Count(party) > 1)
                {
                    character = Interface::SelectCharacter(graphics, background, party, "SELECT PLAYER TO VIEW INVENTORY", true, true, false, false, blur);
                }

                if (character != Character::Class::NONE)
                {
                    update = Interface::ManageInventory(graphics, background, party, party[character], blur);

                    if (!Engine::IsAlive(party[character]) || update.Update)
                    {
                        break;
                    }
                }

                if (Engine::Count(party) <= 1)
                {
                    break;
                }
            }
        }

        return update;
    }
}

#endif