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

    Controls::User RenderSection(Graphics::Base &graphics, Scene::Base &background, Section::Base &section, Party::Base &party, std::string &text)
    {
        // texture for section text / default text
        SDL_Texture *texture = nullptr;

        // texture for left panel (either party stats, or specific images)
        SDL_Texture *panel = nullptr;

        auto action = Controls::User();

        // includes tile-sized borders on left, right, and middle (divider)
        auto panel_w = (graphics.Width - BloodSword::TileSize * 3) / 2;

        // includes tile-sized top, bottom borders and one row of buttons
        auto panel_h = (graphics.Height - BloodSword::TileSize * 3);

        // maximum size of viewable text
        auto text_w = panel_w - BloodSword::Pad * 2;

        auto text_h = panel_h - BloodSword::Pad * 2;

        // location where both panels are rendered
        auto origin = Point(graphics.Width - (panel_w * 2 + BloodSword::TileSize * 3), graphics.Height - (panel_h + BloodSword::TileSize * 3)) / 2;

        if (text.length() > 0)
        {
            texture = Graphics::CreateText(graphics, text.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, text_w);
        }
        else
        {
            texture = Graphics::CreateText(graphics, "You decide what to do next.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, text_w);
        }

        if (section.ImageAsset.length() > 0)
        {
            panel = Asset::Create(graphics.Renderer, section.ImageAsset.c_str());
        }

        auto done = false;

        while (!done)
        {
            auto overlay = Scene::Base();

            if (panel)
            {
                // add left panel
                overlay.VerifyAndAdd(Scene::Element(panel, origin + Point(BloodSword::Pad, BloodSword::Pad)));
            }

            if (texture)
            {
                // add text
            }

            action = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, action, true);
        }

        Free(&panel);

        Free(&texture);

        return action;
    }

    Book::Location ProcessSection(Graphics::Base &graphics, Scene::Base &background, Section::Base &section, Party::Base &party)
    {
        Book::Location next = Interface::ProcessBackground(graphics, background, section, party);

        if (!Book::IsUndefined(next))
        {
            auto once = false;

            auto done = false;

            auto section_text = section.Text;

            while (!done)
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
                    auto input = Interface::RenderSection(graphics, background, section, party, section_text);

                    if (input.Type == Controls::Type::NEXT)
                    {
                        // get next location
                        next = Interface::NextSection(graphics, background, section, party);

                        if (Book::IsDefined(next) || !Engine::IsAlive(party))
                        {
                            done = true;
                        }
                    }
                    else
                    {
                        if (input.Type == Controls::Type::EXIT)
                        {
                            done = true;
                        }
                    }
                }
                else
                {
                    done = true;
                }
            }
        }

        return next;
    }
}

#endif
