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

// classes and functions for managing story sections
namespace BloodSword::Story
{
    class Base
    {
    public:
        Book::Destination Section = {Book::Number::NONE, -1};

        std::vector<Feature::Type> Features = {};

        std::vector<Choice::Base> Choices = {};

        Position ImagePosition = Position::NONE;

        Battle::Base Battle;

        std::string ImageAsset;

        std::string Text;

        std::vector<Conditions::Base> Background = {};

        std::vector<Conditions::Base> Events = {};

        std::vector<Conditions::Base> Next = {};

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
            }
        }

        return story;
    }

    // process story
    Book::Destination Play(Graphics::Base &graphics, Story::Base &story, Party::Base &party)
    {
        Book::Destination next = {Book::Number::NONE, -1};

        // process any background events
        if (story.Background.size() > 0)
        {
        }

        // process current events
        if (story.Events.size() > 0)
        {
        }

        return next;
    }

    // get next destination
    Book::Destination Next(Graphics::Base &graphics, Story::Base &story, Party::Base &party)
    {
        Book::Destination next = {Book::Number::NONE, -1};

        auto battleResult = Battle::Result::NONE;

        if (story.Next.size() > 0)
        {
            if (story.Battle.IsDefined())
            {
                // fight battle
                auto result = Interface::Battle(graphics, story.Battle, party);
            }

            if (Engine::IsAlive(party))
            {
                // process choices if any
                if (story.Choices.size() > 0)
                {
                }
                else
                {
                    // select next destination
                }
            }
        }

        return next;
    }
}

#endif