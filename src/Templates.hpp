#ifndef __TEMPLATES_HPP__
#define __TEMPLATES_HPP__

#include <unordered_map>
#include <string>

namespace BloodSword
{
    template <typename T>
    T Find(std::unordered_map<T, const char *> &unordered_map, const char *key)
    {
        auto result = T::NONE;

        auto keys = unordered_map.begin();

        while (keys != unordered_map.end())
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
