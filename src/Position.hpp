#ifndef __POSITION_HPP__
#define __POSITION_HPP__

#include "Primitives.hpp"
#include "Templates.hpp"

namespace BloodSword::Positions
{
    BloodSword::Mapping<BloodSword::Position> Mapping = {
        {Position::NONE, "NONE"},
        {Position::TOP, "TOP"},
        {Position::LEFT, "LEFT"},
        {Position::RIGHT, "RIGHT"},
        {Position::BOTTOM, "BOTTOM"}};

    BloodSword::Position Map(const char *position)
    {
        return BloodSword::Find(Positions::Mapping, position);
    }

    BloodSword::Position Map(std::string position)
    {
        return Positions::Map(position.c_str());
    }
}

#endif
