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

        // starting locations of party
        std::vector<BloodSword::Point> Origins = {};

        // starting locations of opponents
        std::vector<BloodSword::Point> Spawn = {};

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
        void Viewable(int view_x, int view_y)
        {
            this->ViewX = view_x;

            this->ViewY = view_y;
        }

        // initialize the map
        Base(int size_x, int size_y)
        {
            Initialize(size_x, size_y);
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
            auto load_error = false;

            this->Width = !data["width"].is_null() ? int(data["width"]) : 0;

            this->Height = !data["height"].is_null() ? int(data["height"]) : 0;

            this->ViewX = !data["viewx"].is_null() ? int(data["viewx"]) : 8;

            this->ViewY = !data["viewy"].is_null() ? int(data["viewy"]) : 8;

            this->X = !data["x"].is_null() ? int(data["x"]) : 0;

            this->Y = !data["y"].is_null() ? int(data["y"]) : 0;

            this->DrawX = !data["drawx"].is_null() ? int(data["drawx"]) : 64;

            this->DrawY = !data["drawy"].is_null() ? int(data["drawy"]) : 64;

            this->TileSize = !data["tilesize"].is_null() ? int(data["tilesize"]) : 64;

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

                                map.Id = !tile["id"].is_null() ? int(tile["id"]) : -1;

                                map.Type = !tile["type"].is_null() ? Map::MapObject(std::string(tile["type"])) : Map::Object::NONE;

                                map.Occupant = !tile["occupant"].is_null() ? Map::MapObject(std::string(tile["occupant"])) : Map::Object::NONE;

                                map.Asset = !tile["asset"].is_null() ? Asset::Map(std::string(tile["asset"])) : Asset::Type::NONE;

                                map.TemporaryAsset = !tile["temporary_asset"].is_null() ? Asset::Map(std::string(tile["temporary_asset"])) : Asset::Type::NONE;

                                map.Lifetime = !tile["lifetime"].is_null() ? int(tile["lifetime"]) : -1;
                            }
                        }
                        else
                        {
                            load_error = true;

                            break;
                        }
                    }
                }
                else
                {
                    load_error = true;
                }

                if (!data["origins"].is_null() && data["origins"].is_array() && data["origins"].size() > 0)
                {
                    this->Origins.clear();

                    for (auto i = 0; i < int(data["origins"].size()); i++)
                    {
                        auto x = !data["origins"][i]["x"].is_null() ? int(data["origins"][i]["x"]) : -1;

                        auto y = !data["origins"][i]["y"].is_null() ? int(data["origins"][i]["y"]) : -1;

                        if (x >= 0 && x < this->Width && y >= 0 && y < this->Height)
                        {
                            this->Origins.push_back(BloodSword::Point(x, y));
                        }
                    }
                }

                if (!data["spawn"].is_null() && data["spawn"].is_array() && data["spawn"].size() > 0)
                {
                    this->Spawn.clear();

                    for (auto i = 0; i < int(data["spawn"].size()); i++)
                    {
                        auto x = !data["spawn"][i]["x"].is_null() ? int(data["spawn"][i]["x"]) : -1;

                        auto y = !data["spawn"][i]["y"].is_null() ? int(data["spawn"][i]["y"]) : -1;

                        if (x >= 0 && x < this->Width && y >= 0 && y < this->Height)
                        {
                            this->Spawn.push_back(BloodSword::Point(x, y));
                        }
                    }
                }
            }
            else
            {
                load_error = true;
            }

            return load_error;
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

        void Save(const char *filename)
        {
            nlohmann::json map;

            map["width"] = this->Width;

            map["height"] = this->Height;

            map["viewx"] = this->ViewX;

            map["viewy"] = this->ViewY;

            map["drawx"] = this->DrawX;

            map["drawy"] = this->DrawY;

            map["tilesize"] = this->TileSize;

            nlohmann::json tiles;

            for (auto y = 0; y < Height; y++)
            {
                nlohmann::json row;

                for (auto x = 0; x < Width; x++)
                {
                    nlohmann::json tile;

                    tile.emplace("type", Map::ObjectMapping[this->Tiles[y][x].Type]);

                    tile.emplace("occupant", Map::ObjectMapping[this->Tiles[y][x].Occupant]);

                    tile.emplace("asset", Asset::TypeMapping[this->Tiles[y][x].Asset]);

                    tile.emplace("temporary_asset", Asset::TypeMapping[this->Tiles[y][x].TemporaryAsset]);

                    tile.emplace("lifetime", this->Tiles[y][x].Lifetime);

                    tile.emplace("id", this->Tiles[y][x].Id);

                    row.push_back(tile);
                }

                map["tiles"].push_back(row);
            }

            if (this->Origins.size() > 0)
            {
                nlohmann::json origins;

                for (auto &origin : this->Origins)
                {
                    nlohmann::json point;

                    point.emplace("x", origin.X);

                    point.emplace("y", origin.Y);

                    origins.push_back(point);
                }

                map["origins"] = origins;
            }

            if (this->Spawn.size() > 0)
            {
                nlohmann::json spawn_sites;

                for (auto &spawn : this->Spawn)
                {
                    nlohmann::json point;

                    point.emplace("x", spawn.X);

                    point.emplace("y", spawn.Y);

                    spawn_sites.push_back(point);
                }

                map["spawn"] = spawn_sites;
            }

            std::ofstream file(filename);

            if (file.is_open())
            {
                file << map.dump();

                file.close();
            }
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
