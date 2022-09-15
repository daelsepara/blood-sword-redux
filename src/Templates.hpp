#ifndef __TEMPLATES_HPP__
#define __TEMPLATES_HPP__

#include <string>
#include <unordered_map>

namespace BloodSword
{
    template <typename T>
    using Mapping = std::unordered_map<T, const char *>;

    template <typename T>
    using IntMapping = std::unordered_map<T, int>;

    template <typename T>
    T Find(Mapping<T> &unordered_map, const char *key)
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
