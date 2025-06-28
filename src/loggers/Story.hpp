#ifndef __STORY_LOGGER_HPP__
#define __STORY_LOGGER_HPP__

#include "../InterfaceBattle.hpp"
#include "../ConditionsEvaluate.hpp"

namespace BloodSword::StoryLogger
{
    // log major story events
    void LogSectionHeader(const char *header, Book::Location location, bool newline = true)
    {
#if defined(DEBUG)
        std::cerr << "["
                  << header
                  << "] "
                  << Book::String(location);

        if (newline)
        {
            std::cerr << std::endl;
        }
#endif
    }

    // log story search results
    void LogSearch(Book::Location location, int current)
    {
#if defined(DEBUG)
        StoryLogger::LogSectionHeader("FIND", location, false);

        std::cerr << " == " << (current != -1 ? "FOUND" : "NOT FOUND") << std::endl;
#endif
    }
}

#endif
