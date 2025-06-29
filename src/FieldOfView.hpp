#ifndef __FIELD_OF_HPP__
#define __FIELD_OF_HPP__

#include <optional>

#include "Map.hpp"

// adapted from: https://github.com/BenMakesGames/FoV
namespace BloodSword::FieldOfView
{
    class Slope
    {
    public:
        int X = 0;

        int Y = 0;

        Slope(int x, int y) : X(x), Y(y) {}

        Slope() {}

        bool Greater(int y, int x) { return this->Y * x > this->X * y; }

        bool GreaterOrEqual(int y, int x) { return this->Y * x >= this->X * y; }

        bool Less(int y, int x) { return this->Y * x < this->X * y; }

        bool LessOrEqual(int y, int x) { return this->Y * x <= this->X * y; }
    };

    double GetDistance(int x, int y)
    {
        return std::sqrt(x * x + y * y);
    }

    double GetDistance(Point origin, int x, int y)
    {
        auto dx = origin.X - x;

        auto dy = origin.Y - y;

        return GetDistance(dx, dy);
    }

    Point TranslateLocalToMap(int x, int y, Point origin, int octant)
    {
        Point point = {-1, -1};

        switch (octant)
        {
        case 0:

            point = Point(origin.X + x, origin.Y - y);

            break;

        case 1:

            point = Point(origin.X + y, origin.Y - x);

            break;

        case 2:

            point = Point(origin.X - y, origin.Y - x);

            break;

        case 3:

            point = Point(origin.X - x, origin.Y - y);

            break;

        case 4:

            point = Point(origin.X - x, origin.Y + y);

            break;

        case 5:

            point = Point(origin.X - y, origin.Y + x);

            break;

        case 6:

            point = Point(origin.X + y, origin.Y + x);

            break;

        case 7:

            point = Point(origin.X + x, origin.Y + y);

            break;

        default:

            throw std::invalid_argument("Octant must be between 0 and 7 inclusive.");

            break;
        }

        return point;
    }

    bool BlocksLight(Map::Base &map, int x, int y)
    {
        return (x < 0 || x >= map.Width || y < 0 || y >= map.Height) || map(x, y).IsOccupied() || map(x, y).IsBlocked();
    }

    bool BlocksLight(Map::Base &map, int x, int y, int octant, Point origin)
    {
        auto point = TranslateLocalToMap(x, y, origin, octant);

        return (x < 0 || x >= map.Width || y < 0 || y >= map.Height) || map[point].IsOccupied() || map[point].IsBlocked();
    }

    int Sign(int value)
    {
        return value < 0 ? -1 : (value > 0 ? 1 : 0);
    }

    namespace Diamond
    {
        int ComputeTopY(Map::Base &map, Slope top, int octant, Point origin, int x)
        {
            if (top.X == 1)
            {
                return x;
            }

            auto topY = ((x * 2 - 1) * top.Y + top.X) / (top.X * 2);

            auto ay = (topY * 2 + 1) * top.X;

            if (BlocksLight(map, x, topY, octant, origin))
            {
                if (top.GreaterOrEqual(ay, x * 2))
                {
                    topY++;
                }
            }
            else
            {
                if (top.Greater(ay, x * 2 + 1))
                {
                    topY++;
                }
            }

            return topY;
        }

        void ComputeOctant(Map::Base &map, Points &visible, int octant, Point origin, int radius, int x, Slope top, Slope bottom)
        {
            for (; x <= radius; x++)
            {
                auto topY = ComputeTopY(map, top, octant, origin, x);

                auto bottomY = bottom.Y == 0 ? 0 : ((x * 2 - 1) * bottom.Y + bottom.X) / (bottom.X * 2);

                std::optional<bool> wasOpaque = std::nullopt;

                for (auto y = topY; y >= bottomY; y--)
                {
                    auto t = TranslateLocalToMap(x, y, origin, octant);

                    auto inRange = radius < 0 || GetDistance(x, y) <= radius;

                    if (inRange)
                    {
                        visible.push_back(t);
                    }

                    auto isOpaque = !inRange || BlocksLight(map, t.X, t.Y);

                    if (isOpaque && ((y == topY && top.LessOrEqual(y * 2 - 1, x * 2) && !BlocksLight(map, x, y - 1, octant, origin)) || (y == bottomY && bottom.GreaterOrEqual(y * 2 + 1, x * 2) && !BlocksLight(map, x, y + 1, octant, origin))))
                    {
                        isOpaque = false;
                    }

                    if (x == radius)
                    {
                        continue;
                    }

                    if (isOpaque)
                    {
                        if (wasOpaque == false)
                        {
                            if (!inRange || y == bottomY)
                            {
                                bottom = Slope(x * 2, y * 2 + 1);

                                break;
                            }

                            ComputeOctant(map, visible, octant, origin, radius, x + 1, top, Slope(x * 2, y * 2 + 1));
                        }

                        wasOpaque = true;
                    }
                    else
                    {
                        if (wasOpaque == true)
                        {
                            top = Slope(x * 2, y * 2 + 1);
                        }

                        wasOpaque = false;
                    }
                }

                if (wasOpaque == true)
                {
                    break;
                }
            }
        }

        Points Compute(Map::Base &map, Point origin, int radius)
        {
            Points visible = {origin};

            for (auto octant = 0; octant < 8; octant++)
            {
                ComputeOctant(map, visible, octant, origin, radius, 1, Slope(1, 1), Slope(1, 0));
            }

            return visible;
        }
    }

    namespace ShadowCast
    {
        void ComputeOctant(Map::Base &map, Points &visible, int octant, Point origin, int radius, int x, Slope top, Slope bottom)
        {
            while (x <= radius)
            {
                auto topY = top.X == 1 ? x : ((x * 2 + 1) * top.Y + top.X - 1) / (top.X * 2);

                auto bottomY = bottom.Y == 0 ? 0 : ((x * 2 - 1) * bottom.Y + bottom.X) / (bottom.X * 2);

                std::optional<bool> wasOpaque = std::nullopt;

                for (auto y = topY; y >= bottomY; y--)
                {
                    auto t = TranslateLocalToMap(x, y, origin, octant);

                    auto inRange = radius < 0 || GetDistance(x, y) <= radius;

                    if (inRange)
                    {
                        visible.push_back(t);
                    }

                    auto isOpaque = !inRange || BlocksLight(map, t.X, t.Y);

                    if (x == radius)
                    {
                        continue;
                    }

                    if (isOpaque)
                    {
                        if (wasOpaque == false)
                        {
                            auto newBottom = Slope(x * 2 - 1, y * 2 + 1);

                            if (!inRange || y == bottomY)
                            {
                                bottom = newBottom;

                                break;
                            }

                            ComputeOctant(map, visible, octant, origin, radius, x + 1, top, newBottom);
                        }

                        wasOpaque = true;
                    }
                    else
                    {
                        if (wasOpaque == true)
                        {
                            top = Slope(x * 2 + 1, y * 2 + 1);
                        }

                        wasOpaque = false;
                    }
                }

                if (wasOpaque != false)
                {
                    break;
                }

                x++;
            }
        }

        Points Compute(Map::Base &map, Point origin, int radius)
        {
            Points visible = {origin};

            for (auto octant = 0; octant < 8; octant++)
            {
                ComputeOctant(map, visible, octant, origin, radius, 1, Slope(1, 1), Slope(1, 0));
            }

            return visible;
        }
    }

    namespace RayCast
    {
        void TraceLine(Map::Base &map, Points &visible, Point origin, int x2, int y2, int rangeLimit)
        {
            auto xDiff = x2 - origin.X;

            auto yDiff = y2 - origin.Y;

            auto xLen = std::abs(xDiff);

            auto yLen = std::abs(yDiff);

            auto xInc = Sign(xDiff);

            auto yInc = Sign(yDiff) << 16;

            auto index = (origin.Y << 16) + origin.X;

            if (xLen < yLen)
            {
                std::swap(xLen, yLen);

                std::swap(xInc, yInc);
            }

            auto errorInc = yLen * 2;

            auto error = -xLen;

            auto errorReset = xLen * 2;

            while (--xLen >= 0)
            {
                index += xInc;

                error += errorInc;

                if (error > 0)
                {
                    error -= errorReset;

                    index += yInc;
                }

                auto x = index & 0xFFFF;

                auto y = index >> 16;

                if (rangeLimit >= 0 && GetDistance(origin, x, y) > rangeLimit)
                {
                    break;
                }

                visible.push_back(Point(x, y));

                if (BlocksLight(map, x, y))
                {
                    break;
                }
            }
        }

        Points Compute(Map::Base &map, Point origin, int radius)
        {
            Points visible = {origin};

            if (radius == 0)
            {
                return visible;
            }

            SDL_Rect area;

            area.x = origin.X - radius;

            area.y = origin.Y - radius;

            area.w = radius * 2 + 1;

            area.h = radius * 2 + 1;

            for (auto x = area.x; x < area.x + area.w; x++)
            {
                TraceLine(map, visible, origin, x, area.y, radius);

                TraceLine(map, visible, origin, x, area.y + area.h - 1, radius);
            }

            for (auto y = area.y + 1; y < area.y + area.h - 1; y++)
            {
                TraceLine(map, visible, origin, area.x, y, radius);

                TraceLine(map, visible, origin, area.x + area.w - 1, y, radius);
            }

            return visible;
        }
    }
}

#endif
