#ifndef __STORY_HPP__
#define __STORY_HPP__

#include <string>

#include "nlohmann/json.hpp"
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

    Mapping<Story::Feature> FeatureMapping = {
        {Feature::NONE, "NONE"},
        {Feature::DESTINATIONS, "DESTINATIONS"},
        {Feature::CHOICES, "CHOICES"},
        {Feature::BATTLE, "BATTLE"},
        {Feature::SHOP, "SHOP"}};

    Story::Feature MapFeature(const char *feature)
    {
        return BloodSword::Find(Story::FeatureMapping, feature);
    }

    Story::Feature MapFeature(std::string feature)
    {
        return Story::MapFeature(feature.c_str());
    }

    class Base
    {
    public:
        Book::Destination Section = {Book::Number::NONE, 0};

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
        return {Book::Number::NONE, 0};
    }

    // TODO: load story from file
    Story::Base Load(const char *file)
    {
        auto story = Story::Base();

        std::ifstream ifs(file);

        if (ifs.good())
        {
            auto json = nlohmann::json::parse(ifs);

            if (!json["story"].is_null())
            {
                auto data = json["story"];

                if (!data["section"].is_null())
                {
                    // set section
                }

                if (!data["features"].is_null() && data["features"].is_array() && data["features"].size() > 0)
                {
                    // set features
                    auto features = std::vector<Story::Feature>();

                    story.Features = features;
                }

                if (!data["destinations"].is_null() && data["destinations"].is_array() && data["destinations"].size() > 0)
                {
                    // set destinations
                    auto destinations = std::vector<Book::Destination>();

                    story.Destinations = destinations;
                }

                if (!data["choices"].is_null() && data["choices"].is_array() && data["choices"].size() > 0)
                {
                    // set choices
                    auto choices = std::vector<Choice::Base>();

                    story.Choices = choices;
                }
            }
        }

        return story;
    }
}

#endif