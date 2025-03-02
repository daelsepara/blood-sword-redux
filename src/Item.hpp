#ifndef __ITEM_HPP__
#define __ITEM_HPP__

#include <string>
#include <unordered_map>
#include <vector>

#include "nlohmann/json.hpp"
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
        Item::Properties Properties = {};

        // item type
        Item::Type Type = Item::Type::NONE;

        // kind of items that can be stored in this container  (e.g. Money, Arrow)
        Item::Type Contains = Item::Type::NONE;

        // amount/number of the items above it currently contains
        int Quantity = 0;

        int Limit = -1;

        // item name
        std::string Name;

        Base() {}

        Base(const char *name,
             Item::Type type,
             BloodSword::IntMapping<Attribute::Type> attributes,
             Item::Properties properties,
             Item::Type contains,
             int quantity,
             int limit) : Attributes(attributes),
                          Properties(properties),
                          Type(type),
                          Contains(contains),
                          Quantity(quantity),
                          Limit(limit),
                          Name(name) {}

        Base(const char *name,
             Item::Type type,
             BloodSword::IntMapping<Attribute::Type> attributes,
             Item::Properties properties,
             Item::Type contains,
             int quantity) : Attributes(attributes),
                             Properties(properties),
                             Type(type),
                             Contains(contains),
                             Quantity(quantity),
                             Name(name) {}

        Base(const char *name,
             Item::Type type,
             Item::Properties properties,
             Item::Type contains,
             int quantity) : Base(name, type, {}, properties, contains, quantity) {}

        Base(const char *name,
             Item::Type type,
             Item::Properties properties,
             Item::Type contains,
             int quantity,
             int limit) : Base(name, type, {}, properties, contains, quantity, limit) {}

        Base(const char *name,
             Item::Type type,
             BloodSword::IntMapping<Attribute::Type> attributes,
             Item::Properties properties) : Base(name,
                                                 type,
                                                 attributes,
                                                 properties,
                                                 Item::Type::NONE, 1) {}

        Base(const char *name,
             Item::Type type,
             Item::Properties properties) : Base(name, type, {}, properties, Item::Type::NONE, 1) {}

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
            return (this->Is(Property::CONTAINER) && (this->Contains == type) && (this->Quantity >= quantity) && ((this->Limit != -1 && quantity >= 1) || this->Limit == -1));
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

            if (this->Is(Property::CONTAINER) && (this->Contains == item) && ((this->Limit == -1) || (((this->Quantity + quantity) <= this->Limit) && ((this->Quantity + quantity) >= 0))))
            {
                this->Quantity += quantity;

                // minimum
                this->Quantity = std::max(0, this->Quantity);

                if (this->Limit != -1)
                {
                    // maximum
                    this->Quantity = std::min(this->Quantity, this->Limit);
                }

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
                result = this->Add(type, -quantity);
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

    BloodSword::IntMapping<Attribute::Type> LoadAttributes(nlohmann::json &data)
    {
        BloodSword::IntMapping<Attribute::Type> attributes = {};

        for (auto &[key, value] : data.items())
        {
            auto attribute = Attribute::Map(std::string(key));

            if (attribute != Attribute::Type::NONE)
            {
                attributes[attribute] = int(value);
            }
        }

        return attributes;
    }

    Item::Properties LoadProperties(nlohmann::json &data)
    {
        auto properties = Item::Properties();

        for (auto i = 0; i < data.size(); i++)
        {
            properties.push_back(Item::MapProperty(std::string(data[i])));
        }

        return properties;
    }
}

namespace BloodSword::Items
{
    typedef std::vector<Item::Base> Inventory;

    typedef BloodSword::IntMapping<Item::CardType> Deck;

    Items::Inventory Load(nlohmann::json &data)
    {
        auto items = Items::Inventory();

        for (auto i = 0; i < data.size(); i++)
        {
            auto item = Item::Base();

            if (!data[i]["attributes"].is_null() && data[i]["attributes"].is_object())
            {
                item.Attributes = Item::LoadAttributes(data[i]["attributes"]);
            }

            if (!data[i]["properties"].is_null() && data[i]["properties"].is_array() && data[i]["properties"].size() > 0)
            {
                item.Properties = Item::LoadProperties(data[i]["properties"]);
            }

            item.Type = !data[i]["type"].is_null() ? Item::Map(std::string(data[i]["type"])) : Item::Type::NONE;

            item.Contains = !data[i]["contains"].is_null() ? Item::Map(std::string(data[i]["contains"])) : Item::Type::NONE;

            item.Quantity = !data[i]["quantity"].is_null() ? int(data[i]["quantity"]) : 0;

            item.Limit = !data[i]["limiy"].is_null() ? int(data[i]["limit"]) : -1;

            item.Name = !data[i]["name"].is_null() ? std::string(data[i]["name"]) : std::string();

            if (item.Name.size() > 0)
            {
                items.push_back(item);
            }
        }

        return items;
    }

    nlohmann::json Data(Items::Inventory &items)
    {
        nlohmann::json data;

        for (auto &item : items)
        {
            nlohmann::json row;

            if (item.Attributes.size() > 0)
            {
                nlohmann::json attributes;

                for (auto &attribute : item.Attributes)
                {
                    auto attribute_name = std::string(Attribute::TypeMapping[attribute.first]);

                    attributes.emplace(attribute_name, attribute.second);
                }

                row["attributes"] = attributes;
            }

            if (item.Properties.size() > 0)
            {
                nlohmann::json properties;

                for (auto &property : item.Properties)
                {
                    properties.push_back(std::string(Item::PropertyMapping[property]));
                }

                row["properties"] = properties;
            }

            row["name"] = item.Name;

            row["contains"] = std::string(Item::TypeMapping[item.Contains]);

            row["type"] = std::string(Item::TypeMapping[item.Type]);

            row["quantity"] = item.Quantity;

            data.push_back(row);
        }

        return data;
    }

    Items::Deck LoadDeck(nlohmann::json data)
    {
        Items::Deck deck = {};

        for (auto &[key, value] : data.items())
        {
            auto card = Item::MapCard(std::string(key));

            if (card != Item::CardType::NONE)
            {
                deck[card] = int(value);
            }
        }

        return deck;
    }

    nlohmann::json DeckData(Items::Deck &deck)
    {
        nlohmann::json deck_list;

        if (deck.size() > 0)
        {
            for (auto &card : deck)
            {
                auto card_name = std::string(Item::CardMapping[card.first]);

                deck_list.emplace(card_name, card.second);
            }
        }

        return deck_list;
    }
}

#endif
