#ifndef __STORY_HPP__
#define __STORY_HPP__

#include <string>

#include "nlohmann/json.hpp"
#include "Templates.hpp"
#include "Book.hpp"
#include "Battle.hpp"
#include "Choice.hpp"
#include "Primitives.hpp"

// classes and functions for managing story sections
namespace BloodSword::Story
{
    enum class Feature
    {
        NONE = -1,
        DESTINATIONS,
        CHOICES,
        BATTLE,
        SHOP
    };

    class Base
    {
    public:
        Book::Destination Section = std::make_pair<Book::Number, int>(Book::Number::None, 0);

        std::vector<Story::Feature> Features = {};

        std::vector<Book::Destination> Destinations = {};

        std::vector<Choice::Base> Choices = {};

        Position ImagePosition = Position::NONE;

        Battle::Base Battle;

        std::string ImageAsset;

        std::string Text;

        Base() {}

        bool Has(Story::Feature feature)
        {
            auto search = BloodSword::Find(this->Features, feature);

            return (search != this->Features.end()) && (*search != Story::Feature::NONE);
        }
    };

    // process story
    Book::Destination Play(Story::Base &story)
    {
        return {Book::Number::None, 0};
    }

    // load story from file
    Story::Base Load(const char *file)
    {
        auto story = Story::Base();

        std::ifstream ifs(file);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data["story"].is_null())
            {
                if (!data["story"]["section"].is_null())
                {
                    // set section
                }

                if (!data["story"]["features"].is_null() && data["story"]["features"].is_array() && data["story"]["features"].size() > 0)
                {
                    // set features
                }

                if (!data["story"]["destinations"].is_null() && data["story"]["destinations"].is_array() && data["story"]["destinations"].size() > 0)
                {
                    // set destinations
                }

                if (!data["story"]["choices"].is_null() && data["story"]["choices"].is_array() && data["story"]["choices"].size() > 0)
                {
                    // set choices
                }
            }
        }

        return story;
    }
}

#endif