#ifndef __ITEM_TYPES_HPP__
#define __ITEM_TYPES_HPP__

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
        LEATHER_JERKIN,
        RINGMAIL,
        SILVER_ARMOUR,
        SHURIKEN,
        SILVER_RING,
        CRESCENTIUM_STEEL_SWORD,
        MAUVE_LIQUID,
        CHIMERA_SPITTLE,
        BLUTGETRANKER,
        OPAL_MEDALLION,
        OCTAGONAL_GLASS_PRISM,
        LOGE_SKYRUNNER,
        VELLUM_SCROLL,
        FOOD,
        STEEL_SCEPTRE,
        CHARGE,
        BRONZE_KEY,
        LARGE_STAMPED_GOLD_COIN,
        EMERALD_SCARAB,
        JEWELLED_SWORD,
        BATTLE_AXE,
        BRONZE_ARMOUR,
        BRONZE_BREASTPLATE,
        DAGGER_OF_VISLET,
        GOLDEN_SNUFF_BOX,
        DRAGONLORD_GEM,
        RUBY_RING,
        BIRCH_BARK_SCROLL,
        BLUE_ICE_JEWEL,
        BRONZE_KNIFE,
        FIRE_BLACKENED_SKULL,
        SCROLL_INVISIBILITY,
        SCROLL_HEALING,
        SCROLL_ADJUST,
        SCROLL_TIME_BLINK,
        SCROLL_PRECOGNITION,
        DAGGER,
        BLACK_LIQUID,
        RING_OF_WARDING,
        GREY_PEARL,
        SHARP_KNIFE,
        CASKET_OF_KALIUM,
        TALISMAN_OF_FLAME,
        GILDED_BRIDLE,
        POTION_OF_HEALING,
        BLUE_TOUCHSTONE,
        LEAD_RING,
        YELLOW_SCROLL,
        TARRY_BLACK_SUBSTANCE,
        EFFERVESCENT_LIQUID
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
        {Item::Type::LEATHER_JERKIN, "LEATHER JERKIN"},
        {Item::Type::RINGMAIL, "RINGMAIL"},
        {Item::Type::SILVER_ARMOUR, "SILVER ARMOUR"},
        {Item::Type::SHURIKEN, "SHURIKEN"},
        {Item::Type::SILVER_RING, "SILVER RING"},
        {Item::Type::MAUVE_LIQUID, "MAUVE LIQUID"},
        {Item::Type::CHIMERA_SPITTLE, "CHIMERA-SPITTLE"},
        {Item::Type::CRESCENTIUM_STEEL_SWORD, "CRESCENTIUM STEEL SWORD"},
        {Item::Type::BLUTGETRANKER, "BLUTGETRANKER"},
        {Item::Type::OPAL_MEDALLION, "OPAL MEDALLION"},
        {Item::Type::OCTAGONAL_GLASS_PRISM, "OCTAGONAL GLASS PRISM"},
        {Item::Type::LOGE_SKYRUNNER, "LOGE SKYRUNNER"},
        {Item::Type::VELLUM_SCROLL, "VELLUM SCROLL"},
        {Item::Type::FOOD, "FOOD"},
        {Item::Type::STEEL_SCEPTRE, "STEEL SCEPTRE"},
        {Item::Type::CHARGE, "CHARGE"},
        {Item::Type::BRONZE_KEY, "BRONZE KEY"},
        {Item::Type::LARGE_STAMPED_GOLD_COIN, "LARGE STAMPED GOLD COIN"},
        {Item::Type::EMERALD_SCARAB, "EMERALD SCARAB"},
        {Item::Type::JEWELLED_SWORD, "JEWELLED SWORD"},
        {Item::Type::BATTLE_AXE, "BATTLE AXE"},
        {Item::Type::BRONZE_ARMOUR, "BRONZE ARMOUR"},
        {Item::Type::BRONZE_BREASTPLATE, "BRONZE BREASTPLATE"},
        {Item::Type::DAGGER_OF_VISLET, "DAGGER OF VISLET"},
        {Item::Type::GOLDEN_SNUFF_BOX, "GOLDEN SNUFF-BOX"},
        {Item::Type::DRAGONLORD_GEM, "DRAGONLORD GEM"},
        {Item::Type::RUBY_RING, "RUBY RING"},
        {Item::Type::BIRCH_BARK_SCROLL, "BIRCH-BARK SCROLL"},
        {Item::Type::BLUE_ICE_JEWEL, "BLUE ICE JEWEL"},
        {Item::Type::BRONZE_KNIFE, "BRONZE KNIFE"},
        {Item::Type::FIRE_BLACKENED_SKULL, "FIRE-BLACKENED SKULL"},
        {Item::Type::SCROLL_INVISIBILITY, "SCROLL: INVISIBILITY"},
        {Item::Type::SCROLL_HEALING, "SCROLL: HEALING"},
        {Item::Type::SCROLL_ADJUST, "SCROLL: ADJUST"},
        {Item::Type::SCROLL_TIME_BLINK, "SCROLL: TIME BLINK"},
        {Item::Type::SCROLL_PRECOGNITION, "SCROLL: PRECOGNITION"},
        {Item::Type::BLACK_LIQUID, "BLACK LIQUID"},
        {Item::Type::DAGGER, "DAGGER"},
        {Item::Type::RING_OF_WARDING, "RING OF WARDING"},
        {Item::Type::GREY_PEARL, "GREY PEARL"},
        {Item::Type::SHARP_KNIFE, "SHARP KNIFE"},
        {Item::Type::CASKET_OF_KALIUM, "CASKET OF KALIUM"},
        {Item::Type::TALISMAN_OF_FLAME, "TALISMAN OF FLAME"},
        {Item::Type::GILDED_BRIDLE, "GILDED BRIDLE"},
        {Item::Type::POTION_OF_HEALING, "POTION OF HEALING"},
        {Item::Type::BLUE_TOUCHSTONE, "BLUE TOUCHSTONE"},
        {Item::Type::LEAD_RING, "LEAD RING"},
        {Item::Type::YELLOW_SCROLL, "YELLOW SCROLL"},
        {Item::Type::TARRY_BLACK_SUBSTANCE, "TARRY BLACK SUBSTANCE"},
        {Item::Type::EFFERVESCENT_LIQUID, "EFFERVESCENT LIQUID"}};

    Item::Type Map(const char *item)
    {
        return BloodSword::Find(Item::TypeMapping, item);
    }

    Item::Type Map(std::string item)
    {
        return Item::Map(item.c_str());
    }

    BloodSword::UnorderedMap<Item::Type, Item::Type> MeleeRequirements = {
        {Item::Type::STEEL_SCEPTRE, Item::Type::CHARGE}};

    BloodSword::UnorderedMap<Item::Type, Item::Type> RangedRequirements = {
        {Item::Type::BOW, Item::Type::ARROW}};

    BloodSword::UnorderedMap<Item::Type, Item::Type> StorageRequirements = {
        {Item::Type::BOW, Item::Type::QUIVER},
        {Item::Type::GOLD, Item::Type::POUCH}};

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

    BloodSword::Mapping<Item::CardType> CardMapping = {
        {Item::CardType::NONE, "NONE"},
        {Item::CardType::KING_OF_SERPENTS, "KING OF SERPENTS"},
        {Item::CardType::ACE_OF_STARS, "ACE OF STARS"},
        {Item::CardType::ACE_OF_SWORDS, "ACE OF SWORDS"},
        {Item::CardType::ACE_OF_RINGS, "ACE OF RINGS"},
        {Item::CardType::BUFFOON, "BUFFOON"}};

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
