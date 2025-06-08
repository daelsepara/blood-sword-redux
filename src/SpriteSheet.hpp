#ifndef __SPRITE_SHEET_HPP__
#define __SPRITE_SHEET_HPP__

#include "InterfaceBattle.hpp"

// generate sprite sheet
namespace BloodSword::SpriteSheet
{
    void RenderAsset(SDL_Surface *surface, SDL_Surface *surface_asset, SDL_Rect &rect)
    {
        if (surface_asset)
        {
            auto converted_asset = SDL_ConvertSurface(surface_asset, surface->format, 0);

            if (converted_asset)
            {
                // place icon in the correct position
                SDL_SetSurfaceAlphaMod(converted_asset, SDL_ALPHA_OPAQUE);

                SDL_BlitSurface(converted_asset, nullptr, surface, &rect);

                // cleanup
                BloodSword::Free(&converted_asset);
            }

            // cleanup
            BloodSword::Free(&surface_asset);
        }
    }

    void Main(const char *image_file, int width)
    {
        // load default module
        Interface::LoadModules();

        // load settings
        Interface::Initialize(Interface::SettingsFile.c_str());

        // load asset location mappings
        Asset::Load(std::string(Interface::Settings["assets"]).c_str());

        if (Asset::Locations.size() > 0)
        {
            SDL_Rect rect;

            auto height = int(std::ceil(Asset::Locations.size() / width));

            rect.w = width * BloodSword::TileSize;

            rect.h = height * BloodSword::TileSize;

            auto surface = Graphics::CreateSurface(rect.w, rect.h);

            if (surface)
            {
                // fill entire sheet
                SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

                auto x = 0;

                auto y = 0;

                for (const auto &[asset, name] : Asset::TypeMapping)
                {
                    if (asset != Asset::Type::NONE)
                    {
                        auto surface_asset = BloodSword::Asset::Surface(asset);

                        if (surface_asset == nullptr)
                        {
                            std::cerr << "Asset: [" << name << "] not found!" << std::endl;
                        }

                        rect.x = (width - x - 1) * BloodSword::TileSize;

                        rect.y = (height - y - 1) * BloodSword::TileSize;

                        SpriteSheet::RenderAsset(surface, surface_asset, rect);

                        x++;

                        if (x == width)
                        {
                            x = 0;

                            y++;
                        }
                    }
                }

                IMG_SavePNG(surface, image_file);

                SDL_FreeSurface(surface);
            }
        }
    }
}

#endif
