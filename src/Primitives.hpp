#ifndef __PRIMITIVES_HPP__
#define __PRIMITIVES_HPP__

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

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
    };

    bool operator==(Point p1, Point p2)
    {
        return p1.X == p2.X && p1.Y == p2.Y;
    }

    bool operator!=(Point p1, Point p2)
    {
        return !(p1 == p2);
    }

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