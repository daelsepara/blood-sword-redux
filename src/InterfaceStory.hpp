#ifndef __INTERFACE_STORY_HPP__
#define __INTERFACE_STORY_HPP__

#include "Choice.hpp"
#include "Conditions.hpp"
#include "Section.hpp"
#include "Interface.hpp"

namespace BloodSword::Interface
{
    Book::Location RenderChoices(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, std::vector<Choice::Base> &choices)
    {
        Book::Location next = {Book::Number::NONE, -1};

        // TODO: render choices

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

        if (section.Next.size() > 0)
        {
            if (section.Battle.IsDefined())
            {
                // fight battle
                party.LastBattle = Interface::RenderBattle(graphics, section.Battle, party);
            }

            if (Engine::IsAlive(party))
            {
                // process choices if any
                if (section.Choices.size() > 0)
                {
                    next = Interface::RenderChoices(graphics, background, party, section.Choices);
                }
                else if (section.Next.size() > 0)
                {
                    // process through each condition
                    for (auto &condition : section.Next)
                    {
                        auto eval = Section::Conditions::Process(graphics, background, party, condition);

                        if (eval.Result)
                        {
                            next = condition.Location;

                            break;
                        }
                    }
                }
            }
        }

        return next;
    }

    Book::Location RenderSection(Graphics::Base &graphics, Scene::Base &background, Section::Base &section, Party::Base &party, std::string &text)
    {
        Book::Location next = {Book::Number::NONE, -1};

        // texture for section text / default text
        SDL_Texture *texture = nullptr;

        // texture for left panel (either party stats, or specific images)
        SDL_Texture *image = nullptr;

        // total size of all dividers
        auto space = BloodSword::TileSize * 3 + BloodSword::Pad * 4;

        // includes tile-sized borders on left, right, and middle (divider)
        auto panel_w = (graphics.Width - BloodSword::TileSize * 2 - space) / 2;

        // includes tile-sized top, bottom borders and one row of buttons
        auto panel_h = (graphics.Height - space);

        // maximum size of viewable text
        auto text_w = panel_w - BloodSword::Pad * 2;

        auto text_h = panel_h - BloodSword::Pad * 2;

        // location where left-most panel is rendered
        auto origin = Point((graphics.Width - (panel_w * 2 + BloodSword::TileSize + BloodSword::Pad * 2)) / 2, BloodSword::TileSize + BloodSword::Pad * 2);

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
            image = Interface::GeneratePartyStats(graphics, party, panel_w - BloodSword::Pad * 2, panel_h - BloodSword::Pad * 2);
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
            overlay.Add(Scene::Element(origin.X + panel_w + BloodSword::TileSize + BloodSword::Pad * 2, origin.Y, panel_w, panel_h, Color::Background, Color::Active, BloodSword::Border));

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
                    done = true;
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

        return next;
    }

    Book::Location ProcessSection(Graphics::Base &graphics, Scene::Base &background, Section::Base &section, Party::Base &party)
    {
        Book::Location next = Interface::ProcessBackground(graphics, background, section, party);

        if (!Book::IsUndefined(next))
        {
            auto once = false;

            auto section_text = section.Text;

            while (true)
            {
                // TODO: Process events
                if (!once)
                {
                    auto results = Interface::ProcessEvents(graphics, background, section, party);

                    for (auto result : results)
                    {
                        section_text += result.Text;
                    }

                    once = true;
                }

                // TODO: Render section text
                if (Engine::IsAlive(party))
                {
                    next = Interface::RenderSection(graphics, background, section, party, section_text);

                    break;
                }
                else
                {
                    Interface::MessageBox(graphics, background, Graphics::RichText("GAME OVER!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Highlight, 4, Color::Active, true);

                    break;
                }
            }
        }

        return next;
    }
}

#endif
