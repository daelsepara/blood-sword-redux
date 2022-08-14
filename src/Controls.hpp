#ifndef __CONTROLS_HPP__
#define __CONTROLS_HPP__

#include <SDL.h>

namespace BloodSword::Controls
{
    enum class Type
    {
        NONE = -1,
        ANY = 0,
        WARRIOR,
        TRICKSTER,
        SAGE,
        ENCHANTER
    };

    class Base
    {
    public:
        Controls::Type Type = Controls::Type::ANY;

        int ID = -1;

        int Left = -1;
        int Right = -1;
        int Up = -1;
        int Down = -1;

        int X = 0;
        int Y = 0;

        int W = 0;
        int H = 0;

        // color of border when hightlighted
        Uint32 Highlight;
    };
}

#endif