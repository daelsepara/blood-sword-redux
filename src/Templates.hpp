#ifndef __TEMPLATES_HPP__
#define __TEMPLATES_HPP__

#include <string>
#include <unordered_map>

namespace BloodSword
{
    // useful templates and classes

    // smart pointer
    template <typename T>
    using Smart = std::shared_ptr<T>;

    // array (vector of vectors)
    template <typename T>
    using Array = std::vector<std::vector<T>>;

    // maps object/type to a constant character string
    template <typename T>
    using Mapping = std::unordered_map<T, const char *>;

    // maps object/type to an integer
    template <typename T>
    using IntMapping = std::unordered_map<T, int>;

    // search for a constant string key in a map
    template <typename T>
    T Find(Mapping<T> &unordered_map, const char *key)
    {
        auto result = T::NONE;

        for (auto &keys : unordered_map)
        {
            if (std::strcmp(keys.second, key) == 0)
            {
                result = keys.first;

                break;
            }
        }

        return result;
    }

    // search for a key in a vector of type/objects
    template <typename T, typename R = typename std::vector<T>::const_iterator>
    R Find(std::vector<T> &vector, T &key)
    {
        return std::find(vector.begin(), vector.end(), key);
    }

    // search for a key in a vector (of type/objects) and use a custom compare function
    template <typename T, typename R = typename std::vector<T>::const_iterator>
    R Find(std::vector<T> &vector, T &key, bool F(T &, T &))
    {
        return std::find_if(vector.begin(), vector.end(), [&](T &f)
                            { return F(key, f); });
    }

    // check if the vector contains the key/object
    template <typename T>
    bool Found(std::vector<T> &vector, T &key)
    {
        return BloodSword::Find(vector, key) != vector.end();
    }
}

#endif
