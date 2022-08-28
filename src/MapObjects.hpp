#ifndef __MAP_OBJECTS_HPP__
#define __MAP_OBJECTS_HPP__

#include <map>
#include <string>

namespace BloodSword::Map
{
    enum class Object
    {
        None = -1,
        Player,
        Enemy,
        Passable,
        EnemyPassable,
        Obstacle,
        TemporaryObstacle,
        Exit
    };

    std::map<Map::Object, const char *> OjectMapping = {
        {Object::None, "NONE"},
        {Object::Player, "PLAYER"},
        {Object::Enemy, "ENEMY"},
        {Object::Passable, "PASSABLE"},
        {Object::EnemyPassable, "ENEMY_PASSABLE"},
        {Object::Obstacle, "OBSTACLE"},
        {Object::TemporaryObstacle, "TEMPORARY_OBSTACLE"},
        {Object::Exit, "EXIT"},
    };

    Map::Object GetObject(const char *object)
    {
        auto result = Map::Object::None;

        auto objects = Map::OjectMapping.begin();

        while (objects != Map::OjectMapping.end())
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
