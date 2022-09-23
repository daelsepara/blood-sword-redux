#ifndef __PRIMITIVES_HPP__
#define __PRIMITIVES_HPP__

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Version.hpp"

namespace BloodSword
{
    // cartesian coordinates
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
    };

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
        if (*surface)
        {
            SDL_FreeSurface(*surface);

            *surface = NULL;
        }
    }

    // free texture
    void Free(SDL_Texture **texture)
    {
        if (*texture)
        {
            SDL_DestroyTexture(*texture);

            *texture = NULL;
        }
    }

    // free textures
    void Free(std::vector<SDL_Texture *> &textures)
    {
        if (!textures.empty())
        {
            for (auto &texture : textures)
            {
                Free(&texture);
            }
        }
    }
}
#endif
