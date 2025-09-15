#ifndef __GENERATOR_HPP__
#define __GENERATOR_HPP__

#include "Character.hpp"

// functions for generating characters
namespace BloodSword::Generate
{
    // warriors's attributes based on rank
    void WarriorAttributes(Character::Base &warrior)
    {
        if (warrior.Class != Character::Class::WARRIOR)
        {
            return;
        }

        if (warrior.Rank <= 2)
        {
            warrior.Rank = 2;

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 1));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (warrior.Rank == 3)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 2));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (warrior.Rank == 4)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (warrior.Rank == 5)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 1));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (warrior.Rank == 6)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 2));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (warrior.Rank == 7)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (warrior.Rank == 8)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 1));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (warrior.Rank == 9)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 2));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (warrior.Rank == 10)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 4, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (warrior.Rank == 11)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 4, 1));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (warrior.Rank == 12)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 4, 2));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (warrior.Rank == 13)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 5, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (warrior.Rank == 14)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 10, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 5, 1));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (warrior.Rank == 15)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 10, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 5, 2));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (warrior.Rank == 16)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 10, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 6, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (warrior.Rank == 17)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 10, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 6, 1));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (warrior.Rank == 18)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 10, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 6, 2));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (warrior.Rank == 19)
        {
            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 10, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 7, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else
        {
            warrior.Rank = 20;

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 11, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 7, 1));

            warrior.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }

        warrior.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, warrior.Rank * 6, 0));
    }

    // trickster's attributes based on rank
    void TricksterAttributes(Character::Base &trickster)
    {
        if (trickster.Class != Character::Class::TRICKSTER)
        {
            return;
        }

        if (trickster.Rank <= 2)
        {
            trickster.Rank = 2;

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (trickster.Rank == 3)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 6, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 1));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (trickster.Rank == 4)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 2));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (trickster.Rank == 5)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (trickster.Rank == 6)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 1));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (trickster.Rank == 7)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 2));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (trickster.Rank == 8)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
        else if (trickster.Rank == 9)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 1));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
        else if (trickster.Rank == 10)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 2));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
        else if (trickster.Rank == 11)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 4, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
        else if (trickster.Rank == 12)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 4, 1));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
        else if (trickster.Rank == 13)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 4, 2));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
        else if (trickster.Rank == 14)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 5, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 10, 0));
        }
        else if (trickster.Rank == 15)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 5, 1));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 10, 0));
        }
        else if (trickster.Rank == 16)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 5, 2));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 10, 0));
        }
        else if (trickster.Rank == 17)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 6, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 10, 0));
        }
        else if (trickster.Rank == 18)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 6, 1));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 10, 0));
        }
        else if (trickster.Rank == 18)
        {
            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 6, 2));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 10, 0));
        }
        else
        {
            trickster.Rank = 20;

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 10, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 10, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 7, 0));

            trickster.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 11, 0));
        }

        trickster.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, trickster.Rank * 6, 0));
    }

    // sage's attributes based on rank
    void SageAttributes(Character::Base &sage)
    {
        if (sage.Class != Character::Class::SAGE)
        {
            return;
        }

        if (sage.Rank <= 2)
        {
            sage.Rank = 2;

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (sage.Rank == 3)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 7, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 1));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (sage.Rank == 4)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 2));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (sage.Rank == 5)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (sage.Rank == 6)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 1));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (sage.Rank == 7)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 2));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (sage.Rank == 8)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (sage.Rank == 9)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 1));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (sage.Rank == 10)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 2));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (sage.Rank == 11)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 4, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (sage.Rank == 12)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 4, 1));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (sage.Rank == 13)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 4, 2));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (sage.Rank == 14)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 5, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (sage.Rank == 15)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 5, 1));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (sage.Rank == 16)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 10, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 5, 2));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
        else if (sage.Rank == 17)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 10, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 6, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
        else if (sage.Rank == 18)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 10, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 6, 1));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
        else if (sage.Rank == 19)
        {
            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 10, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 6, 2));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
        else
        {
            sage.Rank = 20;

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 10, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 10, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 7, 0));

            sage.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 10, 0));
        }

        sage.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, sage.Rank * 5, 0));
    }

    // enchanter's attributes based on rank
    void EnchanterAttributes(Character::Base &enchanter)
    {
        if (enchanter.Class != Character::Class::ENCHANTER)
        {
            return;
        }

        if (enchanter.Rank <= 2)
        {
            enchanter.Rank = 2;

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 6, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, -1));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (enchanter.Rank == 3)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 6, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (enchanter.Rank == 4)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 1));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 6, 0));
        }
        else if (enchanter.Rank == 5)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 2));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (enchanter.Rank == 6)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 1, 3));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (enchanter.Rank == 7)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 8, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 1));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (enchanter.Rank == 8)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 7, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 2));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (enchanter.Rank == 9)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 2, 3));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (enchanter.Rank == 10)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (enchanter.Rank == 11)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 1));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 7, 0));
        }
        else if (enchanter.Rank == 12)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 2));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (enchanter.Rank == 13)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 9, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 3, 3));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (enchanter.Rank == 14)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 8, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 10, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 4, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (enchanter.Rank == 15)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 10, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 4, 1));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (enchanter.Rank == 16)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 10, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 4, 2));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (enchanter.Rank == 17)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 10, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 4, 3));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 8, 0));
        }
        else if (enchanter.Rank == 18)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 10, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 5, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
        else if (enchanter.Rank == 19)
        {
            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 10, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 5, 1));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }
        else
        {
            enchanter.Rank = 20;

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::FIGHTING_PROWESS, 9, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, 11, 0));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::DAMAGE, 5, 2));

            enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::AWARENESS, 9, 0));
        }

        enchanter.Attributes.push_back(Attribute::Base(Attribute::Type::ENDURANCE, enchanter.Rank * 5, 0));
    }

    // set character-specific skills
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

            character.SkillImmunity.push_back(Skills::Type::PARALYZING_TOUCH);

            break;

        case Character::Class::ENCHANTER:

            character.Skills.push_back(Skills::Type::SPELLS);

            break;

        default:

            break;
        }
    }

    void EnchanterSpells(Character::Base &character)
    {
        if (!character.Has(Skills::Type::SPELLS) || character.Class != Character::Class::ENCHANTER)
        {
            return;
        }

        character.Spells.clear();

        character.Spells.push_back(Spells::Base(Spells::Type::VOLCANO_SPRAY, Spells::Class::BLASTING, true, true, 1));

        character.Spells.push_back(Spells::Base(Spells::Type::NIGHTHOWL, Spells::Class::PSYCHIC, true, true, 1, 5));

        character.Spells.push_back(Spells::Base(Spells::Type::WHITE_FIRE, Spells::Class::BLASTING, true, true, 1));

        character.Spells.push_back(Spells::Base(Spells::Type::SWORDTHRUST, Spells::Class::BLASTING, true, true, 2));

        character.Spells.push_back(Spells::Base(Spells::Type::EYE_OF_THE_TIGER, Spells::Class::PSYCHIC, true, true, 2, 5));

        character.Spells.push_back(Spells::Base(Spells::Type::IMMEDIATE_DELIVERANCE, Spells::Class::PSYCHIC, true, true, 2));

        character.Spells.push_back(Spells::Base(Spells::Type::MISTS_OF_DEATH, Spells::Class::PSYCHIC, true, true, 3));

        character.Spells.push_back(Spells::Base(Spells::Type::THE_VAMPIRE_SPELL, Spells::Class::PSYCHIC, true, true, 3));

        character.Spells.push_back(Spells::Base(Spells::Type::PILLAR_OF_SALT, Spells::Class::PSYCHIC, true, true, 3, 5));

        character.Spells.push_back(Spells::Base(Spells::Type::SHEET_LIGHTNING, Spells::Class::BLASTING, true, true, 4));

        character.Spells.push_back(Spells::Base(Spells::Type::GHASTLY_TOUCH, Spells::Class::PSYCHIC, true, false, 4));

        character.Spells.push_back(Spells::Base(Spells::Type::NEMESIS_BOLT, Spells::Class::BLASTING, true, true, 5));

        character.Spells.push_back(Spells::Base(Spells::Type::SERVILE_ENTHRALMENT, Spells::Class::PSYCHIC, true, true, 5));

        character.Spells.push_back(Spells::Base(Spells::Type::SUMMON_FALTYN, Spells::Class::PSYCHIC, false, false, 1));

        character.Spells.push_back(Spells::Base(Spells::Type::PREDICTION, Spells::Class::PSYCHIC, false, false, 1));

        character.Spells.push_back(Spells::Base(Spells::Type::DETECT_ENCHANTMENT, Spells::Class::PSYCHIC, false, false, 1));
    }

    // set character spells
    void Spells(Character::Base &character)
    {
        if (character.Has(Skills::Type::SPELLS))
        {
            switch (character.Class)
            {
            case Character::Class::ENCHANTER:

                Generate::EnchanterSpells(character);

                break;

            default:

                break;
            }
        }
    }

    // set character attributes based on class
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

        character.Attributes.push_back(Attribute::Base(Attribute::Type::ARMOUR, 0, 0));
    }

    // calculate rank from experience
    int CalculateRankFromExperience(int experience)
    {
        auto rank = (experience / 250) + 1;

        rank = std::min(20, rank);

        rank = std::max(1, rank);

        return rank;
    }

    // set character rank based on experience points
    void Rank(Character::Base &character)
    {
        // set rank based on experience points and enforce bounds
        character.Rank = Generate::CalculateRankFromExperience(character.Experience);
    }

    // calculate base experience from rank
    int CalculateExperienceFromRank(int rank)
    {
        auto filtered_rank = std::min(20, rank);

        filtered_rank = std::max(1, filtered_rank);

        return (filtered_rank - 1) * 250;
    }

    // set character experience points based on rank
    void Experience(Character::Base &character)
    {
        // enforce rank bounds and set starting experience points
        character.Experience = Generate::CalculateExperienceFromRank(character.Rank);
    }

    // warrior's starting items
    void WarriorItems(Character::Base &warrior)
    {
        if (warrior.Rank <= 8)
        {
            warrior.Add(Item::Base("SWORD", Item::Type::SWORD, {Item::Property::WEAPON, Item::Property::PRIMARY, Item::Property::EQUIPPED}, Item::Type::NONE, 1, 1, Asset::Map("SWORD")));

            warrior.Add(Item::Base("CHAINMAIL", Item::Type::CHAINMAIL, {{Attribute::Type::ARMOUR, 3}}, {Item::Property::ARMOUR, Item::Property::EQUIPPED}, 1, 1, Asset::Map("CHAINMAIL")));
        }
    }

    // trickster's starting items
    void TricksterItems(Character::Base &trickster)
    {
        if (trickster.Rank <= 8)
        {
            trickster.Add(Item::Base("SWORD", Item::Type::SWORD, {Item::Property::WEAPON, Item::Property::PRIMARY, Item::Property::EQUIPPED}, Item::Type::NONE, 1, 1, Asset::Map("SWORD")));

            trickster.Add(Item::Base("BOW", Item::Type::BOW, {Item::Property::WEAPON, Item::Property::RANGED, Item::Property::EQUIPPED}, Item::Type::NONE, 1, 1, Asset::Map("ARCHERY")));

            trickster.Add(Item::Base("QUIVER", Item::Type::QUIVER, {Item::Property::CONTAINER, Item::Property::CANNOT_DROP, Item::Property::CANNOT_TRADE}, Item::Type::ARROW, 6, Item::Unlimited, Asset::Map("QUIVER")));

            trickster.Add(Item::Base("LEATHER ARMOUR", Item::Type::LEATHER_ARMOUR, {{Attribute::Type::ARMOUR, 2}}, {Item::Property::ARMOUR, Item::Property::EQUIPPED}, 1, 1, Asset::Map("LEATHER ARMOUR")));
        }
    }

    // sage's starting items
    void SageItems(Character::Base &sage)
    {
        if (sage.Rank <= 8)
        {
            sage.Add(Item::Base("QUARTERSTAFF", Item::Type::QUARTERSTAFF, {Item::Property::WEAPON, Item::Property::PRIMARY, Item::Property::EQUIPPED}, Item::Type::NONE, 1, 1, Asset::Map("QUARTERSTAFF")));

            sage.Add(Item::Base("BOW", Item::Type::BOW, {Item::Property::WEAPON, Item::Property::RANGED, Item::Property::EQUIPPED}, Item::Type::NONE, 1, 1, Asset::Map("ARCHERY")));

            sage.Add(Item::Base("QUIVER", Item::Type::QUIVER, {Item::Property::CONTAINER, Item::Property::CANNOT_DROP, Item::Property::CANNOT_TRADE}, Item::Type::ARROW, 6, Item::Unlimited, Asset::Map("QUIVER")));

            sage.Add(Item::Base("RINGMAIL", Item::Type::RINGMAIL, {{Attribute::Type::ARMOUR, 2}}, {Item::Property::ARMOUR, Item::Property::EQUIPPED}, 1, 1, Asset::Map("RINGMAIL")));
        }
    }

    // enchanter's starting items
    void EnchanterItems(Character::Base &enchanter)
    {
        if (enchanter.Rank <= 8)
        {
            enchanter.Add(Item::Base("SWORD", Item::Type::SWORD, {Item::Property::WEAPON, Item::Property::PRIMARY, Item::Property::EQUIPPED}, Item::Type::NONE, 1, 1, Asset::Map("ARCHERY")));

            enchanter.Add(Item::Base("SILVER ARMOUR", Item::Type::SILVER_ARMOUR, {{Attribute::Type::ARMOUR, 2}}, {Item::Property::ARMOUR, Item::Property::EQUIPPED}, 1, 1, Asset::Map("SILVER ARMOUR")));
        }
    }

    // set starting items
    void Items(Character::Base &character)
    {
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

    // set starting gold
    void Money(Character::Base &character)
    {
        character.Rank = std::min(20, character.Rank);

        character.Rank = std::max(1, character.Rank);

        character.Add(Item::Base("MONEY POUCH", Item::Type::POUCH, {Item::Property::CONTAINER, Item::Property::CANNOT_DROP, Item::Property::CANNOT_TRADE}, Item::Type::GOLD, character.Rank * 5, Item::Unlimited, Asset::Map("MONEY")));
    }

    // set character asset
    void Assets(Character::Base &character)
    {
        switch (character.Class)
        {
        case Character::Class::WARRIOR:

            character.Asset = Asset::Map("WARRIOR");

            break;

        case Character::Class::TRICKSTER:

            character.Asset = Asset::Map("TRICKSTER");

            break;

        case Character::Class::SAGE:

            character.Asset = Asset::Map("SAGE");

            break;

        case Character::Class::ENCHANTER:

            character.Asset = Asset::Map("ENCHANTER");

            break;

        default:

            character.Asset = Asset::Map("PERSON");

            break;
        }
    }

    // clear character definition
    void Clear(Character::Base &character)
    {
        // clear attributes and skills
        character.Attributes.clear();

        character.Skills.clear();

        character.Status.clear();

        character.Spells.clear();

        character.Status.clear();

        character.Items.clear();

        character.Targets.clear();

        character.CalledToMind.clear();

        character.SkillImmunity.clear();

        character.SpellImmunity.clear();
    }

    // set fight / shoot modes
    void Combat(Character::Base &character)
    {
        switch (character.Class)
        {
        case Character::Class::TRICKSTER:

            character.Shoot = Skills::Type::ARCHERY;

            break;

        case Character::Class::SAGE:

            character.Shoot = Skills::Type::ARCHERY;

            break;

        default:

            break;
        }
    }

    void Target(Character::Base &character)
    {
        switch (character.Class)
        {
        case Character::Class::WARRIOR:

            character.Target = Target::Type::WARRIOR;

            break;

        case Character::Class::TRICKSTER:

            character.Target = Target::Type::TRICKSTER;

            break;

        case Character::Class::SAGE:

            character.Target = Target::Type::SAGE;

            break;

        case Character::Class::ENCHANTER:

            character.Target = Target::Type::ENCHANTER;

            break;

        default:

            break;
        }
    }

    // generate character based on class and rank
    Character::Base Character(Character::Class character_class, int rank)
    {
        auto character = Character::Base(character_class, rank);

        Generate::Experience(character);

        Generate::Combat(character);

        Generate::Attributes(character);

        Generate::Skills(character);

        Generate::Spells(character);

        Generate::Items(character);

        Generate::Money(character);

        Generate::Assets(character);

        Generate::Target(character);

        return character;
    }

    Character::Base NPC(const char *name, Skills::Type fight, Skills::Type shoot, Skills::List skills, Attributes::List attributes, int moves, Asset::Type asset)
    {
        auto character = Character::Base(name, attributes, Character::ControlType::NPC, skills, moves);

        character.Fight = fight;

        character.Shoot = shoot;

        character.Asset = asset;

        return character;
    }

    // generate NPC
    Character::Base NPC(const char *name, Skills::List skills, Attributes::List attributes, int moves, Asset::Type asset)
    {
        return NPC(name, Skills::Type::NONE, Skills::Type::NONE, skills, attributes, moves, asset);
    }

    // generate NPC
    Character::Base NPC(const char *name, Skills::List skills, int fpr, int psy, int awr, int end, int arm, int dmg_v, int dmg_m, int moves, Asset::Type asset)
    {
        auto FPR = Attribute::Base(Attribute::Type::FIGHTING_PROWESS, fpr, 0);

        auto PSY = Attribute::Base(Attribute::Type::PSYCHIC_ABILITY, psy, 0);

        auto AWR = Attribute::Base(Attribute::Type::AWARENESS, awr, 0);

        auto END = Attribute::Base(Attribute::Type::ENDURANCE, end, 0);

        auto ARM = Attribute::Base(Attribute::Type::ARMOUR, 0, arm);

        auto DMG = Attribute::Base(Attribute::Type::DAMAGE, dmg_v, dmg_m);

        return Generate::NPC(name, skills, {FPR, PSY, AWR, END, ARM, DMG}, moves, asset);
    }

    Character::Base NPC(const char *name, Skills::Type fight, Skills::Type shoot, Skills::List skills, int fpr, int psy, int awr, int end, int arm, int dmg_v, int dmg_m, int moves, Asset::Type asset)
    {
        auto npc = Generate::NPC(name, skills, fpr, psy, awr, end, arm, dmg_v, dmg_m, moves, asset);

        npc.Fight = fight;

        npc.Shoot = shoot;

        return npc;
    }

    Character::Base NPC(const char *name, Skills::Type fight, Skills::Type shoot, Skills::List skills, Target::List targets, int fpr, int psy, int awr, int end, int arm, int dmg_v, int dmg_m, int moves, Asset::Type asset)
    {
        auto npc = Generate::NPC(name, fight, shoot, skills, fpr, psy, awr, end, arm, dmg_v, dmg_m, moves, asset);

        npc.Targets = targets;

        return npc;
    }

    Character::Base NPC(const char *name, Skills::Type fight, Skills::Type shoot, Target::Type target, Skills::List skills, Target::List targets, int fpr, int psy, int awr, int end, int arm, int dmg_v, int dmg_m, int moves, Asset::Type asset)
    {
        auto npc = Generate::NPC(name, fight, shoot, skills, fpr, psy, awr, end, arm, dmg_v, dmg_m, moves, asset);

        npc.Target = target;

        npc.Targets = targets;

        return npc;
    }
}

#endif
