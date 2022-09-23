#ifndef __ASSET_HPP__
#define __ASSET_HPP__

#include <fstream>
#include <string>
#include <unordered_map>

#include "nlohmann/json.hpp"
#include "AssetTypes.hpp"
#include "Primitives.hpp"

namespace BloodSword::Asset
{
    AssetMapping<std::string> Locations = {};

    AssetMapping<SDL_Texture *> Textures = {};

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
            }

            BloodSword::Free(&surface);
        }

        return texture;
    }

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

    SDL_Texture *Get(Asset::Type asset)
    {
        SDL_Texture *texture = NULL;

        if (Asset::Textures.find(asset) != Asset::Textures.end())
        {
            texture = Asset::Textures[asset];
        }

        return texture;
    }

    SDL_Texture *Get(Asset::Type asset, Uint8 alpha)
    {
        auto texture = Asset::Get(asset);

        if (texture)
        {
            SDL_SetTextureColorMod(texture, alpha, alpha, alpha);
        }

        return texture;
    }

    SDL_Texture *Copy(SDL_Renderer *renderer, Asset::Type asset)
    {
        SDL_Texture *texture = NULL;

        if (Asset::Locations.find(asset) != Asset::Locations.end())
        {
            texture = Asset::Create(renderer, Asset::Locations[asset].c_str());
        }

        return texture;
    }

    SDL_Texture *Copy(SDL_Renderer *renderer, Asset::Type asset, Uint8 alpha)
    {
        auto texture = Asset::Copy(renderer, asset);

        if (texture)
        {
            SDL_SetTextureColorMod(texture, alpha, alpha, alpha);
        }

        return texture;
    }
}

#endif
