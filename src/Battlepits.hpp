#ifndef __BATTLEPITS_HPP__
#define __BATTLEPITS_HPP__

#include "FieldOfView.hpp"
#include "Random.hpp"

namespace BloodSword::Room
{
    const int None = -1;

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
    enum class Connection
    {
        NONE = -1,
        TUNNELS,
        WALLS
    };

    BloodSword::Mapping<Battlepits::Connection> ConnectionMapping = {
        {Connection::NONE, "NONE"},
        {Connection::TUNNELS, "TUNNELS"},
        {Connection::WALLS, "WALLS"}};

    Battlepits::Connection Map(const char *connection)
    {
        return BloodSword::Find(Battlepits::ConnectionMapping, connection);
    }

    Battlepits::Connection Map(std::string connection)
    {
        return Battlepits::Map(connection.c_str());
    }

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
    void Place(Map::Base &map, Room::Base &room, int room_id)
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

                    map[point].Room = room_id;
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

                map[point].Occupant = Map::Object::NONE;

                map[point].Explored = false;

                map[point].Asset = asset;

                map[point].TemporaryAsset = Asset::Type::NONE;

                map[point].Lifetime = Map::Unlimited;

                map[point].Room = Room::None;

                map[point].Id = Map::NotFound;
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

    typedef bool (*Checker)(Map::Base &map, Point point);

    bool Empty(Map::Base &map, Point point)
    {
        return (map[point].Type == Map::Object::NONE);
    }

    bool Blocked(Map::Base &map, Point point)
    {
        auto &tile = map[point];

        return tile.Type == Map::Object::NONE || (tile.Type != Map::Object::PASSABLE && tile.Type != Map::Object::ENEMY_PASSABLE);
    }

    bool Passable(Map::Base &map, Point point)
    {
        auto &tile = map[point];

        return tile.Type == Map::Object::PASSABLE;
    }

    void AssignCornerAssets(BloodSword::Map::Base &map, BloodSword::Map::Tile &tile, int x, int y, bool inner_tunnel)
    {
        Battlepits::Checker Filled = inner_tunnel ? Empty : Passable;

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

    void AssignTunnelAssets(BloodSword::Map::Base &map, BloodSword::Map::Tile &tile, int x, int y, bool inner_tunnel)
    {
        Battlepits::Checker Check = inner_tunnel ? Blocked : Passable;

        Battlepits::Checker Filled = inner_tunnel ? Empty : Passable;

        auto top_edge = (y == 0);

        auto left_edge = (x == 0);

        auto right_edge = (x >= map.Width - 1);

        auto bottom_edge = (y >= map.Height - 1);

        auto top = top_edge || Check(map, {x, y - 1});

        auto left = left_edge || Check(map, {x - 1, y});

        auto right = right_edge || Check(map, {x + 1, y});

        auto bottom = bottom_edge || Check(map, {x, y + 1});

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
            else if (!(top_edge && left_edge))
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
            else if (!(top_edge && right_edge))
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
            else if (!(bottom_edge && right_edge))
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
            else if (!(bottom_edge && left_edge))
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
            else if (!top_edge)
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
            else if (!bottom_edge)
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
            else if (!left_edge)
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
            else if (!right_edge)
            {
                tile.Asset = Asset::Type::BORDER_RIGHT;
            }
        }
        else
        {
            // non-blocked and passable space
            Battlepits::AssignCornerAssets(map, tile, x, y, inner_tunnel);
        }
    }

    void SetTunnelWalls(Map::Base &map)
    {
        for (auto y = 1; y < map.Height - 1; y++)
        {
            for (auto x = 0; x < map.Width - 1; x++)
            {
                auto &tile = map[Point(x, y)];

                if (tile.Type == Map::Object::NONE)
                {
                    auto top = Passable(map, {x, y - 1});

                    auto left = Passable(map, {x - 1, y});

                    auto right = Passable(map, {x + 1, y});

                    auto bottom = Passable(map, {x, y + 1});

                    auto top_left = Passable(map, {x - 1, y - 1});

                    auto top_right = Passable(map, {x + 1, y - 1});

                    auto bottom_left = Passable(map, {x - 1, y + 1});

                    auto bottom_right = Passable(map, {x + 1, y + 1});

                    if (top || left || right || bottom || top_right || top_left || bottom_left || bottom_right)
                    {
                        tile.Asset = Asset::Type::WALL;

                        tile.Type = Map::Object::OBSTACLE;
                    }
                }
            }
        }
    }

    void AssignTunnelAssets(Map::Base &map, bool inner_tunnel)
    {
        for (auto y = 0; y < map.Height; y++)
        {
            for (auto x = 0; x < map.Width; x++)
            {
                auto &tile = map[Point(x, y)];

                auto tile_type = inner_tunnel ? Map::Object::PASSABLE : Map::Object::NONE;

                if (tile.Type == tile_type)
                {
                    Battlepits::AssignTunnelAssets(map, tile, x, y, inner_tunnel);
                }
            }
        }
    }

    void Generate(Map::Base &map, std::vector<Room::Base> &rooms, int max_rooms, int min_size, int max_size, Battlepits::Connection connection, bool inner_tunnel)
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
                auto room_id = int(rooms.size());

                Battlepits::Place(map, room, room_id);

                // if not the first room, create tunnel to previous room
                if (room_id > 0)
                {
                    auto &last = rooms.back();

                    Battlepits::ConnectRooms(map, rooms, room, last);
                }

                rooms.push_back(room);
            }
        }

        if (connection == Connection::TUNNELS)
        {
            Battlepits::AssignTunnelAssets(map, inner_tunnel);
        }
        else if (connection == Connection::WALLS)
        {
            Battlepits::SetTunnelWalls(map);
        }
    }

    Map::Base Generate(int width, int height, int max_rooms, int min_size, int max_size, Battlepits::Connection connection, bool inner_tunnel)
    {
        auto map = Map::Base(width, height);

        auto rooms = std::vector<Room::Base>();

        Battlepits::Generate(map, rooms, max_rooms, min_size, max_size, connection, inner_tunnel);

        return map;
    }
}

#endif
