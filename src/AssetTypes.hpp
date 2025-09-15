#ifndef __ASSET_TYPES_HPP__
#define __ASSET_TYPES_HPP__

#include "Templates.hpp"

namespace BloodSword::Asset
{
    int NONE = -1;

    // asset type
    typedef int Type;

    template <typename T>
    using AssetMapping = BloodSword::UnorderedMap<Asset::Type, T>;

    typedef std::vector<Asset::Type> List;

    BloodSword::UnorderedMap<Asset::Type, std::string> TypeMapping = {};

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

    template <typename T>
    void MapTypes(BloodSword::UnorderedMap<T, Asset::Type> &types, BloodSword::UnorderedMap<T, const char *> &assets)
    {
        types.clear();

        for (auto asset : assets)
        {
            auto asset_type = Asset::Map(std::string(asset.second));

            types[asset.first] = asset_type;
        }
    }

    void MapTypes(std::vector<Asset::Type> &types, std::vector<const char *> &assets)
    {
        types.clear();

        for (auto asset : assets)
        {
            auto asset_type = Asset::Map(std::string(asset));

            types.push_back(asset_type);
        }
    }
}

#endif
