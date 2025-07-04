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
            if (Book::IsDefined(items[id].Description) || Items::FoundDescription(items[id].Type))
            {
                if (!items[id].Revealed && Book::IsDefined(items[id].Description))
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

            if (!items[id].Has(Item::Property::CANNOT_DROP) && !character.Has(Character::Status::TASK))
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
                    else if ((input == Controls::Type::IDENTIFY) || (input == Controls::Type::ABOUT))
                    {
                        auto is_sage = (character.Class == Character::Class::SAGE);

                        auto is_revealed = items[id].Revealed;

                        if ((is_sage || is_revealed) || (party.Has(Character::Class::SAGE) && Engine::IsAlive(party[Character::Class::SAGE])))
                        {
                            if (Book::IsDefined(items[id].Description))
                            {
                                if (items[id].Revealed)
                                {
                                    Interface::ShowBookDescription(graphics, background, items[id].Description);

                                    done = true;
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
                            else if (Items::FoundDescription(items[id].Type) && input == Controls::Type::ABOUT)
                            {
                                Interface::ShowBookDescription(graphics, background, items[id].Type);
                            }
                            else
                            {
                                std::string message = "INTERNAL ERROR: " + std::string(input == Controls::Type::IDENTIFY ? "IDENTIFY" : "ABOUT");

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

                                            update.Update = true;
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
                                    update.Update = items[id].Has(Item::Property::EQUIPPED);

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
                            Interface::ItemEffects(graphics, background, party, character, items[id].Type);
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
                            if (Book::IsDefined(items[id].Description))
                            {
                                Interface::ShowBookDescription(graphics, background, items[id].Description);
                            }

                            // handles effects such as potion of healing, etc.
                            Interface::ItemEffects(graphics, background, party, character, items[id].Type);
                        }

                        update.Update = true;

                        done = true;

                        exit = true;
                    }
                    else if (input == Controls::Type::READ)
                    {
                        if (Book::IsDefined(items[id].Effects))
                        {
                            party.ChosenCharacter = character.Class;

                            update.Next = items[id].Effects;
                        }
                        else
                        {
                            // handles effects such as potion of healing, etc.
                            Interface::ItemEffects(graphics, background, party, character, items[id].Type);
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

    // (party) manage item found in current section
    bool ManageItem(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Items::Inventory &items, int id)
    {
        auto update = false;

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

    Interface::ItemResult ManageInventory(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Class character, bool blur = true)
    {
        Interface::ItemResult update;

        if (character != Character::Class::NONE)
        {
            if (party.Has(character))
            {
                if (Engine::IsAlive(party[character]))
                {
                    update = Interface::ManageInventory(graphics, background, party, party[character], blur);

                    if (!Engine::IsAlive(party[character]))
                    {
                        update.Update = true;
                    }
                }
                else
                {
                    auto message = Engine::IsDead(party[character]);

                    Interface::MessageBox(graphics, background, message, Color::Highlight);
                }
            }
            else
            {
                auto message = Engine::NotInParty(character);

                Interface::MessageBox(graphics, background, message, Color::Highlight);
            }
        }

        return update;
    }

    // show spellbook popup
    bool RenderGrimoire(Graphics::Base &graphics, Scene::Base &background, Character::Base &character)
    {
        auto update = false;

        auto done = false;

        auto input = Controls::User();

        auto pad = BloodSword::OddPad;

        auto text = Graphics::CreateText(graphics, "UNCALL FROM MIND", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto psy = Engine::Score(character, Attribute::Type::PSYCHIC_ABILITY);

        while (!done)
        {
            auto overlay = Interface::Spells(Point(0, 0), graphics.Width, graphics.Height, character, Color::Background, Color::Active, BloodSword::Border);

            auto &popup = overlay.Elements[0];

            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CALL_TO_MIND), popup.X + popup.W - (BloodSword::TileSize + BloodSword::Pad), popup.Y + BloodSword::Pad));

            if (Input::IsValid(overlay, input) && Engine::IsSpell(input.Type))
            {
                // add spell captions
                auto &control = overlay.Controls[input.Current];

                // assumes that spell controls are listed first in the pop-up window
                auto spell_id = control.Id;

                if (spell_id >= 0 && spell_id < character.Spells.size())
                {
                    auto &spell = character.Spells[spell_id];

                    if (character.HasCalledToMind(spell.Type) || Spells::In(Spells::Basic, spell.Type))
                    {
                        overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsActive[spell.Type], control.X, control.Y + control.H + pad));

                        if (!Spells::In(Spells::Basic, spell.Type))
                        {
                            overlay.VerifyAndAdd(Scene::Element(text, popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));
                        }
                    }
                    else
                    {
                        overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsInactive[spell.Type], control.X, control.Y + control.H + pad));

                        overlay.VerifyAndAdd(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CALL_TO_MIND], popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));
                    }
                }
            }

            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

            if (Input::IsValid(overlay, input) && input.Selected)
            {
                if (input.Type != Controls::Type::BACK)
                {
                    // call/uncall to/from mind
                    if (Engine::IsSpell(input.Type) && BloodSword::Has(Interface::ControlSpellMapping, input.Type))
                    {
                        auto &type = Interface::ControlSpellMapping[input.Type];

                        auto search = character.Find(type);

                        if (search != character.Spells.end())
                        {
                            auto &spellbook = *search;

                            if (character.HasCalledToMind(spellbook.Type))
                            {
                                // uncall from mind
                                character.Forget(spellbook.Type);
                            }
                            else if (!spellbook.IsBasic())
                            {
                                // call to mind
                                character.CallToMind(spellbook.Type);
                            }
                        }
                    }
                }
                else
                {
                    update = (psy != Engine::Score(character, Attribute::Type::PSYCHIC_ABILITY));

                    done = true;
                }
            }
        }

        BloodSword::Free(&text);

        return update;
    }

    // consolidated party (character) information panel
    Interface::ItemResult PartyInformation(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, int character = 0)
    {
        Interface::ItemResult result;

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

        if (party.Count() == 1 || party[character].Has(Character::Status::TASK) || party[character].Has(Character::Status::ALONE))
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

                                result.Update = true;
                            }
                            else if (party[character].Class == Character::Class::ENCHANTER)
                            {
                                result.Update = Interface::RenderGrimoire(graphics, background, party[character]);
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
                        result = Interface::ManageInventory(graphics, background, party, party[character], true);

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

        return result;
    }
}

#endif
