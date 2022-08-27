#ifndef __MAP_OBJECTS_HPP__
#define __MAP_OBJECTS_HPP__

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
}

#endif