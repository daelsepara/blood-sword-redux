#ifndef __FONTS_HPP__
#define __FONTS_HPP__

#include <fstream>
#include <SDL_ttf.h>
#include "nlohmann/json.hpp"

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