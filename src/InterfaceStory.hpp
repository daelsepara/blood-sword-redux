#ifndef __INTERFACE_STORY_HPP__
#define __INTERFACE_STORY_HPP__

#include "Choice.hpp"
#include "Conditions.hpp"
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
    Book::Location ProcessBackground(Graphics::Base &graphics, Scene::Base &background, Story::Base &story, Party::Base &party)
    {
        Book::Location next = {Book::Number::NONE, -1};

        if (story.Background.size() > 0)
        {
            for (auto &condition : story.Background)
            {
                if (Story::Conditions::Process(graphics, background, party, condition))
                {
                    next = condition.Location;

                    break;
                }
            }
        }

        return next;
    }

    // process real-time events
    void ProcessEvents(Graphics::Base &graphics, Scene::Base &background, Story::Base &story, Party::Base &party)
    {
        if (story.Events.size() > 0)
        {
            for (auto &condition : story.Events)
            {
                if (Engine::IsAlive(party))
                {
                    Story::Conditions::Process(graphics, background, party, condition);
                }
            }
        }
    }

    // get next location
    Book::Location NextSection(Graphics::Base &graphics, Scene::Base &background, Story::Base &story, Party::Base &party)
    {
        Book::Location next = {Book::Number::NONE, -1};

        if (story.Next.size() > 0)
        {
            if (story.Battle.IsDefined())
            {
                // fight battle
                party.LastBattle = Interface::RenderBattle(graphics, story.Battle, party);
            }

            if (Engine::IsAlive(party))
            {
                // process choices if any
                if (story.Choices.size() > 0)
                {
                    next = Interface::RenderChoices(graphics, background, party, story.Choices);
                }
                else if (story.Next.size() > 0)
                {
                    // process through each condition
                    for (auto &condition : story.Next)
                    {
                        if (Story::Conditions::Process(graphics, background, party, condition))
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

    Book::Location RenderStory(Graphics::Base &graphics, Story::Base &story, Party::Base &party)
    {
        auto scene = Scene::Base();

        Book::Location next = Interface::ProcessBackground(graphics, scene, story, party);

        if (!Book::IsUndefined(next))
        {
            auto once = false;

            auto done = false;

            while (!done)
            {
                // TODO: render story
                if (!once)
                {
                    Interface::ProcessEvents(graphics, scene, story, party);

                    once = true;
                }

                if (Engine::IsAlive(party))
                {
                    Controls::User input;

                    Input::WaitForInput(graphics, scene, input);

                    if (input.Type == Controls::Type::NEXT)
                    {
                        // get next location
                        next = Interface::NextSection(graphics, scene, story, party);

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
