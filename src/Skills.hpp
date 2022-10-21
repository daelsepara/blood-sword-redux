#ifndef __SKILLS_HPP__
#define __SKILLS_HPP__

#include <string>
#include <unordered_map>
#include <vector>

#include "Templates.hpp"
#include "AssetTypes.hpp"

namespace BloodSword::Skills
{
    // skill type
    enum class Type
    {
        NONE = -1,
        ARCHERY,
        DODGING,
        QUARTERSTAFF,
        HEALING,
        ESP,
        PARANORMAL_SIGHT,
        LEVITATION,
        EXORCISM,
        SPELLS,
        CALL_TO_MIND,
        CAST_SPELL,
        SHURIKEN
    };

    template <typename T>
    using Mapped = std::unordered_map<Skills::Type, T>;

    Mapping<Skills::Type> TypeMapping = {
        {Type::NONE, "NONE"},
        {Type::ARCHERY, "ARCHERY"},
        {Type::DODGING, "DODGING"},
        {Type::QUARTERSTAFF, "QUARTERSTAFF"},
        {Type::HEALING, "HEALING"},
        {Type::ESP, "ESP"},
        {Type::PARANORMAL_SIGHT, "PARANORMAL SIGHT"},
        {Type::LEVITATION, "LEVITATION"},
        {Type::EXORCISM, "EXORCISM"},
        {Type::SPELLS, "SPELLS"},
        {Type::CALL_TO_MIND, "CALL TO MIND"},
        {Type::CAST_SPELL, "CAST SPELL"},
        {Type::SHURIKEN, "SHURIKEN"}};

    std::vector<Skills::Type> BattleSkills = {
        Type::ARCHERY,
        Type::DODGING,
        Type::QUARTERSTAFF,
        Type::SPELLS,
        Type::SHURIKEN};

    std::vector<Skills::Type> StorySkills = {
        Type::HEALING,
        Type::ESP,
        Type::PARANORMAL_SIGHT,
        Type::SPELLS,
        Type::LEVITATION,
        Type::EXORCISM};

    std::unordered_map<Skills::Type, Asset::Type> Assets = {
        {Type::NONE, Asset::Type::NONE},
        {Type::ARCHERY, Asset::Type::ARCHERY},
        {Type::DODGING, Asset::Type::DODGING},
        {Type::QUARTERSTAFF, Asset::Type::QUARTERSTAFF},
        {Type::HEALING, Asset::Type::HEALING},
        {Type::ESP, Asset::Type::ESP},
        {Type::PARANORMAL_SIGHT, Asset::Type::PARANORMAL_SIGHT},
        {Type::LEVITATION, Asset::Type::LEVITATION},
        {Type::EXORCISM, Asset::Type::EXORCISM},
        {Type::SPELLS, Asset::Type::SPELLS},
        {Type::SHURIKEN, Asset::Type::SHURIKEN}};

    Skills::Type Map(const char *skill)
    {
        return BloodSword::Find(Skills::TypeMapping, skill);
    }

    Skills::Type Map(std::string skill)
    {
        return Skills::Map(skill.c_str());
    }

    bool In(std::vector<Skills::Type> &list, Skills::Type skill)
    {
        return list.size() > 0 && BloodSword::Find(list, skill) != list.end();
    }

    bool IsBattleSkill(Skills::Type skill)
    {
        return Skills::In(Skills::BattleSkills, skill);
    }

    bool IsStorySkill(Skills::Type skill)
    {
        return Skills::In(Skills::StorySkills, skill);
    }
}

#endif
