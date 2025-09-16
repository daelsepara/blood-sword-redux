#ifndef __ITEM_TARGET_EFFECTS_HPP__
#define __ITEM_TARGET_EFFECTS_HPP__

#include "Templates.hpp"

// item target effects
namespace BloodSword::Item
{
    // item target effect types
    enum class TargetEffect
    {
        NONE = -1,
        KILL_TARGET,
        DAMAGE_TARGET,
        THROW,
        SELF_BURN,
        IMMUNE
    };

    // list of item target effects
    typedef std::vector<Item::TargetEffect> TargetEffects;

    // mapping of item target effects to strings
    BloodSword::StringMap<Item::TargetEffect> TargetEffectMapping = {
        {Item::TargetEffect::NONE, "NONE"},
        {Item::TargetEffect::KILL_TARGET, "KILL TARGET"},
        {Item::TargetEffect::DAMAGE_TARGET, "DAMAGE TARGET"},
        {Item::TargetEffect::THROW, "THROW"},
        {Item::TargetEffect::SELF_BURN, "SELF BURN"},
        {Item::TargetEffect::IMMUNE, "IMMUNE"}};

    // map string to item target effect
    Item::TargetEffect MapTargetEffect(const char *target_effect)
    {
        return BloodSword::Find(Item::TargetEffectMapping, target_effect);
    }

    // map string to item target effect
    Item::TargetEffect MapTargetEffect(std::string target_effect)
    {
        return Item::MapTargetEffect(target_effect.c_str());
    }
}

#endif
