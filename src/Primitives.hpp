#ifndef __PRIMITIVES_HPP__
#define __PRIMITIVES_HPP__

#if defined(_WIN32)
#define NOMINMAX
#undef min
#undef max
#endif

#include <iomanip>
#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Version.hpp"

namespace BloodSword
{
    typedef std::vector<SDL_Texture *> Textures;

    // default tile dimensions
    const int TileSize = 64;

    const int HalfTile = 32;

    const int QuarterTile = 16;

    const int DoubleTile = TileSize * 2;

    const int TripleTile = TileSize * 3;

    const int QuadTile = TileSize * 4;

    const int PentaTile = TileSize * 5;

    const int OctaTile = TileSize * 8;

    const int DecaTile = TileSize * 10;

    const int Wrap = TileSize * 6;

    const int PanelSize = TileSize * 9;

    const int DescriptionSize = TileSize * 15;

    // max moves
    const int MaximumMoves = 1000;

    // padding sizes
    const int HugePad = 32;

    const int LargePad = 16;

    const int OddPad = 10;

    const int Pad = 8;

    const int MidPad = 6;

    const int SmallPad = 4;

    const int Pixel = 2;

    // delay constants
    const int FrameDelay = 25;

    const int StandardDelay = 100;

    const int OneSecond = 1000;

    const int TwoSeconds = 2000;

    // border constants
    const int Border = 4;

    // scroll speed (pixels)
    const int ScrollSpeed = 50;

    // cartesian coordinates
    class Point
    {
    public:
        int X;

        int Y;

        Point(int x, int y) : X(x), Y(y) {}

        Point() {}

        Point &operator*=(const Point &p)
        {
            this->X *= p.X;

            this->Y *= p.Y;

            return *this;
        }

        Point &operator+=(const Point &p)
        {
            this->X += p.X;

            this->Y += p.Y;

            return *this;
        }

        Point &operator-=(const Point &p)
        {
            this->X -= p.X;

            this->Y -= p.Y;

            return *this;
        }

        bool operator==(const Point &p)
        {
            return this->X == p.X && this->Y == p.Y;
        }

        bool operator!=(const Point &p)
        {
            return !(*this == p);
        }

        Point operator+(const Point &p)
        {
            return Point(this->X + p.X, this->Y + p.Y);
        }

        Point operator-(const Point &p)
        {
            return Point(this->X - p.X, this->Y - p.Y);
        }

        Point operator*(const Point &p)
        {
            return Point(this->X * p.X, this->Y * p.Y);
        }

        Point operator+(int p)
        {
            return Point(this->X + p, this->Y + p);
        }

        Point operator-(int p)
        {
            return *this + (-p);
        }

        Point operator*(int p)
        {
            return Point(this->X * p, this->Y * p);
        }

        Point operator/(const Point &p)
        {
            return Point(this->X / p.X, this->Y / p.Y);
        }

        Point operator/(int p)
        {
            return Point(this->X / p, this->Y / p);
        }

        // check if point is a valid map coordinate
        bool IsNone()
        {
            return *this == Point(-1, -1);
        }
    };

    typedef std::vector<Point> Points;

    bool In(Points &points, Point point)
    {
        auto result = false;

        for (auto &item : points)
        {
            if (item == point)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    bool In(Points &points, int x, int y)
    {
        return In(points, Point(x, y));
    }

    // load an image as an SDL surface
    SDL_Surface *Load(const char *image)
    {
        auto surface = IMG_Load(image);

        if (!surface)
        {
            std::cerr << "Unable to load image " << image << "! SDL Error: " << SDL_GetError() << std::endl;
        }

        return surface;
    }

    // free surface
    void Free(SDL_Surface **surface)
    {
        if (*surface != nullptr)
        {
            SDL_FreeSurface(*surface);

            *surface = nullptr;
        }
    }

    // free texture
    void Free(SDL_Texture **texture)
    {
        if (*texture != nullptr)
        {
            SDL_DestroyTexture(*texture);

            *texture = nullptr;
        }
    }

    // free textures
    void Free(BloodSword::Textures &textures)
    {
        if (!textures.empty())
        {
            for (auto &texture : textures)
            {
                BloodSword::Free(&texture);
            }

            textures.clear();
        }
    }

    template <typename T>
    void Free(std::unordered_map<T, SDL_Texture *> &textures)
    {
        if (!textures.empty())
        {
            for (auto &texture : textures)
            {
                BloodSword::Free(&texture.second);
            }

            textures.clear();
        }
    }

    // left pad string
    std::string LeftPad(std::string &str, int n)
    {
        if (str.size() < n)
        {
            std::ostringstream oss;

            oss << std::setw(n) << std::left << str;

            return oss.str();
        }
        else
        {
            return str;
        }
    }

    std::string CleanString(std::string text, const char *chars)
    {
        for (auto i = 0; i < strlen(chars); ++i)
        {
            text.erase(std::remove(text.begin(), text.end(), chars[i]), text.end());
        }

        return text;
    }

    void Size(SDL_Texture *texture, int *texture_w, int *texture_h)
    {
        if (texture)
        {
            SDL_QueryTexture(texture, nullptr, nullptr, texture_w, texture_h);
        }
    }

    int Width(SDL_Texture *texture)
    {
        auto width = 0;

        BloodSword::Size(texture, &width, nullptr);

        return width;
    }

    int Width(Textures &textures)
    {
        auto width = 0;

        for (auto i = 0; i < textures.size(); i++)
        {
            width = std::max(width, BloodSword::Width(textures[i]));
        }

        return width;
    }

    int Height(SDL_Texture *texture)
    {
        auto height = 0;

        BloodSword::Size(texture, nullptr, &height);

        return height;
    }

    int Height(Textures &textures)
    {
        auto height = 0;

        for (auto i = 0; i < textures.size(); i++)
        {
            height = std::max(height, BloodSword::Height(textures[i]));
        }

        return height;
    }
}

#endif
