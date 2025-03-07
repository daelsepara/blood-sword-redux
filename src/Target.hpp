#ifndef __TARGET_HPP__
#define __TARGET_HPP__

#include <vector>

#include "nlohmann/json.hpp"
#include "Templates.hpp"

// for specific targetting
namespace BloodSword::Target
{
    enum class Type
    {
        NONE = -1,
        PLAYER,
        ENEMY,
        WARRIOR,
        TRICKSTER,
        SAGE,
        ENCHANTER,
        ASSASSIN,
        BARBARIAN,
        CORPSE
    };

    typedef std::vector<Target::Type> List;

    BloodSword::Mapping<Target::Type> Mapping = {
        {Target::Type::NONE, "NONE"},
        {Target::Type::PLAYER, "PLAYER"},
        {Target::Type::ENEMY, "ENEMY"},
        {Target::Type::WARRIOR, "WARRIOR"},
        {Target::Type::TRICKSTER, "TRICKSTER"},
        {Target::Type::SAGE, "SAGE"},
        {Target::Type::ENCHANTER, "ENCHANTER"},
        {Target::Type::ASSASSIN, "ASSASSIN"},
        {Target::Type::BARBARIAN, "BARBARIAN"},
        {Target::Type::CORPSE, "CORPSE"}};

    Target::Type Map(const char *target)
    {
        return BloodSword::Find(Target::Mapping, target);
    }

    Target::Type Map(std::string target)
    {
        return Target::Map(target.c_str());
    }

    Target::List Load(nlohmann::json &data)
    {
        auto targets = Target::List();

        for (auto i = 0; i < data.size(); i++)
        {
            auto target = !data[i].is_null() ? Target::Map(std::string(data[i])) : Target::Type::NONE;

            if (target != Target::Type::NONE)
            {
                targets.push_back(target);
            }
        }

        return targets;
    }

    nlohmann::json Data(Target::List &targets)
    {
        nlohmann::json data;

        for (auto &target : targets)
        {
            if (target != Target::Type::NONE)
            {
                data.push_back(Target::Mapping[target]);
            }
        }

        return data;
    }
}

#endif
