#ifndef __ATTRIBUTE_HPP__
#define __ATTRIBUTE_HPP__

namespace BloodSword::Attribute
{
    enum class Type
    {
        None = -1,
        FightingProwess,
        Awareness,
        PsychicAbility,
        Endurance,
        Damage
    };

    class Base
    {
    public:
        Attribute::Type Type = Type::None;

        int Value = 0;

        int Modifier = 0;

        int Maximum = 0;

        Base(Attribute::Type type, int value, int modifier)
        {
            this->Type = type;

            this->Value = value;

            this->Maximum = value;

            this->Modifier = modifier;
        }
    };
}

#endif