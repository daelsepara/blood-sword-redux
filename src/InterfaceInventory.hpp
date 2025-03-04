#ifndef __INTERFACE_INVENTORY_HPP__
#define __INTERFACE_INVENTORY_HPP__

#include "Section.hpp"
#include "Interface.hpp"

// inventory management
namespace BloodSword::Interface
{
    // check character item limit
    bool CheckItemLimit(Character::Base &character)
    {
        return !Engine::IsAlive(character) || (character.ItemLimit == Items::Unlimited) || (character.Items.size() <= character.ItemLimit);
    }

    // check party item limit
    bool CheckItemLimit(Party::Base &party)
    {
        auto ok = true;

        for (auto i = 0; i < party.Count(); i++)
        {
            ok &= Interface::CheckItemLimit(party[i]);
        }

        return ok;
    }

    void TransferItem(Items::Inventory &destination, Items::Inventory &source, int id)
    {
        if (id >= 0 && id < source.size())
        {
            destination.push_back(source[id]);

            source.erase(source.begin() + id);
        }
    }

    // DROP
    bool DropItem(Graphics::Base &graphics, Scene::Base &background, std::string action, Items::Inventory &items, int id)
    {
        auto result = false;

        if (id >= 0 && id <= items.size())
        {
            std::string message = action + " THE " + items[id].Name;

            Interface::MessageBox(graphics, background, message, Color::Active);

            items.erase(items.begin() + id);

            result = true;
        }

        return result;
    }

    // GIVE / TAKE
    bool TransferItem(Graphics::Base &graphics, Scene::Base &background, Character::Base &receiver, Items::Inventory &source, int id)
    {
        auto result = false;

        if (!Engine::IsAlive(receiver))
        {
            std::string message = Engine::IsDead(receiver);

            Interface::MessageBox(graphics, background, receiver.Name, Color::Highlight);
        }
        else if (Interface::CheckItemLimit(receiver) && (receiver.Items.size() < receiver.ItemLimit || receiver.ItemLimit == Items::Unlimited))
        {
            std::string message = receiver.Name + " TAKES THE " + source[id].Name;

            Interface::MessageBox(graphics, background, message, Color::Active);

            Interface::TransferItem(receiver.Items, source, id);

            result = true;
        }
        else
        {
            std::string message = receiver.Name + "'S INVENTORY IS FULL!";

            Interface::MessageBox(graphics, background, message, Color::Highlight);
        }

        return result;
    }

    void ManageItem(Graphics::Base &graphics, Scene::Base &background, Story::Base &story, Party::Base &party, Character::Base &character, Items::Inventory &items, int id)
    {
        auto assets = std::vector<Asset::Type>();

        auto controls = std::vector<Controls::Type>();

        auto captions = std::vector<std::string>();

        // use item
        assets.push_back(Asset::Type::USE);

        controls.push_back(Controls::Type::USE);

        captions.push_back("USE");

        if ((items[id].Has(Item::Property::WEAPON) || items[id].Has(Item::Property::ARMOUR)) && !items[id].Has(Item::Property::EQUIPPED))
        {
            // equip weapon / armour
            assets.push_back(Asset::Type::CONFIRM);

            controls.push_back(Controls::Type::EQUIP);

            captions.push_back("EQUIP");
        }
        else if ((items[id].Has(Item::Property::WEAPON) || items[id].Has(Item::Property::ARMOUR)) && items[id].Has(Item::Property::EQUIPPED))
        {
            // unequip weapon / armour
            assets.push_back(Asset::Type::CANCEL);

            controls.push_back(Controls::Type::UNEQUIP);

            captions.push_back("UNEQUIP");
        }

        if (items[id].Has(Item::Property::CONTAINER) && items[id].Contains == Item::Type::GOLD && items[id].Quantity > 0)
        {
            // money
            assets.push_back(Asset::Type::MONEY);

            controls.push_back(Controls::Type::MONEY);

            captions.push_back("GOLD");
        }

        if (Engine::Count(party) > 1)
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
                    }
                    else
                    {
                        Interface::ErrorMessage(graphics, background, Interface::MSG_ITEMS);
                    }
                }
                else if (input == Controls::Type::DROP)
                {
                    if (Interface::Confirm(graphics, background, "ARE YOU SURE?", Color::Background, Color::Highlight, BloodSword::Border, Color::Active, true))
                    {
                        std::string action = character.Name + " DROPPED";

                        Interface::DropItem(graphics, background, action, items, id);

                        done = true;
                    }
                }
                else
                {
                    Interface::NotImplemented(graphics, background);
                }
            }
        }
    }

    void ManageItem(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Items::Inventory &items, int id)
    {
        auto assets = std::vector<Asset::Type>();

        auto controls = std::vector<Controls::Type>();

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
                    }
                }
                else
                {
                    Interface::NotImplemented(graphics, background);
                }
            }
        }
    }

    void ShowInventory(Graphics::Base &graphics, Scene::Base &background, Story::Base &story, Party::Base &party, Character::Base &character)
    {
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

            auto x = (graphics.Width - w) / 2;

            auto y = (graphics.Height - h * (limit + 1)) / 2 - BloodSword::QuarterTile * 2;

            auto input = Controls::User();

            auto done = false;

            auto frame_x = x - BloodSword::HalfTile;

            auto frame_y = y - BloodSword::HalfTile;

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
                            Interface::ManageItem(graphics, background, story, party, character, character.Items, choice);
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

            Free(menu);
        }
    }

    void ShowInventory(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Items::Inventory &items)
    {
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

            auto x = (graphics.Width - w) / 2;

            auto y = (graphics.Height - h * (limit + 1)) / 2 - BloodSword::QuarterTile * 2;

            auto input = Controls::User();

            auto done = false;

            auto frame_x = x - BloodSword::HalfTile;

            auto frame_y = y - BloodSword::HalfTile;

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
                            Interface::ManageItem(graphics, background, party, items, choice);
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

            Free(menu);
        }
    }

    void ManageInventory(Graphics::Base &graphics, Scene::Base &background, Story::Base &story, Party::Base &party, Character::Base &character, bool blur = true)
    {
        if (!Engine::IsAlive(character))
        {
            Interface::MessageBox(graphics, background, Engine::IsDead(character), Color::Highlight);
        }
        else if (character.Items.size() > 0)
        {
            Interface::ShowInventory(graphics, background, story, party, character);
        }
        else
        {
            Interface::MessageBox(graphics, background, "YOU DO NOT HAVE ANY ITEMS", Color::Highlight);
        }
    }

    void ManageInventory(Graphics::Base &graphics, Scene::Base &background, Story::Base &story, Party::Base &party, bool blur = true)
    {
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
                    Interface::ManageInventory(graphics, background, story, party, party[character], blur);
                }

                if (Engine::Count(party) == 1)
                {
                    break;
                }
            }
        }
    }
}

#endif