#ifndef __FONTS_HPP__
#define __FONTS_HPP__

#include <fstream>

#include <SDL.h>
#include <SDL_ttf.h>

#include "nlohmann/json.hpp"

namespace BloodSword::Color
{
    Uint32 Active = 0xFFFFFFFF;
    Uint32 Inactive = 0xFF606060;
    Uint32 Highlight = 0xFFFF0000;
    Uint32 Background = 0xFF000000;

    SDL_Color cActive = {255, 255, 255, 255};
    SDL_Color cInactive = {96, 96, 96, 255};
    SDL_Color cHighlight = {255, 0, 0, 255};
    SDL_Color cBackground = {0, 0, 0, 255};

    SDL_Color Set(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        return {r, g, b, a};
    }

    SDL_Color Set(Uint8 r, Uint8 g, Uint8 b)
    {
        return Color::Set(r, g, b, 255);
    }

    Uint32 Set(SDL_Color color)
    {
        return (color.a >> 24 | color.r >> 16 | color.g >> 8 | color.b);
    }
}

namespace BloodSword::Fonts
{
    TTF_Font *Caption = NULL;
    TTF_Font *Normal = NULL;
    TTF_Font *Fixed = NULL;

    void Initialize(const char *font_ttf, int caption, int normal, int fixed)
    {
        TTF_Init();

        Caption = TTF_OpenFont(font_ttf, caption);

        Normal = TTF_OpenFont(font_ttf, normal);

        Fixed = TTF_OpenFont(font_ttf, fixed);

        TTF_SetFontKerning(Fixed, 0);
    }

    void Free()
    {
        if (Caption)
        {
            TTF_CloseFont(Caption);

            Caption = NULL;
        }

        if (Normal)
        {
            TTF_CloseFont(Normal);

            Normal = NULL;
        }

        if (Fixed)
        {
            TTF_CloseFont(Fixed);

            Fixed = NULL;
        }

        TTF_Quit();
    }

    bool Load(const char *settings)
    {
        auto result = false;

        std::ifstream ifs(settings);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data["font-settings"].is_null())
            {
                std::string font = !data["font-settings"]["font"].is_null() ? data["font-settings"]["font"] : "fonts/bookman-old-style.ttf";
                auto caption = !data["font-settings"]["caption"].is_null() ? ((int)data["font-settings"]["caption"]) : 22;
                auto normal = !data["font-settings"]["normal"].is_null() ? ((int)data["font-settings"]["normal"]) : 24;
                auto fixed = !data["font-settings"]["fixed"].is_null() ? ((int)data["font-settings"]["fixed"]) : 24;

                Fonts::Initialize(font.c_str(), caption, normal, fixed);
            }

            ifs.close();
        }

        return result;
    }
}
#endif
