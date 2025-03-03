#ifndef __ITEM_TYPES_HPP__
#define __ITEM_TYPES_HPP__

#include <string>

#include "Templates.hpp"

namespace BloodSword::Item
{
    // item type
    enum class Type
    {
        NONE = -1,
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
        SHURIKEN,
        SILVER_RING,
        CRESCENTIUM_STEEL_SWORD,
        MAUVE_LIQUID
    };

    // for the magus kalugen's card game in battlepits
    enum class CardType
    {
        NONE = -1,
        KING_OF_SERPENTS,
        ACE_OF_STARS,
        ACE_OF_SWORDS,
        ACE_OF_RINGS,
        BUFFOON
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
        {Item::Type::SHURIKEN, "SHURIKEN"},
        {Item::Type::SILVER_RING, "SILVER RING"},
        {Item::Type::MAUVE_LIQUID, "MAUVE LIQUID"},
        {Item::Type::CRESCENTIUM_STEEL_SWORD, "CRESCENTIUM STEEL SWORD"}};

    BloodSword::Mapping<Item::CardType> CardMapping = {
        {Item::CardType::NONE, "NONE"},
        {Item::CardType::KING_OF_SERPENTS, "KING OF SERPENTS"},
        {Item::CardType::ACE_OF_STARS, "ACE OF STARS"},
        {Item::CardType::ACE_OF_SWORDS, "ACE OF SWORDS"},
        {Item::CardType::ACE_OF_RINGS, "ACE OF RINGS"},
        {Item::CardType::BUFFOON, "BUFFOON"}};

    Item::Type Map(const char *item)
    {
        return BloodSword::Find(Item::TypeMapping, item);
    }

    Item::Type Map(std::string item)
    {
        return Item::Map(item.c_str());
    }

    Item::CardType MapCard(const char *item)
    {
        return BloodSword::Find(Item::CardMapping, item);
    }

    Item::CardType MapCard(std::string item)
    {
        return Item::MapCard(item.c_str());
    }
}

#endif
