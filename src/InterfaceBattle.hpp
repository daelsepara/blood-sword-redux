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

        scene.Add(Scene::Element(Asset::Get(Asset::Type::EXIT), battle.Map.DrawX, battle.Map.DrawY + battle.Map.SizeY * battle.Map.TileSize));

        scene.Add(Controls::Base(Controls::Type::EXIT, id, id, id + 1, id - battle.Map.SizeX, id, battle.Map.DrawX, battle.Map.DrawY + battle.Map.SizeY * battle.Map.TileSize, battle.Map.TileSize, battle.Map.TileSize, Color::Active));

        return scene;
    }

    // fight battle
    Battle::Result Battle(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party)
    {
        auto result = Battle::Result::NONE;

        if (battle.Duration != 0 && battle.Opponents.Count() > 0 && party.Count() > 0)
        {
            int round = 0;

            Engine::Queue order = {};

            // start with current character
            auto combatant = 0;

            auto spells = false;

            auto actions = false;

            auto animating = false;

            auto movement = Animation::Base();

            auto scene = Interface::BattleScene(battle, party);

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
                    }

                    // spells popup
                    if (spells)
                    {
                    }

                    // actions popup
                    if (actions)
                    {
                    }

                    // move to next character in battle order
                    next = Engine::Next(order, combatant);
                }

                // end of round effects

                // move to next round
                round++;
            }
        }

        return result;
    }
}

#endif
