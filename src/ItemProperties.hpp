#ifndef __ITEM_PROPERTIES_HPP__
#define __ITEM_PROPERTIES_HPP__

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
        CANNOT_DROP,
        CANNOT_TRADE,
        LIQUID,
        INVISIBLE,
        READABLE,
        EDIBLE,
        COMBAT,
        RUSTY,
        BROKEN,
        CURSED,
        REQUIRES_TARGET,
        RESURRECTION,
        ALL_RANGES,
        ACCESSORY
    };

    typedef std::vector<Item::Property> Properties;

    BloodSword::Mapping<Item::Property> PropertyMapping = {
        {Item::Property::NONE, "NONE"},
        {Item::Property::NORMAL, "NORMAL"},
        {Item::Property::WEAPON, "WEAPON"},
        {Item::Property::ARMOUR, "ARMOUR"},
        {Item::Property::ACCESSORY, "ACCESSORY"},
        {Item::Property::CONTAINER, "CONTAINER"},
        {Item::Property::EQUIPPED, "EQUIPPED"},
        {Item::Property::POISONED, "POISONED"},
        {Item::Property::PRIMARY, "PRIMARY"},
        {Item::Property::SECONDARY, "SECONDARY"},
        {Item::Property::RANGED, "RANGED"},
        {Item::Property::CANNOT_DROP, "CANNOT DROP"},
        {Item::Property::CANNOT_TRADE, "CANNOT TRADE"},
        {Item::Property::LIQUID, "LIQUID"},
        {Item::Property::INVISIBLE, "INVISIBLE"},
        {Item::Property::READABLE, "READABLE"},
        {Item::Property::EDIBLE, "EDIBLE"},
        {Item::Property::COMBAT, "COMBAT"},
        {Item::Property::RUSTY, "RUSTY"},
        {Item::Property::BROKEN, "BROKEN"},
        {Item::Property::CURSED, "CURSED"},
        {Item::Property::REQUIRES_TARGET, "REQUIRES TARGET"},
        {Item::Property::RESURRECTION, "RESURRECTION"},
        {Item::Property::ALL_RANGES, "ALL RANGES"}};

    Item::Properties Invisible = {
        Item::Property::NONE,
        Item::Property::CONTAINER,
        Item::Property::LIQUID,
        Item::Property::READABLE,
        Item::Property::CANNOT_DROP,
        Item::Property::CANNOT_TRADE,
        Item::Property::COMBAT,
        Item::Property::REQUIRES_TARGET,
        Item::Property::ALL_RANGES};

    Item::Property MapProperty(const char *property)
    {
        return BloodSword::Find(Item::PropertyMapping, property);
    }

    Item::Property MapProperty(std::string property)
    {
        return Item::MapProperty(property.c_str());
    }

    bool IsInvisible(Item::Property property)
    {
        return property != Item::Property::NONE && BloodSword::Has(Item::Invisible, property);
    }
}

#endif
