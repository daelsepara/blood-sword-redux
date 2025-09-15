#ifndef __CHOICE_HPP__
#define __CHOICE_HPP__

#include "nlohmann/json.hpp"
#include "Conditions.hpp"

// classes and functions for managing story choices
namespace BloodSword::Choice
{
    // choice base class
    class Base
    {
    public:
        // Text to display
        std::string Text;

        Conditions::Base Condition;

        Base() {}
    };

    // list of choices
    typedef std::vector<Choice::Base> List;

    // parse choice from json
    Choice::Base Parse(nlohmann::json &data)
    {
        auto choice = Choice::Base();

        if (!data.is_null())
        {
            if (!data["text"].is_null())
            {
                // set text
                choice.Text = std::string(data["text"]);
            }

            // parse condition
            if (!data["condition"].is_null())
            {
                choice.Condition = Conditions::Parse(data["condition"]);
            }
        }

        return choice;
    }
}

#endif
