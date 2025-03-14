#ifndef __CHARACTER_TYPES_HPP__
#define __CHARACTER_TYPES_HPP__

#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "Templates.hpp"
#include "Target.hpp"

namespace BloodSword::Character
{
    enum class ControlType
    {
        NONE = -1,
        PLAYER,
        NPC
    };

    BloodSword::Mapping<Character::ControlType> ControlTypeMapping = {
        {Character::ControlType::NONE, "NONE"},
        {Character::ControlType::PLAYER, "PLAYER"},
        {Character::ControlType::NPC, "NPC"}};

    Character::ControlType MapControlType(const char *control_type)
    {
        return BloodSword::Find(Character::ControlTypeMapping, control_type);
    }

    Character::ControlType MapControlType(std::string control_type)
    {
        return Character::MapControlType(control_type.c_str());
    }

    // character status types
    enum class Status
    {
        NONE = -1,
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
        FPR_PLUS2,
        EXCLUDED,
        TACTICS,
        MESMERISED,
        INSTANT_DEATH,
        ENTANGLED
    };

    BloodSword::Mapping<Character::Status> StatusMapping = {
        {Character::Status::NONE, "NONE"},
        {Character::Status::DEFENDING, "DEFENDING"},
        {Character::Status::DEFENDED, "DEFENDED"},
        {Character::Status::FLEEING, "FLEEING"},
        {Character::Status::ENTHRALLED, "ENTHRALLED"},
        {Character::Status::AWAY, "AWAY"},
        {Character::Status::PARALYZED, "PARALYZED"},
        {Character::Status::KNOCKED_OUT, "KNOCKED OUT"},
        {Character::Status::IN_BATTLE, "IN BATTLE"},
        {Character::Status::NIGHTHOWL, "NIGHTHOWL"},
        {Character::Status::FPR_PLUS1, "FPR +1"},
        {Character::Status::FPR_PLUS2, "FPR +2"},
        {Character::Status::EXCLUDED, "EXCLUDED"},
        {Character::Status::TACTICS, "TACTICS"},
        {Character::Status::MESMERISED, "MESMERISED"},
        {Character::Status::INSTANT_DEATH, "INSTANT DEATH"},
        {Character::Status::ENTANGLED, "ENTANGLED"}};

    BloodSword::IntMapping<Character::Status> Duration = {
        {Character::Status::NONE, -1},
        {Character::Status::DEFENDING, -1},
        {Character::Status::DEFENDED, 1},
        {Character::Status::FLEEING, -1},
        {Character::Status::ENTHRALLED, -1},
        {Character::Status::AWAY, -1},
        {Character::Status::PARALYZED, -1},
        {Character::Status::KNOCKED_OUT, 1},
        {Character::Status::IN_BATTLE, -1},
        {Character::Status::NIGHTHOWL, 5},
        {Character::Status::FPR_PLUS1, 5},
        {Character::Status::FPR_PLUS2, 5},
        {Character::Status::EXCLUDED, -1},
        {Character::Status::TACTICS, -1},
        {Character::Status::MESMERISED, -1},
        {Character::Status::INSTANT_DEATH, -1},
        {Character::Status::ENTANGLED, -1}};

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

    typedef std::vector<Character::Class> Classes;

    BloodSword::Mapping<Character::Class> ClassMapping = {
        {Character::Class::NONE, "NONE"},
        {Character::Class::WARRIOR, "WARRIOR"},
        {Character::Class::TRICKSTER, "TRICKSTER"},
        {Character::Class::SAGE, "SAGE"},
        {Character::Class::ENCHANTER, "ENCHANTER"}};

    Character::Classes All = {
        Character::Class::WARRIOR,
        Character::Class::TRICKSTER,
        Character::Class::SAGE,
        Character::Class::ENCHANTER};

    Character::Class Map(const char *character_class)
    {
        return BloodSword::Find(Character::ClassMapping, character_class);
    }

    Character::Class Map(std::string character_class)
    {
        return Character::Map(character_class.c_str());
    }
}

#endif
