#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <algorithm>
#include <string>
#include <vector>

#include "Asset.hpp"
#include "Attribute.hpp"
#include "Item.hpp"

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
        std::vector<Attribute::Base> Attributes = {};

        std::vector<Character::Status> Status = {};

        std::vector<Skills::Type> Skills = {};

        std::vector<Item::Base> Items = {};

        ControlType ControlType = ControlType::None;

        Character::Type Type = Character::Type::None;

        std::string Name = "";

        Asset::Type Asset = Asset::Type::NONE;

        int Moves = 100;

        int Rank = 0;

        Base(const char *name, Character::Type type, std::vector<Attribute::Base> attributes, std::vector<Skills::Type> skills, int moves)
        {
            this->Name = name;

            this->Type = type;

            this->Attributes = attributes;

            this->Skills = skills;

            this->Moves = moves;
        }

        bool Has(Skills::Type skill)
        {
            return std::find(this->Skills.begin(), this->Skills.end(), skill) != this->Skills.end();
        }

        void Add(Skills::Type skill)
        {
            if (!this->Has(skill))
            {
                this->Skills.push_back(skill);
            }
        }

        bool Has(Character::Status status)
        {
            return std::find(this->Status.begin(), this->Status.end(), status) != this->Status.end();
        }

        bool Is(Character::Status status)
        {
            return this->Has(status);
        }

        void Add(Character::Status status)
        {
            if (!this->Has(status))
            {
                this->Status.push_back(status);
            }
        }

        void Remove(Character::Status status)
        {
            if (this->Has(status))
            {
                for (auto i = 0; i < this->Status.size(); i++)
                {
                    if (this->Status[i] == status)
                    {
                        this->Status.erase(this->Status.begin() + i);

                        break;
                    }
                }
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

        void Value(Attribute::Type attribute, int value)
        {
            for (auto i = 0; i < this->Attributes.size(); i++)
            {
                if (this->Attributes[i].Type == attribute)
                {
                    this->Attributes[i].Value = value;

                    break;
                }
            }
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

        void Modifier(Attribute::Type attribute, int value)
        {
            for (auto i = 0; i < this->Attributes.size(); i++)
            {
                if (this->Attributes[i].Type == attribute)
                {
                    this->Attributes[i].Modifier = value;

                    break;
                }
            }
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

        void Maximum(Attribute::Type attribute, int value)
        {
            for (auto i = 0; i < this->Attributes.size(); i++)
            {
                if (this->Attributes[i].Type == attribute)
                {
                    this->Attributes[i].Maximum = value;

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

        // has item of specific type
        int Find(Item::Type item)
        {
            auto result = -1;

            for (auto i = 0; i < this->Items.size(); i++)
            {
                if (this->Items[i].Type == item)
                {
                    result = i;

                    break;
                }
            }

            return result;
        }

        bool Has(Item::Type item)
        {
            auto result = this->Find(item);

            return (result >= 0 && result < this->Items.size());
        }

        // has the container containing a sufficient amount of the item
        int Find(Item::Type container, Item::Type item, int quantity)
        {
            auto result = -1;

            for (auto i = 0; i < this->Items.size(); i++)
            {
                if (this->Items[i].Type == container && this->Items[i].Has(Item::Property::Container) && this->Items[i].Has(item, quantity))
                {
                    result = i;

                    break;
                }
            }

            return result;
        }

        bool Has(Item::Type container, Item::Type item, int quantity)
        {
            auto result = this->Find(container, item, quantity);

            return (result >= 0 && result < this->Items.size());
        }

        bool Has(Item::Type container, Item::Type item)
        {
            return this->Has(container, item, 1);
        }

        // has item with specific property
        int Find(Item::Type item, Item::Property property)
        {
            auto result = -1;

            for (auto i = 0; i < this->Items.size(); i++)
            {
                if (this->Items[i].Type == item && this->Items[i].Has(property))
                {
                    result = i;
                }
            }

            return result;
        }

        bool Has(Item::Type item, Item::Property property)
        {
            auto result = this->Find(item, property);

            return (result >= 0 && result < this->Items.size());
        }

        // has item with specific property
        int Find(Item::Type item, Item::Property property, Attribute::Type attribute)
        {
            auto result = -1;

            for (auto i = 0; i < this->Items.size(); i++)
            {
                if (this->Items[i].Type == item && this->Items[i].Has(property) && this->Items[i].Has(attribute))
                {
                    result = i;
                }
            }

            return result;
        }

        bool Has(Item::Type item, Item::Property property, Attribute::Type attribute)
        {
            auto result = this->Find(item, property, attribute);

            return (result >= 0 && result < this->Items.size());
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
