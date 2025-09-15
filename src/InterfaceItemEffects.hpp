#ifndef __INTERFACE_ITEM_EFFECTS_HPP__
#define __INTERFACE_ITEM_EFFECTS_HPP__

#include "Interface.hpp"

namespace BloodSword::Interface
{
    // removes item from inventory
    void ConsumeItem(Character::Base &character, int item_id)
    {
        character.Items.erase(character.Items.begin() + item_id);
    }

    // individual item effects
    void ProcessEffects(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Base &character, int item_id)
    {
        if (!Engine::IsAlive(character))
        {
            Interface::MessageBox(graphics, background, Engine::IsDead(character), Color::Highlight);

            return;
        }

        auto &item = character.Items[item_id];

        if (item.Has(Item::Property::CURSED))
        {
            auto cursed_item = item;

            // cursed items destroy others items
            character.Items = {cursed_item};
        }
        else if (item.Type == Item::Type::MAUVE_LIQUID)
        {
            auto result = Interface::Roll(graphics, background, character.Asset, Asset::Map("DRINK"), 1, 0).Sum;

            if (result)
            {
                // increase stats
                character.Modifier(Attribute::Type::FIGHTING_PROWESS, character.Modifier(Attribute::Type::FIGHTING_PROWESS) + 1);

                character.Modifier(Attribute::Type::AWARENESS, character.Modifier(Attribute::Type::AWARENESS) + 1);

                // decrease stats
                character.Modifier(Attribute::Type::PSYCHIC_ABILITY, character.Modifier(Attribute::Type::PSYCHIC_ABILITY) - 1);

                character.Modifier(Attribute::Type::ENDURANCE, character.Modifier(Attribute::Type::ENDURANCE) - 1);

                std::string message = "FIGHTING PROWESS: +1\nAWARENESS: +1\nPSYCHIC ABILITY: -1\nENDURANCE: -1";

                Interface::TextBox(graphics, background, message.c_str(), (BloodSword::PentaTile));
            }
            else
            {
                auto message = "Your heart bursts under the strain of the drug you have imbibed";

                Interface::MessageBox(graphics, background, message, Color::Highlight);

                character.Value(Attribute::Type::ENDURANCE, 0);
            }

            Interface::ConsumeItem(character, item_id);
        }
        else if (item.Type == Item::Type::BLACK_LIQUID)
        {
            auto message = Engine::IsDead(character);

            Interface::MessageBox(graphics, background, message, Color::Highlight);

            character.Value(Attribute::Type::ENDURANCE, 0);

            Interface::ConsumeItem(character, item_id);
        }
        else if (item.Type == Item::Type::FOOD)
        {
            if (!Engine::Healed(character))
            {
                Interface::MessageBox(graphics, background, "HEALED 1 ENDURANCE", Color::Active);

                Engine::GainEndurance(character, 1);

                Interface::ConsumeItem(character, item_id);
            }
            else
            {
                std::string message = character.Name + " IS AT MAXIMUM ENDURANCE";

                Interface::MessageBox(graphics, background, message, Color::Inactive);
            }
        }
        else if (item.Type == Item::Type::SCROLL_HEALING)
        {
            if (!Engine::Healed(character))
            {
                auto rolls = Engine::Roll(2);

                std::string message = "HEALED " + std::to_string(rolls.Sum) + " ENDURANCE";

                Interface::MessageBox(graphics, background, message, Color::Active);

                Engine::GainEndurance(character, rolls.Sum);

                Interface::ConsumeItem(character, item_id);
            }
            else
            {
                std::string message = character.Name + " IS AT MAXIMUM ENDURANCE";

                Interface::MessageBox(graphics, background, message, Color::Inactive);
            }
        }
        else if (item.Type == Item::Type::POTION_OF_HEALING)
        {
            if (!Engine::Healed(character))
            {
                auto rolls = Engine::Roll(2);

                std::string message = "HEALED " + std::to_string(rolls.Sum) + " ENDURANCE";

                Interface::MessageBox(graphics, background, message, Color::Active);

                Engine::GainEndurance(character, rolls.Sum);

                Interface::ConsumeItem(character, item_id);
            }
            else
            {
                std::string message = character.Name + " IS AT MAXIMUM ENDURANCE";

                Interface::MessageBox(graphics, background, message, Color::Inactive);
            }
        }
        else if (item.Type == Item::Type::SCROLL_ADJUST)
        {
            Interface::PermanentAttributeGain(graphics, background, character, 1, -1);

            Interface::PermanentAttributeGain(graphics, background, character, 1, 1);

            Interface::ConsumeItem(character, item_id);
        }
        else if (item.Type == Item::Type::CHIMERA_SPITTLE)
        {
            if (!character.Has(Character::Status::DYING_SLOWLY))
            {
                character.Add(Character::Status::DYING_SLOWLY);

                character.DelayedEffects[Character::Status::DYING_SLOWLY] = character.Items[item_id].Type;
            }

            Interface::ConsumeItem(character, item_id);
        }
        else if (item.Type == Item::Type::EFFERVESCENT_LIQUID)
        {
            if (!character.Has(Character::Status::IMMUNE_DYING_SLOWLY))
            {
                character.Add(Character::Status::IMMUNE_DYING_SLOWLY);
            }

            Interface::ConsumeItem(character, item_id);
        }
        else if (item.Type == Item::Type::BIRCH_BARK_SCROLL)
        {
            for (auto character = 0; character < party.Count(); character++)
            {
                if (Engine::IsAlive(party[character]))
                {
                    if (!Interface::Test(graphics, background, party[character], Attribute::Type::PSYCHIC_ABILITY))
                    {
                        Interface::MessageBox(graphics, background, party[character].Name + " SUCCUMBS TO DREADFUL PUNITION", Color::Highlight);

                        Engine::GainEndurance(party[character], -8, true);
                    }
                    else
                    {
                        Interface::MessageBox(graphics, background, party[character].Name + " RESISTS DREADFUL PUNITION", Color::Active);
                    }
                }
            }

            Interface::ConsumeItem(character, item_id);
        }
        else if (item.Type == Item::Type::SCROLL_PRECOGNITION)
        {
            auto rolls = Interface::Roll(graphics, background, character.Asset, Asset::Map("PRECOGNITION"), 3, 0);

            auto number = rolls.Rolls[0] * 100 + rolls.Rolls[1] * 10 + rolls.Rolls[2];

            auto book = party.Location.first;

            Book::Location location = {book, number};

            auto random = Story::CurrentBook.Find(location);

            if (random >= 0 && random < Story::CurrentBook.Sections.size())
            {
                auto &section = Story::CurrentBook.Sections[random];

                auto text = Engine::MapTokens(party, section.Text);

                if (text.empty())
                {
                    text = "You see nothing of the past nor the future.";
                }

                Interface::TextBox(graphics, background, text, Color::Active, BloodSword::DescriptionSize);
            }

            Interface::ConsumeItem(character, item_id);
        }
    }

    void ItemEffects(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Base &character, Item::Type item)
    {
        if (Engine::IsAlive(character))
        {
            if (item != Item::Type::NONE && character.Has(item))
            {
                auto item_id = -1;

                // search for id in inventory
                for (auto i = 0; i < character.Items.size(); i++)
                {
                    if (character.Items[i].Type == item)
                    {
                        item_id = i;

                        break;
                    }
                }

                if (item_id != Item::NotFound)
                {
                    if (Book::IsDefined(character.Items[item_id].Effects) && !Book::Equal(character.Location, character.Items[item_id].Effects))
                    {
                        Interface::ShowBookDescription(graphics, background, character.Items[item_id].Effects);
                    }
                    else if (Items::FoundDescription(character.Items[item_id].Type))
                    {
                        Interface::ShowBookDescription(graphics, background, character.Items[item_id].Type);
                    }

                    // process effects
                    Interface::ProcessEffects(graphics, background, party, character, item_id);
                }
                else
                {
                    std::string message = "Internal Error: ITEM EFFECTS";

                    Interface::InternalError(graphics, background, message);
                }
            }
            else if (item != Item::Type::NONE)
            {
                std::string message = "YOU DO NOT HAVE THE " + std::string(Item::TypeMapping[item]) + "!";

                Interface::MessageBox(graphics, background, message, Color::Highlight);
            }
        }

        // display death message
        if (!Engine::IsAlive(character))
        {
            Interface::ErrorMessage(graphics, background, MSG_DIED);
        }
    }
}

#endif
