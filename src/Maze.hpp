#ifndef __MAZE_HPP__
#define __MAZE_HPP__

#include "FieldOfView.hpp"
#include "Random.hpp"

// classes and functions for generating a maze on the map
namespace BloodSword::Maze
{
    // generate list of valid nearby locations
    Points Neighbors(Map::Base &map, Point &coords)
    {
        auto neighbors = Points();

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

    // check if location has not been visited yet
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

    // get a random nearby location that has not been visited yet
    Point RandomUnvisited(Random::Base &random, Map::Base &map, Point &coords)
    {
        auto unvisited = Points();

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

    // remove object between the two points, make it traversable
    void Remove(Map::Base &map, Point p1, Point p2)
    {
        auto tile = (p1 + p2) / 2;

        map.Put(tile, Map::Object::PASSABLE, Asset::NONE, -1);
    }

    // remove location from the list
    void Remove(Points &list, Point &coords)
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

    // generate maze on the map
    void Generate(Map::Base &map, int width, int height)
    {
        // locations not yet visited
        auto unvisited = Points();

        // locations already visited
        auto visited = Points();

        auto random = Random::Base();

        map.Initialize(width, height);

        // initialize map
        for (auto y = 0; y < map.Height; y++)
        {
            for (auto x = 0; x < map.Width; x++)
            {
                if (y % 2 == 0 || x % 2 == 0)
                {
                    map.Put(x, y, Map::Object::OBSTACLE, Asset::Map("WALL"), -1);
                }
                else
                {
                    map.Put(x, y, Map::Object::NONE, Asset::NONE, -1);

                    unvisited.push_back(Point(x, y));
                }
            }
        }

        // current location
        Point current = unvisited.back();

        unvisited.pop_back();

        visited.push_back(current);

        map.Put(current, Map::Object::PASSABLE, Asset::NONE, -1);

        // generate maze
        while (unvisited.size() != 0)
        {
            if (Maze::Unvisited(map, current))
            {
                // random nearby location
                auto neighbor = Maze::RandomUnvisited(random, map, current);

                Maze::Remove(map, current, neighbor);

                Maze::Remove(unvisited, neighbor);

                map.Put(neighbor, Map::Object::PASSABLE, Asset::NONE, -1);

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
