#ifndef __ITEM_PROPERTIES_HPP__
#define __ITEM_PROPERTIES_HPP__

#include <string>
#include <unordered_map>
#include <vector>

#include "Templates.hpp"

namespace BloodSword::Item
{
    // item property
    enum class Property
    {
        NONE = -1,
        NORMAL = 0,
        WEAPON,
        ARMOUR,
        CONTAINER,
        EQUIPPED,
        POISONED,
        PRIMARY,
        SECONDARY,
        RANGED,
        CANNOT_DROP
    };

    typedef std::vector<Item::Property> Properties;

    BloodSword::Mapping<Item::Property> PropertyMapping = {
        {Item::Property::NONE, "NONE"},
        {Item::Property::NORMAL, "NORMAL"},
        {Item::Property::WEAPON, "WEAPON"},
        {Item::Property::ARMOUR, "ARMOUR"},
        {Item::Property::CONTAINER, "CONTAINER"},
        {Item::Property::EQUIPPED, "EQUIPPED"},
        {Item::Property::POISONED, "POISONED"},
        {Item::Property::PRIMARY, "PRIMARY"},
        {Item::Property::SECONDARY, "SECONDARY"},
        {Item::Property::RANGED, "RANGED"},
        {Item::Property::CANNOT_DROP, "CANNOT DROP"}};

    Item::Property MapProperty(const char *property)
    {
        return BloodSword::Find(Item::PropertyMapping, property);
    }

    Item::Property MapProperty(std::string property)
    {
        return Item::MapProperty(property.c_str());
    }
}

#endif
