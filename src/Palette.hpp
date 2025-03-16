#ifndef __PALETTE_HPP__
#define __PALETTE_HPP__

#include <SDL.h>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
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
        {0XFF92D8FC, 0XFF12587C, 0XFF1875FA, 0XFF000000},
        {0XFFC7A8FF, 0XFFA270FF, 0XFF5A0BED, 0XFF000000}};

    // names for the palettes above
    std::vector<std::string> Names = {
        "DEFAULT",
        "CGA (WHITE/MAGENTA/CYAN)",
        "CGA (GREEN/RED/BROWN)",
        "GAMEBOY",
        "ANOTHER WORLD",
        "DESERT BROWN",
        "GREEN/YELLOW/ORANGE",
        "BLUE/RED/WHITE",
        "AQUAMARINE",
        "VIOLET/INDIGO/LAVENDER"};

    // current palette set
    int Current = 0;

    bool Load(const char *settings)
    {
        auto result = false;

        std::ifstream ifs(settings);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            std::vector<std::string> names = {};

            BloodSword::Array<Uint32> list = {};

            if (!data["palettes"].is_null() && data["palettes"].is_array() && data["palettes"].size() > 0)
            {
                for (auto i = 0; i < data["palettes"].size(); i++)
                {
                    auto colors = std::vector<Uint32>();

                    if (!data["palettes"][i]["colors"].is_null() && data["palettes"][i]["colors"].is_array() && data["palettes"][i]["colors"].size() > 0)
                    {
                        for (auto index = 0; index < data["palettes"][i]["colors"].size(); index++)
                        {
                            Uint32 color = std::stoul(std::string(data["palettes"][i]["colors"][index]), nullptr, 16);

                            colors.push_back(color);
                        }
                    }

                    if (colors.size() < 4)
                    {
                        colors.push_back(0xFF000000);
                    }

                    if ((colors).size() >= 4)
                    {
                        names.push_back(std::string(data["palettes"][i]["name"]));

                        list.push_back(colors);
                    }
                }
            }

            if (names.size() > 0 && list.size() > 0 && names.size() == list.size())
            {
                std::cerr << "[LOADED] " << std::to_string(list.size()) << " palettes" << std::endl;

                Palette::List = list;

                Palette::Names = names;
            }

            ifs.close();
        }

        return result;
    }

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
