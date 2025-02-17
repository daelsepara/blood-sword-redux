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

    Book::Location RenderSection(Graphics::Base &graphics, Section::Base &section, Party::Base &party)
    {
        auto scene = Scene::Base();

        Book::Location next = Interface::ProcessBackground(graphics, scene, section, party);

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
                    auto results = Interface::ProcessEvents(graphics, scene, section, party);

                    for (auto result : results)
                    {
                        section_text += result.Text;
                    }

                    once = true;
                }

                // TODO: Render section text
                if (Engine::IsAlive(party))
                {
                    Controls::User input;

                    Input::WaitForInput(graphics, scene, input);

                    if (input.Type == Controls::Type::NEXT)
                    {
                        // get next location
                        next = Interface::NextSection(graphics, scene, section, party);

                        if (Book::IsDefined(next) || !Engine::IsAlive(party))
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
