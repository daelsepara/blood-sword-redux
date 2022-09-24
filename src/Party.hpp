#ifndef __PARTY_HPP__
#define __PARTY_HPP__

#include <stdexcept>
#include <vector>

#include "Character.hpp"

namespace BloodSword::Party
{
    class Base
    {
    public:
        std::vector<Character::Base> Members = {};

        Base() {}

        Base(std::vector<Character::Base> members) : Members(members) {}

        Base(Character::Base character) : Members(std::vector<Character::Base>({character})) {}

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

        int Find(Character::Class characterClass)
        {
            auto found = -1;

            if (this->Count() > 0)
            {
                for (auto i = 0; i < this->Count(); i++)
                {
                    if ((*this)[i].Class == characterClass)
                    {
                        found = i;

                        break;
                    }
                }
            }

            return found;
        }

        bool Has(Character::Class characterClass)
        {
            auto found = this->Find(characterClass);

            return found >= 0 && found < this->Count();
        }

        Character::Base &operator[](Character::Class characterClass)
        {
            if (!this->Has(characterClass))
            {
                throw std::invalid_argument("Character not in party!");
            }

            return (*this)[this->Find(characterClass)];
        }
    };
}

#endif
