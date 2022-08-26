#ifndef __ASSET_HPP__
#define __ASSET_HPP__

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include "nlohmann/json.hpp"

namespace BloodSword::Asset
{
    enum class Type
    {
        NONE = -1,
        WARRIOR,
        TRICKSTER,
        SAGE,
        ENCHANTER
    };

    std::map<Asset::Type, const char *> TypeMapping = {
        {Type::NONE, "NONE"},
        {Type::WARRIOR, "WARRIOR"},
        {Type::TRICKSTER, "TRICKSTER"},
        {Type::SAGE, "SAGE"},
        {Type::ENCHANTER, "ENCHANTER"},
    };

    typedef std::pair<Asset::Type, SDL_Texture *> Texture;

    typedef std::pair<Asset::Type, std::string> Location;

    std::vector<Asset::Location> Path = {};

    std::vector<Asset::Texture> Graphics = {};

    Asset::Type GetType(const char *asset)
    {
        auto result = Asset::Type::NONE;

        auto types = Asset::TypeMapping.begin();

        while (types != Asset::TypeMapping.end())
        {
            if (std::strcmp(types->second, asset) == 0)
            {
                result = types->first;

                break;
            }

            types++;
        }

        return result;
    }

    Asset::Type GetType(std::string asset)
    {
        return Asset::GetType(asset.c_str());
    }

    void Unload()
    {
        if (Asset::Graphics.size() > 0)
        {
            for (auto i = 0; i < Asset::Graphics.size(); i++)
            {
                if (Asset::Graphics[i].second && Asset::Graphics[i].second)
                {
                    SDL_DestroyTexture(Asset::Graphics[i].second);

                    Asset::Graphics[i].second = NULL;
                }
            }

            Asset::Graphics.clear();

            Asset::Path.clear();
        }
    }

    bool Load(SDL_Renderer *renderer, const char *assets)
    {
        auto result = false;

        Asset::Unload();

        Asset::Path.clear();

        Asset::Graphics.clear();

        std::ifstream ifs(assets);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data["asset"].is_null() && data["asset"].is_array() && data["asset"].size() > 0)
            {
                for (auto i = 0; i < data["asset"].size(); i++)
                {
                    auto object = !data["asset"][i]["id"].is_null() ? Asset::GetType(std::string(data["asset"][i]["id"])) : Asset::Type::NONE;

                    auto path = !data["asset"][i]["path"].is_null() ? std::string(data["asset"][i]["path"]) : "";

                    if (!path.empty() && object != Asset::Type::NONE)
                    {
                        auto surface = IMG_Load(path.c_str());

                        if (surface)
                        {
                            auto texture = SDL_CreateTextureFromSurface(renderer, surface);

                            if (texture)
                            {
                                Asset::Path.push_back(std::make_pair(object, path));

                                Asset::Graphics.push_back(std::make_pair(object, texture));
                            }

                            SDL_FreeSurface(surface);
                        }
                    }
                }
            }

            ifs.close();

            result = !Path.empty() && !Graphics.empty() && (Graphics.size() == Path.size());
        }

        return result;
    }

    SDL_Texture *Get(Asset::Type asset, Uint8 alpha)
    {
        SDL_Texture *texture = NULL;

        if (Asset::Graphics.size() > 0)
        {
            for (auto i = 0; i < Graphics.size(); i++)
            {
                if (Asset::Graphics[i].first == asset)
                {
                    texture = Asset::Graphics[i].second;

                    break;
                }
            }
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
