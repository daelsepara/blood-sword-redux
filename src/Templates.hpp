#ifndef __TEMPLATES_HPP__
#define __TEMPLATES_HPP__

#include <map>
#include <string>

namespace BloodSword
{
    template <typename T>
    T Find(std::map<T, const char *> &map, const char *key)
    {
        auto result = T::NONE;

        auto keys = map.begin();

        while (keys != map.end())
        {
            if (std::strcmp(keys->second, key) == 0)
            {
                result = keys->first;

                break;
            }

            keys++;
        }

        return result;
    }
}

#endif
