#ifndef __INTERFACE_BATTLE_HPP__
#define __INTERFACE_BATTLE_HPP__

#include "InterfaceBattleItems.hpp"

// battle engine
namespace BloodSword::Interface
{
    // helper function (next character in battle order)
    bool NextCharacter(Battle::Base &battle, Scene::Base &scene, Party::Base &party, Engine::Queue &order, int &combatant, Controls::User &input, bool &end_turn)
    {
        auto next_round = Engine::NextInQueue(order, combatant);

        auto character = order[combatant].Id;

        if (Engine::IsPlayer(order, combatant))
        {
            input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[character].Class]);
        }
        else if (Engine::IsEnemy(order, combatant) && battle.Opponents[character].Is(Character::Status::ENTHRALLED))
        {
            input.Current = Interface::Find(battle.Map, scene.Controls, Controls::Type::ENEMY, character);
        }

        end_turn = true;

        return next_round;
    }

    // fight action
    bool ResolveFight(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Character::Base &attacker, Character::Base &defender, Skills::Type skill)
    {
        auto alive = true;

        auto window_w = BloodSword::OctaTile + BloodSword::HalfTile;

        auto window_h = BloodSword::WindowTile - BloodSword::Pad;

        auto window = origin + (Point(w, h) - Point(window_w, window_h)) / 2;

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

            if (Interface::Target(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, attacker, defender.Asset, Attribute::Type::FIGHTING_PROWESS, roll, modifier, asset, true, Item::Property::PRIMARY))
            {
                auto hit = Interface::CombatDamage(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, attacker, defender, skill, asset, true, attacker.Has(Skills::Type::IGNORE_ARMOUR));

                if (attacker.Has(Character::Status::STRONG))
                {
                    auto rolls = Engine::Roll(1);

                    std::string strong_damage = std::string("STRONG: +") + std::to_string(rolls.Sum) + " DAMAGE";

                    Interface::MessageBox(graphics, background, strong_damage, attacker.IsPlayer() ? Color::Active : Color::Highlight);

                    hit += rolls.Sum;
                }

                if (hit > 0 && defender.Has(Character::Status::TEMPORARY_INVULNERABILITY))
                {
                    Interface::MessageBox(graphics, background, "INVULNERABLE: NO DAMAGE INFLICTED!", defender.IsPlayer() ? Color::Active : Color::Highlight);

                    defender.Remove(Character::Status::TEMPORARY_INVULNERABILITY);

                    hit = 0;
                }

                if (hit > 0)
                {
                    if (defender.Has(Character::Status::WEAKENED))
                    {
                        Interface::MessageBox(graphics, background, "WEAKENED: +1 DAMAGE", defender.IsPlayer() ? Color::Highlight : Color::Active);

                        hit++;
                    }

                    alive &= Engine::GainEndurance(defender, -hit, true);

                    auto effect = BloodSword::Find(Interface::SkillEffects, skill);

                    // process attacks which do not apply an efect first
                    if (alive && skill == Skills::Type::POISONED_BITE)
                    {
                        auto bite = Interface::Roll(graphics, background, defender.Asset, Asset::Type::FANGS, 1, 0).Sum;

                        if (bite == 6)
                        {
                            Interface::MessageBox(graphics, background, "POISON FLOWS INTO THE WOUND!", defender.IsPlayer() ? Color::Highlight : Color::Active);

                            auto venom = Interface::Roll(graphics, background, defender.Asset, Asset::Type::DAMAGE, 3, 0).Sum;

                            alive &= Engine::GainEndurance(defender, -venom, true);
                        }
                        else
                        {
                            Interface::MessageBox(graphics, background, "POISON RESISTED!", defender.IsPlayer() ? Color::Active : Color::Highlight);
                        }
                    }
                    else if (alive && effect != Character::Status::NONE && !defender.IsImmune(skill) && !defender.Has(effect))
                    {
                        // do not stack up effects
                        auto resisted = false;

                        if (skill == Skills::Type::PARALYZING_TOUCH)
                        {
                            if (Interface::Test(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, defender, Attribute::Type::PSYCHIC_ABILITY, 2, 0, Attribute::Assets[Attribute::Type::PSYCHIC_ABILITY], true))
                            {
                                Interface::MessageBox(graphics, background, "PARALYZING TOUCH RESISTED!", defender.IsPlayer() ? Color::Active : Color::Highlight);

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

                            Interface::MessageBox(graphics, background, Character::StatusMapping[effect], Color::Active);
                        }
                    }
                    else if (alive && defender.Has(Skills::Type::RETRIBUTIVE_FIRE) && attacker.IsArmed())
                    {
                        // check if defender has retributive fire
                        Interface::MessageBox(graphics, background, attacker.Name + ": -1 ENDURANCE (RETRIBUTIVE FIRE)", attacker.IsPlayer() ? Color::Highlight : Color::Active);

                        Engine::GainEndurance(attacker, -1, true);
                    }
                }
            }
        }

        return alive;
    }

    // fight helper function
    void Fight(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Party::Base &party, Character::Base &attacker, int attacker_id, Character::Base &defender, int defender_id, Skills::Type melee)
    {
        auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

        auto map_w = battle.Map.ViewX * battle.Map.TileSize;

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        // resolve fight, check if defender dies
        auto alive = Interface::ResolveFight(graphics, background, draw, map_w, map_h, attacker, defender, melee);

        if (!alive)
        {
            Interface::MessageBox(graphics, background, defender.Name + " KILLED!", defender.IsPlayer() ? Color::Highlight : Color::Active);

            Interface::Resurrect(graphics, background, party, defender);

            if (!Engine::IsAlive(defender))
            {
                Interface::DropItemsOnDeath(battle, defender);

                battle.Map.Remove(defender.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, defender_id);
            }
        }
        else if (defender.Is(Character::Status::PARALYZED))
        {
            // paralyzed combatants are removed from combat
            battle.Map.Remove(defender.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, defender_id);
        }
        else if (!defender.Is(Character::Status::KNOCKED_OUT) && !defender.Is(Character::Status::DEFENDING))
        {
            // retaliate only if not knocked out and not defending
            defender.Add(Character::Status::IN_COMBAT);

            // reset alive checks
            Interface::ResolveFight(graphics, background, draw, map_w, map_h, defender, attacker, defender.Fight);

            if (!Engine::IsAlive(attacker))
            {
                Interface::MessageBox(graphics, background, attacker.Name + " KILLED!", attacker.IsPlayer() ? Color::Highlight : Color::Active);

                Interface::Resurrect(graphics, background, party, attacker);

                if (!Engine::IsAlive(attacker))
                {
                    Interface::DropItemsOnDeath(battle, attacker);

                    battle.Map.Remove(attacker.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, attacker_id);
                }
            }
            else if (attacker.Is(Character::Status::PARALYZED))
            {
                // paralyzed combatants are removed from combat
                battle.Map.Remove(attacker.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, attacker_id);
            }
        }
    }

    // shoot action
    bool Shoot(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Character::Base &attacker, Character::Base &defender, Skills::Type shot, Asset::Type asset)
    {
        auto alive = true;

        auto window_w = BloodSword::OctaTile + BloodSword::HalfTile;

        auto window_h = BloodSword::WindowTile - BloodSword::Pad;

        auto window = origin + (Point(w, h) - Point(window_w, window_h)) / 2;

        if (!attacker.Is(Character::Status::DEFENDING) && Engine::IsAlive(attacker))
        {
            auto roll = defender.Is(Character::Status::DEFENDING) ? 3 : 2;

            roll += attacker.Has(Character::Status::NIGHTHOWL) ? 1 : 0;

            auto modifier = defender.Has(Skills::Type::DODGING) ? 1 : 0;

            if (Interface::Target(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, attacker, defender.Asset, Attribute::Type::FIGHTING_PROWESS, roll, modifier, asset, true, Item::Property::RANGED))
            {
                auto hit = Interface::CombatDamage(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, attacker, defender, shot, asset, true, attacker.Has(Skills::Type::IGNORE_ARMOUR));

                if (hit > 0 && defender.Has(Character::Status::TEMPORARY_INVULNERABILITY))
                {
                    Interface::MessageBox(graphics, background, "INVULNERABLE: NO DAMAGE INFLICTED!", defender.IsPlayer() ? Color::Active : Color::Highlight);

                    defender.Remove(Character::Status::TEMPORARY_INVULNERABILITY);

                    hit = 0;
                }

                if (hit > 0)
                {
                    if (defender.Has(Character::Status::WEAKENED))
                    {
                        Interface::MessageBox(graphics, background, "WEAKENED: +1 DAMAGE", defender.IsPlayer() ? Color::Highlight : Color::Active);

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
    void Shoot(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Party::Base &party, Character::Base &attacker, Character::Base &defender, int defenderid)
    {
        auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

        auto map_w = battle.Map.ViewX * battle.Map.TileSize;

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        auto asset = Engine::CanShoot(attacker) ? Skills::Assets[attacker.Shoot] : Asset::Type::SHOOT;

        auto alive = Interface::Shoot(graphics, background, draw, map_w, map_h, attacker, defender, attacker.Shoot, asset);

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

        Interface::Resurrect(graphics, background, party, defender);

        if (!alive)
        {
            Interface::MessageBox(graphics, background, defender.Name + " KILLED!", defender.IsPlayer() ? Color::Highlight : Color::Active);

            Interface::Resurrect(graphics, background, party, defender);

            if (!Engine::IsAlive(defender))
            {
                Interface::DropItemsOnDeath(battle, defender);

                battle.Map.Remove(defender.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, defenderid);
            }
        }
    }

    // checks if enthrallment is broken
    void CheckEnthrallment(Graphics::Base &graphics, Battle::Base &battle, Scene::Base &scene, Character::Base &character, Graphics::RichText &text)
    {
        auto is_enemy = character.IsEnemy();

        // check if enthralment is broken
        if (is_enemy && character.Is(Character::Status::ENTHRALLED))
        {
            auto roll = Engine::Roll(1);

            if (roll.Sum == 6)
            {
                Interface::MessageBox(graphics, scene, text.Text, Color::Highlight);

                Engine::Cancel(character, Character::Status::ENTHRALLED);
            }
        }
    }

    // resolve spell
    void ResolveSpell(Graphics::Base &graphics, Battle::Base &battle, Scene::Base &background, Party::Base &party, Character::Base &caster, Character::Base &target, int targetid, Spells::Type spell)
    {
        auto alive = true;

        auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

        auto map_w = battle.Map.ViewX * battle.Map.TileSize;

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        auto popup_w = BloodSword::OctaTile + BloodSword::HalfTile;

        auto popup_h = BloodSword::WindowTile - BloodSword::Pad;

        auto popup = draw + (Point(map_w, map_h) - Point(popup_w, popup_h)) / 2;

        auto affected = target.Name + " SUCCUMBS TO " + std::string(Spells::TypeMapping[spell]);

        auto resisted = target.Name + " RESISTS " + std::string(Spells::TypeMapping[spell]);

        if (spell == Spells::Type::VOLCANO_SPRAY)
        {
            auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true);

            alive &= Engine::GainEndurance(target, -hit, true);
        }
        else if (spell == Spells::Type::WHITE_FIRE)
        {
            auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true);

            alive &= Engine::GainEndurance(target, -hit, true);
        }
        else if (spell == Spells::Type::SWORDTHRUST)
        {
            auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true);

            alive &= Engine::GainEndurance(target, -hit, true);
        }
        else if (spell == Spells::Type::NEMESIS_BOLT)
        {
            auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true);

            alive &= Engine::GainEndurance(target, -hit, true);
        }
        else if (spell == Spells::Type::NIGHTHOWL)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, target, Attribute::Type::PSYCHIC_ABILITY, Spells::Difficulty[spell], Spells::DifficultyModifier[spell], Spells::Assets[spell], true))
            {
                Interface::MessageBox(graphics, background, affected, target.IsEnemy() ? Color::Highlight : Color::Active);

                target.Add(Character::Status::NIGHTHOWL);
            }
            else
            {
                Interface::MessageBox(graphics, background, resisted, target.IsEnemy() ? Color::Highlight : Color::Active);
            }
        }
        else if (spell == Spells::Type::MISTS_OF_DEATH)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, target, Attribute::Type::PSYCHIC_ABILITY, Spells::Difficulty[spell], Spells::DifficultyModifier[spell], Spells::Assets[spell], true))
            {
                Interface::MessageBox(graphics, background, affected, target.IsEnemy() ? Color::Active : Color::Highlight);

                auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true, true);

                alive &= Engine::GainEndurance(target, -hit, true);
            }
            else
            {
                Interface::MessageBox(graphics, background, resisted, target.IsEnemy() ? Color::Highlight : Color::Active);
            }
        }
        else if (spell == Spells::Type::THE_VAMPIRE_SPELL)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, target, Attribute::Type::PSYCHIC_ABILITY, Spells::Difficulty[spell], Spells::DifficultyModifier[spell], Spells::Assets[spell], true))
            {
                Interface::MessageBox(graphics, background, affected, target.IsEnemy() ? Color::Active : Color::Highlight);

                auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true, true);

                alive &= Engine::GainEndurance(target, -hit, true);

                // caster gains endurance
                Engine::GainEndurance(caster, std::abs(hit / 2), true);
            }
            else
            {
                Interface::MessageBox(graphics, background, resisted, target.IsEnemy() ? Color::Highlight : Color::Active);
            }
        }
        else if (spell == Spells::Type::GHASTLY_TOUCH)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, target, Attribute::Type::PSYCHIC_ABILITY, Spells::Difficulty[spell], Spells::DifficultyModifier[spell], Spells::Assets[spell], true))
            {
                Interface::MessageBox(graphics, background, affected, target.IsEnemy() ? Color::Active : Color::Highlight);

                auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true, true);

                alive &= Engine::GainEndurance(target, -hit, true);
            }
            else
            {
                Interface::MessageBox(graphics, background, resisted, target.IsEnemy() ? Color::Highlight : Color::Active);

                auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::AlternateDamage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true, true);

                alive &= Engine::GainEndurance(target, -hit, true);
            }
        }
        else if (spell == Spells::Type::SHEET_LIGHTNING)
        {
            auto hit = Interface::CombatDamage(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, caster, target, Spells::Damage[spell], Spells::DamageModifier[spell], Spells::Assets[spell], true);

            alive &= Engine::GainEndurance(target, -hit, true);
        }
        else if (spell == Spells::Type::SERVILE_ENTHRALMENT)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, BloodSword::Border, target, Attribute::Type::PSYCHIC_ABILITY, Spells::Difficulty[spell], Spells::DifficultyModifier[spell], Spells::Assets[spell], true))
            {
                Interface::MessageBox(graphics, background, affected, target.IsEnemy() ? Color::Active : Color::Highlight);

                target.Add(Character::Status::ENTHRALLED);
            }
            else
            {
                Interface::MessageBox(graphics, background, resisted, target.IsEnemy() ? Color::Highlight : Color::Active);
            }
        }

        if (!alive)
        {
            Interface::MessageBox(graphics, background, target.Name + " KILLED!", target.IsPlayer() ? Color::Highlight : Color::Active);

            Interface::DropItemsOnDeath(battle, target);

            battle.Map.Remove(target.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, targetid);
        }
    }

    // resolve spell with multiple targets
    void ResolveSpell(Graphics::Base &graphics, Battle::Base &battle, Scene::Base &background, Character::Base &caster, Party::Base &targets, Spells::Type spell)
    {
        auto spellbook = caster.Find(spell);

        auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

        auto map_w = battle.Map.ViewX * battle.Map.TileSize;

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        if (spellbook != caster.Spells.end() && spellbook->MultipleTargets())
        {
            for (auto target = 0; target < targets.Count(); target++)
            {
                if (Engine::IsAlive(targets[target]))
                {
                    Interface::ResolveSpell(graphics, battle, background, targets, caster, targets[target], target, spell);
                }
            }
        }
        else if (spell == Spells::Type::EYE_OF_THE_TIGER)
        {
            Graphics::TextList tiger_eye =
                {Graphics::RichText("PLAYER FPR/DMG ROLLS +2", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                 Graphics::RichText(" PARTY FPR/DMG ROLLS +1", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0)};

            auto popup = draw + (Point(map_w, map_h) - Point(BloodSword::OctaTile, (BloodSword::DoubleTile + BloodSword::QuarterTile - BloodSword::SmallPad))) / 2;

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
        else if (spell == Spells::Type::IMMEDIATE_DELIVERANCE)
        {
            if (battle.Has(Controls::Type::FLEE))
            {
                Interface::ShowBookDescription(graphics, background, battle.ActionCancels[Controls::Type::FLEE]);
            }
            else
            {
                for (auto character = 0; character < targets.Count(); character++)
                {
                    if (Engine::IsAlive(targets[character]))
                    {
                        targets[character].Add(Character::Status::FLEEING);
                    }
                }
            }
        }
    }

    // resolve ambush (round 0) attacks
    void RenderAmbushRangedAttack(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party)
    {
        // initialize random number generator
        auto random = Random::Base();

        for (auto i = 0; i < battle.Opponents.Count(); i++)
        {
            // current attacker
            auto &attacker = battle.Opponents[i];

            if (Engine::IsAlive(attacker) && Engine::CanShoot(attacker))
            {
                // list of targets
                auto targets = std::vector<int>();

                for (auto j = 0; j < party.Count(); j++)
                {
                    if (Engine::CanTarget(party[j], true))
                    {
                        targets.push_back(j);
                    }
                }

                if (targets.size() > 0)
                {
                    // center map on attacker
                    Interface::Center(battle, Map::Object::ENEMY, i);

                    // generate battle scene
                    auto scene = Interface::BattleScene(battle, party);

                    // select target
                    auto selected = targets[random.NextInt() % targets.size()];

                    auto &defender = party[selected];

                    Interface::Shoot(graphics, scene, battle, party, attacker, defender, selected);
                }
            }
        }
    }

    // set player (indefinite) AWAY status
    void CheckPartyAwayStatus(Battle::Base &battle, Party::Base &party)
    {
        // set (indefinite) AWAY status of players not participating in the battle
        if (battle.Has(Battle::Condition::WARRIOR_AWAY) && party.Has(Character::Class::WARRIOR))
        {
            party[Character::Class::WARRIOR].Add(Character::Status::AWAY);
        }

        if (battle.Has(Battle::Condition::TRICKSTER_AWAY) && party.Has(Character::Class::TRICKSTER))
        {
            party[Character::Class::TRICKSTER].Add(Character::Status::AWAY);
        }

        if (battle.Has(Battle::Condition::SAGE_AWAY) && party.Has(Character::Class::SAGE))
        {
            party[Character::Class::SAGE].Add(Character::Status::AWAY);
        }

        if (battle.Has(Battle::Condition::ENCHANTER_AWAY) && party.Has(Character::Class::ENCHANTER))
        {
            party[Character::Class::ENCHANTER].Add(Character::Status::AWAY);
        }
    }

    // select spell targets
    int SelectSpellTargets(Battle::Base &battle, Party::Base &party, Engine::Queue &spell_targets, Spells::Type spell)
    {
        auto target = Target::NotFound;

        if (spell != Spells::Type::NONE)
        {
            for (auto i = 0; i < spell_targets.size(); i++)
            {
                auto target_id = spell_targets[i].Id;

                auto &character = (spell_targets[i].Type == Character::ControlType::NPC) ? battle.Opponents[target_id] : party[target_id];

                auto status = BloodSword::Find(Interface::SpellEffects, spell);

                if (Engine::IsAlive(character) && (status == Character::Status::NONE || (status != Character::Status::NONE && !character.Has(status))))
                {
                    target = i;

                    break;
                }
            }
        }

        return target;
    }

    // check if enemy character can cast spells
    bool CanCastSpells(Battle::Base &battle, Party::Base &party, Character::Base &caster, int caster_id)
    {
        auto cast = false;

        // count number of live opponents
        auto opponents = caster.ControlType == Character::ControlType::NPC ? Engine::Count(party) : Engine::Count(battle.Opponents);

        // determine if this is the caster's turn to cast spells (also check if list is being generated)
        auto my_turn = (BloodSword::Has(battle.Casters, caster_id) || battle.Casters.size() == 0);

        if (Engine::IsAlive(caster) && caster.Has(Skills::Type::SPELLS) && my_turn)
        {
            for (auto &strategy : caster.SpellStrategy)
            {
                auto targets = (opponents >= strategy.Min && opponents <= strategy.Max);

                auto spells_cast = Spells::Count(battle.AlreadyCast, strategy.AlreadyCast);

                if (targets && (strategy.Uses > 0) && (spells_cast < strategy.Count))
                {
                    cast = true;
                }

                BattleLogger::LogSpellStrategy(strategy.AlreadyCast, spells_cast, cast);

                if (cast)
                {
                    break;
                }
            }
        }
        else if (battle.Casters.size() > 0)
        {
            BattleLogger::LogCaster(caster, caster_id);
        }

        return cast;
    }

    // generate list of spell casters
    void GenerateCasters(Battle::Base &battle, Party::Base &party)
    {
        // temporary list
        auto temp_casters = std::vector<int>();

        // clear spell casters list
        battle.Casters.clear();

        // generate initial list
        for (auto i = 0; i < battle.Opponents.Count(); i++)
        {
            if (Interface::CanCastSpells(battle, party, battle.Opponents[i], i))
            {
                temp_casters.push_back(i);
            }
        }

        // transfer list
        battle.Casters = temp_casters;

        // create random subset
        if (battle.MaxCasters != Battle::Unlimited && battle.Casters.size() > 1)
        {
            for (auto i = 0; i < battle.Opponents.Count(); i++)
            {
                std::shuffle(battle.Casters.begin(), battle.Casters.end(), Engine::Random.Generator());
            }

            auto limit = std::min(battle.MaxCasters, int(battle.Casters.size()));

            std::vector<int> subset(battle.Casters.begin(), battle.Casters.begin() + limit);

            battle.Casters = subset;
        }

        BattleLogger::LogCasters(battle);
    }

    // enemy casts spells
    void EnemyCastSpells(Graphics::Base &graphics, Scene::Base &scene, Battle::Base &battle, Party::Base &party, Character::Base &character, Point &src)
    {
        auto map_w = battle.Map.ViewX * battle.Map.TileSize;

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

        // TODO: improve enemy casting strategy
        if (character.CalledToMind.size() > 0)
        {
            auto spell = character.CalledToMind[0];

            // find spell targets. prioritize targets with low endurance
            auto targets = Engine::Queue();

            if (spell != Spells::Type::GHASTLY_TOUCH)
            {
                targets = Engine::SpellTargets(battle.Map, party, src, true, false);
            }
            else
            {
                // spell needs adjacent targets
                targets = Engine::FightTargets(battle.Map, party, src, true, false);
            }

            BattleLogger::LogTargets("SPELL", character.Target, battle.Map[src].Id, targets.size());

            // cast spell
            if (Interface::Cast(graphics, scene, draw, map_w, map_h, character, spell, true))
            {
                // add to internal tracking
                battle.AlreadyCast.push_back(spell);

                // spellcasting successful
                Interface::MessageBox(graphics, scene, std::string(Spells::TypeMapping[spell]) + " SUCCESSFULLY CAST", Color::Highlight);

                // update caster's spell strategy
                character.CastSpell(spell);

                auto search = character.Find(spell);

                if (search != character.Spells.end())
                {
                    auto &spellbook = *search;

                    if (!spellbook.IsBasic() && spellbook.IsBattle)
                    {
                        if (!spellbook.RequiresTarget())
                        {
                            BattleLogger::LogSpellCasting(character.Target, battle.Map[src].Id, spell);

                            // resolve spell
                            Interface::ResolveSpell(graphics, battle, scene, character, party, spell);
                        }
                        else
                        {
                            if (targets.size() > 0)
                            {
                                auto target = Interface::SelectSpellTargets(battle, party, targets, spell);

                                if (target >= 0 && target < targets.size())
                                {
                                    auto target_id = targets[target].Id;

                                    auto &defender = targets[target].Type == Character::ControlType::NPC ? battle.Opponents[target_id] : party[target_id];

                                    std::string spell_action = "CASTS " + std::string(Spells::TypeMapping[spell]);

                                    BattleLogger::LogAction(spell_action.c_str(), character.Target, battle.Map[src].Id, defender.Target, target_id);

                                    Interface::ResolveSpell(graphics, battle, scene, party, character, defender, target_id, spell);
                                }
                                else
                                {
                                    BattleLogger::LogNoSpellTargets(spell);
                                }
                            }
                            else
                            {
                                BattleLogger::LogNoSpellTargets(spell);
                            }
                        }
                    }
                    else
                    {
                        BattleLogger::LogSpellUnusable(spell);
                    }
                }
                else
                {
                    BattleLogger::LogSpellMissing(spell);
                }
            }
            else
            {
                // spellcasting unsuccessful!
                Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_CAST), Color::Highlight);
            }
        }
        else if (character.Spells.size() > 0)
        {
            // call to mind
            for (auto &strategy : character.SpellStrategy)
            {
                if (strategy.Uses > 0 && Engine::Count(party) >= strategy.Min && Engine::Count(party) <= strategy.Max && !character.HasCalledToMind(strategy.Spell))
                {
                    character.CallToMind(strategy.Spell);

                    auto spell_string = std::string(Spells::TypeMapping[strategy.Spell]) + " CALLED TO MIND!";

                    Interface::MessageBox(graphics, scene, spell_string, Color::Highlight);

                    break;
                }
            }
        }
    }

    // enemy does ranged attacks
    void EnemyShoots(Graphics::Base &graphics, Scene::Base &scene, Battle::Base &battle, Party::Base &party, Engine::Queue &opponents, Character::Base &character, Point &src)
    {
        Engine::Queue targets;

        if (character.Targets.size() > 0 && character.TargetProbability > 0 && character.TargetProbability < 100)
        {
            // PREFERRED TARGET
            targets = Engine::RangedTargets(battle.Map, battle.Opponents, party, src, true);
        }
        else
        {
            // DEFAULT
            targets = Engine::RangedTargets(battle.Map, party, src, true, false);
        }

        BattleLogger::LogTargets("SHOOT", character.Target, battle.Map[src].Id, targets.size());

        // shoot only when there are no nearby player enemies
        if (targets.size() > 0 && opponents.size() == 0)
        {
            for (auto &target : targets)
            {
                auto &defender = (target.Type == Character::ControlType::PLAYER) ? party[target.Id] : battle.Opponents[target.Id];

                // shoot first available target
                if (!defender.IsImmune(character.Shoot))
                {
                    BattleLogger::LogAction("SHOOTS", character.Target, battle.Map[src].Id, defender.Target, target.Id);

                    // shoot
                    Interface::Shoot(graphics, scene, battle, party, character, defender, target.Id);

                    break;
                }
            }
        }
    }

    // enemy moves to target
    bool EnemyMoves(Scene::Base &scene, Animation::Base &movement, Battle::Base &battle, Party::Base &party, Character::Base &character, Point &src)
    {
        Engine::Queue targets;

        // check if enemy can move towards the player-controlled characters
        if (character.Targets.size() > 0 && character.TargetProbability > 0 && character.TargetProbability < 100)
        {
            // preferred targetting
            targets = Engine::MoveTargets(battle.Map, battle.Opponents, party, src, true);
        }
        else
        {
            // default
            targets = Engine::MoveTargets(battle.Map, party, src, true, false);
        }

        auto valid_target = false;

        BattleLogger::LogTargets("MOVE", character.Target, battle.Map[src].Id, targets.size());

        for (auto &target : targets)
        {
            auto end = battle.Map.Find(target.Type == Character::ControlType::PLAYER ? Map::Object::PLAYER : Map::Object::ENEMY, target.Id);

            if (!end.IsNone())
            {
                valid_target = Interface::Move(battle.Map, character, movement, src, end);

                if (valid_target)
                {
                    break;
                }
            }
        }

        return valid_target;
    }

    // resolve enemy fights
    Engine::Queue EnemyFights(Battle::Base &battle, Party::Base &party, Character::Base &character, Point &src)
    {
        // check if enemy can move towards the player-controlled characters
        if (character.Targets.size() > 0 && character.TargetProbability > 0 && character.TargetProbability < 100)
        {
            // PREFERRED TARGET
            return Engine::FightTargets(battle.Map, battle.Opponents, party, src, true);
        }
        else
        {
            // DEFAULT
            return Engine::FightTargets(battle.Map, party, src, true, false);
        }
    }

    // clones character's attributes
    void CloneCharacters(Battle::Base &battle, Party::Base &party)
    {
        std::vector<Battle::Condition> cloning = {
            Battle::Condition::CLONE_WARRIOR,
            Battle::Condition::CLONE_TRICKSTER,
            Battle::Condition::CLONE_SAGE,
            Battle::Condition::CLONE_ENCHANTER};

        for (auto clone : cloning)
        {
            if (battle.Has(clone))
            {
                auto character_class = Character::Class::NONE;

                switch (clone)
                {
                case Battle::Condition::CLONE_WARRIOR:
                    character_class = Character::Class::WARRIOR;
                    break;
                case Battle::Condition::CLONE_TRICKSTER:
                    character_class = Character::Class::TRICKSTER;
                    break;
                case Battle::Condition::CLONE_SAGE:
                    character_class = Character::Class::SAGE;
                    break;
                case Battle::Condition::CLONE_ENCHANTER:
                    character_class = Character::Class::ENCHANTER;
                    break;
                default:
                    break;
                }

                if (character_class != Character::Class::NONE)
                {
                    if (party.Has(character_class) && Engine::IsAlive(party[character_class]))
                    {
                        auto &character = party[character_class];

                        Attribute::Types attributes = {
                            Attribute::Type::FIGHTING_PROWESS,
                            Attribute::Type::PSYCHIC_ABILITY,
                            Attribute::Type::AWARENESS,
                            Attribute::Type::ENDURANCE,
                            Attribute::Type::DAMAGE,
                        };

                        for (auto i = 0; i < battle.Opponents.Count(); i++)
                        {
                            for (auto attribute : attributes)
                            {
                                auto value = battle.Opponents[i].Value(attribute);

                                auto modifier = battle.Opponents[i].Modifier(attribute);

                                // set maximum attibute value
                                battle.Opponents[i].Maximum(attribute, character.Maximum(attribute) + value);

                                // clone attributes
                                battle.Opponents[i].Set(attribute, character.Maximum(attribute) + value, character.Modifier(attribute) + modifier);
                            }
                        }
                    }
                }
            }
        }
    }

    // cooldown AWAY status (since they are not part of the queue while they are AWAY)
    bool CoolDownStatus(Party::Base &party, Character::Status status)
    {
        auto update = false;

        // cooldown AWAY status
        for (auto character = 0; character < party.Count(); character++)
        {
            if (Engine::IsAlive(party[character]) && party[character].Is(status))
            {
                update |= Engine::CoolDown(party[character], status);
            }
        }

        return update;
    }

    // fight battle
    Battle::Result RenderBattle(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party)
    {
        // set initial result (DETERMINE = battle result to be determined)
        auto result = Battle::Result::DETERMINE;

        // adjust battle UI dimensions and locations
        auto total_w = graphics.Width - 12 * BloodSword::TileSize;

        auto total_h = graphics.Height - 6 * BloodSword::TileSize;

        battle.Map.ViewX = std::min(battle.Map.ViewX, (total_w / BloodSword::TileSize));

        battle.Map.ViewY = std::min(battle.Map.ViewY, (total_h / BloodSword::TileSize));

        battle.Map.DrawX = (graphics.Width - battle.Map.ViewX * battle.Map.TileSize) / 2;

        battle.Map.DrawY = (graphics.Height - battle.Map.ViewY * battle.Map.TileSize) / 2;

        battle.Map.X = 0;

        battle.Map.Y = 0;

        if (battle.Duration != 0 && Engine::IsAlive(party))
        {
            // check if there are characters to clone
            Interface::CloneCharacters(battle, party);

            if (battle.Has(Battle::Condition::TACTICS) || party.Has(Character::Status::TACTICS))
            {
                // setup player locations
                Interface::Tactics(graphics, battle, party);

                // remove TACTICS status from party
                party.Remove(Character::Status::TACTICS);
            }

            if (party.Has(Character::Status::AMBUSH_RANGED) && battle.Has(Battle::Condition::AMBUSH_PLAYER_RANGED))
            {
                // add additional RANGED ambush attacks
                battle.AmbushRounds++;

                // remove AMBUSH RANGED status from party
                party.Remove(Character::Status::AMBUSH_RANGED);
            }

            if (battle.Has(Battle::Condition::ENTANGLED))
            {
                // apply ENTANGLED
                party.Add(Character::Status::ENTANGLED);
            }

            // Check if any players in the party are AWAY / not participating
            Interface::CheckPartyAwayStatus(battle, party);

            // set player starting locations
            Interface::SetPlayerLocations(battle, party);

            if (battle.Has(Battle::Condition::REPLICATE) && battle.Opponents.Count() == 1)
            {
                // opponents to be replicated
                auto replicated = battle.Opponents[0];

                // replicate opponents to the number of players
                for (auto i = 0; i < Engine::Combatants(party) - 1; i++)
                {
                    battle.Opponents.Add(replicated);
                }
            }

            // set enemy starting locations
            Interface::SetEnemyLocations(battle, party);

            // place other characters adjacent to other party members
            if (party.HasOthers() && party.Count() > 1 && !battle.Has(Battle::Condition::EXCLUDE_OTHERS))
            {
                for (auto i = 0; i < party.Count(); i++)
                {
                    if (Character::OtherClass(party[i].Class) && Engine::IsAlive(party[i]))
                    {
                        Interface::PlayerTactics(graphics, battle, party, party[i].Class);
                    }
                }
            }

            // final checks on placement of all combatants
            Interface::FinalLocationChecks(battle, party);

            // round string or status messages
            SDL_Texture *round_string = nullptr;

            // current tile
            SDL_Texture *texture = nullptr;

            std::vector<std::string> captions_text = {
                "EXIT",
                "CENTER",
                "MOVE",
                "DEFEND",
                "FIGHT",
                "QUARTERSTAFF",
                "SHOOT",
                "SHURIKEN",
                "FLEE",
                "SPELLS",
                "ITEMS"};

            Controls::List caption_controls = {
                Controls::Type::EXIT,
                Controls::Type::CENTER,
                Controls::Type::MOVE,
                Controls::Type::DEFEND,
                Controls::Type::FIGHT,
                Controls::Type::QUARTERSTAFF,
                Controls::Type::SHOOT,
                Controls::Type::SHURIKEN,
                Controls::Type::FLEE,
                Controls::Type::SPELLS,
                Controls::Type::ITEMS};

            // create captions textures
            auto captions = Graphics::CreateText(graphics, Graphics::GenerateTextList(captions_text, Fonts::Caption, Color::Active, 0));

            Asset::List action_assets = {
                Asset::Type::MOVE,
                Asset::Type::SHOOT,
                Asset::Type::SHURIKEN,
                Asset::Type::FIGHT,
                Asset::Type::QUARTERSTAFF,
                Asset::Type::SPELLS,
                Asset::Type::ITEMS};

            Controls::List action_controls = {
                Controls::Type::MOVE,
                Controls::Type::SHOOT,
                Controls::Type::SHURIKEN,
                Controls::Type::FIGHT,
                Controls::Type::QUARTERSTAFF,
                Controls::Type::SPELLS,
                Controls::Type::ITEMS};

            // controls list
            Controls::List battle_actions = {
                Controls::Type::MOVE,
                Controls::Type::DEFEND,
                Controls::Type::FIGHT,
                Controls::Type::QUARTERSTAFF,
                Controls::Type::SHOOT,
                Controls::Type::SHURIKEN,
                Controls::Type::FLEE,
                Controls::Type::SPELLS,
                Controls::Type::ITEMS};

            BloodSword::UnorderedMap<Controls::Type, SDL_Texture *> highlight = {};

            for (auto i = 0; i < action_assets.size(); i++)
            {
                auto texture = Asset::Copy(graphics.Renderer, action_assets[i], Color::Highlight);

                highlight[action_controls[i]] = texture;
            }

            auto infow = battle.Map.TileSize * 5;

            auto enemy_stats = Interface::GenerateStats(graphics, battle.Opponents, infow);

            auto enemy_status = Interface::GenerateStatus(graphics, battle.Opponents, infow);

            auto party_stats = Interface::GenerateStats(graphics, party, infow);

            auto party_status = Interface::GenerateStatus(graphics, party, infow);

            auto asset = Asset::Type::NONE;

            auto lifetime = Battle::Unlimited;

            // move animation
            auto movement = Animation::Base();

            // spell to cast
            auto cast = Spells::Type::NONE;

            // quarterstaff targetting
            auto knockout = Skills::Type::NONE;

            // spells popup
            auto spells = false;

            // items popup
            auto items = false;

            // actions popup
            auto actions = false;

            // animation in progress
            auto animating = false;

            // select destination
            auto move = false;

            // fight targetting
            auto fight = false;

            // shoot targetting
            auto shoot = false;

            // spell targetting
            auto spell = false;

            // user input
            auto input = Controls::User();

            // previous user input
            auto previous = Controls::User();

            auto pad = BloodSword::Pad;

            auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

            auto info_x = battle.Map.DrawX + (battle.Map.ViewX * 2 + 1) * battle.Map.TileSize / 2 + BloodSword::TileSize + pad;

            auto info_y = battle.Map.DrawY + BloodSword::SmallPad;

            auto map_w = battle.Map.ViewX * battle.Map.TileSize;

            auto map_h = battle.Map.ViewY * battle.Map.TileSize;

            auto text_y = battle.Map.DrawY - (BloodSword::TileSize + BloodSword::HalfTile) - pad;

            // set "IN BATTLE" status
            party.Add(Character::Status::IN_BATTLE);

            battle.Opponents.Add(Character::Status::IN_BATTLE);

            // focus/cursor blink variables
            Uint64 blink_start = SDL_GetTicks64();

            Uint64 blink_delay = BloodSword::StandardDelay;

            auto blinking = false;

            // clear any dropped items
            battle.Loot.clear();

            // initialize round
            battle.Round = 0;

            // exit battle flag
            battle.ExitBattle = false;

            // check if no one is undergoing a mission
            if (battle.Has(Battle::Condition::MISSION) && battle.Duration != Battle::Unlimited && Engine::Count(party, Character::ControlType::PLAYER, Character::Status::AWAY) == 0)
            {
                battle.Duration = Battle::Unlimited;
            }

            while ((battle.Round < battle.Duration || battle.Duration == Battle::Unlimited) && Engine::IsAlive(party) && Engine::IsAlive(battle.Opponents, Character::ControlType::NPC) && !Engine::IsFleeing(party) && (Engine::InBattle(party) > 0) && !battle.ExitBattle)
            {
                // activate SLOW MURDER before building the queue
                for (auto i = 0; i < party.Count(); i++)
                {
                    if (Engine::IsAlive(party[i]) && party[i].Has(Character::Status::SLOW_MURDER))
                    {
                        if (!Engine::GainEndurance(party[i], -1, true))
                        {
                            auto origin = battle.Map.Find(Map::Object::PLAYER, i);

                            Interface::Center(battle, Map::Object::PLAYER, i);

                            auto scene = Interface::BattleScene(battle, party, party[i], i, origin, false);

                            Interface::Resurrect(graphics, scene, party, party[i]);

                            battle.Map.Remove(Map::Object::PLAYER, i);
                        }
                    }
                }

                // initialize battle order
                battle.Order = {};

                // spells already cast by NPC players
                battle.AlreadyCast.clear();

                // generate spell casters this turn
                Interface::GenerateCasters(battle, party);

                auto ambush = false;

                auto shot_ambush = false;

                if (battle.Has(Battle::Condition::AMBUSH_PLAYER) && battle.Round < battle.AmbushRounds)
                {
                    // players get a free initial turn
                    battle.Order = Engine::Build(party, Attribute::Type::AWARENESS, true, true);

                    ambush = true;
                }
                else if (battle.Has(Battle::Condition::AMBUSH_NPC) && battle.Round < battle.AmbushRounds)
                {
                    // enemy combatants get a free initial turn
                    battle.Order = Engine::Build(battle.Opponents, Attribute::Type::AWARENESS, true, true);

                    ambush = true;
                }
                else if (battle.Has(Battle::Condition::AMBUSH_PLAYER_RANGED) && battle.Round < battle.AmbushRounds)
                {
                    // ranged attackers get a free initial turn
                    battle.Order = Engine::Shooters(party, Attribute::Type::AWARENESS, true, true);

                    shot_ambush = true;
                }
                else if (battle.Has(Battle::Condition::AMBUSH_NPC_RANGED) && battle.Round < battle.AmbushRounds)
                {
                    // enemies get a free ranged attack
                    Interface::RenderAmbushRangedAttack(graphics, battle, party);

                    shot_ambush = true;
                }
                else
                {
                    // otherwise create battle order (default)
                    battle.Order = Engine::Build(party, battle.Opponents, Attribute::Type::AWARENESS, true, true);
                }

                // regenerate round string
                BloodSword::Free(&round_string);

                round_string = Graphics::CreateText(graphics, (std::string("ROUND ") + std::to_string(battle.Round + 1) + ((ambush || shot_ambush) ? std::string(" (AMBUSH)") : "")).c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

                // reset IN COMBAT status
                Engine::ResetCombatStatus(party);

                Engine::ResetCombatStatus(battle.Opponents);

                // start of round effects
                battle.NextRound = false;

                // start with current character
                battle.Combatant = 0;

                // save current map/party/opponents this round
                battle.SaveRound(party);

                while (!battle.NextRound && Engine::IsAlive(party) && Engine::IsAlive(battle.Opponents, Character::ControlType::NPC) && !Engine::IsFleeing(party) && !battle.ExitBattle)
                {
                    // move to next round
                    if (battle.Order.size() <= 0)
                    {
                        break;
                    }

                    auto is_enemy = Engine::IsEnemy(battle.Order, battle.Combatant);

                    auto is_player = Engine::IsPlayer(battle.Order, battle.Combatant);

                    auto character_id = battle.Order[battle.Combatant].Id;

                    auto &character = is_player ? party[character_id] : battle.Opponents[character_id];

                    auto origin = battle.Map.Find(is_player ? Map::Object::PLAYER : Map::Object::ENEMY, character_id);

                    // center map on player
                    Interface::Center(battle, is_player ? Map::Object::PLAYER : Map::Object::ENEMY, character_id);

                    // regenerate scene
                    auto scene = Interface::BattleScene(battle, party, character, character_id, origin, shot_ambush);

                    // start of character turn
                    if (battle.Round > 0 && Engine::CoolDown(character))
                    {
                        // regenerate stats
                        Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);
                    }

                    battle.EndTurn = false;

                    Interface::SearchInCombatTargets(battle, party, battle.Order, battle.Combatant);

                    while (!battle.EndTurn && Engine::IsAlive(party) && Engine::IsAlive(battle.Opponents, Character::ControlType::NPC) && !Engine::IsFleeing(party) && !battle.ExitBattle)
                    {
                        auto overlay = Scene::Base();

                        auto refresh_textures = false;

                        auto performed_action = false;

                        auto regenerate_scene = false;

                        // main loop
                        if (!animating)
                        {
                            auto src = battle.Map.Find(Engine::IsPlayer(battle.Order, battle.Combatant) ? Map::Object::PLAYER : Map::Object::ENEMY, character_id);

                            if (!src.IsNone())
                            {
                                // can perform action
                                auto has_actions = !character.Is(Character::Status::PARALYZED) && !character.Is(Character::Status::AWAY) && character.Is(Character::Status::IN_BATTLE) && Engine::IsAlive(character);

                                // full engage
                                auto full_engage = Engine::Count(battle.Opponents, Skills::Type::ATTACKS_ENGAGED) >= Engine::Count(battle.Opponents);

                                // enemy action (fight/shoot/cast/move)
                                if (is_enemy && !character.Is(Character::Status::ENTHRALLED))
                                {
                                    // enemy combatant is not paralyzed
                                    if (has_actions)
                                    {
                                        // check if there are adjacent player combatants
                                        auto opponents = Interface::EnemyFights(battle, party, character, src);

                                        if (character.Has(Skills::Type::SLOW_MURDER) && Engine::Count(party, Character::ControlType::PLAYER, Character::Status::SLOW_MURDER) < Engine::Count(party) && Interface::Roll(graphics, scene, character.Asset, Asset::Type::MISTS_OF_DEATH, 1, 0).Sum == 1)
                                        {
                                            std::string slow_murder = character.Name + " UNLEASHES THE SLOW MURDER SPELL!";

                                            Interface::MessageBox(graphics, scene, slow_murder, Color::Highlight);

                                            for (auto i = 0; i < party.Count(); i++)
                                            {
                                                std::string resists = party[i].Name + " RESISTS THE SLOW MURDER SPELL!";

                                                std::string succumb = party[i].Name + " SUCCUMBS TO THE SLOW MURDER SPELL!";

                                                if (Engine::CanTarget(party[i], true) && !party[i].Has(Character::Status::SLOW_MURDER))
                                                {
                                                    if (!Interface::Test(graphics, scene, party[i], Attribute::Type::PSYCHIC_ABILITY))
                                                    {
                                                        Interface::MessageBox(graphics, scene, succumb, Color::Highlight);

                                                        party[i].Add(Character::Status::SLOW_MURDER);
                                                    }
                                                    else
                                                    {
                                                        Interface::MessageBox(graphics, scene, resists, Color::Active);
                                                    }
                                                }
                                            }
                                        }
                                        else if (opponents.size() > 0 && character.HasChargedWeapon(Item::Type::CHARGE, 1, true) != Item::NotFound)
                                        {
                                            // enemy has charged melee weapon
                                            character.Add(Character::Status::IN_COMBAT);

                                            auto target_id = opponents[0].Id;

                                            auto &defender = (opponents[0].Type == Character::ControlType::PLAYER) ? party[target_id] : battle.Opponents[target_id];

                                            BattleLogger::LogAction("BLASTS", character.Target, character_id, defender.Target, target_id);

                                            auto item_id = character.HasChargedWeapon(Item::Type::CHARGE, 1, true);

                                            auto target = battle.Map.Find(opponents[0].Type == Character::ControlType::PLAYER ? Map::Object::PLAYER : Map::Object::ENEMY, target_id);

                                            // discharge weapon
                                            Interface::TargetAction(graphics, scene, battle, party, character, item_id, target);
                                        }
                                        else if (Interface::CanCastSpells(battle, party, character, character_id) && !battle.Has(Battle::Condition::NO_COMBAT))
                                        {
                                            character.Add(Character::Status::IN_COMBAT);

                                            // cast or call to mind spell
                                            Interface::EnemyCastSpells(graphics, scene, battle, party, character, src);
                                        }
                                        else if (opponents.size() > 0 && !battle.Has(Battle::Condition::NO_COMBAT))
                                        {
                                            character.Add(Character::Status::IN_COMBAT);

                                            BattleLogger::LogTargets("FIGHT", character.Target, character_id, opponents.size());

                                            Engine::ResetSpells(character);

                                            // fight
                                            auto defender_id = opponents[0].Id;

                                            auto &defender = ((opponents[0].Type == Character::ControlType::PLAYER) ? party[defender_id] : battle.Opponents[defender_id]);

                                            BattleLogger::LogAction("FIGHTS", character.Target, character_id, defender.Target, defender_id);

                                            Interface::Fight(graphics, scene, battle, party, character, character_id, defender, defender_id, character.Fight);
                                        }
                                        else if (character.Has(Skills::Type::ATTACKS_ENGAGED) && !full_engage)
                                        {
                                            if (Engine::CanShoot(character) && opponents.size() == 0)
                                            {
                                                auto target_id = party.Index(battle.InCombatTarget);

                                                if (target_id >= 0 && target_id < party.Count() && Engine::CanTarget(party[target_id], true))
                                                {
                                                    // shoot at IN COMBAT target
                                                    BattleLogger::LogAction("SHOOTS", character.Target, character_id, party[target_id].Target, target_id);

                                                    Interface::Shoot(graphics, scene, battle, party, character, party[target_id], target_id);

                                                    if (!Engine::CanTarget(party[target_id], true))
                                                    {
                                                        Interface::SearchInCombatTargets(battle, party, battle.Order, battle.Combatant);
                                                    }
                                                }
                                            }
                                        }
                                        else if (Engine::CanShoot(character) && !battle.Has(Battle::Condition::NO_COMBAT))
                                        {
                                            character.Add(Character::Status::IN_COMBAT);

                                            // do ranged attacks
                                            Interface::EnemyShoots(graphics, scene, battle, party, opponents, character, src);
                                        }
                                        else if (character.Moves > 0 && Move::Available(battle.Map, src) && !character.Is(Character::Status::ENTANGLED))
                                        {
                                            // enemy moves next to a target
                                            animating = Interface::EnemyMoves(scene, movement, battle, party, character, src);
                                        }
                                    }

                                    if (!animating)
                                    {
                                        performed_action = true;

                                        refresh_textures = true;
                                    }
                                    else
                                    {
                                        regenerate_scene = true;
                                    }
                                }
                                else if (has_actions)
                                {
                                    // player-controlled characters including enthralled enemies

                                    if (!actions && !items && !spells)
                                    {
                                        if (Input::IsValid(scene, input))
                                        {
                                            auto &control = scene.Controls[input.Current];

                                            if (control.OnMap && battle.Map.IsValid(control.Map))
                                            {
                                                // draw path to destination
                                                if (move)
                                                {
                                                    auto src = is_player ? battle.Map.Find(Map::Object::PLAYER, character_id) : battle.Map.Find(Map::Object::ENEMY, character_id);

                                                    auto dst = control.Map;

                                                    overlay = Interface::Path(battle.Map, character, src, dst);

                                                    overlay.VerifyAndAdd(Scene::Element(Interface::Message[Interface::MSG_DEST], battle.Map.DrawX, text_y));
                                                }
                                                else if (fight)
                                                {
                                                    // fight mode
                                                    overlay.VerifyAndAdd(Scene::Element(Interface::Message[Interface::MSG_OPPONENT], battle.Map.DrawX, text_y));
                                                }
                                                else if (shoot || spell)
                                                {
                                                    // shoot mode
                                                    overlay.VerifyAndAdd(Scene::Element(Interface::Message[Interface::MSG_TARGET], battle.Map.DrawX, text_y));
                                                }
                                                else
                                                {
                                                    // round number
                                                    overlay.VerifyAndAdd(Scene::Element(round_string, battle.Map.DrawX, text_y));
                                                }

                                                // show character stats
                                                if (battle.Map[control.Map].Occupant == Map::Object::PLAYER)
                                                {
                                                    auto stats_id = battle.Map[control.Map].Id;

                                                    if (stats_id >= 0 && stats_id < party.Count())
                                                    {
                                                        // stats
                                                        overlay.VerifyAndAdd(Scene::Element(party_stats[stats_id], info_x, info_y, Color::Background, Color::Active, BloodSword::Border));

                                                        auto &stats = overlay.Elements.back();

                                                        // status
                                                        overlay.VerifyAndAdd(Scene::Element(party_status[stats_id], info_x, info_y + stats.H + pad * 4, Color::Background, Color::Active, BloodSword::Border));
                                                    }
                                                }
                                                else if (battle.Map[control.Map].Occupant == Map::Object::ENEMY)
                                                {
                                                    auto stats_id = battle.Map[control.Map].Id;

                                                    if (stats_id >= 0 && stats_id < battle.Opponents.Count())
                                                    {
                                                        // enemy stats
                                                        overlay.VerifyAndAdd(Scene::Element(enemy_stats[stats_id], info_x, info_y, Color::Background, Color::Active, BloodSword::Border));

                                                        auto &stats = overlay.Elements.back();

                                                        // status
                                                        overlay.VerifyAndAdd(Scene::Element(enemy_status[stats_id], info_x, info_y + stats.H + pad * 4, Color::Background, Color::Active, BloodSword::Border));
                                                    }
                                                }
                                                else if (battle.Map[control.Map].IsExit())
                                                {
                                                    if (asset != battle.Map[control.Map].Asset)
                                                    {
                                                        asset = battle.Map[control.Map].Asset;

                                                        BloodSword::Free(&texture);

                                                        texture = Graphics::CreateText(graphics, "EXIT", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);
                                                    }

                                                    overlay.VerifyAndAdd(Scene::Element(texture, info_x, info_y, Color::Background, Color::Inactive, 4));
                                                }
                                                else if (battle.Map[control.Map].IsTemporarilyBlocked())
                                                {
                                                    if (asset != battle.Map[control.Map].TemporaryAsset || lifetime != battle.Map[control.Map].Lifetime)
                                                    {
                                                        asset = battle.Map[control.Map].TemporaryAsset;

                                                        lifetime = battle.Map[control.Map].Lifetime;

                                                        std::string text = " OBSTACLE (" + std::to_string(battle.Map[control.Map].Lifetime) + ") ";

                                                        BloodSword::Free(&texture);

                                                        texture = Graphics::CreateText(graphics, text.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);
                                                    }

                                                    overlay.VerifyAndAdd(Scene::Element(texture, info_x, info_y, Color::Background, Color::Inactive, 4));
                                                }
                                            }
                                            else
                                            {
                                                // round number
                                                overlay.VerifyAndAdd(Scene::Element(round_string, battle.Map.DrawX, text_y));

                                                auto caption_id = Controls::Find(caption_controls, control.Type);

                                                if (caption_id >= 0 && caption_id < captions.size())
                                                {
                                                    if (input.Current >= 0 && input.Current < scene.Controls.size())
                                                    {
                                                        if (captions[caption_id])
                                                        {
                                                            // center texture
                                                            auto center = (control.W - BloodSword::Width(captions[caption_id])) / 2;

                                                            overlay.VerifyAndAdd(Scene::Element(captions[caption_id], control.X + center, control.Y + control.H + BloodSword::Pad));
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    else if (spells)
                                    {
                                        // spells popup
                                        overlay = Interface::Spells(draw, map_w, map_h, character, Color::Background, Color::Active, BloodSword::Border, true);

                                        // round number
                                        overlay.VerifyAndAdd(Scene::Element(round_string, battle.Map.DrawX, text_y));

                                        if (Input::IsValid(overlay, input))
                                        {
                                            if (input.Type != Controls::Type::BACK)
                                            {
                                                auto &control = overlay.Controls[input.Current];

                                                auto &spell_caption = character.Spells[control.Id];

                                                auto &popup = overlay.Elements[0];

                                                if (character.HasCalledToMind(spell_caption.Type) && spell_caption.IsBattle && !spell_caption.IsBasic())
                                                {
                                                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CAST_SPELL), popup.X + popup.W - (BloodSword::TileSize + BloodSword::Pad), popup.Y + BloodSword::Pad));

                                                    overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsActive[spell_caption.Type], control.X, control.Y + control.H + pad));

                                                    overlay.VerifyAndAdd(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CAST_SPELL], popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));
                                                }
                                                else if (!spell_caption.IsBasic() && spell_caption.IsBattle)
                                                {
                                                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CALL_TO_MIND), popup.X + popup.W - (BloodSword::TileSize + BloodSword::Pad), popup.Y + BloodSword::Pad));

                                                    overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsInactive[spell_caption.Type], control.X, control.Y + control.H + pad));

                                                    overlay.VerifyAndAdd(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CALL_TO_MIND], popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));
                                                }
                                                else
                                                {
                                                    overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsInactive[spell_caption.Type], control.X, control.Y + control.H + pad));
                                                }
                                            }
                                        }
                                    }
                                    else if (actions)
                                    {
                                        // actions popup
                                        overlay = Interface::BattleActions(draw, map_w, map_h, battle, is_player ? party : battle.Opponents, character_id, Color::Background, Color::Active, BloodSword::Border, shot_ambush);

                                        // round number
                                        overlay.VerifyAndAdd(Scene::Element(round_string, battle.Map.DrawX, text_y));

                                        if (Input::IsValid(overlay, input))
                                        {
                                            // actions popup captions
                                            auto &control = overlay.Controls[input.Current];

                                            if (BloodSword::Has(Interface::BattleControlCaptions, control.Type))
                                            {
                                                overlay.VerifyAndAdd(Scene::Element(Interface::BattleControlCaptions[control.Type], control.X, control.Y + control.H + pad));
                                            }
                                        }
                                    }

                                    // focus cursor
                                    if (!actions && !items && !spells)
                                    {
                                        if (blinking)
                                        {
                                            // focus cursor
                                            Interface::Focus(battle.Map, battle.Order, battle.Combatant, overlay);
                                        }

                                        input.Blink = false;
                                    }
                                    else
                                    {
                                        input.Blink = !input.Blink;
                                    }

                                    if (move)
                                    {
                                        Interface::HighlightControl(scene, highlight[Controls::Type::MOVE], Controls::Type::MOVE);
                                    }
                                    else if (fight)
                                    {
                                        Interface::HighlightControl(scene, highlight[Controls::Type::FIGHT], Controls::Type::FIGHT);
                                    }
                                    else if (spell || spells)
                                    {
                                        Interface::HighlightControl(scene, highlight[Controls::Type::SPELLS], Controls::Type::SPELLS);
                                    }
                                    else if (shoot)
                                    {
                                        Interface::HighlightControl(scene, highlight[Controls::Type::SHOOT], Controls::Type::SHOOT);

                                        Interface::HighlightControl(scene, highlight[Controls::Type::SHURIKEN], Controls::Type::SHURIKEN);
                                    }
                                    else if (items)
                                    {
                                        Interface::HighlightControl(scene, highlight[Controls::Type::ITEMS], Controls::Type::ITEMS);
                                    }

                                    // wait for input
                                    if (!items)
                                    {
                                        input = Input::WaitForInput(graphics, scene, overlay, input, (actions || spells), (actions || spells));
                                    }

                                    if (!actions && !items && !spells)
                                    {
                                        auto blink_end = SDL_GetTicks64();

                                        if (blink_end - blink_start >= blink_delay)
                                        {
                                            blinking = !blinking;

                                            blink_start = blink_end;
                                        }
                                    }

                                    if (Input::Check(input))
                                    {
                                        if (Input::IsValid(scene, input) && !actions && !items && !spells && (Controls::Find(battle_actions, input.Type) == Controls::NotFound))
                                        {
                                            auto &control = scene.Controls[input.Current];

                                            if (control.OnMap && battle.Map[scene.Controls[input.Current].Map].Id == character_id && ((is_player && battle.Map[scene.Controls[input.Current].Map].IsPlayer()) || (is_enemy && battle.Map[scene.Controls[input.Current].Map].IsEnemy())))
                                            {
                                                knockout = character.Fight;

                                                previous = input;

                                                actions = true;

                                                fight = false;

                                                items = false;

                                                move = false;

                                                shoot = false;

                                                spell = false;
                                            }
                                            else if (control.OnMap && battle.Map.IsValid(control.Map) && (input.Type == Controls::Type::LOCATION || input.Type == Controls::Type::MAP_EXIT))
                                            {
                                                // setup animation
                                                if (move)
                                                {
                                                    auto opponents = Engine::FightTargets(battle.Map, battle.Opponents, src, true, false);

                                                    auto &control = scene.Controls[input.Current];

                                                    auto no_opponents = (opponents.size() == 0);

                                                    auto has_defended = (is_player && character.Is(Character::Status::DEFENDED));

                                                    auto can_move = (opponents.size() > 0 && (is_enemy || has_defended || ambush));

                                                    if (no_opponents || can_move || battle.Has(Battle::Condition::NO_COMBAT))
                                                    {
                                                        auto start = battle.Map.Find(Engine::IsPlayer(battle.Order, battle.Combatant) ? Map::Object::PLAYER : Map::Object::ENEMY, character_id);

                                                        auto end = control.Map;

                                                        // find a path to the destination and animate
                                                        animating = Interface::Move(battle.Map, character, movement, start, end);

                                                        if (animating)
                                                        {
                                                            Engine::Cancel(character, Character::Status::FLEEING);

                                                            refresh_textures = true;
                                                        }
                                                        else
                                                        {
                                                            // no route to destination
                                                            Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_MOVE), Color::Highlight);
                                                        }

                                                        regenerate_scene = true;
                                                    }
                                                    else
                                                    {
                                                        // enemies nearby
                                                        Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_NEARBY), Color::Highlight);

                                                        regenerate_scene = true;
                                                    }

                                                    move = !move;
                                                }
                                                else if (spell && cast == Spells::Type::PILLAR_OF_SALT)
                                                {
                                                    character.Add(Character::Status::IN_COMBAT);

                                                    if (Interface::Cast(graphics, scene, draw, map_w, map_h, character, cast, true))
                                                    {
                                                        // spellcasting successful
                                                        auto spell_string = std::string(Spells::TypeMapping[cast]) + " SUCCESSFULLY CAST";

                                                        Interface::MessageBox(graphics, scene, spell_string, Color::Active);

                                                        // resolve spell
                                                        battle.Map.Put(control.Map, Map::Object::TEMPORARY_OBSTACLE, Asset::Type::PILLAR_OF_SALT, 5);

                                                        refresh_textures = true;
                                                    }
                                                    else
                                                    {
                                                        // spellcasting unsuccessful!
                                                        Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_CAST), Color::Highlight);
                                                    }

                                                    cast = Spells::Type::NONE;

                                                    performed_action = true;

                                                    spell = false;
                                                }
                                            }
                                            else if (control.OnMap && battle.Map.IsValid(control.Map) && input.Type == Controls::Type::ENEMY)
                                            {
                                                if (fight)
                                                {
                                                    // opponent chosen
                                                    auto distance = battle.Map.Distance(src, control.Map);

                                                    if (distance == 1)
                                                    {
                                                        character.Add(Character::Status::IN_COMBAT);

                                                        Engine::ResetStatusAndSpells(character);

                                                        auto fighter = battle.Map[control.Map].Id;

                                                        // fight
                                                        Interface::Fight(graphics, scene, battle, party, character, character_id, battle.Opponents[fighter], fighter, knockout);

                                                        // checks if enthrallment is broken
                                                        Interface::CheckEnthrallment(graphics, battle, scene, character, Interface::Text[Interface::MSG_ENTHRAL]);

                                                        refresh_textures = true;

                                                        performed_action = true;
                                                    }

                                                    knockout = Skills::Type::NONE;

                                                    fight = false;
                                                }
                                                else if (shoot)
                                                {
                                                    // target chosen
                                                    auto distance = battle.Map.Distance(src, control.Map);

                                                    if ((distance > 1) || shot_ambush)
                                                    {
                                                        auto shooter = battle.Map[control.Map].Id;

                                                        auto &shot_target = battle.Opponents[shooter];

                                                        if (!shot_target.IsImmune(character.Shoot))
                                                        {
                                                            character.Add(Character::Status::IN_COMBAT);

                                                            Engine::ResetStatus(character);

                                                            // shoot
                                                            Interface::Shoot(graphics, scene, battle, party, character, shot_target, shooter);

                                                            // checks if enthrallment is broken
                                                            Interface::CheckEnthrallment(graphics, battle, scene, character, Interface::Text[Interface::MSG_ENTHRAL]);

                                                            refresh_textures = true;

                                                            performed_action = true;
                                                        }
                                                        else
                                                        {
                                                            Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_RANGED), Color::Highlight);

                                                            regenerate_scene = true;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        // enemies nearby
                                                        Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_NEARBY), Color::Highlight);

                                                        regenerate_scene = true;
                                                    }

                                                    shoot = false;
                                                }
                                                else if (spell && cast != Spells::Type::NONE && cast != Spells::Type::PILLAR_OF_SALT)
                                                {
                                                    auto spellbook = character.Find(cast);

                                                    spell = false;

                                                    if (spellbook != character.Spells.end())
                                                    {
                                                        auto distance = battle.Map.Distance(src, control.Map);

                                                        if (!spellbook->Ranged && distance != 1)
                                                        {
                                                            // must be adjacent
                                                            Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_ADJACENT), Color::Highlight);
                                                        }
                                                        else
                                                        {
                                                            auto &spell_target = battle.Opponents[battle.Map[control.Map].Id];

                                                            if (!spell_target.IsImmune(cast) && !spell_target.Has(Skills::Type::MAGIC_IMMUNITY))
                                                            {
                                                                character.Add(Character::Status::IN_COMBAT);

                                                                if (Interface::Cast(graphics, scene, draw, map_w, map_h, character, spell_target.Asset, cast, true))
                                                                {
                                                                    // spellcasting successful
                                                                    auto spell_string = std::string(Spells::TypeMapping[cast]) + " SUCCESSFULLY CAST";

                                                                    Interface::MessageBox(graphics, scene, spell_string, Color::Active);

                                                                    // resolve spell
                                                                    Interface::ResolveSpell(graphics, battle, scene, battle.Opponents, character, spell_target, battle.Map[control.Map].Id, cast);

                                                                    refresh_textures = true;
                                                                }
                                                                else
                                                                {
                                                                    // spellcasting unsuccessful!
                                                                    Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_CAST), Color::Highlight);
                                                                }

                                                                performed_action = true;
                                                            }
                                                            else
                                                            {
                                                                Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_SPELL), Color::Highlight);

                                                                regenerate_scene = true;
                                                            }
                                                        }
                                                    }

                                                    cast = Spells::Type::NONE;
                                                }
                                            }
                                            else if (input.Type == Controls::Type::CENTER)
                                            {
                                                Interface::Center(battle, is_player ? Map::Object::PLAYER : Map::Object::ENEMY, character_id);

                                                regenerate_scene = true;
                                            }
                                            else if (input.Type == Controls::Type::MAP_DOWN)
                                            {
                                                if (battle.Map.Y < (battle.Map.Height - battle.Map.ViewY))
                                                {
                                                    battle.Map.Y++;

                                                    regenerate_scene = true;
                                                }
                                            }
                                            else if (input.Type == Controls::Type::MAP_UP)
                                            {
                                                if (battle.Map.Y > 0)
                                                {
                                                    battle.Map.Y--;

                                                    regenerate_scene = true;
                                                }
                                            }
                                            else if (input.Type == Controls::Type::MAP_LEFT)
                                            {
                                                if (battle.Map.X > 0)
                                                {
                                                    battle.Map.X--;

                                                    regenerate_scene = true;
                                                }
                                            }
                                            else if (input.Type == Controls::Type::MAP_RIGHT)
                                            {
                                                if (battle.Map.X < (battle.Map.Width - battle.Map.ViewX))
                                                {
                                                    battle.Map.X++;

                                                    regenerate_scene = true;
                                                }
                                            }
                                            else if (input.Type == Controls::Type::EXIT)
                                            {
                                                result = Battle::Result::NONE;

                                                battle.EndTurn = true;

                                                battle.NextRound = true;

                                                battle.ExitBattle = true;
                                            }

                                            input.Clear();
                                        }
                                        else if ((actions && Input::IsValid(overlay, input)) || (!actions && !items && !spells && Input::IsValid(scene, input) && Controls::Find(battle_actions, input.Type) != Controls::NotFound))
                                        {
                                            // regenerate controls
                                            if (!actions)
                                            {
                                                regenerate_scene = true;
                                            }

                                            knockout = character.Fight;

                                            fight = false;

                                            if (input.Type != Controls::Type::MOVE)
                                            {
                                                move = false;
                                            }

                                            items = false;

                                            shoot = false;

                                            spells = false;

                                            spell = false;

                                            if (input.Type == Controls::Type::MOVE)
                                            {
                                                // toggles between move / hover mode
                                                move = !move;

                                                if (actions)
                                                {
                                                    input = previous;
                                                }
                                            }
                                            else if (input.Type == Controls::Type::FIGHT || input.Type == Controls::Type::QUARTERSTAFF)
                                            {
                                                auto opponents = Engine::FightTargets(battle.Map, battle.Opponents, src, true, false);

                                                if (opponents.size() == 1)
                                                {
                                                    character.Add(Character::Status::IN_COMBAT);

                                                    Engine::ResetStatusAndSpells(character);

                                                    knockout = ((input.Type == Controls::Type::QUARTERSTAFF) && character.Has(Skills::Type::QUARTERSTAFF)) ? Skills::Type::QUARTERSTAFF : character.Fight;

                                                    // fight
                                                    Interface::Fight(graphics, scene, battle, party, character, character_id, battle.Opponents[opponents[0].Id], opponents[0].Id, knockout);

                                                    refresh_textures = true;

                                                    performed_action = true;
                                                }
                                                else if (opponents.size() > 1)
                                                {
                                                    if ((input.Type == Controls::Type::QUARTERSTAFF) && character.Has(Skills::Type::QUARTERSTAFF))
                                                    {
                                                        knockout = Skills::Type::QUARTERSTAFF;
                                                    }
                                                    else
                                                    {
                                                        knockout = character.Fight;
                                                    }

                                                    fight = true;
                                                }
                                                else
                                                {
                                                    knockout = Skills::Type::NONE;
                                                }

                                                if (actions)
                                                {
                                                    input = previous;
                                                }
                                            }
                                            else if (input.Type == Controls::Type::SHOOT || input.Type == Controls::Type::SHURIKEN)
                                            {
                                                if (battle.Has(Controls::Type::SHOOT))
                                                {
                                                    Interface::ShowBookDescription(graphics, scene, battle.ActionCancels[Controls::Type::SHOOT]);
                                                }
                                                else
                                                {
                                                    Engine::Queue targets = {};

                                                    if (shot_ambush)
                                                    {
                                                        targets = Engine::MoveTargets(battle.Map, battle.Opponents, src, true);
                                                    }
                                                    else
                                                    {
                                                        targets = Engine::RangedTargets(battle.Map, battle.Opponents, src, true, false);
                                                    }

                                                    auto target_id = targets[0].Id;

                                                    if (targets.size() == 1)
                                                    {
                                                        if (!battle.Opponents[target_id].IsImmune(character.Shoot))
                                                        {
                                                            character.Add(Character::Status::IN_COMBAT);

                                                            Engine::ResetStatusAndSpells(character);

                                                            // shoot
                                                            Interface::Shoot(graphics, scene, battle, party, character, battle.Opponents[target_id], target_id);

                                                            // checks if enthrallment is broken
                                                            Interface::CheckEnthrallment(graphics, battle, scene, character, Interface::Text[Interface::MSG_ENTHRAL]);

                                                            refresh_textures = true;

                                                            performed_action = true;
                                                        }
                                                        else
                                                        {
                                                            Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_RANGED), Color::Highlight);

                                                            regenerate_scene = true;
                                                        }
                                                    }
                                                    else if (targets.size() > 1)
                                                    {
                                                        shoot = true;
                                                    }

                                                    if (actions)
                                                    {
                                                        input = previous;
                                                    }
                                                }
                                            }
                                            else if (input.Type == Controls::Type::SPELLS)
                                            {
                                                if (battle.Has(Controls::Type::SPELLS))
                                                {
                                                    Interface::ShowBookDescription(graphics, scene, battle.ActionCancels[Controls::Type::SPELLS]);
                                                }
                                                else
                                                {
                                                    spells = true;
                                                }
                                            }
                                            else if (input.Type == Controls::Type::DEFEND)
                                            {
                                                character.Add(Character::Status::DEFENDING);

                                                Engine::ResetStatusAndSpells(character);

                                                refresh_textures = true;

                                                performed_action = true;
                                            }
                                            else if (input.Type == Controls::Type::FLEE)
                                            {
                                                if (battle.Has(Controls::Type::FLEE))
                                                {
                                                    Interface::ShowBookDescription(graphics, scene, battle.ActionCancels[Controls::Type::FLEE]);
                                                }
                                                else
                                                {
                                                    if (!character.Is(Character::Status::FLEEING))
                                                    {
                                                        character.Add(Character::Status::FLEEING);
                                                    }

                                                    Engine::ResetSpells(character);

                                                    refresh_textures = true;

                                                    performed_action = true;
                                                }
                                            }
                                            else if (input.Type == Controls::Type::ITEMS)
                                            {
                                                items = true;
                                            }
                                            else if (input.Type == Controls::Type::BACK)
                                            {
                                                if (actions)
                                                {
                                                    input = previous;
                                                }
                                            }

                                            actions = false;
                                        }
                                        else if (spells && Input::IsValid(overlay, input))
                                        {
                                            spells = false;

                                            spell = false;

                                            if (Engine::IsSpell(input.Type) && BloodSword::Has(Interface::ControlSpellMapping, input.Type))
                                            {
                                                auto &type = Interface::ControlSpellMapping[input.Type];

                                                auto search = character.Find(type);

                                                if (search != character.Spells.end())
                                                {
                                                    auto &spellbook = *search;

                                                    if (!spellbook.IsBasic() && spellbook.IsBattle)
                                                    {
                                                        if (character.HasCalledToMind(spellbook.Type))
                                                        {
                                                            if (spellbook.RequiresTarget())
                                                            {
                                                                cast = spellbook.Type;

                                                                auto targets = Engine::SpellTargets(battle.Map, battle.Opponents, src, true, false);

                                                                // cast immediate if there is only one target
                                                                if (targets.size() == 1 && cast != Spells::Type::PILLAR_OF_SALT)
                                                                {
                                                                    auto target = battle.Map.Find(Map::Object::ENEMY, targets[0].Id);

                                                                    if (battle.Map.IsValid(target))
                                                                    {
                                                                        auto distance = battle.Map.Distance(src, target);

                                                                        auto &spell_target = battle.Opponents[battle.Map[target].Id];

                                                                        if (!spellbook.Ranged && distance != 1)
                                                                        {
                                                                            // must be adjacent
                                                                            Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_ADJACENT), Color::Highlight);

                                                                            regenerate_scene = true;
                                                                        }
                                                                        else if (!spell_target.IsImmune(cast) && !spell_target.Has(Skills::Type::MAGIC_IMMUNITY))
                                                                        {
                                                                            character.Add(Character::Status::IN_COMBAT);

                                                                            if (Interface::Cast(graphics, scene, draw, map_w, map_h, character, spell_target.Asset, cast, true))
                                                                            {
                                                                                // spellcasting successful
                                                                                Interface::MessageBox(graphics, scene, std::string(Spells::TypeMapping[cast]) + " SUCCESSFULLY CAST", Color::Active);

                                                                                // resolve spell
                                                                                Interface::ResolveSpell(graphics, battle, scene, battle.Opponents, character, spell_target, battle.Map[target].Id, cast);

                                                                                refresh_textures = true;
                                                                            }
                                                                            else
                                                                            {
                                                                                // spellcasting unsuccessful!
                                                                                Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_CAST), Color::Highlight);
                                                                            }

                                                                            performed_action = true;
                                                                        }
                                                                        else
                                                                        {
                                                                            Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_SPELL), Color::Highlight);

                                                                            regenerate_scene = true;
                                                                        }
                                                                    }

                                                                    cast = Spells::Type::NONE;
                                                                }
                                                                else
                                                                {
                                                                    // select target
                                                                    spell = true;
                                                                }
                                                            }
                                                            else
                                                            {
                                                                cast = Spells::Type::NONE;

                                                                spell = false;

                                                                if (spellbook.Type == Spells::Type::IMMEDIATE_DELIVERANCE && (battle.Has(Battle::Condition::CANNOT_FLEE) || battle.Map.Find(Map::Object::EXIT).IsNone()))
                                                                {
                                                                    Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_FLEE), Color::Highlight);

                                                                    regenerate_scene = true;
                                                                }
                                                                else
                                                                {
                                                                    character.Add(Character::Status::IN_COMBAT);

                                                                    if (Interface::Cast(graphics, scene, draw, map_w, map_h, character, spellbook.Type, true))
                                                                    {
                                                                        // spellcasting successful
                                                                        Interface::MessageBox(graphics, scene, std::string(Spells::TypeMapping[spellbook.Type]) + " SUCCESSFULLY CAST", Color::Active);

                                                                        // check if spell targets own party
                                                                        auto my_party = (spellbook.Type == Spells::Type::EYE_OF_THE_TIGER) || (spellbook.Type == Spells::Type::IMMEDIATE_DELIVERANCE);

                                                                        // resolve spell
                                                                        Interface::ResolveSpell(graphics, battle, scene, character, my_party ? party : battle.Opponents, spellbook.Type);

                                                                        refresh_textures = true;
                                                                    }
                                                                    else
                                                                    {
                                                                        // spellcasting unsuccessful!
                                                                        Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_CAST), Color::Highlight);
                                                                    }

                                                                    performed_action = true;
                                                                }
                                                            }
                                                        }
                                                        else
                                                        {
                                                            // call to mind
                                                            character.CallToMind(spellbook.Type);

                                                            refresh_textures = true;

                                                            performed_action = true;
                                                        }
                                                    }
                                                }
                                                else if (actions)
                                                {
                                                    input = previous;
                                                }
                                            }
                                            else if (actions)
                                            {
                                                input = previous;
                                            }

                                            // remove highlights
                                            if (!spells && !spell)
                                            {
                                                regenerate_scene = true;
                                            }
                                        }
                                        else if (items)
                                        {
                                            auto update = Interface::ShowInventory(graphics, scene, battle, party, character, src);

                                            if (update)
                                            {
                                                refresh_textures = true;

                                                performed_action = true;
                                            }

                                            if (actions)
                                            {
                                                input = previous;
                                            }

                                            items = false;

                                            regenerate_scene = true;
                                        }
                                    }
                                }
                                else
                                {
                                    // skip this combatant (i.e. cannot perform any action)
                                    performed_action = true;
                                }
                            }
                            else
                            {
                                // skip this combatant (i.e. absent from battlefield)
                                performed_action = true;
                            }
                        }
                        else
                        {
                            // animate movement
                            animating = !Graphics::Animate(graphics, scene, movement, BloodSword::FrameDelay);

                            if (!animating)
                            {
                                // cancel fleeing status
                                Engine::Cancel(character, Character::Status::FLEEING);

                                if (is_player)
                                {
                                    battle.Map.Put(movement.Current, Map::Object::PLAYER, character_id);
                                }
                                else
                                {
                                    battle.Map.Put(movement.Current, Map::Object::ENEMY, character_id);
                                }

                                Input::Flush();

                                performed_action = true;
                            }
                        }

                        if (refresh_textures)
                        {
                            // regenerate stats
                            Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);
                        }

                        if (performed_action)
                        {
                            // clear input
                            input.Clear();

                            // next character in battle order
                            battle.NextRound = Interface::NextCharacter(battle, scene, party, battle.Order, battle.Combatant, input, battle.EndTurn);
                        }
                        else if (regenerate_scene)
                        {
                            // regenerate scene (on map movement, movement, etc.)
                            scene = Interface::BattleScene(battle, party, character, character_id, origin, shot_ambush);
                        }
                    }

                    // check if within endurance limit
                    if (battle.Endurance != Battle::Unlimited)
                    {
                        if (Engine::Min(battle.Opponents, Attribute::Type::ENDURANCE, true) <= battle.Endurance)
                        {
                            result = Battle::Result::VICTORY;

                            battle.ExitBattle = true;

                            battle.NextRound = true;

                            battle.EndTurn = true;
                        }
                        else if (Engine::Min(party, Attribute::Type::ENDURANCE, true) <= battle.Endurance)
                        {
                            result = Battle::Result::DEFEAT;

                            battle.ExitBattle = true;

                            battle.NextRound = true;

                            battle.EndTurn = true;
                        }
                    }
                }

                // cool down AWAY status
                auto cooldown_party = Interface::CoolDownStatus(party, Character::Status::AWAY);

                auto cooldown_enemy = Interface::CoolDownStatus(battle.Opponents, Character::Status::AWAY);

                if (cooldown_party || cooldown_enemy)
                {
                    // regenerate stats
                    Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);
                }

                // end of round effects
                battle.Map.CoolDown();

                // move to next round
                battle.Round++;
            }

            // round limit exceeded
            if (battle.Duration != Battle::Unlimited && battle.Round >= battle.Duration)
            {
                // regenerate final battle scene
                auto scene = BattleScene(battle, party);

                Interface::MessageBox(graphics, scene, Interface::GetText(Interface::MSG_FIGHT), Color::Active);
            }

            BloodSword::Free(&texture);

            BloodSword::Free(&round_string);

            BloodSword::Free(highlight);

            BloodSword::Free(captions);

            BloodSword::Free(party_status);

            BloodSword::Free(party_stats);

            BloodSword::Free(enemy_status);

            BloodSword::Free(enemy_stats);
        }

        // copy initial result
        auto initial_result = result;

        // handle cases where this battle's location is not recorded
        if (!battle.Has(Battle::Condition::SKIP_LOCATION))
        {
            // set location of last battle
            if (Book::IsDefined(battle.Location))
            {
                party.BattleLocation = battle.Location;
            }
            else
            {
                party.BattleLocation = party.Location;
            }
        }

        // check if party flees
        if (Engine::IsFleeing(party))
        {
            if (battle.FleeDamage.Rounds > 0)
            {
                auto scene = BattleScene(battle, party);

                Interface::DamageParty(graphics, scene, party, battle.FleeDamage.Rounds, battle.FleeDamage.Value, battle.FleeDamage.Modifier, battle.FleeDamage.IgnoreArmour, true, true);
            }

            // kill abandoned players
            Engine::KillAllParalyzed(party);

            // clear "IN BATTLE" status
            Engine::ResetAll(party);

            Engine::ResetAll(battle.Opponents);

            result = Battle::Result::FLEE;
        }
        else if (result == Battle::Result::DETERMINE)
        {
            // determine results of battle
            if (Engine::InBattle(party) == 0 && Engine::Count(party) > 0)
            {
                result = battle.Has(Battle::Condition::CANNOT_FLEE) ? Battle::Result::DEFEAT : Battle::Result::FLEE;

                Engine::KillAllParalyzed(party);
            }
            else if (Engine::IsAlive(party))
            {
                auto enthralled = Engine::Count(battle.Opponents, Character::ControlType::NPC, Character::Status::ENTHRALLED);

                auto alive = Engine::Count(battle.Opponents);

                if (enthralled > 0 && enthralled >= alive)
                {
                    result = Battle::Result::ENTHRALLED;
                }
                else
                {
                    result = Battle::Result::VICTORY;
                }
            }
            else
            {
                // paralyzed players did not make it out
                Engine::KillAllParalyzed(party);

                result = Battle::Result::DEFEAT;
            }

            // clear "IN BATTLE" status
            Engine::ResetAll(party);

            Engine::ResetAll(battle.Opponents);
        }

        if (result != Battle::Result::NONE && battle.Has(Battle::Condition::SURVIVORS))
        {
            auto survivors = 0;

            // add survivors
            for (auto i = 0; i < battle.Opponents.Count(); i++)
            {
                if (Engine::IsAlive(battle.Opponents[i]))
                {
                    party.Survivors.push_back(battle.Opponents[i]);

                    survivors++;
                }
            }

            // log battle survivors
            BattleLogger::LogSurvivors(battle.Opponents.Location, "BATTLE", "PARTY", survivors, party.Survivors.size());
        }

        if ((result == Battle::Result::VICTORY || result == Battle::Result::ENTHRALLED))
        {
            // regnerate final scene
            auto scene = BattleScene(battle, party);

            if (battle.Loot.size() > 0)
            {
                // pick up any loot
                Interface::ShowInventory(graphics, scene, party, battle.Loot);
            }

            if (party.Has(Item::Type::GOLDEN_SNUFF_BOX) && Interface::Confirm(graphics, scene, "OPEN THE GOLDEN SNUFF-BOX?", Color::Background, Color::Inactive, BloodSword::Border, Color::Active, true))
            {
                for (auto character = 0; character < party.Count(); character++)
                {
                    if (Engine::IsAlive(party[character]))
                    {
                        auto roll = Interface::Roll(graphics, scene, party[character].Asset, Asset::Type::GOLDEN_SNUFF_BOX, 1, 0).Sum;

                        if (roll)
                        {
                            auto damage = Interface::Roll(graphics, scene, party[character].Asset, Asset::Type::DAMAGE, 1, 0).Sum;

                            Engine::GainEndurance(party[character], -damage, false);

                            std::string message = party[character].Name + " LOSES " + std::to_string(damage) + " ENDURANCE";

                            Interface::MessageBox(graphics, scene, message, Color::Highlight);
                        }
                        else
                        {
                            auto heal = Interface::Roll(graphics, scene, party[character].Asset, Asset::Type::HEAL, 1, 0).Sum;

                            Engine::GainEndurance(party[character], heal, false);

                            std::string message = party[character].Name + " GAINS " + std::to_string(heal) + " ENDURANCE";

                            Interface::MessageBox(graphics, scene, message, Color::Active);
                        }
                    }
                }
            }
        }

        // log battle results
        BattleLogger::LogBattleResults(battle, party, initial_result, result);

        return result;
    }
}

#endif
