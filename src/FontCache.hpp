#ifndef __FONT_CACHE_HPP__
#define __FONT_CACHE_HPP__

#include <fstream>
#include <string>

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Graphics.hpp"
#include "Primitives.hpp"
#include "Templates.hpp"

// simple implementation of a font (glyph) cache
namespace BloodSword::FontCache
{
    class Glyph
    {
    public:
        SDL_Texture *Texture = nullptr;

        int Width = -1;

        int Height = -1;

        Glyph(Graphics::Base &graphics, TTF_Font *font, const char *text, SDL_Color text_color, int style)
        {
            // set style
            TTF_SetFontStyle(font, style);

            auto surface = TTF_RenderUTF8_Blended(font, text, text_color);

            if (surface)
            {
                // create texture
                this->Texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

                if (this->Texture)
                {
                    // get texture dimensions
                    BloodSword::Size(this->Texture, &this->Width, &this->Height);
                }

                BloodSword::Free(&surface);
            }
        }

        Glyph() {}

        void Free()
        {
            if (this->Texture)
            {
                BloodSword::Free(&this->Texture);
            }
        }
    };

    FontCache::Glyph Null = FontCache::Glyph();

    class Base
    {
    public:
        BloodSword::UnorderedMap<std::string, FontCache::Glyph> Glyphs = {};

        Base() {}

        bool Has(std::string &text)
        {
            return BloodSword::Has(this->Glyphs, text);
        }

        FontCache::Glyph &operator[](std::string chr)
        {
            return this->Has(chr) ? this->Glyphs[chr] : FontCache::Null;
        }

        SDL_Texture *Texture(std::string chr)
        {
            return this->Has(chr) ? this->Glyphs[chr].Texture : nullptr;
        }

        void Free()
        {
            for (auto &item : this->Glyphs)
            {
                auto glyph = item.second;

                glyph.Free();
            }

            this->Glyphs.clear();
        }

        void CreateGlyph(BloodSword::Graphics::Base &graphics, TTF_Font *font, std::string &text, SDL_Color text_color, int style)
        {
            // ensure that there are no duplicates
            if (!this->Has(text))
            {
                this->Glyphs[text] = FontCache::Glyph(graphics, font, text.c_str(), text_color, style);
            }
        }

        // add character to cache
        void Add(Graphics::Base &graphics, TTF_Font *font, const char *text, SDL_Color text_color, int style)
        {
            for (auto c = 0; c < std::strlen(text); c++)
            {
                auto chr = std::string(1, text[c]);

                this->CreateGlyph(graphics, font, chr, text_color, style);
            }
        }

        // create texture cache of individual characters
        void Create(Graphics::Base &graphics, TTF_Font *font, const char *text, SDL_Color text_color, int style)
        {
            this->Free();

            this->Add(graphics, font, text, text_color, style);
        }

        // add words to cache
        void Add(Graphics::Base &graphics, TTF_Font *font, std::vector<std::string> &collection, SDL_Color text_color, int style)
        {
            for (auto &text : collection)
            {
                this->CreateGlyph(graphics, font, text, text_color, style);
            }
        }

        // create cache of words
        void Create(Graphics::Base &graphics, TTF_Font *font, std::vector<std::string> &collection, SDL_Color text_color, int style)
        {
            this->Free();

            this->Add(graphics, font, collection, text_color, style);
        }
    };
}

#endif
