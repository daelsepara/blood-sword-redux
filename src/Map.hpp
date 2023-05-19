#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <vector>

#include "Asset.hpp"
#include "MapObjects.hpp"

namespace BloodSword::Map
{
    // Define neighbors (X, Y): Up, Down, Left, Right
    const std::vector<Point> Directions = {Point(0, -1), Point(1, 0), Point(0, 1), Point(-1, 0)};

    // a single map tile
    class Tile
    {
    public:
        // tile type classification
        Map::Object Type = Map::Object::NONE;

        // the tile's current occupant
        Map::Object Occupant = Map::Object::NONE;

        // asset type of tile
        Asset::Type Asset = Asset::Type::NONE;

        // asset type of the temporary occupant
        Asset::Type TemporaryAsset = Asset::Type::NONE;

        // lifetime of the temporary occupant, -1 if none
        int Lifetime = -1;

        // Id of the current occupant, -1 if none
        int Id = -1;

        // tile is occupied by an enemy
        bool IsEnemy()
        {
            return this->Occupant == Map::Object::ENEMY;
        }

        // tile is occupied by a player
        bool IsPlayer()
        {
            return this->Occupant == Map::Object::PLAYER;
        }

        // tile is an exit point, e.g. stairs, door
        bool IsExit()
        {
            return (this->Type == Map::Object::EXIT);
        }

        // tile is temporarily blocked
        bool IsTemporarilyBlocked()
        {
            return (this->Occupant == Map::Object::TEMPORARY_OBSTACLE && this->Lifetime > 0);
        }

        // tile is blocked
        bool IsBlocked()
        {
            return (this->Type == Map::Object::OBSTACLE || this->IsTemporarilyBlocked());
        }

        // tile is occupied
        bool IsOccupied()
        {
            return this->Occupant != Map::Object::NONE || this->IsTemporarilyBlocked();
        }

        // tile is not occupied and is traversable
        bool IsPassable()
        {
            return this->Occupant == Map::Object::NONE && this->Type == Map::Object::PASSABLE;
        }

        // tile is traversable by an enemy
        bool IsPassableToEnemy()
        {
            return this->Occupant == Map::Object::NONE && (this->Type == Map::Object::PASSABLE || this->Type == Map::Object::ENEMY_PASSABLE);
        }
    };

    class Base
    {
    private:
        // map tiles
        Array<Map::Tile> Tiles = {};

    public:
        // width (size in number of tiles)
        int Width = 0;

        // height (size in number of tiles)
        int Height = 0;

        // viewable width (number of tiles)
        int ViewX = 8;

        // viewable height (number of tiles)
        int ViewY = 8;

        // x offset (navigation)
        int X = 0;

        // y offset (navigation)
        int Y = 0;

        // x offset (pixels, drawing)
        int DrawX = 64;

        // y offset (pixels, drawing)
        int DrawY = 64;

        // size of each tile (pixels)
        int TileSize = 64;

        // initialize the map
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

        // set viewable region
        void Viewable(int viewx, int viewy)
        {
            this->ViewX = viewx;

            this->ViewY = viewy;
        }

        // initialize the map
        Base(int sizex, int sizey)
        {
            Initialize(sizex, sizey);
        }

        // an empty map
        Base() {}

        // access location on the map
        Map::Tile &operator()(int x, int y)
        {
            return this->Tiles[y][x];
        }

        // access location on the map
        Map::Tile &operator()(const Point &point)
        {
            return (*this)(point.X, point.Y);
        }

        // access location on the map
        Map::Tile &operator[](const Point &point)
        {
            return (*this)(point);
        }

        // check if map is valid
        bool IsValid()
        {
            return this->Width > 0 && this->Height > 0 && this->Tiles.size() > 0;
        }

        // check if a location is within map boundaries
        bool IsValid(Point coords)
        {
            return coords.X >= 0 && coords.Y >= 0 && coords.X < this->Width && coords.Y < this->Height;
        }

        // check if location is visible in current map view
        bool IsVisible(Point coords)
        {
            auto valid = this->IsValid(coords);
            auto viewx = coords.X >= this->X && coords.X < (this->X + this->ViewX);
            auto viewy = coords.Y >= this->Y && coords.Y < (this->Y + this->ViewY);

            return valid && viewx && viewy;
        }

        // check if a location is within map boundaries
        bool IsValid(int x, int y)
        {
            return this->IsValid(Point(x, y));
        }

        // let an object occupy a location on the map
        void Put(Point location, Map::Object object, int id)
        {
            if (this->IsValid(location))
            {
                auto &tile = (*this)[location];

                tile.Occupant = object;

                tile.Id = id;
            }
        }

        // temporary object
        void Put(Point location, Map::Object object, Asset::Type asset, int duration)
        {
            if (this->IsValid(location))
            {
                auto &tile = (*this)[location];

                tile.Occupant = object;

                tile.TemporaryAsset = asset;

                tile.Lifetime = duration;
            }
        }

        // set the location type and asset
        void Put(Point location, Map::Object type, Asset::Type asset)
        {
            if (this->IsValid(location))
            {
                auto &tile = (*this)[location];

                tile.Type = type;

                tile.Asset = asset;
            }
        }

        // let an object occupy a location on the map
        void Put(int x, int y, Map::Object object, int id)
        {
            this->Put(Point(x, y), object, id);
        }

        // set the location type and asset
        void Put(int x, int y, Map::Object type, Asset::Type asset)
        {
            this->Put(Point(x, y), type, asset);
        }

        bool Load(nlohmann::json &data)
        {
            auto LoadError = false;

            this->Width = !data["width"].is_null() ? (int)data["width"] : 0;
            this->Height = !data["height"].is_null() ? (int)data["height"] : 0;
            this->TileSize = !data["tilesize"].is_null() ? (int)data["tilesize"] : 0;

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
                                auto &map = (*this)(x, y);

                                auto &tile = data["tiles"][y][x];

                                map.Id = !tile["id"].is_null() ? (int)tile["id"] : -1;
                                map.Type = !tile["type"].is_null() ? Map::MapObject(std::string(tile["type"])) : Map::Object::NONE;
                                map.Occupant = !tile["occupant"].is_null() ? Map::MapObject(std::string(tile["occupant"])) : Map::Object::NONE;
                                map.Asset = !tile["asset"].is_null() ? Asset::Map(std::string(tile["asset"])) : Asset::Type::NONE;
                                map.TemporaryAsset = !tile["temporary_asset"].is_null() ? Asset::Map(std::string(tile["temporary_asset"])) : Asset::Type::NONE;
                                map.Lifetime = !tile["lifetime"].is_null() ? (int)tile["lifetime"] : -1;
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

            return LoadError;
        }

        // load map from json file
        bool Load(const char *filename)
        {
            auto LoadError = false;

            std::ifstream file(filename);

            if (file.good())
            {
                auto data = nlohmann::json::parse(file);

                LoadError = this->Load(data);
                
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
                    auto test = Point(x, y);

                    auto &tile = (*this)[test];

                    if (tile.Type == object || tile.Occupant == object)
                    {
                        point = test;

                        break;
                    }
                }

                if (!point.IsNone())
                {
                    break;
                }
            }

            return point;
        }

        // find occupant with this id
        Point Find(Map::Object occupant, int id)
        {
            auto point = Point(-1, -1);

            for (auto y = 0; y < this->Height; y++)
            {
                for (auto x = 0; x < this->Width; x++)
                {
                    auto search = Point(x, y);

                    auto &tile = (*this)[search];

                    if (tile.Occupant == occupant && tile.Id == id)
                    {
                        point = search;

                        break;
                    }
                }

                if (!point.IsNone())
                {
                    break;
                }
            }

            return point;
        }

        // find occupant except occupant with id
        Point Except(Map::Object occupant, int id)
        {
            auto point = Point(-1, -1);

            for (auto y = 0; y < this->Height; y++)
            {
                for (auto x = 0; x < this->Width; x++)
                {
                    auto search = Point(x, y);

                    auto &tile = (*this)[search];

                    if (tile.Occupant == occupant && tile.Id != id)
                    {
                        point = search;

                        break;
                    }
                }

                if (!point.IsNone())
                {
                    break;
                }
            }

            return point;
        }

        // computes the manhattan distance between two points on the map
        int Distance(Point src, Point dst)
        {
            auto dist = -1;

            if (this->IsValid(src) && this->IsValid(dst))
            {
                auto path = dst - src;

                dist = std::abs(path.X) + std::abs(path.Y);
            }

            return dist;
        }

        // check if adjacent to a type of occupant
        bool Adjacent(Point src, Map::Object occupant)
        {
            bool adjacent = false;

            if (this->IsValid(src) && (*this)[src].IsOccupied())
            {
                for (auto &direction : Map::Directions)
                {
                    auto neighbor = src + direction;

                    if (this->IsValid(neighbor) && (*this)[neighbor].Occupant == occupant)
                    {
                        adjacent = true;

                        break;
                    }
                }
            }

            return adjacent;
        }

        // remove occupant from map
        void Remove(Map::Object occupant, int id)
        {
            auto remove = this->Find(occupant, id);

            if (!remove.IsNone())
            {
                (*this)[remove].Occupant = Map::Object::NONE;
                (*this)[remove].Id = -1;
            }
        }

        void CoolDown()
        {
            for (auto y = 0; y < this->Height; y++)
            {
                for (auto x = 0; x < this->Width; x++)
                {
                    auto &tile = (*this)[Point(x, y)];

                    if (tile.IsTemporarilyBlocked())
                    {
                        tile.Lifetime--;

                        if (tile.Lifetime == 0)
                        {
                            tile.TemporaryAsset = Asset::Type::NONE;

                            tile.Occupant = Map::Object::NONE;

                            tile.Lifetime = -1;

                            tile.Id = -1;
                        }
                    }
                }
            }
        }
    };
}

#endif
