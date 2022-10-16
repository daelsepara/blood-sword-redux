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
                result = control.ID;

                break;
            }
        }

        return result;
    }

    Scene::Base BattleActions(Point origin, int w, int h, Battle::Base &battle, Party::Base &party, int id, Uint32 background, Uint32 border, int bordersize)
    {
        auto overlay = Scene::Base();

        std::vector<Controls::Type> controls = {};

        auto isplayer = party[id].ControlType == Character::ControlType::PLAYER;

        auto &character = party[id];

        auto src = isplayer ? battle.Map.Find(Map::Object::PLAYER, id) : battle.Map.Find(Map::Object::ENEMY, id);

        if (!src.IsNone())
        {
            // can move
            if (Move::Available(battle.Map, src))
            {
                controls.push_back(Controls::Type::MOVE);
            }

            // can fight
            if (battle.Map.Adjacent(src, Map::Object::ENEMY))
            {
                controls.push_back(Controls::Type::FIGHT);
            }
            else if (isplayer && !battle.Map.Find(Map::Object::ENEMY).IsNone() && character.Has(Skills::Type::ARCHERY) && character.IsArmed(Item::Type::BOW, Item::Type::QUIVER, Item::Type::ARROW))
            {
                controls.push_back(Controls::Type::SHOOT);
            }
            else if (!isplayer && !battle.Map.Except(Map::Object::ENEMY, id).IsNone() && character.Has(Skills::Type::SHOOT_SHURIKEN) && character.IsArmed(Item::Type::SHURIKEN))
            {
                controls.push_back(Controls::Type::SHOOT_SHURIKEN);
            }

            // has quarterstaff skill
            if (character.Has(Skills::Type::QUARTERSTAFF) && character.IsArmed(Item::Type::QUARTERSTAFF))
            {
                controls.push_back(Controls::Type::QUARTERSTAFF);
            }

            // can cast spells
            if (character.Has(Skills::Type::SPELLS))
            {
                controls.push_back(Controls::Type::SPELLS);
            }

            // defend
            controls.push_back(Controls::Type::DEFEND);

            if (isplayer && !battle.Map.Find(Map::Object::EXIT).IsNone() && !battle.Is(Battle::Condition::CANNOT_FLEE) && Engine::CanFlee(battle.Map, party, id))
            {
                controls.push_back(Controls::Type::FLEE);
            }

            if (isplayer && character.Items.size() > 0)
            {
                controls.push_back(Controls::Type::ITEMS);
            }
        }

        controls.push_back(Controls::Type::BACK);

        auto popupw = (std::max((int)(controls.size()), 2) + 2) * 64;

        auto popuph = 160;

        auto screen = origin + Point(w - popupw, h - popuph) / 2;

        overlay.Add(Scene::Element(screen, popupw, popuph, background, border, bordersize));

        auto pad = 16;

        for (auto i = 0; i < controls.size(); i++)
        {
            SDL_Texture *texture = NULL;

            texture = Asset::Get(Interface::BattleControls[controls[i]]);

            if (texture)
            {
                auto texturew = 0;

                auto textureh = 0;

                SDL_QueryTexture(texture, NULL, NULL, &texturew, &textureh);

                auto lt = i > 0 ? i - 1 : i;

                auto rt = i < controls.size() - 1 ? i + 1 : i;

                overlay.Add(Controls::Base(controls[i], i, lt, rt, i, i, screen.X + i * texturew + pad, screen.Y + pad + 32, 64, 64, Color::Highlight));

                overlay.Add(Scene::Element(texture, screen.X + i * texturew + pad, screen.Y + pad + 32));
            }
        }

        return overlay;
    }

    // regenerate map
    Scene::Base BattleScene(Battle::Base &battle, Party::Base &party)
    {
        auto scene = Interface::Map(battle.Map, party, battle.Opponents, 1);

        auto id = scene.Controls.size();

        scene.Add(Scene::Element(Asset::Get(Asset::Type::EXIT), battle.Map.DrawX, battle.Map.DrawY + battle.Map.ViewY * battle.Map.TileSize));

        scene.Add(Controls::Base(Controls::Type::EXIT, id, id, id + 1, id - battle.Map.ViewX, id, battle.Map.DrawX, battle.Map.DrawY + battle.Map.ViewY * battle.Map.TileSize, battle.Map.TileSize, battle.Map.TileSize, Color::Active));

        return scene;
    }

    // generate stats
    std::vector<SDL_Texture *> GenerateStats(Graphics::Base &graphics, Party::Base &party, int width, bool names = true, bool inbattle = true)
    {
        return Interface::Attributes(graphics, party, Fonts::Normal, Color::Active, Color::Highlight, TTF_STYLE_NORMAL, width, names, inbattle);
    }

    // fight action
    bool Fight(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Character::Base &attacker, Character::Base &defender)
    {
        auto alive = true;

        auto fightw = 512;

        auto fighth = 208;

        auto fight = origin + (Point(w, h) - Point(fightw, fighth)) / 2;

        auto damagew = 512;

        auto damageh = 280;

        auto damage = origin + (Point(w, h) - Point(damagew, damageh)) / 2;

        if (!attacker.Is(Character::Status::DEFENDING))
        {
            if (Engine::IsAlive(attacker))
            {
                auto roll = defender.Is(Character::Status::DEFENDING) ? 3 : 2;

                if (Interface::Test(graphics, background, fight, fightw, fighth, Color::Active, 4, attacker, Attribute::Type::FIGHTING_PROWESS, roll, 0, true))
                {
                    auto hit = Interface::Damage(graphics, background, damage, damagew, damageh, Color::Active, 4, attacker, defender, true);

                    alive &= Engine::Damage(defender, hit, true);
                }
            }
        }

        return alive;
    }

    // shoot action
    bool Shoot(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Character::Base &attacker, Character::Base &defender, Asset::Type asset)
    {
        auto alive = true;

        auto shootw = 512;

        auto shooth = 208;

        auto fight = origin + (Point(w, h) - Point(shootw, shooth)) / 2;

        auto damagew = 512;

        auto damageh = 280;

        auto damage = origin + (Point(w, h) - Point(damagew, damageh)) / 2;

        if (!attacker.Is(Character::Status::DEFENDING))
        {
            if (Engine::IsAlive(attacker))
            {
                auto roll = defender.Is(Character::Status::DEFENDING) ? 3 : 2;

                if (Interface::Test(graphics, background, fight, shootw, shooth, Color::Active, 4, attacker, Attribute::Type::FIGHTING_PROWESS, roll, 0, true, true, asset))
                {
                    auto hit = Interface::Damage(graphics, background, damage, damagew, damageh, Color::Active, 4, attacker, defender, true, true, asset);

                    alive &= Engine::Damage(defender, hit, true);
                }
            }
        }

        return alive;
    }

    // fight battle
    Battle::Result Battle(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party)
    {
        auto result = Battle::Result::NONE;

        if (battle.Duration != 0 && Engine::IsAlive(battle.Opponents) && Engine::IsAlive(party))
        {
            // initialize captions
            auto captionw = 320;

            auto captions = Graphics::CreateText(
                graphics,
                {Graphics::RichText("Exit", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, captionw)});

            auto enemyStats = GenerateStats(graphics, battle.Opponents, battle.Map.TileSize * 5);

            auto partyStats = GenerateStats(graphics, party, battle.Map.TileSize * 5);

            int round = 0;

            Engine::Queue order = {};

            // start with current character
            auto combatant = 0;

            auto spells = false;

            auto actions = false;

            auto animating = false;

            auto movement = Animation::Base();

            auto scene = Interface::BattleScene(battle, party);

            auto move = false;

            auto input = Controls::User();

            auto pad = 10;

            auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

            auto infox = battle.Map.DrawX + (battle.Map.ViewX * 2 + 1) * battle.Map.TileSize / 2 + pad;

            auto infoy = battle.Map.DrawY + pad;

            while ((round < battle.Duration || battle.Duration == Battle::Unlimited) && Engine::IsAlive(party) && Engine::IsAlive(battle.Opponents) && Engine::IsFleeing(party))
            {
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

                while (!next)
                {
                    auto overlay = Scene::Base();

                    auto isenemy = Engine::IsEnemy(order, combatant);

                    auto isplayer = Engine::IsPlayer(order, combatant);

                    auto &character = isplayer ? party[order[combatant].ID] : battle.Opponents[order[combatant].ID];

                    // start of character turn
                    Engine::CoolDown(character);

                    // main loop
                    if (!animating)
                    {
                        auto src = battle.Map.Find(Engine::IsPlayer(order, combatant) ? Map::Object::PLAYER : Map::Object::ENEMY, order[combatant].ID);

                        if (!src.IsNone())
                        {
                            // if enemy, move to/fight/shoot at nearest target
                            if (isenemy && !character.Is(Character::Status::ENTHRALLED))
                            {
                                if (!character.Is(Character::Status::PARALYZED))
                                {
                                    // enemy combatant is not paralyzed
                                }
                                else if (character.Is(Character::Status::PARALYZED))
                                {
                                    next = Engine::Next(order, combatant);

                                    if (isplayer)
                                    {
                                        input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[character.Class]);
                                    }
                                    else if (isenemy && character.Is(Character::Status::ENTHRALLED))
                                    {
                                        input.Current = Interface::Find(battle.Map, scene.Controls, Controls::Type::ENEMY, order[combatant].ID);
                                    }
                                }
                            }
                            else
                            {
                                if (!character.Is(Character::Status::PARALYZED))
                                {
                                    if (!spells && !actions && Input::IsValid(scene, input))
                                    {
                                        auto &control = scene.Controls[input.Current];

                                        if (control.OnMap)
                                        {
                                            // draw path to destination
                                            if (move)
                                            {
                                                auto src = isplayer ? battle.Map.Find(Map::Object::PLAYER, order[combatant].ID) : battle.Map.Find(Map::Object::ENEMY, order[combatant].ID);

                                                auto dst = control.Map;

                                                overlay = Interface::Path(battle.Map, character, src, dst);
                                            }

                                            if (battle.Map[control.Map].Occupant == Map::Object::PLAYER)
                                            {
                                                if (battle.Map[control.Map].Id >= 0 && battle.Map[control.Map].Id < party.Count())
                                                {
                                                    // stats
                                                    overlay.Add(Scene::Element(partyStats[battle.Map[control.Map].Id], infox, infoy, Color::Background, Color::Active, 4));
                                                }
                                            }
                                            else if (battle.Map[control.Map].Occupant == Map::Object::ENEMY)
                                            {
                                                if (battle.Map[control.Map].Id >= 0 && battle.Map[control.Map].Id < battle.Opponents.Count())
                                                {
                                                    // enemy stats
                                                    overlay.Add(Scene::Element(enemyStats[battle.Map[control.Map].Id], infox, infoy, Color::Background, Color::Active, 4));
                                                }
                                            }
                                        }
                                        else
                                        {
                                            // captions for other controls
                                            auto exitid = Controls::Find(scene.Controls, Controls::Type::EXIT);

                                            auto caption = control.ID - exitid;

                                            if (caption >= 0 && caption < captions.size())
                                            {
                                                auto control = exitid + caption;

                                                overlay.Add(Scene::Element(captions[caption], scene.Controls[control].X, scene.Controls[control].Y + scene.Controls[control].H + 8));
                                            }
                                        }
                                    }
                                    else if (spells)
                                    {
                                        // spells popup
                                        overlay = Interface::Spells(draw, battle.Map.ViewX * battle.Map.TileSize, battle.Map.ViewY * battle.Map.TileSize, character, Color::Background, Color::Active, 4, true);
                                    }
                                    else if (actions)
                                    {
                                        // actions popup
                                        overlay = Interface::BattleActions(draw, battle.Map.ViewX * battle.Map.TileSize, battle.Map.ViewY * battle.Map.TileSize, battle, isplayer ? party : battle.Opponents, order[combatant].ID, Color::Background, Color::Active, 4);
                                    }
                                }
                                else
                                {
                                    // move to next character in battle order
                                    next = Engine::Next(order, combatant);

                                    if (isplayer)
                                    {
                                        input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[character.Class]);
                                    }
                                    else if (isenemy && character.Is(Character::Status::ENTHRALLED))
                                    {
                                        input.Current = Interface::Find(battle.Map, scene.Controls, Controls::Type::ENEMY, order[combatant].ID);
                                    }
                                }
                            }
                        }
                        else
                        {
                            // move to next character in battle order
                            next = Engine::Next(order, combatant);

                            if (isplayer)
                            {
                                input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[character.Class]);
                            }
                            else if (isenemy && character.Is(Character::Status::ENTHRALLED))
                            {
                                input.Current = Interface::Find(battle.Map, scene.Controls, Controls::Type::ENEMY, order[combatant].ID);
                            }
                        }
                    }
                    else
                    {
                        animating = !Graphics::Animate(graphics, scene, movement);

                        Graphics::Refresh(graphics);

                        if (!animating)
                        {
                            if (Engine::IsPlayer(order, combatant))
                            {
                                battle.Map.Put(movement.Current, Map::Object::PLAYER, order[combatant].ID);
                            }
                            else
                            {
                                battle.Map.Put(movement.Current, Map::Object::ENEMY, order[combatant].ID);
                            }

                            Input::Flush();

                            input.Current = -1;

                            input.Selected = false;

                            scene = Interface::BattleScene(battle, party);

                            next = Engine::Next(order, combatant);

                            if (isplayer)
                            {
                                input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[character.Class]);
                            }
                            else if (isenemy && character.Is(Character::Status::ENTHRALLED))
                            {
                                input.Current = Interface::Find(battle.Map, scene.Controls, Controls::Type::ENEMY, order[combatant].ID);
                            }
                        }
                    }
                }

                // end of round effects

                // move to next round
                round++;
            }

            // check if party flees
            if (Engine::IsFleeing(party))
            {
                result = Battle::Result::FLEE;
            }

            Free(captions);

            Free(partyStats);

            Free(enemyStats);
        }

        // determine results of battle
        if (result == Battle::Result::DETERMINE)
        {
            if (Engine::IsAlive(party))
            {
                result = Battle::Result::VICTORY;
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
