#ifndef __POSITION_HPP__
#define __POSITION_HPP__

#include "Primitives.hpp"
#include "Templates.hpp"

// functions for managing relative positions
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

    // mapping of position types to strings
    BloodSword::StringMap<Position::Type> TypeMapping = {
        {Position::Type::NONE, "NONE"},
        {Position::Type::TOP, "TOP"},
        {Position::Type::LEFT, "LEFT"},
        {Position::Type::RIGHT, "RIGHT"},
        {Position::Type::BOTTOM, "BOTTOM"}};

    // map string to position type
    Position::Type Map(const char *position)
    {
        return BloodSword::Find(Position::TypeMapping, position);
    }

    // map string to position type
    Position::Type Map(std::string position)
    {
        return Position::Map(position.c_str());
    }
}

#endif
