#ifndef __SKILLS_HPP__
#define __SKILLS_HPP__

#include <map>
#include <string>

namespace BloodSword::Skills
{
    enum class Type
    {
        NONE = -1,
        ARCHERY,
        DODGING,
        QUARTERSTAFF,
        HEALING,
        SPELLS,
        ESP,
        PARANORMAL_SIGHT
    };

    std::map<Skills::Type, const char *> SkillsMapping = {
        {Skills::Type::NONE, "NONE"},
        {Skills::Type::ARCHERY, "ARCHERY"},
        {Skills::Type::DODGING, "DODGING"},
        {Skills::Type::QUARTERSTAFF, "QUARTERSTAFF"},
        {Skills::Type::HEALING, "HEALING"},
        {Skills::Type::SPELLS, "SPELLS"},
        {Skills::Type::ESP, "ESP"},
        {Skills::Type::PARANORMAL_SIGHT, "PARANORMAL SIGHT"}};

    Skills::Type GetType(const char *skill)
    {
        auto result = Skills::Type::NONE;

        auto skills = Skills::SkillsMapping.begin();

        while (skills != Skills::SkillsMapping.end())
        {
            if (std::strcmp(skills->second, skill) == 0)
            {
                result = skills->first;

                break;
            }

            skills++;
        }

        return result;
    }

    Skills::Type GetType(std::string skill)
    {
        return Skills::GetType(skill.c_str());
    }
}

#endif
