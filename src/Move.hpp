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

        Smart<Move::Node> Parent = nullptr;

        Node(int x, int y, int cost, Smart<Move::Node> &parent)
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
        void SetDistance(Smart<Move::Node> &node)
        {
            this->Distance = std::abs(node->X - X) + std::abs(node->Y - Y);
        }
    };

    typedef std::vector<Smart<Move::Node>> Moves;

    bool IsPassable(Map::Base &map, Smart<Move::Node> &target, int X, int Y, bool IsEnemy, bool Unrestricted)
    {
        auto result = false;

        if (X >= 0 && X < map.Width && Y >= 0 && Y < map.Height)
        {
            Map::Tile &Tile = map.Tiles[Y][X];

            auto NotOccupied = !Tile.IsOccupied();

            auto IsValid = (Unrestricted || NotOccupied);

            auto IsDestination = (Y == target->Y && X == target->X) && IsValid;

            auto IsPassable = Tile.IsPassable() && IsValid;

            auto IsPassableToEnemy = IsEnemy && Tile.IsPassableToEnemy() && IsValid;

            auto IsExit = !IsEnemy && Tile.IsExit() && IsValid;

            auto IsTarget = IsEnemy && Tile.IsPlayer();

            result = (IsPassable || IsDestination || IsPassableToEnemy || IsExit || IsTarget);
        }

        return result;
    }

    bool IsPassable(Map::Base &map, Smart<Move::Node> &target, Point &current, bool IsEnemy, bool Unrestricted)
    {
        return Move::IsPassable(map, target, current.X, current.Y, IsEnemy, Unrestricted);
    }

    // Get all traversible nodes from current node
    Moves Nodes(Map::Base &map, Smart<Move::Node> &current, Smart<Move::Node> &target, bool IsEnemy, bool Unrestricted)
    {
        // Define neighbors (X, Y): Up, Down, Left, Right
        std::vector<std::pair<int, int>> neighbors = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

        auto traversable = Moves();

        if (map.Width > 0 && map.Height > 0)
        {
            auto index = 0;

            for (auto &neighbor : neighbors)
            {
                // Check if within map boundaries and if passable and/or leads to destination
                if (Move::IsPassable(map, target, current->X + neighbor.first, current->Y + neighbor.second, IsEnemy, Unrestricted))
                {
                    auto X = current->X + neighbor.first;

                    auto Y = current->Y + neighbor.second;

                    auto Cost = current->Cost + 1;

                    traversable.push_back(std::make_shared<Move::Node>(X, Y, Cost, current));

                    traversable[index]->SetDistance(target);

                    index++;
                }
            }
        }

        return traversable;
    }

    bool Compare(Smart<Move::Node> &a, Smart<Move::Node> &b)
    {
        return a->X == b->X && a->Y == b->Y;
    }

    // Get index of node from a list
    Moves::const_iterator Find(Moves &nodes, Smart<Move::Node> &node)
    {
        return BloodSword::Find(nodes, node, Compare);
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
    bool Is(Moves &nodes, Smart<Move::Node> &node)
    {
        return Move::Find(nodes, node) != nodes.end();
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
            auto active = Moves();

            // List of nodes already visited
            auto visited = Moves();

            active.push_back(start);

            auto IsEnemy = map.Tiles[srcY][srcX].IsEnemy();

            while (!active.empty())
            {
                // Sort based on CostDistance
                std::sort(active.begin(), active.end(), [](Smart<Move::Node> &src, Smart<Move::Node> &dst)
                          { return src->CostDistance() < dst->CostDistance(); });

                auto check = active.front();

                if (check->X == end->X && check->Y == end->Y)
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
                    if (Move::Is(visited, node))
                    {
                        continue;
                    }

                    // It's already in the active list, but that's OK, maybe this new node has a better value (e.g. We might zigzag earlier but this is now straighter).
                    if (Move::Is(active, node))
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

    Move::Path FindPath(Map::Base &map, int srcX, int srcY, int dstX, int dstY)
    {
        return Move::FindPath(map, srcX, srcY, dstX, dstY, false);
    }

    Move::Path FindPath(Map::Base &map, Point &src, Point &dst, bool unrestricted)
    {
        return Move::FindPath(map, src.X, src.Y, dst.X, dst.Y, unrestricted);
    }

    Move::Path FindPath(Map::Base &map, Point &src, Point &dst)
    {
        return Move::FindPath(map, src.X, src.Y, dst.X, dst.Y);
    }

    // return the number of valid moves that can be made in the path
    int Count(Map::Base &map, Move::Path &path, bool IsEnemy)
    {
        auto count = 0;

        if (path.Points.size() > 2)
        {
            // skip source and destination
            for (auto current = ++path.Points.begin(); current != --path.Points.end(); current++)
            {
                auto next = std::next(current);

                auto target = std::make_shared<Move::Node>(next->X, next->Y);

                // check if move to next location is possible
                if (!Move::IsPassable(map, target, *current, IsEnemy, false))
                {
                    break;
                }

                count++;
            }
        }

        return count;
    }
}
#endif
