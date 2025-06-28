#ifndef __EVAL_LOGGER_HPP__
#define __EVAL_LOGGER_HPP__

#include <algorithm>

#include "../Book.hpp"
#include "../Item.hpp"
#include "../Party.hpp"
#include "../Templates.hpp"
#include "../InterfaceItemEffects.hpp"
#include "../Conditions.hpp"

namespace BloodSword::EvalLogger
{
    void Log(Conditions::Base &condition, bool result, bool failed, std::string text)
    {
#if defined(DEBUG)
        // debug info
        std::cerr << "[CONDITION] "
                  << std::string(Conditions::TypeMapping[condition.Type])
                  << " ([RESULT] "
                  << (result ? "TRUE" : "FALSE")
                  << ", [FAILED] "
                  << (failed ? "TRUE" : "FALSE");

        if (text.size() > 0)
        {
            std::cerr << ", [TEXT] " << text;
        }

        std::cerr << ")";

        if (condition.Invert)
        {
            std::cerr << " [INVERTED]";
        }

        std::cerr << std::endl;
#endif
    }
}

#endif
