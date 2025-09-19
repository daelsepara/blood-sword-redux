#ifndef __ENGINE_LOGGER_HPP__
#define __ENGINE_LOGGER_HPP__

#include "../Target.hpp"

namespace BloodSword::EngineLogger
{
    // BloodSword Engine "Simple Intelligence Logger"
    void Log(const char *action, const char *attacker, const char *target, int dist, int path = -1, int vuln = -1, int prob = -1, int threshold = -1)
    {
#if defined(DEBUG)
        std::cerr << "[" << attacker << "]"
                  << " [" << action << "]"
                  << " [" << target << "]";

        if (path != -1)
        {
            std::cerr << " [PATH] " << std::to_string(path);
        }

        std::cerr << " [DIST] " << std::to_string(dist);

        if (vuln != -1)
        {
            std::cerr << " [VULN] " << std::to_string(vuln);
        }

        if (prob != -1 && threshold != -1)
        {
            std::cerr << " [PROB] "
                      << std::to_string(prob)
                      << " "
                      << (prob <= threshold ? "<=" : ">")
                      << " "
                      << std::to_string(threshold);
        }

        std::cerr << std::endl;
#endif
    }

    // log enemy action and target
    void Log(const char *action, Target::Type attacker, int id, Target::Type target, int target_id, int dist, int path = -1, int vuln = -1, int prob = -1, int threshold = -1)
    {
        auto action_string = std::string(action) + " TARGET " + std::to_string(target_id);

        auto attacker_string = Target::Mapping[attacker] + " " + std::to_string(id);

        auto target_string = Target::Mapping[target] + " " + std::to_string(target_id);

        EngineLogger::Log(action_string.c_str(), attacker_string.c_str(), target_string.c_str(), dist, path, vuln, prob, threshold);
    }

    // log enemy action and target
    void Log(const char *action, Target::Type target, int id, int target_id, int dist, int path = -1, int vuln = -1)
    {
        auto action_string = std::string(action) + " TARGET " + std::to_string(target_id);

        auto attacker_string = std::string("ENEMY ") + std::to_string(id);

        auto target_string = Target::Mapping[target] + " " + std::to_string(target_id);

        EngineLogger::Log(action_string.c_str(), attacker_string.c_str(), target_string.c_str(), dist, path, vuln);
    }
}

#endif
