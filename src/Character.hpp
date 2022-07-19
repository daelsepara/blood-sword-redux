#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <vector>

namespace BloodSword::Abilities
{
    enum class Type
    {
        None = -1,
        FightingProwess,
        Awareness,
        PsychicAbility,
        Endurance
    };

    class Base
    {
    public:
        BloodSword::Abilities::Type Type = BloodSword::Abilities::Type::None;

        int Value = 0;

        int Modifier = 0;

        Base(BloodSword::Abilities::Type type, int value, int modifier)
        {
            Type = type;

            Value = value;

            Modifier = modifier;
        }
    };
}

namespace BloodSword::Skills
{
    enum class Type
    {
        None = -1,
        Archery,
        Dodging,
        QuarterStaff,
        Spells
    };
}

namespace BloodSword::Character
{
    enum class ControlType
    {
        NONE = -1,
        PLAYER,
        NPC
    };

    enum class Type
    {
        NONE = -1,
        WARRIOR,
        TRICKSTER,
        SAGE,
        ENCHANTER
    };

    class Base
    {
    public:
        BloodSword::Character::ControlType ControlType = ControlType::NONE;

        BloodSword::Character::Type Type = Type::NONE;

        std::vector<BloodSword::Abilities::Base> Abilities = {};

        const char *Name = nullptr;
    };
}

namespace BloodSword::Party
{
    class Base
    {
    public:
        std::vector<BloodSword::Character::Base> Members = {};
    };
}
#endif