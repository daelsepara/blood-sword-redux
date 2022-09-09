#ifndef __GENERATOR_HPP__
#define __GENERATOR_HPP__

#include <vector>

#include "Attribute.hpp"
#include "Book.hpp"
#include "Character.hpp"
#include "Skills.hpp"

namespace BloodSword::Generate
{
    void WarriorAttributes(Character::Base &warrior)
    {
        if (warrior.Rank <= 2)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 12, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 1));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (warrior.Rank == 3)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 18, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 2));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (warrior.Rank == 4)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 24, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (warrior.Rank == 5)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 30, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 1));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (warrior.Rank == 6)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 36, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 2));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (warrior.Rank == 7)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 42, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (warrior.Rank == 8)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 48, 0));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 1));
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
    }

    void TricksterAttributes(Character::Base &trickster)
    {
        if (trickster.Rank <= 2)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 12, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (trickster.Rank == 3)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 18, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 1));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (trickster.Rank == 4)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 24, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 2));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (trickster.Rank == 5)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 30, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (trickster.Rank == 6)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 36, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 1));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (trickster.Rank == 7)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 42, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 2));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (trickster.Rank == 8)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 48, 0));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 1));
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
    }

    void SageAttributes(Character::Base &sage)
    {
        if (sage.Rank <= 2)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 10, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (sage.Rank == 3)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 15, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 1));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (sage.Rank == 4)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 20, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 2));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (sage.Rank == 5)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 25, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (sage.Rank == 6)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 30, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 1));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (sage.Rank == 7)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 35, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 2));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (sage.Rank == 8)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 40, 0));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 1));
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
    }

    void EnchanterAttributes(Character::Base &enchanter)
    {
        if (enchanter.Rank <= 2)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 6, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 10, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, -1));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (enchanter.Rank == 3)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 6, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 15, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (enchanter.Rank == 4)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 20, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 1));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (enchanter.Rank == 5)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 25, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 2));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (enchanter.Rank == 6)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 30, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 3));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (enchanter.Rank == 7)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 35, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 1));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (enchanter.Rank == 8)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 40, 0));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 2));
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
    }

    void Skills(Character::Base &character)
    {
        character.Skills.clear();

        switch (character.Class)
        {
        case Character::Class::WARRIOR:
            break;
        case Character::Class::TRICKSTER:
            character.Skills.push_back(Skills::Type::DODGING);
            character.Skills.push_back(Skills::Type::ARCHERY);
            break;
        case Character::Class::SAGE:
            character.Skills.push_back(Skills::Type::HEALING);
            character.Skills.push_back(Skills::Type::QUARTERSTAFF);
            character.Skills.push_back(Skills::Type::ESP);
            character.Skills.push_back(Skills::Type::PARANORMAL_SIGHT);
            character.Skills.push_back(Skills::Type::LEVITATION);
            character.Skills.push_back(Skills::Type::EXORCISM);
            character.Skills.push_back(Skills::Type::ARCHERY);
            break;
        case Character::Class::ENCHANTER:
            character.Skills.push_back(Skills::Type::SPELLS);
            break;
        default:
            break;
        }
    }

    void Clear(Character::Base &character)
    {
        // clear attributes and skills
        character.Attributes.clear();
        character.Skills.clear();
        character.Status.clear();
        character.Spells.clear();
        character.Status.clear();
        character.Items.clear();
    }

    void Attributes(Character::Base &character)
    {
        // set starting attributes scores
        switch (character.Class)
        {
        case Character::Class::WARRIOR:
            Generate::WarriorAttributes(character);
            break;
        case Character::Class::TRICKSTER:
            Generate::TricksterAttributes(character);
            break;
        case Character::Class::SAGE:
            Generate::SageAttributes(character);
            break;
        case Character::Class::ENCHANTER:
            Generate::EnchanterAttributes(character);
            break;
        default:
            break;
        }
    }

    void Rank(Character::Base &character)
    {
        // set rank based on experience points and enforce bounds
        character.Rank = (character.Experience / 250) + 1;
        character.Rank = std::min(20, character.Rank);
        character.Rank = std::max(1, character.Rank);
    }

    void Experience(Character::Base &character)
    {
        // enforce rank bounds and set starting experience points
        character.Rank = std::min(20, character.Rank);
        character.Rank = std::max(1, character.Rank);
        character.Experience = (character.Rank - 1) * 250;
    }

    void WarriorItems(Character::Base &warrior)
    {
        if (warrior.Rank <= 8)
        {
            warrior.Items.push_back(Item::Base("SWORD", Item::Type::SWORD, {Item::Property::WEAPON, Item::Property::EQUIPPED}));
            warrior.Items.push_back(Item::Base("CHAINMAIL", Item::Type::CHAINMAIL, {{Attribute::Type::ARMOUR, 3}}, {Item::Property::ARMOUR, Item::Property::EQUIPPED}));
        }
    }

    void TricksterItems(Character::Base &trickster)
    {
        if (trickster.Rank <= 8)
        {
            trickster.Items.push_back(Item::Base("SWORD", Item::Type::SWORD, {Item::Property::WEAPON, Item::Property::EQUIPPED}));
            trickster.Items.push_back(Item::Base("BOW", Item::Type::BOW, {Item::Property::WEAPON, Item::Property::EQUIPPED}));
            trickster.Items.push_back(Item::Base("QUIVER", Item::Type::QUIVER, {Item::Property::CONTAINER}, Item::Type::ARROW, 6));
            trickster.Items.push_back(Item::Base("LEATHER ARMOUR", Item::Type::LEATHER_ARMOUR, {{Attribute::Type::ARMOUR, 2}}, {Item::Property::ARMOUR, Item::Property::EQUIPPED}));
        }
    }

    void SageItems(Character::Base &sage)
    {
        if (sage.Rank <= 8)
        {
            sage.Items.push_back(Item::Base("QUARTERSTAFF", Item::Type::QUARTERSTAFF, {Item::Property::WEAPON, Item::Property::EQUIPPED}));
            sage.Items.push_back(Item::Base("BOW", Item::Type::BOW, {Item::Property::WEAPON, Item::Property::EQUIPPED}));
            sage.Items.push_back(Item::Base("QUIVER", Item::Type::QUIVER, {Item::Property::CONTAINER}, Item::Type::ARROW, 6));
            sage.Items.push_back(Item::Base("RINGMAIL", Item::Type::RINGMAIL, {{Attribute::Type::ARMOUR, 2}}, {Item::Property::ARMOUR, Item::Property::EQUIPPED}));
        }
    }

    void Money(Character::Base &character)
    {
        character.Rank = std::min(20, character.Rank);
        character.Rank = std::max(1, character.Rank);
        character.Items.push_back(Item::Base("MONEY POUCH", Item::Type::POUCH, {Item::Property::CONTAINER}, Item::Type::GOLD, character.Rank * 5));
    }

    void EnchanterItems(Character::Base &enchanter)
    {
        if (enchanter.Rank <= 8)
        {
            enchanter.Items.push_back(Item::Base("SWORD", Item::Type::SWORD, {Item::Property::WEAPON, Item::Property::EQUIPPED}));
            enchanter.Items.push_back(Item::Base("SILVER ARMOUR", Item::Type::SILVER_ARMOUR, {{Attribute::Type::ARMOUR, 2}}, {Item::Property::ARMOUR, Item::Property::EQUIPPED}));
        }
    }

    void Items(Character::Base &character)
    {
        // set starting items
        switch (character.Class)
        {
        case Character::Class::WARRIOR:
            Generate::WarriorItems(character);
            break;
        case Character::Class::TRICKSTER:
            Generate::TricksterItems(character);
            break;
        case Character::Class::SAGE:
            Generate::SageItems(character);
            break;
        case Character::Class::ENCHANTER:
            Generate::EnchanterItems(character);
            break;
        default:
            break;
        }
    }

    Character::Base Character(Character::Class characterClass, int rank)
    {
        auto character = Character::Base(characterClass, rank);

        Generate::Experience(character);

        Generate::Attributes(character);

        Generate::Skills(character);

        Generate::Items(character);

        Generate::Money(character);

        return character;
    }
}

#endif
