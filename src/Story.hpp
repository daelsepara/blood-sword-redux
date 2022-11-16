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

                // set book and section number
                if (!data["section"].is_null())
                {
                    auto book = !data["section"]["book"].is_null() ? Book::MapBook(std::string(data["section"]["book"])) : Book::Number::NONE;

                    auto number = !data["section"]["number"].is_null() ? std::stoi(std::string(data["section"]["number"])) : 0;

                    story.Section = {book, number};
                }

                // set features
                if (!data["features"].is_null() && data["features"].is_array() && data["features"].size() > 0)
                {
                    auto features = std::vector<Story::Feature>();

                    for (auto i = 0; i < data["features"].size(); i++)
                    {
                        auto feature = !data["features"][i].is_null() ? Story::MapFeature(std::string(data["features"][i])) : Feature::NONE;

                        features.push_back(feature);
                    }

                    story.Features = features;
                }

                // read destinations
                if (!data["destinations"].is_null() && data["destinations"].is_array() && data["destinations"].size() > 0)
                {
                    auto destinations = std::vector<Book::Destination>();

                    for (auto i = 0; i < data["destinations"].size(); i++)
                    {
                        auto book = !data["destinations"][i]["book"].is_null() ? Book::MapBook(std::string(data["destinations"][i]["book"])) : Book::Number::NONE;

                        auto number = !data["destinations"][i]["number"].is_null() ? std::stoi(std::string(data["destinations"][i]["number"])) : 0;

                        destinations.push_back({book, number});
                    }

                    story.Destinations = destinations;
                }

                // read choices
                if (!data["choices"].is_null() && data["choices"].is_array() && data["choices"].size() > 0)
                {
                    auto choices = std::vector<Choice::Base>();

                    for (auto i = 0; i < data["choices"].size(); i++)
                    {
                    }

                    story.Choices = choices;
                }
            }
        }

        return story;
    }
}

#endif