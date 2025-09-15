#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <fstream>

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include <SDL.h>
#include <SDL_ttf.h>

// functions and classes for handling SDL colors
namespace BloodSword::Color
{
    // get red component
    Uint8 R(Uint32 c) { return (Uint8)((c & 0xFF0000) >> 16); }

    // get green component
    Uint8 G(Uint32 c) { return (Uint8)((c & 0x00FF00) >> 8); }

    // get blue component
    Uint8 B(Uint32 c) { return (Uint8)(c & 0x0000FF); }

    // get alpha component
    Uint8 A(Uint32 c) { return (Uint8)(c >> 24); }

    // set opacity
    Uint32 O(Uint32 c, Uint8 a) { return ((c & ((Uint32)0x00FFFFFF)) | ((Uint32)(a << 24))); }

    // active color (default)
    Uint32 Active = 0xFFFFFFFF;

    // inactive color (default)
    Uint32 Inactive = 0xFF606060;

    // highlight color (default)
    Uint32 Highlight = 0xFFFF0000;

    // background color (default)
    Uint32 Background = 0xFF000000;

    // transparent color (default)
    Uint32 Transparent = 0x00000000;

    // blur mask
    Uint32 Blur = 0x7F000000;

    // create SDL_Color from RGBA values
    SDL_Color S(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        return {r, g, b, a};
    }

    // create SDL_Color from RGB values (alpha = 255)
    SDL_Color S(Uint8 r, Uint8 g, Uint8 b)
    {
        return Color::S(r, g, b, 255);
    }

    // create SDL_Color from Uint32 color
    SDL_Color S(Uint32 c)
    {
        return Color::S(R(c), G(c), B(c), A(c));
    }

    // create Uint32 color from RGBA values
    Uint32 U(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        return (Uint32)(a << 24 | r << 16 | g << 8 | b);
    }

    // create Uint32 color from RGB values (alpha = 255)
    Uint32 U(SDL_Color color)
    {
        return Color::U(color.r, color.g, color.b, color.a);
    }
}

#endif
