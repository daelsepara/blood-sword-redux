#ifndef __SKILLS_HPP__
#define __SKILLS_HPP__

#include <string>

#include "Templates.hpp"

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
        SPELLS
    };

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
        {Type::SPELLS, "SPELLS"}};

    Skills::Type Map(const char *skill)
    {
        return BloodSword::Find(Skills::TypeMapping, skill);
    }

    Skills::Type Map(std::string skill)
    {
        return Skills::Map(skill.c_str());
    }
}

#endif
