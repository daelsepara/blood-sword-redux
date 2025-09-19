#ifndef __CHARACTER_TYPES_HPP__
#define __CHARACTER_TYPES_HPP__

#include "nlohmann/json.hpp"
#include "Templates.hpp"

namespace BloodSword::Character
{
    // character control type
    enum class ControlType
    {
        NONE = -1,
        PLAYER,
        NPC
    };

    // control type to string mapping
    BloodSword::ConstStrings<Character::ControlType> ControlTypeMapping = {
        {Character::ControlType::NONE, "NONE"},
        {Character::ControlType::PLAYER, "PLAYER"},
        {Character::ControlType::NPC, "NPC"}};

    // map string to control type
    Character::ControlType MapControlType(const char *control_type)
    {
        return BloodSword::Find(Character::ControlTypeMapping, control_type);
    }

    // map string to control type
    Character::ControlType MapControlType(std::string control_type)
    {
        return Character::MapControlType(control_type.c_str());
    }

    // character classes (player and NPC)
    enum class Class
    {
        NONE = -1,
        WARRIOR,
        TRICKSTER,
        SAGE,
        ENCHANTER,
        IMRAGARN
    };

    // list of character classes
    typedef std::vector<Character::Class> Classes;

    // character class to string mapping
    BloodSword::StringMap<Character::Class> ClassMapping = {
        {Character::Class::NONE, "NONE"},
        {Character::Class::WARRIOR, "WARRIOR"},
        {Character::Class::TRICKSTER, "TRICKSTER"},
        {Character::Class::SAGE, "SAGE"},
        {Character::Class::ENCHANTER, "ENCHANTER"},
        {Character::Class::IMRAGARN, "IMRAGARN"}};

    // all normal character classes
    Character::Classes All = {
        Character::Class::WARRIOR,
        Character::Class::TRICKSTER,
        Character::Class::SAGE,
        Character::Class::ENCHANTER};

    // map string to character class
    Character::Class Map(const char *character_class)
    {
        return BloodSword::Find(Character::ClassMapping, character_class);
    }

    // map string to character class
    Character::Class Map(std::string character_class)
    {
        return Character::Map(character_class.c_str());
    }

    // check if character class is not a normal class
    bool OtherClass(Character::Class character)
    {
        auto found = true;

        for (auto normal : Character::All)
        {
            if (normal == character)
            {
                found = false;

                break;
            }
        }

        return found;
    }
}

#endif
