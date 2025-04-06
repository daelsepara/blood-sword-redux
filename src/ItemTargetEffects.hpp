#ifndef __ITEM_TARGET_EFFECTS_HPP__
#define __ITEM_TARGET_EFFECTS_HPP__

#include "Templates.hpp"

namespace BloodSword::Item
{
    // item property
    enum class TargetEffect
    {
        NONE = -1,
        KILL_TARGET,
        DAMAGE_TARGET,
        THROW,
        SELF_BURN,
        IMMUNE
    };

    typedef std::vector<Item::TargetEffect> TargetEffects;

    BloodSword::Mapping<Item::TargetEffect> TargetEffectMapping = {
        {Item::TargetEffect::NONE, "NONE"},
        {Item::TargetEffect::KILL_TARGET, "KILL TARGET"},
        {Item::TargetEffect::DAMAGE_TARGET, "DAMAGE TARGET"},
        {Item::TargetEffect::THROW, "THROW"},
        {Item::TargetEffect::SELF_BURN, "SELF BURN"},
        {Item::TargetEffect::IMMUNE, "IMMUNE"}};

    Item::TargetEffect MapTargetEffect(const char *target_effect)
    {
        return BloodSword::Find(Item::TargetEffectMapping, target_effect);
    }

    Item::TargetEffect MapTargetEffect(std::string target_effect)
    {
        return Item::MapTargetEffect(target_effect.c_str());
    }
}

#endif
