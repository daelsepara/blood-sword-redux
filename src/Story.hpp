#ifndef __STORY_HPP__
#define __STORY_HPP__

#include <string>

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "Choice.hpp"
#include "Conditions.hpp"
#include "Feature.hpp"
#include "Primitives.hpp"
#include "InterfaceBattle.hpp"
#include "InterfaceStory.hpp"
#include "Templates.hpp"

// classes and functions for managing story sections
namespace BloodSword::Story
{
    BloodSword::Mapping<BloodSword::Position> PositionMapping = {
        {Position::NONE, "NONE"},
        {Position::TOP, "TOP"},
        {Position::LEFT, "LEFT"},
        {Position::RIGHT, "RIGHT"},
        {Position::BOTTOM, "BOTTOM"}};

    BloodSword::Position MapPosition(const char *position)
    {
        return BloodSword::Find(Story::PositionMapping, position);
    }

    BloodSword::Position MapPosition(std::string position)
    {
        return Story::MapPosition(position.c_str());
    }

    class Base
    {
    public:
        Book::Location Section = {Book::Number::NONE, -1};

        std::vector<Feature::Type> Features = {};

        std::vector<Choice::Base> Choices = {};

        Position ImagePosition = Position::NONE;

        Battle::Base Battle;

        std::string ImageAsset;

        std::string Text;

        std::vector<Story::Conditions::Base> Background = {};

        std::vector<Story::Conditions::Base> Events = {};

        std::vector<Story::Conditions::Base> Next = {};

        Base() {}

        bool Has(Feature::Type feature)
        {
            auto search = BloodSword::Find(this->Features, feature);

            return (search != this->Features.end()) && (*search != Feature::Type::NONE);
        }
    };

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

                    auto number = !data["section"]["number"].is_null() ? std::stoi(std::string(data["section"]["number"])) : -1;

                    story.Section = {book, number};
                }

                // set features
                if (!data["features"].is_null() && data["features"].is_array() && data["features"].size() > 0)
                {
                    auto features = std::vector<Feature::Type>();

                    for (auto i = 0; i < data["features"].size(); i++)
                    {
                        auto feature = !data["features"][i].is_null() ? Feature::Map(std::string(data["features"][i])) : Feature::Type::NONE;

                        features.push_back(feature);
                    }

                    story.Features = features;
                }

                // read choices
                if (!data["choices"].is_null() && data["choices"].is_array() && data["choices"].size() > 0)
                {
                    auto choices = std::vector<Choice::Base>();

                    for (auto i = 0; i < data["choices"].size(); i++)
                    {
                        auto choice = Choice::Parse(data["choices"][i]);

                        choices.push_back(choice);
                    }

                    story.Choices = choices;
                }

                // load battle
                if (!data["battle"].is_null())
                {
                    story.Battle.Initialize(data);
                }

                // image asset and position
                story.ImagePosition = !data["imagePosition"].is_null() ? Story::MapPosition(data["imagePosition"]) : Position::NONE;

                story.ImageAsset = !data["imageAsset"].is_null() ? std::string(data["imageAsset"]) : std::string();

                // story section text
                story.Text = !data["text"].is_null() ? std::string(data["text"]) : std::string();

                // load background events conditions
                if (!data["background"].is_null() && data["background"].is_array() && data["background"].size() > 0)
                {
                    auto background = std::vector<Story::Conditions::Base>();

                    for (auto i = 0; i < data["background"].size(); i++)
                    {
                        auto condition = Conditions::Parse(data["background"][i]);

                        background.push_back(condition);
                    }

                    story.Background = background;
                }

                // load real-time/run-once events conditions
                if (!data["events"].is_null() && data["events"].is_array() && data["events"].size() > 0)
                {
                    auto events = std::vector<Story::Conditions::Base>();

                    for (auto i = 0; i < data["event"].size(); i++)
                    {
                        auto condition = Conditions::Parse(data["event"][i]);

                        events.push_back(condition);
                    }

                    story.Events = events;
                }

                // load "next section" conditions
                if (!data["next"].is_null() && data["next"].is_array() && data["next"].size() > 0)
                {
                    auto next = std::vector<Story::Conditions::Base>();

                    for (auto i = 0; i < data["next"].size(); i++)
                    {
                        auto condition = Conditions::Parse(data["next"][i]);

                        next.push_back(condition);
                    }

                    story.Next = next;
                }
            }
        }

        return story;
    }
}

#endif
