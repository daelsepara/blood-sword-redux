#ifndef __ASSET_TYPES_HPP__
#define __ASSET_TYPES_HPP__

#include "Templates.hpp"

namespace BloodSword::Asset
{
    // asset missing or invalid
    int NONE = -1;

    // asset type id
    typedef int Type;

    template <typename T>
    using AssetMapping = BloodSword::UnorderedMap<Asset::Type, T>;

    // asset list
    typedef std::vector<Asset::Type> List;

    BloodSword::UnorderedMap<Asset::Type, std::string> TypeMapping = {};

    // get asset type id
    Asset::Type Map(std::string asset)
    {
        auto result = Asset::NONE;

        for (auto &type : Asset::TypeMapping)
        {
            if (type.second == asset)
            {
                result = type.first;

                break;
            }
        }

        return result;
    }

    // get asset type id
    Asset::Type Map(const char *asset)
    {
        return Asset::Map(std::string(asset));
    }

    // generate T -> asset type id mapping
    template <typename T>
    void MapTypes(BloodSword::UnorderedMap<T, Asset::Type> &types, BloodSword::UnorderedMap<T, const char *> &assets)
    {
        types.clear();

        for (auto asset : assets)
        {
            types[asset.first] = Asset::Map(std::string(asset.second));
        }
    }

    // generate vector of asset type ids, must be a vector of type T
    void MapTypes(Asset::List &types, std::vector<const char *> &assets)
    {
        types.clear();

        for (auto asset : assets)
        {
            auto asset_type = Asset::Map(asset);

            types.push_back(asset_type);
        }
    }
}

#endif
