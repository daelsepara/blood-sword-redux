#ifndef __HELP_HPP__
#define __HELP_HPP__

#include "Graphics.hpp"

namespace BloodSword::Help
{
    enum class Face
    {
        NORMAL = 0,
        BOLD,
        ITALIC
    };

    enum class Align
    {
        LEFT = 0,
        CENTER,
        RIGHT
    };

    BloodSword::Mapping<Help::Face> FaceMapping = {
        {Help::Face::NORMAL, "NORMAL"},
        {Help::Face::BOLD, "BOLD"},
        {Help::Face::ITALIC, "ITALIC"}};

    BloodSword::Mapping<Help::Align> AlignMapping = {
        {Help::Align::LEFT, "LEFT"},
        {Help::Align::CENTER, "CENTER"},
        {Help::Align::RIGHT, "RIGHT"}};

    class Item
    {
    public:
        Help::Align Align = Help::Align::LEFT;

        Help::Face Face = Help::Face::NORMAL;

        std::string Text = std::string();

        Item() {}
    };

    class Section
    {
    public:
        std::string Header = std::string();

        std::string Image = std::string();

        std::vector<Help::Item> Items = {};

        Section() {}
    };

    typedef std::vector<Help::Section> Sections;

    SDL_Texture *GenerateSection(Graphics::Base &graphics, TTF_Font *font, Help::Section &section, int wrap)
    {
        auto text = std::string();

        for (auto &item : section.Items)
        {
            if (text.size() > 0)
            {
                text += "\n";
            }

            text += item.Text;
        }

        auto width = 0;

        auto height = 0;

        Graphics::Estimate(font, text.c_str(), &width, &height);

        width = std::max(wrap, width);

        SDL_Texture *texture = nullptr;

        SDL_Surface *surface = Graphics::CreateSurface(width, height);

        if (surface)
        {
            SDL_Rect rect;

            rect.w = surface->w;

            rect.h = surface->h;

            rect.x = 0;

            rect.y = 0;

            for (auto &item : section.Items)
            {
                int style = 0;

                switch (item.Face)
                {
                case Face::NORMAL:
                    style = TTF_STYLE_NORMAL;

                    break;
                case Face::BOLD:
                    style = TTF_STYLE_BOLD;

                    break;
                case Face::ITALIC:
                    style = TTF_STYLE_ITALIC;

                    break;
                default:
                    style = TTF_STYLE_NORMAL;

                    break;
                }

                auto surface_asset = Graphics::CreateSurfaceText(item.Text.c_str(), font, Color::S(Color::Active), style, width);

                if (item.Align == Align::RIGHT)
                {
                    rect.x = width - surface_asset->w;
                }
                else if (item.Align == Align::CENTER)
                {
                    rect.x = (width - surface_asset->w) / 2;
                }
                else
                {
                    rect.x = 0;
                }

                Graphics::RenderAsset(surface, surface_asset, rect);

                rect.y += surface_asset->h;

                BloodSword::Free(&surface_asset);
            }

            texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

            BloodSword::Free(&surface);
        }

        return texture;
    }
}

#endif
