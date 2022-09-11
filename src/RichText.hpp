#ifndef __RICH_TEXT_HPP__
#define __RICH_TEXT_HPP__

#include <string>

#include "Fonts.hpp"

namespace BloodSword::Graphics
{
    class RichText
    {
    public:
        std::string Text = "";

        TTF_Font *Font = NULL;

        SDL_Color Color = Color::S(Color::Active);

        int Style = TTF_STYLE_NORMAL;

        int Wrap = 0;

        RichText(std::string text, TTF_Font *font, SDL_Color color, int style, int wrap)
        {
            this->Text = text;

            this->Font = font;

            this->Color = color;

            this->Style = style;

            this->Wrap = wrap;
        }
    };
}

#endif
