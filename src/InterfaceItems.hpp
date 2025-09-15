#ifndef __INTERFACE_ITEMS_HPP__
#define __INTERFACE_ITEMS_HPP__

#include "InterfaceItemEffects.hpp"

// item interface functions
namespace BloodSword::Interface
{
    // transfer (drop/give/take) item between inventories
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

    // drop item from inventory to ground (or section)
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

    // give / take item between characters
    bool TransferItem(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Base &receiver, Items::Inventory &source, int id)
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
        else if (source[id].Type == Item::Type::ARROW)
        {
            if (receiver.Has(Item::Container(Item::Type::ARROW)))
            {
                std::string message = receiver.Name + " TAKES THE " + std::to_string(source[id].Quantity) + " " + source[id].Name;

                Interface::MessageBox(graphics, background, message, Color::Active);

                receiver.Add(source[id].Type, source[id].Quantity);

                source.erase(source.begin() + id);

                result = true;
            }
            else
            {
                std::string quiver = receiver.Name + " DOES NOT HAVE A QUIVER";

                Interface::MessageBox(graphics, background, quiver, Color::Highlight);
            }
        }
        else if (Interface::CanReceive(receiver))
        {
            std::string message = receiver.Name + " TAKES THE " + source[id].Name;

            Interface::TransferItem(graphics, background, message, Color::Active, receiver.Items, source, id);

            // process CURSED items
            for (auto i = 0; i < receiver.Items.size(); i++)
            {
                if (receiver.Items[i].Has(Item::Property::CURSED))
                {
                    Interface::ItemEffects(graphics, background, party, receiver, receiver.Items[i].Type);
                }
            }

            result = true;
        }
        else
        {
            std::string message = receiver.Name + "'S INVENTORY IS FULL!";

            Interface::MessageBox(graphics, background, message, Color::Highlight);
        }

        return result;
    }
}

#endif
