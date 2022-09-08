#ifndef __CHARACTER_TYPES_HPP__
#define __CHARACTER_TYPES_HPP__

#include <map>
#include <string>

namespace BloodSword::Character
{
    enum class Class
    {
        NONE = -1,
        WARRIOR,
        TRICKSTER,
        SAGE,
        ENCHANTER
    };

    std::map<Character::Class, const char *> ClassMapping = {
        {Class::NONE, "NONE"},
        {Class::WARRIOR, "WARRIOR"},
        {Class::TRICKSTER, "TRICKSTER"},
        {Class::SAGE, "SAGE"},
        {Class::ENCHANTER, "ENCHANTER"}};

    Character::Class GetClass(const char *character)
    {
        auto result = Class::NONE;

        auto characterClass = Character::ClassMapping.begin();

        while (characterClass != Character::ClassMapping.end())
        {
            if (std::strcmp(characterClass->second, character) == 0)
            {
                result = characterClass->first;

                break;
            }

            characterClass++;
        }

        return result;
    }

    Character::Class GetClass(std::string character)
    {
        return Character::GetClass(character.c_str());
    }
}

#endif
