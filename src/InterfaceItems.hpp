#ifndef __INTERFACE_ITEMS_HPP__
#define __INTERFACE_ITEMS_HPP__

#include "Section.hpp"
#include "Interface.hpp"

namespace BloodSword::Interface
{
    void TransferItem(Graphics::Base &graphics, Scene::Base &background, std::string message, Uint32 border, Items::Inventory &destination, Items::Inventory &source, int id)
    {
        if (id >= 0 && id < source.size())
        {
            Interface::MessageBox(graphics, background, message, Color::Active);

            if (source[id].Is(Item::Property::EQUIPPED))
            {
                source[id].Remove(Item::Property::EQUIPPED);
            }

            Items::Add(destination, source[id]);

            source.erase(source.begin() + id);
        }
    }

    // DROP
    bool DropItem(Graphics::Base &graphics, Scene::Base &background, std::string action, Items::Inventory &destination, Items::Inventory &items, int id)
    {
        auto result = false;

        if (id >= 0 && id <= items.size())
        {
            std::string message = action + " THE " + items[id].Name;

            // if equipped, then un-equip first then drop
            if (items[id].Has(Item::Property::EQUIPPED))
            {
                items[id].Remove(Item::Property::EQUIPPED);
            }

            Interface::TransferItem(graphics, background, message, Color::Active, destination, items, id);

            result = true;
        }

        return result;
    }

    // GIVE / TAKE
    bool TransferItem(Graphics::Base &graphics, Scene::Base &background, Character::Base &receiver, Items::Inventory &source, int id)
    {
        auto result = false;

        if (!Engine::IsAlive(receiver))
        {
            std::string message = Engine::IsDead(receiver);

            Interface::MessageBox(graphics, background, receiver.Name, Color::Highlight);
        }
        else if (source[id].Type == Item::Type::GOLD)
        {
            std::string message = receiver.Name + " TAKES THE " + std::to_string(source[id].Quantity) + " " + source[id].Name;

            Interface::MessageBox(graphics, background, message, Color::Active);

            receiver.Add(source[id].Type, source[id].Quantity);

            source.erase(source.begin() + id);

            result = true;
        }
        else if (Interface::CheckItemLimit(receiver) && (receiver.Items.size() < receiver.ItemLimit || receiver.ItemLimit == Items::Unlimited))
        {
            std::string message = receiver.Name + " TAKES THE " + source[id].Name;

            Interface::TransferItem(graphics, background, message, Color::Active, receiver.Items, source, id);

            result = true;
        }
        else
        {
            std::string message = receiver.Name + "'S INVENTORY IS FULL!";

            Interface::MessageBox(graphics, background, message, Color::Highlight);
        }

        return result;
    }

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

            // remove
            character.Items.erase(character.Items.begin() + id);
        }
        else if (item.Type == Item::Type::FOOD)
        {
            if (!Engine::Healed(character))
            {
                Interface::MessageBox(graphics, background, "HEALED 1 ENDURANCE", Color::Active);

                Engine::GainEndurance(character, 1);

                // remove
                character.Items.erase(character.Items.begin() + id);
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