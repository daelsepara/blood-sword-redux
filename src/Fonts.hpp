#ifndef __FONTS_HPP__
#define __FONTS_HPP__

#include <fstream>
#include <string>

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include <SDL.h>
#include <SDL_ttf.h>

#include "nlohmann/json.hpp"
#include "ZipFileLibrary.hpp"

// classes and functions for handling fonts
namespace BloodSword::Fonts
{
    // font for captions (titles, headings)
    TTF_Font *Caption = nullptr;

    // normal font for most text
    TTF_Font *Normal = nullptr;

    // fixed-width font for tables, code, etc.
    TTF_Font *Fixed = nullptr;

    // font IDs
    enum class ID
    {
        NONE = -1,
        CAPTION,
        NORMAL,
        FIXED
    };

    // set up font types
    void Initialize(const char *zipfile, const char *font_ttf, int caption, int normal, int fixed)
    {
        TTF_Init();

        // read file from zip archive
        auto font = BloodSword::ZipFile::Read(zipfile, font_ttf);

        // create a modifiable buffer
        auto buffer = font.data();

        // create chunk from memory buffer
        auto rw = SDL_RWFromMem((void *)buffer, font.size());

        font.clear();

        if (rw)
        {
            Fonts::Caption = TTF_OpenFontRW(rw, 0, caption);

            Fonts::Normal = TTF_OpenFontRW(rw, 0, normal);

            Fonts::Fixed = TTF_OpenFontRW(rw, 0, fixed);

            TTF_SetFontKerning(Fonts::Fixed, 1);

            SDL_RWclose(rw);
        }
    }

    // set up font types
    void Initialize(const char *font_ttf, int caption, int normal, int fixed)
    {
        TTF_Init();

        Fonts::Caption = TTF_OpenFont(font_ttf, caption);

        Fonts::Normal = TTF_OpenFont(font_ttf, normal);

        Fonts::Fixed = TTF_OpenFont(font_ttf, fixed);

        TTF_SetFontKerning(Fonts::Fixed, 1);
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

    void Load(nlohmann::json &data, const char *zipfile)
    {
        auto is_zip = (zipfile != nullptr);

        if (!data["font-settings"].is_null())
        {
            auto font = std::string(!data["font-settings"]["font"].is_null() ? data["font-settings"]["font"] : "fonts/bookman-old-style.ttf");

            auto caption = !data["font-settings"]["caption"].is_null() ? int(data["font-settings"]["caption"]) : 22;

            auto normal = !data["font-settings"]["normal"].is_null() ? int(data["font-settings"]["normal"]) : 24;

            auto fixed = !data["font-settings"]["fixed"].is_null() ? int(data["font-settings"]["fixed"]) : 24;

            if (is_zip)
            {
                Fonts::Initialize(zipfile, font.c_str(), caption, normal, fixed);
            }
            else
            {
                Fonts::Initialize(font.c_str(), caption, normal, fixed);
            }
        }
    }

    // load font definitions from settings file
    void Load(const char *fonts)
    {
        std::ifstream ifs(fonts);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            Fonts::Load(data, nullptr);

            ifs.close();
        }
    }

    // load font definitions from settings file in zip archive
    void Load(const char *fonts, const char *zipfile)
    {
        if (zipfile == nullptr)
        {
            Fonts::Load(fonts);
        }
        else
        {
            auto ifs = ZipFile::Read(zipfile, fonts);

            if (!ifs.empty())
            {
                auto data = nlohmann::json::parse(ifs);

                Fonts::Load(data, zipfile);

                ifs.clear();
            }
        }
    }

    // load font definitions from settings file
    void Load(std::string fonts)
    {
        Fonts::Load(fonts.c_str());
    }

    // load font definitions from settings file in zip archive
    void Load(std::string fonts, std::string zipfile)
    {
        Fonts::Load(fonts.c_str(), zipfile.c_str());
    }

    // get font by ID
    TTF_Font *Set(Fonts::ID id)
    {
        TTF_Font *font = nullptr;

        switch (id)
        {
        case Fonts::ID::CAPTION:

            font = Fonts::Caption;

            break;

        case Fonts::ID::NORMAL:

            font = Fonts::Normal;

            break;

        case Fonts::ID::FIXED:

            font = Fonts::Fixed;

            break;

        default:

            font = Fonts::Normal;

            break;
        }

        return font;
    }
}

#endif
