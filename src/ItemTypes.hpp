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

    Mapping<Item::Type> TypeMapping = {
        {Type::NONE, "NONE"},
        {Type::SWORD, "SWORD"},
        {Type::BOW, "BOW"},
        {Type::ARROW, "ARROW"},
        {Type::QUARTERSTAFF, "QUARTERSTAFF"},
        {Type::QUIVER, "QUIVER"},
        {Type::POUCH, "POUCH"},
        {Type::GOLD, "GOLD"},
        {Type::CHAINMAIL, "CHAINMAIL"},
        {Type::LEATHER_ARMOUR, "LEATHER ARMOUR"},
        {Type::RINGMAIL, "RINGMAIL"},
        {Type::SILVER_ARMOUR, "SILVER ARMOUR"},
        {Type::SHURIKEN, "SHURIKEN"}};

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
