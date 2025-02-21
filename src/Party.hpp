#ifndef __PARTY_HPP__
#define __PARTY_HPP__

#include <stdexcept>
#include <vector>
#include <fstream>

#include "nlohmann/json.hpp"
#include "BattleResults.hpp"
#include "Book.hpp"
#include "Character.hpp"

namespace BloodSword::Party
{
    class Base
    {
    private:
        // members in the party
        std::vector<Character::Base> Members = {};

    public:
        // current book and section number
        Book::Location Location = {Book::Number::NONE, -1};

        Book::Location PreviousLocation = {Book::Number::NONE, -1};

        // for choices / conditions requiring a selected character
        Character::Class ChosenCharacter = Character::Class::NONE;

        // for choices / conditions requiring the result of the last battle
        Battle::Result LastBattle = Battle::Result::NONE;

        // for choices / conditions requiring a chosen number (1-6)
        int ChosenNumber = -1;

        // holds survivors from previous battles
        std::vector<Character::Base> Survivors = {};

        Base() {}

        Base(std::vector<Character::Base> members) : Members(members) {}

        Base(Character::Base character) : Members(std::vector<Character::Base>({character})) {}

        // add character to party
        void Add(Character::Base &character)
        {
            this->Members.push_back(character);
        }

        // number of characters in the party
        int Count()
        {
            return int(this->Members.size());
        }

        // access party member by index
        Character::Base &operator[](int index)
        {
            if (index < 0 || index > this->Count() - 1)
            {
                throw std::invalid_argument("Character not in party!");
            }

            return this->Members[index];
        }

        // check if a specific character class is present in party
        std::vector<Character::Base>::const_iterator Find(Character::Class character_class)
        {
            auto found = this->Members.end();

            for (auto character = this->Members.begin(); character != this->Members.end(); character++)
            {
                if (character->Class == character_class)
                {
                    found = character;

                    break;
                }
            }

            return found;
        }

        // check if party has a member of a specific character class
        bool Has(Character::Class character_class)
        {
            return this->Find(character_class) != this->Members.end();
        }

        // check if party has an item of type
        bool Has(Item::Type item)
        {
            auto result = false;

            for (auto character = this->Members.begin(); character != this->Members.end(); character++)
            {
                if (character->Has(item))
                {
                    result = true;

                    break;
                }
            }

            return result;
        }

        // access party by character class
        Character::Base &operator[](Character::Class character_class)
        {
            if (!this->Has(character_class))
            {
                throw std::invalid_argument("Character not in party!");
            }

            return (*this)[this->Find(character_class) - this->Members.begin()];
        }

        // remove character from party (based on index)
        void Remove(int index)
        {
            if (index >= 0 && index < this->Count())
            {
                this->Members.erase(this->Members.begin() + index);
            }
        }

        void Clear()
        {
            this->Members.clear();
        }

        // remove character from party (based on character class)
        void Remove(Character::Class character_class)
        {
            auto found = -1;

            if (!this->Has(character_class))
            {
                throw std::invalid_argument("Character not in party!");
            }

            for (auto i = 0; i < this->Count(); i++)
            {
                auto member = (*this)[i];

                if (member.Class == character_class)
                {
                    found = i;

                    break;
                }
            }

            if (found >= 0 && found < this->Count())
            {
                this->Remove(found);
            }
        }

        // Load party from json data
        void Load(nlohmann::json &data)
        {
            // set party location
            if (!data["location"].is_null())
            {
                auto book = !data["location"]["book"].is_null() ? Book::MapBook(std::string(data["location"]["book"])) : Book::Number::NONE;

                auto number = !data["location"]["number"].is_null() ? int(data["location"]["number"]) : -1;

                this->Location = {book, number};
            }

            // set party previous location
            if (!data["previous_location"].is_null())
            {
                auto book = !data["previous_location"]["book"].is_null() ? Book::MapBook(std::string(data["previous_location"]["book"])) : Book::Number::NONE;

                auto number = !data["previous_location"]["number"].is_null() ? int(data["previous_location"]["number"]) : -1;

                this->PreviousLocation = {book, number};
            }

            this->ChosenNumber = !data["chosen_number"].is_null() ? int(data["chosen_number"]) : -1;

            this->ChosenCharacter = !data["chosen_character"].is_null() ? Character::Map(std::string(data["chosen_character"])) : Character::Class::NONE;

            this->LastBattle = !data["last_battle"].is_null() ? Battle::MapResult(std::string(data["last_battle"])) : Battle::Result::NONE;

            // load party
            this->Clear();

            if (!data["members"].is_null() && data["members"].is_array() && data["members"].size() > 0)
            {
                for (auto i = 0; i < data["members"].size(); i++)
                {
                    auto character = Character::Load(data["members"][i]);

                    this->Add(character);
                }
            }

            // load survivors
            this->Survivors.clear();

            if (!data["survivors"].is_null() && data["survivors"].is_array() && data["survivors"].size() > 0)
            {
                for (auto i = 0; i < data["survivors"].size(); i++)
                {
                    auto character = Character::Load(data["survivors"][i]);

                    this->Survivors.push_back(character);
                }
            }
        }
    };

    Party::Base Initialize(nlohmann::json &data)
    {
        auto party = Party::Base();

        if (!data.is_null())
        {
            party.Load(data);
        }

        return party;
    }

    nlohmann::json Data(Party::Base &party)
    {
        nlohmann::json data;

        if (Book::IsDefined(party.Location))
        {
            auto book = Book::Mapping[party.Location.first];

            auto number = int(party.Location.second);

            nlohmann::json location;

            location["book"] = book;

            location["number"] = number;

            data["location"] = location;
        }

        if (Book::IsDefined(party.PreviousLocation))
        {
            auto book = Book::Mapping[party.PreviousLocation.first];

            auto number = int(party.PreviousLocation.second);

            nlohmann::json location;

            location["book"] = book;

            location["number"] = number;

            data["previous_location"] = location;
        }

        if (party.LastBattle != Battle::Result::NONE)
        {
            data["last_battle"] = Battle::ResultMapping[party.LastBattle];
        }

        if (party.ChosenCharacter != Character::Class::NONE)
        {
            data["chosen_character"] = Character::ClassMapping[party.ChosenCharacter];
        }

        if (party.ChosenNumber != -1)
        {
            data["chosen_number"] = party.ChosenNumber;
        }

        if (party.Count() > 0)
        {
            nlohmann::json members;

            for (auto character = 0; character < party.Count(); character++)
            {
                auto character_data = Character::Data(party[character]);

                members.push_back(character_data);
            }

            data["members"] = members;
        }

        return data;
    }

    void Save(Party::Base &party, const char *filename, const char *name)
    {
        nlohmann::json data;

        data.emplace(std::string(name), Party::Data(party));

        std::ofstream file(filename);

        if (file.is_open())
        {
            file << data.dump();

            file.close();
        }
    }

    Party::Base Load(const char *filename, const char *name)
    {
        auto party = Party::Base();

        std::ifstream file(filename);

        if (file.good())
        {
            auto data = nlohmann::json::parse(file);

            party = Party::Initialize(data[std::string(name)]);

            file.close();
        }

        return party;
    }
}

#endif
