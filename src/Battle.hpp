#ifndef __BATTLE_HPP__
#define __BATTLE_HPP__

#include <vector>

#include "nlohmann/json.hpp"
#include "BattleResults.hpp"
#include "Map.hpp"
#include "Party.hpp"

// classes for describing battle (combatants, map, conditions)
namespace BloodSword::Battle
{
    enum class Condition
    {
        NONE = -1,
        NO_COMBAT,
        AMBUSH_PLAYER,
        AMBUSH_NPC,
        CANNOT_FLEE,
        SURVIVORS,
        AMBUSH_RANGED,
        WARRIOR_AWAY,
        TRICKSTER_AWAY,
        SAGE_AWAY,
        ENCHANTER_AWAY,
        HEAL_SURVIVORS,
        TACTICS,
        LAST_BATTLE,
        SKIP_LOCATION,
        ENTANGLED,
        REPLICATE
    };

    const int Unlimited = -1;

    BloodSword::Mapping<Battle::Condition> ConditionMapping = {
        {Battle::Condition::NONE, "NONE"},
        {Battle::Condition::NO_COMBAT, "NO COMBAT"},
        {Battle::Condition::AMBUSH_PLAYER, "AMBUSH PLAYER"},
        {Battle::Condition::AMBUSH_NPC, "AMBUSH NPC"},
        {Battle::Condition::CANNOT_FLEE, "CANNOT FLEE"},
        {Battle::Condition::SURVIVORS, "SURVIVORS"},
        {Battle::Condition::AMBUSH_RANGED, "AMBUSH RANGED"},
        {Battle::Condition::WARRIOR_AWAY, "WARRIOR AWAY"},
        {Battle::Condition::TRICKSTER_AWAY, "TRICKSTER AWAY"},
        {Battle::Condition::SAGE_AWAY, "SAGE AWAY"},
        {Battle::Condition::ENCHANTER_AWAY, "ENCHANTER AWAY"},
        {Battle::Condition::HEAL_SURVIVORS, "HEAL SURVIVORS"},
        {Battle::Condition::TACTICS, "TACTICS"},
        {Battle::Condition::LAST_BATTLE, "LAST BATTLE"},
        {Battle::Condition::SKIP_LOCATION, "SKIP LOCATION"},
        {Battle::Condition::ENTANGLED, "ENTANGLED"},
        {Battle::Condition::REPLICATE, "REPLICATE"}};

    Battle::Condition MapCondition(const char *condition)
    {
        return BloodSword::Find(Battle::ConditionMapping, condition);
    }

    Battle::Condition MapCondition(std::string condition)
    {
        return BloodSword::Find(Battle::ConditionMapping, condition.c_str());
    }

    typedef std::vector<Battle::Condition> Conditions;

    struct FleeDamage
    {
        int Rounds = 0;

        int Value = 0;

        int Modifier = 0;

        bool IgnoreArmour = false;

        FleeDamage() {}

        FleeDamage(int rounds, int value, int modifier, bool ignore_armour) : Rounds(rounds), Value(value), Modifier(modifier), IgnoreArmour(false) {}
    };

    class Base
    {
    public:
        // battle conditions (see Battle::Condition types)
        Battle::Conditions Conditions = {};

        // battle map
        Map::Base Map;

        // enemy combatants
        Party::Base Opponents;

        // round limit (-1 if unlimited)
        int Duration = Battle::Unlimited;

        // endurance limit (-1 if fighting to death)
        int Endurance = -1;

        // surviving opponents start at this round (0 - start immediately)
        int SurvivorStart = -1;

        // limit of survivors carried over into the next battle (0 - no one survives)
        int SurvivorLimit = 0;

        Book::Location Survivors = Book::Undefined;

        // max spell casters
        int MaxCasters = Battle::Unlimited;

        // internal: spells already cast this round
        Spells::List AlreadyCast = {};

        // internal: allowed casters this round
        std::vector<int> Casters = {};

        // internal: dropped items during combat
        Items::Inventory Loot = {};

        // message to display when cancelling a battle action
        BloodSword::UnorderedMap<Controls::Type, Book::Location> ActionCancels = {};

        // fleeing damage
        Battle::FleeDamage FleeDamage = {};

        // internal: target that is IN COMBAT status
        Character::Class InCombatTarget = Character::Class::NONE;

        Base(Battle::Conditions conditions, Map::Base &map, Party::Base &opponents, int duration) : Conditions(conditions), Map(map), Opponents(opponents), Duration(duration) {}

        Base() {}

        bool Has(Battle::Condition condition)
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

        bool Has(Controls::Type action)
        {
            auto actions = (this->ActionCancels.size() > 0);

            auto cancel = (this->ActionCancels.find(action) != this->ActionCancels.end());

            return (actions && cancel && Book::IsDefined(this->ActionCancels[action]));
        }

        bool IsDefined()
        {
            return (this->Opponents.Count() > 0 || Book::IsDefined(this->Survivors) || this->Has(Battle::Condition::LAST_BATTLE)) && Map.IsValid();
        }

        // initialize battle from data
        void Initialize(nlohmann::json &data)
        {
            if (!data.is_null())
            {
                if (!data["conditions"].is_null() && data["conditions"].is_array() && data["conditions"].size() > 0)
                {
                    auto conditions = Battle::Conditions();

                    for (auto i = 0; i < data["conditions"].size(); i++)
                    {
                        auto condition = !data["conditions"][i].is_null() ? Battle::MapCondition(std::string(data["conditions"][i])) : Battle::Condition::NONE;

                        if (condition != Battle::Condition::NONE)
                        {
                            conditions.push_back(condition);
                        }
                    }

                    this->Conditions = conditions;
                }

                this->Duration = !data["duration"].is_null() ? int(data["duration"]) : Battle::Unlimited;

                this->Endurance = !data["endurance"].is_null() ? int(data["endurance"]) : Battle::Unlimited;

                // initialize map from file
                if (!data["map"].is_null())
                {
                    if (data["map"].is_object())
                    {
                        this->Map.Setup(data["map"]);
                    }
                    else
                    {
                        auto map = std::string(data["map"]);

                        if (map.size() > 0)
                        {
                            this->Map.Load(map.c_str());
                        }
                    }
                }

                if (!data["opponents"].is_null() && data["opponents"].is_object() && data["opponents"].size() > 0)
                {
                    this->Opponents = Party::Initialize(data["opponents"]);
                }

                // get survivors from previous battles
                if (!data["survivors"].is_null())
                {
                    this->Survivors = Book::Load(data["survivors"]);
                }

                this->SurvivorStart = !data["survivor_start"].is_null() ? int(data["survivor_start"]) : Battle::Unlimited;

                this->SurvivorLimit = !data["survivor_limit"].is_null() ? int(data["survivor_limit"]) : 0;

                this->MaxCasters = !data["max_casters"].is_null() ? int(data["max_casters"]) : Battle::Unlimited;

                if (!data["action_cancels"].is_null() && data["action_cancels"].is_object())
                {
                    this->ActionCancels.clear();

                    for (auto &[key, val] : data["action_cancels"].items())
                    {
                        auto action = Controls::MapType(key);

                        auto cancel = !val.is_null() ? Book::Load(val) : Book::Undefined;

                        if (action != Controls::Type::NONE && Book::IsDefined(cancel))
                        {
                            this->ActionCancels[action] = cancel;
                        }
                    }
                }

                if (!data["flee_damage"].is_null() && data["flee_damage"].is_object())
                {
                    auto rounds = !data["flee_damage"]["rounds"].is_null() ? int(data["flee_damage"]["rounds"]) : 0;

                    auto value = !data["flee_damage"]["value"].is_null() ? int(data["flee_damage"]["value"]) : 0;

                    auto modifier = !data["flee_damage"]["modifier"].is_null() ? int(data["flee_damage"]["modifier"]) : 0;

                    auto ignore_armour = (!data["flee_damage"]["ignore_armour"].is_null() && data["flee_damage"]["ignore_armour"].is_boolean()) ? data["flee_damage"]["ignore_armour"].get<bool>() : false;

                    this->FleeDamage = Battle::FleeDamage(rounds, value, modifier, ignore_armour);
                }
            }
        }
    };

    Battle::Base Load(const char *filename)
    {
        Battle::Base battle;

        std::ifstream file(filename);

        if (file.good())
        {
            auto data = nlohmann::json::parse(file);

            if (!data["battle"].is_null())
            {
                battle.Initialize(data["battle"]);
            }

            file.close();
        }

        return battle;
    }
}

#endif
