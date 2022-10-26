#ifndef __CHARACTER_TYPES_HPP__
#define __CHARACTER_TYPES_HPP__

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

    Mapping<Character::ControlType> ControlTypeMapping = {
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

    // character status types
    enum class Status
    {
        NONE,
        DEFENDING,
        DEFENDED,
        FLEEING,
        ENTHRALLED,
        AWAY,
        PARALYZED,
        KNOCKED_OUT,
        IN_BATTLE,
        NIGHTHOWL,
        FPR_PLUS1,
        FPR_PLUS2
    };

    Mapping<Character::Status> StatusMapping = {
        {Status::NONE, "NONE"},
        {Status::DEFENDING, "DEFENDING"},
        {Status::DEFENDED, "DEFENDED"},
        {Status::FLEEING, "FLEEING"},
        {Status::ENTHRALLED, "ENTHRALLED"},
        {Status::AWAY, "AWAY"},
        {Status::PARALYZED, "PARALYZED"},
        {Status::KNOCKED_OUT, "KNOCKED OUT"},
        {Status::IN_BATTLE, "IN BATTLE"},
        {Status::NIGHTHOWL, "NIGHTHOWL"},
        {Status::FPR_PLUS1, "FPR +1"},
        {Status::FPR_PLUS2, "FPR +2"}};

    Character::Status MapStatus(const char *status)
    {
        return BloodSword::Find(Character::StatusMapping, status);
    }

    Character::Status MapStatus(std::string status)
    {
        return Character::MapStatus(status.c_str());
    }

    // character classes (player and NPC)
    enum class Class
    {
        NONE = -1,
        WARRIOR,
        TRICKSTER,
        SAGE,
        ENCHANTER
    };

    Mapping<Character::Class> ClassMapping = {
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
