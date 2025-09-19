#ifndef __RICH_TEXT_HPP__
#define __RICH_TEXT_HPP__

#include <string>

#include "Color.hpp"
#include "Fonts.hpp"

namespace BloodSword::Graphics
{
    // wrapper class for specifying a formatted string (font, color, style and wrapping limit)
    class RichText
    {
    public:
        // raw text
        std::string Text = std::string();

        // font to render text in
        TTF_Font *Font = nullptr;

        // Font ID to reload fonts once available
        Fonts::ID ID = Fonts::ID::NONE;

        // color
        SDL_Color Color = Color::S(Color::Active);

        // style
        int Style = TTF_STYLE_NORMAL;

        // line wrap limit (pixels)
        int Wrap = 0;

        RichText(const char *text, Fonts::ID id, SDL_Color color, int style, int wrap) : Text(text), ID(id), Color(color), Style(style), Wrap(wrap) {}

        RichText(std::string text, TTF_Font *font, SDL_Color color, int style, int wrap) : Text(text), Font(font), Color(color), Style(style), Wrap(wrap) {}

        RichText(std::string text, TTF_Font *font, Uint32 color, int style, int wrap) : Text(text), Font(font), Color(Color::S(color)), Style(style), Wrap(wrap) {}

        RichText() {}

        // load font if not already set
        void Load()
        {
            this->Font = Fonts::Set(this->ID);

            if (this->ID == Fonts::ID::NONE)
            {
                this->ID = Fonts::ID::NORMAL;
            }
        }
    };

    // list of rich text
    typedef std::vector<Graphics::RichText> TextList;

    // reload fonts
    void SetFonts(Graphics::TextList &collection)
    {
        for (auto &text : collection)
        {
            text.Load();
        }
    }

    // generate list of rich text from list of strings
    Graphics::TextList GenerateTextList(std::vector<std::string> collection, TTF_Font *font, Uint32 color, int wrap = 0)
    {
        auto text_list = Graphics::TextList();

        for (auto text : collection)
        {
            text_list.push_back(Graphics::RichText(text.c_str(), font, Color::S(color), TTF_STYLE_NORMAL, wrap));
        }

        return text_list;
    }
}

#endif
