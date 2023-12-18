#ifndef __BATTLE_HPP__
#define __BATTLE_HPP__

#include <vector>

#include "Map.hpp"
#include "Party.hpp"

// classes for describing battle (combatants, map, conditions)
namespace BloodSword::Battle
{
    enum class Condition
    {
        NONE = -1,
        AMBUSH_PLAYER,
        AMBUSH_NPC,
        CANNOT_FLEE,
        SURVIVORS
    };

    enum class Result
    {
        NONE = -1,
        DETERMINE,
        DEFEAT,
        VICTORY,
        FLEE,
        ENTHRALLED
    };

    const int Unlimited = -1;

    BloodSword::Mapping<Battle::Condition> ConditionMapping = {
        {Battle::Condition::AMBUSH_PLAYER, "AMBUSH PLAYER"},
        {Battle::Condition::AMBUSH_NPC, "AMBUSH NPC"},
        {Battle::Condition::CANNOT_FLEE, "CANNOT FLEE"},
        {Battle::Condition::SURVIVORS, "SURVIVORS"}};

    Battle::Condition MapCondition(const char *condition)
    {
        return BloodSword::Find(Battle::ConditionMapping, condition);
    }

    Battle::Condition MapCondition(std::string condition)
    {
        return BloodSword::Find(Battle::ConditionMapping, condition.c_str());
    }

    class Base
    {
    public:
        // battle conditions (see Battle::Condition types)
        std::vector<Battle::Condition> Conditions = {};

        // battle map
        Map::Base Map;

        // enemy combatants
        Party::Base Opponents;

        // round limit (-1 if unlimited)
        int Duration = Battle::Unlimited;

        Base(std::vector<Battle::Condition> conditions, Map::Base &map, Party::Base &opponents, int duration) : Conditions(conditions), Map(map), Opponents(opponents), Duration(duration) {}

        Base() {}

        bool Is(Battle::Condition condition)
        {
            auto result = false;

            for (auto &check : this->Conditions)
            {
                if (check == condition)
                {
                    result = true;

                    break;
                }
            }

            return result;
        }

        bool IsDefined()
        {
            return this->Opponents.Count() > 0 && Map.IsValid();
        }

        // initialize battle from data
        void Initialize(nlohmann::json data)
        {
            if (!data["battle"].is_null())
            {
                if (!data["battle"]["conditions"].is_null() && data["battle"]["conditions"].is_array() && data["battle"]["conditions"].size() > 0)
                {
                    auto conditions = std::vector<Battle::Condition>();

                    for (auto i = 0; i < data["battle"]["conditions"].size(); i++)
                    {
                        auto condition = !data["battle"]["conditions"][i].is_null() ? Battle::MapCondition(std::string(data["battle"]["conditions"][i])) : Battle::Condition::NONE;

                        conditions.push_back(condition);
                    }

                    this->Conditions = conditions;
                }

                this->Duration = !data["battle"]["duration"].is_null() ? (int)data["battle"]["duration"] : -1;

                // initialize map from file
                if (!data["battle"]["map"].is_null())
                {
                    auto map = std::string(data["battle"]["map"]);

                    if (!map.empty())
                    {
                        this->Map.Load(map.c_str());
                    }
                }

                // TODO: load opponents
                if (!data["battle"]["opponents"].is_null())
                {
                }
            }
        }
    };
}

#endif
