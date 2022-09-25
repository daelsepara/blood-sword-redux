#ifndef __PARTY_HPP__
#define __PARTY_HPP__

#include <stdexcept>
#include <vector>

#include "Book.hpp"
#include "Character.hpp"

namespace BloodSword::Party
{
    class Base
    {
    private:
        std::vector<Character::Base> Members = {};

    public:
        Book::Destination Location = {Book::Number::None, -1};

        Base() {}

        Base(std::vector<Character::Base> members) : Members(members) {}

        Base(Character::Base character) : Members(std::vector<Character::Base>({character})) {}

        void Add(Character::Base &character)
        {
            this->Members.push_back(character);
        }

        int Count()
        {
            return (int)(this->Members.size());
        }

        Character::Base &operator[](int index)
        {
            if (index < 0 || index > this->Count() - 1)
            {
                throw std::invalid_argument("Character not in party!");
            }

            return this->Members[index];
        }

        std::vector<Character::Base>::const_iterator Find(Character::Class characterClass)
        {
            auto found = this->Members.end();

            for (auto character = this->Members.begin(); character != this->Members.end(); character++)
            {
                if ((*character).Class == characterClass)
                {
                    found = character;

                    break;
                }
            }

            return found;
        }

        bool Has(Character::Class characterClass)
        {
            return this->Find(characterClass) != this->Members.end();
        }

        Character::Base &operator[](Character::Class characterClass)
        {
            if (!this->Has(characterClass))
            {
                throw std::invalid_argument("Character not in party!");
            }

            return (*this)[this->Find(characterClass) - this->Members.begin()];
        }
    };
}

#endif
