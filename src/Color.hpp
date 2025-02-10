#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <fstream>
#include <SDL.h>
#include <SDL_ttf.h>

// functions and classes for handling SDL colors
namespace BloodSword::Color
{
    Uint8 R(Uint32 c) { return (Uint8)((c & 0xFF0000) >> 16); }
    
    Uint8 G(Uint32 c) { return (Uint8)((c & 0x00FF00) >> 8); }
    
    Uint8 B(Uint32 c) { return (Uint8)(c & 0x0000FF); }
    
    Uint8 A(Uint32 c) { return (Uint8)(c >> 24); }
    
    Uint32 O(Uint32 c, Uint8 a) { return ((c & ((Uint32)0x00FFFFFF)) | ((Uint32)(a << 24))); }

    Uint32 Active = 0xFFFFFFFF;
    
    Uint32 Inactive = 0xFF606060;
    
    Uint32 Highlight = 0xFFFF0000;
    
    Uint32 Background = 0xFF000000;
    
    Uint32 Transparent = 0x00000000;

    SDL_Color S(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        return {r, g, b, a};
    }

    SDL_Color S(Uint8 r, Uint8 g, Uint8 b)
    {
        return Color::S(r, g, b, 255);
    }

    SDL_Color S(Uint32 c)
    {
        return Color::S(R(c), G(c), B(c), A(c));
    }

    Uint32 U(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        return (Uint32)(a << 24 | r << 16 | g << 8 | b);
    }

    Uint32 U(SDL_Color color)
    {
        return Color::U(color.r, color.g, color.b, color.a);
    }
}
#endif
