#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <algorithm>
#include <string>
#include <vector>

#include "AssetTypes.hpp"
#include "Attribute.hpp"
#include "CharacterClasses.hpp"
#include "ControlTypes.hpp"
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

    typedef std::vector<Spells::Type> Memorized;

    template <typename T>
    using Mapped = std::unordered_map<Character::Class, T>;

    class Base
    {
    public:
        BloodSword::IntMapping<Character::Status> Status = {};

        Characteristics Attributes = {};

        Abilities Skills = {};

        Inventory Items = {};

        Grimoire Spells = {};

        Memorized CalledToMind = {};

        Character::ControlType ControlType = ControlType::NONE;

        Character::Class Class = Character::Class::NONE;

        std::string Name;

        Asset::Type Asset = Asset::Type::NONE;

        int Experience = 0;

        int Moves = BloodSword::MaximumMoves;

        int Rank = 0;

        int ItemLimit = 6;

        Base(const char *name,
             Character::Class character_class,
             Characteristics attributes,
             Abilities skills,
             int moves,
             int rank) : Attributes(attributes),
                         Skills(skills),
                         ControlType(ControlType::PLAYER),
                         Class(character_class),
                         Name(name),
                         Moves(moves),
                         Rank(rank) {}

        Base(const char *name,
             Character::Class character_class,
             Characteristics attributes,
             Abilities skills) : Base(name,
                                      character_class,
                                      attributes,
                                      skills,
                                      BloodSword::MaximumMoves,
                                      2) {}

        Base(const char *name,
             Character::Class character_class,
             Characteristics attributes) : Base(name, character_class, attributes, {}, BloodSword::MaximumMoves, 2) {}

        Base(const char *name,
             Character::Class character_class) : Base(name, character_class, {}, {}, BloodSword::MaximumMoves, 2) {}

        Base(Character::Class character_class, int rank) : Base(Character::ClassMapping[character_class], character_class, {}, {}, BloodSword::MaximumMoves, rank) {}

        Base(Character::Class character_class) : Base(Character::ClassMapping[character_class], character_class, {}, {}, BloodSword::MaximumMoves, 2) {}

        Base(const char *name,
             Characteristics attributes,
             Character::ControlType control,
             Abilities skills,
             int moves) : Attributes(attributes),
                          Skills(skills),
                          ControlType(control),
                          Name(name),
                          Moves(moves) {}

        Base() {}

        bool Has(Skills::Type skill)
        {
            return this->Skills.size() > 0 && BloodSword::Found(this->Skills, skill);
        }

        void Add(Skills::Type skill)
        {
            if (!this->Has(skill))
            {
                this->Skills.push_back(skill);
            }
        }

        // does a character have this status?
        bool Has(Character::Status status)
        {
            auto has_status = this->Status.find(status) != this->Status.end();

            auto is_active = this->Status[status] != 0;

            return has_status && is_active;
        }

        bool Is(Character::Status status)
        {
            return this->Has(status);
        }

        // add status (and duration, -1 if permanent)
        void Add(Character::Status status, int duration)
        {
            this->Status[status] = duration;
        }

        // add status
        void Add(Character::Status status)
        {
            this->Add(status, Character::Duration[status]);
        }

        // remove status
        void Remove(Character::Status status)
        {
            if (this->Has(status))
            {
                this->Status.erase(status);
            }
        }

        // cooldown status
        void Cooldown(Character::Status status)
        {
            if (this->Has(status) && this->Status[status] > 0)
            {
                this->Status[status]--;
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
                if (attribute->Type == type)
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

            return this->Is(attribute) ? attribute->Value : 0;
        }

        void Value(Attribute::Type type, int value)
        {
            auto attribute = this->Attribute(type);

            if (this->Is(attribute))
            {
                attribute->Value = value;
            }
        }

        int Modifier(Attribute::Type type)
        {
            auto attribute = this->Attribute(type);

            return this->Is(attribute) ? attribute->Modifier : 0;
        }

        void Modifier(Attribute::Type type, int modifier)
        {
            auto attribute = this->Attribute(type);

            if (this->Is(attribute))
            {
                attribute->Modifier = modifier;
            }
        }

        int Maximum(Attribute::Type type)
        {
            auto attribute = this->Attribute(type);

            return this->Is(attribute) ? attribute->Maximum : 0;
        }

        void Maximum(Attribute::Type type, int maximum)
        {
            auto attribute = this->Attribute(type);

            if (this->Is(attribute))
            {
                attribute->Maximum = maximum;
            }
        }

        void Set(Attribute::Type type, int value, int modifier, int maximum)
        {
            auto attribute = this->Attribute(type);

            if (this->Is(attribute))
            {
                attribute->Value = value;

                attribute->Modifier = modifier;

                attribute->Maximum = maximum;
            }
        }

        void Set(Attribute::Type type, int value, int modifier)
        {
            auto attribute = this->Attribute(type);

            if (this->Is(attribute))
            {
                attribute->Value = value;

                attribute->Modifier = modifier;

                attribute->Value = std::min(attribute->Value, attribute->Maximum);
            }
        }

        // has item of specific type
        Inventory::const_iterator Find(Item::Type type)
        {
            auto result = this->Items.end();

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if (item->Type == type)
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
                if (item->Type == container && item->Has(Item::Property::CONTAINER) && item->Has(type, quantity))
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
                if (item->Type == container && item->Has(Item::Property::CONTAINER) && item->Contains == type)
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
                if (item->Type == type && item->Has(property))
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
                if (item->Type == type && item->Has(property) && item->Has(attribute))
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

        // has any item with all the properties
        Inventory::const_iterator Find(std::vector<Item::Property> properties)
        {
            auto result = this->Items.end();

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                auto has = true;

                for (auto property : properties)
                {
                    has &= item->Has(property);
                }

                if (has)
                {
                    result = item;

                    break;
                }
            }

            return result;
        }

        // has any item with specific property
        Inventory::const_iterator Find(Item::Property property)
        {
            return this->Find(std::vector<Item::Property>{property});
        }

        bool Has(Item::Property property)
        {
            return this->Find(property) != this->Items.end();
        }

        // modifiers from items and spells called to mind (if PSYCHIC AWARENESS)
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

            if (attribute == Attribute::Type::PSYCHIC_ABILITY)
            {
                modifiers -= this->CalledToMind.size();
            }

            return modifiers;
        }

        // is armed with a specific weapon
        bool IsArmed(Item::Type weapon)
        {
            auto armed = false;

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if (item->Has(Item::Property::WEAPON) && item->Has(Item::Property::EQUIPPED) && item->Type == weapon)
                {
                    armed = true;

                    break;
                }
            }

            return armed;
        }

        // is the character armed?
        bool IsArmed()
        {
            auto armed = false;

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if (item->Has(Item::Property::WEAPON) && item->Has(Item::Property::EQUIPPED) && item->Type != Item::Type::BOW)
                {
                    armed = true;

                    break;
                }
            }

            return armed;
        }

        // is character armed with a ranged weapon?
        bool IsArmed(Item::Type weapon, Item::Type container, Item::Type ammo)
        {
            return this->Has(container, ammo, 1) && this->Has(weapon);
        }

        // is the character a player character?
        bool IsPlayer()
        {
            return this->ControlType == Character::ControlType::PLAYER;
        }

        // recall the spell that was called to mind
        Memorized::iterator Recall(Spells::Type spell)
        {
            auto found = this->CalledToMind.end();

            for (auto search = this->CalledToMind.begin(); search != this->CalledToMind.end(); search++)
            {
                if (*search == spell)
                {
                    found = search;

                    break;
                }
            }

            return found;
        }

        // search for spell in grimoire
        Grimoire::iterator Find(Spells::Type spell)
        {
            auto found = this->Spells.end();

            for (auto search = this->Spells.begin(); search != this->Spells.end(); search++)
            {
                if (search->Type == spell)
                {
                    found = search;

                    break;
                }
            }

            return found;
        }

        // check if character knows this spell
        bool Has(Spells::Type spell)
        {
            return this->Find(spell) != this->Spells.end();
        }

        // check if spell was called to mind
        bool HasCalledToMind(Spells::Type spell)
        {
            return this->CalledToMind.size() > 0 && BloodSword::Find(this->CalledToMind, spell) != this->CalledToMind.end();
        }

        // call a spell to mind
        void CallToMind(Spells::Type spell)
        {
            if (this->Has(spell) && !this->HasCalledToMind(spell))
            {
                this->CalledToMind.push_back(spell);
            }
        }

        // forget a spell that was called to mind
        void Forget(Spells::Type spell)
        {
            if (this->Has(spell) && this->HasCalledToMind(spell))
            {
                auto recall = this->Recall(spell);

                if (recall != this->CalledToMind.end())
                {
                    this->CalledToMind.erase(recall);
                }
            }
        }

        // reset spell complexities
        void ResetSpellComplexities()
        {
            for (auto &spell : this->Spells)
            {
                spell.CurrentComplexity = spell.Complexity;
            }
        }

        void Remove(Item::Type ammo, int quantity)
        {
            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if (item->Has(ammo, quantity))
                {
                    item->Quantity -= quantity;

                    break;
                }
            }
        }

        // count ammo in all containers in character's possessions
        int Quantity(Item::Type ammo)
        {
            auto quantity = 0;

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                quantity += item->Has(ammo, 1) ? item->Quantity : 0;
            }

            return quantity;
        }

        BloodSword::IntMapping<Character::Status> LoadStatus(nlohmann::json data)
        {
            BloodSword::IntMapping<Character::Status> all_status = {};

            for (auto &[key, value] : data.items())
            {
                auto status = Character::MapStatus(std::string(key));

                if (status != Character::Status::NONE)
                {
                    all_status[status] = int(value);
                }
            }

            return all_status;
        }

        void Load(nlohmann::json data)
        {
            this->Name = !data["name"].is_null() ? std::string(data["name"]) : std::string();

            this->Rank = !data["rank"].is_null() ? int(data["rank"]) : 1;

            this->Experience = !data["experience"].is_null() ? int(data["experience"]) : 0;

            this->Moves = !data["moves"].is_null() ? int(data["moves"]) : BloodSword::MaximumMoves;

            this->ItemLimit = !data["item_limit"].is_null() ? int(data["item_limit"]) : 6;

            this->Class = !data["class"].is_null() ? Character::Map(std::string(data["class"])) : Character::Class::NONE;

            this->ControlType = !data["control_type"].is_null() ? Character::MapControlType(std::string(data["control_type"])) : Character::ControlType::NONE;

            this->Asset = !data["asset"].is_null() ? Asset::Map(std::string(data["asset"])) : Asset::Type::NONE;

            if (!data["attributes"].is_null() && data["attributes"].is_array() && data["attributes"].size() > 0)
            {
                this->Attributes = Attributes::Load(data["attributes"]);
            }

            if (!data["skills"].is_null() && data["skills"].is_array() && data["skills"].size() > 0)
            {
                this->Skills = Skills::Load(data["skills"]);
            }

            if (!data["items"].is_null() && data["items"].is_array() && data["items"].size() > 0)
            {
                this->Items = Items::Load(data["items"]);
            }

            if (!data["spells"].is_null() && data["spells"].is_array() && data["spells"].size() > 0)
            {
                this->Spells = Spells::Load(data["spells"]);
            }

            if (!data["called_to_mind"].is_null() && data["called_to_mind"].is_array() && data["called_to_mind"].size() > 0)
            {
                this->CalledToMind = Spells::Recall(data["called_to_mind"]);
            }

            if (!data["status"].is_null() && data["status"].is_object() && data["status"].size() > 0)
            {
                this->Status = this->LoadStatus(data["status"]);
            }
        }
    };

    Character::Base Load(nlohmann::json data)
    {
        auto character = Character::Base();

        character.Load(data);

        return character;
    }

    nlohmann::json Data(Character::Base character)
    {
        nlohmann::json data;

        data["name"] = character.Name;

        data["asset"] = Asset::TypeMapping[character.Asset];

        data["experience"] = character.Experience;

        data["moves"] = character.Moves;

        data["rank"] = character.Rank;

        data["item_limit"] = character.ItemLimit;

        data["control_type"] = Character::ControlTypeMapping[character.ControlType];

        data["class"] = Character::ClassMapping[character.Class];

        data["attributes"] = Attributes::Data(character.Attributes);

        if (character.Skills.size() > 0)
        {
            data["skills"] = Skills::Data(character.Skills);
        }

        if (character.Items.size() > 0)
        {
            data["items"] = Items::Data(character.Items);
        }

        if (character.Spells.size() > 0)
        {
            data["spells"] = Spells::SpellBook(character.Spells);
        }

        if (character.CalledToMind.size() > 0)
        {
            data["called_to_mind"] = Spells::Memory(character.CalledToMind);
        }

        if (character.Status.size() > 0)
        {
            nlohmann::json row;

            for (auto &status : character.Status)
            {
                auto status_name = std::string(Character::StatusMapping[status.first]);

                row.emplace(status_name, status.second);
            }

            data["status"] = row;
        }

        return data;
    }
}

#endif
