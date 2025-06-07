#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <algorithm>

#include "nlohmann/json.hpp"
#include "Primitives.hpp"
#include "AssetTypes.hpp"
#include "Attribute.hpp"
#include "Book.hpp"
#include "CharacterClasses.hpp"
#include "CharacterStatus.hpp"
#include "ControlTypes.hpp"
#include "Target.hpp"
#include "Item.hpp"
#include "Skills.hpp"
#include "Spells.hpp"

namespace BloodSword::Character
{
    template <typename T>
    using Mapped = std::unordered_map<Character::Class, T>;

    class Base
    {
    public:
        BloodSword::IntMapping<Character::Status> Status = {};

        Attributes::List Attributes = {};

        Skills::List Skills = {};

        Items::Inventory Items = {};

        Spells::Grimoire Spells = {};

        Spells::List CalledToMind = {};

        Spells::Strategy SpellStrategy = {};

        Character::ControlType ControlType = ControlType::NONE;

        Character::Class Class = Character::Class::NONE;

        std::string Name = std::string();

        Asset::Type Asset = Asset::Type::NONE;

        Skills::Type Fight = Skills::Type::NONE;

        Skills::Type Shoot = Skills::Type::NONE;

        int Experience = 0;

        int Moves = BloodSword::MaximumMoves;

        int Rank = 0;

        int EncumbranceLimit = 10;

        Spells::List SpellImmunity = {};

        Skills::List SkillImmunity = {};

        // target type (self)
        Target::Type Target = Target::Type::NONE;

        // additional targets
        Target::List Targets = {};

        // target probability
        int TargetProbability = 0;

        // location (survivor tracking in a battle)
        Book::Location Location = Book::Undefined;

        BloodSword::UnorderedMap<Character::Status, Item::Type> DelayedEffects = {};

        Base(const char *name,
             Character::Class character_class,
             Attributes::List attributes,
             Skills::List skills,
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
             Attributes::List attributes,
             Skills::List skills) : Base(name,
                                         character_class,
                                         attributes,
                                         skills,
                                         BloodSword::MaximumMoves,
                                         2) {}

        Base(const char *name,
             Character::Class character_class,
             Attributes::List attributes) : Base(name, character_class, attributes, {}, BloodSword::MaximumMoves, 2) {}

        Base(const char *name,
             Character::Class character_class) : Base(name, character_class, {}, {}, BloodSword::MaximumMoves, 2) {}

        Base(Character::Class character_class, int rank) : Base(Character::ClassMapping[character_class], character_class, {}, {}, BloodSword::MaximumMoves, rank) {}

        Base(Character::Class character_class) : Base(Character::ClassMapping[character_class], character_class, {}, {}, BloodSword::MaximumMoves, 2) {}

        Base(const char *name,
             Attributes::List attributes,
             Character::ControlType control,
             Skills::List skills,
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

        bool HasAny(Skills::List skills)
        {
            auto result = false;

            for (auto &skill : skills)
            {
                result |= this->Has(skill);
            }

            return result;
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
            auto has_status = BloodSword::Has(this->Status, status);

            auto is_active = this->Status[status] != 0;

            return has_status && is_active && status != Character::Status::NONE;
        }

        bool HasDelayedEffect(Character::Status status)
        {
            return BloodSword::Has(this->DelayedEffects, status);
        }

        Book::Location DelayedEffect(Character::Status status)
        {
            auto location = Book::Undefined;

            if (this->HasDelayedEffect(status))
            {
                auto item_type = this->DelayedEffects[status];

                if (item_type != Item::Type::NONE && Items::Found(item_type))
                {
                    location = Items::Defaults[item_type].DelayedEffect;
                }
            }

            return location;
        }

        bool Is(Character::Status status)
        {
            return this->Has(status);
        }

        // add status (and duration, -1 if permanent)
        void Add(Character::Status status, int duration)
        {
            if (status != Character::Status::NONE)
            {
                this->Status[status] = duration;
            }
        }

        // add status
        void Add(Character::Status status)
        {
            if (status != Character::Status::NONE)
            {
                this->Add(status, Character::Duration[status]);
            }
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

        bool Is(Attributes::List::iterator attribute)
        {
            return attribute != this->Attributes.end();
        }

        Attributes::List::iterator Attribute(Attribute::Type type)
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

                attribute->Value = std::min(attribute->Value, attribute->Maximum);

                if (type != Attribute::Type::ENDURANCE)
                {
                    attribute->Value = std::max(1, attribute->Value);
                }
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
                attribute->Maximum = std::max(0, maximum);
            }
        }

        void Set(Book::Location location)
        {
            this->Location = location;
        }

        void Set(Attribute::Type type, int value, int modifier, int maximum)
        {
            auto attribute = this->Attribute(type);

            if (this->Is(attribute))
            {
                attribute->Value = value;

                attribute->Modifier = modifier;

                attribute->Maximum = maximum;

                attribute->Value = std::min(attribute->Value, attribute->Maximum);
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
        Items::Inventory::iterator Find(Item::Type type)
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

        int Count(Item::Type type)
        {
            auto count = 0;

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if (item->Type == type)
                {
                    count++;
                }
            }

            return count;
        }

        bool Has(Item::Type item)
        {
            return this->Find(item) != this->Items.end();
        }

        bool HasAll(Items::List items)
        {
            auto has_all = true;

            for (auto item : items)
            {
                has_all &= this->Has(item);
            }

            return has_all;
        }

        bool HasCharged(Item::Type item, Item::Type charge, int quantity)
        {
            auto found = this->Find(item);

            auto has_item = found != this->Items.end();

            auto has_charge = false;

            if (has_item)
            {
                auto charged = *found;

                has_charge = (charged.Type == item && charged.Contains == charge && charged.Quantity >= quantity);
            }

            return has_item && has_charge;
        }

        int HasChargedWeapon(Item::Type charge, int quantity, bool melee)
        {
            auto found = -1;

            Item::Properties properties = {Item::Property::CONTAINER, Item::Property::CONTAINER};

            if (!melee)
            {
                properties.push_back(Item::Property::RANGED);
            }

            for (auto i = 0; i < this->Items.size(); i++)
            {
                if (this->Items[i].HasAll(properties) && this->Items[i].Contains == charge && this->Items[i].Quantity >= quantity)
                {
                    found = i;

                    break;
                }
            }

            return found;
        }

        bool AddCharge(Item::Type item, Item::Type charge, int quantity)
        {
            auto result = this->HasCharged(item, charge, quantity < 0 ? -quantity : 0);

            if (result)
            {
                auto charged_item = this->Find(item);

                (*charged_item).Quantity += quantity;
            }

            return result;
        }

        // has a container with a sufficient amount of the item
        Items::Inventory::const_iterator Find(Item::Type container, Item::Type type, int quantity)
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

        bool Has(Item::Type container, Item::Type item, int quantity)
        {
            return this->Find(container, item, quantity) != this->Items.end();
        }

        bool Has(Item::Type container, Item::Type item)
        {
            return this->Has(container, item, 1);
        }

        // has type of item with specific property
        Items::Inventory::const_iterator Find(Item::Type type, Item::Property property)
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
        Items::Inventory::const_iterator Find(Item::Type type, Item::Property property, Attribute::Type attribute)
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
        Items::Inventory::iterator Find(Item::Properties properties)
        {
            return Items::Find(this->Items, properties);
        }

        // has any item with specific property
        Items::Inventory::const_iterator Find(Item::Property property)
        {
            return this->Find(Item::Properties{property});
        }

        bool Has(Item::Property property)
        {
            return this->Find(property) != this->Items.end();
        }

        // modifiers from items and spells called to mind (if PSYCHIC ABILITY)
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

        // is character armed with a ranged weapon?
        bool IsArmed(Item::Type weapon, Item::Type container, Item::Type content)
        {
            return this->Has(container, content, 1) && this->Has(weapon);
        }

        // is character armed with a charged weapon?
        bool IsArmed(Item::Type weapon, Item::Type content)
        {
            auto container = Item::Container(weapon);

            return this->IsArmed(weapon, container, content);
        }

        // is armed with a specific weapon
        bool IsArmed(Item::Type weapon)
        {
            auto melee = Item::Requirements(weapon);

            auto ranged = Item::Requirements(weapon, true);

            if (melee != Item::Type::NONE || ranged != Item::Type::NONE)
            {
                if (melee != Item::Type::NONE)
                {
                    return this->IsArmed(weapon, melee);
                }
                else if (ranged != Item::Type::NONE)
                {
                    return this->IsArmed(weapon, ranged);
                }
            }

            auto armed = false;

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if (item->HasAll({Item::Property::WEAPON, Item::Property::EQUIPPED}) && !item->Has(Item::Property::BROKEN) && item->Type == weapon)
                {
                    armed = true;

                    break;
                }
            }

            return armed;
        }

        // is the character armed?
        bool IsArmed(Item::Property weapon)
        {
            auto armed = false;

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if (item->HasAll({Item::Property::WEAPON, Item::Property::EQUIPPED, weapon}) && !item->Has(Item::Property::BROKEN))
                {
                    armed = true;

                    break;
                }
            }

            return armed;
        }

        bool IsArmed()
        {
            return this->IsArmed(Item::Property::PRIMARY);
        }

        int WeaponModifier(Item::Property weapon_type, Attribute::Type attribute)
        {
            auto modifier = 0;

            auto found = this->Find({Item::Property::WEAPON, Item::Property::EQUIPPED, weapon_type});

            if (found != this->Items.end() && !found->Has(Item::Property::BROKEN))
            {
                modifier = found->Modifier(attribute);
            }

            return modifier;
        }

        int EquippedWeapon(Item::Property weapon_type)
        {
            auto equipped = -1;

            if (weapon_type != Item::Property::NONE)
            {
                for (auto i = 0; i < this->Items.size(); i++)
                {
                    if (this->Items[i].HasAll({Item::Property::WEAPON, weapon_type, Item::Property::EQUIPPED}))
                    {
                        equipped = i;

                        break;
                    }
                }
            }

            return equipped;
        }

        int TotalEncumbrance()
        {
            auto total = 0;

            for (auto &item : this->Items)
            {
                total += item.Encumbrance;
            }

            return total;
        }

        int SpaceLeft()
        {
            return (this->EncumbranceLimit - this->TotalEncumbrance());
        }

        int EquippedArmour()
        {
            auto equipped = -1;

            for (auto i = 0; i < this->Items.size(); i++)
            {
                if (this->Items[i].HasAll({Item::Property::ARMOUR, Item::Property::EQUIPPED}))
                {
                    equipped = i;

                    break;
                }
            }

            return equipped;
        }

        // is the character a player character?
        bool IsPlayer()
        {
            return this->ControlType == Character::ControlType::PLAYER;
        }

        bool IsEnemy()
        {
            return this->ControlType == Character::ControlType::NPC;
        }

        bool IsImmune(Skills::Type skill)
        {
            return this->SkillImmunity.size() > 0 && BloodSword::Found(this->SkillImmunity, skill);
        }

        bool IsImmune(Spells::Type spell)
        {
            return this->SpellImmunity.size() > 0 && BloodSword::Found(this->SpellImmunity, spell);
        }

        // recall the spell that was called to mind
        Spells::List::iterator Recall(Spells::Type spell)
        {
            return BloodSword::Search(this->CalledToMind, spell);
        }

        // search for spell in grimoire
        Spells::Grimoire::iterator Find(Spells::Type spell)
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
            return this->CalledToMind.size() > 0 && BloodSword::Has(this->CalledToMind, spell);
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

        // NPC casts spell (updates strategy)
        void CastSpell(Spells::Type spell)
        {
            for (auto &strategy : this->SpellStrategy)
            {
                if (strategy.Uses > 0 && strategy.Spell == spell)
                {
                    strategy.Uses--;

                    break;
                }
            }
        }

        void Add(Item::Type something, int quantity)
        {
            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if (item->Add(something, quantity))
                {
                    break;
                }
            }
        }

        void Add(Item::Base item)
        {
            Items::Add(this->Items, item);
        }

        void Remove(Item::Type content, int quantity)
        {
            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                if (item->Remove(content, quantity))
                {
                    break;
                }
            }
        }

        void Remove(Item::Type item)
        {
            if (this->Has(item))
            {
                auto index = this->Find(item);

                if (index != this->Items.end())
                {
                    this->Items.erase(index);
                }
            }
        }

        // count item quantities in all containers in character's possessions
        int Quantity(Item::Type content)
        {
            auto quantity = 0;

            for (auto item = this->Items.begin(); item != this->Items.end(); item++)
            {
                quantity += item->Has(content, 1) ? item->Quantity : 0;
            }

            return quantity;
        }
    };

    BloodSword::IntMapping<Character::Status> LoadStatus(nlohmann::json &data)
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

    BloodSword::UnorderedMap<Character::Status, Item::Type> LoadDelayedEffects(nlohmann::json &data)
    {
        auto delayed_effects = BloodSword::UnorderedMap<Character::Status, Item::Type>();

        for (auto &[key, value] : data.items())
        {
            auto status = Character::MapStatus(std::string(key));

            auto item = Item::Map(std::string(value));

            if (status != Character::Status::NONE)
            {
                delayed_effects[status] = item;
            }
        }

        return delayed_effects;
    }

    Character::Base Load(nlohmann::json &data)
    {
        auto character = Character::Base();

        character.Rank = !data["rank"].is_null() ? int(data["rank"]) : 1;

        character.Experience = !data["experience"].is_null() ? int(data["experience"]) : 0;

        character.Moves = !data["moves"].is_null() ? int(data["moves"]) : BloodSword::MaximumMoves;

        character.EncumbranceLimit = !data["encumbrance_limit"].is_null() ? int(data["encumbrance_limit"]) : 10;

        character.Class = !data["class"].is_null() ? Character::Map(std::string(data["class"])) : Character::Class::NONE;

        character.Name = !data["name"].is_null() ? std::string(data["name"]) : std::string(Character::ClassMapping[character.Class]);

        character.ControlType = !data["control_type"].is_null() ? Character::MapControlType(std::string(data["control_type"])) : Character::ControlType::NONE;

        character.Asset = !data["asset"].is_null() ? Asset::Map(std::string(data["asset"])) : Asset::Type::NONE;

        character.Fight = !data["fight"].is_null() ? Skills::Map(std::string(data["fight"])) : Skills::Type::NONE;

        character.Shoot = !data["shoot"].is_null() ? Skills::Map(std::string(data["shoot"])) : Skills::Type::NONE;

        character.Target = !data["target"].is_null() ? Target::Map(std::string(data["target"])) : Target::Type::NONE;

        if (!data["attributes"].is_null() && data["attributes"].is_array() && data["attributes"].size() > 0)
        {
            character.Attributes = Attributes::Load(data["attributes"]);
        }

        if (!data["skills"].is_null() && data["skills"].is_array() && data["skills"].size() > 0)
        {
            character.Skills = Skills::Load(data["skills"]);
        }

        if (!data["items"].is_null() && data["items"].is_array() && data["items"].size() > 0)
        {
            character.Items = Items::Load(data["items"]);
        }

        if (!data["spells"].is_null() && data["spells"].is_array() && data["spells"].size() > 0)
        {
            character.Spells = Spells::Load(data["spells"]);
        }

        if (!data["called_to_mind"].is_null() && data["called_to_mind"].is_array() && data["called_to_mind"].size() > 0)
        {
            character.CalledToMind = Spells::LoadList(data["called_to_mind"]);
        }

        if (!data["status"].is_null() && data["status"].is_object() && data["status"].size() > 0)
        {
            character.Status = Character::LoadStatus(data["status"]);
        }

        if (!data["delayed_effects"].is_null() && data["delayed_effects"].is_object() && data["delayed_effects"].size() > 0)
        {
            character.DelayedEffects = Character::LoadDelayedEffects(data["delayed_effects"]);
        }

        if (!data["spell_immunity"].is_null() && data["spell_immunity"].is_array() && data["spell_immunity"].size() > 0)
        {
            character.SpellImmunity = Spells::LoadList(data["spell_immunity"]);
        }

        if (!data["skill_immunity"].is_null() && data["skill_immunity"].is_array() && data["skill_immunity"].size() > 0)
        {
            character.SkillImmunity = Skills::Load(data["skill_immunity"]);
        }

        if (!data["targets"].is_null() && data["targets"].is_array() && data["targets"].size() > 0)
        {
            character.Targets = Target::Load(data["targets"]);
        }

        character.TargetProbability = !data["target_probability"].is_null() ? int(data["target_probability"]) : 0;

        if (!data["spell_strategy"].is_null() && data["spell_strategy"].is_array() && data["spell_strategy"].size() > 0)
        {
            character.SpellStrategy = Spells::LoadStrategy(data["spell_strategy"]);
        }

        if (!data["location"].is_null() && data["location"].is_object())
        {
            character.Location = Book::Load(data["location"]);
        }

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

        data["encumbrance_limit"] = character.EncumbranceLimit;

        data["control_type"] = Character::ControlTypeMapping[character.ControlType];

        data["class"] = Character::ClassMapping[character.Class];

        data["attributes"] = Attributes::Data(character.Attributes);

        if (character.Target != Target::Type::NONE)
        {
            data["target"] = Target::Mapping[character.Target];
        }

        if (character.Fight != Skills::Type::NONE)
        {
            data["fight"] = Skills::TypeMapping[character.Fight];
        }

        if (character.Shoot != Skills::Type::NONE)
        {
            data["shoot"] = Skills::TypeMapping[character.Shoot];
        }

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
            data["spells"] = Spells::Data(character.Spells);
        }

        if (character.CalledToMind.size() > 0)
        {
            data["called_to_mind"] = Spells::Data(character.CalledToMind);
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

        if (character.DelayedEffects.size() > 0)
        {
            nlohmann::json row;

            for (auto &effect : character.DelayedEffects)
            {
                auto status = std::string(Character::StatusMapping[effect.first]);

                auto item = std::string(Item::TypeMapping[effect.second]);

                row.emplace(status, item);
            }

            data["delayed_effects"] = row;
        }

        if (character.SpellImmunity.size() > 0)
        {
            data["spell_immunity"] = Spells::Data(character.SpellImmunity);
        }

        if (character.Targets.size() > 0)
        {
            data["targets"] = Target::Data(character.Targets);
        }

        data["target_probability"] = character.TargetProbability;

        if (Book::IsDefined(character.Location))
        {
            data["location"] = Book::Data(character.Location);
        }

        if (character.SpellStrategy.size() > 0)
        {
            data["spell_strategy"] = Spells::Data(character.SpellStrategy);
        }

        return data;
    }
}

#endif
