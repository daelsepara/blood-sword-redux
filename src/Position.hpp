#ifndef __POSITION_HPP__
#define __POSITION_HPP__

#include "Primitives.hpp"
#include "Templates.hpp"

namespace BloodSword::Position
{
    // relative positions
    enum class Type
    {
        NONE = -1,
        TOP,
        LEFT,
        RIGHT,
        BOTTOM
    };

    BloodSword::Mapping<Position::Type> TypeMapping = {
        {Position::Type::NONE, "NONE"},
        {Position::Type::TOP, "TOP"},
        {Position::Type::LEFT, "LEFT"},
        {Position::Type::RIGHT, "RIGHT"},
        {Position::Type::BOTTOM, "BOTTOM"}};

    Position::Type Map(const char *position)
    {
        return BloodSword::Find(Position::TypeMapping, position);
    }

    Position::Type Map(std::string position)
    {
        return Position::Map(position.c_str());
    }
}

#endif
