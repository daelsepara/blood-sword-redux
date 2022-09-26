#ifndef __ITEM_PROPERTIES_HPP__
#define __ITEM_PROPERTIES_HPP__

#include <string>
#include <unordered_map>

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
        POISONED
    };

    Mapping<Item::Property> PropertyMapping = {
        {Property::NONE, "NONE"},
        {Property::NORMAL, "NORMAL"},
        {Property::WEAPON, "WEAPON"},
        {Property::ARMOUR, "ARMOUR"},
        {Property::CONTAINER, "CONTAINER"},
        {Property::EQUIPPED, "EQUIPPED"},
        {Property::POISONED, "POISONED"}};

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
