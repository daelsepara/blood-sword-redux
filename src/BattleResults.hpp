#ifndef __BATTLE_RESULTS_HPP__
#define __BATTLE_RESULTS_HPP__

#include "Templates.hpp"

namespace BloodSword::Battle
{
    // battle result types
    enum class Result
    {
        NONE = -1,
        DETERMINE,
        DEFEAT,
        VICTORY,
        FLEE,
        ENTHRALLED
    };

    // battle result to string mapping
    BloodSword::StringMap<Battle::Result> ResultMapping = {
        {Battle::Result::NONE, "NONE"},
        {Battle::Result::DETERMINE, "DETERMINE"},
        {Battle::Result::DEFEAT, "DEFEAT"},
        {Battle::Result::VICTORY, "VICTORY"},
        {Battle::Result::FLEE, "FLEE"},
        {Battle::Result::ENTHRALLED, "ENTHRALLED"}};

    // map string to battle result
    Battle::Result MapResult(std::string result)
    {
        return BloodSword::Find(Battle::ResultMapping, result);
    }

    // map string to battle result
    Battle::Result MapResult(const char *result)
    {
        return Battle::MapResult(std::string(result));
    }
}

#endif
