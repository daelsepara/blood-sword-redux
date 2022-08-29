#ifndef __CHARACTER_TYPES_HPP__
#define __CHARACTER_TYPES_HPP__

#include <map>
#include <string>

namespace BloodSword::Character
{
    enum class Type
    {
        None = -1,
        Warrior,
        Trickster,
        Sage,
        Enchanter
    };

    std::map<Character::Type, const char *> CharacterMapping = {
        {Character::Type::None, "NONE"},
        {Character::Type::Warrior, "WARRIOR"},
        {Character::Type::Trickster, "TRICKSTER"},
        {Character::Type::Sage, "SAGE"},
        {Character::Type::Enchanter, "ENCHANTER"}};

    Character::Type GetType(const char *character)
    {
        auto result = Character::Type::None;

        auto characters = Character::CharacterMapping.begin();

        while (characters != Character::CharacterMapping.end())
        {
            if (std::strcmp(characters->second, character) == 0)
            {
                result = characters->first;

                break;
            }

            characters++;
        }

        return result;
    }

    Character::Type GetType(std::string character)
    {
        return Character::GetType(character.c_str());
    }
}

#endif
