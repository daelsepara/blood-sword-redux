#ifndef __SKILLS_HPP__
#define __SKILLS_HPP__

#include <map>
#include <string>

namespace BloodSword::Skills
{
    enum class Type
    {
        None = -1,
        Archery,
        Dodging,
        QuarterStaff,
        Healing,
        Spells
    };

    std::map<Skills::Type, const char *> SkillsMapping = {
        {Skills::Type::None, "NONE"},
        {Skills::Type::Archery, "ARCHERY"},
        {Skills::Type::Dodging, "DODGING"},
        {Skills::Type::QuarterStaff, "QUARTERSTAFF"},
        {Skills::Type::Healing, "HEALING"},
        {Skills::Type::Spells, "SPELLS"}};

    Skills::Type GetType(const char *skill)
    {
        auto result = Skills::Type::None;

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
