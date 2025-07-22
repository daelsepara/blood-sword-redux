#ifndef __MOVE_HPP__
#define __MOVE_HPP__

#include <algorithm>
#include <memory>

#include "FieldOfView.hpp"
#include "Random.hpp"

// A C++ version of A* pathfinding algorithm from https://dotnetcoretutorials.com/2020/07/25/a-search-pathfinding-algorithm-in-c/
// Most of the comments from the original version are preserved and/or have minor modifications.
//
// This version uses smart pointers
namespace BloodSword::Move
{
    auto Random = Random::Base();

    // Path found by A* algorithm
    class Path
    {
    public:
        // List of coordinates of the path
        BloodSword::Points Points;

        BloodSword::Point Closest;

        Path() {}
    };

    // Class representing a node in the graph
    class Node
    {
    public:
        int X = -1;

        int Y = -1;

        int Cost = 0.0;

        int Distance = 0;

        Smart<Move::Node> Parent = nullptr;

        Node(int x, int y, int cost, Smart<Move::Node> &parent) : X(x), Y(y), Cost(cost), Parent(parent) {}

        Node(Point point, int cost, Smart<Move::Node> &parent) : Node(point.X, point.Y, cost, parent) {}

        Node(int x, int y) : X(x), Y(y) {}

        Node(Point point) : X(point.X), Y(point.Y) {}

        Node() {}

        // Total cost to traverse this node
        int CostDistance()
        {
            return (this->Cost + this->Distance);
        }

        // The distance is essentially the estimated distance, ignoring obstacles to our target.
        // So how many nodes left and right, up and down, ignoring obstacles, to get there.
        //
        // Computes the 2D Manhattan Distance
        void SetDistance(Smart<Move::Node> &node)
        {
            this->Distance = std::abs(node->X - X) + std::abs(node->Y - Y);
        }
    };

    // list of nodes
    typedef std::vector<Smart<Move::Node>> Moves;

    Point operator+(const Smart<Node> &node, const Point &p)
    {
        return Point(node->X + p.X, node->Y + p.Y);
    }

    // is the node equal to the point?
    bool Is(Smart<Move::Node> &a, Point &b)
    {
        return a->X == b.X && a->Y == b.Y;
    }

    // compare equality between two nodes
    bool Compare(Smart<Move::Node> &a, Smart<Move::Node> &b)
    {
        return a->X == b->X && a->Y == b->Y;
    }

    // check if location is traversible
    bool IsPassable(Map::Base &map, Point &location, bool is_enemy)
    {
        auto result = false;

        if (map.IsValid(location))
        {
            Map::Tile &Tile = map[location];

            auto is_valid = !Tile.IsOccupied() && !Tile.IsBlocked();

            auto is_passable = Tile.IsPassable();

            auto is_passable_enemy = Tile.IsPassableToEnemy() && is_enemy;

            auto is_exit = Tile.IsExit();

            result = (is_passable || is_passable_enemy || is_exit) && is_valid;
        }

        return result;
    }

    // check if location is traversable or if it is the target destination
    bool IsPassable(Map::Base &map, Smart<Move::Node> &target, Point &location, bool is_enemy, int enemy_target = Map::NotFound)
    {
        auto result = false;

        if (map.IsValid(location))
        {
            auto &tile = map[location];

            auto is_enemy_target = (map[location].IsEnemy() && Move::Is(target, location) && map[location].Id == enemy_target && enemy_target != Map::NotFound);

            auto is_target = (map[location].IsPlayer() && Move::Is(target, location));

            auto is_destination = Move::Is(target, location) && !tile.IsOccupied();

            auto is_passable = Move::IsPassable(map, location, is_enemy);

            result = (is_passable || is_destination || (is_enemy && (is_target || is_enemy_target)));
        }

        return result;
    }

    // Get all traversible nodes from current node
    Moves Nodes(Map::Base &map, Smart<Move::Node> &current, Smart<Move::Node> &target, bool is_enemy, int enemy_target = Map::NotFound)
    {
        auto traversable = Moves();

        auto directions = Map::Directions;

        if (is_enemy)
        {
            std::shuffle(directions.begin(), directions.end(), Move::Random.Generator());
        }

        if (map.Width > 0 && map.Height > 0)
        {
            for (auto &neighbor : directions)
            {
                auto next = current + neighbor;

                // Check if within map boundaries and if passable and/or leads to destination
                if (Move::IsPassable(map, target, next, is_enemy, enemy_target))
                {
                    traversable.push_back(std::make_shared<Move::Node>(next, current->Cost + 1, current));

                    traversable.back()->SetDistance(target);
                }
            }
        }

        return traversable;
    }

    // Get index of node from a list
    Moves::const_iterator Find(Moves &nodes, Smart<Move::Node> &node)
    {
        return BloodSword::Find(nodes, node, Move::Compare);
    }

    // Remove node from list
    void Remove(Moves &nodes, Smart<Move::Node> &node)
    {
        auto found = Move::Find(nodes, node);

        if (found != nodes.end())
        {
            nodes.erase(found);
        }
    }

    // Check if node is on the list
    bool In(Moves &nodes, Smart<Move::Node> &node)
    {
        return Move::Find(nodes, node) != nodes.end();
    }

    // Find path from src to dst using the A* algorithm
    Move::Path FindPath(Map::Base &map, Point src, Point dst, int enemy_target = Map::NotFound)
    {
        auto path = Move::Path();

        auto valid = map.IsValid(src) && map.IsValid(dst);

        if (map.Width > 0 && map.Height > 0 && valid)
        {
            auto start = std::make_shared<Move::Node>(src);

            auto end = std::make_shared<Move::Node>(dst);

            start->SetDistance(end);

            // List of nodes to be checked
            auto active = Moves();

            // List of nodes already visited
            auto visited = Moves();

            active.push_back(start);

            auto is_enemy = map[src].IsEnemy();

            auto min_distance = map.Distance(src, dst);

            path.Closest = src;

            while (!active.empty())
            {
                // Sort based on CostDistance
                std::sort(active.begin(), active.end(), [](Smart<Move::Node> &src, Smart<Move::Node> &dst)
                          { return src->CostDistance() < dst->CostDistance(); });

                auto check = active.front();

                if (Move::Compare(check, end))
                {
                    // We found the destination and we can be sure (because of the sort order above)
                    // that it's the most low cost option.
                    auto node = check;

                    while (node)
                    {
                        path.Points.push_back(Point(node->X, node->Y));

                        node = node->Parent;
                    }

                    // Reverse list of coordinates so path leads from src to dst
                    std::reverse(path.Points.begin(), path.Points.end());

                    return path;
                }

                visited.push_back(check);

                auto test = Point(check->X, check->Y);

                auto dist = map.Distance(test, dst);

                // check if this is closest point to destination
                if (dist < min_distance && !map[test].IsOccupied())
                {
                    path.Closest = test;

                    min_distance = dist;
                }

                Move::Remove(active, check);

                auto nodes = Move::Nodes(map, check, end, is_enemy, enemy_target);

                for (auto &node : nodes)
                {
                    // We have already visited this node so we don't need to do so again!
                    if (Move::In(visited, node))
                    {
                        continue;
                    }

                    // It's already in the active list, but that's OK, maybe this new node has a better value (e.g. We might zigzag earlier but this is now straighter).
                    if (Move::In(active, node))
                    {
                        auto existing = *Move::Find(active, node);

                        if (existing->CostDistance() > node->CostDistance())
                        {
                            Move::Remove(active, existing);

                            active.push_back(node);
                        }
                    }
                    else
                    {
                        // We've never seen this node before so add it to the list.
                        active.push_back(node);
                    }
                }
            }
        }

        return path;
    }

    // return the number of valid moves that can be made in the path
    int Count(Map::Base &map, Move::Path &path, bool is_enemy, int enemy_target = Map::NotFound)
    {
        auto count = 0;

        if (path.Points.size() > 1)
        {
            // skip source and destination
            for (auto current = path.Points.begin(); current != --path.Points.end(); current++)
            {
                auto next = std::next(current);

                auto target = std::make_shared<Move::Node>(*next);

                // check if move to next location is possible
                if (!Move::IsPassable(map, target, *current, is_enemy, enemy_target) && current != path.Points.begin())
                {
                    break;
                }

                count++;
            }
        }

        return count;
    }

    int Count(Map::Base &map, Move::Path &path)
    {
        return Move::Count(map, path, false);
    }

    // check if src is not blocked
    bool Available(Map::Base &map, Point src)
    {
        auto traversable = Points();

        if (map.Width > 0 && map.Height > 0 && map.IsValid(src))
        {
            auto is_enemy = map[src].IsEnemy();

            for (auto &neighbor : Map::Directions)
            {
                auto next = src + neighbor;

                if (map.IsValid(next))
                {
                    // Check if within map boundaries and if passable
                    if (Move::IsPassable(map, next, is_enemy))
                    {
                        traversable.push_back(next);
                    }
                }
            }
        }

        return !traversable.empty();
    }
}

#endif
