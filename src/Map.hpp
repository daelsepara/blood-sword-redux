#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <vector>

#include "Asset.hpp"

namespace BloodSword::Map
{
    typedef std::pair<int, int> Point;

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

    class Tile
    {
    public:
        Map::Object Type = Map::Object::None;

        Map::Object Occupant = Map::Object::None;

        Asset::Type Asset = Asset::Type::NONE;

        int Lifetime = -1;

        int Id = 0;

        bool IsEnemy()
        {
            return this->Occupant == Map::Object::Enemy;
        }

        bool IsPlayer()
        {
            return this->Occupant == Map::Object::Player;
        }

        bool IsExit()
        {
            return (this->Type == Map::Object::Exit);
        }

        bool IsBlocked()
        {
            return (this->Type == Map::Object::Obstacle || (this->Occupant == Map::Object::TemporaryObstacle && this->Lifetime > 0));
        }

        bool IsTemporarilyBlocked()
        {
            return (this->Occupant == Map::Object::TemporaryObstacle && this->Lifetime > 0);
        }

        bool IsOccupied()
        {
            return this->Occupant != Map::Object::None;
        }

        bool IsPassable()
        {
            return this->Occupant == Map::Object::None && this->Type == Map::Object::Passable;
        }

        bool IsPassableToEnemy()
        {
            return this->Occupant == Map::Object::None && (this->Type == Map::Object::Passable || this->Type == Map::Object::EnemyPassable);
        }
    };

    class Base
    {
    public:
        // map tiles
        std::vector<std::vector<Map::Tile>> Tiles = {};

        // dimensions (number of tiles)
        int SizeX = 0;

        int SizeY = 0;

        // offsets (navigation)
        int MapX = 0;

        int MapY = 0;

        // Dimensions (size in pixels)
        int Width = 0;

        int Height = 0;

        // offsets (pixels, drawing)
        int DrawX = 0;

        int DrawY = 0;
    };
}

#endif