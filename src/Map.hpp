#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <vector>

#include "Asset.hpp"
#include "MapObjects.hpp"
#include "Random.hpp"

namespace BloodSword::Map
{
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

        void Initialize(int sizex, int sizey)
        {
            this->Width = sizex;

            this->Height = sizey;

            this->Tiles.clear();

            this->Tiles.resize(sizey);

            for (auto i = 0; i < sizey; i++)
            {
                this->Tiles[i] = std::vector<Map::Tile>(sizex);
            }
        }

        Base(int sizex, int sizey)
        {
            Initialize(sizex, sizey);
        }

        Base()
        {
        }

        void Put(int x, int y, Map::Object object, int id)
        {
            if (x >= 0 && x < Width && y >= 0 && y < Height)
            {
                this->Tiles[y][x].Occupant = object;
                this->Tiles[y][x].Id = id;
            }
        }

        void Put(int x, int y, Map::Object type, Asset::Type asset)
        {
            if (x >= 0 && x < Width && y >= 0 && y < Height)
            {
                this->Tiles[y][x].Occupant = Map::Object::NONE;
                this->Tiles[y][x].Type = type;
                this->Tiles[y][x].Asset = asset;
            }
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
                                    this->Tiles[y][x].Id = !data["tiles"][y][x]["id"].is_null() ? (int)data["tiles"][y][x]["id"] : -1;
                                    this->Tiles[y][x].Type = !data["tiles"][y][x]["type"].is_null() ? Map::MapObject(std::string(data["tiles"][y][x]["type"])) : Map::Object::NONE;
                                    this->Tiles[y][x].Occupant = !data["tiles"][y][x]["occupant"].is_null() ? Map::MapObject(std::string(data["tiles"][y][x]["occupant"])) : Map::Object::NONE;
                                    this->Tiles[y][x].Asset = !data["tiles"][y][x]["asset"].is_null() ? Asset::Map(std::string(data["tiles"][y][x]["asset"])) : Asset::Type::NONE;
                                    this->Tiles[y][x].TemporaryAsset = !data["tiles"][y][x]["temporary_asset"].is_null() ? Asset::Map(std::string(data["tiles"][y][x]["temporary_asset"])) : Asset::Type::NONE;
                                    this->Tiles[y][x].Lifetime = !data["tiles"][y][x]["lifetime"].is_null() ? (int)data["tiles"][y][x]["lifetime"] : -1;
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

        // maze generation
        bool IsValid(Point &coords)
        {
            return coords.X >= 0 && coords.Y >= 0 && coords.X < this->Width && coords.Y < this->Height;
        }

        std::vector<Point> Neighbors(Point &coords)
        {
            std::vector<Point> neighbors = {};

            for (int i = 0; i < Map::Directions.size(); i++)
            {
                int new_x = coords.X + Map::Directions[i].X * 2;
                int new_y = coords.Y + Map::Directions[i].Y * 2;

                auto neighbor = Point(new_x, new_y);

                if (this->IsValid(neighbor))
                {
                    neighbors.push_back(neighbor);
                }
            }

            return neighbors;
        }

        bool Unvisited(Point &coords)
        {
            auto neighbords = this->Neighbors(coords);

            auto result = false;

            for (int i = 0; i < neighbords.size(); i++)
            {
                if (this->Tiles[neighbords[i].Y][neighbords[i].X].Type == Map::Object::NONE)
                {
                    result = true;

                    break;
                }
            }

            return result;
        }

        Point RandomUnvisited(Random::Base &random, Point &coords)
        {
            std::vector<Point> unvisited_neighbors = {};

            auto neighbors = this->Neighbors(coords);

            for (int i = 0; i < neighbors.size(); i++)
            {
                if (this->Tiles[neighbors[i].Y][neighbors[i].X].Type == Map::Object::NONE)
                {
                    unvisited_neighbors.push_back(neighbors[i]);
                }
            }

            int index = random.NextInt() % unvisited_neighbors.size();

            return unvisited_neighbors[index];
        }

        void Remove(Point p1, Point p2)
        {
            auto tile = Point((p1.X + p2.X) / 2, (p1.Y + p2.Y) / 2);
            this->Tiles[tile.Y][tile.X].Type = Map::Object::PASSABLE;
            this->Tiles[tile.Y][tile.X].Asset = Asset::Type::NONE;
        }

        void Remove(std::vector<Point> &list, Point &coords)
        {
            for (int i = 0; i < list.size(); i++)
            {
                if (list[i] == coords)
                {
                    list.erase(list.begin() + i);
                }
            }
        }

        void Generate()
        {
            auto random = Random::Base();

            std::vector<Point> unvisited = {};
            std::vector<Point> visited = {};

            // initialize
            for (auto y = 0; y < this->Height; y++)
            {
                for (auto x = 0; x < this->Width; x++)
                {
                    if (y % 2 == 0 || x % 2 == 0)
                    {
                        this->Tiles[y][x].Asset = Asset::Type::WALL;
                        this->Tiles[y][x].Type = Map::Object::OBSTACLE;
                    }
                    else
                    {
                        this->Tiles[y][x].Type = Map::Object::NONE;
                        unvisited.push_back(Point(x, y));
                    }
                }
            }

            // Generate
            Point current = unvisited.back();

            unvisited.pop_back();
            visited.push_back(current);

            this->Tiles[current.Y][current.X].Type = Map::Object::PASSABLE;
            this->Tiles[current.Y][current.X].Asset = Asset::Type::NONE;

            while (unvisited.size() != 0)
            {
                if (this->Unvisited(current))
                {
                    auto neighbor = this->RandomUnvisited(random, current);
                    this->Remove(current, neighbor);

                    current = neighbor;
                    this->Remove(unvisited, current);

                    visited.push_back(current);
                    this->Tiles[current.Y][current.X].Type = Map::Object::PASSABLE;
                    this->Tiles[current.Y][current.X].Asset = Asset::Type::NONE;
                }
                else if (visited.size() != 0)
                {
                    current = visited.back();
                    visited.pop_back();
                }
            }
        }
    };
}

#endif
