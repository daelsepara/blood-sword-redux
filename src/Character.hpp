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
    typedef std::vector<Skills::Type> Abilities;
    typedef std::vector<Attribute::Base> Characteristics;
    typedef std::vector<Item::Base> Inventory;
    typedef std::vector<Spells::Base> Grimoire;

    class Base
    {
    public:
        IntMapping<Character::Status> Status = {};

        Characteristics Attributes = {};

        Abilities Skills = {};

        Inventory Items = {};

        Grimoire Spells = {};

        ControlType ControlType = ControlType::NONE;

        Character::Class Class = Character::Class::NONE;

        std::string Name;

        Asset::Type Asset = Asset::Type::NONE;

        int Experience = 0;

        int Moves = 100;

        int Rank = 0;

        int ItemLimit = 6;

        Base(const char *name,
             Character::Class characterClass,
             Characteristics attributes,
             Abilities skills,
             int moves) : Attributes(attributes),
                          Skills(skills),
                          Class(characterClass),
                          Name(name),
                          Moves(moves) {}

        Base(const char *name,
             Character::Class characterClass,
             Characteristics attributes,
             Abilities skills) : Base(name,
                                      characterClass,
                                      attributes,
                                      skills,
                                      100) {}

        Base(const char *name,
             Character::Class characterClass,
             Characteristics attributes) : Base(name, characterClass, attributes, {}, 100) {}

        Base(const char *name,
             Character::Class characterClass) : Base(name, characterClass, {}, {}, 100) {}

        Base(Character::Class characterClass, int rank) : Base(Character::ClassMapping[characterClass], characterClass, {}, {}, 100)
        {
            this->Rank = rank;
        }

        Base(Character::Class characterClass) : Base(Character::ClassMapping[characterClass], characterClass, {}, {}, 100)
        {
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

        bool Is(Characteristics::iterator attribute)
        {
            return attribute != this->Attributes.end();
        }

        Characteristics::iterator Attribute(Attribute::Type type)
        {
            auto result = this->Attributes.end();

            for (auto attribute = this->Attributes.begin(); attribute != this->Attributes.end(); attribute++)
            {
                if ((*attribute).Type == type)
                {
                    result = attribute;

                    break;
                }
            }

            return result;
        }

        int Value(Attribute::Type type)
        {
            auto attribute = this->Attribute(type);

            return this->Is(attribute) ? (*attribute).Value : 0;
        }

        void Value(Attribute::Type type, int value)
        {
            auto attribute = this->Attribute(type);

            if (this->Is(attribute))
            {
                (*attribute).Value = value;
            }
        }

        int Modifier(Attribute::Type type)
        {
            auto attribute = this->Attribute(type);

            return this->Is(attribute) ? (*attribute).Modifier : 0;
        }

        void Modifier(Attribute::Type type, int modifier)
        {
            auto attribute = this->Attribute(type);

            if (this->Is(attribute))
            {
                (*attribute).Modifier = modifier;
            }
        }

        int Maximum(Attribute::Type type)
        {
            auto attribute = this->Attribute(type);

            return this->Is(attribute) ? (*attribute).Maximum : 0;
        }

        void Maximum(Attribute::Type type, int maximum)
        {
            auto attribute = this->Attribute(type);

            if (this->Is(attribute))
            {
                (*attribute).Maximum = maximum;
            }
        }

        void Set(Attribute::Type type, int value, int modifier, int maximum)
        {
            auto attribute = this->Attribute(type);

            if (this->Is(attribute))
            {
                (*attribute).Value = value;

                (*attribute).Modifier = modifier;

                (*attribute).Maximum = maximum;
            }
        }

        void Set(Attribute::Type type, int value, int modifier)
        {
            auto attribute = this->Attribute(type);

            if (this->Is(attribute))
            {
                (*attribute).Value = value;

                (*attribute).Modifier = modifier;
            }
        }

        // has item of specific type
        Inventory::const_iterator Find(Item::Type type)
        {
            auto result = this->Items.end();

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if ((*item).Type == type)
                {
                    result = item;

                    break;
                }
            }

            return result;
        }

        bool Has(Item::Type item)
        {
            return this->Find(item) != this->Items.end();
        }

        // has a container with a sufficient amount of the item
        Inventory::const_iterator Find(Item::Type container, Item::Type type, int quantity)
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

        Inventory::const_iterator Find(Item::Type container, Item::Type type)
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
            return this->Find(container, item, quantity) != this->Items.end();
        }

        bool Has(Item::Type container, Item::Type item)
        {
            return this->Has(container, item, 1);
        }

        // has type of item with specific property
        Inventory::const_iterator Find(Item::Type type, Item::Property property)
        {
            auto result = this->Items.end();

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if ((*item).Type == type && (*item).Has(property))
                {
                    result = item;

                    break;
                }
            }

            return result;
        }

        bool Has(Item::Type item, Item::Property property)
        {
            return this->Find(item, property) != this->Items.end();
        }

        // has type of item with specific property and attribute
        Inventory::const_iterator Find(Item::Type type, Item::Property property, Attribute::Type attribute)
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
            return this->Find(item, property, attribute) != this->Items.end();
        }

        // has any item with specific property
        Inventory::const_iterator Find(Item::Property property)
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
            return this->Find(property) != this->Items.end();
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
