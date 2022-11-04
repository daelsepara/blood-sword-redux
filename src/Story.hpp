#ifndef __STORY_HPP__
#define __STORY_HPP__

#include <string>

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

        Position ImageLocation = Position::NONE;

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
    Story::Base Load(const char* file)
    {
        auto story = Story::Base();

        return story;
    }
}

#endif