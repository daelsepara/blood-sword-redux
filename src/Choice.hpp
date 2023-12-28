#ifndef __CHOICE_HPP__
#define __CHOICE_HPP__

#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "Attribute.hpp"
#include "CharacterClasses.hpp"
#include "Item.hpp"
#include "Conditions.hpp"
#include "Graphics.hpp"

// classes and functions for managing story choices
namespace BloodSword::Choice
{
    class Base
    {
    public:
        BloodSword::Story::Conditions::Base Condition;

        Base() {}
    };

    Choice::Base Parse(nlohmann::json json)
    {
        auto choice = Choice::Base();

        choice.Condition = Story::Conditions::Parse(json);

        return choice;
    }

    bool Process(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Choice::Base &choice)
    {
        return Story::Conditions::Process(graphics, background, party, choice.Condition);
    }
}

#endif
