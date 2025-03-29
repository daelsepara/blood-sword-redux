#ifndef __MAP_OBJECTS_HPP__
#define __MAP_OBJECTS_HPP__

#include "Templates.hpp"

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
        EXIT
    };

    BloodSword::Mapping<Map::Object> ObjectMapping = {
        {Map::Object::NONE, "NONE"},
        {Map::Object::PLAYER, "PLAYER"},
        {Map::Object::ENEMY, "ENEMY"},
        {Map::Object::PASSABLE, "PASSABLE"},
        {Map::Object::ENEMY_PASSABLE, "ENEMY PASSABLE"},
        {Map::Object::OBSTACLE, "OBSTACLE"},
        {Map::Object::TEMPORARY_OBSTACLE, "TEMPORARY OBSTACLE"},
        {Map::Object::EXIT, "EXIT"}};

    Map::Object MapObject(const char *object)
    {
        return BloodSword::Find(Map::ObjectMapping, object);
    }

    Map::Object MapObject(std::string object)
    {
        return Map::MapObject(object.c_str());
    }
}

#endif
