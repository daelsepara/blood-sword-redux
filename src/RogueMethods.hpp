#ifndef __ROGUE_METHODS_HPP__
#define __ROGUE_METHODS_HPP__

#include "RogueBase.hpp"

namespace BloodSword::Rogue
{
    // items
    Rogue::Update UseItem(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Character::Class character_class, int id);
    Rogue::Update ManageItem(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Character::Class character_class, int id);
    Rogue::Update ShowInventory(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Character::Base &character);
    void ManageItem(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Items::Inventory &items, int id);
    void ManageInventory(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Character::Base &character, bool blur);
    void ShowLoot(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Point point);

    // battle
    Controls::Type SelectAction(Graphics::Base &graphics, Scene::Base &background, Character::Base &character);
    Rogue::Update Battle(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, int enemy);
    void RefreshStats(Graphics::Base &graphics, Textures &textures, Character::Base &character, int id, int stats_width);
    void ResolveFight(Graphics::Base &graphics, Scene::Base &background, Character::Base &attacker, Character::Base &defender);
    void ResolveShoot(Graphics::Base &graphics, Scene::Base &background, Character::Base &attacker, Character::Base &defender, int defenderid);
    void ResolveSpell(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Base &caster, Character::Base &target, int targetid, Spells::Type spell);
    void ResolveSpell(Graphics::Base &graphics, Scene::Base &background, Character::Base &caster, Party::Base &targets, Spells::Type spell);
    void Move(Character::Base &character, Character::Status move);
    bool Fight(Graphics::Base &graphics, Scene::Base &background, Character::Base &attacker, Character::Base &defender, Skills::Type skill);
    bool Shoot(Graphics::Base &graphics, Scene::Base &background, Character::Base &attacker, Character::Base &defender, Skills::Type shot, Asset::Type asset);
}

#endif
