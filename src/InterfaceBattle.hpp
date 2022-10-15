#ifndef __INTERFACE_BATTLE_HPP__
#define __INTERFACE_BATTLE_HPP__

#include "Battle.hpp"
#include "Interface.hpp"

namespace BloodSword::Interface
{
    Scene::Base BattleActions(Point origin, int w, int h, Battle::Base &battle, Party::Base &party, int id, Uint32 background, Uint32 border, int bordersize)
    {
        auto overlay = Scene::Base();

        std::vector<Controls::Type> controls = {};

        if (party[id].ControlType == Character::ControlType::PLAYER)
        {
            auto &character = party[id];

            auto src = battle.Map.Find(Map::Object::PLAYER, id);

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
                else if (!battle.Map.Find(Map::Object::ENEMY).IsNone() && character.Has(Skills::Type::ARCHERY) && character.IsArmed(Item::Type::BOW, Item::Type::QUIVER, Item::Type::ARROW))
                {
                    controls.push_back(Controls::Type::SHOOT);
                }

                // has quarterstaff skill
                if (character.Has(Skills::Type::QUARTERSTAFF) && character.IsArmed(Item::Type::QUARTERSTAFF))
                {
                    controls.push_back(Controls::Type::QUARTERSTAFF);
                }

                if (character.Has(Skills::Type::SPELLS))
                {
                    controls.push_back(Controls::Type::SPELLS);
                }

                controls.push_back(Controls::Type::DEFEND);

                if (!battle.Map.Find(Map::Object::EXIT).IsNone() && !battle.Is(Battle::Condition::CANNOT_FLEE) && Engine::CanFlee(battle.Map, party, id))
                {
                    controls.push_back(Controls::Type::FLEE);
                }

                if (character.Items.size() > 0)
                {
                    controls.push_back(Controls::Type::ITEMS);
                }
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

    // fight battle
    Battle::Result Battle(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party)
    {
        auto result = Battle::Result::NONE;

        if (battle.Duration != 0 && battle.Opponents.Count() > 0 && party.Count() > 0)
        {
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

            auto input = Controls::User();

            while (round < battle.Duration || battle.Duration == Battle::Unlimited)
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

                    // start of character turn
                    if (Engine::IsPlayer(order, combatant))
                    {
                        Engine::CoolDown(party[order[combatant].ID]);
                    }
                    else
                    {
                        Engine::CoolDown(battle.Opponents[order[combatant].ID]);
                    }

                    // main loop
                    if (!animating)
                    {
                        auto src = battle.Map.Find(Engine::IsPlayer(order, combatant) ? Map::Object::PLAYER : Map::Object::ENEMY, order[combatant].ID);

                        if (!src.IsNone())
                        {
                            // if enemy, move to/fight/shoot at nearest target
                            if (Engine::IsEnemy(order, combatant))
                            {
                                auto &enemy = battle.Opponents[order[combatant].ID];

                                if (!enemy.Is(Character::Status::PARALYZED) && !enemy.Is(Character::Status::ENTHRALLED))
                                {
                                    // enemy combatant is not player controlled or paralyze
                                }
                                else if (enemy.Is(Character::Status::PARALYZED))
                                {
                                    next = Engine::Next(order, combatant);

                                    if (Engine::IsPlayer(order, combatant))
                                    {
                                        input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[order[combatant].ID].Class]);
                                    }

                                    continue;
                                }
                                else
                                {
                                    // enemy combatant is enthralled
                                }
                            }
                            else
                            {
                                auto &character = party[order[combatant].ID];

                                if (!character.Is(Character::Status::PARALYZED))
                                {
                                    if (spells)
                                    {
                                        // spells popup
                                    }
                                    else if (actions)
                                    {
                                        // actions popup
                                    }
                                }
                                else
                                {
                                    // move to next character in battle order
                                    next = Engine::Next(order, combatant);

                                    if (Engine::IsPlayer(order, combatant))
                                    {
                                        input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[order[combatant].ID].Class]);
                                    }
                                }
                            }
                        }
                        else
                        {
                            // move to next character in battle order
                            next = Engine::Next(order, combatant);

                            if (Engine::IsPlayer(order, combatant))
                            {
                                input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[order[combatant].ID].Class]);
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

                            if (Engine::IsPlayer(order, combatant))
                            {
                                input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[order[combatant].ID].Class]);
                            }
                        }
                    }
                }

                // end of round effects

                // move to next round
                round++;
            }

            Free(partyStats);

            Free(enemyStats);
        }

        return result;
    }
}

#endif
