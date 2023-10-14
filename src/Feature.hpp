#ifndef __FEATURE_HPP__
#define __FEATURE_HPP__

#include "Templates.hpp"

namespace BloodSword::Feature
{
    enum class Type
    {
        NONE = -1,
        LOCATIONS,
        CHOICES,
        BATTLE,
        SHOP
    };

    BloodSword::Mapping<Feature::Type> TypeMapping = {
        {Feature::Type::NONE, "NONE"},
        {Feature::Type::LOCATIONS, "LOCATIONS"},
        {Feature::Type::CHOICES, "CHOICES"},
        {Feature::Type::BATTLE, "BATTLE"},
        {Feature::Type::SHOP, "SHOP"}};

    Feature::Type Map(const char *feature)
    {
        return BloodSword::Find(Feature::TypeMapping, feature);
    }

    Feature::Type Map(std::string feature)
    {
        return Feature::Map(feature.c_str());
    }
}
#endif
