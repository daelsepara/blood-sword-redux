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

        std::string Name;

        Asset::Type Asset = Asset::Type::NONE;

        int Experience = 0;

        int Moves = 100;

        int Rank = 0;

        int ItemLimit = 6;

        void Initialize(const char *name, Character::Class characterClass, std::vector<Attribute::Base> attributes, std::vector<Skills::Type> skills, int moves)
        {
            this->Name = name;

            this->Class = characterClass;

            this->Attributes = attributes;

            this->Skills = skills;

            this->Moves = moves;
        }

        Base(const char *name, Character::Class characterClass, std::vector<Attribute::Base> attributes, std::vector<Skills::Type> skills, int moves)
        {
            this->Initialize(name, characterClass, attributes, skills, moves);
        }

        Base(const char *name, Character::Class characterClass, std::vector<Attribute::Base> attributes, std::vector<Skills::Type> skills)
        {
            this->Initialize(name, characterClass, attributes, skills, 100);
        }

        Base(const char *name, Character::Class characterClass, std::vector<Attribute::Base> attributes)
        {
            this->Initialize(name, characterClass, attributes, {}, 100);
        }

        Base(const char *name, Character::Class characterClass)
        {
            this->Initialize(name, characterClass, {}, {}, 100);
        }

        Base(Character::Class characterClass, int rank)
        {
            this->Initialize(Character::ClassMapping[characterClass], characterClass, {}, {}, 100);

            this->Rank = rank;
        }

        Base(Character::Class characterClass)
        {
            this->Initialize(Character::ClassMapping[characterClass], characterClass, {}, {}, 100);
        }

        bool Has(Skills::Type skill)
        {
            return BloodSword::Found(this->Skills, skill);
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

        int Value(Attribute::Type type)
        {
            auto result = 0;

            for (auto &attribute : this->Attributes)
            {
                if (attribute.Type == type)
                {
                    result = attribute.Value;

                    break;
                }
            }

            return result;
        }

        void Value(Attribute::Type type, int value)
        {
            for (auto &attribute : this->Attributes)
            {
                if (attribute.Type == type)
                {
                    attribute.Value = value;

                    break;
                }
            }
        }

        int Modifier(Attribute::Type type)
        {
            auto result = 0;

            for (auto &attribute : this->Attributes)
            {
                if (attribute.Type == type)
                {
                    result = attribute.Modifier;

                    break;
                }
            }

            return result;
        }

        void Modifier(Attribute::Type type, int value)
        {
            for (auto &attribute : this->Attributes)
            {
                if (attribute.Type == type)
                {
                    attribute.Modifier = value;

                    break;
                }
            }
        }

        int Maximum(Attribute::Type type)
        {
            auto result = 0;

            for (auto &attribute : this->Attributes)
            {
                if (attribute.Type == type)
                {
                    result = attribute.Maximum;

                    break;
                }
            }

            return result;
        }

        void Maximum(Attribute::Type type, int value)
        {
            for (auto &attribute : this->Attributes)
            {
                if (attribute.Type == type)
                {
                    attribute.Maximum = value;

                    break;
                }
            }
        }

        void Set(Attribute::Type type, int value, int modifier, int maximum)
        {
            for (auto &attribute : this->Attributes)
            {
                if (attribute.Type == type)
                {
                    attribute.Value = value;

                    attribute.Modifier = modifier;

                    attribute.Maximum = maximum;

                    break;
                }
            }
        }

        void Set(Attribute::Type type, int value, int modifier)
        {
            for (auto &attribute : this->Attributes)
            {
                if (attribute.Type == type)
                {
                    attribute.Value = value;

                    attribute.Modifier = modifier;

                    break;
                }
            }
        }

        // has item of specific type
        std::vector<Item::Base>::const_iterator Find(Item::Type type)
        {
            auto result = this->Items.end();

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if ((*item).Type == type)
                {
                    return item;
                }
            }

            return result;
        }

        bool Has(Item::Type item)
        {
            auto result = this->Find(item);

            return result != this->Items.end();
        }

        // has a container with a sufficient amount of the item
        std::vector<Item::Base>::const_iterator Find(Item::Type container, Item::Type type, int quantity)
        {
            auto result = this->Items.end();

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if ((*item).Type == container && (*item).Has(Item::Property::CONTAINER) && (*item).Has(type, quantity))
                {
                    result = item;

                    break;
                }
            }

            return result;
        }

        std::vector<Item::Base>::const_iterator Find(Item::Type container, Item::Type type)
        {
            auto result = this->Items.end();

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if ((*item).Type == container && (*item).Has(Item::Property::CONTAINER) && (*item).Contains == type)
                {
                    result = item;

                    break;
                }
            }

            return result;
        }

        bool Has(Item::Type container, Item::Type item, int quantity)
        {
            auto result = this->Find(container, item, quantity);

            return result != this->Items.end();
        }

        bool Has(Item::Type container, Item::Type item)
        {
            return this->Has(container, item, 1);
        }

        // has type of item with specific property
        std::vector<Item::Base>::const_iterator Find(Item::Type type, Item::Property property)
        {
            auto result = this->Items.end();

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if ((*item).Type == type && (*item).Has(property))
                {
                    result = item;
                }
            }

            return result;
        }

        bool Has(Item::Type item, Item::Property property)
        {
            auto result = this->Find(item, property);

            return result != this->Items.end();
        }

        // has type of item with specific property and attribute
        std::vector<Item::Base>::const_iterator Find(Item::Type type, Item::Property property, Attribute::Type attribute)
        {
            auto result = this->Items.end();

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if ((*item).Type == type && (*item).Has(property) && (*item).Has(attribute))
                {
                    result = item;

                    break;
                }
            }

            return result;
        }

        bool Has(Item::Type item, Item::Property property, Attribute::Type attribute)
        {
            auto result = this->Find(item, property, attribute);

            return result != this->Items.end();
        }

        // has any item with specific property
        std::vector<Item::Base>::const_iterator Find(Item::Property property)
        {
            auto result = this->Items.end();

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if ((*item).Has(property))
                {
                    result = item;

                    break;
                }
            }

            return result;
        }

        bool Has(Item::Property property)
        {
            auto result = this->Find(property);

            return result != this->Items.end();
        }

        int Modifiers(Attribute::Type attribute)
        {
            auto modifiers = 0;

            for (auto &item : this->Items)
            {
                if (item.Is(Item::Property::EQUIPPED) && item.Has(attribute))
                {
                    modifiers += item.Attributes[attribute];
                }
            }

            return modifiers;
        }
    };
}

#endif
