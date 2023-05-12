#ifndef __ITEM_TYPES_HPP__
#define __ITEM_TYPES_HPP__

#include <string>

#include "Templates.hpp"

namespace BloodSword::Item
{
    // item type
    enum class Type
    {
        NONE,
        SWORD,
        BOW,
        ARROW,
        QUARTERSTAFF,
        QUIVER,
        POUCH,
        GOLD,
        CHAINMAIL,
        LEATHER_ARMOUR,
        RINGMAIL,
        SILVER_ARMOUR,
        SHURIKEN
    };

    BloodSword::Mapping<Item::Type> TypeMapping = {
        {Item::Type::NONE, "NONE"},
        {Item::Type::SWORD, "SWORD"},
        {Item::Type::BOW, "BOW"},
        {Item::Type::ARROW, "ARROW"},
        {Item::Type::QUARTERSTAFF, "QUARTERSTAFF"},
        {Item::Type::QUIVER, "QUIVER"},
        {Item::Type::POUCH, "POUCH"},
        {Item::Type::GOLD, "GOLD"},
        {Item::Type::CHAINMAIL, "CHAINMAIL"},
        {Item::Type::LEATHER_ARMOUR, "LEATHER ARMOUR"},
        {Item::Type::RINGMAIL, "RINGMAIL"},
        {Item::Type::SILVER_ARMOUR, "SILVER ARMOUR"},
        {Item::Type::SHURIKEN, "SHURIKEN"}};

    Item::Type Map(const char *item)
    {
        return BloodSword::Find(Item::TypeMapping, item);
    }

    Item::Type Map(std::string item)
    {
        return Item::Map(item.c_str());
    }
}

#endif
