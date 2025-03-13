#ifndef __SECTION_HPP__
#define __SECTION_HPP__

#include <string>

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "Choice.hpp"
#include "Conditions.hpp"
#include "Feature.hpp"
#include "Primitives.hpp"
#include "Templates.hpp"
#include "Position.hpp"
#include "Battle.hpp"

// classes and functions for managing story sections
namespace BloodSword::Section
{
    class Base
    {
    public:
        Book::Location Location = Book::Undefined;

        Features::List Features = {};

        Choice::List Choices = {};

        Position::Type ImagePosition = Position::Type::NONE;

        Battle::Base Battle;

        std::string ImageAsset;

        std::string Text;

        Conditions::List Background = {};

        Conditions::List Events = {};

        Conditions::List Next = {};

        Items::Inventory Items = {};

        Base() {}

        bool Has(Feature::Type feature)
        {
            auto search = BloodSword::Find(this->Features, feature);

            return (search != this->Features.end()) && (*search != Feature::Type::NONE);
        }
    };

    Section::Base Load(nlohmann::json &data)
    {
        auto section = Section::Base();

        // set book and section number
        if (!data["location"].is_null())
        {
            section.Location = Book::Load(data["location"]);
        }

        // set features
        if (!data["features"].is_null() && data["features"].is_array() && data["features"].size() > 0)
        {
            auto features = Features::List();

            for (auto i = 0; i < data["features"].size(); i++)
            {
                auto feature = !data["features"][i].is_null() ? Feature::Map(std::string(data["features"][i])) : Feature::Type::NONE;

                features.push_back(feature);
            }

            section.Features = features;
        }

        // read choices
        if (!data["choices"].is_null() && data["choices"].is_array() && data["choices"].size() > 0)
        {
            auto choices = Choice::List();

            for (auto i = 0; i < data["choices"].size(); i++)
            {
                auto choice = Choice::Parse(data["choices"][i]);

                choices.push_back(choice);
            }

            section.Choices = choices;
        }

        // load battle
        if (!data["battle"].is_null())
        {
            if (data["battle"].is_object())
            {
                section.Battle.Initialize(data["battle"]);
            }
            else if (data["battle"].is_string())
            {
                section.Battle = Battle::Load(std::string(data["battle"]).c_str());
            }
        }

        // image asset and position
        section.ImagePosition = !data["image_position"].is_null() ? Position::Map(data["image_position"]) : Position::Type::NONE;

        section.ImageAsset = !data["image_asset"].is_null() ? std::string(data["image_asset"]) : std::string();

        // story section text
        section.Text = !data["text"].is_null() ? std::string(data["text"]) : std::string();

        // load background events conditions
        if (!data["background"].is_null() && data["background"].is_array() && data["background"].size() > 0)
        {
            auto background = Conditions::List();

            for (auto i = 0; i < data["background"].size(); i++)
            {
                auto condition = Conditions::Parse(data["background"][i]);

                background.push_back(condition);
            }

            section.Background = background;
        }

        // load real-time/run-once events conditions
        if (!data["events"].is_null() && data["events"].is_array() && data["events"].size() > 0)
        {
            auto events = Conditions::List();

            for (auto i = 0; i < data["events"].size(); i++)
            {
                auto condition = Conditions::Parse(data["events"][i]);

                events.push_back(condition);
            }

            section.Events = events;
        }

        // load "next section" conditions
        if (!data["next"].is_null() && data["next"].is_array() && data["next"].size() > 0)
        {
            auto conditions = Conditions::List();

            for (auto i = 0; i < data["next"].size(); i++)
            {
                auto condition = Conditions::Parse(data["next"][i]);

                conditions.push_back(condition);
            }

            section.Next = conditions;
        }

        if (!data["items"].is_null() && data["items"].is_array() && data["items"].size() > 0)
        {
            section.Items = Items::Load(data["items"]);
        }

        return section;
    }

    Section::Base Load(const char *filename)
    {
        auto section = Section::Base();

        std::ifstream ifs(filename);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data.is_null())
            {
                section = Section::Load(data);
            }

            ifs.close();
        }

        return section;
    }
}

namespace BloodSword::Story
{
    typedef std::vector<Section::Base> Sections;

    class Base
    {
    public:
        std::string Title = std::string();

        std::string Description = std::string();

        Story::Sections Sections = Story::Sections();

        Base() {}

        int Find(Book::Location location)
        {
            auto section = -1;

            for (auto i = 0; i < this->Sections.size(); i++)
            {
                if (Book::Equal(location, this->Sections[i].Location))
                {
                    section = i;

                    break;
                }
            }

            return section;
        }
    };

    // global "book" variable to save on passing by reference all the time
    Story::Base CurrentBook = Story::Base();

    void Load(const char *filename)
    {
        // re-initalize
        Story::CurrentBook = Story::Base();

        // make a reference
        auto &story = Story::CurrentBook;

        std::ifstream ifs(filename);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data.is_null() && data.is_object())
            {
                story.Title = !data["title"].is_null() ? std::string(data["title"]) : "Blood Sword";

                story.Description = !data["description"].is_null() ? std::string(data["description"]) : "Blood Sword gamebook";

                if (!data["sections"].is_null() && data["sections"].is_array() && data["sections"].size() > 0)
                {
                    auto sections = Story::Sections();

                    if (!data["sections"].is_null() && data["sections"].is_array() && data["sections"].size() > 0)
                    {
                        for (auto i = 0; i < data["sections"].size(); i++)
                        {
                            auto section = Section::Base();

                            if (!data["sections"][i].is_null() && data["sections"][i].is_object())
                            {
                                section = Section::Load(data["sections"][i]);
                            }
                            else if (!data["sections"][i].is_null() && data["sections"][i].is_string())
                            {
                                section = Section::Load(std::string(data["sections"][i]).c_str());
                            }

                            if (Book::IsDefined(section.Location))
                            {
                                sections.push_back(section);
                            }
                        }
                    }

                    if (sections.size() > 0)
                    {
                        story.Sections = sections;
                    }
                }
            }

            ifs.close();
        }

        std::cerr << "[LOADED] " << story.Sections.size() << " SECTIONS" << std::endl;
    }
}
#endif
