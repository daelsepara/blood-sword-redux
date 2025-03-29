#ifndef __BATTLE_RESULTS_HPP__
#define __BATTLE_RESULTS_HPP__

#include "Templates.hpp"

namespace BloodSword::Battle
{
    enum class Result
    {
        NONE = -1,
        DETERMINE,
        DEFEAT,
        VICTORY,
        FLEE,
        ENTHRALLED
    };

    BloodSword::Mapping<Battle::Result> ResultMapping = {
        {Battle::Result::NONE, "NONE"},
        {Battle::Result::DETERMINE, "DETERMINE"},
        {Battle::Result::DEFEAT, "DEFEAT"},
        {Battle::Result::VICTORY, "VICTORY"},
        {Battle::Result::FLEE, "FLEE"},
        {Battle::Result::ENTHRALLED, "ENTHRALLED"}};

    Battle::Result MapResult(const char *result)
    {
        return BloodSword::Find(Battle::ResultMapping, result);
    }

    Battle::Result MapResult(std::string result)
    {
        return Battle::MapResult(result.c_str());
    }
}

#endif
