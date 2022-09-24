#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <vector>

#include "Asset.hpp"
#include "MapObjects.hpp"

namespace BloodSword::Map
{
    // Define neighbors (X, Y): Up, Down, Left, Right
    const std::vector<Point> Directions = {Point(0, -1), Point(1, 0), Point(0, 1), Point(-1, 0)};

    class Tile
    {
    public:
        Map::Object Type = Map::Object::NONE;

        Map::Object Occupant = Map::Object::NONE;

        Asset::Type Asset = Asset::Type::NONE;

        Asset::Type TemporaryAsset = Asset::Type::NONE;

        int Lifetime = -1;

        int Id = -1;

        bool IsEnemy()
        {
            return this->Occupant == Map::Object::ENEMY;
        }

        bool IsPlayer()
        {
            return this->Occupant == Map::Object::PLAYER;
        }

        bool IsExit()
        {
            return (this->Type == Map::Object::EXIT);
        }

        bool IsTemporarilyBlocked()
        {
            return (this->Occupant == Map::Object::TEMPORARY_OBSTACLE && this->Lifetime > 0);
        }

        bool IsBlocked()
        {
            return (this->Type == Map::Object::OBSTACLE || this->IsTemporarilyBlocked());
        }

        bool IsOccupied()
        {
            return this->Occupant != Map::Object::NONE;
        }

        bool IsPassable()
        {
            return this->Occupant == Map::Object::NONE && this->Type == Map::Object::PASSABLE;
        }

        bool IsPassableToEnemy()
        {
            return this->Occupant == Map::Object::NONE && (this->Type == Map::Object::PASSABLE || this->Type == Map::Object::ENEMY_PASSABLE);
        }
    };

    class Base
    {
    public:
        // map tiles
        Array<Map::Tile> Tiles = {};

        // dimensions (size in number of tiles)
        int Width = 0;

        int Height = 0;

        // viewable size (number of tiles)
        int SizeX = 8;

        int SizeY = 8;

        // offsets (navigation)
        int MapX = 0;

        int MapY = 0;

        // offsets (pixels, drawing)
        int DrawX = 64;

        int DrawY = 64;

        // size of each tile (pixels)
        int TileSize = 64;

        void Initialize(int width, int height)
        {
            this->Width = width;

            this->Height = height;

            this->Tiles.clear();

            this->Tiles.resize(height);

            for (auto i = 0; i < height; i++)
            {
                this->Tiles[i] = std::vector<Map::Tile>(width);
            }
        }

        void Viewable(int sizex, int sizey)
        {
            this->SizeX = sizex;

            this->SizeY = sizey;
        }

        Base(int sizex, int sizey)
        {
            Initialize(sizex, sizey);
        }

        Base() {}

        Map::Tile &operator()(int x, int y)
        {
            return this->Tiles[y][x];
        }

        Map::Tile &operator()(const Point &point)
        {
            return (*this)(point.X, point.Y);
        }

        Map::Tile &operator[](const Point &point)
        {
            return (*this)(point);
        }

        bool IsValid(Point coords)
        {
            return coords.X >= 0 && coords.Y >= 0 && coords.X < this->Width && coords.Y < this->Height;
        }

        bool IsValid(int x, int y)
        {
            return this->IsValid(Point(x, y));
        }

        void Put(int x, int y, Map::Object object, int id)
        {
            if (this->IsValid(x, y))
            {
                (*this)(x, y).Occupant = object;
                (*this)(x, y).Id = id;
            }
        }

        void Put(int x, int y, Map::Object type, Asset::Type asset)
        {
            if (this->IsValid(x, y))
            {
                (*this)(x, y).Type = type;
                (*this)(x, y).Asset = asset;
            }
        }

        void Put(Point location, Map::Object object, int id)
        {
            this->Put(location.X, location.Y, object, id);
        }

        void Put(Point location, Map::Object type, Asset::Type asset)
        {
            this->Put(location.X, location.Y, type, asset);
        }

        // load from json file
        bool Load(const char *filename)
        {
            auto LoadError = false;

            std::ifstream file(filename);

            if (file.good())
            {
                auto data = nlohmann::json::parse(file);

                this->Width = !data["width"].is_null() ? (int)data["width"] : 0;
                this->Height = !data["height"].is_null() ? (int)data["height"] : 0;
                this->TileSize = !data["tilesize"].is_null() ? (int)data["tilesize"] : 0;

                LoadError = false;

                if (this->Width > 0 && this->Height > 0)
                {
                    this->Initialize(this->Width, this->Height);

                    if (!data["tiles"].is_null() && data["tiles"].is_array() && data["tiles"].size() == this->Height)
                    {
                        for (auto y = 0; y < this->Height; y++)
                        {
                            if (!data["tiles"][y].is_null() && data["tiles"][y].is_array() && data["tiles"][y].size() == this->Width)
                            {
                                for (auto x = 0; x < this->Width; x++)
                                {
                                    (*this)(x, y).Id = !data["tiles"][y][x]["id"].is_null() ? (int)data["tiles"][y][x]["id"] : -1;
                                    (*this)(x, y).Type = !data["tiles"][y][x]["type"].is_null() ? Map::MapObject(std::string(data["tiles"][y][x]["type"])) : Map::Object::NONE;
                                    (*this)(x, y).Occupant = !data["tiles"][y][x]["occupant"].is_null() ? Map::MapObject(std::string(data["tiles"][y][x]["occupant"])) : Map::Object::NONE;
                                    (*this)(x, y).Asset = !data["tiles"][y][x]["asset"].is_null() ? Asset::Map(std::string(data["tiles"][y][x]["asset"])) : Asset::Type::NONE;
                                    (*this)(x, y).TemporaryAsset = !data["tiles"][y][x]["temporary_asset"].is_null() ? Asset::Map(std::string(data["tiles"][y][x]["temporary_asset"])) : Asset::Type::NONE;
                                    (*this)(x, y).Lifetime = !data["tiles"][y][x]["lifetime"].is_null() ? (int)data["tiles"][y][x]["lifetime"] : -1;
                                }
                            }
                            else
                            {
                                LoadError = true;

                                break;
                            }
                        }
                    }
                    else
                    {
                        LoadError = true;
                    }
                }
                else
                {
                    LoadError = true;
                }

                file.close();
            }
            else
            {
                LoadError = true;
            }

            return !LoadError;
        }

        // find object
        Point Find(Map::Object object)
        {
            auto point = Point(-1, -1);

            for (auto y = 0; y < this->Height; y++)
            {
                for (auto x = 0; x < this->Width; x++)
                {
                    if ((*this)(x, y).Type == object)
                    {
                        point.X = x;

                        point.Y = y;

                        break;
                    }
                }
            }

            return point;
        }

        // find occupant
        Point Find(Map::Object occupant, int id)
        {
            auto point = Point(-1, -1);

            for (auto y = 0; y < this->Height; y++)
            {
                for (auto x = 0; x < this->Width; x++)
                {
                    if ((*this)(x, y).Occupant == occupant && (*this)(x, y).Id == id)
                    {
                        point.X = x;

                        point.Y = y;

                        break;
                    }
                }
            }

            return point;
        }
    };
}

#endif
