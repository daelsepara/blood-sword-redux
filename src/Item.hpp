#ifndef __ITEM_HPP__
#define __ITEM_HPP__

#include <map>
#include <string>
#include <vector>

#include "Attribute.hpp"

namespace BloodSword::Item
{
    enum class Property
    {
        NORMAL = 0,
        WEAPON,
        ARMOUR,
        CONTAINER,
        EQUIPPED,
        POISONED
    };

    std::map<Item::Property, const char *> PropertyMapping = {
        {Property::NORMAL, "NORMAL"},
        {Property::WEAPON, "WEAPON"},
        {Property::ARMOUR, "ARMOUR"},
        {Property::CONTAINER, "CONTAINER"},
        {Property::EQUIPPED, "EQUIPPED"},
        {Property::POISONED, "POISONED"}};

    enum class Type
    {
        NONE,
        SWORD,
        BOW,
        ARROW,
        QUARTERSTAFF,
        QUIVER,
        POUCH,
        GOLD
    };

    std::map<Item::Type, const char *> TypeMapping = {
        {Type::NONE, "NONE"},
        {Type::SWORD, "SWORD"},
        {Type::BOW, "BOW"},
        {Type::ARROW, "ARROW"},
        {Type::QUARTERSTAFF, "QUARTERSTAFF"},
        {Type::QUIVER, "QUIVER"},
        {Type::POUCH, "POUCH"},
        {Type::GOLD, "GOLD"}};

    class Base
    {
    public:
        // attributes modified
        std::vector<Attribute::Type> Attributes = {};

        // attribute modifiers
        std::vector<int> Modifiers = {};

        // item properties
        std::vector<Item::Property> Properties = {};

        // kind of items that can be stored in this container  (e.g. Money, Arrow)
        Item::Type Contains = Item::Type::NONE;

        // amount/number of the items above it currently contains
        int Quantity = 0;

        // item type
        Item::Type Type = Item::Type::NONE;

        // item name
        std::string Name = "";

        Base(const char *name, Item::Type type, std::vector<Item::Property> properties)
        {
            this->Name = name;

            this->Type = type;

            this->Properties = properties;
        }

        bool Has(Item::Property property)
        {
            return std::find(this->Properties.begin(), this->Properties.end(), property) != this->Properties.end();
        }

        bool Is(Item::Property property)
        {
            return this->Has(property);
        }

        bool Has(Item::Type item, int quantity)
        {
            return (this->Is(Property::CONTAINER) && this->Contains == item && this->Quantity >= quantity && quantity >= 1);
        }

        bool Has(Attribute::Type attribute)
        {
            return std::find(this->Attributes.begin(), this->Attributes.end(), attribute) != this->Attributes.end();
        }

        int Modifier(Attribute::Type attribute)
        {
            auto modifier = 0;

            if (this->Has(attribute))
            {
                auto it = std::find(this->Attributes.begin(), this->Attributes.end(), attribute);

                if (it != this->Attributes.end())
                {
                    int index = it - this->Attributes.begin();

                    if (index >= 0 && index < this->Modifiers.size())
                    {
                        modifier = this->Modifiers.at(index);
                    }
                }
            }

            return modifier;
        }

        bool Has(Item::Type item)
        {
            return this->Has(item, 1);
        }

        bool Add(Item::Type item, int quantity)
        {
            auto result = false;

            if (this->Is(Property::CONTAINER) && this->Contains == item && quantity >= 1)
            {
                this->Quantity += quantity;

                result = true;
            }

            return result;
        }

        bool Remove(Item::Type item, int quantity)
        {
            auto result = false;

            if (this->Has(item, quantity))
            {
                this->Quantity -= quantity;

                result = true;
            }

            return result;
        }

        bool Remove(Item::Type item)
        {
            return this->Remove(item, 1);
        }
    };
}

#endif
