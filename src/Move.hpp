#ifndef __MOVE_HPP__
#define __MOVE_HPP__

#include <algorithm>
#include <memory>
#include <vector>

#include "Map.hpp"

// A C++ version of A* pathfinding algorithm from https://dotnetcoretutorials.com/2020/07/25/a-search-pathfinding-algorithm-in-c/
// Most of the comments from the original version are preserved and/or have minor modifications.
//
// This version uses smart pointers
namespace BloodSword::Move
{
    // Cartesian coordinates (see Path class below)
    class Point
    {
    public:
        int X;

        int Y;

        Point(int x, int y)
        {
            this->X = x;

            this->Y = y;
        }

        Point()
        {
        }
    };

    // Path found by A* algorithm
    class Path
    {
    public:
        // List of coordinates of the path
        std::vector<Move::Point> Points;

        Path()
        {
        }
    };

    // Class representing a node in the graph
    class Node
    {
    public:
        int X;

        int Y;

        int Cost = 0.0;

        int Distance = 0;

        std::shared_ptr<Move::Node> Parent = nullptr;

        Node(int x, int y, int cost, std::shared_ptr<Move::Node> &parent)
        {
            this->X = x;

            this->Y = y;

            this->Cost = cost;

            this->Parent = parent;
        }

        Node(int x, int y)
        {
            this->X = x;

            this->Y = y;
        }

        Node()
        {
        }

        // Total cost to traverse this node
        int CostDistance()
        {
            return (this->Cost + this->Distance);
        }

        // The distance is essentially the estimated distance, ignoring obstacles to our target.
        // So how many nodes left and right, up and down, ignoring obstacles, to get there.
        //
        // Computes the 2D Manhattan Distance
        void SetDistance(std::shared_ptr<Move::Node> &node)
        {
            this->Distance = std::abs(node->X - X) + std::abs(node->Y - Y);
        }
    };

    bool IsPassable(Map::Base &map, std::shared_ptr<Move::Node> &target, int X, int Y, bool IsEnemy, bool Unrestricted)
    {
        auto result = false;

        if (X >= 0 && X < map.Width && Y >= 0 && Y < map.Height)
        {
            Map::Tile &Tile = map.Tiles[Y][X];

            auto NotOccupied = !Tile.IsOccupied();

            auto IsDestination = (Y == target->Y && X == target->X);

            auto IsPassable = Tile.IsPassable() && (Unrestricted || NotOccupied);

            auto IsPassableToEnemy = IsEnemy && Tile.IsPassableToEnemy() && (Unrestricted || NotOccupied);

            auto IsExit = !IsEnemy && Tile.IsExit() && NotOccupied;

            auto IsTarget = IsEnemy && Tile.IsPlayer();

            result = (IsPassable || IsDestination || IsPassableToEnemy || IsExit || IsTarget);
        }

        return result;
    }

    // Get all traversible nodes from current node
    std::vector<std::shared_ptr<Move::Node>> Nodes(Map::Base &map, std::shared_ptr<Move::Node> &current, std::shared_ptr<Move::Node> &target, bool IsEnemy, bool Unrestricted)
    {
        // Define neighbors (X, Y): Up, Down, Left, Right
        std::vector<std::pair<int, int>> neighbors = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

        auto traversable = std::vector<std::shared_ptr<Move::Node>>();

        if (map.Width > 0 && map.Height > 0)
        {
            auto index = 0;

            for (auto i = 0; i < neighbors.size(); i++)
            {
                // Check if within map boundaries and if passable and/or leads to destination
                if (Move::IsPassable(map, target, current->X + neighbors[i].first, current->Y + neighbors[i].second, IsEnemy, Unrestricted))
                {
                    auto X = current->X + neighbors[i].first;

                    auto Y = current->Y + neighbors[i].second;

                    auto Cost = current->Cost + 1;

                    traversable.push_back(std::make_shared<Move::Node>(X, Y, Cost, current));

                    traversable[index]->SetDistance(target);

                    index++;
                }
            }
        }

        return traversable;
    }

    // Get index of node from a list
    int Index(std::vector<std::shared_ptr<Move::Node>> &nodes, std::shared_ptr<Move::Node> &node)
    {
        auto index = -1;

        for (auto i = 0; i < nodes.size(); i++)
        {
            if (nodes[i]->X == node->X && nodes[i]->Y == node->Y)
            {
                index = i;

                break;
            }
        }

        return index;
    }

    // Remove node from list
    void Remove(std::vector<std::shared_ptr<Move::Node>> &nodes, std::shared_ptr<Move::Node> &node)
    {
        auto index = Move::Index(nodes, node);

        if (index >= 0 && index < nodes.size())
        {
            nodes.erase(nodes.begin() + index);
        }
    }

    // Check if node is on the list
    bool Any(std::vector<std::shared_ptr<Move::Node>> &nodes, std::shared_ptr<Move::Node> &node)
    {
        auto index = Move::Index(nodes, node);

        return (index >= 0 && index < nodes.size());
    }

    // Find path from src to dst using the A* algorithm
    Move::Path FindPath(Map::Base &map, int srcX, int srcY, int dstX, int dstY, bool Unrestricted)
    {
        auto path = Move::Path();

        auto ValidX = srcX >= 0 && srcX < map.Width && dstX >= 0 && dstX < map.Width;

        auto ValidY = srcY >= 0 && srcY < map.Height && dstY >= 0 && dstY < map.Height;

        if (map.Width > 0 && map.Height > 0 && ValidX && ValidY)
        {
            auto start = std::make_shared<Move::Node>(srcX, srcY);

            auto end = std::make_shared<Move::Node>(dstX, dstY);

            start->SetDistance(end);

            // List of nodes to be checked
            auto active = std::vector<std::shared_ptr<Move::Node>>();

            // List of nodes already visited
            auto visited = std::vector<std::shared_ptr<Move::Node>>();

            active.push_back(start);

            auto IsEnemy = map.Tiles[srcY][srcX].IsEnemy();

            while (active.size() > 0)
            {
                // Sort based on CostDistance
                std::sort(active.begin(), active.end(), [](std::shared_ptr<Move::Node> &src, std::shared_ptr<Move::Node> &dst)
                          { return src->CostDistance() < dst->CostDistance(); });

                auto check = active.front();

                if (check->X == end->X && check->Y == end->Y)
                {
                    // We found the destination and we can be sure (because of the sort order above)
                    // that it's the most low cost option.
                    auto node = check;

                    while (node)
                    {
                        path.Points.push_back(Move::Point(node->X, node->Y));

                        node = node->Parent;
                    }

                    // Reverse list of coordinates so path leads from src to dst
                    std::reverse(path.Points.begin(), path.Points.end());

                    return path;
                }

                visited.push_back(check);

                Move::Remove(active, check);

                auto nodes = Move::Nodes(map, check, end, IsEnemy, Unrestricted);

                for (auto i = 0; i < nodes.size(); i++)
                {
                    auto node = nodes[i];

                    // We have already visited this node so we don't need to do so again!
                    if (Move::Any(visited, node))
                    {
                        continue;
                    }

                    // It's already in the active list, but that's OK, maybe this new node has a better value (e.g. We might zigzag earlier but this is now straighter).
                    if (Move::Any(active, node))
                    {
                        auto existing = active[Move::Index(active, node)];

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

    Move::Path FindPath(Map::Base &map, int srcX, int srcY, int dstX, int dstY)
    {
        return Move::FindPath(map, srcX, srcY, dstX, dstY, false);
    }
}
#endif
