#ifndef __BATTLEPITS_HPP__
#define __BATTLEPITS_HPP__

#include "FieldOfView.hpp"
#include "Random.hpp"

namespace BloodSword::Room
{
    class Base
    {
    public:
        int X1 = -1;

        int Y1 = -1;

        int X2 = -1;

        int Y2 = -1;

        int Width = -1;

        int Height = -1;

        Point Center()
        {
            auto x = this->X1 + this->Width / 2;

            auto y = this->Y1 + this->Height / 2;

            return Point(x, y);
        }

        std::pair<Point, Point> Inner()
        {
            return {Point(this->X1 + 1, this->Y1 + 1), Point(this->X2 - 1, this->Y2 - 1)};
        }

        Base(int x, int y, int width, int height) : X1(x), Y1(y), Width(width), Height(height)
        {
            this->X2 = this->X1 + this->Width;

            this->Y2 = this->Y1 + this->Height;
        }

        bool Inside(Point point)
        {
            return ((point.X > this->X1 && point.X < this->X2 - 1) && (point.Y > this->Y1 && point.Y < this->Y2 - 1));
        }

        bool Intersects(const Room::Base &room)
        {
            return (this->X1 <= room.X2 && this->X2 >= room.X1 && this->Y1 <= room.Y2 && this->Y2 >= room.Y1);
        }

        bool Intersects(const std::vector<Room::Base> &rooms)
        {
            auto result = false;

            for (auto room : rooms)
            {
                result |= this->Intersects(room);

                if (result)
                {
                    break;
                }
            }

            return result;
        }
    };
}

namespace BloodSword::Battlepits
{
    bool Inside(std::vector<Room::Base> &rooms, Room::Base &room, Point point)
    {
        auto result = room.Inside(point);

        for (auto room : rooms)
        {
            result |= room.Inside(point);
        }

        return result;
    }

    void VerticalTunnel(Points &points, int x, int y1, int y2)
    {
        auto delta = 0;

        if (y1 < y2)
        {
            delta = 1;
        }
        else if (y1 > y2)
        {
            delta = -1;
        }

        for (auto y = y1; y != y2; y += delta)
        {
            points.push_back(Point(x, y));
        }
    }

    void HorizontalTunnel(Points &points, int x1, int x2, int y)
    {
        auto delta = 0;

        if (x1 < x2)
        {
            delta = 1;
        }
        else if (x1 > x2)
        {
            delta = -1;
        }

        for (auto x = x1; x != x2; x += delta)
        {
            points.push_back(Point(x, y));
        }
    }

    Points TunnelBetween(Point start, Point end)
    {
        auto random = Random::Base();

        Points points = {};

        if (random.NextDouble() < 0.5)
        {
            Battlepits::VerticalTunnel(points, start.X, start.Y, end.Y);

            Battlepits::HorizontalTunnel(points, start.X, end.X, end.Y);
        }
        else
        {
            Battlepits::HorizontalTunnel(points, start.X, end.X, start.Y);

            Battlepits::VerticalTunnel(points, end.X, start.Y, end.Y);
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
                // do not overwrite tunnels
                if (map[Point(x, y)].Type != Map::Object::PASSABLE)
                {
                    map[Point(x, y)].Type = Map::Object::OBSTACLE;

                    map[Point(x, y)].Asset = Asset::Type::WALL;
                }
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

    void Generate(Map::Base &map, int max_rooms, int min_size, int max_size)
    {
        auto rooms = std::vector<Room::Base>();

        auto random = Random::Base();

        // clear
        for (auto y = 0; y < map.Height; y++)
        {
            for (auto x = 0; x < map.Width; x++)
            {
                map[Point(x, y)].Type = Map::Object::NONE;

                map[Point(x, y)].Asset = Asset::Type::IMPASSABLE;
            }
        }

        for (auto r = 0; r < max_rooms; r++)
        {
            auto width = random.NextInt(min_size, max_size);

            auto height = random.NextInt(min_size, max_size);

            auto x = random.NextInt(0, map.Width - width - 1);

            auto y = random.NextInt(0, map.Height - height - 1);

            auto room = Room::Base(x, y, width, height);

            if (!room.Intersects(rooms))
            {
                Battlepits::Place(map, room);

                auto &last = rooms.back();

                if (rooms.size() > 0)
                {
                    for (auto point : Battlepits::TunnelBetween(room.Center(), last.Center()))
                    {
                        if (!Battlepits::Inside(rooms, room, point))
                        {
                            if (map[point].Type != Map::Object::PASSABLE)
                            {
                                map[point].Type = Map::Object::PASSABLE;

                                map[point].Asset = Asset::Type::NONE;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }

                rooms.push_back(room);
            }
        }
    }

    Map::Base Generate(int width, int height, int max_rooms, int min_size, int max_size)
    {
        auto map = Map::Base(width, height);

        Battlepits::Generate(map, max_rooms, min_size, max_size);

        return map;
    }
}

#endif
