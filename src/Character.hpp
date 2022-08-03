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
        None = -1,
        Player,
        NPC
    };

    enum class Type
    {
        None = -1,
        Warrior,
        Trickster,
        Sage,
        Enchanter
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
        ControlType ControlType = ControlType::None;

        Character::Type Type = Character::Type::None;

        std::vector<Attribute::Base> Attributes = {};

        std::vector<Character::Status> Status = {};

        std::vector<Skills::Type> Skills = {};

        std::string Name = "";

        int Moves = 100;

        Base(const char *name, Character::Type type, std::vector<Attribute::Base> attributes, std::vector<Skills::Type> skills, int moves)
        {
            this->Name = name;

            this->Type = type;

            this->Attributes = attributes;

            this->Skills = skills;

            this->Moves = moves;
        }

        bool Has(Character::Status status)
        {
            return std::find(this->Status.begin(), this->Status.end(), status) != this->Status.end();
        }

        bool Has(Skills::Type skill)
        {
            return std::find(this->Skills.begin(), this->Skills.end(), skill) != this->Skills.end();
        }

        void Add(Character::Status status)
        {
            if (!Has(status))
            {
                this->Status.push_back(status);
            }
        }

        void Add(Skills::Type skill)
        {
            if (!Has(skill))
            {
                this->Skills.push_back(skill);
            }
        }

        int Value(Attribute::Type attribute)
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

        int Modifier(Attribute::Type attribute)
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

        int Maximum(Attribute::Type attribute)
        {
            auto result = 0;

            for (auto i = 0; i < this->Attributes.size(); i++)
            {
                if (this->Attributes[i].Type == attribute)
                {
                    result = this->Attributes[i].Maximum;

                    break;
                }
            }

            return result;
        }

        void Set(Attribute::Type attribute, int value, int modifier)
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

        void Set(Attribute::Type attribute, int value, int modifier, int maximum)
        {
            for (auto i = 0; i < this->Attributes.size(); i++)
            {
                if (this->Attributes[i].Type == attribute)
                {
                    this->Attributes[i].Value = value;

                    this->Attributes[i].Modifier = modifier;

                    this->Attributes[i].Maximum = maximum;

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
        std::vector<Character::Base> Members = {};

        Base(std::vector<Character::Base> members)
        {
            Members = members;
        }
    };
}
#endif