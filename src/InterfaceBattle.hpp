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

    Scene::Base BattleActions(Point origin, int w, int h, Battle::Base &battle, Party::Base &party, int id, Uint32 background, Uint32 border, int bordersize)
    {
        auto overlay = Scene::Base();

        std::vector<Controls::Type> controls = {};

        auto isplayer = party[id].IsPlayer();

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

        auto maph = battle.Map.ViewY * battle.Map.TileSize;

        scene.Add(Scene::Element(Asset::Get(Asset::Type::EXIT), battle.Map.DrawX, battle.Map.DrawY + maph));

        scene.Add(Controls::Base(Controls::Type::EXIT, id, id, id + 1, id - battle.Map.ViewX, id, battle.Map.DrawX, battle.Map.DrawY + maph, battle.Map.TileSize, battle.Map.TileSize, Color::Active));

        return scene;
    }

    // generate stats
    std::vector<SDL_Texture *> GenerateStats(Graphics::Base &graphics, Party::Base &party, int width, bool names = true, bool inbattle = true)
    {
        return Interface::Attributes(graphics, party, Fonts::Normal, Color::Active, Color::Highlight, TTF_STYLE_NORMAL, width, names, inbattle);
    }

    // generate status
    std::vector<SDL_Texture *> GenerateStatus(Graphics::Base &graphics, Party::Base &party, int width, bool inbattle = true)
    {
        return Interface::Status(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, width, inbattle);
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

                auto modifier = defender.Has(Skills::Type::DODGING) ? 1 : 0;

                if (Interface::Test(graphics, background, fight, fightw, fighth, Color::Active, 4, attacker, Attribute::Type::FIGHTING_PROWESS, roll, modifier, true))
                {
                    auto hit = Interface::Damage(graphics, background, damage, damagew, damageh, Color::Active, 4, attacker, defender, true);

                    alive &= Engine::Damage(defender, hit, true);
                }
            }
        }

        return alive;
    }

    // Fight helper function
    void Fight(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Character::Base &attacker, int attackerid, Character::Base &defender, int defenderid)
    {
        auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

        auto mapw = battle.Map.ViewX * battle.Map.TileSize;

        auto maph = battle.Map.ViewY * battle.Map.TileSize;

        auto alive = true;

        alive &= Interface::Fight(graphics, background, draw, mapw, maph, attacker, defender);

        if (!alive)
        {
            battle.Map.Remove(defender.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, defenderid);
        }
        else
        {
            alive &= Interface::Fight(graphics, background, draw, mapw, maph, defender, attacker);

            if (!alive)
            {
                battle.Map.Remove(attacker.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, attackerid);
            }
        }
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

                auto modifier = defender.Has(Skills::Type::DODGING) ? 1 : 0;

                if (Interface::Test(graphics, background, fight, shootw, shooth, Color::Active, 4, attacker, Attribute::Type::FIGHTING_PROWESS, roll, modifier, true, true, asset))
                {
                    auto hit = Interface::Damage(graphics, background, damage, damagew, damageh, Color::Active, 4, attacker, defender, true, true, asset);

                    alive &= Engine::Damage(defender, hit, true);
                }
            }
        }

        return alive;
    }

    // shoot helper
    void Shoot(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Character::Base &attacker, Character::Base &defender, int defenderid)
    {
        auto asset = Asset::Type::ARCHERY;

        auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

        auto mapw = battle.Map.ViewX * battle.Map.TileSize;

        auto maph = battle.Map.ViewY * battle.Map.TileSize;

        if (!attacker.IsPlayer() && attacker.Has(Skills::Type::SHOOT_SHURIKEN))
        {
            asset = Asset::Type::SHOOT_SHURIKEN;
        }

        auto alive = Interface::Shoot(graphics, background, draw, mapw, maph, attacker, defender, asset);

        if (attacker.IsPlayer() && attacker.Has(Skills::Type::ARCHERY))
        {
            attacker.Remove(Item::Type::ARROW, 1);
        }

        if (!alive)
        {
            battle.Map.Remove(defender.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, defenderid);
        }
    }

    // helper function (next character in battle order)
    bool Next(Battle::Base &battle, Scene::Base &scene, Party::Base &party, Engine::Queue &order, int &combatant, Controls::User &input, bool &endturn)
    {
        auto next = Engine::Next(order, combatant);

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
    void RegenerateStats(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party, std::vector<SDL_Texture *> partyStats, std::vector<SDL_Texture *> partyStatus, std::vector<SDL_Texture *> enemyStats, std::vector<SDL_Texture *> enemyStatus)
    {
        Free(enemyStats);

        Free(enemyStatus);

        Free(partyStats);

        Free(partyStatus);

        auto infow = battle.Map.TileSize * 5;

        enemyStats = Interface::GenerateStats(graphics, battle.Opponents, infow);

        enemyStatus = Interface::GenerateStatus(graphics, battle.Opponents, infow);

        partyStats = Interface::GenerateStats(graphics, party, infow);

        partyStatus = Interface::GenerateStatus(graphics, party, infow);
    }

    // checks if enthrallment is broken
    void CheckEnthrallment(Graphics::Base &graphics, Scene::Base &scene, Character::Base &character, Graphics::RichText &text)
    {
        auto isenemy = !character.IsPlayer();

        // check if enthralment is broken
        if (isenemy && character.Is(Character::Status::ENTHRALLED))
        {
            auto roll = Engine::Roll(1, 0);

            if (roll.Sum == 6)
            {
                Interface::MessageBox(graphics, scene, text, Color::Background, Color::Highlight, 4, Color::Active, true);

                character.Remove(Character::Status::ENTHRALLED);
            }
        }
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

            auto infow = battle.Map.TileSize * 5;

            auto enemyStats = Interface::GenerateStats(graphics, battle.Opponents, infow);

            auto enemyStatus = Interface::GenerateStatus(graphics, battle.Opponents, infow);

            auto partyStats = Interface::GenerateStats(graphics, party, infow);

            auto partyStatus = Interface::GenerateStatus(graphics, party, infow);

            std::vector<Graphics::RichText> text = {
                Graphics::RichText("SELECT OPPONENT", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                Graphics::RichText("SELECT TARGET", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                Graphics::RichText("SELECT DESTINATION", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                Graphics::RichText("ENTHRALMENT BROKEN!", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0)};

            auto textures = Graphics::CreateText(graphics, text);

            int round = 0;

            Engine::Queue order = {};

            // start with current character
            auto combatant = 0;

            auto spells = false;

            auto actions = false;

            auto animating = false;

            auto scene = Interface::BattleScene(battle, party);

            auto movement = Animation::Base();

            auto previous = Controls::User();

            auto input = Controls::User();

            auto move = false;

            auto exit = false;

            // fight targetting
            auto fight = false;

            // shoot targetting
            auto shoot = false;

            // spell targetting
            auto spell = false;

            auto pad = 10;

            auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

            auto infox = battle.Map.DrawX + (battle.Map.ViewX * 2 + 1) * battle.Map.TileSize / 2 + pad;

            auto infoy = battle.Map.DrawY + pad;

            auto mapw = battle.Map.ViewX * battle.Map.TileSize;

            auto maph = battle.Map.ViewY * battle.Map.TileSize;

            // set "IN BATTLE" status
            for (auto member = 0; member < party.Count(); member++)
            {
                party[member].Add(Character::Status::IN_BATTLE);
            }

            for (auto member = 0; member < battle.Opponents.Count(); member++)
            {
                battle.Opponents[member].Add(Character::Status::IN_BATTLE);
            }

            while ((round < battle.Duration || battle.Duration == Battle::Unlimited) && Engine::IsAlive(party) && Engine::IsAlive(battle.Opponents) && !Engine::IsFleeing(party) && !exit)
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
                    auto isenemy = Engine::IsEnemy(order, combatant);

                    auto isplayer = Engine::IsPlayer(order, combatant);

                    auto &character = isplayer ? party[order[combatant].Id] : battle.Opponents[order[combatant].Id];

                    auto blinking = false;

                    // start of character turn
                    Engine::CoolDown(character);

                    auto endturn = false;

                    while (!endturn)
                    {
                        auto overlay = Scene::Base();

                        // main loop
                        if (!animating)
                        {
                            auto src = battle.Map.Find(Engine::IsPlayer(order, combatant) ? Map::Object::PLAYER : Map::Object::ENEMY, order[combatant].Id);

                            if (!src.IsNone())
                            {
                                // if enemy, move to/fight/shoot at nearest target
                                if (isenemy && !character.Is(Character::Status::ENTHRALLED))
                                {
                                    // enemy combatant is not paralyzed
                                    if (!character.Is(Character::Status::PARALYZED))
                                    {
                                        // check if there are adjacent combatants to fight
                                        auto opponents = Engine::FightTargets(battle.Map, party, src, true, false);

                                        if (opponents.size() > 0)
                                        {
                                            // fight
                                            Interface::Fight(graphics, scene, battle, character, order[combatant].Id, party[opponents[0].Id], opponents[0].Id);

                                            // regenerate stats
                                            Interface::RegenerateStats(graphics, battle, party, partyStats, partyStatus, enemyStats, enemyStatus);

                                            // next character in battle order
                                            next = Interface::Next(battle, scene, party, order, combatant, input, endturn);
                                        }
                                        else if (character.Moves > 0 && Move::Available(battle.Map, src))
                                        {
                                            // check if enemy can move towards the player-controlled characters
                                            auto targets = Engine::MoveTargets(battle.Map, party, src, true, false);

                                            if (targets.size() > 0)
                                            {
                                                auto validtarget = false;

                                                for (auto &target : targets)
                                                {
                                                    auto end = battle.Map.Find(Map::Object::PLAYER, target.Id);

                                                    if (!end.IsNone())
                                                    {
                                                        validtarget = Interface::Move(battle.Map, character, movement, src, end);

                                                        if (validtarget)
                                                        {
                                                            scene = Interface::BattleScene(battle, party);

                                                            animating = true;

                                                            break;
                                                        }
                                                    }
                                                }

                                                if (!validtarget)
                                                {
                                                    // next character in battle order
                                                    next = Interface::Next(battle, scene, party, order, combatant, input, endturn);
                                                }
                                            }
                                        }
                                        else if (character.Has(Skills::Type::ARCHERY) || character.Has(Skills::Type::SHOOT_SHURIKEN))
                                        {
                                            auto targets = Engine::ShotTargets(battle.Map, party, src, true, false);

                                            if (targets.size() > 0)
                                            {
                                                // shoot
                                                Interface::Shoot(graphics, scene, battle, character, party[targets[0].Id], targets[0].Id);

                                                // regenerate stats
                                                Interface::RegenerateStats(graphics, battle, party, partyStats, partyStatus, enemyStats, enemyStatus);

                                                // next character in battle order
                                                next = Interface::Next(battle, scene, party, order, combatant, input, endturn);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        // next character in battle order
                                        next = Interface::Next(battle, scene, party, order, combatant, input, endturn);
                                    }
                                }
                                else
                                {
                                    if (!character.Is(Character::Status::PARALYZED))
                                    {
                                        // captions
                                        if (!spells && !actions && Input::IsValid(scene, input))
                                        {
                                            auto &control = scene.Controls[input.Current];

                                            if (control.OnMap)
                                            {
                                                // draw path to destination
                                                if (move)
                                                {
                                                    auto src = isplayer ? battle.Map.Find(Map::Object::PLAYER, order[combatant].Id) : battle.Map.Find(Map::Object::ENEMY, order[combatant].Id);

                                                    auto dst = control.Map;

                                                    overlay = Interface::Path(battle.Map, character, src, dst);

                                                    overlay.Add(Scene::Element(textures[2], battle.Map.DrawX, battle.Map.TileSize / 2));
                                                }
                                                else if (fight)
                                                {
                                                    // fight mode
                                                    overlay.Add(Scene::Element(textures[0], battle.Map.DrawX, battle.Map.TileSize / 2));
                                                }
                                                else if (shoot)
                                                {
                                                    // shoot mode
                                                    overlay.Add(Scene::Element(textures[1], battle.Map.DrawX, battle.Map.TileSize / 2));
                                                }

                                                // show character stats
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

                                                auto caption = control.Id - exitid;

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
                                            overlay = Interface::Spells(draw, mapw, maph, character, Color::Background, Color::Active, 4, true);
                                        }
                                        else if (actions)
                                        {
                                            // actions popup
                                            overlay = Interface::BattleActions(draw, mapw, maph, battle, isplayer ? party : battle.Opponents, order[combatant].Id, Color::Background, Color::Active, 4);
                                        }

                                        if (!move && !fight && !shoot && !spell && !actions && !spells)
                                        {
                                            if (blinking)
                                            {
                                                // focus cursor
                                                Interface::Focus(battle.Map, order, combatant, overlay);
                                            }

                                            blinking = !blinking;

                                            input.Blink = false;
                                        }
                                        else
                                        {
                                            input.Blink = !input.Blink;
                                        }

                                        input = Input::WaitForInput(graphics, scene, overlay, input, actions || spells, true);

                                        if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
                                        {
                                            if (!actions && !fight && !shoot && !spells)
                                            {
                                                if (Input::IsValid(scene, input) && battle.Map[scene.Controls[input.Current].Map].Id == order[combatant].Id)
                                                {
                                                    previous = input;

                                                    actions = true;

                                                    spells = false;

                                                    move = false;

                                                    fight = false;

                                                    shoot = false;

                                                    spell = false;

                                                    input.Current = -1;
                                                }
                                                else if (input.Type == Controls::Type::DESTINATION)
                                                {
                                                    // setup animation
                                                    if (move)
                                                    {
                                                        if (Input::IsValid(scene, input))
                                                        {
                                                            auto &control = scene.Controls[input.Current];

                                                            if (control.OnMap && battle.Map.IsValid(control.Map))
                                                            {
                                                                auto start = battle.Map.Find(Engine::IsPlayer(order, combatant) ? Map::Object::PLAYER : Map::Object::ENEMY, order[combatant].Id);

                                                                auto end = control.Map;

                                                                // find a path to the destination
                                                                animating = Interface::Move(battle.Map, character, movement, start, end);

                                                                if (animating)
                                                                {
                                                                    scene = Interface::BattleScene(battle, party);
                                                                }
                                                            }
                                                        }

                                                        move = !move;
                                                    }
                                                }
                                                else if (input.Type == Controls::Type::ENEMY && Input::IsValid(scene, input))
                                                {
                                                    auto &control = scene.Controls[input.Current];

                                                    if (fight)
                                                    {
                                                        // opponent chosen
                                                        auto distance = battle.Map.Distance(src, control.Map);

                                                        if (distance == 1)
                                                        {
                                                            // fight
                                                            Interface::Fight(graphics, scene, battle, character, order[combatant].Id, battle.Opponents[battle.Map[control.Map].Id], battle.Map[control.Map].Id);

                                                            // regenerate stats
                                                            Interface::RegenerateStats(graphics, battle, party, partyStats, partyStatus, enemyStats, enemyStatus);

                                                            // checks if enthrallment is broken
                                                            Interface::CheckEnthrallment(graphics, scene, character, text[3]);

                                                            // next character in battle order
                                                            next = Interface::Next(battle, scene, party, order, combatant, input, endturn);
                                                        }

                                                        fight = false;
                                                    }
                                                    else if (shoot)
                                                    {
                                                        // target chosen
                                                        auto distance = battle.Map.Distance(src, control.Map);

                                                        if (distance == 1)
                                                        {
                                                            // shoot
                                                            Interface::Shoot(graphics, scene, battle, character, battle.Opponents[battle.Map[control.Map].Id], battle.Map[control.Map].Id);

                                                            // regenerate stats
                                                            Interface::RegenerateStats(graphics, battle, party, partyStats, partyStatus, enemyStats, enemyStatus);

                                                            // checks if enthrallment is broken
                                                            Interface::CheckEnthrallment(graphics, scene, character, text[3]);

                                                            // next character in battle order
                                                            next = Interface::Next(battle, scene, party, order, combatant, input, endturn);
                                                        }

                                                        shoot = false;
                                                    }
                                                }
                                                else if (input.Type == Controls::Type::EXIT)
                                                {
                                                    next = true;

                                                    endturn = true;

                                                    exit = true;
                                                }
                                            }
                                            else if (actions)
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
                                                        fight = false;

                                                        // fight
                                                        Interface::Fight(graphics, scene, battle, character, order[combatant].Id, party[opponents[0].Id], opponents[0].Id);

                                                        // regenerate stats
                                                        Interface::RegenerateStats(graphics, battle, party, partyStats, partyStatus, enemyStats, enemyStatus);

                                                        // next character in battle order
                                                        next = Interface::Next(battle, scene, party, order, combatant, input, endturn);
                                                    }
                                                    else if (opponents.size() > 1)
                                                    {
                                                        fight = true;
                                                    }
                                                    else
                                                    {
                                                        fight = false;
                                                    }
                                                }
                                                else if (input.Type == Controls::Type::SHOOT || input.Type == Controls::Type::SHOOT_SHURIKEN)
                                                {
                                                    auto targets = Engine::ShotTargets(battle.Map, battle.Opponents, src, true, false);

                                                    if (targets.size() == 1)
                                                    {
                                                        shoot = false;

                                                        // shoot
                                                        Interface::Shoot(graphics, scene, battle, character, battle.Opponents[targets[0].Id], targets[0].Id);

                                                        // regenerate stats
                                                        Interface::RegenerateStats(graphics, battle, party, partyStats, partyStatus, enemyStats, enemyStatus);

                                                        // checks if enthrallment is broken
                                                        Interface::CheckEnthrallment(graphics, scene, character, text[3]);

                                                        // next character in battle order
                                                        next = Interface::Next(battle, scene, party, order, combatant, input, endturn);
                                                    }
                                                    else if (targets.size() > 1)
                                                    {
                                                        shoot = true;
                                                    }
                                                    else
                                                    {
                                                        shoot = false;
                                                    }
                                                }
                                                else if (input.Type == Controls::Type::SPELLS)
                                                {
                                                    spells = true;
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        // next character in battle order
                                        next = Interface::Next(battle, scene, party, order, combatant, input, endturn);
                                    }
                                }
                            }
                            else
                            {
                                // next character in battle order
                                next = Interface::Next(battle, scene, party, order, combatant, input, endturn);
                            }
                        }
                        else
                        {
                            animating = !Graphics::Animate(graphics, scene, movement);

                            Graphics::Refresh(graphics);

                            if (!animating)
                            {
                                // cancel fleeing status
                                if (character.Is(Character::Status::FLEEING))
                                {
                                    character.Remove(Character::Status::FLEEING);
                                }

                                if (isplayer)
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

                                scene = Interface::BattleScene(battle, party);

                                // next character in battle order
                                next = Interface::Next(battle, scene, party, order, combatant, input, endturn);
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

            Free(partyStatus);

            Free(partyStats);

            Free(enemyStatus);

            Free(enemyStats);

            Free(textures);
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
