#ifndef __HELP_HPP__
#define __HELP_HPP__

#include <fstream>

#include "nlohmann/json.hpp"
#include "Graphics.hpp"

// classes and functions for rendering help sections
namespace BloodSword::Help
{
    // font face styles
    enum class Face
    {
        NONE = -1,
        NORMAL = 0,
        BOLD,
        ITALIC,
        BOLD_ITALIC
    };

    // text alignment
    enum class Align
    {
        NONE = -1,
        LEFT = 0,
        CENTER,
        RIGHT
    };

    // font face style to string mapping
    BloodSword::ConstStrings<Help::Face> FaceMapping = {
        {Help::Face::NONE, "NONE"},
        {Help::Face::NORMAL, "NORMAL"},
        {Help::Face::BOLD, "BOLD"},
        {Help::Face::ITALIC, "ITALIC"},
        {Help::Face::BOLD_ITALIC, "BOLD-ITALIC"}};

    // text alignment to string mapping
    BloodSword::ConstStrings<Help::Align> AlignMapping = {
        {Help::Align::NONE, "NONE"},
        {Help::Align::LEFT, "LEFT"},
        {Help::Align::CENTER, "CENTER"},
        {Help::Align::RIGHT, "RIGHT"}};

    // map string to font face style
    Help::Face MapFace(const char *face)
    {
        return BloodSword::Find(Help::FaceMapping, face);
    }

    // map string to font face style
    Help::Face MapFace(std::string face)
    {
        return Help::MapFace(face.c_str());
    }

    // map string to text alignment
    Help::Align MapAlign(const char *alignment)
    {
        return BloodSword::Find(Help::AlignMapping, alignment);
    }

    // map string to text alignment
    Help::Align MapAlign(std::string alignment)
    {
        return Help::MapAlign(alignment.c_str());
    }

    // text items in section
    class Item
    {
    public:
        // text alignment
        Help::Align Align = Help::Align::LEFT;

        // font face style
        Help::Face Face = Help::Face::NORMAL;

        // text content
        std::string Text = std::string();

        Item() {}
    };

    // main section
    class Section
    {
    public:
        // section header
        std::string Header = std::string();

        // section image (if any)
        std::string Image = std::string();

        // list of items
        std::vector<Help::Item> Items = {};

        Section() {}
    };

    // list of sections
    typedef std::vector<Help::Section> Sections;

    // set item stile
    int SetStyle(Help::Item &item)
    {
        auto style = 0;

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

        case Face::BOLD_ITALIC:

            style = TTF_STYLE_BOLD | TTF_STYLE_ITALIC;

            break;

        default:

            style = TTF_STYLE_NORMAL;

            break;
        }

        return style;
    }

    // generate texture from all text items
    SDL_Texture *GenerateSection(Graphics::Base &graphics, TTF_Font *font, Help::Section &section, int wrap)
    {
        auto height = 0;

        auto width = 0;

        for (auto &item : section.Items)
        {
            auto style = Help::SetStyle(item);

            // create test surface
            auto test_surface = Graphics::CreateSurfaceText(item.Text.c_str(), font, Color::S(Color::Active), style, wrap);

            height += test_surface->h;

            width = std::max(width, test_surface->w);

            // free test surface
            BloodSword::Free(&test_surface);
        }

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
                auto style = Help::SetStyle(item);

                auto surface_asset = Graphics::CreateSurfaceText(item.Text.c_str(), font, Color::S(Color::Active), style, width);

                if (surface_asset)
                {
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
            }

            texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

            BloodSword::Free(&surface);
        }

        return texture;
    }

    // load a single help item from json data
    Help::Item LoadItem(nlohmann::json &data)
    {
        auto item = Help::Item();

        item.Face = !data["face"].is_null() ? Help::MapFace(data["face"]) : Help::Face::NONE;

        item.Align = !data["align"].is_null() ? Help::MapAlign(data["align"]) : Help::Align::NONE;

        item.Text = !data["text"].is_null() ? std::string(data["text"]) : std::string();

        return item;
    }

    // load help sections from file
    Help::Sections Load(const char *help)
    {
        auto sections = Help::Sections();

        std::ifstream ifs(help);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data["sections"].is_null() && data["sections"].is_array() && SafeCast(data["sections"].size()) > 0)
            {
                for (auto i = 0; i < SafeCast(data["sections"].size()); i++)
                {
                    auto section = Help::Section();

                    section.Header = !data["sections"][i]["header"].is_null() ? std::string(data["sections"][i]["header"]) : std::string();

                    section.Image = !data["sections"][i]["image"].is_null() ? std::string(data["sections"][i]["image"]) : std::string();

                    if (!data["sections"][i]["items"].is_null() && data["sections"][i]["items"].is_array() && SafeCast(data["sections"][i]["items"].size()) > 0)
                    {
                        for (auto j = 0; j < SafeCast(data["sections"][i]["items"].size()); j++)
                        {
                            auto item = Help::LoadItem(data["sections"][i]["items"][j]);

                            if (SafeCast(item.Text.size()) > 0 && item.Align != Help::Align::NONE && item.Face != Help::Face::NONE)
                            {
                                section.Items.push_back(item);
                            }
                        }
                    }

                    if (SafeCast(section.Items.size()) > 0)
                    {
                        sections.push_back(section);
                    }
                }
            }

            ifs.close();
        }

        return sections;
    }

    // load help sections from file
    Help::Sections Load(std::string help)
    {
        return Help::Load(help.c_str());
    }
}

#endif
