#ifndef __FEATURE_HPP__
#define __FEATURE_HPP__

#include "nlohmann/json.hpp"

#include "Templates.hpp"

namespace BloodSword::Feature
{
    enum class Type
    {
        NONE = -1,
        LOCATIONS,
        CHOICES,
        BATTLE,
        SHOP,
        ENDING
    };

    BloodSword::Mapping<Feature::Type> TypeMapping = {
        {Feature::Type::NONE, "NONE"},
        {Feature::Type::LOCATIONS, "LOCATIONS"},
        {Feature::Type::CHOICES, "CHOICES"},
        {Feature::Type::BATTLE, "BATTLE"},
        {Feature::Type::SHOP, "SHOP"},
        {Feature::Type::ENDING, "ENDING"}};

    Feature::Type Map(const char *feature)
    {
        return BloodSword::Find(Feature::TypeMapping, feature);
    }

    Feature::Type Map(std::string feature)
    {
        return Feature::Map(feature.c_str());
    }
}

namespace BloodSword::Features
{
    typedef std::vector<Feature::Type> List;

    Features::List Load(nlohmann::json &data)
    {
        auto features = Features::List();

        for (auto i = 0; i < int(data.size()); i++)
        {
            auto feature = !data[i].is_null() ? Feature::Map(std::string(data[i])) : Feature::Type::NONE;

            features.push_back(feature);
        }

        return features;
    }

    nlohmann::json Data(Features::List &features)
    {
        nlohmann::json data;

        for (auto &feature : features)
        {
            if (feature != Feature::Type::NONE)
            {
                data.push_back(Feature::TypeMapping[feature]);
            }
        }

        return data;
    }
}
#endif
