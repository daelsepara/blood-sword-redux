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

        // return room's center coordinates
        Point Center()
        {
            auto x = this->X1 + this->Width / 2;

            auto y = this->Y1 + this->Height / 2;

            return Point(x, y);
        }

        // return room's inner area boundaries
        std::pair<Point, Point> Inner()
        {
            return {Point(this->X1 + 1, this->Y1 + 1), Point(this->X2 - 1, this->Y2 - 1)};
        }

        Base(int x, int y, int width, int height) : X1(x), Y1(y), Width(width), Height(height)
        {
            this->X2 = this->X1 + this->Width;

            this->Y2 = this->Y1 + this->Height;
        }

        // check if point is inside the room
        bool Inside(Point point)
        {
            return ((point.X > this->X1 && point.X < this->X2 - 1) && (point.Y > this->Y1 && point.Y < this->Y2 - 1));
        }

        // check if a room intersects another room
        bool Intersects(const Room::Base &room)
        {
            return (this->X1 <= room.X2 && this->X2 >= room.X1 && this->Y1 <= room.Y2 && this->Y2 >= room.Y1);
        }

        // check if room intersects any of the other rooms
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

    // check if point is inside any of the other rooms
    bool Inside(std::vector<Room::Base> &rooms, Room::Base &room, Point point)
    {
        auto result = room.Inside(point);

        for (auto room : rooms)
        {
            result |= room.Inside(point);
        }

        return result;
    }
}

namespace BloodSword::Battlepits
{
    // create vertical tunnel between two points and add to list
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

    // create horizontal tunnel between two points and add to list
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

    // create tunnels between two points
    Points CreateTunnels(Point start, Point end)
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

    // randomly place room in the map
    void Place(Map::Base &map, Room::Base &room)
    {
        // create walls around the room
        for (auto y = room.Y1; y < room.Y2; y++)
        {
            for (auto x = room.X1; x < room.X2; x++)
            {
                auto point = Point(x, y);

                if (room.Inside(point))
                {
                    // carve out inner room area
                    map[point].Type = Map::Object::PASSABLE;

                    map[point].Asset = Asset::Type::NONE;
                }
                else if (map[point].Type != Map::Object::PASSABLE)
                {
                    // do not overwrite tunnels
                    map[point].Type = Map::Object::OBSTACLE;

                    map[point].Asset = Asset::Type::WALL;
                }
            }
        }
    }

    // clears the entire map
    void ClearMap(BloodSword::Map::Base &map, Asset::Type asset)
    {
        for (auto y = 0; y < map.Height; y++)
        {
            for (auto x = 0; x < map.Width; x++)
            {
                auto point = Point(x, y);

                map[point].Type = Map::Object::NONE;

                map[point].Asset = asset;
            }
        }
    }

    // connect rooms with a tunnel
    void ConnectRooms(Map::Base &map, std::vector<Room::Base> &rooms, Room::Base &room, Room::Base &last)
    {
        auto tunnel = Battlepits::CreateTunnels(room.Center(), last.Center());

        for (auto point : tunnel)
        {
            if (!Room::Inside(rooms, room, point))
            {
                // lay out tunnel but stop when it encounters //  an existing
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

    // create room with random dimensions
    Room::Base CreateRoom(Random::Base &random, Map::Base &map, int min_size, int max_size)
    {
        auto width = random.NextInt(min_size, max_size);

        auto height = random.NextInt(min_size, max_size);

        auto x = random.NextInt(0, map.Width - width - 1);

        auto y = random.NextInt(0, map.Height - height - 1);

        return Room::Base(x, y, width, height);
    }

    // draw tunnel borders and corners within the tile / surrounding tiles
    bool InnerTunnel = false;

    typedef bool (*Checker)(Map::Base &map, Point point);

    // tile is empty
    bool Empty(Map::Base &map, Point point)
    {
        return (map[point].Asset == Asset::Type::NONE && map[point].Type == Map::Object::NONE);
    }

    // tile is empty, has no assigned asset and is not passable
    bool Blocked(Map::Base &map, Point point)
    {
        auto &tile = map[point];

        return (tile.Asset == Asset::Type::NONE && (tile.Type == Map::Object::NONE || (tile.Type != Map::Object::PASSABLE && tile.Type != Map::Object::ENEMY_PASSABLE)));
    }

    // tile is passable and has not assigned asset
    bool Passable(Map::Base &map, Point point)
    {
        auto &tile = map[point];

        return (tile.Asset == Asset::Type::NONE && tile.Type == Map::Object::PASSABLE);
    }

    // assign assets to fix rendering of corners
    void AssignCornerAssets(BloodSword::Map::Base &map, BloodSword::Map::Tile &tile, int x, int y)
    {
        Battlepits::Checker Filled = InnerTunnel ? Empty : Passable;

        auto tl = Filled(map, {x - 1, y - 1});

        auto tr = Filled(map, {x + 1, y - 1});

        auto bl = Filled(map, {x - 1, y + 1});

        auto br = Filled(map, {x + 1, y + 1});

        if (tl && tr && bl && br)
        {
            tile.Asset = Asset::Type::FOUR_CORNERS;
        }
        else if (tr && tl && bl)
        {
            tile.Asset = Asset::Type::TOP_LEFT_CORNERS;
        }
        else if (tl && tr && br)
        {
            tile.Asset = Asset::Type::TOP_RIGHT_CORNERS;
        }
        else if (tl && bl && br)
        {
            tile.Asset = Asset::Type::BOTTOM_LEFT_CORNERS;
        }
        else if (bl && br && tr)
        {
            tile.Asset = Asset::Type::BOTTOM_RIGHT_CORNERS;
        }
        else if (tl && tr)
        {
            tile.Asset = Asset::Type::TOP_CORNERS;
        }
        else if (bl && br)
        {
            tile.Asset = Asset::Type::BOTTOM_CORNERS;
        }
        else if (tl && bl)
        {
            tile.Asset = Asset::Type::LEFT_CORNERS;
        }
        else if (tr && br)
        {
            tile.Asset = Asset::Type::RIGHT_CORNERS;
        }
        else if (tl)
        {
            tile.Asset = Asset::Type::TOP_LEFT_CORNER;
        }
        else if (tr)
        {
            tile.Asset = Asset::Type::TOP_RIGHT_CORNER;
        }
        else if (bl)
        {
            tile.Asset = Asset::Type::BOTTOM_LEFT_CORNER;
        }
        else if (br)
        {
            tile.Asset = Asset::Type::BOTTOM_RIGHT_CORNER;
        }
    }

    void AssignTunnelAssets(BloodSword::Map::Base &map, BloodSword::Map::Tile &tile, int x, int y)
    {
        Battlepits::Checker Check = InnerTunnel ? Blocked : Passable;

        Battlepits::Checker Filled = InnerTunnel ? Empty : Passable;

        // check if tile is on the edges
        auto edge_top = InnerTunnel ? (y == 0) : false;

        auto edge_left = InnerTunnel ? (x == 0) : false;

        auto edge_right = InnerTunnel ? (x >= map.Width - 1) : false;

        auto edge_bottom = InnerTunnel ? (y >= map.Height - 1) : false;

        // check borders
        auto top = edge_top || Check(map, {x, y - 1});

        auto left = edge_left || Check(map, {x - 1, y});

        auto right = edge_right || Check(map, {x + 1, y});

        auto bottom = edge_bottom || Check(map, {x, y + 1});

        if (top && left && right)
        {
            tile.Asset = Asset::Type::BORDER_BOTTOM_OPEN;
        }
        else if (top && right && bottom)
        {
            tile.Asset = Asset::Type::BORDER_LEFT_OPEN;
        }
        else if (left && right && bottom)
        {
            tile.Asset = Asset::Type::BORDER_TOP_OPEN;
        }
        else if (top && left && bottom)
        {
            tile.Asset = Asset::Type::BORDER_RIGHT_OPEN;
        }
        else if (top && left)
        {
            if (Filled(map, {x + 1, y + 1}))
            {
                tile.Asset = Asset::Type::TOP_LEFT_ELBOW;
            }
            else
            {
                tile.Asset = Asset::Type::BORDER_TOP_LEFT;
            }
        }
        else if (top && right)
        {
            if (Filled(map, {x - 1, y + 1}))
            {
                tile.Asset = Asset::Type::TOP_RIGHT_ELBOW;
            }
            else
            {
                tile.Asset = Asset::Type::BORDER_TOP_RIGHT;
            }
        }
        else if (bottom && right)
        {
            if (Filled(map, {x - 1, y - 1}))
            {
                tile.Asset = Asset::Type::BOTTOM_RIGHT_ELBOW;
            }
            else
            {
                tile.Asset = Asset::Type::BORDER_BOTTOM_RIGHT;
            }
        }
        else if (bottom && left)
        {
            if (Filled(map, {x + 1, y - 1}))
            {
                tile.Asset = Asset::Type::BOTTOM_LEFT_ELBOW;
            }
            else
            {
                tile.Asset = Asset::Type::BORDER_BOTTOM_LEFT;
            }
        }
        else if (top && bottom)
        {
            tile.Asset = Asset::Type::BORDER_HORIZONTAL;
        }
        else if (left && right)
        {
            tile.Asset = Asset::Type::BORDER_VERTICAL;
        }
        else if (top)
        {
            if (Filled(map, {x - 1, y + 1}) && Filled(map, {x + 1, y + 1}))
            {
                tile.Asset = Asset::Type::TOP_BOTTOM_CORNERS;
            }
            else if (Filled(map, {x - 1, y + 1}))
            {
                tile.Asset = Asset::Type::TOP_BOTTOM_LEFT_CORNER;
            }
            else if (Filled(map, {x + 1, y + 1}))
            {
                tile.Asset = Asset::Type::TOP_BOTTOM_RIGHT_CORNER;
            }
            else
            {
                tile.Asset = Asset::Type::BORDER_TOP;
            }
        }
        else if (bottom)
        {
            if (Filled(map, {x - 1, y - 1}) && Filled(map, {x + 1, y - 1}))
            {
                tile.Asset = Asset::Type::BOTTOM_TOP_CORNERS;
            }
            else if (Filled(map, {x - 1, y - 1}))
            {
                tile.Asset = Asset::Type::BOTTOM_TOP_LEFT_CORNER;
            }
            else if (Filled(map, {x + 1, y - 1}))
            {
                tile.Asset = Asset::Type::BOTTOM_TOP_RIGHT_CORNER;
            }
            else
            {
                tile.Asset = Asset::Type::BORDER_BOTTOM;
            }
        }
        else if (left)
        {
            if (Filled(map, {x + 1, y - 1}) && Filled(map, {x + 1, y + 1}))
            {
                tile.Asset = Asset::Type::LEFT_RIGHT_CORNERS;
            }
            else if (Filled(map, {x + 1, y - 1}))
            {
                tile.Asset = Asset::Type::LEFT_TOP_RIGHT_CORNER;
            }
            else if (Filled(map, {x + 1, y + 1}))
            {
                tile.Asset = Asset::Type::LEFT_BOTTOM_RIGHT_CORNER;
            }
            else
            {
                tile.Asset = Asset::Type::BORDER_LEFT;
            }
        }
        else if (right)
        {
            if (Filled(map, {x - 1, y - 1}) && Filled(map, {x - 1, y + 1}))
            {
                tile.Asset = Asset::Type::RIGHT_LEFT_CORNERS;
            }
            else if (Filled(map, {x - 1, y - 1}))
            {
                tile.Asset = Asset::Type::RIGHT_TOP_LEFT_CORNER;
            }
            else if (Filled(map, {x - 1, y + 1}))
            {
                tile.Asset = Asset::Type::RIGHT_BOTTOM_LEFT_CORNER;
            }
            else
            {
                tile.Asset = Asset::Type::BORDER_RIGHT;
            }
        }
        else
        {
            // non-filled spaces
            Battlepits::AssignCornerAssets(map, tile, x, y);
        }
    }

    void AssignTunnelAssets(Map::Base &map)
    {
        for (auto y = 0; y < map.Height; y++)
        {
            for (auto x = 0; x < map.Width; x++)
            {
                auto &tile = map[Point(x, y)];

                auto tile_type = InnerTunnel ? Map::Object::PASSABLE : Map::Object::NONE;

                if (tile.Type == tile_type)
                {
                    Battlepits::AssignTunnelAssets(map, tile, x, y);
                }
            }
        }
    }

    void Generate(Map::Base &map, std::vector<Room::Base> &rooms, int max_rooms, int min_size, int max_size)
    {
        // initialize RNG
        auto random = Random::Base();

        // clear rooms
        rooms.clear();

        // clear entire map
        Battlepits::ClearMap(map, Asset::Type::NONE);

        for (auto r = 0; r < max_rooms; r++)
        {
            auto room = Battlepits::CreateRoom(random, map, min_size, max_size);

            // check if room placement is valid
            if (!room.Intersects(rooms))
            {
                // place room
                Battlepits::Place(map, room);

                // if not the first room, create tunnel to previous room
                if (rooms.size() > 0)
                {
                    auto &last = rooms.back();

                    Battlepits::ConnectRooms(map, rooms, room, last);
                }

                rooms.push_back(room);
            }
        }

        Battlepits::AssignTunnelAssets(map);
    }

    Map::Base Generate(int width, int height, int max_rooms, int min_size, int max_size)
    {
        auto map = Map::Base(width, height);

        auto rooms = std::vector<Room::Base>();

        Battlepits::Generate(map, rooms, max_rooms, min_size, max_size);

        return map;
    }
}

#endif
