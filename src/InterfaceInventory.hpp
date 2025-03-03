#ifndef __INTERFACE_INVENTORY_HPP__
#define __INTERFACE_INVENTORY_HPP__

#include "Graphics.hpp"
#include "Interface.hpp"
#include "Section.hpp"

// inventory management
namespace BloodSword::Interface
{
    void ManageInventory(Graphics::Base &graphics, Scene::Base &background, Story::Base &story, Character::Base &character, bool blur = false)
    {
        if (!Engine::IsAlive(character))
        {
            Interface::MessageBox(graphics, background, Engine::IsDead(character), Color::Highlight);
        }
        else
        {
            Interface::MessageBox(graphics, background, "NOT IMPLEMENTED YET", Color::Active);
        }
    }

    void PartyInventory(Graphics::Base &graphics, Scene::Base &background, Story::Base &story, Party::Base &party, bool blur = false)
    {
        if (!Engine::IsAlive(party))
        {
            Interface::ErrorMessage(graphics, background, MSG_DEAD);
        }
        else
        {
            auto character = Engine::FirstClass(party);

            if (Engine::Count(party) > 1)
            {
                character = Interface::SelectCharacter(graphics, background, party, "SELECT PLAYER TO VIEW INVENTORY", true, false);
            }

            if (character != Character::Class::NONE)
            {
                Interface::ManageInventory(graphics, background, story, party[character], blur);
            }
        }
    }
}

#endif