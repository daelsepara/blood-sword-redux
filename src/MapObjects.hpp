#ifndef __MAP_OBJECTS_HPP__
#define __MAP_OBJECTS_HPP__

#include "Templates.hpp"

// map objects and their string representations
namespace BloodSword::Map
{
    // the tile's (or its occupant) type
    enum class Object
    {
        NONE = -1,
        PLAYER,
        ENEMY,
        PASSABLE,
        ENEMY_PASSABLE,
        OBSTACLE,
        TEMPORARY_OBSTACLE,
        EXIT,
        ITEM,
        ITEMS,
        PARTY,
        ENEMIES,
        TRIGGER
    };

    // mapping of map object types to strings
    BloodSword::ConstStrings<Object> ObjectMapping = {
        {Object::NONE, "NONE"},
        {Object::PLAYER, "PLAYER"},
        {Object::ENEMY, "ENEMY"},
        {Object::PASSABLE, "PASSABLE"},
        {Object::ENEMY_PASSABLE, "ENEMY PASSABLE"},
        {Object::OBSTACLE, "OBSTACLE"},
        {Object::TEMPORARY_OBSTACLE, "TEMPORARY OBSTACLE"},
        {Object::EXIT, "EXIT"},
        {Object::ITEM, "ITEM"},
        {Object::ITEMS, "ITEMS"},
        {Object::PARTY, "PARTY"},
        {Object::ENEMIES, "ENEMIES"},
        {Object::TRIGGER, "TRIGGER"}};

    // get map object type from string
    Object MapObject(const char *object)
    {
        return BloodSword::Find(Map::ObjectMapping, object);
    }

    // get map object type from string
    Object MapObject(std::string object)
    {
        return Map::MapObject(object.c_str());
    }
}

#endif
