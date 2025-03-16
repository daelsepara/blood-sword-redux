#ifndef __ITEM_EFFECTS_HPP__
#define __ITEM_EFFECTS_HPP__

#include "Interface.hpp"

namespace BloodSword::Interface
{
    // removes item from inventory
    void ConsumeItem(Character::Base &character, int id)
    {
        character.Items.erase(character.Items.begin() + id);
    }

    // individual item effects
    void ProcessEffects(Graphics::Base &graphics, Scene::Base &background, Character::Base &character, int id)
    {
        auto &item = character.Items[id];

        auto ether = Items::Inventory();

        if (item.Type == Item::Type::MAUVE_LIQUID)
        {
            auto result = Interface::Roll(graphics, background, character.Asset, Asset::Type::DRINK, 1, 0);

            if (result > 2)
            {
                // increase stats
                character.Modifier(Attribute::Type::FIGHTING_PROWESS, character.Modifier(Attribute::Type::FIGHTING_PROWESS) + 1);

                character.Modifier(Attribute::Type::AWARENESS, character.Modifier(Attribute::Type::AWARENESS) + 1);

                // decrease stats
                character.Modifier(Attribute::Type::PSYCHIC_ABILITY, character.Modifier(Attribute::Type::PSYCHIC_ABILITY) - 1);

                character.Modifier(Attribute::Type::ENDURANCE, character.Modifier(Attribute::Type::ENDURANCE) - 1);

                std::string message = "FIGHTING PROWESS: +1\nAWARENESS: +1\nPSYCHIC ABILITY: -1\nENDURANCE: -1";

                Interface::TextBox(graphics, background, message.c_str(), (BloodSword::TileSize * 5));
            }
            else
            {
                auto message = "Your heart bursts under the strain of the drug you have imbibed";

                Interface::MessageBox(graphics, background, message, Color::Highlight);

                character.Value(Attribute::Type::ENDURANCE, 0);
            }

            Interface::ConsumeItem(character, id);
        }
        else if (item.Type == Item::Type::FOOD)
        {
            if (!Engine::Healed(character))
            {
                Interface::MessageBox(graphics, background, "HEALED 1 ENDURANCE", Color::Active);

                Engine::GainEndurance(character, 1);

                Interface::ConsumeItem(character, id);
            }
            else
            {
                std::string message = character.Name + " IS AT MAXIMUM ENDURANCE";

                Interface::MessageBox(graphics, background, message, Color::Inactive);
            }
        }
    }

    void ItemEffects(Graphics::Base &graphics, Scene::Base &background, Character::Base &character, Item::Type item)
    {
        if (Engine::IsAlive(character))
        {
            if (item != Item::Type::NONE && character.Has(item))
            {
                auto id = -1;

                for (auto i = 0; i < character.Items.size(); i++)
                {
                    if (character.Items[i].Type == item)
                    {
                        id = i;

                        break;
                    }
                }

                if (id != -1)
                {
                    if (Book::IsDefined(character.Items[id].Effects) && !Book::Equal(character.Location, character.Items[id].Effects))
                    {
                        // show text description
                        auto description = Story::CurrentBook.Find(character.Items[id].Effects);

                        if (description >= 0 && description < Story::CurrentBook.Sections.size() && !Story::CurrentBook.Sections[description].Text.empty())
                        {
                            auto item_description = Story::CurrentBook.Sections[description].Text;

                            auto wrap = graphics.Width - BloodSword::TileSize * 8;

                            Interface::TextBox(graphics, background, item_description, wrap);
                        }
                    }

                    // process effects
                    Interface::ProcessEffects(graphics, background, character, id);
                }
                else
                {
                    std::string message = "INTERNAL ERROR: ITEM EFFECTS";

                    Interface::InternalError(graphics, background, message);
                }
            }
            else if (item != Item::Type::NONE)
            {
                std::string message = "YOU DO NOT HAVE THE " + std::string(Item::TypeMapping[item]) + "!";

                Interface::MessageBox(graphics, background, message, Color::Highlight);
            }
        }
        else
        {
            Interface::ErrorMessage(graphics, background, MSG_DIED);
        }

        // display death message
        if (!Engine::IsAlive(character))
        {
            Interface::ErrorMessage(graphics, background, MSG_DIED);
        }
    }
}

#endif
