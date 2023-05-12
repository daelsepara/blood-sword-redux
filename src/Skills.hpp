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
        {Skills::Type::NONE, "NONE"},
        {Skills::Type::ARCHERY, "ARCHERY"},
        {Skills::Type::DODGING, "DODGING"},
        {Skills::Type::QUARTERSTAFF, "QUARTERSTAFF"},
        {Skills::Type::HEALING, "HEALING"},
        {Skills::Type::ESP, "ESP"},
        {Skills::Type::PARANORMAL_SIGHT, "PARANORMAL SIGHT"},
        {Skills::Type::LEVITATION, "LEVITATION"},
        {Skills::Type::EXORCISM, "EXORCISM"},
        {Skills::Type::SPELLS, "SPELLS"},
        {Skills::Type::CALL_TO_MIND, "CALL TO MIND"},
        {Skills::Type::CAST_SPELL, "CAST SPELL"},
        {Skills::Type::SHURIKEN, "SHURIKEN"}};

    std::vector<Skills::Type> BattleSkills = {
        Skills::Type::ARCHERY,
        Skills::Type::DODGING,
        Skills::Type::QUARTERSTAFF,
        Skills::Type::SPELLS,
        Skills::Type::SHURIKEN};

    std::vector<Skills::Type> StorySkills = {
        Skills::Type::HEALING,
        Skills::Type::ESP,
        Skills::Type::PARANORMAL_SIGHT,
        Skills::Type::SPELLS,
        Skills::Type::LEVITATION,
        Skills::Type::EXORCISM};

    std::unordered_map<Skills::Type, Asset::Type> Assets = {
        {Skills::Type::NONE, Asset::Type::NONE},
        {Skills::Type::ARCHERY, Asset::Type::ARCHERY},
        {Skills::Type::DODGING, Asset::Type::DODGING},
        {Skills::Type::QUARTERSTAFF, Asset::Type::QUARTERSTAFF},
        {Skills::Type::HEALING, Asset::Type::HEALING},
        {Skills::Type::ESP, Asset::Type::ESP},
        {Skills::Type::PARANORMAL_SIGHT, Asset::Type::PARANORMAL_SIGHT},
        {Skills::Type::LEVITATION, Asset::Type::LEVITATION},
        {Skills::Type::EXORCISM, Asset::Type::EXORCISM},
        {Skills::Type::SPELLS, Asset::Type::SPELLS},
        {Skills::Type::SHURIKEN, Asset::Type::SHURIKEN}};

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
