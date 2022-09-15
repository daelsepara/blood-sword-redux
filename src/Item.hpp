#ifndef __ITEM_HPP__
#define __ITEM_HPP__

#include <string>
#include <unordered_map>
#include <vector>

#include "Attribute.hpp"
#include "ItemProperties.hpp"
#include "ItemTypes.hpp"

namespace BloodSword::Item
{
    class Base
    {
    public:
        // attributes and modifiers
        IntMapping<Attribute::Type> Attributes = {};

        // item properties
        std::vector<Item::Property> Properties = {};

        // item type
        Item::Type Type = Item::Type::NONE;

        // kind of items that can be stored in this container  (e.g. Money, Arrow)
        Item::Type Contains = Item::Type::NONE;

        // amount/number of the items above it currently contains
        int Quantity = 0;

        // item name
        std::string Name = "";

        void Initialize(const char *name, Item::Type type, IntMapping<Attribute::Type> attributes, std::vector<Item::Property> properties, Item::Type contains, int quantity)
        {
            this->Name = name;

            this->Type = type;

            this->Attributes = attributes;

            this->Properties = properties;

            this->Contains = contains;

            this->Quantity = quantity;
        }

        Base(const char *name, Item::Type type, IntMapping<Attribute::Type> attributes, std::vector<Item::Property> properties, Item::Type contains, int quantity)
        {
            this->Initialize(name, type, attributes, properties, contains, quantity);
        }

        Base(const char *name, Item::Type type, std::vector<Item::Property> properties, Item::Type contains, int quantity)
        {
            this->Initialize(name, type, {}, properties, contains, quantity);
        }

        Base(const char *name, Item::Type type, IntMapping<Attribute::Type> attributes, std::vector<Item::Property> properties)
        {
            this->Initialize(name, type, attributes, properties, Item::Type::NONE, 1);
        }

        Base(const char *name, Item::Type type, std::vector<Item::Property> properties)
        {
            this->Initialize(name, type, {}, properties, Item::Type::NONE, 1);
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
            return this->Attributes.find(attribute) != this->Attributes.end();
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

        int Modifier(Attribute::Type attribute)
        {
            auto modifier = 0;

            if (this->Has(attribute))
            {
                modifier = this->Attributes[attribute];
            }

            return modifier;
        }

        int Modifier(Attribute::Type attribute, Item::Property property)
        {
            auto modifier = 0;

            if (this->Has(attribute) && this->Has(property))
            {
                modifier = this->Attributes[attribute];
            }

            return modifier;
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

        bool Remove(Item::Property property)
        {
            auto result = this->Has(property);

            if (result)
            {
                for (auto i = 0; i < this->Properties.size(); i++)
                {
                    if (this->Properties[i] == property)
                    {
                        this->Properties.erase(this->Properties.begin() + i);

                        break;
                    }
                }
            }

            return result;
        }

        bool Remove(Attribute::Type attribute)
        {
            auto result = this->Has(attribute);

            if (result)
            {
                this->Attributes.erase(attribute);
            }

            return result;
        }
    };
}

#endif
