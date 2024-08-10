#ifndef __RICH_TEXT_HPP__
#define __RICH_TEXT_HPP__

#include <string>

#include "Fonts.hpp"

namespace BloodSword::Graphics
{
    // wrapper class for specifying a formatted string (font, color, style and wrapping limit)
    class RichText
    {
    public:
        // raw text
        std::string Text;

        // font to render text in
        TTF_Font *Font = nullptr;

        // color
        SDL_Color Color = Color::S(Color::Active);

        // style
        int Style = TTF_STYLE_NORMAL;

        // line wrap limit (pixels)
        int Wrap = 0;

        RichText(std::string text, TTF_Font *font, SDL_Color color, int style, int wrap) : Text(text), Font(font), Color(color), Style(style), Wrap(wrap) {}

        RichText(std::string text, TTF_Font *font, Uint32 color, int style, int wrap) : Text(text), Font(font), Color(Color::S(color)), Style(style), Wrap(wrap) {}
    };
}

#endif
