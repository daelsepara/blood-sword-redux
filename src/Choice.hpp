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

        BloodSword::Section::Conditions::Base Condition;

        Base() {}
    };

    Choice::Base Parse(nlohmann::json &data)
    {
        auto choice = Choice::Base();

        if (!data.is_null())
        {
            if (!data["text"].is_null())
            {
                std::cerr << "Choice::Parse: Loading text ..." << std::endl;

                // set text
                choice.Text = std::string(data["text"]);
            }

            // parse condition
            if (!data["condition"].is_null())
            {
                std::cerr << "Choice::Parse: Loading conditions ..." << std::endl;

                choice.Condition = Section::Conditions::Parse(data["condition"]);
            }
        }

        return choice;
    }
}

#endif
