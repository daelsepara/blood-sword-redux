#ifndef __ASSET_HPP__
#define __ASSET_HPP__

#include <map>
#include <string>
#include <vector>
#include <SDL.h>

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

    typedef std::pair<Asset::Type, SDL_Surface *> Surface;
    typedef std::pair<Asset::Type, std::string> Location;

    std::vector<Asset::Location> Path = {};
    std::vector<Asset::Surface> Graphics = {};

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

    SDL_Surface *Get(Asset::Type asset, Uint8 alpha)
    {
        SDL_Surface *surface = NULL;

        if (Asset::Graphics.size() > 0)
        {
            for (auto i = 0; i < Graphics.size(); i++)
            {
                if (Asset::Graphics[i].first == asset)
                {
                    surface = Asset::Graphics[i].second;

                    break;
                }
            }
        }

        if (surface)
        {
            SDL_SetSurfaceColorMod(surface, alpha, alpha, alpha);
        }

        return surface;
    }

    SDL_Surface *Get(Asset::Type asset)
    {
        return Asset::Get(asset, 0xFF);
    }

    SDL_Surface *Copy(SDL_Surface *Surface)
    {
        return Surface ? SDL_ConvertSurface(Surface, Surface->format, 0) : NULL;
    }

    SDL_Surface *Copy(SDL_Surface *Surface, Uint8 alpha)
    {
        auto surface = Asset::Copy(Surface);

        if (surface)
        {
            SDL_SetSurfaceColorMod(surface, alpha, alpha, alpha);
        }

        return surface;
    }
}

#endif