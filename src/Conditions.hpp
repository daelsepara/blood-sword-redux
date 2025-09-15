#ifndef __CONDITIONS_HPP__
#define __CONDITIONS_HPP__

#include <algorithm>

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "ConditionTypes.hpp"

namespace BloodSword::Conditions
{
    // conditions base class
    class Base
    {
    public:
        // condition type
        Conditions::Type Type = Type::NONE;

        // destination (book, section)
        Book::Location Location = Book::Undefined;

        // internal variables used to validate "condition"
        std::vector<std::string> Variables = {};

        // flag to indicate that the result (or condition) is inverted
        bool Invert = false;

        // alternate location
        Book::Location Failure = Book::Undefined;

        // additional text to display
        Book::Location AdditionalText = Book::Undefined;

        Base() {}

        Base(Conditions::Type type,
             Book::Location location,
             std::vector<std::string> variables,
             bool invert) : Type(type),
                            Variables(variables),
                            Invert(invert) {}

        Base(Conditions::Type type,
             Book::Location location,
             std::vector<std::string> variables) : Type(type),
                                                   Variables(variables) {}

        Base(const char *type, Book::Location location, std::vector<std::string> variables, bool invert = false)
        {
            this->Type = Conditions::Map(type);

            this->Variables = variables;

            this->Invert = invert;
        }
    };

    // list of conditions
    typedef std::vector<Conditions::Base> List;

    // parse condition from JSON data
    Conditions::Base Parse(nlohmann::json &data)
    {
        auto condition = Conditions::Base();

        if (!data.is_null())
        {
            if (!data["location"].is_null())
            {
                condition.Location = Book::Load(data["location"]);
            }

            // load alternate location
            if (!data["failure"].is_null())
            {
                condition.Failure = Book::Load(data["failure"]);
            }

            // load additional text
            if (!data["additional_text"].is_null())
            {
                condition.AdditionalText = Book::Load(data["additional_text"]);
            }

            // set condition type
            condition.Type = !data["type"].is_null() ? Conditions::Map(std::string(data["type"])) : Conditions::Type::NONE;

            // set variables
            if (!data["variables"].is_null() && data["variables"].is_array() && data["variables"].size() > 0)
            {
                auto variables = std::vector<std::string>();

                for (auto i = 0; i < data["variables"].size(); i++)
                {
                    variables.push_back(std::string(data["variables"][i]));
                }

                condition.Variables = variables;
            }

            // set invert condition
            condition.Invert = (!data["invert"].is_null() && data["invert"].is_boolean()) ? data["invert"].get<bool>() : false;
        }

        return condition;
    }
}

#endif
