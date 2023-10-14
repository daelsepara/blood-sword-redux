#ifndef __INTERFACE_STORY_HPP__
#define __INTERFACE_STORY_HPP__

#include "Choice.hpp"
#include "Conditions.hpp"
#include "Interface.hpp"

namespace BloodSword::Interface
{
    Book::Location Choices(Party::Base &party, std::vector<Choice::Base> &choices)
    {
        Book::Location next = {Book::Number::NONE, -1};

        // TODO: render choices

        return next;
    }

    Book::Location Next(Party::Base &party, std::vector<Story::Conditions::Base> &conditions)
    {
        Book::Location next = {Book::Number::NONE, -1};

        // process through each condition
        for (auto &condition : conditions)
        {
            if (Story::Conditions::Process(party, condition))
            {
                next = condition.Location;

                break;
            }
        }

        return next;
    }
}

#endif
