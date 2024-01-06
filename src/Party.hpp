#ifndef __PARTY_HPP__
#define __PARTY_HPP__

#include <stdexcept>
#include <vector>

#include "Book.hpp"
#include "Character.hpp"

namespace BloodSword::Battle
{
    enum class Result
    {
        NONE = -1,
        DETERMINE,
        DEFEAT,
        VICTORY,
        FLEE,
        ENTHRALLED
    };
}

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
            return (int)(this->Members.size());
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
        std::vector<Character::Base>::const_iterator Find(Character::Class characterClass)
        {
            auto found = this->Members.end();

            for (auto character = this->Members.begin(); character != this->Members.end(); character++)
            {
                if (character->Class == characterClass)
                {
                    found = character;

                    break;
                }
            }

            return found;
        }

        // check if party has a member of a specific character class
        bool Has(Character::Class characterClass)
        {
            return this->Find(characterClass) != this->Members.end();
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
        Character::Base &operator[](Character::Class characterClass)
        {
            if (!this->Has(characterClass))
            {
                throw std::invalid_argument("Character not in party!");
            }

            return (*this)[this->Find(characterClass) - this->Members.begin()];
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
        void Remove(Character::Class characterClass)
        {
            auto found = -1;

            if (!this->Has(characterClass))
            {
                throw std::invalid_argument("Character not in party!");
            }

            for (auto i = 0; i < this->Count(); i++)
            {
                auto member = (*this)[i];

                if (member.Class == characterClass)
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
        void Load(nlohmann::json data)
        {
            if (!data["party"].is_null())
            {
                // set party location
                if (!data["party"]["location"].is_null())
                {
                    auto book = !data["party"]["location"]["book"].is_null() ? Book::MapBook(std::string(data["party"]["location"]["book"])) : Book::Number::NONE;

                    auto number = !data["party"]["location"]["number"].is_null() ? std::stoi(std::string(data["party"]["location"]["number"])) : -1;

                    this->Location = {book, number};
                }

                // load party members
                if (!data["party"]["members"].is_null() && data["party"]["members"].is_array() && data["party"]["members"].size() > 0)
                {
                    for (auto i = 0; i < (int)(data["party"]["members"].size()); i++)
                    {
                        auto character = Character::Load(data["party"]["members"][i]);

                        this->Add(character);
                    }
                }
            }
        }
    };

    Party::Base Load(nlohmann::json data)
    {
        auto party = Party::Base();

        party.Load(data);

        return party;
    }
}

#endif
