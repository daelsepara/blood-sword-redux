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
    typedef std::vector<Character::Base> List;

    class Base
    {
    private:
        // members in the party
        Party::List Members = {};

    public:
        // current book and section number
        Book::Location Location = Book::Undefined;

        Book::Location PreviousLocation = Book::Undefined;

        // for choices / conditions requiring a selected character
        Character::Class ChosenCharacter = Character::Class::NONE;

        // for choices / conditions requiring the result of the last battle
        Battle::Result LastBattle = Battle::Result::NONE;

        // location of last battle
        Book::Location BattleLocation = Book::Undefined;

        // for choices / conditions requiring a chosen number (1-6)
        int ChosenNumber = -1;

        // holds survivors from previous battles
        Party::List Survivors = {};

        // for the magus kalugen's card game
        Items::Deck Cards = {};

        Base() {}

        Base(Party::List members) : Members(members) {}

        Base(Character::Base character) : Members(Party::List({character})) {}

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
                throw std::invalid_argument("CHARACTER NOT IN PARTY!");
            }

            return this->Members[index];
        }

        // check if a specific character class is present in party
        Party::List::const_iterator Find(Character::Class character_class)
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

        // find index of party member that is a valid target
        int Find(Target::Type target)
        {
            auto found = -1;

            for (auto i = 0; i < this->Members.size(); i++)
            {
                if (this->Members[i].Target == target)
                {
                    found = i;

                    break;
                }
            }

            return found;
        }

        // check if anyone in the party has this status
        int Find(Character::Status status)
        {
            auto found = -1;

            for (auto i = 0; i < this->Members.size(); i++)
            {
                if (this->Members[i].Has(status))
                {
                    found = i;

                    break;
                }
            }

            return found;
        }

        // check if party has valid target
        bool Has(Target::Type target)
        {
            return (this->Find(target) != -1);
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

        bool Has(Character::Status status)
        {
            return (this->Find(status) != -1);
        }

        bool Has(Item::CardType card)
        {
            return BloodSword::Found(this->Cards, card);
        }

        // access party by character class
        Character::Base &operator[](Character::Class character_class)
        {
            if (!this->Has(character_class))
            {
                throw std::invalid_argument("CHARACTER NOT IN PARTY!");
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

        void Set(Book::Location location)
        {
            this->PreviousLocation = this->Location;

            this->Location = location;

            for (auto &member : this->Members)
            {
                member.Location = location;
            }
        }

        // remove character from party (based on character class)
        void Remove(Character::Class character_class)
        {
            auto found = -1;

            if (!this->Has(character_class))
            {
                throw std::invalid_argument("CHARACTER NOT IN PARTY!");
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

        // add status to entire party
        void Add(Character::Status status)
        {
            for (auto i = 0; i < this->Count(); i++)
            {
                this->Members[i].Add(status);
            }
        }

        // remove status from the entire party
        void Remove(Character::Status status)
        {
            for (auto i = 0; i < this->Count(); i++)
            {
                this->Members[i].Remove(status);
            }
        }

        void Remove(std::vector<Character::Status> statuses)
        {
            for (auto i = 0; i < this->Count(); i++)
            {
                for (auto status : statuses)
                {
                    this->Members[i].Remove(status);
                }
            }
        }

        void ResetSpells()
        {
            for (auto i = 0; i < this->Count(); i++)
            {
                if (this->Members[i].Has(Skills::Type::SPELLS))
                {
                    this->Members[i].ResetSpellComplexities();
                }
            }
        }

        // Load party from json data
        void Load(nlohmann::json &data)
        {
            // set party location
            if (!data["location"].is_null())
            {
                this->Location = Book::Load(data["location"]);
            }

            // set party previous location
            if (!data["previous_location"].is_null())
            {
                this->PreviousLocation = Book::Load(data["previous_location"]);
            }

            this->ChosenNumber = !data["chosen_number"].is_null() ? int(data["chosen_number"]) : -1;

            this->ChosenCharacter = !data["chosen_character"].is_null() ? Character::Map(std::string(data["chosen_character"])) : Character::Class::NONE;

            this->LastBattle = !data["last_battle"].is_null() ? Battle::MapResult(std::string(data["last_battle"])) : Battle::Result::NONE;

            if (!data["battle_location"].is_null())
            {
                this->BattleLocation = Book::Load(data["battle_location"]);
            }

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

            // load deck of cards
            if (!data["cards"].is_null() && data["cards"].is_object())
            {
                this->Cards = Items::LoadDeck(data["cards"]);
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
            data["location"] = Book::Data(party.Location);
        }

        if (Book::IsDefined(party.PreviousLocation))
        {
            data["previous_location"] = Book::Data(party.PreviousLocation);
        }

        if (party.LastBattle != Battle::Result::NONE)
        {
            data["last_battle"] = Battle::ResultMapping[party.LastBattle];
        }

        if (Book::IsDefined(party.BattleLocation))
        {
            data["battle_location"] = Book::Data(party.BattleLocation);
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

        if (party.Cards.size() > 0)
        {
            data["cards"] = Items::DeckData(party.Cards);
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
