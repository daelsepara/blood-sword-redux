#ifndef __INTERFACE_STORY_HPP__
#define __INTERFACE_STORY_HPP__

#include "InterfaceBattle.hpp"
#include "ConditionsEvaluate.hpp"

namespace BloodSword::Interface
{
    // log major story events
    void LogSectionHeader(const char *header, Book::Location location, bool newline = true)
    {
        std::cerr << "["
                  << header
                  << "] "
                  << Book::String(location);

        if (newline)
        {
            std::cerr << std::endl;
        }
    }

    // log story search results
    void LogSearch(Book::Location location, int current)
    {
        Interface::LogSectionHeader("FIND", location, false);

        std::cerr << " == " << (current != -1 ? "FOUND" : "NOT FOUND") << std::endl;
    }

    // render story section choices (if present)
    Book::Location RenderChoices(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Choice::List &choices, bool after_battle = false)
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
                        auto eval = Conditions::Process(graphics, background, party, choices[choice].Condition);

                        if (eval.Failed)
                        {
                            // fails test
                            Interface::MessageBox(graphics, background, Graphics::RichText(eval.Text.c_str(), Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Highlight, BloodSword::Border, Color::Active, true);

                            // jumps to next destination
                            next = choices[choice].Condition.Failure;

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

    // set variable based on results of last evaluated condition
    void SetVariable(Party::Base &party, Conditions::Base &condition, bool result)
    {
        if (((condition.Type == Conditions::Type::IF_TRUE_SET && result) || (condition.Type == Conditions::Type::IF_FALSE_SET && !result)) && condition.Variables.size() > 1)
        {
            party.Set(condition.Variables[0], condition.Variables[1]);
        }
    }

    // process background events
    Book::Location ProcessBackground(Graphics::Base &graphics, Scene::Base &background, Party::Base &party)
    {
        auto current = Story::CurrentBook.Find(party.Location);

        auto &section = (current >= 0 && current < Story::CurrentBook.Sections.size()) ? Story::CurrentBook.Sections[current] : Story::CurrentBook.Sections[0];

        Book::Location next = Book::Undefined;

        if (section.Background.size() > 0)
        {
            Interface::LogSectionHeader("BACKGROUND", section.Location);

            auto last_result = false;

            for (auto &condition : section.Background)
            {
                if (condition.Type != Conditions::Type::IF_TRUE_SET && condition.Type != Conditions::Type::IF_FALSE_SET)
                {
                    auto eval = Conditions::Process(graphics, background, party, condition);

                    last_result = (eval.Failed || eval.Result);

                    if (eval.Failed)
                    {
                        if (Book::IsDefined(condition.Failure))
                        {
                            next = condition.Failure;

                            break;
                        }
                    }
                    else if (eval.Result)
                    {
                        // process multiple background events until they resolve into a new location
                        if (Book::IsDefined(condition.Location))
                        {
                            next = condition.Location;

                            break;
                        }
                    }
                }
                else
                {
                    Interface::SetVariable(party, condition, last_result);
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
            Interface::LogSectionHeader("EVENT", section.Location);

            auto last_result = false;

            for (auto &condition : section.Events)
            {
                if (Engine::IsAlive(party))
                {
                    if (condition.Type != Conditions::Type::IF_TRUE_SET && condition.Type != Conditions::Type::IF_FALSE_SET)
                    {
                        auto eval = Conditions::Process(graphics, background, party, condition);

                        last_result = (eval.Result || eval.Failed);

                        results.push_back(eval);
                    }
                    else
                    {
                        Interface::SetVariable(party, condition, last_result);
                    }
                }
            }
        }

        return results;
    }

    // process events before battle
    Book::Location ProcessBattleEvents(Graphics::Base &graphics, Scene::Base &background, Party::Base &party)
    {
        auto next = Book::Undefined;

        auto current = Story::CurrentBook.Find(party.Location);

        auto &section = (current >= 0 && current < Story::CurrentBook.Sections.size()) ? Story::CurrentBook.Sections[current] : Story::CurrentBook.Sections[0];

        if (section.BattleEvents.size() > 0)
        {
            Interface::LogSectionHeader("PRE-BATTLE", section.Location);

            for (auto &condition : section.BattleEvents)
            {
                // ignore results
                auto eval = Conditions::Process(graphics, background, party, condition);

                if ((eval.Result && Book::IsDefined(condition.Location)) || (eval.Failed && Book::IsDefined(condition.Failure)))
                {
                    if (eval.Failed)
                    {
                        next = condition.Failure;
                    }
                    else if (eval.Result)
                    {
                        next = condition.Location;
                    }

                    break;
                }
            }
        }

        return next;
    }

    // get next location (story jump)
    Book::Location NextSection(Graphics::Base &graphics, Scene::Base &background, Party::Base &party)
    {
        auto current = Story::CurrentBook.Find(party.Location);

        auto &section = (current >= 0 && current < Story::CurrentBook.Sections.size()) ? Story::CurrentBook.Sections[current] : Story::CurrentBook.Sections[0];

        Book::Location next = Book::Undefined;

        auto after_battle = false;

        auto exit_battle = false;

        if (section.BattleEvents.size() > 0 && !section.ProcessedBattleEvents)
        {
            // process this only once
            next = Interface::ProcessBattleEvents(graphics, background, party);

            section.ProcessedBattleEvents = true;
        }

        // fight battle
        if (Engine::IsAlive(party) && section.Battle.IsDefined() && Book::IsUndefined(next))
        {
            Interface::LogSectionHeader("BATTLE", section.Location);

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

        if (Engine::IsAlive(party) && !exit_battle && Book::IsUndefined(next))
        {
            if (section.Next.size() > 0)
            {
                Interface::LogSectionHeader("NEXT", section.Location);

                auto last_result = false;

                // process through each condition
                for (auto &condition : section.Next)
                {
                    if (Engine::IsAlive(party))
                    {
                        if (condition.Type != Conditions::Type::IF_TRUE_SET && condition.Type != Conditions::Type::IF_FALSE_SET)
                        {
                            auto eval = Conditions::Process(graphics, background, party, condition);

                            last_result = (eval.Result || eval.Failed);

                            // handle 'NEXT' situations that behave like events
                            if (eval.Result)
                            {
                                if (!eval.Text.empty())
                                {
                                    Interface::MessageBox(graphics, background, eval.Text, Color::Active);
                                }

                                if (Book::IsDefined(condition.Location))
                                {
                                    next = condition.Location;

                                    break;
                                }
                            }
                            else if (eval.Failed)
                            {
                                if (!eval.Text.empty())
                                {
                                    Interface::MessageBox(graphics, background, eval.Text, Color::Highlight);
                                }

                                if (Book::IsDefined(condition.Failure))
                                {
                                    next = condition.Failure;

                                    break;
                                }
                            }
                        }
                        else
                        {
                            Interface::SetVariable(party, condition, last_result);
                        }
                    }
                }
            }

            if (Engine::IsAlive(party) && !Book::IsDefined(next) && section.Choices.size() > 0)
            {
                // process through any choices
                while (true)
                {
                    Interface::LogSectionHeader("CHOICE", section.Location);

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

    // when character is in a task / alone
    void CharacterStoryControls(Party::Base &party, Scene::Base &overlay, Point buttons, Point scroll_top, Point scroll_bot, bool arrow_up, bool arrow_dn, Character::Class character)
    {
        auto current = Story::CurrentBook.Find(party.Location);

        auto &section = (current >= 0 && current < Story::CurrentBook.Sections.size()) ? Story::CurrentBook.Sections[current] : Story::CurrentBook.Sections[0];

        auto elements = overlay.Elements.size();

        auto controls = overlay.Controls.size();

        auto button_spacing = BloodSword::TileSize + BloodSword::Pad;

        auto num_buttons = 0;

        auto id = 0;

        auto heal = (character == Character::Class::SAGE);

        auto spells = (character == Character::Class::ENCHANTER);

        // add button textures
        if (section.Battle.IsDefined())
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::FIGHT), buttons.X, buttons.Y));

            // fight button hotspot
            overlay.Add(Controls::Base(Controls::Type::FIGHT, id, id, id + 1, id, id, buttons.X, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::RIGHT), buttons.X, buttons.Y));

            // next button hotspot
            overlay.Add(Controls::Base(Controls::Type::NEXT, id, id, id + 1, id, id, buttons.X, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }

        num_buttons++;

        id++;

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
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 2, id + 2, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            // scroll buttons hotspots
            id++;

            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id + 1, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id + 1, id - 1, id + 1, id, id - 1, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_up)
        {
            // exit hotspot when scroll up is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id + 1, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll up hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id - 1, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_dn)
        {
            // exit hotspot when scroll down is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id + 1, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll down hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id, id - 1, id, id, id - 1, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
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

    // generate (default) story controls
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

            // fight button hotspot
            overlay.Add(Controls::Base(Controls::Type::FIGHT, id, id, id + 1, id, id, buttons.X, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::RIGHT), buttons.X, buttons.Y));

            // next button hotspot
            overlay.Add(Controls::Base(Controls::Type::NEXT, id, id, id + 1, id, id, buttons.X, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }

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
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 2, id + 2, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            // scroll buttons hotspots
            id++;

            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id + 1, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id + 1, id - 1, id + 1, id, id - 1, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_up)
        {
            // exit hotspot when scroll up is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id + 1, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll up hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id - 1, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_dn)
        {
            // exit hotspot when scroll down is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id + 1, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll down hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id, id - 1, id, id, id - 1, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
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

    // generate character inventory controls when no image is rendered on the left pane
    void InventoryControls(Scene::Base &overlay, Party::Base &party, SDL_Texture *image, Point origin)
    {
        auto id = overlay.Controls.size();

        auto next_control = Controls::Find(overlay.Controls, Controls::Type::NEXT);

        if (next_control == -1)
        {
            next_control = Controls::Find(overlay.Controls, Controls::Type::FIGHT);
        }

        auto offset_y = BloodSword::Height(image) / party.Count();

        for (auto i = 0; i < party.Count(); i++)
        {
            if (party[i].Class != Character::Class::NONE)
            {
                auto up = (i > 0);

                auto dn = (i < party.Count() - 1);

                overlay.Add(Controls::Base(Interface::CharacterControls[party[i].Class], id, id, id, (up ? id - 1 : next_control), (dn ? id + 1 : next_control), origin.X + BloodSword::Pad, origin.Y + offset_y * i + BloodSword::Pad, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

                id++;
            }
        }

        overlay.Controls[next_control].Up = id - 1;
    }

    // generate story controls when processing an item effect
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

            // next button hotspot
            overlay.Add(Controls::Base(Controls::Type::FIGHT, id, id, id + 1, id, id, buttons.X, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else
        {
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::RIGHT), buttons.X, buttons.Y));

            // next button hotspot
            overlay.Add(Controls::Base(Controls::Type::NEXT, id, id, id + 1, id, id, buttons.X, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }

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
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 2, id + 2, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            // scroll buttons hotspots
            id++;

            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id + 1, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id + 1, id - 1, id + 1, id, id - 1, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_up)
        {
            // exit hotspot when scroll up is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id + 1, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll up hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id - 1, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_dn)
        {
            // exit hotspot when scroll down is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id + 1, id, buttons.X + num_buttons * button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll down hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id, id - 1, id, id, id - 1, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
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

    // generate story controls when at a story ending and / or party is incapacitated
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
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 2, id + 2, id, buttons.X + button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            // scroll buttons hotspots
            id++;

            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id + 1, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id + 1, id - 1, id + 1, id, id - 1, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_up)
        {
            // exit hotspot when scroll up is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id + 1, id, buttons.X + button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll up hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id, id - 1, id, id, id - 1, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
        }
        else if (arrow_dn)
        {
            // exit hotspot when scroll down is present
            overlay.Add(Controls::Base(Controls::Type::EXIT, id, id - 1, id + 1, id + 1, id, buttons.X + button_spacing, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            num_buttons++;

            id++;

            // scroll down hotspot
            overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id, id - 1, id, id, id - 1, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
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

    // global effects based on section FEATURES
    void ApplyGlobalEffects(Section::Base &section, Party::Base &party)
    {
        if (section.Has(Feature::Type::DOUBLE_HEALING))
        {
            if (party.Has(Character::Class::SAGE) && Engine::IsAlive(party[Character::Class::SAGE]))
            {
                party[Character::Class::SAGE].Add(Character::Status::DOUBLE_HEALING);
            }
        }
        else
        {
            if (Engine::IsAlive(party))
            {
                party.Remove(Character::Status::DOUBLE_HEALING);
            }
        }
    }

    // story controls
    void SetupControls(Scene::Base &overlay, Section::Base &section, Party::Base &party, SDL_Texture *image, Point origin, Point buttons, Point scroll_top, Point scroll_bot, bool arrow_up, bool arrow_dn)
    {
        if (section.Has(Feature::Type::ENDING) || !Engine::IsAlive(party))
        {
            // section is an ending and/or entire party has been incapacitated
            Interface::EndingControls(party, overlay, buttons, scroll_top, scroll_bot, arrow_up, arrow_dn);
        }
        else if (section.Has(Feature::Type::TASK) || section.Has(Feature::Type::ALONE))
        {
            // section is party of a class-specific task
            auto character = Character::Class::NONE;

            if (section.Has(Feature::Type::TASK_WARRIOR) || Engine::FirstClass(party, Character::Status::ALONE) == Character::Class::WARRIOR)
            {
                character = Character::Class::WARRIOR;
            }
            else if (section.Has(Feature::Type::TASK_TRICKSTER) || Engine::FirstClass(party, Character::Status::ALONE) == Character::Class::TRICKSTER)
            {
                character = Character::Class::TRICKSTER;
            }
            else if (section.Has(Feature::Type::TASK_SAGE) || Engine::FirstClass(party, Character::Status::ALONE) == Character::Class::SAGE)
            {
                character = Character::Class::SAGE;
            }
            else if (section.Has(Feature::Type::TASK_ENCHANTER) || Engine::FirstClass(party, Character::Status::ALONE) == Character::Class::ENCHANTER)
            {
                character = Character::Class::ENCHANTER;
            }

            Interface::CharacterStoryControls(party, overlay, buttons, scroll_top, scroll_bot, arrow_up, arrow_dn, character);

            if (section.ImageAsset.length() == 0)
            {
                Interface::InventoryControls(overlay, party, image, origin);
            }
        }
        else if (section.Has(Feature::Type::ITEM_EFFECT))
        {
            // section is an item effect
            Interface::ItemControls(party, overlay, buttons, scroll_top, scroll_bot, arrow_up, arrow_dn);
        }
        else
        {
            // default
            Interface::StoryControls(party, overlay, buttons, scroll_top, scroll_bot, arrow_up, arrow_dn);

            if (section.ImageAsset.length() == 0)
            {
                // setup character inventories
                Interface::InventoryControls(overlay, party, image, origin);
            }
        }
    }

    // render story section
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

        std::vector<std::string> captions = {
            "CONTINUE",
            "BATTLE",
            "ITEMS",
            "HEAL",
            "SPELLS",
            "INVENTORY",
            "GAME",
            "EXIT"};

        Controls::Collection caption_controls = {
            Controls::Type::NEXT,
            Controls::Type::FIGHT,
            Controls::Type::ITEMS,
            Controls::Type::HEAL,
            Controls::Type::SPELLS,
            Controls::Type::INVENTORY,
            Controls::Type::GAME,
            Controls::Type::EXIT};

        // create captions textures
        auto textures = Graphics::CreateText(graphics, Graphics::GenerateTextList(captions, Fonts::Caption, Color::Active, 0));

        // global effects (based on FEATURES of this section)
        Interface::ApplyGlobalEffects(section, party);

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
            Interface::SetupControls(overlay, section, party, image, origin, buttons, scroll_top, scroll_bot, arrow_up, arrow_dn);

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

            auto caption_id = Controls::Find(caption_controls, input.Type);

            if (caption_id >= 0 && caption_id < captions.size())
            {
                if (input.Current >= 0 && input.Current < overlay.Controls.size())
                {
                    auto &control = overlay.Controls[input.Current];

                    if (textures[caption_id])
                    {
                        // center texture
                        auto center = (control.W - BloodSword::Width(textures[caption_id])) / 2;

                        overlay.VerifyAndAdd(Scene::Element(textures[caption_id], control.X + center, control.Y + control.H + BloodSword::Pad));
                    }
                }
            }

            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

            if ((input.Selected && input.Type != Controls::Type::NONE && !input.Hold) || input.Up || input.Down)
            {
                if (input.Type == Controls::Type::NEXT || input.Type == Controls::Type::FIGHT)
                {
                    auto input_type = input.Type;

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
                        else if (section.ImageAsset.empty())
                        {
                            BloodSword::Free(&image);

                            // regenerate party stats
                            image = Interface::GeneratePartyStats(graphics, party, panel_w - BloodSword::LargePad, panel_h - BloodSword::LargePad);
                        }
                    }
                    else
                    {
                        Interface::ErrorMessage(graphics, overlay, Interface::MSG_OVER);
                    }

                    input.Current = Controls::Find(overlay.Controls, input_type);

                    input.Selected = false;
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

                    input.Current = Controls::Find(overlay.Controls, Controls::Type::SPELLS);

                    input.Selected = false;
                }
                else if (input.Type == Controls::Type::HEAL)
                {
                    if (party.Has(Character::Class::SAGE))
                    {
                        Interface::Heal(graphics, overlay, party, party[Character::Class::SAGE], true);
                    }

                    input.Current = Controls::Find(overlay.Controls, Controls::Type::HEAL);

                    input.Selected = false;
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

                    input.Current = Controls::Find(overlay.Controls, Controls::Type::ITEMS);

                    input.Selected = false;
                }
                else if (input.Type == Controls::Type::INVENTORY)
                {
                    Interface::ItemResult update;

                    if (section.Has(Feature::Type::TASK) || section.Has(Feature::Type::ALONE))
                    {
                        auto character = Character::Class::NONE;

                        if (section.Has(Feature::Type::TASK_WARRIOR) || Engine::FirstClass(party, Character::Status::ALONE) == Character::Class::WARRIOR)
                        {
                            character = Character::Class::WARRIOR;
                        }
                        else if (section.Has(Feature::Type::TASK_TRICKSTER) || Engine::FirstClass(party, Character::Status::ALONE) == Character::Class::TRICKSTER)
                        {
                            character = Character::Class::TRICKSTER;
                        }
                        else if (section.Has(Feature::Type::TASK_SAGE) || Engine::FirstClass(party, Character::Status::ALONE) == Character::Class::SAGE)
                        {
                            character = Character::Class::SAGE;
                        }
                        else if (section.Has(Feature::Type::TASK_ENCHANTER) || Engine::FirstClass(party, Character::Status::ALONE) == Character::Class::ENCHANTER)
                        {
                            character = Character::Class::ENCHANTER;
                        }

                        if (character != Character::Class::NONE)
                        {
                            update = Interface::ManageInventory(graphics, overlay, party, character, true);
                        }
                    }
                    else
                    {
                        update = Interface::ManageInventory(graphics, overlay, party, true);
                    }

                    if (update.Update && section.ImageAsset.empty())
                    {
                        BloodSword::Free(&image);

                        // regenerate party stats
                        image = Interface::GeneratePartyStats(graphics, party, panel_w - BloodSword::LargePad, panel_h - BloodSword::LargePad);
                    }

                    if (Book::IsDefined(update.Next))
                    {
                        next = update.Next;

                        done = true;
                    }

                    input.Current = Controls::Find(overlay.Controls, Controls::Type::INVENTORY);

                    input.Selected = false;
                }
                else if (Interface::IsCharacter(input.Type))
                {
                    auto character = Interface::GetCharacter(input.Type);

                    if (character != Character::Class::NONE && party.Has(character))
                    {
                        auto in_task = Character::Class::NONE;

                        if (section.Has(Feature::Type::TASK) || section.Has(Feature::Type::ALONE))
                        {
                            if (section.Has(Feature::Type::TASK_WARRIOR) || Engine::FirstClass(party, Character::Status::ALONE) == Character::Class::WARRIOR)
                            {
                                in_task = Character::Class::WARRIOR;
                            }
                            else if (section.Has(Feature::Type::TASK_TRICKSTER) || Engine::FirstClass(party, Character::Status::ALONE) == Character::Class::WARRIOR)
                            {
                                in_task = Character::Class::TRICKSTER;
                            }
                            else if (section.Has(Feature::Type::TASK_SAGE) || Engine::FirstClass(party, Character::Status::ALONE) == Character::Class::WARRIOR)
                            {
                                in_task = Character::Class::SAGE;
                            }
                            else if (section.Has(Feature::Type::TASK_ENCHANTER) || Engine::FirstClass(party, Character::Status::ALONE) == Character::Class::WARRIOR)
                            {
                                in_task = Character::Class::ENCHANTER;
                            }
                        }

                        if (in_task == Character::Class::NONE || (in_task != Character::Class::NONE && character == in_task))
                        {
                            if (Engine::IsAlive(party[character]))
                            {
                                auto update = Interface::ManageInventory(graphics, overlay, party, party[character], true);

                                if (update.Update && section.ImageAsset.empty())
                                {
                                    BloodSword::Free(&image);

                                    // regenerate party stats
                                    image = Interface::GeneratePartyStats(graphics, party, panel_w - BloodSword::LargePad, panel_h - BloodSword::LargePad);
                                }

                                if (Book::IsDefined(update.Next))
                                {
                                    next = update.Next;

                                    done = true;
                                }
                            }
                            else
                            {
                                Interface::MessageBox(graphics, overlay, Engine::IsDead(party[character]), Color::Highlight);
                            }
                        }
                        else if (in_task != Character::Class::NONE && character != in_task)
                        {
                            std::string message = party[character].Name + " IS AWAY";

                            Interface::MessageBox(graphics, overlay, message, Color::Highlight);
                        }
                    }

                    input.Selected = false;
                }
                else if (input.Type == Controls::Type::GAME)
                {
                    auto update = Interface::GameMenu(graphics, overlay, party);

                    if (update && section.ImageAsset.empty())
                    {
                        BloodSword::Free(&image);

                        // regenerate party stats
                        image = Interface::GeneratePartyStats(graphics, party, panel_w - BloodSword::LargePad, panel_h - BloodSword::LargePad);
                    }

                    input.Current = Controls::Find(overlay.Controls, Controls::Type::GAME);

                    input.Selected = false;
                }
                else if (input.Type == Controls::Type::EXIT)
                {
                    if (Interface::Confirm(graphics, overlay, "ARE YOU SURE?", Color::Background, Color::Active, BloodSword::Border, Color::Active, false))
                    {
                        done = true;
                    }

                    input.Selected = false;
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

                    input.Selected = false;
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

                    input.Selected = false;
                }
            }
        }

        BloodSword::Free(&image);

        BloodSword::Free(&texture);

        BloodSword::Free(textures);

        return next;
    }

    // play current story section
    Book::Location ProcessSection(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, int current)
    {
        auto &section = (current >= 0 && current < Story::CurrentBook.Sections.size()) ? Story::CurrentBook.Sections[current] : Story::CurrentBook.Sections[0];

        Interface::LogSectionHeader("SECTION", section.Location);

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

    // play the current loaded book
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
                Interface::LogSearch(location, current);

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
