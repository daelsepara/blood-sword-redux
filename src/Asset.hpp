#ifndef __ASSET_HPP__
#define __ASSET_HPP__

#include <fstream>

#include "nlohmann/json.hpp"
#include "AssetTypes.hpp"
#include "Color.hpp"
#include "Primitives.hpp"

namespace BloodSword::Asset
{
    // mapping of assets to their relative location
    Asset::Mapping<std::string> Locations = {};

    // mapping of asset types their respective loaded texture
    Asset::Mapping<SDL_Texture *> Textures = {};

    // create texture from a file
    SDL_Texture *Create(SDL_Renderer *renderer, const char *path)
    {
        SDL_Texture *texture = nullptr;

        auto surface = BloodSword::Load(path);

        if (surface)
        {
            texture = SDL_CreateTextureFromSurface(renderer, surface);

            if (texture)
            {
                SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

                SDL_SetTextureColorMod(texture, Color::R(Color::Active), Color::G(Color::Active), Color::B(Color::Active));
            }

            BloodSword::Free(&surface);
        }

        return texture;
    }

    // re-create surface from asset and color adjust
    SDL_Surface *Surface(Asset::Type asset, Uint32 blur)
    {
        SDL_Surface *surface = nullptr;

        if (asset != Asset::NONE)
        {
            surface = BloodSword::Load(Asset::Locations[asset].c_str());

            if (surface)
            {
                SDL_SetSurfaceColorMod(surface, Color::R(blur), Color::G(blur), Color::B(blur));
            }
        }

        return surface;
    }

    // reloads asset as an SDL surface and apply current palette
    SDL_Surface *Surface(Asset::Type asset)
    {
        return Asset::Surface(asset, Color::Active);
    }

    // unload all assets
    void Unload()
    {
        if (Asset::Textures.size() > 0)
        {
            for (auto &texture : Asset::Textures)
            {
                BloodSword::Free(&texture.second);
            }

            Asset::Textures.clear();

            Asset::Locations.clear();
        }
    }

    // load all assets and create textures
    bool Load(SDL_Renderer *renderer, const char *assets)
    {
        auto result = false;

        Asset::Unload();

        Asset::Locations.clear();

        Asset::Textures.clear();

        Asset::TypeMapping.clear();

        std::ifstream ifs(assets);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data["assets"].is_null() && data["assets"].is_array() && data["assets"].size() > 0)
            {
                auto asset_type = 0;

                for (auto i = 0; i < data["assets"].size(); i++)
                {
                    auto object = !data["assets"][i]["id"].is_null() ? std::string(data["assets"][i]["id"]) : std::string();

                    auto path = !data["assets"][i]["path"].is_null() ? std::string(data["assets"][i]["path"]) : "";

                    if (!path.empty() && !object.empty())
                    {
                        auto texture = Asset::Create(renderer, path.c_str());

                        if (texture)
                        {
                            Asset::Locations[asset_type] = path;

                            Asset::Textures[asset_type] = texture;

                            // update type mapping
                            Asset::TypeMapping[asset_type] = object;
                        }

                        asset_type++;
                    }
                }
            }

            ifs.close();

            result = (!Locations.empty() && !Textures.empty() && (Textures.size() == Locations.size()));
        }

        return result;
    }

    // load asset locations
    bool Load(const char *assets)
    {
        auto result = false;

        Asset::Unload();

        Asset::Locations.clear();

        Asset::Textures.clear();

        Asset::TypeMapping.clear();

        std::ifstream ifs(assets);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data["assets"].is_null() && data["assets"].is_array() && data["assets"].size() > 0)
            {
                auto asset_type = 0;

                for (auto i = 0; i < data["assets"].size(); i++)
                {
                    auto object = !data["assets"][i]["id"].is_null() ? std::string(data["assets"][i]["id"]) : std::string();

                    auto path = !data["assets"][i]["path"].is_null() ? std::string(data["assets"][i]["path"]) : std::string();

                    if (!path.empty() && !object.empty())
                    {
                        // update type mapping
                        Asset::TypeMapping[asset_type] = object;

                        // update location
                        Asset::Locations[asset_type] = path;

                        asset_type++;
                    }
                }
            }

            ifs.close();

            result = !Locations.empty();
        }

        return result;
    }

    // load assets and create textures
    bool Load(SDL_Renderer *renderer, std::string assets)
    {
        return Asset::Load(renderer, assets.c_str());
    }

    // get texture associated with the asset type
    SDL_Texture *Get(Asset::Type asset)
    {
        SDL_Texture *texture = nullptr;

        if (BloodSword::Has(Asset::Textures, asset))
        {
            texture = Asset::Textures[asset];
        }

        return texture;
    }

    // get texture associated with the asset type string
    SDL_Texture *Get(const char *asset)
    {
        return Asset::Get(Asset::Map(asset));
    }

    // get texture associated with the asset type string
    SDL_Texture *Get(std::string asset)
    {
        return Asset::Get(Asset::Map(asset));
    }

    // get texture associated with the asset type and modulate the color
    SDL_Texture *Get(Asset::Type asset, Uint8 alpha)
    {
        auto texture = Asset::Get(asset);

        if (texture)
        {
            SDL_SetTextureColorMod(texture, alpha, alpha, alpha);
        }

        return texture;
    }

    // get texture associated with the asset type and modulate the color
    SDL_Texture *Get(Asset::Type asset, Uint32 color)
    {
        auto texture = Asset::Get(asset);

        if (texture)
        {
            SDL_SetTextureColorMod(texture, Color::R(color), Color::G(color), Color::B(color));
        }

        return texture;
    }

    // create a copy of the asset. must be de-allocated manually
    SDL_Texture *Copy(SDL_Renderer *renderer, Asset::Type asset)
    {
        SDL_Texture *texture = nullptr;

        if (BloodSword::Has(Asset::Locations, asset))
        {
            texture = Asset::Create(renderer, Asset::Locations[asset].c_str());
        }

        return texture;
    }

    // copy the texture associated with the asset and modulate the color. must be
    // de-allocated manually
    SDL_Texture *Copy(SDL_Renderer *renderer, Asset::Type asset, Uint32 color)
    {
        auto texture = Asset::Copy(renderer, asset);

        if (texture)
        {
            SDL_SetTextureColorMod(texture, Color::R(color), Color::G(color), Color::B(color));
        }

        return texture;
    }
}

#endif
