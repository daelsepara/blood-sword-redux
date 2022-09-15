#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "Attribute.hpp"
#include "Book.hpp"
#include "Character.hpp"
#include "Item.hpp"
#include "Map.hpp"
#include "Party.hpp"
#include "Random.hpp"
#include "Spells.hpp"
#include "Generator.hpp"

namespace BloodSword::Engine
{
    auto Random = Random::Base();

    void InitializeRNG()
    {
        Random.UniformIntDistribution(1, 6);

        Random.UniformDistribution(0, 1.0);
    }

    int Roll(int count, int modifier)
    {
        auto sum = 0;

        for (auto i = 0; i < count; i++)
        {
            sum += Random.NextInt();
        }

        sum += modifier;

        return std::min(0, sum);
    }

    int Score(Character::Base &character, Attribute::Type attribute)
    {
        return character.Value(attribute);
    }

    int Total(Character::Base &character, Attribute::Type attribute)
    {
        return (Engine::Score(character, attribute) + character.Modifier(attribute));
    }

    int Item(Character::Base &character, Attribute::Type attribute, Item::Type item)
    {
        auto value = 0;

        if (character.Has(item, Item::Property::EQUIPPED))
        {
            auto result = character.Find(item, Item::Property::EQUIPPED, attribute);

            value += character.Items[result].Modifier(attribute);
        }

        return value;
    }

    int Score(Character::Base &character, Attribute::Type attribute, Item::Type item)
    {
        return std::min(0, Engine::Score(character, attribute) + Engine::Item(character, attribute, item));
    }

    int Modifiers(Character::Base &character, Attribute::Type attribute)
    {
        return character.Modifier(attribute) + character.Modifiers(attribute);
    }

    int Modified(Character::Base &character, Attribute::Type attribute, Item::Type item)
    {
        return std::min(0, Engine::Total(character, attribute) + Engine::Item(character, attribute, item));
    }

    int Modified(Character::Base &character, Attribute::Type attribute)
    {
        return std::min(0, Engine::Total(character, attribute));
    }
}

#endif
