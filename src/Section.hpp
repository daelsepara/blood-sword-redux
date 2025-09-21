#ifndef __SECTION_HPP__
#define __SECTION_HPP__

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "Choice.hpp"
#include "Feature.hpp"
#include "Position.hpp"
#include "Battle.hpp"
#include "ZipFileLibrary.hpp"

// classes and functions for managing story sections
namespace BloodSword::Section
{
    // section base class
    class Base
    {
    public:
        // book and section number
        Book::Location Location = Book::Undefined;

        // features of the section
        Features::List Features = {};

        // choices available in the section
        Choice::List Choices = {};

        // position of image on screen
        Position::Type ImagePosition = Position::Type::NONE;

        // battle associated with the section
        Battle::Base Battle;

        // image asset file name
        std::string ImageAsset = std::string();

        // story section text
        std::string Text = std::string();

        // background events (if any)
        Conditions::List Background = {};

        // real-time/run-once events (if any)
        Conditions::List Events = {};

        // jumps to next section based on conditions
        Conditions::List Next = {};

        // items found in this section
        Items::Inventory Items = {};

        // events happening before battle
        Conditions::List BattleEvents = {};

        Base() {}

        // check if section has a feature
        bool Has(Feature::Type feature)
        {
            return BloodSword::Has(this->Features, feature);
        }
    };

    // load section from json data
    Section::Base GenerateSection(nlohmann::json &data, const char *zip_file)
    {
        auto is_zip = (zip_file != nullptr);

        // initialize section
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
                if (is_zip)
                {
                    section.Battle = Battle::Load(std::string(data["battle"]).c_str(), zip_file);
                }
                else
                {
                    section.Battle = Battle::Load(std::string(data["battle"]).c_str());
                }
            }
        }

        // image asset and position
        section.ImagePosition = !data["image_position"].is_null() ? Position::Map(data["image_position"]) : Position::Type::NONE;

        // image asset file name
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

        // events happening before battle
        if (!data["battle_events"].is_null() && data["battle_events"].is_array() && data["battle_events"].size() > 0)
        {
            auto battle_events = Conditions::List();

            for (auto i = 0; i < data["battle_events"].size(); i++)
            {
                auto condition = Conditions::Parse(data["battle_events"][i]);

                battle_events.push_back(condition);
            }

            section.BattleEvents = battle_events;
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

        // load items found in section
        if (!data["items"].is_null() && data["items"].is_array() && data["items"].size() > 0)
        {
            section.Items = Items::Load(data["items"]);
        }

        return section;
    }

    Section::Base GenerateSection(nlohmann::json &data)
    {
        return Section::GenerateSection(data, nullptr);
    }

    // load section from file
    Section::Base Load(const char *filename)
    {
        auto section = Section::Base();

        std::ifstream ifs(filename);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data.is_null())
            {
                section = Section::GenerateSection(data);
            }

            ifs.close();
        }

        return section;
    }

    // load section from zip archive
    Section::Base Load(const char *filename, const char *zip_file)
    {
        auto section = Section::Base();

        auto ifs = ZipFile::Read(zip_file, filename);

        if (!ifs.empty())
        {
            auto data = nlohmann::json::parse(ifs);

            Section::GenerateSection(data, zip_file);

            ifs.clear();
        }

        return section;
    }
}

// classes and functions for managing the entire story
namespace BloodSword::Story
{
    // list of sections
    typedef std::vector<Section::Base> Sections;

    // story base class
    class Base
    {
    public:
        // title of the story
        std::string Title = std::string();

        // description
        std::string Description = std::string();

        // sections in the story
        Story::Sections Sections = Story::Sections();

        Base() {}

        // find section by book and section number
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

    // load data from json data (and zip file)
    void GenerateStory(nlohmann::json &data, const char *zip_file)
    {
        auto is_zip = (zip_file != nullptr);

        // re-initalize
        Story::CurrentBook = Story::Base();

        // make a reference
        auto &book = Story::CurrentBook;

        if (!data.is_null() && data.is_object())
        {
            book.Title = !data["title"].is_null() ? std::string(data["title"]) : "Blood Sword";

            book.Description = !data["description"].is_null() ? std::string(data["description"]) : "Blood Sword gamebook";

            if (!data["sections"].is_null() && data["sections"].is_array() && data["sections"].size() > 0)
            {
                auto sections = Story::Sections();

                for (auto i = 0; i < data["sections"].size(); i++)
                {
                    auto section = Section::Base();

                    if (!data["sections"][i].is_null() && data["sections"][i].is_object())
                    {
                        section = Section::GenerateSection(data["sections"][i]);
                    }
                    else if (!data["sections"][i].is_null() && data["sections"][i].is_string())
                    {
                        if (is_zip)
                        {
                            section = Section::Load(std::string(data["sections"][i]).c_str(), zip_file);
                        }
                        else
                        {
                            section = Section::Load(std::string(data["sections"][i]).c_str());
                        }
                    }

                    if (Book::IsDefined(section.Location))
                    {
                        sections.push_back(section);
                    }
                }

                if (sections.size() > 0)
                {
                    book.Sections = sections;
                }
            }
        }

#if defined(DEBUG)
        std::cerr << "[LOADED] " << book.Sections.size() << " SECTIONS" << std::endl;
#endif
    }

    // load story from json data
    void GenerateStory(nlohmann::json &data)
    {
        Story::GenerateStory(data, nullptr);
    }

    // load story from file
    void Load(const char *story)
    {
        std::ifstream ifs(story);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            Story::GenerateStory(data);

            ifs.close();
        }
    }

    // load story from file
    void Load(std::string story)
    {
        Story::Load(story.c_str());
    }

    // load story from zip archive
    void Load(const char *story, const char *zip_file)
    {
        if (zip_file == nullptr)
        {
            Story::Load(story);
        }
        else
        {
            auto ifs = ZipFile::Read(zip_file, story);

            if (!ifs.empty())
            {
                auto data = nlohmann::json::parse(ifs);

                Story::GenerateStory(data, zip_file);

                ifs.clear();
            }
        }
    }

    // load story from file
    void Load(std::string story, std::string zip_file)
    {
        Story::Load(story.c_str(), zip_file.empty() ? nullptr : zip_file.c_str());
    }

    // load ranks from json data
    std::vector<int> InitializeRanks(nlohmann::json &data)
    {
        auto result = std::vector<int>();

        if (!data.is_null() && data.is_object())
        {
            if (!data["ranks"].is_null() && data["ranks"].is_array() && data["ranks"].size() > 0)
            {
                for (auto rank = 0; rank < data["ranks"].size(); rank++)
                {
                    result.push_back(int(data["ranks"][rank]));
                }
            }
        }

        return result;
    }

    // load character ranks from file
    std::vector<int> LoadRanks(const char *ranks)
    {
        auto result = std::vector<int>();

        std::ifstream ifs(ranks);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            result = Story::InitializeRanks(data);

            ifs.close();
        }

        return result;
    }

    // load character ranks from file
    std::vector<int> LoadRanks(std::string ranks)
    {
        return Story::LoadRanks(ranks.c_str());
    }

    // load character ranks from zip archive
    std::vector<int> LoadRanks(const char *ranks, const char *zip_file)
    {
        auto result = std::vector<int>();

        if (zip_file == nullptr)
        {
            result = Story::LoadRanks(ranks);
        }
        else
        {
            auto ifs = ZipFile::Read(zip_file, ranks);

            if (!ifs.empty())
            {
                auto data = nlohmann::json::parse(ifs);

                result = Story::InitializeRanks(data);

                ifs.clear();
            }
        }

        return result;
    }

    // load character ranks from zip archive
    std::vector<int> LoadRanks(std::string ranks, std::string zip_file)
    {
        return Story::LoadRanks(ranks.c_str(), zip_file.empty() ? nullptr : zip_file.c_str());
    }
}

#endif
