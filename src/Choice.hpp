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
        // Text to display
        std::string Text;

        BloodSword::Story::Conditions::Base Condition;

        Base() {}
    };

    Choice::Base Parse(nlohmann::json json)
    {
        auto choice = Choice::Base();

        if (!json["choice"].is_null())
        {
            auto data = json["choice"];

            if (!data["text"].is_null())
            {
                // set text
                choice.Text = std::string(data["text"]);
            }

            // parse condition
            choice.Condition = Story::Conditions::Parse(data);
        }

        return choice;
    }
}

#endif
