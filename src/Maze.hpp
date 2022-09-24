#ifndef __MAZE_HPP__
#define __MAZE_HPP__

#include "Map.hpp"
#include "Random.hpp"

namespace BloodSword::Maze
{
    std::vector<Point> Neighbors(Map::Base &map, Point &coords)
    {
        std::vector<Point> neighbors = {};

        for (auto direction : Map::Directions)
        {
            auto neighbor = coords + direction * 2;

            if (map.IsValid(neighbor))
            {
                neighbors.push_back(neighbor);
            }
        }

        return neighbors;
    }

    bool Unvisited(Map::Base &map, Point &coords)
    {
        auto neighbors = Maze::Neighbors(map, coords);

        auto result = false;

        for (auto &neighbor : neighbors)
        {
            if (map[neighbor].Type == Map::Object::NONE)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    Point RandomUnvisited(Random::Base &random, Map::Base &map, Point &coords)
    {
        std::vector<Point> unvisited = {};

        auto neighbors = Maze::Neighbors(map, coords);

        for (auto &neighbor : neighbors)
        {
            if (map[neighbor].Type == Map::Object::NONE)
            {
                unvisited.push_back(neighbor);
            }
        }

        return unvisited[random.NextInt() % unvisited.size()];
    }

    void Remove(Map::Base &map, Point p1, Point p2)
    {
        auto tile = (p1 + p2) / 2;

        map.Put(tile, Map::Object::PASSABLE, Asset::Type::NONE);
    }

    void Remove(std::vector<Point> &list, Point &coords)
    {
        for (auto point = list.begin(); point != list.end(); point++)
        {
            if (*point == coords)
            {
                list.erase(point);

                break;
            }
        }
    }

    void Generate(Map::Base &map, int width, int height)
    {
        std::vector<Point> unvisited = {};

        std::vector<Point> visited = {};

        auto random = Random::Base();

        map.Initialize(width, height);

        // initialize
        for (auto y = 0; y < map.Height; y++)
        {
            for (auto x = 0; x < map.Width; x++)
            {
                if (y % 2 == 0 || x % 2 == 0)
                {
                    map.Put(x, y, Map::Object::OBSTACLE, Asset::Type::WALL);
                }
                else
                {
                    map.Put(x, y, Map::Object::NONE, Asset::Type::NONE);

                    unvisited.push_back(Point(x, y));
                }
            }
        }

        // generate
        Point current = unvisited.back();

        unvisited.pop_back();

        visited.push_back(current);

        map.Put(current, Map::Object::PASSABLE, Asset::Type::NONE);

        while (unvisited.size() != 0)
        {
            if (Maze::Unvisited(map, current))
            {
                auto neighbor = Maze::RandomUnvisited(random, map, current);

                Maze::Remove(map, current, neighbor);
                Maze::Remove(unvisited, neighbor);

                map.Put(neighbor, Map::Object::PASSABLE, Asset::Type::NONE);

                visited.push_back(neighbor);

                current = neighbor;
            }
            else if (visited.size() != 0)
            {
                current = visited.back();

                visited.pop_back();
            }
        }
    }
}
#endif
