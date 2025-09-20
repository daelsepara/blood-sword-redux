#ifndef __ASSET_TYPES_HPP__
#define __ASSET_TYPES_HPP__

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include <SDL.h>

#include "Templates.hpp"

namespace BloodSword::Asset
{
    // asset missing or invalid
    int NONE = -1;

    // asset type id
    typedef int Type;

    // mapping of asset type ids to other types
    template <typename T>
    using Mapping = BloodSword::UnorderedMap<Asset::Type, T>;

    // lookup table for mapping T to asset type id
    template <typename T>
    using Lookup = BloodSword::UnorderedMap<T, Asset::Type>;

    // mapping of T to SDL sextures
    template <typename T>
    using TextureMap = BloodSword::UnorderedMap<T, SDL_Texture *>;

    // list of textures
    typedef std::vector<SDL_Texture *> TextureList;

    // asset list
    typedef std::vector<Asset::Type> List;

    // asset type to string mapping
    Asset::Mapping<std::string> TypeMapping = {};

    // get asset type id
    Asset::Type Map(std::string asset)
    {
        return BloodSword::Find(Asset::TypeMapping, asset, Asset::NONE);
    }

    // get asset type id
    Asset::Type Map(const char *asset)
    {
        return Asset::Map(std::string(asset));
    }

    // generate T -> asset type id mapping
    template <typename T>
    void MapTypes(Asset::Lookup<T> &types, BloodSword::ConstStrings<T> &assets)
    {
        types.clear();

        for (auto &asset : assets)
        {
            types[asset.first] = Asset::Map(std::string(asset.second));
        }
    }

    // generate vector of asset type ids, must be a vector of type T
    void MapTypes(Asset::List &types, std::vector<const char *> &assets)
    {
        types.clear();

        for (auto &asset : assets)
        {
            auto asset_type = Asset::Map(asset);

            types.push_back(asset_type);
        }
    }
}

#endif
