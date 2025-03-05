#ifndef __ITEM_HPP__
#define __ITEM_HPP__

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "Attribute.hpp"
#include "ItemProperties.hpp"
#include "ItemTypes.hpp"

namespace BloodSword::Item
{
    const int Unlimited = -1;

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

        int Limit = Item::Unlimited;

        // item name
        std::string Name;

        // book description as sections
        Book::Location Description = {Book::Number::NONE, -1};

        // item effects as sections
        Book::Location Effects = {Book::Number::NONE, -1};

        // flag to check if it's revealed (i.e. with the SAGE)
        bool Revealed = false;

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
            return (this->Is(Property::CONTAINER) && (this->Contains == type) && (this->Quantity >= quantity) && ((this->Limit != Item::Unlimited && quantity >= 1) || this->Limit == Item::Unlimited));
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

            if (this->Is(Property::CONTAINER) && (this->Contains == item) && ((this->Limit == Item::Unlimited) || (((this->Quantity + quantity) <= this->Limit) && ((this->Quantity + quantity) >= 0))))
            {
                this->Quantity += quantity;

                // minimum
                this->Quantity = std::max(0, this->Quantity);

                if (this->Limit != Item::Unlimited)
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

        std::string String(bool newline = false)
        {
            auto item_string = this->Name;

            if (newline)
            {
                item_string += "\n";
            }
            else
            {
                item_string += " ";
            }

            if (this->Attributes.size() > 0 || this->Properties.size() > 0 || this->Quantity > 1)
            {
                auto stats = 0;

                item_string += "(";

                if (this->Properties.size() > 0)
                {
                    for (auto &property : this->Properties)
                    {
                        if (property != Item::Property::CONTAINER && property != Item::Property::CANNOT_DROP && property != Item::Property::CANNOT_TRADE)
                        {
                            if (stats > 0)
                            {
                                item_string += ", ";
                            }

                            item_string += Item::PropertyMapping[property];

                            stats++;
                        }
                    }
                }

                if (this->Has(Item::Property::CONTAINER) && this->Contains != Item::Type::NONE)
                {
                    if (stats > 0)
                    {
                        item_string += ", ";
                    }

                    item_string += std::string(Item::TypeMapping[this->Contains]) + ": " + std::to_string(this->Quantity);

                    stats++;
                }

                if (this->Attributes.size() > 0)
                {
                    for (auto &attribute : this->Attributes)
                    {
                        if (stats > 0)
                        {
                            item_string += ", ";
                        }

                        item_string += std::string(Attribute::Abbreviations[attribute.first]) + ": " + std::to_string(attribute.second);

                        stats++;
                    }
                }

                if ((this->Properties.size() == 0 || !this->Has(Item::Property::CONTAINER)) && this->Quantity > 1)
                {
                    if (stats > 0)
                    {
                        item_string += ", ";
                    }

                    item_string += "QUANTITY: " + std::to_string(this->Quantity);
                }

                item_string += ")";
            }

            return item_string;
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

    Item::Base Load(nlohmann::json &data)
    {
        auto item = Item::Base();

        if (!data["attributes"].is_null() && data["attributes"].is_object())
        {
            item.Attributes = Item::LoadAttributes(data["attributes"]);
        }

        if (!data["properties"].is_null() && data["properties"].is_array() && data["properties"].size() > 0)
        {
            item.Properties = Item::LoadProperties(data["properties"]);
        }

        item.Type = !data["type"].is_null() ? Item::Map(std::string(data["type"])) : Item::Type::NONE;

        item.Contains = !data["contains"].is_null() ? Item::Map(std::string(data["contains"])) : Item::Type::NONE;

        item.Quantity = !data["quantity"].is_null() ? int(data["quantity"]) : 0;

        item.Limit = !data["limit"].is_null() ? int(data["limit"]) : Item::Unlimited;

        item.Name = !data["name"].is_null() ? std::string(data["name"]) : std::string();

        if (!data["description"].is_null() && data["description"].is_object())
        {
            item.Description = Book::Load(data["description"]);
        }

        if (!data["effects"].is_null() && data["effects"].is_object())
        {
            item.Effects = Book::Load(data["effects"]);
        }

        // check whether or not description has been revealed
        item.Revealed = (!data["revealed"].is_null() && data["revealed"].is_boolean()) ? data["revealed"].get<bool>() : false;

        return item;
    }
}

namespace BloodSword::Items
{
    typedef std::vector<Item::Base> Inventory;

    typedef std::vector<Item::CardType> Deck;

    // default stats/properties for in-game items
    BloodSword::UnorderedMap<Item::Type, Item::Base> Defaults = {};

    const int Unlimited = -1;

    Items::Deck KalugenDeck = {
        Item::CardType::KING_OF_SERPENTS,
        Item::CardType::ACE_OF_STARS,
        Item::CardType::ACE_OF_SWORDS,
        Item::CardType::ACE_OF_RINGS,
        Item::CardType::BUFFOON};

    std::vector<Asset::Type> CardAssets = {
        Asset::Type::KING_DIAMONDS,
        Asset::Type::ACE_DIAMONDS,
        Asset::Type::ACE_CLUBS,
        Asset::Type::ACE_HEARTS,
        Asset::Type::CARD_JOKER};

    std::vector<int> KalugenValues = {0, 1, 2, 3, 4};

    Items::Inventory Load(nlohmann::json &data)
    {
        auto items = Items::Inventory();

        for (auto i = 0; i < data.size(); i++)
        {
            auto item = Item::Load(data[i]);

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

            row["revealed"] = item.Revealed;

            if (Book::IsDefined(item.Description))
            {
                data["description"] = Book::Data(item.Description);
            }

            if (Book::IsDefined(item.Effects))
            {
                data["effects"] = Book::Data(item.Effects);
            }

            data.push_back(row);
        }

        return data;
    }

    Items::Deck LoadDeck(nlohmann::json data)
    {
        auto deck = Items::Deck();

        for (auto i = 0; i < data.size(); i++)
        {
            auto card = !data[i].is_null() ? Item::MapCard(std::string(data[i])) : Item::CardType::NONE;

            if (card != Item::CardType::NONE)
            {
                deck.push_back(card);
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
                deck_list.push_back(Item::CardMapping[card]);
            }
        }

        return deck_list;
    }

    // loads defaults for items
    void LoadDefaults(const char *filename)
    {
        std::ifstream file(filename);

        if (file.good())
        {
            auto data = nlohmann::json::parse(file);

            if (!data["items"].is_null() && data["items"].is_array() && data["items"].size() > 0)
            {
                for (auto i = 0; i < data["items"].size(); i++)
                {
                    auto item = Item::Load(data["items"][i]);

                    if (item.Type != Item::Type::NONE)
                    {
                        Items::Defaults[item.Type] = item;
                    }
                }
            }

            file.close();
        }
    }
}

#endif
