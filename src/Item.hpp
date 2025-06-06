#ifndef __ITEM_HPP__
#define __ITEM_HPP__

#include <fstream>

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "Attribute.hpp"
#include "ItemTargetEffects.hpp"
#include "ItemProperties.hpp"
#include "ItemTypes.hpp"

namespace BloodSword::Item
{
    const int Unlimited = -1;

    const int NotFound = -1;

    // item damage modifier
    struct Damage
    {
        int Value = 0;

        int Modifier = 0;

        bool IgnoreArmour = false;

        Damage() {}

        Damage(int value, int modifier) : Value(value), Modifier(modifier) {}

        Damage(int value, int modifier, bool ignore_armour) : Value(value), Modifier(modifier), IgnoreArmour(ignore_armour) {}
    };

    class BattleDescription
    {
    public:
        Book::Location Battle = Book::Undefined;

        Book::Location Description = Book::Undefined;

        BattleDescription() {}

        BattleDescription(Book::Location battle, Book::Location description) : Battle(battle), Description(description) {}
    };

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

        // item asset
        Asset::Type Asset = Asset::Type::NONE;

        // item name
        std::string Name;

        // book description as sections
        Book::Location Description = Book::Undefined;

        // item effects as sections
        Book::Location Effects = Book::Undefined;

        // item delayed effects as sections
        Book::Location DelayedEffect = Book::Undefined;

        // list of battlefield effects
        std::vector<Item::BattleDescription> BattleDescriptions = {};

        // flag to check if it's revealed (i.e. with the SAGE)
        bool Revealed = false;

        // flag to see if it drops when used (e.g. thrown)
        bool Drops = false;

        // for specific targetting (damage rolls/modifiers)
        BloodSword::UnorderedMap<Target::Type, Item::Damage> DamageTypes = {};

        // modifiers (+/- damage rolls/modifiers)
        BloodSword::UnorderedMap<Target::Type, Item::Damage> DamageModifiers = {};

        // for specific targetting effects
        BloodSword::UnorderedMap<Target::Type, Item::TargetEffect> TargetEffects = {};

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

        bool Is(Item::Type item)
        {
            return (this->Type == item);
        }

        // has a charged item
        bool Is(Item::Type item, Item::Type charge, int quantity)
        {
            return (this->Is(Property::CONTAINER) && this->Is(item) && (this->Contains == charge) && (this->Quantity >= quantity) && ((this->Limit != Item::Unlimited && quantity >= 1) || this->Limit == Item::Unlimited));
        }

        // check if item has this attribute
        bool Has(Attribute::Type attribute)
        {
            return BloodSword::Has(this->Attributes, attribute);
        }

        bool HasEffect(Target::Type target)
        {
            return BloodSword::Has(this->TargetEffects, target);
        }

        bool HasDamageType(Target::Type target)
        {
            return BloodSword::Has(this->DamageTypes, target);
        }

        bool HasDamageModifier(Target::Type target)
        {
            return BloodSword::Has(this->DamageModifiers, target);
        }

        bool HasAll(Item::Properties properties)
        {
            auto has = true;

            for (auto property : properties)
            {
                has &= this->Has(property);
            }

            return has;
        }

        bool HasAny(Item::Properties properties)
        {
            auto has = false;

            for (auto property : properties)
            {
                has |= this->Has(property);
            }

            return has;
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

        bool IsCharged(Item::Type charge, int quantity)
        {
            return (this->Contains == charge && this->Quantity >= quantity);
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

            return !this->Has(property);
        }

        bool Add(Item::Property property)
        {
            auto result = !this->Has(property);

            if (result)
            {
                this->Properties.push_back(property);
            }

            return this->Has(property);
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

        void Reveal()
        {
            this->Revealed = true;
        }

        void Hide()
        {
            this->Revealed = false;
        }

        std::string String(bool newline = false)
        {
            auto item_string = this->Name;

            if (this->Type != Item::Type::GOLD)
            {
                if (newline)
                {
                    item_string += "\n";
                }
                else
                {
                    item_string += " ";
                }

                auto visible = 0;

                for (auto &property : this->Properties)
                {
                    visible += !Item::IsInvisible(property) ? 1 : 0;
                }

                if (this->Attributes.size() > 0 || (this->Properties.size() > 0 && visible > 0) || this->Quantity > 1)
                {
                    auto stats = 0;

                    item_string += "(";

                    if (this->Properties.size() > 0)
                    {
                        for (auto &property : this->Properties)
                        {
                            if (!Item::IsInvisible(property))
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
            }
            else
            {
                // GOLD pieces
                item_string = std::to_string(this->Quantity) + " " + item_string;
            }

            return item_string;
        }
    };

    Item::Type Requirements(Item::Type item, bool ranged = false)
    {
        auto requirement = Item::Type::NONE;

        if (ranged)
        {
            requirement = BloodSword::Find(Item::RangedRequirements, item);
        }
        else
        {
            requirement = BloodSword::Find(Item::MeleeRequirements, item);
        }

        return requirement;
    }

    Item::Type Container(Item::Type item)
    {
        auto container = BloodSword::Find(Item::StorageRequirements, item);

        return container != Item::Type::NONE ? container : item;
    }

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

        item.Asset = !data["asset"].is_null() ? Asset::Map(data["asset"]) : Asset::Type::NONE;

        item.Name = !data["name"].is_null() ? std::string(data["name"]) : std::string();

        if (!data["description"].is_null() && data["description"].is_object())
        {
            item.Description = Book::Load(data["description"]);
        }

        if (!data["effects"].is_null() && data["effects"].is_object())
        {
            item.Effects = Book::Load(data["effects"]);
        }

        if (!data["delayed_effect"].is_null() && data["delayed_effect"].is_object())
        {
            item.DelayedEffect = Book::Load(data["delayed_effect"]);
        }

        if (!data["battle_descriptions"].is_null() && data["battle_descriptions"].is_array() && data["battle_descriptions"].size() > 0)
        {
            item.BattleDescriptions.clear();

            for (auto i = 0; i < data["battle_descriptions"].size(); i++)
            {
                auto battle = Book::Load(data["battle_descriptions"][i]["battle"]);

                auto description = Book::Load(data["battle_descriptions"][i]["description"]);

                if (Book::IsDefined(battle) && Book::IsDefined(description))
                {
                    item.BattleDescriptions.push_back(Item::BattleDescription(battle, description));
                }
            }
        }

        if (!data["damage_types"].is_null() && data["damage_types"].is_object())
        {
            item.DamageTypes.clear();

            for (auto &[key, val] : data["damage_types"].items())
            {
                if (val.is_object())
                {
                    auto target = Target::Map(std::string(key));

                    auto damage = !val["damage"].is_null() ? int(val["damage"]) : 0;

                    auto modifier = !val["modifier"].is_null() ? int(val["modifier"]) : 0;

                    auto ignore_armour = (!val["ignore_armour"].is_null() && val["ignore_armour"].is_boolean()) ? val["ignore_armour"].get<bool>() : false;

                    if (target != Target::Type::NONE)
                    {
                        item.DamageTypes[target] = Item::Damage(damage, modifier, ignore_armour);
                    }
                }
            }
        }

        if (!data["damage_modifiers"].is_null() && data["damage_modifiers"].is_object())
        {
            item.DamageModifiers.clear();

            for (auto &[key, val] : data["damage_modifiers"].items())
            {
                if (val.is_object())
                {
                    auto target = Target::Map(std::string(key));

                    auto damage = !val["damage"].is_null() ? int(val["damage"]) : 0;

                    auto modifier = !val["modifier"].is_null() ? int(val["modifier"]) : 0;

                    auto ignore_armour = (!val["ignore_armour"].is_null() && val["ignore_armour"].is_boolean()) ? val["ignore_armour"].get<bool>() : false;

                    if (target != Target::Type::NONE)
                    {
                        item.DamageModifiers[target] = Item::Damage(damage, modifier, ignore_armour);
                    }
                }
            }
        }

        if (!data["target_effects"].is_null() && data["target_effects"].is_object())
        {
            item.TargetEffects.clear();

            for (auto &[key, val] : data["target_effects"].items())
            {
                auto target = Target::Map(std::string(key));

                auto effect = Item::MapTargetEffect(std::string(val));

                if (target != Target::Type::NONE && effect != Item::TargetEffect::NONE)
                {
                    item.TargetEffects[target] = effect;
                }
            }
        }

        // check whether or not description has been revealed
        item.Revealed = (!data["revealed"].is_null() && data["revealed"].is_boolean()) ? data["revealed"].get<bool>() : false;

        item.Drops = (!data["drops"].is_null() && data["drops"].is_boolean()) ? data["drops"].get<bool>() : false;

        return item;
    }
}

namespace BloodSword::Items
{
    // default stats/properties for in-game items
    BloodSword::UnorderedMap<Item::Type, Item::Base> Defaults = {};

    BloodSword::UnorderedMap<Item::Type, std::string> Descriptions = {};

    typedef std::vector<Item::Base> Inventory;

    typedef std::vector<Item::CardType> Deck;

    typedef std::vector<Item::Type> List;

    const int Unlimited = -1;

    Items::Deck KalugenDeck = {
        Item::CardType::KING_OF_SERPENTS,
        Item::CardType::ACE_OF_STARS,
        Item::CardType::ACE_OF_SWORDS,
        Item::CardType::ACE_OF_RINGS,
        Item::CardType::BUFFOON};

    Asset::List CardAssets = {
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

            if (Book::IsDefined(item.DelayedEffect))
            {
                data["delayed_effect"] = Book::Data(item.DelayedEffect);
            }

            if (item.BattleDescriptions.size() > 0)
            {
                nlohmann::json battle_descriptions;

                for (auto battle_item : item.BattleDescriptions)
                {
                    if (Book::IsDefined(battle_item.Battle) && Book::IsDefined(battle_item.Description))
                    {
                        nlohmann::json battle_description;

                        battle_description.emplace("battle", Book::Data(battle_item.Battle));

                        battle_description.emplace("description", Book::Data(battle_item.Description));

                        battle_descriptions.push_back(battle_description);
                    }
                }

                if (battle_descriptions.size() > 0)
                {
                    data["battle_descriptions"] = battle_descriptions;
                }
            }

            if (item.DamageTypes.size() > 0)
            {
                nlohmann::json damage_types;

                for (auto &damage : item.DamageTypes)
                {
                    nlohmann::json damage_type;

                    auto target = std::string(Target::Mapping[damage.first]);

                    auto value = damage.second.Value;

                    auto modifier = damage.second.Modifier;

                    damage_type.emplace("value", value);

                    damage_type.emplace("modifier", modifier);

                    damage_types.emplace(target, damage_type);
                }

                row["damage_types"] = damage_types;
            }

            if (item.TargetEffects.size() > 0)
            {
                nlohmann::json target_effects;

                for (auto &targets : item.TargetEffects)
                {
                    nlohmann::json target_effect;

                    auto target = std::string(Target::Mapping[targets.first]);

                    auto effect = std::string(Item::TargetEffectMapping[targets.second]);

                    target_effects.emplace(target, effect);
                }

                if (target_effects.size() > 0)
                {
                    row["target_effects"] = target_effects;
                }
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
    void LoadDefaults(const char *items)
    {
        // clear global map
        Items::Defaults.clear();

        std::ifstream ifs(items);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

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
#if defined(DEBUG)
                std::cerr << "[LOADED] " << Items::Defaults.size() << " items" << std::endl;
#endif
            }

            ifs.close();
        }
    }

    void LoadDefaults(std::string filename)
    {
        Items::LoadDefaults(filename.c_str());
    }

    void LoadDescriptions(const char *items)
    {
        // clear global map
        Items::Descriptions.clear();

        std::ifstream ifs(items);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data["descriptions"].is_null() && data["descriptions"].is_array() && data["descriptions"].size() > 0)
            {
                for (auto i = 0; i < data["descriptions"].size(); i++)
                {
                    auto item = !data["descriptions"][i]["item"].is_null() ? Item::Map(data["descriptions"][i]["item"]) : Item::Type::NONE;

                    std::string description = !data["descriptions"][i]["description"].is_null() ? data["descriptions"][i]["description"] : "";

                    if (item != Item::Type::NONE)
                    {
                        Items::Descriptions[item] = description;
                    }
                }
#if defined(DEBUG)
                std::cerr << "[LOADED] " << Items::Descriptions.size() << " descriptions" << std::endl;
#endif
            }

            ifs.close();
        }
    }

    void LoadDescriptions(std::string items)
    {
        Items::LoadDescriptions(items.c_str());
    }

    bool Found(Item::Type item)
    {
        return BloodSword::Has(Items::Defaults, item);
    }

    bool FoundDescription(Item::Type item)
    {
        return BloodSword::Has(Items::Descriptions, item);
    }

    Items::Inventory::iterator Find(Items::Inventory &items, Item::Type container, Item::Type type)
    {
        auto result = items.end();

        for (auto item = items.begin(); item != items.end(); item++)
        {
            if (item->Type == container && item->Has(Item::Property::CONTAINER) && item->Contains == type)
            {
                result = item;

                break;
            }
        }

        return result;
    }

    // has any item with all the properties
    Items::Inventory::iterator Find(Items::Inventory &items, Item::Properties properties)
    {
        auto result = items.end();

        for (auto item = items.begin(); item != items.end(); item++)
        {
            auto has = item->HasAll(properties);

            if (has)
            {
                result = item;

                break;
            }
        }

        return result;
    }

    // has any item with all the properties
    Items::Inventory::iterator Find(Items::Inventory &items, Item::Type type)
    {
        auto result = items.end();

        for (auto item = items.begin(); item != items.end(); item++)
        {
            auto has = (*item).Type == type;

            if (has)
            {
                result = item;

                break;
            }
        }

        return result;
    }

    void Add(Items::Inventory &items, Item::Base item)
    {
        if (item.Type != Item::Type::NONE && item.Name.size() > 0)
        {
            auto is_container = false;

            auto container = Item::Type::NONE;

            // check if container
            for (auto i = 0; i < items.size(); i++)
            {
                if (items[i].Has(Item::Property::CONTAINER) && items[i].Contains == item.Type)
                {
                    is_container = true;

                    container = items[i].Type;

                    break;
                }
            }

            if (is_container && container != Item::Type::NONE)
            {
                auto found = Items::Find(items, container, item.Type);

                if (found != items.end())
                {
                    (*found).Quantity += item.Quantity;

                    (*found).Quantity = std::min(0, (*found).Quantity);

                    if ((*found).Limit != Item::Unlimited)
                    {
                        (*found).Quantity = std::max((*found).Quantity, (*found).Limit);
                    }
                }
            }
            else
            {
                items.push_back(item);
            }
        }
    }

    bool Included(Items::List list, Item::Type item)
    {
        auto included = false;

        if (item != Item::Type::NONE)
        {
            for (auto i = 0; i < list.size(); i++)
            {
                if (list[i] == item)
                {
                    included = true;

                    break;
                }
            }
        }

        return included;
    }
}

#endif
