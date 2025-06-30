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
        return (double)(x * x + y * y);
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
        return (x < 0 || x >= map.Width || y < 0 || y >= map.Height) || (map(x, y).IsOccupied() && map(x, y).Occupant != Map::Object::ITEMS) || map(x, y).IsBlocked();
    }

    bool BlocksLight(Map::Base &map, int x, int y, int octant, Point origin)
    {
        auto point = TranslateLocalToMap(x, y, origin, octant);

        return (x < 0 || x >= map.Width || y < 0 || y >= map.Height) || (map[point].IsOccupied() && map[point].Occupant != Map::Object::ITEMS) || map[point].IsBlocked();
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
            auto radius_sq = (double)(radius * radius);

            while (x <= radius)
            {
                auto topY = Diamond::ComputeTopY(map, top, octant, origin, x);

                auto bottomY = bottom.Y == 0 ? 0 : ((x * 2 - 1) * bottom.Y + bottom.X) / (bottom.X * 2);

                std::optional<bool> wasOpaque = std::nullopt;

                for (auto y = topY; y >= bottomY; y--)
                {
                    auto t = TranslateLocalToMap(x, y, origin, octant);

                    auto inRange = radius < 0 || GetDistance(x, y) <= radius_sq;

                    if (inRange)
                    {
                        if (!BloodSword::In(visible, t))
                        {
                            visible.push_back(t);
                        }
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

                            Diamond::ComputeOctant(map, visible, octant, origin, radius, x + 1, top, Slope(x * 2, y * 2 + 1));
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

                x++;
            }
        }

        Points Compute(Map::Base &map, Point origin, int radius)
        {
            Points visible = {origin};

            for (auto octant = 0; octant < 8; octant++)
            {
                Diamond::ComputeOctant(map, visible, octant, origin, radius, 1, Slope(1, 1), Slope(1, 0));
            }

            return visible;
        }
    }

    namespace ShadowCast
    {
        void ComputeOctant(Map::Base &map, Points &visible, int octant, Point origin, int radius, int x, Slope top, Slope bottom)
        {
            auto radius_sq = (double)(radius * radius);

            while (x <= radius)
            {
                auto topY = top.X == 1 ? x : ((x * 2 + 1) * top.Y + top.X - 1) / (top.X * 2);

                auto bottomY = bottom.Y == 0 ? 0 : ((x * 2 - 1) * bottom.Y + bottom.X) / (bottom.X * 2);

                std::optional<bool> wasOpaque = std::nullopt;

                for (auto y = topY; y >= bottomY; y--)
                {
                    auto t = TranslateLocalToMap(x, y, origin, octant);

                    auto inRange = radius < 0 || GetDistance(x, y) <= radius_sq;

                    if (inRange)
                    {
                        if (!BloodSword::In(visible, t))
                        {
                            visible.push_back(t);
                        }
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

                            ShadowCast::ComputeOctant(map, visible, octant, origin, radius, x + 1, top, newBottom);
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
                ShadowCast::ComputeOctant(map, visible, octant, origin, radius, 1, Slope(1, 1), Slope(1, 0));
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

            auto range_sq = (double)(rangeLimit * rangeLimit);

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

                if (rangeLimit >= 0 && GetDistance(origin, x, y) > range_sq)
                {
                    break;
                }

                auto current = Point(x, y);

                if (!BloodSword::In(visible, current))
                {
                    visible.push_back(current);
                }

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
                RayCast::TraceLine(map, visible, origin, x, area.y, radius);

                RayCast::TraceLine(map, visible, origin, x, area.y + area.h - 1, radius);
            }

            for (auto y = area.y + 1; y < area.y + area.h - 1; y++)
            {
                RayCast::TraceLine(map, visible, origin, area.x, y, radius);

                RayCast::TraceLine(map, visible, origin, area.x + area.w - 1, y, radius);
            }

            return visible;
        }
    }

    namespace Milazzo
    {
        void SetVisible(Points &visible, int x, int y, int octant, Point origin)
        {
            auto local = TranslateLocalToMap(x, y, origin, octant);

            if (!BloodSword::In(visible, local))
            {
                visible.push_back(TranslateLocalToMap(x, y, origin, octant));
            }
        }

        int ComputeTopY(Map::Base &map, int octant, Point origin, int x, Slope top)
        {
            int topY;

            if (top.X == 1)
            {
                topY = x;
            }
            else
            {
                topY = ((x * 2 - 1) * top.Y + top.X) / (top.X * 2);

                if (BlocksLight(map, x, topY, octant, origin))
                {
                    if (top.GreaterOrEqual(topY * 2 + 1, x * 2) && !BlocksLight(map, x, topY + 1, octant, origin))
                    {
                        topY++;
                    }
                }
                else
                {
                    auto ax = x * 2;

                    if (BlocksLight(map, x + 1, topY + 1, octant, origin))
                    {
                        ax++;
                    }

                    if (top.Greater(topY * 2 + 1, ax))
                    {
                        topY++;
                    }
                }
            }

            return topY;
        }

        int ComputeBottomY(Map::Base &map, int octant, Point origin, int x, Slope bottom)
        {
            int bottomY;

            if (bottom.Y == 0)
            {
                bottomY = 0;
            }
            else
            {
                bottomY = ((x * 2 - 1) * bottom.Y + bottom.X) / (bottom.X * 2);

                if (bottom.GreaterOrEqual(bottomY * 2 + 1, x * 2) && BlocksLight(map, x, bottomY, octant, origin) && !BlocksLight(map, x, bottomY + 1, octant, origin))
                {
                    bottomY++;
                }
            }

            return bottomY;
        }

        void ComputeOctant(Map::Base &map, Points &visible, int octant, Point origin, int radius, int x, Slope top, Slope bottom)
        {
            auto radius_sq = (double)(radius * radius);

            while (x <= radius)
            {
                auto topY = Milazzo::ComputeTopY(map, octant, origin, x, top);

                auto bottomY = Milazzo::ComputeBottomY(map, octant, origin, x, bottom);

                std::optional<bool> wasOpaque = std::nullopt;

                for (auto y = topY; y >= bottomY; y--)
                {
                    if (radius >= 0 && GetDistance(x, y) > radius_sq)
                    {
                        continue;
                    }

                    auto isOpaque = BlocksLight(map, x, y, octant, origin);

                    auto isVisible = isOpaque || ((y != topY || top.Greater(y * 4 - 1, x * 4 + 1)) && (y != bottomY || bottom.Less(y * 4 + 1, x * 4 - 1)));

                    if (isVisible)
                    {
                        Milazzo::SetVisible(visible, x, y, octant, origin);
                    }

                    if (x == radius)
                    {
                        continue;
                    }

                    if (isOpaque)
                    {
                        if (wasOpaque == false)
                        {
                            auto nx = x * 2;

                            auto ny = y * 2 + 1;

                            if (BlocksLight(map, x, y + 1, octant, origin))
                            {
                                nx--;
                            }

                            if (top.Greater(ny, nx))
                            {
                                if (y == bottomY)
                                {
                                    bottom = Slope(nx, ny);

                                    break;
                                }

                                Milazzo::ComputeOctant(map, visible, octant, origin, radius, x + 1, top, Slope(nx, ny));
                            }
                            else if (y == bottomY)
                            {
                                return;
                            }
                        }

                        wasOpaque = true;
                    }
                    else
                    {
                        if (wasOpaque == true)
                        {
                            auto nx = x * 2;

                            auto ny = y * 2 + 1;

                            if (BlocksLight(map, x + 1, y + 1, octant, origin))
                            {
                                nx++;
                            }

                            if (bottom.GreaterOrEqual(ny, nx))
                            {
                                return;
                            }

                            top = Slope(nx, ny);
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
                Milazzo::ComputeOctant(map, visible, octant, origin, radius, 1, Slope(1, 1), Slope(1, 0));
            }

            return visible;
        }
    }

    namespace ShadowCastBinary
    {
        const double BrightnessThreshold = 0.0;

        void CastLight(Map::Base &map, Points &visible, Point origin, int radius, int row, double start, double end, int xx, int xy, int yx, int yy)
        {
            auto radius_sq = (double)(radius * radius);

            auto newStart = 0.0;

            if (start < end)
            {
                return;
            }

            auto blocked = false;

            for (auto distance = row; distance <= radius && !blocked; distance++)
            {
                auto deltaY = -distance;

                for (auto deltaX = -distance; deltaX <= 0; deltaX++)
                {
                    auto currentX = origin.X + deltaX * xx + deltaY * xy;

                    auto currentY = origin.Y + deltaX * yx + deltaY * yy;

                    auto leftSlope = (deltaX - 0.5) / (deltaY + 0.5);

                    auto rightSlope = (deltaX + 0.5) / (deltaY - 0.5);

                    if (!(currentX >= 0 && currentY >= 0 && currentX < map.Width && currentY < map.Height) || start < rightSlope)
                    {
                        continue;
                    }
                    else if (end > leftSlope)
                    {
                        break;
                    }

                    // check if it's within the lightable area and light if needed
                    if (GetDistance(deltaX, deltaY) <= radius_sq)
                    {
                        auto bright = (1.0 - (GetDistance(deltaX, deltaY) / radius_sq));

                        auto current = Point(currentX, currentY);

                        if (bright >= BrightnessThreshold && !BloodSword::In(visible, current))
                        {
                            visible.push_back(current);
                        }
                    }

                    if (blocked)
                    {
                        // previous cell was a blocking one
                        if (BlocksLight(map, currentX, currentY))
                        {
                            // hit a wall
                            newStart = rightSlope;

                            continue;
                        }
                        else
                        {
                            blocked = false;

                            start = newStart;
                        }
                    }
                    else
                    {
                        if (BlocksLight(map, currentX, currentY) && distance < radius)
                        {
                            // hit a wall within sight line
                            blocked = true;

                            CastLight(map, visible, origin, radius, distance + 1, start, leftSlope, xx, xy, yx, yy);

                            newStart = rightSlope;
                        }
                    }
                }
            }
        }

        Points Compute(Map::Base &map, Point origin, int radius)
        {
            Points lightMap = {origin};

            for (auto delta : Map::Diagonals)
            {
                CastLight(map, lightMap, origin, radius, 1, 1.0, 0.0, 0, delta.X, delta.Y, 0);

                CastLight(map, lightMap, origin, radius, 1, 1.0, 0.0, delta.X, 0, 0, delta.Y);
            }

            return lightMap;
        }
    }

    enum class Method
    {
        NONE = -1,
        DIAMOND,
        SHADOW_CAST,
        RAY_CAST,
        MILAZZO,
        BINARY
    };

    BloodSword::Mapping<FieldOfView::Method> MethodMapping = {
        {Method::NONE, "NONE"},
        {Method::DIAMOND, "DIAMOND"},
        {Method::SHADOW_CAST, "SHADOW CAST"},
        {Method::RAY_CAST, "RAY CAST"},
        {Method::MILAZZO, "MILAZZO"},
        {Method::BINARY, "BINARY"}};

    FieldOfView::Method Map(const char *method)
    {
        return BloodSword::Find(FieldOfView::MethodMapping, method);
    }

    FieldOfView::Method Map(std::string method)
    {
        return FieldOfView::Map(method.c_str());
    }

    Points Compute(Map::Base &map, Point origin, int radius, FieldOfView::Method method)
    {
        Points points = {};

        switch (method)
        {
        case Method::DIAMOND:

            points = Diamond::Compute(map, origin, radius);

            break;

        case Method::SHADOW_CAST:

            points = ShadowCast::Compute(map, origin, radius);

            break;

        case Method::RAY_CAST:

            points = RayCast::Compute(map, origin, radius);

            break;

        case Method::MILAZZO:

            points = Milazzo::Compute(map, origin, radius);

            break;

        case Method::BINARY:

            points = ShadowCastBinary::Compute(map, origin, radius);

            break;

        default:

            points = Diamond::Compute(map, origin, radius);

            break;
        }

        return points;
    }
}

#endif
