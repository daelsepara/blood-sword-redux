#ifndef __ASSET_HPP__
#define __ASSET_HPP__

#include <fstream>
#include <string>

#include "nlohmann/json.hpp"
#include "AssetTypes.hpp"
#include "Color.hpp"
#include "Primitives.hpp"

namespace BloodSword::Asset
{
    // mapping of assets to their relative location
    AssetMapping<std::string> Locations = {};

    // mapping of asset types their respective loaded texture
    AssetMapping<SDL_Texture *> Textures = {};

    // create texture from a file
    SDL_Texture *Create(SDL_Renderer *renderer, const char *path)
    {
        SDL_Texture *texture = NULL;

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

    // load all assets
    bool Load(SDL_Renderer *renderer, const char *assets)
    {
        auto result = false;

        Asset::Unload();

        Asset::Locations.clear();

        Asset::Textures.clear();

        std::ifstream ifs(assets);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data["asset"].is_null() && data["asset"].is_array() && data["asset"].size() > 0)
            {
                for (auto i = 0; i < data["asset"].size(); i++)
                {
                    auto object = !data["asset"][i]["id"].is_null() ? Asset::Map(std::string(data["asset"][i]["id"])) : Asset::Type::NONE;

                    auto path = !data["asset"][i]["path"].is_null() ? std::string(data["asset"][i]["path"]) : "";

                    if (!path.empty() && object != Asset::Type::NONE)
                    {
                        auto texture = Asset::Create(renderer, path.c_str());

                        if (texture)
                        {
                            Asset::Locations[object] = path;

                            Asset::Textures[object] = texture;
                        }
                    }
                }
            }

            ifs.close();

            result = (!Locations.empty() && !Textures.empty() && (Textures.size() == Locations.size()));
        }

        return result;
    }

    // get texture associated with the asset type
    SDL_Texture *Get(Asset::Type asset)
    {
        SDL_Texture *texture = NULL;

        auto ptr = Asset::Textures.find(asset);

        if (ptr != Asset::Textures.end())
        {
            texture = ptr->second;
        }

        return texture;
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

    // create a copy of the asset. must be de-allocated manually
    SDL_Texture *Copy(SDL_Renderer *renderer, Asset::Type asset)
    {
        SDL_Texture *texture = NULL;

        auto ptr = Asset::Locations.find(asset);

        if (ptr != Asset::Locations.end())
        {
            texture = Asset::Create(renderer, ptr->second.c_str());
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
