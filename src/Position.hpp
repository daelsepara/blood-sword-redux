#ifndef __POSITION_HPP__
#define __POSITION_HPP__

#include "Primitives.hpp"
#include "Templates.hpp"

namespace BloodSword
{
    BloodSword::Mapping<BloodSword::Position> PositionMapping = {
        {Position::NONE, "NONE"},
        {Position::TOP, "TOP"},
        {Position::LEFT, "LEFT"},
        {Position::RIGHT, "RIGHT"},
        {Position::BOTTOM, "BOTTOM"}};

    BloodSword::Position MapPosition(const char *position)
    {
        return BloodSword::Find(BloodSword::PositionMapping, position);
    }

    BloodSword::Position MapPosition(std::string position)
    {
        return BloodSword::MapPosition(position.c_str());
    }
}

#endif
