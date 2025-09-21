#ifndef __PRIMITIVES_HPP__
#define __PRIMITIVES_HPP__

#if defined(_WIN32)
#define NOMINMAX
#undef min
#undef max
#endif

#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Version.hpp"
#include "ZipFileLibrary.hpp"

// primitive classes, functions, and constants used throughout the program
namespace BloodSword
{
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

    const int TextSize = TileSize * 12;

    const int WindowTile = QuarterTile * 18;

    const int FrameHeight = HalfTile * 5;

    // max moves
    const int MaximumMoves = 1000;

    // padding sizes
    const int EpicPad = 40;

    const int HugePad = 32;

    const int TriplePad = 24;

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
        // x, y
        int X = -1;

        int Y = -1;

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

    // list of points
    typedef std::vector<Point> Points;

    // check if point is in list of points
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

    // check if point is in list of points
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

    // load an image in a zip file as an SDL surface
    SDL_Surface *Load(const char *image, const char *zip_file)
    {
        // read file from zip archive
        auto asset = ZipFile::Read(zip_file, image);

        // create a modifiable buffer
        auto buffer = asset.data();

        // create surface from memory buffer
        auto rw = SDL_RWFromMem((void *)buffer, asset.size());

        if (!rw)
        {
            return nullptr;
        }

        // create surface and close SDL_RWops
        auto surface = IMG_Load_RW(rw, 1);

        asset.clear();

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
    void Free(std::vector<SDL_Texture *> &textures)
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

    // free resources in a map
    template <typename T, typename R>
    void Free(std::unordered_map<T, R> &resources)
    {
        if (!resources.empty())
        {
            for (auto &resource : resources)
            {
                BloodSword::Free(&resource.second);
            }

            resources.clear();
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

    // sanitize string by removing specific characters
    std::string CleanString(std::string text, const char *chars)
    {
        for (auto i = 0; i < strlen(chars); ++i)
        {
            text.erase(std::remove(text.begin(), text.end(), chars[i]), text.end());
        }

        return text;
    }

    // get size of texture
    void Size(SDL_Texture *texture, int *texture_w, int *texture_h)
    {
        if (texture)
        {
            SDL_QueryTexture(texture, nullptr, nullptr, texture_w, texture_h);
        }
    }

    // get width of texture
    int Width(SDL_Texture *texture)
    {
        auto width = 0;

        BloodSword::Size(texture, &width, nullptr);

        return width;
    }

    // get max width of list of textures
    int Width(std::vector<SDL_Texture *> &textures)
    {
        auto width = 0;

        for (auto i = 0; i < textures.size(); i++)
        {
            width = std::max(width, BloodSword::Width(textures[i]));
        }

        return width;
    }

    // get height of texture
    int Height(SDL_Texture *texture)
    {
        auto height = 0;

        BloodSword::Size(texture, nullptr, &height);

        return height;
    }

    // get max height of list of textures
    int Height(std::vector<SDL_Texture *> &textures)
    {
        auto height = 0;

        for (auto i = 0; i < textures.size(); i++)
        {
            height = std::max(height, BloodSword::Height(textures[i]));
        }

        return height;
    }

    // reads entire file into string (for use with nlohmann json)
    std::string Read(const char *source)
    {
        std::ifstream input_file(source);

        std::string file_content((std::istreambuf_iterator<char>(input_file)), (std::istreambuf_iterator<char>()));

        input_file.close();

        return file_content;
    }
}

#endif
