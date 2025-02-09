#ifndef __INTERFACE_BATTLE_HPP__
#define __INTERFACE_BATTLE_HPP__

#include "Battle.hpp"
#include "Interface.hpp"

namespace BloodSword::Interface
{
    // find map control
    int Find(Map::Base &map, std::vector<Controls::Base> &controls, Controls::Type type, int id)
    {
        auto result = -1;

        for (auto &control : controls)
        {
            if (control.Type == type && control.OnMap && map.IsValid(control.Map) && map[control.Map].Id == id)
            {
                result = control.Id;

                break;
            }
        }

        return result;
    }

    Scene::Base BattleActions(Point origin, int w, int h, Battle::Base &battle, Party::Base &party, int id, Uint32 background, Uint32 border, int border_size)
    {
        auto overlay = Scene::Base();

        std::vector<Controls::Type> controls = {};

        auto is_player = party[id].IsPlayer();

        auto &character = party[id];

        auto src = is_player ? battle.Map.Find(Map::Object::PLAYER, id) : battle.Map.Find(Map::Object::ENEMY, id);

        if (!src.IsNone())
        {
            // can move
            if (Move::Available(battle.Map, src))
            {
                controls.push_back(Controls::Type::MOVE);
            }

            if (battle.Map.Adjacent(src, Map::Object::ENEMY))
            {
                // can fight
                controls.push_back(Controls::Type::FIGHT);

                // has quarterstaff skill
                if (character.Has(Skills::Type::QUARTERSTAFF) && character.IsArmed(Item::Type::QUARTERSTAFF))
                {
                    controls.push_back(Controls::Type::QUARTERSTAFF);
                }
            }
            else if (is_player && !battle.Map.Find(Map::Object::ENEMY).IsNone() && character.Has(Skills::Type::ARCHERY) && character.IsArmed(Item::Type::BOW, Item::Type::QUIVER, Item::Type::ARROW))
            {
                // can shoot
                controls.push_back(Controls::Type::SHOOT);
            }
            else if (!is_player && !battle.Map.Except(Map::Object::ENEMY, id).IsNone() && character.Has(Skills::Type::SHURIKEN) && character.IsArmed(Item::Type::SHURIKEN))
            {
                // can shoot shurikien
                controls.push_back(Controls::Type::SHURIKEN);
            }

            // can cast spells
            if (character.Has(Skills::Type::SPELLS))
            {
                controls.push_back(Controls::Type::SPELLS);
            }

            // defend
            controls.push_back(Controls::Type::DEFEND);

            if (is_player && !battle.Map.Find(Map::Object::EXIT).IsNone() && !battle.Is(Battle::Condition::CANNOT_FLEE) && Engine::CanFlee(battle.Map, party, id))
            {
                controls.push_back(Controls::Type::FLEE);
            }

            if (is_player && character.Items.size() > 0)
            {
                controls.push_back(Controls::Type::ITEMS);
            }
        }

        controls.push_back(Controls::Type::BACK);

        auto popup_w = (std::max(int(controls.size()), 2) + (BloodSword::SmallPad / 2)) * BloodSword::TileSize;

        auto popup_h = (5 * BloodSword::HalfTile);

        auto screen = origin + Point(w - popup_w, h - popup_h) / 2;

        overlay.Add(Scene::Element(screen, popup_w, popup_h, background, border, border_size));

        auto pad = BloodSword::QuarterTile;

        for (auto i = 0; i < controls.size(); i++)
        {
            SDL_Texture *texture = nullptr;

            texture = Asset::Get(Interface::BattleControls[controls[i]]);

            if (texture)
            {
                auto texture_w = 0;

                auto texture_h = 0;

                SDL_QueryTexture(texture, nullptr, nullptr, &texture_w, &texture_h);

                auto lt = i > 0 ? i - 1 : i;

                auto rt = i < controls.size() - 1 ? i + 1 : i;

                overlay.Add(Controls::Base(controls[i], i, lt, rt, i, i, screen.X + i * texture_w + pad, screen.Y + pad + BloodSword::HalfTile, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

                overlay.VerifyAndAdd(Scene::Element(texture, screen.X + i * texture_w + pad, screen.Y + pad + BloodSword::HalfTile));
            }
        }

        return overlay;
    }

    // regenerate map
    Scene::Base BattleScene(Battle::Base &battle, Party::Base &party)
    {
        auto scene = Interface::Map(battle.Map, party, battle.Opponents, 1);

        auto id = int(scene.Controls.size());

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::EXIT), battle.Map.DrawX, battle.Map.DrawY + map_h));

        scene.Add(Controls::Base(Controls::Type::EXIT, id, id, id + 1, id - battle.Map.ViewX, id, battle.Map.DrawX, battle.Map.DrawY + map_h, battle.Map.TileSize, battle.Map.TileSize, Color::Active));

        return scene;
    }

    // generate status
    std::vector<SDL_Texture *> GenerateStatus(Graphics::Base &graphics, Party::Base &party, bool in_battle = true)
    {
        return Interface::Status(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, in_battle);
    }

    // fight action
    bool Fight(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Character::Base &attacker, Character::Base &defender, bool knockout = false, bool ignore_armor = false)
    {
        auto alive = true;

        auto fight_w = (8 * BloodSword::TileSize);

        auto fight_h = (13 * BloodSword::QuarterTile);

        auto fight = origin + (Point(w, h) - Point(fight_w, fight_h)) / 2;

        auto damage_w = (8 * BloodSword::TileSize);

        auto damage_h = (18 * BloodSword::QuarterTile - BloodSword::Pad);

        auto damage = origin + (Point(w, h) - Point(damage_w, damage_h)) / 2;

        if (!attacker.Is(Character::Status::DEFENDING))
        {
            if (Engine::IsAlive(attacker))
            {
                auto roll = 2;

                roll += defender.Is(Character::Status::DEFENDING) ? 1 : 0;

                roll += knockout ? 1 : 0;

                roll += attacker.Has(Character::Status::NIGHTHOWL) ? 1 : 0;

                auto modifier = defender.Has(Skills::Type::DODGING) ? 1 : 0;

                if (Interface::Target(graphics, background, fight, fight_w, fight_h, Color::Active, 4, attacker, defender, Attribute::Type::FIGHTING_PROWESS, roll, modifier, true, knockout ? Asset::Type::QUARTERSTAFF : Asset::Type::NONE))
                {
                    auto hit = Interface::Damage(graphics, background, damage, damage_w, damage_h, Color::Active, 4, attacker, defender, true, false, knockout, ignore_armor, knockout ? Asset::Type::QUARTERSTAFF : Asset::Type::NONE);

                    if (knockout && hit > 0)
                    {
                        defender.Add(Character::Status::KNOCKED_OUT, 1);
                    }

                    alive &= Engine::GainEndurance(defender, hit, true);
                }
            }
        }

        return alive;
    }

    // Fight helper function
    void Fight(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Character::Base &attacker, int attacker_id, Character::Base &defender, int defender_id, bool knockout = false, bool ignore_armor = false)
    {
        auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

        auto map_w = battle.Map.ViewX * battle.Map.TileSize;

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        auto alive = true;

        alive &= Interface::Fight(graphics, background, draw, map_w, map_h, attacker, defender, knockout, ignore_armor);

        if (!alive)
        {
            battle.Map.Remove(defender.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, defender_id);

            Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(defender.Name + " KILLED!", Fonts::Normal, defender.IsPlayer() ? Color::Highlight : Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, defender.IsPlayer() ? Color::Highlight : Color::Active, 4, Color::Highlight, true);
        }
        else
        {
            alive &= Interface::Fight(graphics, background, draw, map_w, map_h, defender, attacker, false, defender.Has(Skills::Type::IGNORE_ARMOUR));

            if (!alive)
            {
                battle.Map.Remove(attacker.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, attacker_id);

                Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(attacker.Name + " KILLED!", Fonts::Normal, attacker.IsPlayer() ? Color::Highlight : Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, attacker.IsPlayer() ? Color::Highlight : Color::Active, 4, Color::Highlight, true);
            }
        }
    }

    // shoot action
    bool Shoot(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Character::Base &attacker, Character::Base &defender, bool ignore_armor = false, Asset::Type asset = Asset::Type::ARCHERY)
    {
        auto alive = true;

        auto shoot_w = (8 * BloodSword::TileSize);

        auto shoot_h = (13 * BloodSword::QuarterTile);

        auto fight = origin + (Point(w, h) - Point(shoot_w, shoot_h)) / 2;

        auto damage_w = (8 * BloodSword::TileSize);

        auto damage_h = (18 * BloodSword::QuarterTile - BloodSword::Pad);

        auto damage = origin + (Point(w, h) - Point(damage_w, damage_h)) / 2;

        if (!attacker.Is(Character::Status::DEFENDING))
        {
            if (Engine::IsAlive(attacker))
            {
                auto roll = defender.Is(Character::Status::DEFENDING) ? 3 : 2;

                roll += attacker.Has(Character::Status::NIGHTHOWL) ? 1 : 0;

                auto modifier = defender.Has(Skills::Type::DODGING) ? 1 : 0;

                if (Interface::Target(graphics, background, fight, shoot_w, shoot_h, Color::Active, 4, attacker, defender, Attribute::Type::FIGHTING_PROWESS, roll, modifier, true, asset))
                {
                    auto hit = Interface::Damage(graphics, background, damage, damage_w, damage_h, Color::Active, 4, attacker, defender, true, true, false, ignore_armor, asset);

                    alive &= Engine::GainEndurance(defender, hit, true);
                }
            }
        }

        return alive;
    }

    // shoot helper
    void Shoot(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Character::Base &attacker, Character::Base &defender, int defenderid, bool ignore_armor = false)
    {
        auto asset = Asset::Type::ARCHERY;

        auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

        auto map_w = battle.Map.ViewX * battle.Map.TileSize;

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        if (attacker.Has(Skills::Type::SHURIKEN))
        {
            asset = Asset::Type::SHURIKEN;
        }

        auto alive = Interface::Shoot(graphics, background, draw, map_w, map_h, attacker, defender, ignore_armor, asset);

        if (attacker.Has(Skills::Type::ARCHERY) && !attacker.Has(Skills::Type::SHURIKEN))
        {
            attacker.Remove(Item::Type::ARROW, 1);
        }

        if (!alive)
        {
            battle.Map.Remove(defender.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, defenderid);

            Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(defender.Name + " KILLED!", Fonts::Normal, defender.IsPlayer() ? Color::Highlight : Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, defender.IsPlayer() ? Color::Highlight : Color::Active, 4, Color::Highlight, true);
        }
    }

    // helper function (next character in battle order)
    bool NextCharacter(Battle::Base &battle, Scene::Base &scene, Party::Base &party, Engine::Queue &order, int &combatant, Controls::User &input, bool &endturn)
    {
        auto next = Engine::NextInQueue(order, combatant);

        if (Engine::IsPlayer(order, combatant))
        {
            input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[order[combatant].Id].Class]);
        }
        else if (Engine::IsEnemy(order, combatant) && battle.Opponents[order[combatant].Id].Is(Character::Status::ENTHRALLED))
        {
            input.Current = Interface::Find(battle.Map, scene.Controls, Controls::Type::ENEMY, order[combatant].Id);
        }

        endturn = true;

        return next;
    }

    // generate stats
    void RegenerateStats(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party, std::vector<SDL_Texture *> &party_stats, std::vector<SDL_Texture *> &party_status, std::vector<SDL_Texture *> &enemy_stats, std::vector<SDL_Texture *> &enemy_status)
    {
        Free(enemy_stats);

        Free(enemy_status);

        Free(party_stats);

        Free(party_status);

        auto info_w = battle.Map.TileSize * 5;

        enemy_stats = Interface::GenerateStats(graphics, battle.Opponents, info_w);

        enemy_status = Interface::GenerateStatus(graphics, battle.Opponents);

        party_stats = Interface::GenerateStats(graphics, party, info_w);

        party_status = Interface::GenerateStatus(graphics, party);
    }

    // checks if enthrallment is broken
    void CheckEnthrallment(Graphics::Base &graphics, Battle::Base &battle, Scene::Base &scene, Character::Base &character, Graphics::RichText &text)
    {
        auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

        auto map_w = battle.Map.ViewX * battle.Map.TileSize;

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        auto is_enemy = !character.IsPlayer();

        // check if enthralment is broken
        if (is_enemy && character.Is(Character::Status::ENTHRALLED))
        {
            auto roll = Engine::Roll(1, 0);

            if (roll.Sum == 6)
            {
                Interface::MessageBox(graphics, scene, draw, map_w, map_h, text, Color::Background, Color::Highlight, 4, Color::Active, true);

                character.Remove(Character::Status::ENTHRALLED);
            }
        }
    }

    // resolve spell
    void ResolveSpell(Graphics::Base &graphics, Battle::Base &battle, Scene::Base &background, Character::Base &caster, Character::Base &target, int targetid, Spells::Type spell)
    {
        auto alive = true;

        auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

        auto map_w = battle.Map.ViewX * battle.Map.TileSize;

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        auto popup_w = (8 * BloodSword::TileSize);

        auto popup_h = (18 * BloodSword::QuarterTile - BloodSword::Pad);

        auto popup = draw + (Point(map_w, map_h) - Point(popup_w, popup_h)) / 2;

        auto affected = target.Name + " SUCCUMBS TO " + std::string(Spells::TypeMapping[spell]);

        auto resisted = target.Name + " RESISTS " + std::string(Spells::TypeMapping[spell]);

        if (spell == Spells::Type::VOLCANO_SPRAY)
        {
            auto hit = Interface::Damage(graphics, background, popup, popup_w, popup_h, Color::Active, 4, caster, target, 1, 0, true, false, Spells::Assets[spell]);

            alive &= Engine::GainEndurance(target, hit, true);
        }
        else if (spell == Spells::Type::WHITE_FIRE)
        {
            auto hit = Interface::Damage(graphics, background, popup, popup_w, popup_h, Color::Active, 4, caster, target, 2, 2, true, false, Spells::Assets[spell]);

            alive &= Engine::GainEndurance(target, hit, true);
        }
        else if (spell == Spells::Type::SWORDTHRUST)
        {
            auto hit = Interface::Damage(graphics, background, popup, popup_w, popup_h, Color::Active, 4, caster, target, 3, 3, true, false, Spells::Assets[spell]);

            alive &= Engine::GainEndurance(target, hit, true);
        }
        else if (spell == Spells::Type::NEMESIS_BOLT)
        {
            auto hit = Interface::Damage(graphics, background, popup, popup_w, popup_h, Color::Active, 4, caster, target, 7, 7, true, false, Spells::Assets[spell]);

            alive &= Engine::GainEndurance(target, hit, true);
        }
        else if (spell == Spells::Type::NIGHTHOWL)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, 4, target, Attribute::Type::PSYCHIC_ABILITY, 2, 0, true, Spells::Assets[spell]))
            {
                Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(affected, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, !target.IsPlayer() ? Color::Active : Color::Highlight, 4, Color::Highlight, true);

                target.Add(Character::Status::NIGHTHOWL, 5);
            }
            else
            {
                Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(resisted, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, !target.IsPlayer() ? Color::Highlight : Color::Active, 4, Color::Highlight, true);
            }
        }
        else if (spell == Spells::Type::MISTS_OF_DEATH)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, 4, target, Attribute::Type::PSYCHIC_ABILITY, 2, 0, true, Spells::Assets[spell]))
            {
                Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(affected, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, !target.IsPlayer() ? Color::Active : Color::Highlight, 4, Color::Highlight, true);

                auto hit = Interface::Damage(graphics, background, popup, popup_w, popup_h, Color::Active, 4, caster, target, 2, 0, true, true, Spells::Assets[spell]);

                alive &= Engine::GainEndurance(target, hit, true);
            }
            else
            {
                Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(resisted, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, !target.IsPlayer() ? Color::Highlight : Color::Active, 4, Color::Highlight, true);
            }
        }
        else if (spell == Spells::Type::THE_VAMPIRE_SPELL)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, 4, target, Attribute::Type::PSYCHIC_ABILITY, 2, 0, true, Spells::Assets[spell]))
            {
                Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(affected, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, !target.IsPlayer() ? Color::Active : Color::Highlight, 4, Color::Highlight, true);

                auto hit = Interface::Damage(graphics, background, popup, popup_w, popup_h, Color::Active, 4, caster, target, 4, 0, true, false, Spells::Assets[spell]);

                alive &= Engine::GainEndurance(target, hit, true);

                // caster gains endurance
                Engine::GainEndurance(caster, -std::abs(hit / 2), true);
            }
            else
            {
                Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(resisted, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, !target.IsPlayer() ? Color::Highlight : Color::Active, 4, Color::Highlight, true);
            }
        }
        else if (spell == Spells::Type::GHASTLY_TOUCH)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, 4, target, Attribute::Type::PSYCHIC_ABILITY, 2, 0, true, Spells::Assets[spell]))
            {
                Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(affected, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, !target.IsPlayer() ? Color::Active : Color::Highlight, 4, Color::Highlight, true);

                auto hit = Interface::Damage(graphics, background, popup, popup_w, popup_h, Color::Active, 4, caster, target, 7, 0, true, true, Spells::Assets[spell]);

                alive &= Engine::GainEndurance(target, hit, true);
            }
            else
            {
                Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(resisted, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, !target.IsPlayer() ? Color::Highlight : Color::Active, 4, Color::Highlight, true);

                auto hit = Interface::Damage(graphics, background, popup, popup_w, popup_h, Color::Active, 4, caster, target, 2, 0, true, true, Spells::Assets[spell]);

                alive &= Engine::GainEndurance(target, hit, true);
            }
        }
        else if (spell == Spells::Type::SHEET_LIGHTNING)
        {
            auto hit = Interface::Damage(graphics, background, popup, popup_w, popup_h, Color::Active, 4, caster, target, 2, 2, true, false, Spells::Assets[spell]);

            alive &= Engine::GainEndurance(target, hit, true);
        }
        else if (spell == Spells::Type::SERVILE_ENTHRALMENT)
        {
            if (!Interface::Test(graphics, background, popup, popup_w, popup_h, Color::Active, 4, target, Attribute::Type::PSYCHIC_ABILITY, 2, 0, true, Spells::Assets[spell]))
            {
                Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(affected, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, !target.IsPlayer() ? Color::Active : Color::Highlight, 4, Color::Highlight, true);

                target.Add(Character::Status::ENTHRALLED);
            }
            else
            {
                Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(resisted, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, !target.IsPlayer() ? Color::Highlight : Color::Active, 4, Color::Highlight, true);
            }
        }

        if (!alive)
        {
            battle.Map.Remove(target.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, targetid);

            Interface::MessageBox(graphics, background, draw, map_w, map_h, Graphics::RichText(target.Name + " KILLED!", Fonts::Normal, target.IsPlayer() ? Color::Highlight : Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, target.IsPlayer() ? Color::Highlight : Color::Active, 4, Color::Highlight, true);
        }
    }

    // multiple targets
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
                    Interface::ResolveSpell(graphics, battle, background, caster, targets[target], target, spell);
                }
            }
        }
        else if (spell == Spells::Type::EYE_OF_THE_TIGER)
        {
            std::vector<Graphics::RichText> tiger_eye =
                {Graphics::RichText("CHARACTER FPR/DMG ROLLS +2", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                 Graphics::RichText("    PARTY FPR/DMG ROLLS +1", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0)};

            auto popup = draw + (Point(map_w, map_h) - Point((8 * BloodSword::TileSize), (BloodSword::TileSize * 2 + BloodSword::QuarterTile - BloodSword::SmallPad))) / 2;

            auto tiger = Interface::Choice(graphics, background, tiger_eye, popup, (8 * BloodSword::TileSize), BloodSword::TileSize, 2, Color::Background, Color::Inactive, Color::Active, true);

            if (tiger == 0)
            {
                caster.Add(Character::Status::FPR_PLUS2, 5);
            }
            else if (tiger == 1)
            {
                for (auto character = 0; character < targets.Count(); character++)
                {
                    if (Engine::IsAlive(targets[character]))
                    {
                        targets[character].Add(Character::Status::FPR_PLUS1, 5);
                    }
                }
            }
        }
        else if (spell == Spells::Type::IMMEDIATE_DELIVERANCE)
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

    // fight battle
    Battle::Result RenderBattle(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party)
    {
        auto result = Battle::Result::DETERMINE;

        if (battle.Duration != 0 && Engine::IsAlive(battle.Opponents) && Engine::IsAlive(party))
        {
            // set party starting locations
            for (auto i = 0; i < party.Count(); i++)
            {
                battle.Map.Put(battle.Map.Origins[i], Map::Object::PLAYER, i);
            }

            // set opponents starting locations
            for (auto i = 0; i < battle.Opponents.Count(); i++)
            {
                battle.Map.Put(battle.Map.Spawn[i], Map::Object::ENEMY, i);
            }

            // initialize captions
            auto caption_w = (5 * BloodSword::TileSize);

            auto captions = Graphics::CreateText(
                graphics,
                {Graphics::RichText("EXIT", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, caption_w)});

            auto infow = battle.Map.TileSize * 5;

            auto enemy_stats = Interface::GenerateStats(graphics, battle.Opponents, infow);

            auto enemy_status = Interface::GenerateStatus(graphics, battle.Opponents, infow);

            auto party_stats = Interface::GenerateStats(graphics, party, infow);

            auto party_status = Interface::GenerateStatus(graphics, party, infow);

            SDL_Texture *texture = nullptr;

            auto asset = Asset::Type::NONE;

            auto lifetime = -1;

            std::vector<Graphics::RichText> text = {
                Graphics::RichText("SELECT OPPONENT", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                Graphics::RichText("SELECT TARGET", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                Graphics::RichText("SELECT DESTINATION", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                Graphics::RichText("ENTHRALMENT BROKEN!", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                Graphics::RichText("THERE ARE NEARBY ENEMIES!", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                Graphics::RichText("CANNOT MOVE THERE!", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                Graphics::RichText("CASTER MUST BE ADJACENT TO TARGET!", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                Graphics::RichText("CASTING ATTEMPT WAS UNSUCCESSFUL!", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                Graphics::RichText("YOU CANNOT FLEE THIS BATTLE!", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0)};

            auto messages = Graphics::CreateText(graphics, text);

            int round = 0;

            auto round_string = Graphics::CreateText(graphics, (std::string("ROUND: ") + std::to_string(round + 1)).c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

            auto spells = false;

            auto cast = Spells::Type::NONE;

            auto actions = false;

            auto animating = false;

            auto movement = Animation::Base();

            auto previous = Controls::User();

            auto input = Controls::User();

            auto move = false;

            auto exit = false;

            // fight targetting
            auto fight = false;

            // quarterstaff targetting
            auto knockout = false;

            // shoot targetting
            auto shoot = false;

            // spell targetting
            auto spell = false;

            auto pad = BloodSword::OddPad;

            auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

            auto info_x = battle.Map.DrawX + (battle.Map.ViewX * 2 + 1) * battle.Map.TileSize / 2 + pad;

            auto info_y = battle.Map.DrawY + pad;

            auto map_w = battle.Map.ViewX * battle.Map.TileSize;

            auto map_h = battle.Map.ViewY * battle.Map.TileSize;

            // set "IN BATTLE" status
            for (auto member = 0; member < party.Count(); member++)
            {
                party[member].Add(Character::Status::IN_BATTLE);
            }

            for (auto member = 0; member < battle.Opponents.Count(); member++)
            {
                battle.Opponents[member].Add(Character::Status::IN_BATTLE);
            }

            // focus/cursor blink variables
            Uint64 blink_start = SDL_GetTicks64();

            Uint64 blink_delay = 100;

            auto blinking = false;

            while ((round < battle.Duration || battle.Duration == Battle::Unlimited) && Engine::IsAlive(party) && Engine::IsAlive(battle.Opponents) && !Engine::IsFleeing(party) && !exit)
            {
                // battle order
                Engine::Queue order = {};

                if (round == 0 && battle.Is(Battle::Condition::AMBUSH_PLAYER))
                {
                    // players get a free initial turn
                    order = Engine::Build(party, Attribute::Type::AWARENESS, true, true);
                }
                else if (round == 0 && battle.Is(Battle::Condition::AMBUSH_NPC))
                {
                    // enemy combatants get a free initial turn
                    order = Engine::Build(battle.Opponents, Attribute::Type::AWARENESS, true, true);
                }
                else
                {
                    // otherwise create battle order (default)
                    order = Engine::Build(party, battle.Opponents, Attribute::Type::AWARENESS, true, true);
                }

                // start of round effects
                auto next = false;

                // start with current character
                auto combatant = 0;

                // end of round effects
                battle.Map.CoolDown();

                // regenerate scene
                auto scene = Interface::BattleScene(battle, party);

                while (!next && Engine::IsAlive(party) && Engine::IsAlive(battle.Opponents) && !Engine::IsFleeing(party) && !exit)
                {
                    auto is_enemy = Engine::IsEnemy(order, combatant);

                    auto is_player = Engine::IsPlayer(order, combatant);

                    auto &character = is_player ? party[order[combatant].Id] : battle.Opponents[order[combatant].Id];

                    // start of character turn
                    if (round > 0 && Engine::CoolDown(character))
                    {
                        // regenerate stats
                        Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);
                    }

                    auto end_turn = false;

                    while (!end_turn && Engine::IsAlive(party) && Engine::IsAlive(battle.Opponents) && !Engine::IsFleeing(party) && !exit)
                    {
                        auto overlay = Scene::Base();

                        // main loop
                        if (!animating)
                        {
                            auto src = battle.Map.Find(Engine::IsPlayer(order, combatant) ? Map::Object::PLAYER : Map::Object::ENEMY, order[combatant].Id);

                            if (!src.IsNone())
                            {
                                // if enemy, move to/fight/shoot at nearest target
                                if (is_enemy && !character.Is(Character::Status::ENTHRALLED))
                                {
                                    // enemy combatant is not paralyzed
                                    if (!character.Is(Character::Status::PARALYZED) && character.Is(Character::Status::IN_BATTLE) && Engine::IsAlive(character))
                                    {
                                        // check if there are adjacent combatants to fight
                                        auto opponents = Engine::FightTargets(battle.Map, party, src, true, false);

                                        if (opponents.size() > 0)
                                        {
                                            if (character.Has(Skills::Type::SPELLS))
                                            {
                                                character.ResetSpellComplexities();
                                            }

                                            // check if armor is ignored
                                            auto ignore_armor = character.Has(Skills::Type::IGNORE_ARMOUR);

                                            // fight
                                            Interface::Fight(graphics, scene, battle, character, order[combatant].Id, party[opponents[0].Id], opponents[0].Id, false, ignore_armor);

                                            // regenerate scene
                                            scene = Interface::BattleScene(battle, party);

                                            // regenerate stats
                                            Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);

                                            // next character in battle order
                                            next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                        }
                                        else if (character.Has(Skills::Type::ARCHERY) || character.Has(Skills::Type::SHURIKEN))
                                        {
                                            auto targets = Engine::RangedTargets(battle.Map, party, src, true, false);

                                            if (targets.size() > 0)
                                            {
                                                // check if armor is ignored
                                                auto ignore_armor = character.Has(Skills::Type::IGNORE_ARMOUR);

                                                // shoot
                                                Interface::Shoot(graphics, scene, battle, character, party[targets[0].Id], targets[0].Id, ignore_armor);

                                                // regenerate scene
                                                scene = Interface::BattleScene(battle, party);

                                                // regenerate stats
                                                Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);
                                            }

                                            // next character in battle order
                                            next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                        }
                                        else if (character.Has(Skills::Type::SPELLS))
                                        {
                                            // TODO: enemy cast spells
                                            next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                        }
                                        else if (character.Moves > 0 && Move::Available(battle.Map, src))
                                        {
                                            // check if enemy can move towards the player-controlled characters
                                            auto targets = Engine::MoveTargets(battle.Map, party, src, true, false);

                                            auto valid_target = false;

                                            for (auto &target : targets)
                                            {
                                                auto end = battle.Map.Find(Map::Object::PLAYER, target.Id);

                                                if (!end.IsNone())
                                                {
                                                    valid_target = Interface::Move(battle.Map, character, movement, src, end);

                                                    if (valid_target)
                                                    {
                                                        // regenerate scene
                                                        scene = Interface::BattleScene(battle, party);

                                                        animating = true;

                                                        break;
                                                    }
                                                }
                                            }

                                            if (!valid_target)
                                            {
                                                // next character in battle order
                                                next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                            }
                                        }
                                        else
                                        {
                                            // no valid moves
                                            next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                        }
                                    }
                                    else
                                    {
                                        // next character in battle order
                                        next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                    }
                                }
                                else
                                {
                                    // player-controlled characters including enthralled enemies
                                    if (!character.Is(Character::Status::PARALYZED) && character.Is(Character::Status::IN_BATTLE) && Engine::IsAlive(character))
                                    {
                                        // captions and overlays
                                        if (!spells && !actions)
                                        {
                                            auto &control = scene.Controls[input.Current];

                                            if (control.OnMap && battle.Map.IsValid(control.Map))
                                            {
                                                // draw path to destination
                                                if (move)
                                                {
                                                    auto src = is_player ? battle.Map.Find(Map::Object::PLAYER, order[combatant].Id) : battle.Map.Find(Map::Object::ENEMY, order[combatant].Id);

                                                    auto dst = control.Map;

                                                    overlay = Interface::Path(battle.Map, character, src, dst);

                                                    overlay.VerifyAndAdd(Scene::Element(messages[2], battle.Map.DrawX, battle.Map.TileSize / 2));
                                                }
                                                else if (fight)
                                                {
                                                    // fight mode
                                                    overlay.VerifyAndAdd(Scene::Element(messages[0], battle.Map.DrawX, battle.Map.TileSize / 2));
                                                }
                                                else if (shoot || spell)
                                                {
                                                    // shoot mode
                                                    overlay.VerifyAndAdd(Scene::Element(messages[1], battle.Map.DrawX, battle.Map.TileSize / 2));
                                                }
                                                else
                                                {
                                                    // round number
                                                    overlay.VerifyAndAdd(Scene::Element(round_string, battle.Map.DrawX, battle.Map.TileSize / 2));
                                                }

                                                // show character stats
                                                if (battle.Map[control.Map].Occupant == Map::Object::PLAYER)
                                                {
                                                    if (battle.Map[control.Map].Id >= 0 && battle.Map[control.Map].Id < party.Count())
                                                    {
                                                        // stats
                                                        overlay.VerifyAndAdd(Scene::Element(party_stats[battle.Map[control.Map].Id], info_x, info_y, Color::Background, Color::Active, 4));

                                                        auto &stats = overlay.Elements.back();

                                                        // status
                                                        overlay.VerifyAndAdd(Scene::Element(party_status[battle.Map[control.Map].Id], info_x, info_y + stats.H + pad * 4, Color::Background, Color::Active, 4));
                                                    }
                                                }
                                                else if (battle.Map[control.Map].Occupant == Map::Object::ENEMY)
                                                {
                                                    if (battle.Map[control.Map].Id >= 0 && battle.Map[control.Map].Id < battle.Opponents.Count())
                                                    {
                                                        // enemy stats
                                                        overlay.VerifyAndAdd(Scene::Element(enemy_stats[battle.Map[control.Map].Id], info_x, info_y, Color::Background, Color::Active, 4));

                                                        auto &stats = overlay.Elements.back();

                                                        // status
                                                        overlay.VerifyAndAdd(Scene::Element(enemy_status[battle.Map[control.Map].Id], info_x, info_y + stats.H + pad * 4, Color::Background, Color::Active, 4));
                                                    }
                                                }
                                                else if (battle.Map[control.Map].IsTemporarilyBlocked())
                                                {
                                                    if (asset != battle.Map[control.Map].TemporaryAsset || lifetime != battle.Map[control.Map].Lifetime)
                                                    {
                                                        asset = battle.Map[control.Map].TemporaryAsset;

                                                        lifetime = battle.Map[control.Map].Lifetime;

                                                        std::string text = " OBSTACLE (" + std::to_string(battle.Map[control.Map].Lifetime) + ") ";

                                                        Free(&texture);

                                                        texture = Graphics::CreateText(graphics, text.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);
                                                    }

                                                    overlay.VerifyAndAdd(Scene::Element(texture, info_x, info_y, Color::Background, Color::Inactive, 4));
                                                }
                                            }
                                            else
                                            {
                                                // round number
                                                overlay.VerifyAndAdd(Scene::Element(round_string, battle.Map.DrawX, battle.Map.TileSize / 2));

                                                // captions for other controls
                                                auto exit_id = Controls::Find(scene.Controls, Controls::Type::EXIT);

                                                auto caption = control.Id - exit_id;

                                                if (caption >= 0 && caption < captions.size())
                                                {
                                                    auto control = exit_id + caption;

                                                    overlay.VerifyAndAdd(Scene::Element(captions[caption], scene.Controls[control].X, scene.Controls[control].Y + scene.Controls[control].H + BloodSword::Pad));
                                                }
                                            }
                                        }
                                        else if (spells)
                                        {
                                            // spells popup
                                            overlay = Interface::Spells(draw, map_w, map_h, character, Color::Background, Color::Active, 4, true);

                                            // round number
                                            overlay.VerifyAndAdd(Scene::Element(round_string, battle.Map.DrawX, battle.Map.TileSize / 2));

                                            if (Input::IsValid(overlay, input))
                                            {
                                                if (input.Type != Controls::Type::BACK)
                                                {
                                                    auto &control = overlay.Controls[input.Current];

                                                    auto &spell = character.Spells[control.Id];

                                                    auto &popup = overlay.Elements[0];

                                                    if (character.HasCalledToMind(spell.Type) && spell.IsBattle && !spell.IsBasic())
                                                    {
                                                        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CAST_SPELL), popup.X + popup.W - (BloodSword::TileSize + BloodSword::Pad), popup.Y + BloodSword::Pad));
                                                        overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsActive[spell.Type], control.X, control.Y + control.H + pad));
                                                        overlay.VerifyAndAdd(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CAST_SPELL], popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));
                                                    }
                                                    else if (!spell.IsBasic() && spell.IsBattle)
                                                    {
                                                        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CALL_TO_MIND), popup.X + popup.W - (BloodSword::TileSize + BloodSword::Pad), popup.Y + BloodSword::Pad));
                                                        overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsInactive[spell.Type], control.X, control.Y + control.H + pad));
                                                        overlay.VerifyAndAdd(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CALL_TO_MIND], popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));
                                                    }
                                                    else
                                                    {
                                                        overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsInactive[spell.Type], control.X, control.Y + control.H + pad));
                                                    }
                                                }
                                            }
                                        }
                                        else if (actions)
                                        {
                                            // actions popup
                                            overlay = Interface::BattleActions(draw, map_w, map_h, battle, is_player ? party : battle.Opponents, order[combatant].Id, Color::Background, Color::Active, 4);

                                            // round number
                                            overlay.VerifyAndAdd(Scene::Element(round_string, battle.Map.DrawX, battle.Map.TileSize / 2));

                                            if (Input::IsValid(overlay, input))
                                            {
                                                // actions popup captions
                                                auto &control = overlay.Controls[input.Current];

                                                auto ptr = Interface::BattleControlCaptions.find(control.Type);

                                                if (ptr != Interface::BattleControlCaptions.end())
                                                {
                                                    overlay.VerifyAndAdd(Scene::Element(ptr->second, control.X, control.Y + control.H + pad));
                                                }
                                            }
                                        }

                                        // focus cursor
                                        if (!actions && !spells)
                                        {
                                            if (blinking)
                                            {
                                                // focus cursor
                                                Interface::Focus(battle.Map, order, combatant, overlay);
                                            }

                                            input.Blink = false;
                                        }
                                        else
                                        {
                                            input.Blink = !input.Blink;
                                        }

                                        // wait for input
                                        input = Input::WaitForInput(graphics, scene, overlay, input, actions || spells, true, 0);

                                        if (!actions && !spells)
                                        {
                                            auto blink_end = SDL_GetTicks64();

                                            if (blink_end - blink_start >= blink_delay)
                                            {
                                                blinking = !blinking;

                                                blink_start = blink_end;
                                            }
                                        }

                                        if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
                                        {
                                            if (Input::IsValid(scene, input) && !actions && !spells)
                                            {
                                                auto &control = scene.Controls[input.Current];

                                                if (control.OnMap && battle.Map[scene.Controls[input.Current].Map].Id == order[combatant].Id && ((is_player && battle.Map[scene.Controls[input.Current].Map].IsPlayer()) || (is_enemy && battle.Map[scene.Controls[input.Current].Map].IsEnemy())))
                                                {
                                                    previous = input;

                                                    actions = true;

                                                    spells = false;

                                                    move = false;

                                                    fight = false;

                                                    knockout = false;

                                                    shoot = false;

                                                    spell = false;

                                                    input.Current = -1;
                                                }
                                                else if (control.OnMap && battle.Map.IsValid(control.Map) && input.Type == Controls::Type::LOCATION)
                                                {
                                                    // setup animation
                                                    if (move)
                                                    {
                                                        auto opponents = Engine::FightTargets(battle.Map, battle.Opponents, src, true, false);

                                                        auto &control = scene.Controls[input.Current];

                                                        if (opponents.size() == 0 || (opponents.size() > 0 && (is_enemy || (is_player && character.Is(Character::Status::DEFENDED)))))
                                                        {
                                                            auto start = battle.Map.Find(Engine::IsPlayer(order, combatant) ? Map::Object::PLAYER : Map::Object::ENEMY, order[combatant].Id);

                                                            auto end = control.Map;

                                                            // find a path to the destination and animate
                                                            animating = Interface::Move(battle.Map, character, movement, start, end);

                                                            if (animating)
                                                            {
                                                                if (character.Is(Character::Status::FLEEING))
                                                                {
                                                                    character.Remove(Character::Status::FLEEING);
                                                                }

                                                                // regenerate scene
                                                                scene = Interface::BattleScene(battle, party);
                                                            }
                                                            else
                                                            {
                                                                // no route to destination
                                                                Interface::MessageBox(graphics, scene, draw, map_w, map_h, messages[5], Color::Background, Color::Highlight, 4, Color::Active, true);
                                                            }
                                                        }
                                                        else
                                                        {
                                                            // enemies nearby
                                                            Interface::MessageBox(graphics, scene, draw, map_w, map_h, messages[4], Color::Background, Color::Highlight, 4, Color::Active, true);
                                                        }

                                                        move = !move;
                                                    }
                                                    else if (spell && cast == Spells::Type::PILLAR_OF_SALT)
                                                    {
                                                        if (Interface::Cast(graphics, scene, draw, map_w, map_h, character, cast, true))
                                                        {
                                                            // spellcasting successful
                                                            auto spell_string = std::string(Spells::TypeMapping[cast]) + " SUCESSFULLY CAST";

                                                            Interface::MessageBox(graphics, scene, draw, map_w, map_h, Graphics::RichText(spell_string, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Active, 4, Color::Highlight, true);

                                                            // resolve spell
                                                            battle.Map.Put(control.Map, Map::Object::TEMPORARY_OBSTACLE, Asset::Type::PILLAR_OF_SALT, 5);

                                                            // regenerate stats
                                                            Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);

                                                            // regenerate scene
                                                            scene = Interface::BattleScene(battle, party);
                                                        }
                                                        else
                                                        {
                                                            // spellcasting unsuccessful!
                                                            Interface::MessageBox(graphics, scene, draw, map_w, map_h, messages[7], Color::Background, Color::Highlight, 4, Color::Highlight, true);
                                                        }

                                                        // next character in battle order
                                                        next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);

                                                        spell = false;

                                                        cast = Spells::Type::NONE;
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
                                                            if (character.Is(Character::Status::FLEEING))
                                                            {
                                                                character.Remove(Character::Status::FLEEING);
                                                            }

                                                            if (character.Is(Character::Status::DEFENDED))
                                                            {
                                                                character.Remove(Character::Status::DEFENDED);
                                                            }

                                                            if (character.Has(Skills::Type::SPELLS))
                                                            {
                                                                character.ResetSpellComplexities();
                                                            }

                                                            // check if armor is ignored
                                                            auto ignore_armor = character.Has(Skills::Type::IGNORE_ARMOUR);

                                                            // fight
                                                            Interface::Fight(graphics, scene, battle, character, order[combatant].Id, battle.Opponents[battle.Map[control.Map].Id], battle.Map[control.Map].Id, false, ignore_armor);

                                                            // regenerate scene
                                                            scene = Interface::BattleScene(battle, party);

                                                            // regenerate stats
                                                            Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);

                                                            // checks if enthrallment is broken
                                                            Interface::CheckEnthrallment(graphics, battle, scene, character, text[3]);

                                                            // next character in battle order
                                                            next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                                        }

                                                        fight = false;

                                                        knockout = false;
                                                    }
                                                    else if (shoot)
                                                    {
                                                        // target chosen
                                                        auto distance = battle.Map.Distance(src, control.Map);

                                                        if (distance > 1)
                                                        {
                                                            if (character.Is(Character::Status::FLEEING))
                                                            {
                                                                character.Remove(Character::Status::FLEEING);
                                                            }

                                                            if (character.Is(Character::Status::DEFENDED))
                                                            {
                                                                character.Remove(Character::Status::DEFENDED);
                                                            }

                                                            // check if armor is ignored
                                                            auto ignore_armor = character.Has(Skills::Type::IGNORE_ARMOUR);

                                                            // shoot
                                                            Interface::Shoot(graphics, scene, battle, character, battle.Opponents[battle.Map[control.Map].Id], battle.Map[control.Map].Id, ignore_armor);

                                                            // regenerate scene
                                                            scene = Interface::BattleScene(battle, party);

                                                            // regenerate stats
                                                            Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);

                                                            // checks if enthrallment is broken
                                                            Interface::CheckEnthrallment(graphics, battle, scene, character, text[3]);

                                                            // next character in battle order
                                                            next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                                        }
                                                        else
                                                        {
                                                            // enemies nearby
                                                            Interface::MessageBox(graphics, scene, draw, map_w, map_h, messages[4], Color::Background, Color::Highlight, 4, Color::Active, true);
                                                        }

                                                        shoot = false;
                                                    }
                                                    else if (spell && cast != Spells::Type::NONE && cast != Spells::Type::PILLAR_OF_SALT)
                                                    {
                                                        spell = false;

                                                        auto spellbook = character.Find(cast);

                                                        if (spellbook != character.Spells.end())
                                                        {
                                                            auto distance = battle.Map.Distance(src, control.Map);

                                                            if (!spellbook->Ranged && distance != 1)
                                                            {
                                                                // must be adjacent
                                                                Interface::MessageBox(graphics, scene, messages[6], Color::Background, Color::Highlight, 4, Color::Highlight, true);
                                                            }
                                                            else
                                                            {
                                                                if (Interface::Cast(graphics, scene, draw, map_w, map_h, character, cast, true))
                                                                {
                                                                    // spellcasting successful
                                                                    auto spell_string = std::string(Spells::TypeMapping[cast]) + " SUCESSFULLY CAST";

                                                                    Interface::MessageBox(graphics, scene, draw, map_w, map_h, Graphics::RichText(spell_string, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Active, 4, Color::Highlight, true);

                                                                    // resolve spell
                                                                    Interface::ResolveSpell(graphics, battle, scene, character, battle.Opponents[battle.Map[control.Map].Id], battle.Map[control.Map].Id, cast);

                                                                    // regenerate stats
                                                                    Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);
                                                                }
                                                                else
                                                                {
                                                                    // spellcasting unsuccessful!
                                                                    Interface::MessageBox(graphics, scene, draw, map_w, map_h, messages[7], Color::Background, Color::Highlight, 4, Color::Highlight, true);
                                                                }

                                                                // regenerate scene
                                                                scene = Interface::BattleScene(battle, party);

                                                                // next character in battle order
                                                                next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                                            }
                                                        }

                                                        cast = Spells::Type::NONE;
                                                    }
                                                }
                                                else if (input.Type == Controls::Type::EXIT)
                                                {
                                                    next = true;

                                                    end_turn = true;

                                                    exit = true;

                                                    result = Battle::Result::NONE;
                                                }
                                            }
                                            else if (actions && Input::IsValid(overlay, input))
                                            {
                                                if (input.Type == Controls::Type::MOVE)
                                                {
                                                    // toggles between move/hover mode
                                                    move = !move;
                                                }
                                                else if (input.Type == Controls::Type::FIGHT || input.Type == Controls::Type::QUARTERSTAFF)
                                                {
                                                    auto opponents = Engine::FightTargets(battle.Map, battle.Opponents, src, true, false);

                                                    if (opponents.size() == 1)
                                                    {
                                                        if (character.Is(Character::Status::FLEEING))
                                                        {
                                                            character.Remove(Character::Status::FLEEING);
                                                        }

                                                        if (character.Is(Character::Status::DEFENDED))
                                                        {
                                                            character.Remove(Character::Status::DEFENDED);
                                                        }

                                                        if (character.Has(Skills::Type::SPELLS))
                                                        {
                                                            character.ResetSpellComplexities();
                                                        }

                                                        fight = false;

                                                        knockout = input.Type == Controls::Type::QUARTERSTAFF;

                                                        // check if armor is ignored
                                                        auto ignore_armor = character.Has(Skills::Type::IGNORE_ARMOUR);

                                                        // fight
                                                        Interface::Fight(graphics, scene, battle, character, order[combatant].Id, battle.Opponents[opponents[0].Id], opponents[0].Id, knockout, ignore_armor);

                                                        // regenerate scene
                                                        scene = Interface::BattleScene(battle, party);

                                                        // regenerate stats
                                                        Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);

                                                        // next character in battle order
                                                        next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                                    }
                                                    else if (opponents.size() > 1)
                                                    {
                                                        knockout = (input.Type == Controls::Type::QUARTERSTAFF);

                                                        fight = true;

                                                        input = previous;
                                                    }
                                                    else
                                                    {
                                                        fight = false;

                                                        knockout = false;

                                                        input = previous;
                                                    }
                                                }
                                                else if (input.Type == Controls::Type::SHOOT || input.Type == Controls::Type::SHURIKEN)
                                                {
                                                    auto targets = Engine::RangedTargets(battle.Map, battle.Opponents, src, true, false);

                                                    if (targets.size() == 1)
                                                    {
                                                        if (character.Is(Character::Status::FLEEING))
                                                        {
                                                            character.Remove(Character::Status::FLEEING);
                                                        }

                                                        if (character.Is(Character::Status::DEFENDED))
                                                        {
                                                            character.Remove(Character::Status::DEFENDED);
                                                        }

                                                        if (character.Has(Skills::Type::SPELLS))
                                                        {
                                                            character.ResetSpellComplexities();
                                                        }

                                                        shoot = false;

                                                        // check if armor is ignored
                                                        auto ignore_armor = character.Has(Skills::Type::IGNORE_ARMOUR);

                                                        // shoot
                                                        Interface::Shoot(graphics, scene, battle, character, battle.Opponents[targets[0].Id], targets[0].Id, ignore_armor);

                                                        // regenerate scene
                                                        scene = Interface::BattleScene(battle, party);

                                                        // regenerate stats
                                                        Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);

                                                        // checks if enthrallment is broken
                                                        Interface::CheckEnthrallment(graphics, battle, scene, character, text[3]);

                                                        // next character in battle order
                                                        next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                                    }
                                                    else if (targets.size() > 1)
                                                    {
                                                        shoot = true;

                                                        input = previous;
                                                    }
                                                    else
                                                    {
                                                        shoot = false;

                                                        input = previous;
                                                    }
                                                }
                                                else if (input.Type == Controls::Type::SPELLS)
                                                {
                                                    spells = true;
                                                }
                                                else if (input.Type == Controls::Type::DEFEND)
                                                {
                                                    if (character.Is(Character::Status::DEFENDED))
                                                    {
                                                        character.Remove(Character::Status::DEFENDED);
                                                    }

                                                    if (!character.Is(Character::Status::DEFENDING))
                                                    {
                                                        character.Add(Character::Status::DEFENDING);
                                                    }

                                                    if (character.Has(Skills::Type::SPELLS))
                                                    {
                                                        character.ResetSpellComplexities();
                                                    }

                                                    Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);

                                                    // next character in battle order
                                                    next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                                }
                                                else if (input.Type == Controls::Type::FLEE)
                                                {
                                                    if (!character.Is(Character::Status::FLEEING))
                                                    {
                                                        character.Add(Character::Status::FLEEING);
                                                    }

                                                    if (character.Has(Skills::Type::SPELLS))
                                                    {
                                                        character.ResetSpellComplexities();
                                                    }

                                                    Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);

                                                    // next character in battle order
                                                    next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                                }
                                                else if (input.Type == Controls::Type::BACK)
                                                {
                                                    input = previous;
                                                }

                                                actions = false;
                                            }
                                            else if (spells && Input::IsValid(overlay, input))
                                            {
                                                auto ptr = Interface::ControlSpellMapping.find(input.Type);

                                                spells = false;

                                                if (Engine::IsSpell(input.Type) && ptr != Interface::ControlSpellMapping.end())
                                                {
                                                    auto &type = ptr->second;

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
                                                                    // select target
                                                                    spell = true;

                                                                    cast = spellbook.Type;

                                                                    // unless there is only one valid target
                                                                    auto targets = Engine::RangedTargets(battle.Map, battle.Opponents, src, true, false);

                                                                    if (targets.size() == 1 && cast != Spells::Type::PILLAR_OF_SALT)
                                                                    {
                                                                        auto target = battle.Map.Find(Map::Object::ENEMY, targets[0].Id);

                                                                        if (battle.Map.IsValid(target))
                                                                        {
                                                                            auto distance = battle.Map.Distance(src, target);

                                                                            if (!spellbook.Ranged && distance != 1)
                                                                            {
                                                                                // must be adjacent
                                                                                Interface::MessageBox(graphics, scene, messages[6], Color::Background, Color::Highlight, 4, Color::Highlight, true);
                                                                            }
                                                                            else
                                                                            {
                                                                                if (Interface::Cast(graphics, scene, draw, map_w, map_h, character, cast, true))
                                                                                {
                                                                                    // spellcasting successful
                                                                                    Interface::MessageBox(graphics, scene, draw, map_w, map_h, Graphics::RichText(std::string(Spells::TypeMapping[cast]) + " SUCESSFULLY CAST", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Active, 4, Color::Highlight, true);

                                                                                    // resolve spell
                                                                                    Interface::ResolveSpell(graphics, battle, scene, character, battle.Opponents[battle.Map[target].Id], battle.Map[target].Id, cast);

                                                                                    // regenerate stats
                                                                                    Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);
                                                                                }
                                                                                else
                                                                                {
                                                                                    // spellcasting unsuccessful!
                                                                                    Interface::MessageBox(graphics, scene, draw, map_w, map_h, messages[7], Color::Background, Color::Highlight, 4, Color::Highlight, true);
                                                                                }

                                                                                // regenerate scene
                                                                                scene = Interface::BattleScene(battle, party);

                                                                                // next character in battle order
                                                                                next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                                                            }
                                                                        }

                                                                        spell = false;

                                                                        cast = Spells::Type::NONE;
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    spell = false;

                                                                    cast = Spells::Type::NONE;

                                                                    if (spellbook.Type == Spells::Type::IMMEDIATE_DELIVERANCE && (battle.Is(Battle::Condition::CANNOT_FLEE) || battle.Map.Find(Map::Object::EXIT).IsNone()))
                                                                    {
                                                                        Interface::MessageBox(graphics, scene, draw, map_w, map_h, messages[8], Color::Background, Color::Highlight, 4, Color::Highlight, true);
                                                                    }
                                                                    else
                                                                    {
                                                                        if (Interface::Cast(graphics, scene, draw, map_w, map_h, character, spellbook.Type, true))
                                                                        {
                                                                            // spellcasting successful
                                                                            Interface::MessageBox(graphics, scene, draw, map_w, map_h, Graphics::RichText(std::string(Spells::TypeMapping[spellbook.Type]) + " SUCESSFULLY CAST", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Active, 4, Color::Highlight, true);

                                                                            // check if spell targets own party
                                                                            auto my_party = (spellbook.Type == Spells::Type::EYE_OF_THE_TIGER) || (spellbook.Type == Spells::Type::IMMEDIATE_DELIVERANCE);

                                                                            // resolve spell
                                                                            Interface::ResolveSpell(graphics, battle, scene, character, my_party ? party : battle.Opponents, spellbook.Type);

                                                                            // regenerate stats
                                                                            Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);

                                                                            // regenerate scene
                                                                            scene = Interface::BattleScene(battle, party);
                                                                        }
                                                                        else
                                                                        {
                                                                            // spellcasting unsuccessful!
                                                                            Interface::MessageBox(graphics, scene, draw, map_w, map_h, messages[7], Color::Background, Color::Highlight, 4, Color::Highlight, true);
                                                                        }

                                                                        // next character in battle order
                                                                        next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                                                    }
                                                                }
                                                            }
                                                            else
                                                            {
                                                                // call to mind
                                                                character.CallToMind(spellbook.Type);

                                                                // regenerate stats
                                                                Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);

                                                                // next character in battle order
                                                                next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        input = previous;
                                                    }
                                                }
                                                else
                                                {
                                                    input = previous;
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        // next character in battle order
                                        next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                                    }
                                }
                            }
                            else
                            {
                                // next character in battle order
                                next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                            }
                        }
                        else
                        {
                            animating = !Graphics::Animate(graphics, scene, movement, BloodSword::StandardDelay);

                            if (!animating)
                            {
                                // cancel fleeing status
                                if (character.Is(Character::Status::FLEEING))
                                {
                                    character.Remove(Character::Status::FLEEING);
                                }

                                if (is_player)
                                {
                                    battle.Map.Put(movement.Current, Map::Object::PLAYER, order[combatant].Id);
                                }
                                else
                                {
                                    battle.Map.Put(movement.Current, Map::Object::ENEMY, order[combatant].Id);
                                }

                                Input::Flush();

                                input.Current = -1;

                                input.Selected = false;

                                // regenerate scene
                                scene = Interface::BattleScene(battle, party);

                                // next character in battle order
                                next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                            }
                        }
                    }
                }

                // move to next round
                round++;

                // regenerate round string
                Free(&round_string);

                round_string = Graphics::CreateText(graphics, (std::string("ROUND: ") + std::to_string(round + 1)).c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);
            }

            // check if party flees
            if (Engine::IsFleeing(party))
            {
                result = Battle::Result::FLEE;
            }

            Free(&texture);

            Free(&round_string);

            Free(captions);

            Free(party_status);

            Free(party_stats);

            Free(enemy_status);

            Free(enemy_stats);

            Free(messages);
        }

        // clear "IN BATTLE" status
        for (auto member = 0; member < party.Count(); member++)
        {
            party[member].Remove(Character::Status::IN_BATTLE);

            if (party[member].Has(Skills::Type::SPELLS))
            {
                party[member].ResetSpellComplexities();
            }
        }

        for (auto member = 0; member < battle.Opponents.Count(); member++)
        {
            battle.Opponents[member].Remove(Character::Status::IN_BATTLE);

            if (battle.Opponents[member].Has(Skills::Type::SPELLS))
            {
                battle.Opponents[member].ResetSpellComplexities();
            }
        }

        // determine results of battle
        if (result == Battle::Result::DETERMINE)
        {
            if (Engine::IsAlive(party))
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
                result = Battle::Result::DEFEAT;
            }
        }

        return result;
    }
}

#endif
