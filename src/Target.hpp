#ifndef __TARGET_HPP__
#define __TARGET_HPP__

#include "nlohmann/json.hpp"
#include "Templates.hpp"

// for specific targetting
namespace BloodSword::Target
{
    // target not found
    const int NotFound = -1;

    // target types
    enum class Type
    {
        NONE = -1,
        ANY,
        PLAYER,
        ENEMY,
        WARRIOR,
        TRICKSTER,
        SAGE,
        ENCHANTER,
        ASSASSIN,
        BARBARIAN,
        CORPSE,
        GUARD,
        NIGHT_ELF,
        MAGUS_VYL,
        MAN_IN_BLUE,
        GIANT_SPIDER,
        GIANT,
        NEBULARON,
        EIDOLON
    };

    // list of target types
    typedef std::vector<Target::Type> List;

    // spell type mapping template
    template <typename T>
    using Mapped = BloodSword::UnorderedMap<Target::Type, T>;

    // mapping of target types to strings
    BloodSword::StringMap<Target::Type> Mapping = {
        {Target::Type::NONE, "NONE"},
        {Target::Type::PLAYER, "PLAYER"},
        {Target::Type::ENEMY, "ENEMY"},
        {Target::Type::WARRIOR, "WARRIOR"},
        {Target::Type::TRICKSTER, "TRICKSTER"},
        {Target::Type::SAGE, "SAGE"},
        {Target::Type::ENCHANTER, "ENCHANTER"},
        {Target::Type::ASSASSIN, "ASSASSIN"},
        {Target::Type::BARBARIAN, "BARBARIAN"},
        {Target::Type::CORPSE, "CORPSE"},
        {Target::Type::GUARD, "GUARD"},
        {Target::Type::NIGHT_ELF, "NIGHT ELF"},
        {Target::Type::MAGUS_VYL, "MAGUS VYL"},
        {Target::Type::MAN_IN_BLUE, "MAN IN BLUE"},
        {Target::Type::GIANT_SPIDER, "GIANT SPIDER"},
        {Target::Type::GIANT, "GIANT"},
        {Target::Type::NEBULARON, "NEBULARON"},
        {Target::Type::EIDOLON, "EIDOLON"}};

    // map string to target type
    Target::Type Map(std::string target)
    {
        return BloodSword::Find(Target::Mapping, target);
    }

    // map string to target type
    Target::Type Map(const char *target)
    {
        return Target::Map(std::string(target));
    }

    // load list of target types from json data
    Target::List Load(nlohmann::json &data)
    {
        auto targets = Target::List();

        for (auto i = 0; i < SafeCast(data.size()); i++)
        {
            auto target = !data[i].is_null() ? Target::Map(std::string(data[i])) : Target::Type::NONE;

            if (target != Target::Type::NONE)
            {
                targets.push_back(target);
            }
        }

        return targets;
    }

    // generate json data from list of target types
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
