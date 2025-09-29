#ifndef __BATTLE_HPP__
#define __BATTLE_HPP__

#include "nlohmann/json.hpp"
#include "BattleResults.hpp"
#include "Engine.hpp"
#include "ZipFileLibrary.hpp"

// classes for describing battle (combatants, map, conditions)
namespace BloodSword::Battle
{
    // battle condition types
    enum class Condition
    {
        NONE = -1,
        NO_COMBAT,
        AMBUSH_PLAYER,
        AMBUSH_NPC,
        CANNOT_FLEE,
        SURVIVORS,
        AMBUSH_NPC_RANGED,
        AMBUSH_PLAYER_RANGED,
        WARRIOR_AWAY,
        TRICKSTER_AWAY,
        SAGE_AWAY,
        ENCHANTER_AWAY,
        HEAL_SURVIVORS,
        TACTICS,
        LAST_BATTLE,
        SKIP_LOCATION,
        ENTANGLED,
        REPLICATE,
        CLONE_WARRIOR,
        CLONE_TRICKSTER,
        CLONE_SAGE,
        CLONE_ENCHANTER,
        SLOW_MURDER,
        MISSION,
        EXCLUDE_OTHERS
    };

    // unlimited battle duration
    const int Unlimited = -1;

    // battle condition to string mapping
    BloodSword::ConstStrings<Battle::Condition> ConditionMapping = {
        {Battle::Condition::NONE, "NONE"},
        {Battle::Condition::NO_COMBAT, "NO COMBAT"},
        {Battle::Condition::AMBUSH_PLAYER, "AMBUSH PLAYER"},
        {Battle::Condition::AMBUSH_NPC, "AMBUSH NPC"},
        {Battle::Condition::CANNOT_FLEE, "CANNOT FLEE"},
        {Battle::Condition::SURVIVORS, "SURVIVORS"},
        {Battle::Condition::AMBUSH_NPC_RANGED, "AMBUSH NPC RANGED"},
        {Battle::Condition::AMBUSH_PLAYER_RANGED, "AMBUSH PLAYER RANGED"},
        {Battle::Condition::WARRIOR_AWAY, "WARRIOR AWAY"},
        {Battle::Condition::TRICKSTER_AWAY, "TRICKSTER AWAY"},
        {Battle::Condition::SAGE_AWAY, "SAGE AWAY"},
        {Battle::Condition::ENCHANTER_AWAY, "ENCHANTER AWAY"},
        {Battle::Condition::HEAL_SURVIVORS, "HEAL SURVIVORS"},
        {Battle::Condition::TACTICS, "TACTICS"},
        {Battle::Condition::LAST_BATTLE, "LAST BATTLE"},
        {Battle::Condition::SKIP_LOCATION, "SKIP LOCATION"},
        {Battle::Condition::ENTANGLED, "ENTANGLED"},
        {Battle::Condition::REPLICATE, "REPLICATE"},
        {Battle::Condition::CLONE_WARRIOR, "CLONE WARRIOR"},
        {Battle::Condition::CLONE_TRICKSTER, "CLONE TRICKSTER"},
        {Battle::Condition::CLONE_SAGE, "CLONE SAGE"},
        {Battle::Condition::CLONE_ENCHANTER, "CLONE ENCHANTER"},
        {Battle::Condition::SLOW_MURDER, "SLOW MURDER"},
        {Battle::Condition::MISSION, "MISSION"},
        {Battle::Condition::EXCLUDE_OTHERS, "EXCLUDE OTHERS"}};

    // map string to battle condition
    Battle::Condition MapCondition(const char *condition)
    {
        return BloodSword::Find(Battle::ConditionMapping, condition);
    }

    // map string to battle condition
    Battle::Condition MapCondition(std::string condition)
    {
        return Battle::MapCondition(condition.c_str());
    }

    // list of battle conditions
    typedef std::vector<Battle::Condition> Conditions;

    // list of all cloning battle conditions
    Battle::Conditions Cloning = {
        Battle::Condition::CLONE_WARRIOR,
        Battle::Condition::CLONE_TRICKSTER,
        Battle::Condition::CLONE_SAGE,
        Battle::Condition::CLONE_ENCHANTER};

    // fleeing damage
    struct FleeDamage
    {
        int Rounds = 0;

        int Value = 0;

        int Modifier = 0;

        bool IgnoreArmour = false;

        FleeDamage() {}

        FleeDamage(int rounds, int value, int modifier, bool ignore_armour) : Rounds(rounds), Value(value), Modifier(modifier), IgnoreArmour(false) {}
    };

    // battle base class
    class Base
    {
    public:
        // battle location
        Book::Location Location = Book::Undefined;

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
        Controls::Mapped<Book::Location> ActionCancels = {};

        // number of rounds for ambush
        int AmbushRounds = 0;

        // fleeing damage
        Battle::FleeDamage FleeDamage = {};

        // internal: target that is IN COMBAT status
        Character::Class InCombatTarget = Character::Class::NONE;

        // internal: battle order
        Engine::Queue Order = {};

        // internal: current combatant in battle order
        int Combatant = 0;

        // internal: round tracker
        int Round = 0;

        // internal: start next round
        bool NextRound = false;

        // internal: exit from battle
        bool ExitBattle = false;

        // internal: end current combatant's turn
        bool EndTurn = false;

        // internal: battle map
        Map::Base RoundMap;

        // internal: enemy combatants
        Party::Base RoundOpponents;

        // internal: party
        Party::Base RoundParty;

        Base(Battle::Conditions conditions, Map::Base &map, Party::Base &opponents, int duration) : Conditions(conditions), Map(map), Opponents(opponents), Duration(duration) {}

        Base() {}

        // check if battle has a specific condition
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

        // check if battle action can be cancelled
        bool Has(Controls::Type action)
        {
            auto actions = (SafeCast(this->ActionCancels.size()) > 0);

            auto cancel = BloodSword::Has(this->ActionCancels, action);

            return (actions && cancel && Book::IsDefined(this->ActionCancels[action]));
        }

        // check if battle is defined
        bool IsDefined()
        {
            return (this->Opponents.Count() > 0 || Book::IsDefined(this->Survivors) || this->Has(Battle::Condition::LAST_BATTLE)) && Map.IsValid();
        }

        // save round state
        void SaveRound(Party::Base &party)
        {
            this->RoundOpponents = this->Opponents;

            this->RoundMap = this->Map;

            this->RoundParty = party;
        }

        // initialize battle from data (and zip archive)
        void Initialize(nlohmann::json &data, const char *zip_file)
        {
            auto is_zip = (zip_file != nullptr);

            if (!data.is_null())
            {
                if (!data["location"].is_null())
                {
                    this->Location = Book::Load(data["location"]);
                }

                if (!data["conditions"].is_null() && data["conditions"].is_array() && SafeCast(data["conditions"].size()) > 0)
                {
                    auto conditions = Battle::Conditions();

                    for (auto i = 0; i < SafeCast(data["conditions"].size()); i++)
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

                this->AmbushRounds = !data["ambush_rounds"].is_null() ? int(data["ambush_rounds"]) : 0;

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

                        if (SafeCast(map.size()) > 0)
                        {
                            if (is_zip)
                            {
                                this->Map.Load(map.c_str(), zip_file);
                            }
                            else
                            {
                                this->Map.Load(map.c_str());
                            }
                        }
                    }
                }

                if (!data["opponents"].is_null() && data["opponents"].is_object() && SafeCast(data["opponents"].size()) > 0)
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

        // initialize battle from json data
        void Initialize(nlohmann::json &data)
        {
            this->Initialize(data, nullptr);
        }
    };

    // load battle from json data (and zip archive)
    void Load(Battle::Base &battle, nlohmann::json &data, const char *zip_file)
    {
        if (zip_file == nullptr)
        {
            battle.Initialize(data["battle"]);
        }
        else if (!data["battle"].is_null())
        {
            battle.Initialize(data["battle"], zip_file);
        }
    }

    // load battle from json data
    void Load(Battle::Base &battle, nlohmann::json &data)
    {
        Battle::Load(battle, data, nullptr);
    }

    // load battle from file
    Battle::Base Load(const char *filename)
    {
        Battle::Base battle;

        std::ifstream ifs(filename);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            Battle::Load(battle, data);

            ifs.close();
        }

        return battle;
    }

    // load battle from zip archive
    Battle::Base Load(const char *filename, const char *zip_file)
    {
        Battle::Base battle;

        if (zip_file == nullptr)
        {
            battle = Battle::Load(filename);
        }
        else
        {
            auto ifs = ZipFile::Read(zip_file, filename);

            if (!ifs.empty())
            {
                auto data = nlohmann::json::parse(ifs);

                Battle::Load(battle, data, zip_file);

                ifs.clear();
            }
        }

        return battle;
    }
}

#endif
