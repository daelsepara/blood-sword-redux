#ifndef __CHARACTER_TYPES_HPP__
#define __CHARACTER_TYPES_HPP__

#include <unordered_map>
#include <string>

#include "Templates.hpp"

namespace BloodSword::Character
{
    enum class ControlType
    {
        NONE = -1,
        PLAYER,
        NPC
    };

    std::unordered_map<Character::ControlType, const char *> ControlTypeMapping = {
        {ControlType::NONE, "NONE"},
        {ControlType::PLAYER, "PLAYER"},
        {ControlType::NPC, "NPC"}};

    Character::ControlType MapControlType(const char *controlType)
    {
        return BloodSword::Find(Character::ControlTypeMapping, controlType);
    }

    Character::ControlType MapControlType(std::string controlType)
    {
        return Character::MapControlType(controlType.c_str());
    }

    enum class Status
    {
        NONE,
        DEFENDING,
        DEFENDED,
        FLEEING,
        ENTHRALLED,
        AWAY,
        PARALYZED,
        IN_COMBAT
    };

    std::unordered_map<Character::Status, const char *> StatusMapping = {
        {Status::NONE, "NONE"},
        {Status::DEFENDING, "DEFENDING"},
        {Status::DEFENDED, "DEFENDED"},
        {Status::FLEEING, "FLEEING"},
        {Status::ENTHRALLED, "ENTHRALLED"},
        {Status::AWAY, "AWAY"},
        {Status::PARALYZED, "PARALYZED"},
        {Status::IN_COMBAT, "IN COMBAT"}};

    Character::Status MapStatus(const char *status)
    {
        return BloodSword::Find(Character::StatusMapping, status);
    }

    Character::Status MapStatus(std::string status)
    {
        return Character::MapStatus(status.c_str());
    }

    enum class Class
    {
        NONE = -1,
        WARRIOR,
        TRICKSTER,
        SAGE,
        ENCHANTER
    };

    std::unordered_map<Character::Class, const char *> ClassMapping = {
        {Class::NONE, "NONE"},
        {Class::WARRIOR, "WARRIOR"},
        {Class::TRICKSTER, "TRICKSTER"},
        {Class::SAGE, "SAGE"},
        {Class::ENCHANTER, "ENCHANTER"}};

    Character::Class Map(const char *characterClass)
    {
        return BloodSword::Find(Character::ClassMapping, characterClass);
    }

    Character::Class Map(std::string characterClass)
    {
        return Character::Map(characterClass.c_str());
    }
}

#endif
