#ifndef __VERSION_HPP__
#define __VERSION_HPP__

#include <sstream>
#include <string>

// version information
namespace BloodSword
{
    // version numbers
    const int MajorVersion = 0;

    const int MinorVersion = 2;

    const int BuildNumber = 72;

    // version string
    std::string Version()
    {
        std::stringstream version;

        version << "v"
                << std::to_string(MajorVersion)
                << "."
                << std::to_string(MinorVersion)
                << "."
                << std::to_string(BuildNumber);

        return version.str();
    }
}

#endif
