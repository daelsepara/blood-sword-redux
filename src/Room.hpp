#ifndef __ROOM_HPP__
#define __ROOM_HPP__

#include "FieldOfView.hpp"
#include "Random.hpp"

namespace BloodSword::Room
{
    int MinWidth = 10;

    int MaxWidth = 20;

    class Base
    {
    public:
        int Id = -1;

        int X1 = -1;

        int Y1 = -1;

        int X2 = -1;

        int Y2 = -1;

        int Width = -1;

        int Height = -1;

        Point Center()
        {
            return Point(this->X1 + this->X2, this->Y1 + this->Y2) / 2;
        }

        std::pair<Point, Point> Inner()
        {
            return {Point(this->X1 + 1, this->Y1 + 1), Point(this->X2 - 1, this->Y2 - 1)};
        }

        Base(int x, int y, int width, int height): X1(x), Y1(y), Width(width), Height(height)
        {
            this->X2 = this->X1 + this->Width;

            this->Y2 = this->Y1 + this->Height;
        }
    };
}

namespace BloodSword::Battlepits
{
    Points TunnelBetween(Point start, Point end)
    {
        auto random = Random::Base();

        Points points = {};

        if (random.NextDouble() < 0.5)
        {
            for (auto x = start.X; x <= end.X; x++)
            {
                points.push_back(Point(x, start.Y));
            }

            for (auto y = start.Y; y <= end.Y; y++)
            {
                points.push_back(Point(end.X, y));
            }
        }
        else
        {
            for (auto y = start.Y; y <= end.Y; y++)
            {
                points.push_back(Point(start.X, y));
            }

            for (auto x = start.X; x <= end.X; x++)
            {
                points.push_back(Point(x, end.Y));
            }
        }

        return points;
    }

    void Place(Map::Base &map, Room::Base &room)
    {
        auto inner = room.Inner();

        auto tl = inner.first;

        auto br = inner.second;

        for (auto y = room.Y1; y < room.Y2; y++)
        {
            for (auto x = room.X1; x < room.X2; x++)
            {
                map[Point(x, y)].Type = Map::Object::OBSTACLE;

                map[Point(x, y)].Asset = Asset::Type::WALL;
            }
        }

        for (auto y = tl.Y; y < br.Y; y++)
        {
            for (auto x = tl.X; x < br.X; x++)
            {
                map[Point(x, y)].Type = Map::Object::PASSABLE;

                map[Point(x, y)].Asset = Asset::Type::NONE;
            }
        }
    }

    bool Inside(Room::Base &room, Point point)
    {
        auto inner = room.Inner();

        auto tl = inner.first;

        auto br = inner.second;

        return ((point.X >= tl.X && point.X < br.X) && (point.Y >= tl.Y && point.Y < br.Y));
    }

    void Generate(Map::Base &map)
    {
        // clear
        for (auto y = 0; y < map.Height; y++)
        {
            for (auto x = 0; x < map.Width; x++)
            {
                map[Point(x, y)].Type = Map::Object::NONE;

                map[Point(x, y)].Asset = Asset::Type::NONE;
            }
        }

        auto room_1 = Room::Base(20, 15, 10, 15);

        auto room_2 = Room::Base(35, 15, 10, 15);

        Battlepits::Place(map, room_1);

        Battlepits::Place(map, room_2);

        for (auto point : TunnelBetween(room_1.Center(), room_2.Center()))
        {
            if (!Inside(room_1, point) && !Inside(room_2, point))
            {
                map[point].Type = Map::Object::PASSABLE;

                map[point].Asset = Asset::Type::IMPASSABLE;
            }
        }
    }
}

#endif
