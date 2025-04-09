#ifndef __CHARACTER_STATUS_HPP__
#define __CHARACTER_STATUS_HPP__

#include "Templates.hpp"

namespace BloodSword::Character
{
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
        ENTANGLED,
        DOUBLE_HEALING,
        IN_COMBAT,
        POISONED,
        TASK,
        ALONE,
        AMBUSH_RANGED,
        SLOW_MURDER,
        SKIP_EVENTS,
        BURNED,
        DESOLATION,
        INVISIBLE,
        FIGHTING_BLIND
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
        {Character::Status::ENTANGLED, "ENTANGLED"},
        {Character::Status::DOUBLE_HEALING, "DOUBLE HEALING"},
        {Character::Status::IN_COMBAT, "IN COMBAT"},
        {Character::Status::POISONED, "POISONED"},
        {Character::Status::TASK, "TASK"},
        {Character::Status::ALONE, "ALONE"},
        {Character::Status::AMBUSH_RANGED, "AMBUSH RANGED"},
        {Character::Status::SLOW_MURDER, "SLOW MURDER"},
        {Character::Status::SKIP_EVENTS, "SKIP EVENTS"},
        {Character::Status::BURNED, "BURNED"},
        {Character::Status::DESOLATION, "DESOLATION"},
        {Character::Status::INVISIBLE, "INVISIBLE"},
        {Character::Status::FIGHTING_BLIND, "FIGHTING BLIND"}};

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
        {Character::Status::ENTANGLED, -1},
        {Character::Status::DOUBLE_HEALING, 1},
        {Character::Status::IN_COMBAT, 1},
        {Character::Status::POISONED, -1},
        {Character::Status::TASK, -1},
        {Character::Status::ALONE, -1},
        {Character::Status::SLOW_MURDER, -1},
        {Character::Status::SKIP_EVENTS, -1},
        {Character::Status::BURNED, -1},
        {Character::Status::DESOLATION, -1},
        {Character::Status::INVISIBLE, 1},
        {Character::Status::FIGHTING_BLIND, -1}};

    Character::Status MapStatus(const char *status)
    {
        return BloodSword::Find(Character::StatusMapping, status);
    }

    Character::Status MapStatus(std::string status)
    {
        return Character::MapStatus(status.c_str());
    }
}

#endif