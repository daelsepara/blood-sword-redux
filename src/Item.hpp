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
    // item base class
    class Base
    {
    public:
        // attributes and modifiers
        BloodSword::IntMapping<Attribute::Type> Attributes = {};

        // item properties
        std::vector<Item::Property> Properties = {};

        // item type
        Item::Type Type = Item::Type::NONE;

        // kind of items that can be stored in this container  (e.g. Money, Arrow)
        Item::Type Contains = Item::Type::NONE;

        // amount/number of the items above it currently contains
        int Quantity = 0;

        // item name
        std::string Name;

        Base(const char *name,
             Item::Type type,
             BloodSword::IntMapping<Attribute::Type> attributes,
             std::vector<Item::Property> properties,
             Item::Type contains,
             int quantity) : Attributes(attributes),
                             Properties(properties),
                             Type(type),
                             Contains(contains),
                             Quantity(quantity),
                             Name(name) {}

        Base(const char *name,
             Item::Type type,
             std::vector<Item::Property> properties,
             Item::Type contains,
             int quantity) : Base(name, type, {}, properties, contains, quantity) {}

        Base(const char *name,
             Item::Type type,
             BloodSword::IntMapping<Attribute::Type> attributes,
             std::vector<Item::Property> properties) : Base(name,
                                                            type,
                                                            attributes,
                                                            properties,
                                                            Item::Type::NONE, 1) {}

        Base(const char *name,
             Item::Type type,
             std::vector<Item::Property> properties) : Base(name, type, {}, properties, Item::Type::NONE, 1) {}

        // check if item has this property
        bool Has(Item::Property property)
        {
            return BloodSword::Found(this->Properties, property);
        }

        // check if item has this property
        bool Is(Item::Property property)
        {
            return this->Has(property);
        }

        // this item contains a type of item and of sufficient quanity
        bool Has(Item::Type type, int quantity)
        {
            return (this->Is(Property::CONTAINER) && this->Contains == type && this->Quantity >= quantity && quantity >= 1);
        }

        // check if item has this attribute
        bool Has(Attribute::Type attribute)
        {
            return this->Attributes.find(attribute) != this->Attributes.end();
        }

        // check if this item contains this type of object
        bool Has(Item::Type item)
        {
            return this->Has(item, 1);
        }

        // add item (of quanity) to this container
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

        // return attribute-modifier value of this item, if any
        int Modifier(Attribute::Type attribute)
        {
            auto modifier = 0;

            if (this->Has(attribute))
            {
                modifier = this->Attributes[attribute];
            }

            return modifier;
        }

        // return attribute-modifier value of this item (with specific property), if any
        int Modifier(Attribute::Type attribute, Item::Property property)
        {
            auto modifier = 0;

            if (this->Has(attribute) && this->Has(property))
            {
                modifier = this->Attributes[attribute];
            }

            return modifier;
        }

        // remove a quantity of the item from this container
        bool Remove(Item::Type type, int quantity)
        {
            auto result = false;

            if (this->Has(type, quantity))
            {
                this->Quantity -= quantity;

                result = true;
            }

            return result;
        }

        // remove one unit of this item
        bool Remove(Item::Type type)
        {
            return this->Remove(type, 1);
        }

        // remove item property, e.g. remove 'EQUIPPED' property
        bool Remove(Item::Property property)
        {
            auto result = this->Has(property);

            if (result)
            {
                auto found = BloodSword::Find(this->Properties, property);

                if (found != this->Properties.end())
                {
                    this->Properties.erase(found);
                }
            }

            return result;
        }

        // remove item property, e.g. remove 'FIGHTING PROWESS' attribute, i.e. it will no longer modify
        // the bearer's FIGHTING PROWESS attribute
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
