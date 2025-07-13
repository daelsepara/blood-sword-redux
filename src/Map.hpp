#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <fstream>

#include "nlohmann/json.hpp"
#include "AssetTypes.hpp"
#include "MapObjects.hpp"
#include "Primitives.hpp"

namespace BloodSword::Map
{
    const int Unlimited = -1;

    const int NotFound = -1;

    const int Party = 0;

    // Define neighbors (X, Y): Up, Down, Left, Right
    const Points Directions = {Point(0, -1), Point(1, 0), Point(0, 1), Point(-1, 0)};

    // Define neighbors and diagonals (X, Y): NW, N, NE, E, SE, S, SW, W
    const Points Diagonals = {Point(0, -1), Point(1, -1), Point(1, 0), Point(1, 1), Point(0, 1), Point(-1, 1), Point(-1, 0), Point(-1, -1)};

    // a single map tile
    class Tile
    {
    public:
        // tile type classification
        Map::Object Type = Map::Object::PASSABLE;

        // the tile's current occupant
        Map::Object Occupant = Map::Object::NONE;

        // asset type of tile
        Asset::Type Asset = Asset::Type::NONE;

        // asset type of the temporary occupant
        Asset::Type TemporaryAsset = Asset::Type::NONE;

        // lifetime of the temporary occupant, -1 if none
        int Lifetime = Map::Unlimited;

        // Id of the current occupant, -1 if none
        int Id = Map::NotFound;

        // if tile has been explored (Rogue mode)
        bool Explored = false;

        // room number (Rogue mode)
        int Room = -1;

        // tile is occupied by an enemy
        bool IsEnemy()
        {
            return this->Occupant == Map::Object::ENEMY || this->Occupant == Map::Object::ENEMIES;
        }

        // tile is occupied by a player
        bool IsPlayer()
        {
            return this->Occupant == Map::Object::PLAYER || this->Occupant == Map::Object::PARTY;
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
            return (this->Type == Map::Object::OBSTACLE || this->IsTemporarilyBlocked() || this->Type == Map::Object::NONE);
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
        int DrawX = BloodSword::TileSize + BloodSword::HalfTile;

        // y offset (pixels, drawing)
        int DrawY = BloodSword::TileSize + BloodSword::HalfTile;

        // size of each tile (pixels)
        int TileSize = BloodSword::TileSize;

        // starting locations of party
        BloodSword::Points Origins = {};

        // starting locations of opponents
        BloodSword::Points Spawn = {};

        // starting locations of players who are temporarily away
        BloodSword::Points AwayPlayers = {};

        // starting locations of opponents who are temporarily away
        BloodSword::Points AwayOpponents = {};

        // starting locations of survivors
        BloodSword::Points Survivors = {};

        // initialize the map
        void Initialize(int width, int height)
        {
            this->Width = std::max(0, width);

            this->Height = std::max(0, height);

            // make sure that ViewX and ViewY are within map dimensions
            this->ViewX = std::min(this->Width, width);

            this->ViewY = std::min(this->Height, height);

            // re-intialize
            this->Tiles = std::vector(height, std::vector(width, Map::Tile()));
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

        void CheckBounds()
        {
            if (this->X < 0)
            {
                this->X = 0;
            }

            if (this->X > this->Width - this->ViewX)
            {
                this->X = this->Width - this->ViewX;
            }

            if (this->Y < 0)
            {
                this->Y = 0;
            }

            if (this->Y > this->Height - this->ViewY)
            {
                this->Y = this->Height - this->ViewY;
            }
        }

        BloodSword::Points LoadPoints(nlohmann::json &data)
        {
            auto points = BloodSword::Points();

            if (!data.is_null() && data.is_array() && data.size() > 0)
            {

                for (auto i = 0; i < data.size(); i++)
                {
                    auto x = !data[i]["x"].is_null() ? int(data[i]["x"]) : -1;

                    auto y = !data[i]["y"].is_null() ? int(data[i]["y"]) : -1;

                    if (x >= 0 && x < this->Width && y >= 0 && y < this->Height)
                    {
                        points.push_back(BloodSword::Point(x, y));
                    }
                }
            }

            return points;
        }

        bool Setup(nlohmann::json &data)
        {
            auto load_error = false;

            this->Width = !data["width"].is_null() ? int(data["width"]) : 0;

            this->Height = !data["height"].is_null() ? int(data["height"]) : 0;

            this->ViewX = !data["viewx"].is_null() ? int(data["viewx"]) : 7;

            this->ViewY = !data["viewy"].is_null() ? int(data["viewy"]) : 7;

            this->X = !data["x"].is_null() ? int(data["x"]) : 0;

            this->Y = !data["y"].is_null() ? int(data["y"]) : 0;

            this->DrawX = !data["drawx"].is_null() ? int(data["drawx"]) : BloodSword::TileSize;

            this->DrawY = !data["drawy"].is_null() ? int(data["drawy"]) : BloodSword::TileSize;

            this->TileSize = !data["tilesize"].is_null() ? int(data["tilesize"]) : BloodSword::TileSize;

            // ensure that view is within map dimensions
            this->ViewX = std::min(this->Width, this->ViewX);

            this->ViewY = std::min(this->Height, this->ViewY);

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
                    this->Origins = this->LoadPoints(data["origins"]);
                }

                if (!data["spawn"].is_null() && data["spawn"].is_array() && data["spawn"].size() > 0)
                {
                    this->Spawn = this->LoadPoints(data["spawn"]);
                }

                if (!data["survivors"].is_null() && data["survivors"].is_array() && data["survivors"].size() > 0)
                {
                    this->Survivors = this->LoadPoints(data["survivors"]);
                }

                if (!data["away_players"].is_null() && data["away_players"].is_array() && data["away_players"].size() > 0)
                {
                    this->AwayPlayers = this->LoadPoints(data["away_players"]);
                }

                if (!data["away_opponents"].is_null() && data["away_opponents"].is_array() && data["away_opponents"].size() > 0)
                {
                    this->AwayOpponents = this->LoadPoints(data["away_opponents"]);
                }
            }
            else
            {
                load_error = true;
            }

            return load_error;
        }

        // load map from json file
        bool Load(const char *map)
        {
            auto LoadError = false;

            std::ifstream ifs(map);

            if (ifs.good())
            {
                auto data = nlohmann::json::parse(ifs);

                LoadError = this->Setup(data);

                ifs.close();
            }
            else
            {
                LoadError = true;
            }

            return !LoadError;
        }

        nlohmann::json Points(BloodSword::Points &points)
        {
            nlohmann::json data;

            for (auto &point : points)
            {
                nlohmann::json item;

                item.emplace("x", point.X);

                item.emplace("y", point.Y);

                data.push_back(item);
            }

            return data;
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

            map["x"] = this->X;

            map["y"] = this->Y;

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
                map["origins"] = this->Points(this->Origins);
            }

            if (this->Spawn.size() > 0)
            {
                map["spawn"] = this->Points(this->Spawn);
            }

            if (this->Survivors.size() > 0)
            {
                map["survivors"] = this->Points(this->Survivors);
            }

            if (this->AwayPlayers.size() > 0)
            {
                map["away_players"] = this->Points(this->AwayPlayers);
            }

            if (this->AwayOpponents.size() > 0)
            {
                map["away_opponents"] = this->Points(this->AwayOpponents);
            }

            std::ofstream ofs(filename);

            if (ofs.is_open())
            {
                ofs << map.dump();

                ofs.close();
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

        bool Next(Point src, Map::Object occupant, int id)
        {
            auto next = false;

            if (this->IsValid(src) && !(*this)[src].IsOccupied())
            {
                for (auto &direction : Map::Directions)
                {
                    auto neighbor = src + direction;

                    if (this->IsValid(neighbor) && (*this)[neighbor].Occupant == occupant && (*this)[neighbor].Id != id)
                    {
                        next = true;

                        break;
                    }
                }
            }

            return next;
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

        int Free(Point src)
        {
            int space = 0;

            if (this->IsValid(src) && (*this)[src].IsOccupied())
            {
                for (auto &direction : Map::Directions)
                {
                    auto neighbor = src + direction;

                    if (this->IsValid(neighbor))
                    {
                        auto &tile = (*this)[neighbor];

                        auto occupied = tile.IsOccupied();

                        auto blocked = tile.IsBlocked();

                        auto passable = tile.IsPassable() || tile.IsPassableToEnemy() || tile.IsExit();

                        if (!occupied && !blocked && passable)
                        {
                            space++;
                        }
                    }
                }
            }

            return space;
        }

        // remove occupant from map
        void Remove(Map::Object occupant, int id)
        {
            while (true)
            {
                auto remove = this->Find(occupant, id);

                if (!remove.IsNone())
                {
                    (*this)[remove].Occupant = Map::Object::NONE;

                    (*this)[remove].Id = Map::NotFound;
                }
                else
                {
                    break;
                }
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

                            tile.Lifetime = Map::Unlimited;

                            tile.Id = Map::NotFound;
                        }
                    }
                }
            }
        }
    };
}

#endif
