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
        PARANORMAL_SIGHT,
        LEVITATION,
        EXORCISM
    };

    std::map<Skills::Type, const char *> TypeMapping = {
        {Type::NONE, "NONE"},
        {Type::ARCHERY, "ARCHERY"},
        {Type::DODGING, "DODGING"},
        {Type::QUARTERSTAFF, "QUARTERSTAFF"},
        {Type::HEALING, "HEALING"},
        {Type::SPELLS, "SPELLS"},
        {Type::ESP, "ESP"},
        {Type::PARANORMAL_SIGHT, "PARANORMAL SIGHT"},
        {Type::LEVITATION, "LEVITATION"},
        {Type::EXORCISM, "EXORCISM"}};

    Skills::Type GetType(const char *skill)
    {
        auto result = Type::NONE;

        auto skills = Skills::TypeMapping.begin();

        while (skills != Skills::TypeMapping.end())
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
