#ifndef __INTERFACE_STORY_HPP__
#define __INTERFACE_STORY_HPP__

#include "Choice.hpp"
#include "Conditions.hpp"
#include "Section.hpp"
#include "Interface.hpp"
#include "InterfaceBattle.hpp"
#include "InterfaceInventory.hpp"

namespace BloodSword::Interface
{
    typedef std::vector<Choice::Base> Choices;

    Conditions::Evaluation ProcessItemEvents(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Conditions::Base &condition)
    {
        auto result = false;

        auto text = std::string();

        // TODO: Figure out a way to move this into Conditions namespace
        if (condition.Type == Conditions::Type::ITEM_EFFECT)
        {
            // variables
            // 0 - item (type)
            if (condition.Variables.size() > 0)
            {
                auto item = Item::Map(condition.Variables[0]);

                if (party.ChosenCharacter != Character::Class::NONE && party.Has(party.ChosenCharacter) && Engine::IsAlive(party[party.ChosenCharacter]) && item != Item::Type::NONE)
                {
                    auto &chosen = party[party.ChosenCharacter];

                    if (chosen.Has(item))
                    {
                        Interface::ItemEffects(graphics, background, chosen, item);

                        result = true;
                    }
                    else
                    {
                        text = Engine::NoItem(item);
                    }
                }
                else if (party.ChosenCharacter == Character::Class::NONE || item == Item::Type::NONE)
                {
                    Conditions::InternalError(graphics, background, condition.Type);
                }
                else if (!party.Has(party.ChosenCharacter))
                {
                    text = Engine::NotInParty(party.ChosenCharacter);
                }
                else if (party.Has(party.ChosenCharacter) && !Engine::IsAlive(party[party.ChosenCharacter]))
                {
                    text = Engine::IsDead(party[party.ChosenCharacter]);
                }
            }
        }

        Conditions::Log(condition, result, false, text, {Book::Number::NONE, -1});

        return Conditions::Evaluation(result, text);
    }

    Book::Location RenderChoices(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Choices &choices, bool after_battle = false)
    {
        Book::Location next = Book::Undefined;

        auto limit = std::min(4, int(choices.size()));

        auto start = 0;

        auto last = start + limit;

        auto options = int(choices.size());

        // wrap length
        auto wrap = BloodSword::TileSize * 6;

        auto text_list = Graphics::TextList();

        for (auto &choice : choices)
        {
            text_list.push_back(Graphics::RichText(choice.Text, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, wrap));
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

        auto y = (graphics.Height - h * (limit + (after_battle ? 0 : 1))) / 2 - BloodSword::QuarterTile * (after_battle ? 1 : 2);

        auto input = Controls::User();

        auto done = false;

        auto frame_x = x - BloodSword::HalfTile;

        auto frame_y = y - BloodSword::HalfTile;

        auto frame_w = w + BloodSword::HalfTile * (options > limit ? 4 : 2);

        auto frame_h = (limit * h) + (BloodSword::HalfTile * (!after_battle ? 5 : 3)) + BloodSword::OddPad;

        while (!done)
        {
            auto overlay = Interface::Menu(menu, x, y, w, h, start, last, limit, Color::Background, Color::Background, Color::Active, true);

            // add frame at the back
            overlay.Elements.insert(overlay.Elements.begin(), Scene::Element(frame_x, frame_y, frame_w, frame_h, Color::Background, Color::Active, BloodSword::Border));

            auto &lastControl = overlay.Controls.back();

            auto id = lastControl.Id + 1;

            auto first = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

            auto bottom = overlay.Controls[first + limit - 1].Y + h + BloodSword::LargePad;

            if (!after_battle)
            {
                overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), x - BloodSword::SmallPad, bottom));

                overlay.Add(Controls::Base(Controls::Type::BACK, id, id, id, first + limit - 1, id, x - BloodSword::SmallPad, bottom, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
            }

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

                    if (choice >= 0 && choice < choices.size())
                    {
                        auto eval = Conditions::Evaluation();

                        // TODO: Figure out a way to move this into Conditions namespace
                        if (choices[choice].Condition.Type == Conditions::Type::ITEM_EFFECT)
                        {
                            eval = Interface::ProcessItemEvents(graphics, background, party, choices[choice].Condition);
                        }
                        else
                        {
                            eval = Conditions::Process(graphics, background, party, choices[choice].Condition);
                        }

                        if (eval.Failed)
                        {
                            // fails test
                            Interface::MessageBox(graphics, background, Graphics::RichText(eval.Text.c_str(), Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Highlight, BloodSword::Border, Color::Active, true);

                            // jumps to next destination
                            next = eval.Location;

                            done = true;
                        }
                        else if (!eval.Result)
                        {
                            Interface::MessageBox(graphics, background, eval.Text, Color::Highlight);
                        }
                        else
                        {
                            if (!eval.Text.empty())
                            {
                                Interface::MessageBox(graphics, background, eval.Text, Color::Inactive);
                            }

                            next = choices[choice].Condition.Location;

                            done = true;
                        }
                    }
                }
            }
        }

        BloodSword::Free(menu);

        return next;
    }

    // process background events
    Book::Location ProcessBackground(Graphics::Base &graphics, Scene::Base &background, Party::Base &party)
    {
        auto current = Story::CurrentBook.Find(party.Location);

        auto &section = (current >= 0 && current < Story::CurrentBook.Sections.size()) ? Story::CurrentBook.Sections[current] : Story::CurrentBook.Sections[0];

        Book::Location next = Book::Undefined;

        if (section.Background.size() > 0)
        {
            std::cerr << "[BACKGROUND] " << Book::String(section.Location) << std::endl;

            for (auto &condition : section.Background)
            {
                auto eval = Conditions::Process(graphics, background, party, condition);

                if (eval.Failed)
                {
                    next = eval.Location;

                    break;
                }
                else if (eval.Result)
                {
                    next = condition.Location;

                    break;
                }
            }
        }

        return next;
    }

    // process real-time events
    std::vector<Conditions::Evaluation> ProcessEvents(Graphics::Base &graphics, Scene::Base &background, Party::Base &party)
    {
        auto current = Story::CurrentBook.Find(party.Location);

        auto &section = (current >= 0 && current < Story::CurrentBook.Sections.size()) ? Story::CurrentBook.Sections[current] : Story::CurrentBook.Sections[0];

        auto results = std::vector<Conditions::Evaluation>();

        if (section.Events.size() > 0)
        {
            std::cerr << "[EVENT] " << Book::String(section.Location) << std::endl;

            for (auto &condition : section.Events)
            {
                if (Engine::IsAlive(party))
                {
                    auto eval = Conditions::Evaluation();

                    // TODO: Figure out a way to move this into Conditions namespace
                    if (condition.Type == Conditions::Type::ITEM_EFFECT)
                    {
                        // process item effects first
                        eval = Interface::ProcessItemEvents(graphics, background, party, condition);
                    }
                    else
                    {
                        eval = Conditions::Process(graphics, background, party, condition);
                    }

                    results.push_back(eval);
                }
            }
        }

        return results;
    }

    // get next location
    Book::Location NextSection(Graphics::Base &graphics, Scene::Base &background, Party::Base &party)
    {
        auto current = Story::CurrentBook.Find(party.Location);

        auto &section = (current >= 0 && current < Story::CurrentBook.Sections.size()) ? Story::CurrentBook.Sections[current] : Story::CurrentBook.Sections[0];

        Book::Location next = Book::Undefined;

        auto after_battle = false;

        auto exit_battle = false;

        // fight battle
        if (section.Battle.IsDefined())
        {
            std::cerr << "[BATTLE] " << Book::String(section.Location) << std::endl;

            auto result = Interface::RenderBattle(graphics, section.Battle, party);

            if (result != Battle::Result::NONE)
            {
                party.LastBattle = result;

                after_battle = true;

                if (!Engine::IsAlive(party))
                {
                    auto message = party.Count() > 1 ? Interface::Text[Interface::MSG_DEAD] : Interface::Text[Interface::MSG_DIED];

                    Interface::MessageBox(graphics, background, message, Color::Background, Color::Highlight, 4, Color::Highlight, true);
                }
            }
            else
            {
                exit_battle = true;
            }
        }

        if (Engine::IsAlive(party) && !exit_battle)
        {
            if (section.Next.size() > 0)
            {
                std::cerr << "[NEXT] " << Book::String(section.Location) << std::endl;

                // process through each condition
                for (auto &condition : section.Next)
                {
                    auto eval = Conditions::Evaluation();

                    // TODO: Figure out a way to move this into Conditions namespace
                    if (condition.Type == Conditions::Type::ITEM_EFFECT)
                    {
                        eval = Interface::ProcessItemEvents(graphics, background, party, condition);
                    }
                    else
                    {
                        eval = Conditions::Process(graphics, background, party, condition);
                    }

                    // handle 'NEXT' situations that behave like events
                    if (eval.Result && Book::IsDefined(condition.Location))
                    {
                        if (!eval.Text.empty())
                        {
                            Interface::MessageBox(graphics, background, eval.Text, Color::Active);
                        }

                        next = condition.Location;

                        break;
                    }
                }
            }

            if ((!Book::IsDefined(next) && section.Choices.size() > 0))
            {
                // process through any choices
                while (true)
                {
                    std::cerr << "[CHOICE] " << Book::String(section.Location) << std::endl;

                    next = Interface::RenderChoices(graphics, background, party, section.Choices, after_battle);

                    if (!after_battle || Book::IsDefined(next))
                    {
                        break;
                    }
                }
            }
        }

        return next;
    }

    void StoryControls(Party::Base &party, Scene::Base &overlay, Point buttons, Point scroll_top, Point scroll_bot, bool arrow_up, bool arrow_dn)
    {
        auto current = Story::CurrentBook.Find(party.Location);

        auto &section = (current >= 0 && current < Story::CurrentBook.Sections.size()) ? Story::CurrentBook.Sections[current] : Story::CurrentBook.Sections[0];

        auto elements = overlay.Elements.size();

        auto controls = overlay.Controls.size();

        auto button_spacing = BloodSword::TileSize + BloodSword::Pad;

        auto num_buttons = 0;

        auto id = 0;

        auto heal = party.Has(Character::Class::SAGE);

        auto spells = party.Has(Character::Class::ENCHANTER);

        auto items = section.Items.size() > 0;

        // add button textures
        if (section.Battle.IsDefined())
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::FIGHT), buttons.X, buttons.Y));
        }
        else
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::RIGHT), buttons.X, buttons.Y));
        }

        // next button hotspot
        overlay.Add(Controls::Base(Controls::Type::NEXT, id, id, id + 1, id, id, buttons.X, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

        num_buttons++;

        id++;

        if (items)
        {
            // items in the area
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::ITEMS), buttons.X + num_buttons * button_spacing, buttons.Y));

            overlay.Add(Controls::Base(Controls::Type::ITEMS, id, id - 1, id + 1, id, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;
        }

        if (heal)
        {
            // heal button icon and hotspot
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::HEAL), buttons.X + num_buttons * button_spacing, buttons.Y));

            overlay.Add(Controls::Base(Controls::Type::HEAL, id, id - 1, id + 1, id, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;
        }

        if (spells)
        {
            // spells button icon and hotspot
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::SPELLS), buttons.X + num_buttons * button_spacing, buttons.Y));

            overlay.Add(Controls::Base(Controls::Type::SPELLS, id, id - 1, id + 1, id, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;
        }

        // inventory management and hotpost
        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::INVENTORY), buttons.X + num_buttons * button_spacing, buttons.Y));

        overlay.Add(Controls::Base(Controls::Type::INVENTORY, id, id - 1, id + 1, id, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

        num_buttons++;

        id++;

        // game functions
        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::GAME), buttons.X + num_buttons * button_spacing, buttons.Y));

        overlay.Add(Controls::Base(Controls::Type::GAME, id, id - 1, id + 1, id, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

        num_buttons++;

        id++;

        // exit button icon
        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::EXIT), buttons.X + num_buttons * button_spacing, buttons.Y));

        // scroll button icons
        if (arrow_up)
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::UP), scroll_top.X, scroll_top.Y));
        }

        if (arrow_dn)
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::DOWN), scroll_bot.X, scroll_bot.Y));
        }

        if (arrow_up && arrow_dn)
        {
            // exit hotspot when both scroll buttons are present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id + 1, id + 2, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            // scroll buttons hotspots
            id++;

            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id + 1, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id + 1, id - 1, id + 1, id, id + 1, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_up)
        {
            // exit hotspot when scroll up is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id + 1, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll up hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_dn)
        {
            // exit hotspot when scroll down is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id, id + 1, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll down hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id, id - 1, id, id, id, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else
        {
            // exit hotspot when there are no scroll buttons
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id, id, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;
        }

        auto adjust = (num_buttons * button_spacing) / 2 + BloodSword::HalfTile - BloodSword::Pixel;

        // adjust icon positions at bottom
        for (auto i = 0; i < num_buttons; i++)
        {
            overlay.Elements[elements + i].X -= adjust;

            overlay.Controls[controls + i].X -= adjust;
        }
    }

    void ItemControls(Party::Base &party, Scene::Base &overlay, Point buttons, Point scroll_top, Point scroll_bot, bool arrow_up, bool arrow_dn)
    {
        auto current = Story::CurrentBook.Find(party.Location);

        auto &section = (current >= 0 && current < Story::CurrentBook.Sections.size()) ? Story::CurrentBook.Sections[current] : Story::CurrentBook.Sections[0];

        auto elements = overlay.Elements.size();

        auto controls = overlay.Controls.size();

        auto button_spacing = BloodSword::TileSize + BloodSword::Pad;

        auto num_buttons = 0;

        auto id = 0;

        // add button textures
        if (section.Battle.IsDefined())
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::FIGHT), buttons.X, buttons.Y));
        }
        else
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::RIGHT), buttons.X, buttons.Y));
        }

        // next button hotspot
        overlay.Add(Controls::Base(Controls::Type::NEXT, id, id, id + 1, id, id, buttons.X, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

        num_buttons++;

        id++;

        // game functions
        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::GAME), buttons.X + num_buttons * button_spacing, buttons.Y));

        overlay.Add(Controls::Base(Controls::Type::GAME, id, id - 1, id + 1, id, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

        num_buttons++;

        id++;

        // exit button icon
        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::EXIT), buttons.X + num_buttons * button_spacing, buttons.Y));

        // scroll button icons
        if (arrow_up)
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::UP), scroll_top.X, scroll_top.Y));
        }

        if (arrow_dn)
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::DOWN), scroll_bot.X, scroll_bot.Y));
        }

        if (arrow_up && arrow_dn)
        {
            // exit hotspot when both scroll buttons are present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id + 1, id + 2, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            // scroll buttons hotspots
            id++;

            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id + 1, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id + 1, id - 1, id + 1, id, id + 1, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_up)
        {
            // exit hotspot when scroll up is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id + 1, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll up hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_dn)
        {
            // exit hotspot when scroll down is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id, id + 1, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll down hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id, id - 1, id, id, id, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else
        {
            // exit hotspot when there are no scroll buttons
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id, id, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;
        }

        auto adjust = (num_buttons * button_spacing) / 2 + BloodSword::HalfTile - BloodSword::Pixel;

        // adjust icon positions at bottom
        for (auto i = 0; i < num_buttons; i++)
        {
            overlay.Elements[elements + i].X -= adjust;

            overlay.Controls[controls + i].X -= adjust;
        }
    }

    void EndingControls(Party::Base &party, Scene::Base &overlay, Point buttons, Point scroll_top, Point scroll_bot, bool arrow_up, bool arrow_dn)
    {
        auto elements = overlay.Elements.size();

        auto controls = overlay.Controls.size();

        auto button_spacing = BloodSword::TileSize + BloodSword::Pad;

        auto num_buttons = 0;

        auto id = 0;

        // game functions
        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::GAME), buttons.X, buttons.Y));

        overlay.Add(Controls::Base(Controls::Type::GAME, id, id, id + 1, id, id, buttons.X, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

        num_buttons++;

        id++;

        // exit button icon
        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::EXIT), buttons.X + button_spacing, buttons.Y));

        // scroll button icons
        if (arrow_up)
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::UP), scroll_top.X, scroll_top.Y));
        }

        if (arrow_dn)
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::DOWN), scroll_bot.X, scroll_bot.Y));
        }

        if (arrow_up && arrow_dn)
        {
            // exit hotspot when both scroll buttons are present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id + 1, id + 2, buttons.X + button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            // scroll buttons hotspots
            id++;

            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id + 1, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id + 1, id - 1, id + 1, id, id + 1, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_up)
        {
            // exit hotspot when scroll up is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id, id + 1, id + 1, id, buttons.X + button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll up hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_dn)
        {
            // exit hotspot when scroll down is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id, id + 1, buttons.X + button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll down hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id, id - 1, id, id, id, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else
        {
            // exit hotspot when there are no scroll buttons
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id, id, id, buttons.X + BloodSword::TileSize + BloodSword::Pad, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;
        }

        auto adjust = (num_buttons * button_spacing) / 2 + BloodSword::HalfTile - BloodSword::Pixel;

        // adjust icon positions at bottom
        for (auto i = 0; i < num_buttons; i++)
        {
            overlay.Elements[elements + i].X -= adjust;

            overlay.Controls[controls + i].X -= adjust;
        }
    }

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
                    auto ptr = Interface::ControlSpellMapping.find(input.Type);

                    if (Engine::IsSpell(input.Type) && ptr != Interface::ControlSpellMapping.end())
                    {
                        auto &type = ptr->second;

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

    Book::Location RenderSection(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Party::Base &saved_party, std::string &text)
    {
        auto current = Story::CurrentBook.Find(party.Location);

        auto &section = (current >= 0 && current < Story::CurrentBook.Sections.size()) ? Story::CurrentBook.Sections[current] : Story::CurrentBook.Sections[0];

        Book::Location next = Book::Undefined;

        // texture for section text / default text
        SDL_Texture *texture = nullptr;

        // texture for left panel (either party stats, or specific images)
        SDL_Texture *image = nullptr;

        // total size of all dividers
        auto space = BloodSword::TileSize * 3 + BloodSword::HugePad;

        // includes tile-sized borders on left, right, and middle (divider)
        auto panel_w = (graphics.Width - BloodSword::TileSize * 2 - space) / 2;

        // includes tile-sized top, bottom borders and one row of buttons
        auto panel_h = (graphics.Height - space);

        // maximum size of viewable text
        auto text_w = panel_w - BloodSword::LargePad;

        auto text_h = panel_h - BloodSword::LargePad;

        // location where left-most panel is rendered
        auto origin = Point((graphics.Width - (panel_w * 2 + BloodSword::TileSize + BloodSword::LargePad)) / 2, BloodSword::TileSize + BloodSword::LargePad);

        auto origin_text = origin + Point(panel_w + BloodSword::TileSize + BloodSword::Pad * 3, BloodSword::Pad);

        if (text.length() > 0)
        {
            texture = Graphics::CreateText(graphics, text.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, text_w);
        }
        else
        {
            texture = Graphics::CreateText(graphics, "You decide what to do next.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, text_w);
        }

        // texture dimensions
        auto texture_w = 0;

        auto texture_h = 0;

        // get actual dimension
        BloodSword::Size(texture, &texture_w, &texture_h);

        if (section.ImageAsset.length() > 0)
        {
            image = Asset::Create(graphics.Renderer, section.ImageAsset.c_str());
        }
        else
        {
            image = Interface::GeneratePartyStats(graphics, party, panel_w - BloodSword::LargePad, panel_h - BloodSword::LargePad);
        }

        // location of scroll arrows
        auto scroll_top = origin + Point(panel_w * 2 + BloodSword::TileSize + BloodSword::SmallPad * 5, -BloodSword::MidPad);

        auto scroll_bot = origin + Point(panel_w * 2 + BloodSword::TileSize + BloodSword::SmallPad * 5, panel_h - BloodSword::TileSize + BloodSword::MidPad);

        auto scroll_speed = BloodSword::ScrollSpeed;

        auto scroll_up = false;

        auto scroll_dn = false;

        // location of buttons
        auto buttons = origin + Point(panel_w + BloodSword::TileSize + BloodSword::MidPad + BloodSword::SmallPad, panel_h + BloodSword::Pad);

        // text offset
        auto offset = 0;

        auto input = Controls::User();

        auto done = false;

        while (!done)
        {
            auto overlay = Scene::Base();

            // left panel border
            overlay.Add(Scene::Element(origin.X, origin.Y, panel_w, panel_h, Color::Background, Color::Active, BloodSword::Border));

            if (image)
            {
                // add left panel
                overlay.VerifyAndAdd(Scene::Element(image, origin + Point(BloodSword::Pad, BloodSword::Pad)));
            }

            // text panel border
            overlay.Add(Scene::Element(origin.X + panel_w + BloodSword::TileSize + BloodSword::LargePad, origin.Y, panel_w, panel_h, Color::Background, Color::Active, BloodSword::Border));

            // text panel
            if (texture)
            {
                // text
                overlay.VerifyAndAdd(Scene::Element(texture, origin_text.X, origin_text.Y, text_h, offset));
            }

            auto arrow_up = offset > 0;

            auto arrow_dn = text_h < texture_h && offset < (texture_h - text_h);

            // add story controls
            if (section.Has(Feature::Type::ENDING) || !Engine::IsAlive(party))
            {
                Interface::EndingControls(party, overlay, buttons, scroll_top, scroll_bot, arrow_up, arrow_dn);
            }
            else if (section.Has(Feature::Type::ITEM_EFFECT))
            {
                Interface::ItemControls(party, overlay, buttons, scroll_top, scroll_bot, arrow_up, arrow_dn);
            }
            else
            {
                Interface::StoryControls(party, overlay, buttons, scroll_top, scroll_bot, arrow_up, arrow_dn);
            }

            if (scroll_up)
            {
                input.Current = Controls::Find(overlay.Controls, Controls::Type::SCROLL_UP);

                scroll_up = false;
            }
            else if (scroll_dn)
            {
                input.Current = Controls::Find(overlay.Controls, Controls::Type::SCROLL_DOWN);

                scroll_dn = false;
            }

            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

            if ((input.Selected && input.Type != Controls::Type::NONE && !input.Hold) || input.Up || input.Down)
            {
                if (input.Type == Controls::Type::NEXT)
                {
                    if (Engine::IsAlive(party))
                    {
                        while (!Interface::CheckItemLimit(party))
                        {
                            Interface::ErrorMessage(graphics, overlay, Interface::MSG_ITEMS);

                            auto update = Interface::ManageInventory(graphics, overlay, party, true);

                            if (update.Update && section.ImageAsset.empty())
                            {
                                BloodSword::Free(&image);

                                // regenerate party stats
                                image = Interface::GeneratePartyStats(graphics, party, panel_w - BloodSword::LargePad, panel_h - BloodSword::LargePad);
                            }
                        }

                        next = Interface::NextSection(graphics, overlay, party);

                        if (Engine::IsAlive(party) && Book::IsDefined(next))
                        {
                            done = true;
                        }
                        else if (!Engine::IsAlive(party))
                        {
                            if (section.ImageAsset.empty())
                            {
                                BloodSword::Free(&image);

                                // regenerate party stats
                                image = Interface::GeneratePartyStats(graphics, party, panel_w - BloodSword::LargePad, panel_h - BloodSword::LargePad);
                            }
                        }
                    }
                    else
                    {
                        Interface::ErrorMessage(graphics, overlay, Interface::MSG_OVER);
                    }
                }
                else if (input.Type == Controls::Type::SPELLS)
                {
                    if (party.Has(Character::Class::ENCHANTER))
                    {
                        auto update = Interface::RenderGrimoire(graphics, overlay, party[Character::Class::ENCHANTER]);

                        if (update && section.ImageAsset.empty())
                        {
                            BloodSword::Free(&image);

                            // regenerate party stats
                            image = Interface::GeneratePartyStats(graphics, party, panel_w - BloodSword::LargePad, panel_h - BloodSword::LargePad);
                        }
                    }

                    input.Selected = false;
                }
                else if (input.Type == Controls::Type::HEAL)
                {
                    if (party.Has(Character::Class::SAGE))
                    {
                        Interface::Heal(graphics, overlay, party, party[Character::Class::SAGE], true);

                        input.Current = input.Current = Controls::Find(overlay.Controls, Controls::Type::HEAL);

                        input.Selected = false;
                    }
                }
                else if (input.Type == Controls::Type::ITEMS)
                {
                    auto update = Interface::ShowInventory(graphics, overlay, party, section.Items);

                    if (update && section.ImageAsset.empty())
                    {
                        BloodSword::Free(&image);

                        // regenerate party stats
                        image = Interface::GeneratePartyStats(graphics, party, panel_w - BloodSword::LargePad, panel_h - BloodSword::LargePad);
                    }

                    input.Selected = false;
                }
                else if (input.Type == Controls::Type::INVENTORY)
                {
                    auto update = Interface::ManageInventory(graphics, overlay, party, true);

                    if (update.Update && section.ImageAsset.empty())
                    {
                        BloodSword::Free(&image);

                        // regenerate party stats
                        image = Interface::GeneratePartyStats(graphics, party, panel_w - BloodSword::LargePad, panel_h - BloodSword::LargePad);
                    }

                    input.Selected = false;

                    if (Book::IsDefined(update.Next))
                    {
                        next = update.Next;

                        done = true;
                    }
                }
                else if (input.Type == Controls::Type::GAME)
                {
                    Interface::NotImplemented(graphics, overlay);

                    input.Selected = false;
                }
                else if (input.Type == Controls::Type::EXIT)
                {
                    done = true;
                }
                else if (input.Type == Controls::Type::SCROLL_UP || input.Up)
                {
                    if (text_h < texture_h)
                    {
                        offset -= scroll_speed;

                        if (offset < 0)
                        {
                            offset = 0;
                        }

                        scroll_up = true;
                    }
                }
                else if (input.Type == Controls::Type::SCROLL_DOWN || input.Down)
                {
                    if (text_h < texture_h)
                    {
                        offset += scroll_speed;

                        if (offset > (texture_h - text_h))
                        {
                            offset = texture_h - text_h;
                        }

                        scroll_dn = true;
                    }
                }
            }
        }

        BloodSword::Free(&image);

        BloodSword::Free(&texture);

        return next;
    }

    Book::Location ProcessSection(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, int current)
    {
        auto &section = (current >= 0 && current < Story::CurrentBook.Sections.size()) ? Story::CurrentBook.Sections[current] : Story::CurrentBook.Sections[0];

        std::cerr << "[SECTION] " << Book::String(section.Location) << std::endl;

        // save a copy party prior to background and events (for save game functionality)
        auto saved_party = party;

        // set party location (previous, current)
        party.Set(section.Location);

        auto next = Interface::ProcessBackground(graphics, background, party);

        // skip this section if background events redirect to another location
        if (Book::IsUndefined(next))
        {
            // process events
            auto section_text = section.Text;

            auto results = Interface::ProcessEvents(graphics, background, party);

            for (auto result : results)
            {
                if ((result.Result || result.Failed) && !result.Text.empty())
                {
                    if (!section_text.empty())
                    {
                        section_text += "\n\n";
                    }

                    section_text += result.Text;
                }
            }

            next = Interface::RenderSection(graphics, background, party, saved_party, section_text);
        }

        return next;
    }

    void ProcessStory(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, int current = 0)
    {
        auto &story = Story::CurrentBook;

        if (story.Title.size() > 0 && story.Description.size() > 0)
        {
            auto message = story.Title + "\n\n" + story.Description;

            auto wrap = graphics.Width - BloodSword::TileSize * 6;

            Interface::TextBox(graphics, background, message, wrap);
        }

        while (true)
        {
            auto location = Interface::ProcessSection(graphics, background, party, current);

            if (Book::IsDefined(location) && Engine::IsAlive(party))
            {
                current = story.Find(location);

                // log missing items
                std::cerr << "[FIND] " << Book::String(location) << " == " << (current != -1 ? "FOUND" : "NOT FOUND") << std::endl;

                if (!(current != -1 && Book::IsDefined(story.Sections[current].Location)))
                {
                    Interface::Notify(graphics, background, Interface::MSG_IMPLEMENT);

                    break;
                }
            }
            else
            {
                Interface::Notify(graphics, background, Interface::MSG_PLAY);

                break;
            }
        }
    }
}

#endif
