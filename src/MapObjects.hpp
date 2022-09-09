#ifndef __MAP_OBJECTS_HPP__
#define __MAP_OBJECTS_HPP__

#include <map>
#include <string>

#include "Templates.hpp"

namespace BloodSword::Map
{
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

    std::map<Map::Object, const char *> ObjectMapping = {
        {Object::NONE, "NONE"},
        {Object::PLAYER, "PLAYER"},
        {Object::ENEMY, "ENEMY"},
        {Object::PASSABLE, "PASSABLE"},
        {Object::ENEMY_PASSABLE, "ENEMY PASSABLE"},
        {Object::OBSTACLE, "OBSTACLE"},
        {Object::TEMPORARY_OBSTACLE, "TEMPORARY OBSTACLE"},
        {Object::EXIT, "EXIT"}};

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
