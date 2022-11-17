#ifndef __FEATURE_HPP__
#define __FEATURE_HPP__

#include "Templates.hpp"

namespace BloodSword::Feature
{
    enum class Type
    {
        NONE = -1,
        DESTINATIONS,
        CHOICES,
        BATTLE,
        SHOP
    };

    Mapping<Feature::Type> TypeMapping = {
        {Type::NONE, "NONE"},
        {Type::DESTINATIONS, "DESTINATIONS"},
        {Type::CHOICES, "CHOICES"},
        {Type::BATTLE, "BATTLE"},
        {Type::SHOP, "SHOP"}};

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