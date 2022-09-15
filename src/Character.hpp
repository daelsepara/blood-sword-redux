#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <algorithm>
#include <string>
#include <vector>

#include "AssetTypes.hpp"
#include "Attribute.hpp"
#include "CharacterClasses.hpp"
#include "Item.hpp"
#include "Skills.hpp"
#include "Spells.hpp"
#include "Templates.hpp"

namespace BloodSword::Character
{
    class Base
    {
    public:
        IntMapping<Character::Status> Status = {};

        std::vector<Attribute::Base> Attributes = {};

        std::vector<Skills::Type> Skills = {};

        std::vector<Item::Base> Items = {};

        std::vector<Spells::Base> Spells = {};

        ControlType ControlType = ControlType::NONE;

        Character::Class Class = Character::Class::NONE;

        std::string Name = "";

        Asset::Type Asset = Asset::Type::NONE;

        int Experience = 0;

        int Moves = 100;

        int Rank = 0;

        int ItemLimit = 6;

        void _Intitialize(const char *name, Character::Class characterClass, std::vector<Attribute::Base> attributes, std::vector<Skills::Type> skills, int moves)
        {
            this->Name = name;

            this->Class = characterClass;

            this->Attributes = attributes;

            this->Skills = skills;

            this->Moves = moves;
        }

        Base(const char *name, Character::Class characterClass, std::vector<Attribute::Base> attributes, std::vector<Skills::Type> skills, int moves)
        {
            this->_Intitialize(name, characterClass, attributes, skills, moves);
        }

        Base(const char *name, Character::Class characterClass, std::vector<Attribute::Base> attributes, std::vector<Skills::Type> skills)
        {
            this->_Intitialize(name, characterClass, attributes, skills, 100);
        }

        Base(const char *name, Character::Class characterClass, std::vector<Attribute::Base> attributes)
        {
            this->_Intitialize(name, characterClass, attributes, {}, 100);
        }

        Base(const char *name, Character::Class characterClass)
        {
            this->_Intitialize(name, characterClass, {}, {}, 100);
        }

        Base(Character::Class characterClass, int rank)
        {
            this->_Intitialize(Character::ClassMapping[characterClass], characterClass, {}, {}, 100);

            this->Rank = rank;
        }

        Base(Character::Class characterClass)
        {
            this->_Intitialize(Character::ClassMapping[characterClass], characterClass, {}, {}, 100);
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
            auto hasStatus = this->Status.find(status) != this->Status.end();

            auto isActive = this->Status[status] != 0;

            return hasStatus && isActive;
        }

        bool Is(Character::Status status)
        {
            return this->Has(status);
        }

        void Add(Character::Status status, int duration)
        {
            this->Status[status] = duration;
        }

        void Add(Character::Status status)
        {
            this->Add(status, -1);
        }

        void Remove(Character::Status status)
        {
            if (this->Has(status))
            {
                this->Status.erase(status);
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

        // has a container with a sufficient amount of the item
        int Find(Item::Type container, Item::Type item, int quantity)
        {
            auto result = -1;

            for (auto i = 0; i < this->Items.size(); i++)
            {
                if (this->Items[i].Type == container && this->Items[i].Has(Item::Property::CONTAINER) && this->Items[i].Has(item, quantity))
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

        // has type of item with specific property
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

        // has type of item with specific property and attribute
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

        // has any item with specific property
        int Find(Item::Property property)
        {
            auto result = -1;

            for (auto i = 0; i < this->Items.size(); i++)
            {
                if (this->Items[i].Has(property))
                {
                    result = i;

                    break;
                }
            }

            return result;
        }

        bool Has(Item::Property property)
        {
            auto result = this->Find(property);

            return (result >= 0 && result < this->Items.size());
        }

        int Modifiers(Attribute::Type attribute)
        {
            auto modifiers = 0;

            for (auto i = 0; i < this->Items.size(); i++)
            {
                if (this->Items[i].Has(Item::Property::EQUIPPED) && this->Items[i].Has(attribute))
                {
                    modifiers += this->Items[i].Attributes[attribute];
                }
            }

            return modifiers;
        }
    };
}

#endif
