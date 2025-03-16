#ifndef __PALETTE_HPP__
#define __PALETTE_HPP__

#include <SDL.h>
#include <vector>

#include "Color.hpp"
#include "Templates.hpp"

// functions for switching color palette
namespace BloodSword::Palette
{
    // active, inactive, highlight, background
    BloodSword::Array<Uint32> List = {
        {0XFFFFFFFF, 0XFF404040, 0XFFFF0000, 0XFF000000},
        {0XFF56FFFF, 0XFFFFFFFF, 0XFFFE02A2, 0XFF000000},
        {0XFF00AA00, 0XFFAA5500, 0XFFAA0000, 0XFF000000},
        {0XFFB5AF42, 0XFF3A5122, 0XFF5D782E, 0XFF000000},
        {0XFFF1EDCF, 0XFF78938A, 0XFF525E75, 0XFF000000},
        {0XFFA98C61, 0XFF312915, 0XFF6C3713, 0XFF000000},
        {0XFFFFCF42, 0XFF007550, 0XFFF79239, 0XFF000000},
        {0XFFFFFFFF, 0XFF232D64, 0XFFE61937, 0XFF000000},
        {0XFF92D8FC, 0XFF12587C, 0XFF1875FA, 0XFF000000}};

    // names for the palettes above
    std::vector<const char *> Names = {
        "DEFAULT",
        "CGA (WHITE/MAGENTA/CYAN)",
        "CGA (GREEN/RED/BROWN)",
        "GAMEBOY",
        "ANOTHER WORLD",
        "DESERT BROWN",
        "GREEN/YELLOW/ORANGE",
        "BLUE/RED/WHITE",
        "AQUAMARINE"};

    // current palette set
    int Current = 0;

    // switch palette
    void Switch(int palette = 0, bool override = true)
    {
        if (palette >= 0 && palette < List.size())
        {
            Color::Active = List[palette][0];

            Color::Inactive = List[palette][1];

            Color::Highlight = List[palette][2];

            Color::Background = List[palette][3];

            if (override)
            {
                Current = palette;
            }
        }
    }
}
#endif
