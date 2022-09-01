#ifndef __MAP_OBJECTS_HPP__
#define __MAP_OBJECTS_HPP__

#include <map>
#include <string>

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
        {Object::ENEMY_PASSABLE, "ENEMY_PASSABLE"},
        {Object::OBSTACLE, "OBSTACLE"},
        {Object::TEMPORARY_OBSTACLE, "TEMPORARY_OBSTACLE"},
        {Object::EXIT, "EXIT"}};

    Map::Object GetObject(const char *object)
    {
        auto result = Map::Object::NONE;

        auto objects = Map::ObjectMapping.begin();

        while (objects != Map::ObjectMapping.end())
        {
            if (std::strcmp(objects->second, object) == 0)
            {
                result = objects->first;

                break;
            }

            objects++;
        }

        return result;
    }

    Map::Object GetObject(std::string object)
    {
        return Map::GetObject(object.c_str());
    }
}

#endif
