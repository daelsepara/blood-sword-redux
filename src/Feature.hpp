#ifndef __FEATURE_HPP__
#define __FEATURE_HPP__

#include "nlohmann/json.hpp"
#include "Templates.hpp"

namespace BloodSword::Feature
{
    enum class Type
    {
        NONE = -1,
        ENDING,
        BAD_ENDING,
        ITEM_EFFECT,
        DOUBLE_HEALING,
        TASK,
        TASK_WARRIOR,
        TASK_TRICKSTER,
        TASK_SAGE,
        TASK_ENCHANTER,
        ALONE
    };

    BloodSword::Mapping<Feature::Type> TypeMapping = {
        {Feature::Type::NONE, "NONE"},
        {Feature::Type::ENDING, "ENDING"},
        {Feature::Type::BAD_ENDING, "BAD ENDING"},
        {Feature::Type::ITEM_EFFECT, "ITEM EFFECT"},
        {Feature::Type::DOUBLE_HEALING, "DOUBLE HEALING"},
        {Feature::Type::TASK, "TASK"},
        {Feature::Type::TASK_WARRIOR, "TASK WARRIOR"},
        {Feature::Type::TASK_TRICKSTER, "TASK TRICKSTER"},
        {Feature::Type::TASK_SAGE, "TASK SAGE"},
        {Feature::Type::TASK_ENCHANTER, "TASK ENCHANTER"},
        {Feature::Type::ALONE, "ALONE"}};

    Feature::Type Map(const char *feature)
    {
        return BloodSword::Find(Feature::TypeMapping, feature);
    }

    Feature::Type Map(std::string feature)
    {
        return Feature::Map(feature.c_str());
    }
}

namespace BloodSword::Features
{
    typedef std::vector<Feature::Type> List;

    Features::List Load(nlohmann::json &data)
    {
        auto features = Features::List();

        for (auto i = 0; i < data.size(); i++)
        {
            auto feature = !data[i].is_null() ? Feature::Map(std::string(data[i])) : Feature::Type::NONE;

            features.push_back(feature);
        }

        return features;
    }

    nlohmann::json Data(Features::List &features)
    {
        nlohmann::json data;

        for (auto &feature : features)
        {
            if (feature != Feature::Type::NONE)
            {
                data.push_back(Feature::TypeMapping[feature]);
            }
        }

        return data;
    }
}

#endif
