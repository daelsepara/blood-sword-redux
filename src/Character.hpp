#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <algorithm>
#include <string>
#include <vector>

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
        BloodSword::Attribute::Type Type = BloodSword::Attribute::Type::None;

        int Value = 0;

        int Modifier = 0;

        Base(BloodSword::Attribute::Type type, int value, int modifier)
        {
            this->Type = type;

            this->Value = value;

            this->Modifier = modifier;
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
        Healing,
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

    enum class Status
    {
        Defending,
        Fleeing,
        Enthralled
    };

    class Base
    {
    public:
        BloodSword::Character::ControlType ControlType = ControlType::NONE;

        BloodSword::Character::Type Type = Type::NONE;

        std::vector<BloodSword::Attribute::Base> Attributes = {};

        std::vector<BloodSword::Character::Status> Status = {};

        std::string Name = "";

        Base(const char *name, BloodSword::Character::Type type, std::vector<BloodSword::Attribute::Base> attributes)
        {
            this->Name = name;

            this->Type = type;

            this->Attributes = attributes;
        }

        bool Has(BloodSword::Character::Status status)
        {
            return std::find(this->Status.begin(), this->Status.end(), status) != this->Status.end();
        }

        int Value(BloodSword::Attribute::Type attribute)
        {
            auto result = 0;

            for (auto i = 0; i < this->Attributes.size(); i++)
            {
                if (this->Attributes[i].Type == attribute)
                {
                    result = this->Attributes[i].Value;

                    break;
                }
            }

            return result;
        }

        int Modifier(BloodSword::Attribute::Type attribute)
        {
            auto result = 0;

            for (auto i = 0; i < this->Attributes.size(); i++)
            {
                if (this->Attributes[i].Type == attribute)
                {
                    result = this->Attributes[i].Modifier;

                    break;
                }
            }

            return result;
        }

        void Set(BloodSword::Attribute::Type attribute, int value, int modifier)
        {
            for (auto i = 0; i < this->Attributes.size(); i++)
            {
                if (this->Attributes[i].Type == attribute)
                {
                    this->Attributes[i].Value = value;

                    this->Attributes[i].Modifier = modifier;

                    break;
                }
            }
        }
    };
}

namespace BloodSword::Party
{
    class Base
    {
    public:
        std::vector<BloodSword::Character::Base> Members = {};

        Base(std::vector<BloodSword::Character::Base> members)
        {
            Members = members;
        }
    };
}
#endif