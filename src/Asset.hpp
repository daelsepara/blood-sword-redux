#ifndef __ASSET_HPP__
#define __ASSET_HPP__

#include <fstream>
#include <string>
#include <unordered_map>

#include <SDL.h>
#include <SDL_image.h>

#include "nlohmann/json.hpp"

#include "AssetTypes.hpp"

namespace BloodSword::Asset
{
    AssetMapping<std::string> Locations = {};

    AssetMapping<SDL_Texture *> Textures = {};

    void Unload()
    {
        if (Asset::Textures.size() > 0)
        {
            for (auto it = Asset::Textures.begin(); it != Asset::Textures.end(); it++)
            {
                SDL_DestroyTexture(it->second);
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
                        auto surface = IMG_Load(path.c_str());

                        if (surface)
                        {
                            auto texture = SDL_CreateTextureFromSurface(renderer, surface);

                            if (texture)
                            {
                                Asset::Locations[object] = path;

                                Asset::Textures[object] = texture;
                            }

                            SDL_FreeSurface(surface);
                        }
                    }
                }
            }

            ifs.close();

            result = (!Locations.empty() && !Textures.empty() && (Textures.size() == Locations.size()));
        }

        return result;
    }

    SDL_Texture *Get(Asset::Type asset, Uint8 alpha)
    {
        SDL_Texture *texture = NULL;

        if (Asset::Textures.find(asset) != Asset::Textures.end())
        {
            texture = Asset::Textures[asset];
        }

        if (texture)
        {
            SDL_SetTextureColorMod(texture, alpha, alpha, alpha);
        }

        return texture;
    }

    SDL_Texture *Get(Asset::Type asset)
    {
        return Asset::Get(asset, 0xFF);
    }
}

#endif
