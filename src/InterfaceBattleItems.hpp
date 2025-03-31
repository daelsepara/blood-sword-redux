#ifndef __INTERFACE_BATTLE_ITEMS_HPP__
#define __INTERFACE_BATTLE_ITEMS_HPP__

#include "InterfaceInventory.hpp"

namespace BloodSword::Interface
{
    // (character) manage item while in battle
    bool ManageItem(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Party::Base &party, Character::Base &character, int id)
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
                    else if (!item.Has(Item::Property::REQUIRES_TARGET))
                    {
                        Interface::ItemEffects(graphics, background, character, item.Type);
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

    // (character) show inventory while in battle
    bool ShowInventory(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Party::Base &party, Character::Base &character)
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
                            update = Interface::ManageItem(graphics, background, battle, party, character, choice);
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
}

#endif
