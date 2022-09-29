#ifndef __MOVE_HPP__
#define __MOVE_HPP__

#include <algorithm>
#include <memory>
#include <vector>

#include "Map.hpp"
#include "Templates.hpp"

// A C++ version of A* pathfinding algorithm from https://dotnetcoretutorials.com/2020/07/25/a-search-pathfinding-algorithm-in-c/
// Most of the comments from the original version are preserved and/or have minor modifications.
//
// This version uses smart pointers
namespace BloodSword::Move
{
    // Path found by A* algorithm
    class Path
    {
    public:
        // List of coordinates of the path
        std::vector<Point> Points;

        Path() {}
    };

    // Class representing a node in the graph
    class Node
    {
    public:
        int X;

        int Y;

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
    bool IsPassable(Map::Base &map, Point &location, bool IsEnemy, bool Unrestricted)
    {
        auto result = false;

        if (map.IsValid(location))
        {
            Map::Tile &Tile = map[location];

            auto NotOccupied = !Tile.IsOccupied() && !Tile.IsBlocked();

            auto IsValid = (Unrestricted || NotOccupied);

            auto IsPassable = Tile.IsPassable() && IsValid;

            auto IsPassableToEnemy = IsEnemy && Tile.IsPassableToEnemy() && IsValid;

            auto IsExit = !IsEnemy && Tile.IsExit() && IsValid;

            auto IsTarget = IsEnemy && Tile.IsPlayer();

            result = (IsPassable || IsPassableToEnemy || IsExit || IsTarget);
        }

        return result;
    }

    // check if location is traversable or if it is the target destination
    bool IsPassable(Map::Base &map, Smart<Move::Node> &target, Point &location, bool IsEnemy, bool Unrestricted)
    {
        auto result = false;

        if (map.IsValid(location))
        {
            Map::Tile &Tile = map[location];

            auto NotOccupied = !Tile.IsOccupied() && !Tile.IsBlocked();

            auto IsValid = (Unrestricted || NotOccupied);

            auto IsDestination = Move::Is(target, location) && IsValid;

            auto IsPassable = Move::IsPassable(map, location, IsEnemy, Unrestricted);

            result = (IsPassable || IsDestination);
        }

        return result;
    }

    // Get all traversible nodes from current node
    Moves Nodes(Map::Base &map, Smart<Move::Node> &current, Smart<Move::Node> &target, bool IsEnemy, bool Unrestricted)
    {
        auto traversable = Moves();

        if (map.Width > 0 && map.Height > 0)
        {
            for (auto &neighbor : Map::Directions)
            {
                auto next = current + neighbor;

                // Check if within map boundaries and if passable and/or leads to destination
                if (Move::IsPassable(map, target, next, IsEnemy, Unrestricted))
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
    Move::Path FindPath(Map::Base &map, Point src, Point dst, bool Unrestricted = false)
    {
        auto path = Move::Path();

        auto Valid = map.IsValid(src) && map.IsValid(dst);

        if (map.Width > 0 && map.Height > 0 && Valid)
        {
            auto start = std::make_shared<Move::Node>(src);

            auto end = std::make_shared<Move::Node>(dst);

            start->SetDistance(end);

            // List of nodes to be checked
            auto active = Moves();

            // List of nodes already visited
            auto visited = Moves();

            active.push_back(start);

            auto IsEnemy = map[src].IsEnemy();

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

                Move::Remove(active, check);

                auto nodes = Move::Nodes(map, check, end, IsEnemy, Unrestricted);

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

    // find a path from src X, Y to dst X, Y
    Move::Path FindPath(Map::Base &map, int srcX, int srcY, int dstX, int dstY, bool Unrestricted = false)
    {
        return Move::FindPath(map, Point(srcX, srcY), Point(dstX, dstY), Unrestricted);
    }

    // return the number of valid moves that can be made in the path
    int Count(Map::Base &map, Move::Path &path, bool IsEnemy)
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
                if (!Move::IsPassable(map, target, *current, IsEnemy, false) && current != path.Points.begin())
                {
                    break;
                }

                count++;
            }
        }

        return count;
    }

    // check if src is not blocked
    bool Available(Map::Base &map, Point src)
    {
        auto traversable = std::vector<Point>();

        if (map.Width > 0 && map.Height > 0)
        {
            for (auto &neighbor : Map::Directions)
            {
                auto next = src + neighbor;

                // Check if within map boundaries and if passable
                if (Move::IsPassable(map, next, false, false))
                {
                    traversable.push_back(next);
                }
            }
        }

        return !traversable.empty();
    }
}
#endif
