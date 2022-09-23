#ifndef __PARTY_HPP__
#define __PARTY_HPP__

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
    };
}

#endif
