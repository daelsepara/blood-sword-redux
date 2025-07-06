#ifndef __ROGUE_BATTLE_HPP__
#define __ROGUE_BATTLE_HPP__

#include "RogueBase.hpp"

namespace BloodSword::Rogue
{
    void RenderCombatant(Scene::Base &scene, SDL_Texture *texture, int x, int y, Uint32 color)
    {
        scene.Add(Scene::Element(x, y, BloodSword::Width(texture), BloodSword::Height(texture), Color::Background, color, BloodSword::Border));

        scene.VerifyAndAdd(Scene::Element(texture, Point(x, y)));
    }

    void RenderCombatants(Graphics::Base &graphics, Scene::Base &scene, Party::Base &party, Textures &party_stats, Party::Base &enemies, BloodSword::Textures &enemy_stats, int stats_w, bool is_player, bool is_enemy, int id, Uint32 color)
    {
        // estimate positions from window
        auto window_h = BloodSword::QuarterTile * 18 - BloodSword::Pad;

        auto window_y = (graphics.Height - window_h) / 2;

        auto pad = BloodSword::SmallPad;

        auto enemy_offset = (graphics.Width - (enemies.Count() * stats_w + (enemies.Count() - 1) * pad)) / 2;

        auto enemy_h = BloodSword::Height(enemy_stats);

        auto enemy_y = window_y - (BloodSword::TileSize + enemy_h + BloodSword::Pad * 2);

        for (auto i = 0; i < enemies.Count(); i++)
        {
            if (!is_enemy || id != i)
            {
                auto enemy_x = enemy_offset + i * (stats_w + pad);

                Rogue::RenderCombatant(scene, enemy_stats[i], enemy_x, enemy_y, Color::Inactive);
            }
        }

        if (is_enemy)
        {
            auto enemy_x = enemy_offset + id * (stats_w + pad);

            Rogue::RenderCombatant(scene, enemy_stats[id], enemy_x, enemy_y, color);
        }

        auto party_offset = (graphics.Width - (party.Count() * stats_w + (party.Count() - 1) * pad)) / 2;

        auto party_y = window_y + window_h + BloodSword::TileSize + BloodSword::Pad;

        for (auto i = 0; i < party.Count(); i++)
        {
            if (!is_player || id != i)
            {
                auto party_x = party_offset + i * (stats_w + pad);

                Rogue::RenderCombatant(scene, party_stats[i], party_x, party_y, Color::Inactive);
            }
        }

        if (is_player)
        {
            auto party_x = party_offset + id * (stats_w + pad);

            Rogue::RenderCombatant(scene, party_stats[id], party_x, party_y, color);
        }
    }

    Controls::Type SelectAction(Graphics::Base &graphics, Scene::Base &background, Character::Base &character)
    {
        auto selected = Controls::Type::NONE;

        Asset::List assets = {Asset::Type::DEFEND};

        Controls::List controls = {Controls::Type::DEFEND};

        auto captions = std::vector<std::string>{"DEFEND"};

        // character not in MELEE area
        if (character.Has(Character::Status::MELEE))
        {
            assets.push_back(Asset::Type::MOVE);

            controls.push_back(Controls::Type::MOVE);

            captions.push_back("MOVE TO RANGED");

            assets.push_back(Asset::Type::FIGHT);

            controls.push_back(Controls::Type::FIGHT);

            captions.push_back("FIGHT");

            // has quarterstaff skill
            if (character.Has(Skills::Type::QUARTERSTAFF) && character.IsArmed(Item::Type::QUARTERSTAFF))
            {
                assets.push_back(Asset::Type::QUARTERSTAFF);

                controls.push_back(Controls::Type::QUARTERSTAFF);

                captions.push_back("QUARTERSTAFF");
            }
        }

        if (character.Has(Character::Status::RANGED))
        {
            assets.push_back(Asset::Type::MOVE);

            controls.push_back(Controls::Type::MOVE);

            captions.push_back("MOVE TO MELEE");

            if (Engine::CanShoot(character))
            {
                // can shoot
                if (character.Shoot == Skills::Type::SHURIKEN)
                {
                    assets.push_back(Asset::Type::SHURIKEN);

                    controls.push_back(Controls::Type::SHURIKEN);

                    captions.push_back("SHURIKEN");
                }
                else
                {
                    assets.push_back(Asset::Type::SHOOT);

                    controls.push_back(Controls::Type::SHOOT);

                    captions.push_back("SHOOT");
                }
            }
        }

        if (character.Has(Skills::Type::SPELLS))
        {
            assets.push_back(Asset::Type::SPELLS);

            controls.push_back(Controls::Type::SPELLS);

            captions.push_back("SPELLS");
        }

        if (character.Items.size() > 0)
        {
            assets.push_back(Asset::Type::ITEMS);

            controls.push_back(Controls::Type::ITEMS);

            captions.push_back("ITEMS");
        }

        assets.push_back(Asset::Type::EXIT);

        controls.push_back(Controls::Type::EXIT);

        captions.push_back("EXIT");

        auto values = std::vector<int>(controls.size());

        std::iota(values.begin(), values.end(), 0);

        auto action = character.Name + ": SELECT ACTION";

        auto selection = Interface::SelectIcons(graphics, background, action.c_str(), assets, values, captions, 1, 1, Asset::Type::NONE, false, true, false);

        if (selection.size() == 1)
        {
            selected = controls[selection[0]];
        }

        return selected;
    }

    void Move(Character::Base &character, Character::Status move)
    {
        switch (move)
        {
        case Character::Status::MELEE:

            character.Remove(Character::Status::RANGED);

            character.Add(Character::Status::MELEE);

            break;

        case Character::Status::RANGED:

            character.Remove(Character::Status::MELEE);

            character.Add(Character::Status::RANGED);

            break;

        default:

            character.Remove(Character::Status::MELEE);

            character.Add(Character::Status::RANGED);
        }
    }

    void RefreshStats(Graphics::Base &graphics, Textures &textures, Character::Base &character, int id, int stats_w)
    {
        BloodSword::Free(&textures[id]);

        textures[id] = Rogue::Stats(graphics, character, stats_w);
    }

    // shoot action
    bool Shoot(Graphics::Base &graphics, Scene::Base &background, Character::Base &attacker, Character::Base &defender, Skills::Type shot, Asset::Type asset)
    {
        Input::Flush();

        auto alive = true;

        auto window_w = BloodSword::OctaTile + BloodSword::HalfTile;

        auto window_h = BloodSword::QuarterTile * 18 - BloodSword::Pad;

        auto window = (Point(graphics.Width, graphics.Height) - Point(window_w, window_h)) / 2;

        if (!attacker.Is(Character::Status::DEFENDING) && Engine::IsAlive(attacker))
        {
            auto roll = defender.Is(Character::Status::DEFENDING) ? 3 : 2;

            roll += attacker.Has(Character::Status::NIGHTHOWL) ? 1 : 0;

            auto modifier = defender.Has(Skills::Type::DODGING) ? 1 : 0;

            if (Interface::Target(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, attacker, defender.Asset, Attribute::Type::FIGHTING_PROWESS, roll, modifier, asset, true, Item::Property::RANGED, false))
            {
                Input::Flush();

                auto hit = Interface::CombatDamage(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, attacker, defender, shot, asset, true, attacker.Has(Skills::Type::IGNORE_ARMOUR), false);

                if (hit > 0 && defender.Has(Character::Status::TEMPORARY_INVULNERABILITY))
                {
                    Interface::FlashMessage(graphics, background, "INVULNERABLE: NO DAMAGE INFLICTED!", defender.IsPlayer() ? Color::Active : Color::Highlight);

                    defender.Remove(Character::Status::TEMPORARY_INVULNERABILITY);

                    hit = 0;
                }

                if (hit > 0)
                {
                    if (defender.Has(Character::Status::WEAKENED))
                    {
                        Interface::FlashMessage(graphics, background, "WEAKENED: +1 DAMAGE", defender.IsPlayer() ? Color::Highlight : Color::Active);

                        hit++;
                    }

                    alive &= Engine::GainEndurance(defender, -hit, true);

                    if (shot == Skills::Type::POISONED_DAGGER)
                    {
                        // instant death
                        defender.Value(Attribute::Type::ENDURANCE, 0);

                        alive = false;
                    }
                }
            }
        }

        return alive;
    }

    // shoot helper
    void Shoot(Graphics::Base &graphics, Scene::Base &background, Character::Base &attacker, Character::Base &defender, int defenderid)
    {
        Input::Flush();

        auto asset = Engine::CanShoot(attacker) ? Skills::Assets[attacker.Shoot] : Asset::Type::SHOOT;

        auto alive = Rogue::Shoot(graphics, background, attacker, defender, attacker.Shoot, asset);

        if (Engine::CanShoot(attacker) && attacker.Shoot == Skills::Type::ARCHERY)
        {
            if (attacker.IsArmed(Item::Type::BOW, Item::Requirements(Item::Type::BOW, true)))
            {
                attacker.Remove(Item::Requirements(Item::Type::BOW, true), 1);
            }
            else if (attacker.IsArmed(Item::Type::MAGIC_BOW, Item::Requirements(Item::Type::MAGIC_BOW, true)))
            {
                attacker.Remove(Item::Requirements(Item::Type::MAGIC_BOW, true), 1);
            }
        }
        else if (Engine::CanShoot(attacker) && attacker.Shoot == Skills::Type::SHURIKEN)
        {
            if (attacker.IsArmed(Item::Type::LIMITED_SHURIKEN, Item::Requirements(Item::Type::LIMITED_SHURIKEN, true)))
            {
                attacker.Remove(Item::Requirements(Item::Type::LIMITED_SHURIKEN, true), 1);
            }
        }

        if (!alive)
        {
            Interface::FlashMessage(graphics, background, defender.Name + " KILLED!", defender.IsPlayer() ? Color::Highlight : Color::Active);
        }
    }

    // fight action
    bool Fight(Graphics::Base &graphics, Scene::Base &background, Character::Base &attacker, Character::Base &defender, Skills::Type skill)
    {
        Input::Flush();

        auto alive = true;

        auto window_w = BloodSword::OctaTile + BloodSword::HalfTile;

        auto window_h = BloodSword::QuarterTile * 18 - BloodSword::Pad;

        auto window = (Point(graphics.Width, graphics.Height) - Point(window_w, window_h)) / 2;

        auto knockout = (skill == Skills::Type::QUARTERSTAFF) && attacker.Has(Skills::Type::QUARTERSTAFF);

        auto unarmed = (skill != Skills::Type::ARCHERY) && attacker.IsPlayer() && !attacker.IsArmed();

        auto asset = Asset::Type::NONE;

        if (knockout)
        {
            asset = Asset::Type::QUARTERSTAFF;
        }
        else if (unarmed)
        {
            asset = Asset::Type::UNARMED_COMBAT;
        }
        else if (skill != Skills::Type::NONE)
        {
            asset = Skills::Assets[skill];
        }
        else
        {
            asset = Asset::Type::FIGHT;
        }

        if (!attacker.Is(Character::Status::DEFENDING) && Engine::IsAlive(attacker))
        {
            auto roll = 2;

            roll += defender.Is(Character::Status::DEFENDING) ? 1 : 0;

            roll += knockout ? 1 : 0;

            roll += attacker.Has(Character::Status::NIGHTHOWL) ? 1 : 0;

            auto modifier = defender.Has(Skills::Type::DODGING) ? 1 : 0;

            if (Interface::Target(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, attacker, defender.Asset, Attribute::Type::FIGHTING_PROWESS, roll, modifier, asset, true, Item::Property::PRIMARY, false))
            {
                Input::Flush();

                auto hit = Interface::CombatDamage(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, attacker, defender, skill, asset, true, attacker.Has(Skills::Type::IGNORE_ARMOUR), false);

                if (attacker.Has(Character::Status::STRONG))
                {
                    auto rolls = Engine::Roll(1);

                    std::string strong_damage = std::string("STRONG: +") + std::to_string(rolls.Sum) + " DAMAGE";

                    Interface::FlashMessage(graphics, background, strong_damage, attacker.IsPlayer() ? Color::Active : Color::Highlight);

                    hit += rolls.Sum;
                }

                if (hit > 0 && defender.Has(Character::Status::TEMPORARY_INVULNERABILITY))
                {
                    Interface::FlashMessage(graphics, background, "INVULNERABLE: NO DAMAGE INFLICTED!", defender.IsPlayer() ? Color::Active : Color::Highlight);

                    defender.Remove(Character::Status::TEMPORARY_INVULNERABILITY);

                    hit = 0;
                }

                if (hit > 0)
                {
                    if (defender.Has(Character::Status::WEAKENED))
                    {
                        Interface::FlashMessage(graphics, background, "WEAKENED: +1 DAMAGE", defender.IsPlayer() ? Color::Highlight : Color::Active);

                        hit++;
                    }

                    alive &= Engine::GainEndurance(defender, -hit, true);

                    auto effect = BloodSword::Find(Interface::SkillEffects, skill);

                    // process attacks which do not apply an efect first
                    if (alive && skill == Skills::Type::POISONED_BITE)
                    {
                        Input::Flush();

                        auto bite = Interface::Roll(graphics, background, defender.Asset, Asset::Type::FANGS, 1, 0).Sum;

                        if (bite == 6)
                        {
                            Interface::FlashMessage(graphics, background, "POISON FLOWS INTO THE WOUND!", defender.IsPlayer() ? Color::Highlight : Color::Active);

                            auto venom = Interface::Roll(graphics, background, defender.Asset, Asset::Type::DAMAGE, 3, 0).Sum;

                            alive &= Engine::GainEndurance(defender, -venom, true);
                        }
                        else
                        {
                            Interface::FlashMessage(graphics, background, "POISON RESISTED!", defender.IsPlayer() ? Color::Active : Color::Highlight);
                        }
                    }
                    else if (alive && effect != Character::Status::NONE && !defender.IsImmune(skill) && !defender.Has(effect))
                    {
                        Input::Flush();

                        // do not stack up effects
                        auto resisted = false;

                        if (skill == Skills::Type::PARALYZING_TOUCH)
                        {
                            if (Interface::Test(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, defender, Attribute::Type::PSYCHIC_ABILITY, 2, 0, Attribute::Assets[Attribute::Type::PSYCHIC_ABILITY], true, false))
                            {
                                Interface::FlashMessage(graphics, background, "PARALYZING TOUCH RESISTED!", defender.IsPlayer() ? Color::Active : Color::Highlight);

                                resisted = true;
                            }
                        }
                        else if (skill == Skills::Type::POISONED_DAGGER)
                        {
                            defender.Value(Attribute::Type::ENDURANCE, 0);

                            alive = false;
                        }

                        if (!resisted)
                        {
                            defender.Add(effect);

                            Interface::FlashMessage(graphics, background, Character::StatusMapping[effect], Color::Active);
                        }
                    }
                    else if (alive && defender.Has(Skills::Type::RETRIBUTIVE_FIRE) && attacker.IsArmed())
                    {
                        // check if defender has retributive fire
                        Interface::FlashMessage(graphics, background, attacker.Name + ": -1 ENDURANCE (RETRIBUTIVE FIRE)", attacker.IsPlayer() ? Color::Highlight : Color::Active);

                        Engine::GainEndurance(attacker, -1, true);
                    }
                }
            }
        }

        return alive;
    }

    void ResolveFight(Graphics::Base &graphics, Scene::Base &background, Character::Base &attacker, Character::Base &defender)
    {
        auto alive = Rogue::Fight(graphics, background, attacker, defender, attacker.Fight);

        if (!alive)
        {
            Interface::FlashMessage(graphics, background, defender.Name + " KILLED!", defender.IsPlayer() ? Color::Highlight : Color::Active);
        }
        else if (!defender.Is(Character::Status::KNOCKED_OUT) && !defender.Is(Character::Status::DEFENDING))
        {
            Rogue::Fight(graphics, background, defender, attacker, defender.Fight);

            if (!Engine::IsAlive(attacker))
            {
                Interface::FlashMessage(graphics, background, attacker.Name + " KILLED!", attacker.IsPlayer() ? Color::Highlight : Color::Active);
            }
        }
    }

    void ResolveSpell(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Base &caster, Character::Base &target, int targetid, Spells::Type spell)
    {
        Input::Flush();

        auto alive = true;

        auto popup_w = BloodSword::OctaTile + BloodSword::HalfTile;

        auto popup_h = BloodSword::QuarterTile * 18 - BloodSword::Pad;

        auto popup = (Point(graphics.Width, graphics.Height) - Point(popup_w, popup_h)) / 2;

        auto affected = target.Name + " SUCCUMBS TO " + std::string(Spells::TypeMapping[spell]);

        auto resisted = target.Name + " RESISTS " + std::string(Spells::TypeMapping[spell]);

        if (spell == Spells::Type::VOLCANO_SPRAY)
        {
            auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true, false);

            alive &= Engine::GainEndurance(target, -hit, true);
        }
        else if (spell == Spells::Type::WHITE_FIRE)
        {
            auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true, false);

            alive &= Engine::GainEndurance(target, -hit, true);
        }
        else if (spell == Spells::Type::SWORDTHRUST)
        {
            auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true, false);

            alive &= Engine::GainEndurance(target, -hit, true);
        }
        else if (spell == Spells::Type::NEMESIS_BOLT)
        {
            auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true, false);

            alive &= Engine::GainEndurance(target, -hit, true);
        }
        else if (spell == Spells::Type::NIGHTHOWL)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, target, Attribute::Type::PSYCHIC_ABILITY, Spells::Difficulty[spell], Spells::DifficultyModifier[spell], Spells::Assets[spell], true, false))
            {
                Input::Flush();

                Interface::FlashMessage(graphics, background, affected, target.IsEnemy() ? Color::Highlight : Color::Active);

                target.Add(Character::Status::NIGHTHOWL);
            }
            else
            {
                Interface::FlashMessage(graphics, background, resisted, target.IsEnemy() ? Color::Highlight : Color::Active);
            }
        }
        else if (spell == Spells::Type::MISTS_OF_DEATH)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, target, Attribute::Type::PSYCHIC_ABILITY, Spells::Difficulty[spell], Spells::DifficultyModifier[spell], Spells::Assets[spell], true, false))
            {
                Input::Flush();

                Interface::FlashMessage(graphics, background, affected, target.IsEnemy() ? Color::Active : Color::Highlight);

                auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true, true, false);

                alive &= Engine::GainEndurance(target, -hit, true);
            }
            else
            {
                Interface::FlashMessage(graphics, background, resisted, target.IsEnemy() ? Color::Highlight : Color::Active);
            }
        }
        else if (spell == Spells::Type::THE_VAMPIRE_SPELL)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, target, Attribute::Type::PSYCHIC_ABILITY, Spells::Difficulty[spell], Spells::DifficultyModifier[spell], Spells::Assets[spell], true, false))
            {
                Input::Flush();

                Interface::FlashMessage(graphics, background, affected, target.IsEnemy() ? Color::Active : Color::Highlight);

                auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true, true, false);

                alive &= Engine::GainEndurance(target, -hit, true);

                // caster gains endurance
                Engine::GainEndurance(caster, std::abs(hit / 2), true);
            }
            else
            {
                Interface::FlashMessage(graphics, background, resisted, target.IsEnemy() ? Color::Highlight : Color::Active);
            }
        }
        else if (spell == Spells::Type::GHASTLY_TOUCH)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, target, Attribute::Type::PSYCHIC_ABILITY, Spells::Difficulty[spell], Spells::DifficultyModifier[spell], Spells::Assets[spell], true, false))
            {
                Input::Flush();

                Interface::FlashMessage(graphics, background, affected, target.IsEnemy() ? Color::Active : Color::Highlight);

                auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true, true, false);

                alive &= Engine::GainEndurance(target, -hit, true);
            }
            else
            {
                Input::Flush();

                Interface::FlashMessage(graphics, background, resisted, target.IsEnemy() ? Color::Highlight : Color::Active);

                auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::AlternateDamage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true, true, false);

                alive &= Engine::GainEndurance(target, -hit, true);
            }
        }
        else if (spell == Spells::Type::SHEET_LIGHTNING)
        {
            Input::Flush();

            auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true, false);

            alive &= Engine::GainEndurance(target, -hit, true);
        }

        if (!alive)
        {
            Interface::FlashMessage(graphics, background, target.Name + " KILLED!", target.IsPlayer() ? Color::Highlight : Color::Active);
        }
    }

    // resolve spell with multiple targets
    void ResolveSpell(Graphics::Base &graphics, Scene::Base &background, Character::Base &caster, Party::Base &targets, Spells::Type spell)
    {
        auto spellbook = caster.Find(spell);

        if (spellbook != caster.Spells.end() && spellbook->MultipleTargets())
        {
            for (auto target = 0; target < targets.Count(); target++)
            {
                if (Engine::IsAlive(targets[target]))
                {
                    Rogue::ResolveSpell(graphics, background, targets, caster, targets[target], target, spell);
                }
            }
        }
        else if (spell == Spells::Type::EYE_OF_THE_TIGER)
        {
            Graphics::TextList tiger_eye =
                {Graphics::RichText("PLAYER FPR/DMG ROLLS +2", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                 Graphics::RichText(" PARTY FPR/DMG ROLLS +1", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0)};

            auto popup = (Point(graphics.Width, graphics.Height) - Point(BloodSword::OctaTile, (BloodSword::DoubleTile + BloodSword::QuarterTile - BloodSword::SmallPad))) / 2;

            auto tiger = Interface::Choice(graphics, background, tiger_eye, popup, BloodSword::OctaTile, BloodSword::TileSize, 2, Color::Background, Color::Inactive, Color::Active, true);

            if (tiger == 0)
            {
                caster.Add(Character::Status::FPR_PLUS2);
            }
            else if (tiger == 1)
            {
                for (auto character = 0; character < targets.Count(); character++)
                {
                    if (Engine::IsAlive(targets[character]))
                    {
                        targets[character].Add(Character::Status::FPR_PLUS1);
                    }
                }
            }
        }
    }

    int SelectTarget(Graphics::Base &graphics, Scene::Base &scene, Party::Base &party, Textures &party_stats, Party::Base &enemies, BloodSword::Textures &enemy_stats, int stats_w, bool is_player, bool is_enemy, int id)
    {
        // estimate positions from window
        auto window_h = BloodSword::QuarterTile * 18 - BloodSword::Pad;

        auto window_y = (graphics.Height - window_h) / 2;

        int target = -1;

        auto texture = Graphics::CreateText(graphics, "SELECT TARGET", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto pad = BloodSword::SmallPad;

        auto max_selection = is_player ? enemies.Count() : party.Count();

        auto stats_h = is_player ? BloodSword::Height(enemy_stats) : BloodSword::Height(party_stats);

        auto offset = (graphics.Width - (max_selection * stats_w + (max_selection - 1) * pad)) / 2;

        auto target_y = is_player ? (window_y - (BloodSword::TileSize + stats_h + BloodSword::Pad * 2)) : (window_y + window_h + BloodSword::TileSize + BloodSword::Pad);

        auto selection = is_player ? Engine::First(enemies) : Engine::First(party);

        auto done = false;

        while (!done)
        {
            auto scene = Scene::Base();

            // render all combatants
            Rogue::RenderCombatants(graphics, scene, party, party_stats, enemies, enemy_stats, stats_w, is_player, is_enemy, id, Color::Active);

            // render label
            Interface::Boxed(graphics, scene, texture, Color::Background, Color::Active, BloodSword::Border);

            // calculate target texture x-coordinate
            auto target_x = offset + selection * (stats_w + pad);

            // render target
            Rogue::RenderCombatant(scene, is_player ? enemy_stats[selection] : party_stats[selection], target_x, target_y, Color::Highlight);

            // calculate pointer x-coordinate
            auto pointer_x = offset + selection * (stats_w + pad) + (stats_w - BloodSword::TileSize) / 2;

            // // calculate pointer y-coordinate, render pointer
            if (is_player)
            {
                auto pointer_y = window_y - (BloodSword::TileSize + BloodSword::Pad);

                scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::UP), pointer_x, pointer_y));
            }
            else
            {
                auto pointer_y = window_y + window_h + BloodSword::Pad;

                scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::DOWN), pointer_x, pointer_y));
            }

            auto input = Input::RogueInput(graphics, {scene});

            if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
            {
                if (input.Type == Controls::Type::LEFT)
                {
                    if (selection > 0)
                    {
                        selection--;
                    }
                }
                else if (input.Type == Controls::Type::RIGHT)
                {
                    if (selection < max_selection - 1)
                    {
                        selection++;
                    }
                }
                else if (input.Type == Controls::Type::UP || input.Type == Controls::Type::DOWN)
                {
                    Sound::Play(Sound::Type::ERROR);
                }
                else if (input.Type == Controls::Type::ACTION)
                {
                    if (selection >= 0 && selection < max_selection)
                    {
                        target = selection;

                        done = true;
                    }
                    else
                    {
                        Sound::Play(Sound::Type::ERROR);
                    }
                }
                else if (input.Type == Controls::Type::BACK)
                {
                    done = true;
                }

                input.Selected = false;
            }
        }

        BloodSword::Free(&texture);

        return target;
    }

    Rogue::Update Battle(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, int enemy)
    {
        Rogue::Update update = {false, false, false};

        auto &party = rogue.Party;

        auto &enemies = rogue.Opponents[enemy];

        auto saved_party = party;

        auto saved_enemies = enemies;

        // set "IN BATTLE" status
        party.Add({Character::Status::IN_BATTLE, Character::Status::RANGED});

        enemies.Add({Character::Status::IN_BATTLE, Character::Status::RANGED});

        auto stats_w = BloodSword::TileSize * 3 + BloodSword::HalfTile;

        auto enemy_stats = Rogue::Stats(graphics, enemies, stats_w);

        auto party_stats = Rogue::Stats(graphics, party, stats_w);

        auto round = 0;

        auto exit_battle = false;

        while (Engine::Count(party) > 0 && Engine::Count(enemies) > 0 && Engine::InBattle(party) > 0 && !exit_battle)
        {
            auto battle_order = Engine::Build(party, enemies, Attribute::Type::AWARENESS, true, true);

            auto combatant = 0;

            auto next_round = false;

            while (!next_round && Engine::IsAlive(party) && Engine::IsAlive(enemies, Character::ControlType::NPC) && !exit_battle)
            {
                auto is_enemy = Engine::IsEnemy(battle_order, combatant);

                auto is_player = Engine::IsPlayer(battle_order, combatant);

                auto character_id = battle_order[combatant].Id;

                auto &character = is_player ? party[character_id] : enemies[character_id];

                if (round > 0 && Engine::CoolDown(character))
                {
                    if (is_enemy)
                    {
                        Rogue::RefreshStats(graphics, enemy_stats, character, character_id, stats_w);
                    }
                    else if (is_player)
                    {
                        Rogue::RefreshStats(graphics, party_stats, character, character_id, stats_w);
                    }
                }

                auto scene = Scene::Base();

                Rogue::RenderCombatants(graphics, scene, party, party_stats, enemies, enemy_stats, stats_w, is_player, is_enemy, character_id, Color::Active);

                auto end_turn = false;

                while (!end_turn && Engine::IsAlive(party) && Engine::IsAlive(enemies, Character::ControlType::NPC) && !exit_battle)
                {
                    auto has_actions = !character.Is(Character::Status::PARALYZED) && Engine::IsAlive(character);

                    if (is_enemy)
                    {
                        if (has_actions)
                        {
                            if (character.Has(Character::Status::MELEE))
                            {
                                auto targets = Engine::Build(party, Attribute::Type::ENDURANCE, Character::Status::MELEE, true, true);

                                if (targets.size() == 0)
                                {
                                    targets = Engine::Build(party, Attribute::Type::ENDURANCE, true, true);
                                }

                                if (targets.size() > 0)
                                {
                                    auto target = 0;

                                    if (Engine::Percentile.NextInt() < 50 && targets.size() > 1)
                                    {
                                        target = Engine::Percentile.NextInt(0, targets.size() - 1);
                                    }

                                    auto defender_id = targets[target].Id;

                                    auto &defender = ((targets[target].Type == Character::ControlType::PLAYER) ? party[defender_id] : enemies[defender_id]);

                                    if (defender.Has(Character::Status::MELEE))
                                    {
                                        Interface::FlashMessage(graphics, scene, character.Name + " ATTACKS " + defender.Name, Color::Active);
                                    }
                                    else if (defender.Has(Character::Status::RANGED))
                                    {
                                        Interface::FlashMessage(graphics, scene, character.Name + " RUSHES TO ATTACK " + defender.Name, Color::Active);
                                    }

                                    Rogue::ResolveFight(graphics, scene, character, defender);

                                    Rogue::RefreshStats(graphics, enemy_stats, character, character_id, stats_w);

                                    Rogue::RefreshStats(graphics, party_stats, defender, defender_id, stats_w);
                                }
                            }
                            else if (character.Has(Character::Status::RANGED) && Engine::CanShoot(character) && Engine::Count(party) > 0)
                            {
                                auto targets = Engine::Build(party, Attribute::Type::ENDURANCE, true, true);

                                if (targets.size() > 0)
                                {
                                    auto target = 0;

                                    if (Engine::Percentile.NextInt() < 50 && targets.size() > 1)
                                    {
                                        target = Engine::Percentile.NextInt(0, targets.size() - 1);
                                    }

                                    auto defender_id = targets[target].Id;

                                    auto &defender = ((targets[target].Type == Character::ControlType::PLAYER) ? party[defender_id] : enemies[defender_id]);

                                    Interface::FlashMessage(graphics, scene, character.Name + " SHOOTS AT " + defender.Name, Color::Active);

                                    Rogue::Shoot(graphics, scene, character, defender, defender_id);

                                    Rogue::RefreshStats(graphics, party_stats, defender, defender_id, stats_w);
                                }
                            }
                            else if (!character.Has(Character::Status::MELEE) && !Engine::CanShoot(character) && !character.Has(Skills::Type::SPELLS))
                            {
                                Rogue::Move(character, Character::Status::MELEE);

                                Interface::FlashMessage(graphics, scene, character.Name + " MOVES TO MELEE RANGE", Color::Active);

                                Rogue::RefreshStats(graphics, enemy_stats, character, character_id, stats_w);
                            }
                        }

                        end_turn = true;
                    }
                    else
                    {
                        // skip action if dead or paralyzed
                        if (has_actions)
                        {
                            auto input = Rogue::SelectAction(graphics, scene, character);

                            if (input != Controls::Type::NONE)
                            {
                                if (input == Controls::Type::EXIT)
                                {
                                    exit_battle = true;
                                }
                                else if (input == Controls::Type::MOVE)
                                {
                                    Rogue::Move(character, character.Has(Character::Status::RANGED) ? Character::Status::MELEE : Character::Status::RANGED);

                                    Rogue::RefreshStats(graphics, party_stats, character, character_id, stats_w);

                                    end_turn = true;
                                }
                                else if (input == Controls::Type::DEFEND)
                                {
                                    character.Add(Character::Status::DEFENDING);

                                    Engine::ResetStatusAndSpells(character);

                                    Rogue::RefreshStats(graphics, party_stats, character, character_id, stats_w);

                                    end_turn = true;
                                }
                                else if (input == Controls::Type::FIGHT)
                                {
                                    if (character.Has(Character::Status::MELEE))
                                    {
                                        auto defender_id = enemies.Count() > 1 ? Rogue::SelectTarget(graphics, scene, party, party_stats, enemies, enemy_stats, stats_w, is_player, is_enemy, character_id) : Engine::First(enemies);

                                        if (defender_id >= 0 && defender_id < enemies.Count() && Engine::IsAlive(enemies[defender_id]))
                                        {
                                            auto &defender = enemies[defender_id];

                                            if (defender.Has(Character::Status::MELEE))
                                            {
                                                Interface::FlashMessage(graphics, scene, character.Name + " ATTACKS " + defender.Name, Color::Active);
                                            }
                                            else if (defender.Has(Character::Status::RANGED))
                                            {
                                                Interface::FlashMessage(graphics, scene, character.Name + " RUSHES TO ATTACK " + defender.Name, Color::Active);
                                            }

                                            Rogue::ResolveFight(graphics, scene, character, defender);

                                            Rogue::RefreshStats(graphics, party_stats, character, character_id, stats_w);

                                            Rogue::RefreshStats(graphics, enemy_stats, defender, defender_id, stats_w);

                                            end_turn = true;
                                        }
                                    }
                                    else
                                    {
                                        Input::Flush();

                                        Sound::Play(Sound::Type::ERROR);

                                        Interface::FlashMessage(graphics, scene, "CANNOT ATTACK FROM CURRENT POSITION!", Color::Highlight);
                                    }
                                }
                                else if (input == Controls::Type::SHOOT)
                                {
                                    if (character.Has(Character::Status::RANGED))
                                    {
                                        auto defender_id = enemies.Count() > 1 ? Rogue::SelectTarget(graphics, scene, party, party_stats, enemies, enemy_stats, stats_w, is_player, is_enemy, character_id) : Engine::First(enemies);

                                        if (defender_id >= 0 && defender_id < enemies.Count() && Engine::IsAlive(enemies[defender_id]))
                                        {
                                            auto &defender = enemies[defender_id];

                                            Rogue::Shoot(graphics, scene, character, defender, defender_id);

                                            Rogue::RefreshStats(graphics, party_stats, character, character_id, stats_w);

                                            Rogue::RefreshStats(graphics, enemy_stats, defender, defender_id, stats_w);

                                            end_turn = true;
                                        }
                                    }
                                    else
                                    {
                                        Input::Flush();

                                        Sound::Play(Sound::Type::ERROR);

                                        Interface::FlashMessage(graphics, scene, "CANNOT SHOOT FROM CURRENT POSITION!", Color::Highlight);
                                    }
                                }
                                else if (input == Controls::Type::SPELLS)
                                {
                                    auto spell = Interface::GetSpell(graphics, scene, character, Color::Background, Color::Active, BloodSword::Border, true);

                                    if (spell != Spells::Type::NONE)
                                    {
                                        if (!Spells::IsBasic(spell) && spell != Spells::Type::PILLAR_OF_SALT && spell != Spells::Type::IMMEDIATE_DELIVERANCE && spell != Spells::Type::SERVILE_ENTHRALMENT)
                                        {
                                            if (character.HasCalledToMind(spell))
                                            {
                                                // cast spell
                                                auto spellbook = character.Find(spell);

                                                if (spellbook != character.Spells.end())
                                                {
                                                    if (Spells::RequiresTarget(spell))
                                                    {
                                                        auto defender_id = enemies.Count() > 1 ? Rogue::SelectTarget(graphics, scene, party, party_stats, enemies, enemy_stats, stats_w, is_player, is_enemy, character_id) : Engine::First(enemies);

                                                        if (defender_id >= 0 && defender_id < enemies.Count() && Engine::IsAlive(enemies[defender_id]))
                                                        {
                                                            auto &defender = enemies[defender_id];

                                                            if (!spellbook->Ranged && (!character.Has(Character::Status::MELEE) || !defender.Has(Character::Status::MELEE)))
                                                            {
                                                                Interface::FlashMessage(graphics, scene, Interface::GetText(Interface::MSG_ADJACENT), Color::Highlight);
                                                            }
                                                            else if (spellbook->Ranged)
                                                            {
                                                                Interface::FlashMessage(graphics, scene, character.Name + " CASTING " + std::string(Spells::TypeMapping[spell]), Color::Active);

                                                                Input::Flush();

                                                                if (Interface::Cast(graphics, scene, character, defender.Asset, spell, true, false))
                                                                {
                                                                    // spellcasting successful
                                                                    Interface::FlashMessage(graphics, scene, std::string(Spells::TypeMapping[spell]) + " SUCCESSFULLY CAST", Color::Active);

                                                                    // resolve spell
                                                                    Rogue::ResolveSpell(graphics, scene, enemies, character, defender, defender_id, spell);

                                                                    Rogue::RefreshStats(graphics, party_stats, character, character_id, stats_w);

                                                                    Rogue::RefreshStats(graphics, enemy_stats, defender, defender_id, stats_w);
                                                                }
                                                                else
                                                                {
                                                                    // spellcasting unsuccessful!
                                                                    Interface::FlashMessage(graphics, scene, Interface::GetText(Interface::MSG_CAST), Color::Highlight);
                                                                }

                                                                end_turn = true;
                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        Interface::FlashMessage(graphics, scene, character.Name + " CASTING " + std::string(Spells::TypeMapping[spell]), Color::Active);

                                                        Input::Flush();

                                                        if (Interface::Cast(graphics, scene, character, spell, true, false))
                                                        {
                                                            Interface::FlashMessage(graphics, scene, std::string(Spells::TypeMapping[spell]) + " SUCCESSFULLY CAST", Color::Active);

                                                            auto my_party = (spell == Spells::Type::EYE_OF_THE_TIGER);

                                                            Rogue::ResolveSpell(graphics, scene, character, my_party ? party : enemies, spell);

                                                            if (my_party)
                                                            {
                                                                BloodSword::Free(party_stats);

                                                                party_stats = Rogue::Stats(graphics, party, stats_w);
                                                            }
                                                            else
                                                            {
                                                                BloodSword::Free(enemy_stats);

                                                                enemy_stats = Rogue::Stats(graphics, enemies, stats_w);
                                                            }
                                                        }
                                                        else
                                                        {
                                                            Interface::FlashMessage(graphics, scene, Interface::GetText(Interface::MSG_CAST), Color::Highlight);
                                                        }

                                                        end_turn = true;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                // call spell to mind
                                                character.CallToMind(spell);

                                                end_turn = true;
                                            }
                                        }
                                        else
                                        {
                                            Input::Flush();

                                            Sound::Play(Sound::Type::ERROR);

                                            Interface::FlashMessage(graphics, scene, "YOU CANNOT CAST THIS SPELL!", Color::Highlight);
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            end_turn = true;
                        }
                    }

                    if (end_turn)
                    {
                        next_round = Engine::NextInQueue(battle_order, combatant);
                    }
                }
            }

            round++;
        }

        BloodSword::Free(party_stats);

        BloodSword::Free(enemy_stats);

        if (exit_battle)
        {
            party = saved_party;

            enemies = saved_enemies;
        }
        else
        {
            if (!Engine::IsAlive(party))
            {
                Engine::KillAllParalyzed(party);
            }

            // reset status
            Engine::ResetAll(party);

            Engine::ResetAll(enemies);
        }

        update.Scene = true;

        update.Party = true;

        update.Quit = false;

        return update;
    }
}

#endif