#ifndef __FONTS_HPP__
#define __FONTS_HPP__

#include <fstream>
#include <SDL.h>
#include <SDL_ttf.h>

#include "nlohmann/json.hpp"

// classes and functions for handling fonts
namespace BloodSword::Fonts
{
    TTF_Font *Caption = nullptr;
    TTF_Font *Normal = nullptr;
    TTF_Font *Fixed = nullptr;

    // set up font types
    void Initialize(const char *font_ttf, int caption, int normal, int fixed)
    {
        TTF_Init();

        Fonts::Caption = TTF_OpenFont(font_ttf, caption);

        Fonts::Normal = TTF_OpenFont(font_ttf, normal);

        Fonts::Fixed = TTF_OpenFont(font_ttf, fixed);

        TTF_SetFontKerning(Fonts::Fixed, 0);
    }

    // cleanup fonts used
    void Free()
    {
        if (Fonts::Caption)
        {
            TTF_CloseFont(Fonts::Caption);

            Fonts::Caption = nullptr;
        }

        if (Fonts::Normal)
        {
            TTF_CloseFont(Fonts::Normal);

            Fonts::Normal = nullptr;
        }

        if (Fonts::Fixed)
        {
            TTF_CloseFont(Fonts::Fixed);

            Fonts::Fixed = nullptr;
        }

        TTF_Quit();
    }

    // load font definitions from settings file
    bool Load(const char *settings)
    {
        auto result = false;

        std::ifstream ifs(settings);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data["font-settings"].is_null())
            {
                std::string font = !data["font-settings"]["font"].is_null() ? std::string(data["font-settings"]["font"]) : "fonts/bookman-old-style.ttf";
                auto caption = !data["font-settings"]["caption"].is_null() ? int(data["font-settings"]["caption"]) : 22;
                auto normal = !data["font-settings"]["normal"].is_null() ? int(data["font-settings"]["normal"]) : 24;
                auto fixed = !data["font-settings"]["fixed"].is_null() ? int(data["font-settings"]["fixed"]) : 24;

                Fonts::Initialize(font.c_str(), caption, normal, fixed);
            }

            ifs.close();
        }

        return result;
    }
}
#endif
