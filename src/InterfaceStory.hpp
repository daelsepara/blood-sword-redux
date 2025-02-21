#ifndef __INTERFACE_STORY_HPP__
#define __INTERFACE_STORY_HPP__

#include "Choice.hpp"
#include "Conditions.hpp"
#include "Section.hpp"
#include "Interface.hpp"

namespace BloodSword::Interface
{
    typedef std::vector<Choice::Base> Choices;

    Book::Location RenderChoices(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Choices &choices, bool after_battle = false)
    {
        Book::Location next = {Book::Number::NONE, -1};

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

        // default width
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
                overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), x, bottom));

                overlay.Add(Controls::Base(Controls::Type::BACK, id, id, id, first + limit - 1, id, x, bottom, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
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
                        auto eval = Section::Conditions::Process(graphics, background, party, choices[choice].Condition);

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
                            Interface::MessageBox(graphics, background, Graphics::RichText(eval.Text.c_str(), Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Highlight, BloodSword::Border, Color::Highlight, true);
                        }
                        else
                        {
                            next = choices[choice].Condition.Location;

                            done = true;
                        }
                    }
                }
            }
        }

        Free(menu);

        return next;
    }

    // process background events
    Book::Location ProcessBackground(Graphics::Base &graphics, Scene::Base &background, Section::Base &section, Party::Base &party)
    {
        Book::Location next = {Book::Number::NONE, -1};

        if (section.Background.size() > 0)
        {
            for (auto &condition : section.Background)
            {
                auto eval = Section::Conditions::Process(graphics, background, party, condition);

                if (eval.Result)
                {
                    next = condition.Location;

                    break;
                }
            }
        }

        return next;
    }

    // process real-time events
    std::vector<Section::Conditions::Evaluation> ProcessEvents(Graphics::Base &graphics, Scene::Base &background, Section::Base &section, Party::Base &party)
    {
        auto results = std::vector<Section::Conditions::Evaluation>();

        if (section.Events.size() > 0)
        {
            for (auto &condition : section.Events)
            {
                std::cerr << "Event: "
                          << "[" << Book::Mapping[section.Location.first]
                          << ": " << std::to_string(section.Location.second) << "]"
                          << std::endl;

                if (Engine::IsAlive(party))
                {
                    auto eval = Section::Conditions::Process(graphics, background, party, condition);

                    results.push_back(eval);
                }
            }
        }

        return results;
    }

    // get next location
    Book::Location NextSection(Graphics::Base &graphics, Scene::Base &background, Section::Base &section, Party::Base &party)
    {
        Book::Location next = {Book::Number::NONE, -1};

        auto after_battle = false;

        // fight battle
        if (section.Battle.IsDefined())
        {
            auto result = Interface::RenderBattle(graphics, section.Battle, party);

            if (result != Battle::Result::NONE)
            {
                after_battle = true;
            }
        }

        if (Engine::IsAlive(party))
        {
            if (section.Next.size() > 0)
            {
                // process through each condition
                for (auto &condition : section.Next)
                {
                    std::cerr << "Next: "
                              << "[" << Book::Mapping[condition.Location.first]
                              << ": " << std::to_string(condition.Location.second) << "]"
                              << std::endl;

                    auto eval = Section::Conditions::Process(graphics, background, party, condition);

                    if (eval.Result)
                    {
                        next = condition.Location;

                        break;
                    }
                }
            }
            else if (section.Choices.size() > 0)
            {
                // process through any choices
                while (true)
                {
                    std::cerr << "Choice: "
                              << "[" << Book::Mapping[section.Location.first]
                              << ": " << std::to_string(section.Location.second) << "]"
                              << std::endl;

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

    Book::Location RenderSection(Graphics::Base &graphics, Scene::Base &background, Section::Base &section, Party::Base &party, Party::Base &saved_party, std::string &text)
    {
        Book::Location next = {Book::Number::NONE, -1};

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

            auto id = 0;

            auto arrow_up = offset > 0;

            auto arrow_dn = text_h < texture_h && offset < (texture_h - text_h);

            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::RIGHT), buttons.X, buttons.Y));

            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::EXIT), buttons.X + BloodSword::TileSize + BloodSword::Pad, buttons.Y));

            if (arrow_up)
            {
                overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::UP), scroll_top.X, scroll_top.Y));
            }

            if (arrow_dn)
            {
                overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::DOWN), scroll_bot.X, scroll_bot.Y));
            }

            if (arrow_up || arrow_dn)
            {
                overlay.Add(Controls::Base(Controls::Type::NEXT, id, id, id + 1, id, id, buttons.X, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

                overlay.Add(Controls::Base(Controls::Type::EXIT, id + 1, id, id + 2, id + 1, id + 1, buttons.X + BloodSword::TileSize + BloodSword::Pad, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
            }

            if (arrow_up && arrow_dn)
            {
                overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id + 2, id + 1, id + 2, id + 2, id + 3, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

                overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id + 3, id + 1, id + 3, id + 2, id + 3, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
            }
            else if (arrow_up)
            {
                overlay.Add(Controls::Base(Controls::Type::SCROLL_UP, id + 2, id + 1, id + 2, id + 2, id + 3, scroll_top.X, scroll_top.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
            }
            else if (arrow_dn)
            {
                overlay.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id + 2, id + 1, id + 2, id + 2, id + 2, scroll_bot.X, scroll_bot.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
            }
            else
            {
                overlay.Add(Controls::Base(Controls::Type::NEXT, id, id, id + 1, id, id, buttons.X, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

                overlay.Add(Controls::Base(Controls::Type::EXIT, id + 1, id, id + 1, id + 1, id + 1, buttons.X + BloodSword::TileSize + BloodSword::Pad, buttons.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));
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
                    next = Interface::NextSection(graphics, overlay, section, party);

                    if (Book::IsDefined(next) || !Engine::IsAlive(party))
                    {
                        done = true;
                    }
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

        Free(&image);

        Free(&texture);

        if (!Engine::IsAlive(party))
        {
            Interface::MessageBox(graphics, background, Graphics::RichText("GAME OVER!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Highlight, BloodSword::Border, Color::Active, true);
        }

        return next;
    }

    Book::Location ProcessSection(Graphics::Base &graphics, Scene::Base &background, Section::Base &section, Party::Base &party)
    {
        // save a copy party prior to background and events (for save game functionality)
        auto saved_party = party;

        // set party location (previous, current)
        party.PreviousLocation = party.Location;

        party.Location = section.Location;

        Book::Location next = Interface::ProcessBackground(graphics, background, section, party);

        // skip this section if background events redirect to another location
        if (Book::IsUndefined(next))
        {
            auto once = false;

            auto section_text = section.Text;

            while (true)
            {
                // process events
                if (!once)
                {
                    auto results = Interface::ProcessEvents(graphics, background, section, party);

                    for (auto result : results)
                    {
                        section_text += "\n\n" + result.Text;
                    }

                    once = true;
                }

                if (Engine::IsAlive(party))
                {
                    next = Interface::RenderSection(graphics, background, section, party, saved_party, section_text);

                    break;
                }
                else
                {
                    Interface::MessageBox(graphics, background, Graphics::RichText("GAME OVER!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Highlight, BloodSword::Border, Color::Active, true);

                    break;
                }
            }
        }

        return next;
    }

    void ProcessStory(Graphics::Base &graphics, Scene::Base &background, Story::Base &story, Party::Base &party, int section = 0)
    {
        if (story.Title.size() > 0 && story.Description.size() > 0)
        {
            auto message = story.Title + "\n\n" + story.Description;

            auto wrap = graphics.Width - BloodSword::TileSize * 6;

            Interface::TextBox(graphics, background, Fonts::Normal, message.c_str(), wrap, Color::S(Color::Active), TTF_STYLE_NORMAL, Color::Background, Color::Active, BloodSword::Border, Color::Active, true);
        }

        auto done = false;

        while (!done)
        {
            auto &current = story.Sections[section];

            auto location = Interface::ProcessSection(graphics, background, current, party);

            if (Book::IsDefined(location))
            {
                section = story.Find(location);

                std::cerr << "Find: "
                << "[" << Book::Mapping[location.first]
                << ": " << std::to_string(location.second) << "]"
                << " == " << std::to_string(section)
                << std::endl;

                if (!(section != -1 && Book::IsDefined(story.Sections[section].Location)))
                {
                    Interface::MessageBox(graphics, background, Graphics::RichText("NOT YET IMPLEMENTED", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Active, 4, Color::Inactive, true);

                    done = true;
                }
            }
            else
            {
                done = true;
            }
        }
    }
}

#endif
