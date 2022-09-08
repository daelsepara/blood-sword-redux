#ifndef __GENERATOR_HPP__
#define __GENERATOR_HPP__

#include <vector>

#include "Attribute.hpp"
#include "Book.hpp"
#include "Character.hpp"
#include "Skills.hpp"

namespace BloodSword::Generate
{
    void Warrior(Character::Base &character)
    {
        if (character.Rank <= 2)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 12, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 1));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (character.Rank == 3)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 18, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 2));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (character.Rank == 4)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 24, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (character.Rank == 5)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 30, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 1));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (character.Rank == 6)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 36, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 2));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (character.Rank == 7)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 42, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (character.Rank == 8)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 48, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 1));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
    }

    void Trickster(Character::Base &character)
    {
        if (character.Rank <= 2)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 12, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (character.Rank == 3)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 18, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 1));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (character.Rank == 4)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 24, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 2));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (character.Rank == 5)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 30, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (character.Rank == 6)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 36, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 1));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (character.Rank == 7)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 42, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 2));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (character.Rank == 8)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 48, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 1));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
    }

    void Sage(Character::Base &character)
    {
        if (character.Rank <= 2)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 10, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (character.Rank == 3)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 15, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 1));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (character.Rank == 4)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 20, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 2));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (character.Rank == 5)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 25, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (character.Rank == 6)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 30, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 1));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (character.Rank == 7)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 35, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 2));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (character.Rank == 8)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 40, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 1));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
    }

    void Enchanter(Character::Base &character)
    {
        if (character.Rank <= 2)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 6, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 10, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, -1));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (character.Rank == 3)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 6, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 15, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (character.Rank == 4)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 20, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 1));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (character.Rank == 5)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 25, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 2));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (character.Rank == 6)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 30, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 3));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (character.Rank == 7)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 35, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 1));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (character.Rank == 8)
        {
            character.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, 40, 0));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 2));
            character.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
    }

    void Clear(Character::Base &character)
    {
        // clear attributes and skills
        character.Attributes.clear();
        character.Skills.clear();
        character.Status.clear();
        character.Spells.clear();
        character.StatusDuration.clear();
    }

    Character::Base Character(Character::Class characterClass, Book::Type book, int rank)
    {
        auto character = Character::Base(characterClass);

        Clear(character);

        // set rank and enforce bounds
        character.Rank = rank;
        character.Rank = std::min(20, character.Rank);
        character.Rank = std::max(0, character.Rank);

        // set starting experience points
        character.Experience = (character.Rank - 1) * 250;

        switch (characterClass)
        {
        case Character::Class::WARRIOR:
            Generate::Warrior(character);
            break;
        case Character::Class::TRICKSTER:
            Generate::Trickster(character);
            break;
        case Character::Class::SAGE:
            Generate::Sage(character);
            break;
        case Character::Class::ENCHANTER:
            Generate::Enchanter(character);
            break;
        default:
            break;
        }

        return character;
    }
}

#endif