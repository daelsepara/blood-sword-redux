#ifndef __INTERFACE_BATTLE_SCENE_HPP__
#define __INTERFACE_BATTLE_SCENE_HPP__

#include "loggers/Battle.hpp"

// battle interface functions
namespace BloodSword::Interface
{
    // find map control
    int Find(Map::Base &map, Controls::Collection &controls, Controls::Type type, int id)
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

    // center map on character (map object type, id)
    void Center(Battle::Base &battle, Map::Object character, int id)
    {
        auto src = battle.Map.Find(character, id);

        battle.Map.X = src.X - (battle.Map.ViewX) / 2 + 1;

        battle.Map.Y = src.Y - (battle.Map.ViewY) / 2 + 1;

        battle.Map.CheckBounds();
    }

    // highlight control (bottom)
    void HighlightControl(Scene::Base &scene, SDL_Texture *texture, Controls::Type control)
    {
        auto index = Controls::Find(scene.Controls, control);

        if (Input::IsValid(scene, index, scene.Controls.size()))
        {
            auto &element = scene.Controls[index];

            // highlight control (by putting another texture on top)
            scene.Add(Scene::Element(texture, element.X, element.Y));
        }
    }

    // searches for a player with IN COMBAT status to target
    void SearchInCombatTargets(Battle::Base &battle, Party::Base &party, Engine::Queue order, int combatant)
    {
        if (battle.InCombatTarget == Character::Class::NONE || !party.Has(battle.InCombatTarget) || (party.Has(battle.InCombatTarget) && (!Engine::CanTarget(party[battle.InCombatTarget], true) || !party[battle.InCombatTarget].Is(Character::Status::IN_COMBAT))))
        {
            battle.InCombatTarget = Character::Class::NONE;

            for (auto i = 0; i < order.size(); i++)
            {
                if (order[i].Id >= 0 && order[i].Id < party.Count())
                {
                    auto &target = party[order[i].Id];

                    if (Engine::IsPlayer(order, i) && Engine::CanTarget(target, true) && target.Is(Character::Status::IN_COMBAT))
                    {
                        battle.InCombatTarget = target.Class;

                        break;
                    }
                }
            }
        }
#if defined(DEBUG)
        if (battle.InCombatTarget != Character::Class::NONE)
        {
            std::cerr << "[IN COMBAT] [" << Character::ClassMapping[battle.InCombatTarget] << "]" << std::endl;
        }
#endif
    }

    // generate textures of party status
    Asset::TextureList GenerateStatus(Graphics::Base &graphics, Party::Base &party, bool in_battle = true)
    {
        return Interface::Status(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, in_battle);
    }

    // regenerate textures stats
    void RegenerateStats(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party, Asset::TextureList &party_stats, Asset::TextureList &party_status, Asset::TextureList &enemy_stats, Asset::TextureList &enemy_status)
    {
        BloodSword::Free(enemy_stats);

        BloodSword::Free(enemy_status);

        BloodSword::Free(party_stats);

        BloodSword::Free(party_status);

        auto info_w = battle.Map.TileSize * 5;

        enemy_stats = Interface::GenerateStats(graphics, battle.Opponents, info_w);

        enemy_status = Interface::GenerateStatus(graphics, battle.Opponents);

        party_stats = Interface::GenerateStats(graphics, party, info_w);

        party_status = Interface::GenerateStatus(graphics, party);
    }

    // setup overlay of battle actions
    Scene::Base BattleActions(Point origin, int w, int h, Battle::Base &battle, Party::Base &party, int id, Uint32 background, Uint32 border, int border_size, bool ranged = false)
    {
        Controls::List controls = {};

        auto overlay = Scene::Base();

        auto &character = party[id];

        auto is_player = character.IsPlayer();

        auto src = is_player ? battle.Map.Find(Map::Object::PLAYER, id) : battle.Map.Find(Map::Object::ENEMY, id);

        auto is_enthralled = character.Is(Character::Status::ENTHRALLED);

        // adjacency checks (inverted)
        auto no_enemy = (!battle.Map.Adjacent(src, Map::Object::ENEMY) || ranged);

        auto adj_enemy = !no_enemy;

        auto no_player = !battle.Map.Adjacent(src, Map::Object::PLAYER);

        auto adj_player = !no_player;

        // enemies available
        auto enemies = !battle.Map.Find(Map::Object::ENEMY).IsNone();

        // targets among enemies
        auto comrades = battle.Map.Except(Map::Object::ENEMY, id).IsNone();

        // players available
        auto players = !battle.Map.Find(Map::Object::PLAYER).IsNone();

        // enemy can shoot
        auto enthralled = !is_player && is_enthralled && comrades && no_enemy;

        // not enthralled
        auto normal = !is_player && !is_enthralled && players && no_player;

        // exits
        auto exits = !battle.Map.Find(Map::Object::EXIT).IsNone();

        if (!src.IsNone())
        {
            // can move
            if (Move::Available(battle.Map, src) && !character.Is(Character::Status::ENTANGLED) && !ranged)
            {
                controls.push_back(Controls::Type::MOVE);
            }

            // defend
            controls.push_back(Controls::Type::DEFEND);

            if (!battle.Has(Battle::Condition::NO_COMBAT))
            {
                auto player_fighter = is_player && adj_enemy;

                auto enemy_fighter = !is_player && adj_player;

                if ((player_fighter || enemy_fighter) && !ranged)
                {
                    // can fight
                    controls.push_back(Controls::Type::FIGHT);

                    // has quarterstaff skill
                    if (character.Has(Skills::Type::QUARTERSTAFF) && character.IsArmed(Item::Type::QUARTERSTAFF))
                    {
                        controls.push_back(Interface::ActionControls[Skills::Type::QUARTERSTAFF]);
                    }
                }

                auto player_shooter = is_player && Engine::CanShoot(character) && enemies && no_enemy;

                auto enemy_shooter = (enthralled || normal) && Engine::CanShoot(character);

                if ((player_shooter || enemy_shooter) && Interface::ActionControls[character.Shoot] != Controls::Type::NONE)
                {
                    // can shoot
                    controls.push_back(Interface::ActionControls[character.Shoot]);
                }

                // can cast spells
                if (character.Has(Skills::Type::SPELLS) && !ranged)
                {
                    controls.push_back(Interface::ActionControls[Skills::Type::SPELLS]);
                }

                if (is_player && exits && !battle.Has(Battle::Condition::CANNOT_FLEE) && ((Engine::CanFlee(battle.Map, party, id) && !ranged) || character.Has(Character::Status::INVISIBLE)))
                {
                    controls.push_back(Controls::Type::FLEE);
                }

                if (is_player && character.Items.size() > 0 && !ranged)
                {
                    controls.push_back(Controls::Type::ITEMS);
                }
            }
        }

        controls.push_back(Controls::Type::BACK);

        auto popup_w = (std::max(int(controls.size()), 2) + (BloodSword::SmallPad / 2)) * BloodSword::TileSize;

        auto popup_h = BloodSword::FrameHeight;

        auto screen = origin + Point(w - popup_w, h - popup_h) / 2;

        overlay.Add(Scene::Element(screen, popup_w, popup_h, background, border, border_size));

        auto pad = BloodSword::QuarterTile;

        for (auto i = 0; i < controls.size(); i++)
        {
            auto texture = Asset::Get(Interface::BattleControls[controls[i]]);

            if (texture)
            {
                auto texture_w = 0;

                auto texture_h = 0;

                BloodSword::Size(texture, &texture_w, &texture_h);

                auto lt = i > 0 ? i - 1 : i;

                auto rt = i < controls.size() - 1 ? i + 1 : i;

                overlay.Add(Controls::Base(controls[i], i, lt, rt, i, i, screen.X + i * texture_w + pad, screen.Y + pad + BloodSword::HalfTile, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

                overlay.VerifyAndAdd(Scene::Element(texture, screen.X + i * texture_w + pad, screen.Y + pad + BloodSword::HalfTile));
            }
        }

        return overlay;
    }

    // adds map controls
    void MapControls(Scene::Base &scene, Battle::Base &battle)
    {
        auto mid_y = battle.Map.DrawY + (battle.Map.ViewY) * BloodSword::HalfTile;

        auto mid_x = battle.Map.DrawX + (battle.Map.ViewX) * BloodSword::HalfTile;

        auto left_x = battle.Map.DrawX - BloodSword::TileSize - BloodSword::Pad;

        auto right_x = battle.Map.DrawX + battle.Map.ViewX * BloodSword::TileSize + BloodSword::Pad;

        auto top_y = battle.Map.DrawY - BloodSword::TileSize - BloodSword::Pad;

        auto bottom_y = battle.Map.DrawY + battle.Map.ViewY * BloodSword::TileSize + BloodSword::Pad;

        auto id = scene.Controls.size();

        auto left = (battle.Map.X > 0);

        auto up = (battle.Map.Y > 0);

        auto right = (battle.Map.X + battle.Map.ViewX < battle.Map.Width);

        auto down = (battle.Map.Y + battle.Map.ViewY < battle.Map.Height);

        // add controls to scene
        if (left)
        {
            scene.VerifyAndAdd(Scene::Element(Asset::Get("LEFT"), Point(left_x, mid_y - BloodSword::HalfTile)));

            scene.Add(Controls::Base(Controls::Type::MAP_LEFT, id, id, id, id, id, left_x, mid_y - BloodSword::HalfTile, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            // focus on SCROLL LEFT
            for (auto i = 0; i < battle.Map.ViewY; i++)
            {
                auto control = i * (battle.Map.ViewX);

                if (control >= 0 && control < scene.Controls.size())
                {
                    scene.Controls[control].Left = id;
                }
            }

            id++;
        }

        if (up)
        {
            scene.VerifyAndAdd(Scene::Element(Asset::Get("UP"), Point(mid_x - BloodSword::HalfTile, top_y)));

            scene.Add(Controls::Base(Controls::Type::MAP_UP, id, id, id, id, id, mid_x - BloodSword::HalfTile, top_y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            // focus on SCROLL UP
            for (auto i = 0; i < battle.Map.ViewX; i++)
            {
                scene.Controls[i].Up = id;
            }

            id++;
        }

        if (right)
        {
            scene.VerifyAndAdd(Scene::Element(Asset::Get("RIGHT"), Point(right_x, mid_y - BloodSword::HalfTile)));

            scene.Add(Controls::Base(Controls::Type::MAP_RIGHT, id, id, id, id, id, right_x, mid_y - BloodSword::HalfTile, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            // focus on SCROLL RIGHT
            for (auto i = 0; i < battle.Map.ViewY; i++)
            {
                auto control = i * (battle.Map.ViewX) + (battle.Map.ViewX - 1);

                if (control >= 0 && control < scene.Controls.size())
                {
                    scene.Controls[control].Right = id;
                }
            }

            id++;
        }

        if (down)
        {
            scene.VerifyAndAdd(Scene::Element(Asset::Get("DOWN"), Point(mid_x - BloodSword::HalfTile, bottom_y)));

            scene.Add(Controls::Base(Controls::Type::MAP_DOWN, id, id, id, id, id, mid_x - BloodSword::HalfTile, bottom_y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            // focus on SCROLL DOWN
            auto control = (battle.Map.ViewY - 1) * battle.Map.ViewX + (battle.Map.ViewX / 2) - (battle.Map.ViewX & 1 ? 0 : 1);

            if (control >= 0 && control < scene.Controls.size())
            {
                scene.Controls[control].Down = id;

                if (!(battle.Map.ViewX & 1))
                {
                    scene.Controls[control + 1].Down = id;
                }
            }

            id++;
        }

        auto map_top = (battle.Map.ViewX / 2) - (battle.Map.ViewX & 1 ? 0 : 1);

        auto viewy_half = (battle.Map.ViewY / 2) - (battle.Map.ViewY & 1 ? 0 : 1);

        auto map_bottom = (battle.Map.ViewY - 1) * battle.Map.ViewX + map_top;

        auto map_right = (viewy_half + 1) * battle.Map.ViewX - 1;

        auto map_left = viewy_half * battle.Map.ViewX;

        // fix control switching
        if (left)
        {
            auto my_id = Controls::Find(scene.Controls, Controls::Type::MAP_LEFT);

            scene.Controls[my_id].Up = map_top;

            scene.Controls[my_id].Down = map_bottom;

            scene.Controls[my_id].Right = map_left;
        }

        if (up)
        {
            auto my_id = Controls::Find(scene.Controls, Controls::Type::MAP_UP);

            scene.Controls[my_id].Left = map_left;

            scene.Controls[my_id].Down = map_top;

            scene.Controls[my_id].Right = map_right;
        }

        if (right)
        {
            auto my_id = Controls::Find(scene.Controls, Controls::Type::MAP_RIGHT);

            scene.Controls[my_id].Left = map_right;

            scene.Controls[my_id].Up = map_top;

            scene.Controls[my_id].Down = map_bottom;
        }

        if (down)
        {
            auto my_id = Controls::Find(scene.Controls, Controls::Type::MAP_DOWN);

            scene.Controls[my_id].Left = map_left;

            scene.Controls[my_id].Up = map_bottom;

            scene.Controls[my_id].Right = map_right;

            scene.Controls[my_id].Down = battle.Map.ViewX * battle.Map.ViewY;
        }
    }

    // setup battle scene
    Scene::Base BattleScene(Battle::Base &battle, Party::Base &party, Scene::Elements &assets, Controls::Collection &controls, Point location)
    {
        auto num = int(assets.size() == controls.size() ? controls.size() : 0);

        auto scene = Interface::Map(battle.Map, party, battle.Opponents, num);

        if (num > 0)
        {
            for (auto &asset : assets)
            {
                scene.VerifyAndAdd(asset);
            }

            for (auto &control : controls)
            {
                scene.Add(control);
            }
        }

        Interface::MapControls(scene, battle);

        return scene;
    }

    // regenerate battle map (starting at point location without controls)
    Scene::Base BattleScene(Battle::Base &battle, Party::Base &party, Point location)
    {
        Scene::Elements assets = {};

        Controls::Collection controls = {};

        return Interface::BattleScene(battle, party, assets, controls, location);
    }

    // regenerate battle map
    Scene::Base BattleScene(Battle::Base &battle, Party::Base &party)
    {
        return Interface::BattleScene(battle, party, Point(battle.Map.DrawX, battle.Map.DrawY + BloodSword::TileSize + BloodSword::Pad));
    }

    // regenerate battle map (starting at point location)
    Scene::Base BattleScene(Battle::Base &battle, Party::Base &party, Point location, Character::Base &character, int id, Point origin, bool ranged = false)
    {
        auto map = int(battle.Map.ViewX * battle.Map.ViewY);

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        auto bottom = location.Y + map_h;

        auto space = BloodSword::TileSize + BloodSword::Pad;

        auto is_player = character.IsPlayer();

        auto is_enthralled = character.Is(Character::Status::ENTHRALLED);

        // adjacency checks (inverted)
        auto no_enemy = (!battle.Map.Adjacent(origin, Map::Object::ENEMY) || ranged);

        auto adj_enemy = !no_enemy;

        auto no_player = !battle.Map.Adjacent(origin, Map::Object::PLAYER);

        auto adj_player = !no_player;

        // enemies available
        auto enemies = !battle.Map.Find(Map::Object::ENEMY).IsNone();

        // targets among enemies
        auto comrades = battle.Map.Except(Map::Object::ENEMY, id).IsNone();

        // players available
        auto players = !battle.Map.Find(Map::Object::PLAYER).IsNone();

        // enemy can shoot
        auto enthralled = !is_player && is_enthralled && comrades && no_enemy;

        // not enthralled
        auto normal = !is_player && !is_enthralled && players && no_player;

        // exits
        auto exits = !battle.Map.Find(Map::Object::EXIT).IsNone();

        Asset::List asset_list = {Asset::Map("EXIT")};

        Controls::List controls_list = {Controls::Type::EXIT};

        if (battle.Map.ViewX < battle.Map.Width || battle.Map.ViewY < battle.Map.Height)
        {
            asset_list.push_back(Asset::Map("CENTER"));

            controls_list.push_back(Controls::Type::CENTER);
        }

        if (!origin.IsNone())
        {
            // can move
            if (Move::Available(battle.Map, origin) && !character.Is(Character::Status::ENTANGLED) && !ranged)
            {
                asset_list.push_back(Asset::Map("MOVE"));

                controls_list.push_back(Controls::Type::MOVE);
            }

            // defend
            asset_list.push_back(Asset::Map("DEFEND"));

            controls_list.push_back(Controls::Type::DEFEND);

            if (!battle.Has(Battle::Condition::NO_COMBAT))
            {
                auto player_fighter = is_player && adj_enemy;

                auto enemy_fighter = !is_player && adj_player;

                if ((player_fighter || enemy_fighter) && !ranged)
                {
                    // can fight
                    asset_list.push_back(Asset::Map("FIGHT"));

                    controls_list.push_back(Controls::Type::FIGHT);

                    // has quarterstaff skill
                    if (character.Has(Skills::Type::QUARTERSTAFF) && character.IsArmed(Item::Type::QUARTERSTAFF))
                    {
                        asset_list.push_back(Asset::Map("QUARTERSTAFF"));

                        controls_list.push_back(Interface::ActionControls[Skills::Type::QUARTERSTAFF]);
                    }
                }

                auto player_shooter = is_player && Engine::CanShoot(character) && enemies && no_enemy;

                auto enemy_shooter = (enthralled || normal) && Engine::CanShoot(character);

                if ((player_shooter || enemy_shooter) && Interface::ActionControls[character.Shoot] != Controls::Type::NONE)
                {
                    // can shoot
                    if (character.Shoot == Skills::Type::SHURIKEN)
                    {
                        asset_list.push_back(Asset::Map("SHURIKEN"));
                    }
                    else
                    {
                        asset_list.push_back(Asset::Map("SHOOT"));
                    }

                    controls_list.push_back(Interface::ActionControls[character.Shoot]);
                }

                // can cast spells
                if (character.Has(Skills::Type::SPELLS) && !ranged)
                {
                    asset_list.push_back(Asset::Map("SPELLS"));

                    controls_list.push_back(Interface::ActionControls[Skills::Type::SPELLS]);
                }

                if (is_player && exits && !battle.Has(Battle::Condition::CANNOT_FLEE) && ((Engine::CanFlee(battle.Map, party, id) && !ranged) || character.Has(Character::Status::INVISIBLE)))
                {
                    asset_list.push_back(Asset::Map("FLEE"));

                    controls_list.push_back(Controls::Type::FLEE);
                }

                if (is_player && character.Items.size() > 0 && !ranged)
                {
                    asset_list.push_back(Asset::Map("ITEMS"));

                    controls_list.push_back(Controls::Type::ITEMS);
                }
            }
        }

        Scene::Elements assets = {};

        Controls::Collection controls = {};

        for (auto i = 0; i < controls_list.size(); i++)
        {
            auto texture = Asset::Get(asset_list[i]);

            if (texture)
            {
                auto control = map + i;

                auto lt = i > 0 ? (control - 1) : (control);

                auto rt = i < controls_list.size() - 1 ? (control + 1) : control;

                auto up = i < battle.Map.ViewX ? control - battle.Map.ViewX : control;

                controls.push_back(Controls::Base(controls_list[i], control, lt, rt, up, control, location.X + i * space, bottom, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

                assets.push_back(Scene::Element(texture, location.X + i * space, bottom));
            }
        }

        return Interface::BattleScene(battle, party, assets, controls, location);
    }

    // setup battle scene
    Scene::Base BattleScene(Battle::Base &battle, Party::Base &party, Character::Base &character, int id, Point origin, bool ranged = false)
    {
        return Interface::BattleScene(battle, party, Point(battle.Map.DrawX, battle.Map.DrawY + BloodSword::TileSize + BloodSword::Pad), character, id, origin, ranged);
    }

    // drop any droppable items upon character's death
    void DropItemsOnDeath(Battle::Base &battle, Character::Base &character)
    {
        if (!Engine::IsAlive(character))
        {
            for (auto item = 0; item < character.Items.size(); item++)
            {
                if (character.Items[item].Drops)
                {
                    battle.Loot.push_back(character.Items[item]);
                }
            }
        }
    }

    // set players' starting locations
    void SetPlayerLocations(Battle::Base &battle, Party::Base &party)
    {
        auto player_away = 0;

        auto origin = 0;

        if (battle.Map.Origins.size() > 0)
        {
            // set party starting locations
            for (auto i = 0; i < party.Count(); i++)
            {
                if (Engine::IsAlive(party[i]) && !Character::OtherClass(party[i].Class))
                {
                    if (!party[i].Is(Character::Status::AWAY) && !party[i].Is(Character::Status::EXCLUDED))
                    {
                        if (origin < battle.Map.Origins.size())
                        {
                            battle.Map.Put(battle.Map.Origins[origin], Map::Object::PLAYER, i);

                            origin++;
                        }
                        else
                        {
                            throw std::invalid_argument("BATTLE: PLAYER ORIGIN LOCATIONS INSUFFICIENT!");
                        }
                    }
                    else if (party[i].Status[Character::Status::AWAY] > 0 || party[i].Is(Character::Status::EXCLUDED))
                    {
                        if (player_away < battle.Map.AwayPlayers.size())
                        {
                            battle.Map.Put(battle.Map.AwayPlayers[player_away], Map::Object::PLAYER, i);

                            // remove EXCLUDED status (assumes that they are placed in an isolated location)
                            if (party[i].Is(Character::Status::EXCLUDED))
                            {
                                party[i].Remove(Character::Status::EXCLUDED);
                            }

                            player_away++;
                        }
                        else
                        {
                            throw std::invalid_argument("BATTLE: PLAYER AWAY LOCATIONS INSUFFICIENT!");
                        }
                    }
                }
            }
        }
        else
        {
            throw std::invalid_argument("BATTLE: PLAYER ORIGIN LOCATIONS INSUFFICIENT!");
        }

        BattleLogger::LogCombatants(party, "PLAYERS");
    }

    // set enemy starting locations
    void SetEnemyLocations(Battle::Base &battle, Party::Base &party)
    {
        auto spawn = 0;

        auto enemy_away = 0;

        auto reinforce = 0;

        if (battle.Opponents.Count() > 0)
        {
            // set opposing party's location
            if (Book::IsDefined(battle.Location))
            {
                battle.Opponents.Set(battle.Location);
            }
            else
            {
                battle.Opponents.Set(party.Location);
            }

            if (battle.Map.Spawn.size() >= battle.Opponents.Count())
            {
                // set opponents starting locations
                for (auto i = 0; i < battle.Opponents.Count(); i++)
                {
                    if (Engine::IsAlive(battle.Opponents[i]))
                    {
                        if (!battle.Opponents[i].Is(Character::Status::AWAY))
                        {
                            if (spawn < battle.Map.Spawn.size())
                            {
                                battle.Map.Put(battle.Map.Spawn[spawn], Map::Object::ENEMY, i);

                                spawn++;
                            }
                            else
                            {
                                throw std::invalid_argument("BATTLE: ENEMY SPAWN LOCATIONS INSUFFICIENT!");
                            }
                        }
                        else if (battle.Opponents[i].Status[Character::Status::AWAY] > 0)
                        {
                            // place opponents that arrive at later rounds
                            if (enemy_away < battle.Map.AwayOpponents.size())
                            {
                                battle.Map.Put(battle.Map.AwayOpponents[enemy_away], Map::Object::ENEMY, i);

                                enemy_away++;
                            }
                            else
                            {
                                throw std::invalid_argument("BATTLE: ENEMY AWAY LOCATIONS INSUFFICIENT!");
                            }
                        }
                    }
                }
            }
            else
            {
                throw std::invalid_argument("BATTLE: ENEMY SPAWN LOCATIONS INSUFFICIENT!");
            }
        }

        // place survivors on the battlefield
        if (Book::IsDefined(battle.Survivors) || (battle.Has(Battle::Condition::LAST_BATTLE) && Book::IsDefined(party.BattleLocation)))
        {
            auto source = (battle.Has(Battle::Condition::LAST_BATTLE) && Book::IsDefined(party.BattleLocation)) ? party.BattleLocation : battle.Survivors;

            if (battle.Map.Survivors.size() > 0)
            {
                // gather list of survivors
                auto survivors = Party::Base();

                auto remove = std::vector<int>();

                // look for the survivors in the previous battle
                for (auto i = 0; i < party.Survivors.size(); i++)
                {
                    if (Book::IsDefined(party.Survivors[i].Location) && Engine::IsAlive(party.Survivors[i]) && Book::Equal(party.Survivors[i].Location, source) && survivors.Count() < battle.SurvivorLimit)
                    {
                        BattleLogger::LogGroupAction("SURVIVOR", "ADD", party.Survivors[i].Target, i);

                        survivors.Add(party.Survivors[i]);

                        // get a list of survivors to be removed
                        remove.push_back(i);
                    }
                }

                if (remove.size() > 0)
                {
                    // reverse index to survivor list
                    std::reverse(remove.begin(), remove.end());

                    for (auto i = 0; i < remove.size(); i++)
                    {
                        BattleLogger::LogGroupAction("PARTY SURVIVOR", "DELETE", party.Survivors[remove[i]].Target, remove[i]);

                        // remove from survivor list
                        party.Survivors.erase(party.Survivors.begin() + remove[i]);
                    }
                }

                BattleLogger::LogGroup(source, "REINFORCEMENTS", "PARTY SURVIVORS", survivors.Count(), party.Survivors.size());

                if (battle.Map.Survivors.size() >= survivors.Count())
                {
                    auto id = battle.Opponents.Count();

                    for (auto i = 0; i < survivors.Count(); i++)
                    {
                        // set current location
                        if (Book::IsDefined(battle.Location))
                        {
                            survivors[i].Set(battle.Location);
                        }
                        else
                        {
                            survivors[i].Set(party.Location);
                        }

                        if (battle.Has(Battle::Condition::HEAL_SURVIVORS))
                        {
                            BattleLogger::LogGroupAction("SURVIVOR", "HEAL", survivors[i].Target, i);

                            survivors[i].Value(Attribute::Type::ENDURANCE, survivors[i].Maximum(Attribute::Type::ENDURANCE));
                        }

                        if (battle.SurvivorStart > 0)
                        {
                            survivors[i].Add(Character::Status::AWAY, battle.SurvivorStart);
                        }

                        // add to opponent party
                        battle.Opponents.Add(survivors[i]);

                        if (!survivors[i].Is(Character::Status::AWAY))
                        {
                            if (reinforce < battle.Map.Survivors.size())
                            {
                                // add to map
                                battle.Map.Put(battle.Map.Survivors[reinforce], Map::Object::ENEMY, id + i);

                                reinforce++;
                            }
                            else
                            {
                                throw std::invalid_argument("BATTLE: ENEMY SURVIVOR LOCATIONS INSUFFICIENT!");
                            }
                        }
                        else if (survivors[i].Status[Character::Status::AWAY] > 0)
                        {
                            // place opponents that arrive at later rounds
                            if (enemy_away < battle.Map.AwayOpponents.size())
                            {
                                battle.Map.Put(battle.Map.AwayOpponents[enemy_away], Map::Object::ENEMY, id + i);

                                enemy_away++;
                            }
                            else
                            {
                                throw std::invalid_argument("BATTLE: ENEMY AWAY LOCATIONS INSUFFICIENT!");
                            }
                        }
                    }
                }
            }
            else
            {
                throw std::invalid_argument("BATTLE: ENEMY SURVIVOR LOCATIONS UNDEFINED!");
            }
        }

        BattleLogger::LogCombatants(battle.Opponents, "OPPONENTS");
    }

    // check final locations of party and opponents
    void FinalLocationChecks(Battle::Base &battle, Party::Base &party)
    {
        int players = 0;

        int opponents = 0;

        // count number of players on the battlefield
        for (auto i = 0; i < party.Count(); i++)
        {
            if (!battle.Map.Find(Map::Object::PLAYER, i).IsNone())
            {
                players++;
            }
        }

        // count number of opponents on the battlefield
        for (auto i = 0; i < battle.Opponents.Count(); i++)
        {
            if (!battle.Map.Find(Map::Object::ENEMY, i).IsNone())
            {
                opponents++;
            }
        }

        // all combatants must be placed
        if (players == 0 || opponents == 0)
        {
            throw std::invalid_argument("BATTLE: UNABLE TO RENDER BATTLE!");
        }
    }

    // put other players in positions adjacent to other PLAYER characters
    void PlayerTactics(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party, Character::Class character)
    {
        auto location = Point(battle.Map.DrawX, battle.Map.DrawY + BloodSword::TileSize + BloodSword::Pad);

        auto id = int(battle.Map.ViewX * battle.Map.ViewY);

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        Scene::Elements assets = {
            Scene::Element(Asset::Get("CONFIRM"), location.X, location.Y + map_h),
            Scene::Element(Asset::Get("BACK"), location.X + BloodSword::TileSize + BloodSword::Pad, location.Y + map_h)};

        Controls::Collection controls = {Controls::Base(Controls::Type::CONFIRM, id, id, id + 1, id - battle.Map.ViewX, id, location.X, location.Y + map_h, battle.Map.TileSize, battle.Map.TileSize, Color::Active),
                                         Controls::Base(Controls::Type::BACK, id + 1, id - 1, id + 1, id + 1 - battle.Map.ViewX, id + 1, location.X + BloodSword::TileSize + BloodSword::Pad, location.Y + map_h, battle.Map.TileSize, battle.Map.TileSize, Color::Active)};

        auto input = Controls::User();

        auto done = false;

        auto map = Interface::BattleScene(battle, party, assets, controls, location);

        auto pad = BloodSword::SmallPad;

        auto info_x = battle.Map.DrawX + (battle.Map.ViewX * 2 + 1) * battle.Map.TileSize / 2 + pad;

        auto info_y = battle.Map.DrawY - pad;

        auto tactics_x = battle.Map.DrawX;

        auto tactics_y = battle.Map.DrawY - (BloodSword::TileSize + BloodSword::HalfTile);

        auto tactics = Graphics::CreateText(graphics, "TACTICS PHASE", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto texture = Graphics::CreateText(graphics, "SELECT LOCATION FOR", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto player = -1;

        for (auto i = 0; i < party.Count(); i++)
        {
            if (Engine::IsAlive(party[i]) && party[i].Class == character)
            {
                player = i;

                break;
            }
        }

        if (player == -1)
        {
            return;
        }

        while (!done)
        {
            auto regenerate_scene = false;

            auto overlay = Scene::Base();

            overlay.VerifyAndAdd(Scene::Element(tactics, Point(tactics_x, tactics_y)));

            if (player >= 0 && player < party.Count())
            {
                overlay.VerifyAndAdd(Scene::Element(texture, Point(info_x, info_y)));

                overlay.VerifyAndAdd(Scene::Element(Asset::Get(party[player].Asset), Point(info_x, info_y + pad * 8)));
            }

            input = Input::WaitForInput(graphics, {map, overlay}, map.Controls, input, false);

            if (Input::Check(input))
            {
                if (Input::IsValid(map, input))
                {
                    auto &control = map.Controls[input.Current];

                    if (control.OnMap && battle.Map[control.Map].IsOccupied())
                    {
                        Interface::MessageBox(graphics, map, "THIS SPACE IS OCCUPIED", Color::Highlight);
                    }
                    else if (control.OnMap && battle.Map[control.Map].IsPassable() && !battle.Map[control.Map].IsExit())
                    {
                        auto dst = control.Map;

                        if (battle.Map.Next(dst, Map::Object::PLAYER, player))
                        {
                            auto src = battle.Map.Find(Map::Object::PLAYER, player);

                            if (!src.IsNone())
                            {
                                // remove from another location
                                battle.Map.Remove(Map::Object::PLAYER, player);
                            }

                            battle.Map.Put(dst, Map::Object::PLAYER, player);

                            regenerate_scene = true;
                        }
                        else
                        {
                            Interface::MessageBox(graphics, map, "THERE ARE NO ADJACENT PLAYERS HERE", Color::Highlight);
                        }

                        input.Selected = false;
                    }
                    else if (input.Type == Controls::Type::CONFIRM)
                    {
                        auto dst = battle.Map.Find(Map::Object::PLAYER, player);

                        if (!dst.IsNone())
                        {
                            if (!Interface::Confirm(graphics, map, "PROCEED?", Color::Background, Color::Active, BloodSword::Border, Color::Highlight, true))
                            {
                                input.Selected = false;

                                regenerate_scene = true;
                            }
                            else
                            {
                                done = true;
                            }
                        }
                        else
                        {
                            std::string message = "YOU HAVE NOT PLACED THE " + std::string(Character::ClassMapping[character]);

                            Interface::MessageBox(graphics, map, message, Color::Highlight);
                        }
                    }
                    else if (input.Type == Controls::Type::BACK)
                    {
                        battle.Map.Remove(Map::Object::PLAYER, player);

                        input.Selected = false;

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
                }
            }

            if (regenerate_scene)
            {
                map = Interface::BattleScene(battle, party, assets, controls, location);

                if (input.Type == Controls::Type::MAP_DOWN || input.Type == Controls::Type::MAP_UP || input.Type == Controls::Type::MAP_LEFT || input.Type == Controls::Type::MAP_RIGHT)
                {
                    Controls::Select(input, map.Controls, input.Type);
                }
            }
        }

        BloodSword::Free(&texture);

        BloodSword::Free(&tactics);
    }

    // select placement of characters
    void Tactics(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party)
    {
        auto location = Point(battle.Map.DrawX, battle.Map.DrawY + BloodSword::TileSize + BloodSword::Pad);

        auto save_map = battle.Map;

        // temporarily render enemy locations
        Interface::SetEnemyLocations(battle, party);

        // copy origins
        auto origins = Points();

        auto id = int(battle.Map.ViewX * battle.Map.ViewY);

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        Scene::Elements assets = {
            Scene::Element(Asset::Get("CONFIRM"), location.X, location.Y + map_h),
            Scene::Element(Asset::Get("BACK"), location.X + BloodSword::TileSize + BloodSword::Pad, location.Y + map_h)};

        Controls::Collection controls = {Controls::Base(Controls::Type::CONFIRM, id, id, id + 1, id - battle.Map.ViewX, id, location.X, location.Y + map_h, battle.Map.TileSize, battle.Map.TileSize, Color::Active),
                                         Controls::Base(Controls::Type::BACK, id + 1, id - 1, id + 1, id + 1 - battle.Map.ViewX, id + 1, location.X + BloodSword::TileSize + BloodSword::Pad, location.Y + map_h, battle.Map.TileSize, battle.Map.TileSize, Color::Active)};

        auto input = Controls::User();

        auto done = false;

        auto map = Interface::BattleScene(battle, party, assets, controls, location);

        auto pad = BloodSword::SmallPad;

        auto info_x = battle.Map.DrawX + (battle.Map.ViewX * 2 + 1) * battle.Map.TileSize / 2 + pad;

        auto info_y = battle.Map.DrawY - pad;

        auto tactics_x = battle.Map.DrawX;

        auto tactics_y = battle.Map.DrawY - (BloodSword::TileSize + BloodSword::HalfTile);

        auto tactics = Graphics::CreateText(graphics, "TACTICS PHASE", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto texture = Graphics::CreateText(graphics, "SELECT LOCATION FOR", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto current = 0;

        while (!done)
        {
            auto regenerate_scene = false;

            auto overlay = Scene::Base();

            overlay.VerifyAndAdd(Scene::Element(tactics, Point(tactics_x, tactics_y)));

            if (current >= 0 && current < party.Count())
            {
                overlay.VerifyAndAdd(Scene::Element(texture, Point(info_x, info_y)));

                overlay.VerifyAndAdd(Scene::Element(Asset::Get(party[current].Asset), Point(info_x, info_y + pad * 8)));
            }

            input = Input::WaitForInput(graphics, {map, overlay}, map.Controls, input, false);

            if (Input::Check(input))
            {
                if (Input::IsValid(map, input))
                {
                    auto &control = map.Controls[input.Current];

                    if (control.OnMap && battle.Map[control.Map].IsOccupied())
                    {
                        Interface::MessageBox(graphics, map, "THIS SPACE IS OCCUPIED", Color::Highlight);
                    }
                    else if (control.OnMap && battle.Map[control.Map].IsPassable() && !battle.Map[control.Map].IsExit())
                    {
                        auto remove = -1;

                        // check if this has been added already
                        for (auto i = 0; i < origins.size(); i++)
                        {
                            if (origins[i] == control.Map)
                            {
                                remove = i;

                                break;
                            }
                        }

                        if (remove == -1)
                        {
                            if (current < party.Count())
                            {
                                // add location
                                origins.push_back(control.Map);

                                battle.Map[control.Map].Asset = Asset::Map("SELECT");

                                regenerate_scene = true;

                                current++;
                            }
                            else
                            {
                                Interface::MessageBox(graphics, map, "ALL COMBATANTS HAVE BEEN PLACED", Color::Highlight);
                            }
                        }
                        else
                        {
                            // remove location
                            origins.erase(origins.begin() + remove);

                            battle.Map[control.Map].Asset = Asset::NONE;

                            regenerate_scene = true;

                            current--;
                        }

                        input.Selected = false;
                    }
                    else if (input.Type == Controls::Type::CONFIRM)
                    {
                        if (origins.size() != party.Count())
                        {
                            Interface::MessageBox(graphics, map, "NUMBER OF LOCATIONS INSUFFICIENT", Color::Highlight);
                        }
                        else if (!Interface::Confirm(graphics, map, "PROCEED WITH THIS FORMATION?", Color::Background, Color::Active, BloodSword::Border, Color::Highlight, true))
                        {
                            input.Selected = false;

                            regenerate_scene = true;
                        }
                        else
                        {
                            done = true;
                        }
                    }
                    else if (input.Type == Controls::Type::BACK)
                    {
                        for (auto i = 0; i < origins.size(); i++)
                        {
                            if (battle.Map.IsValid(origins[i]))
                            {
                                battle.Map[origins[i]].Asset = Asset::NONE;
                            }
                        }

                        origins.clear();

                        current = 0;

                        input.Selected = false;

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

                            input.Current = -1;

                            regenerate_scene = true;
                        }
                    }
                }
            }

            if (regenerate_scene)
            {
                map = Interface::BattleScene(battle, party, assets, controls, location);

                if (input.Type == Controls::Type::MAP_DOWN || input.Type == Controls::Type::MAP_UP || input.Type == Controls::Type::MAP_LEFT || input.Type == Controls::Type::MAP_RIGHT)
                {
                    Controls::Select(input, map.Controls, input.Type);
                }
            }
        }

        // restore map
        battle.Map = save_map;

        // set new player ORIGIN locations
        battle.Map.Origins = origins;

        BloodSword::Free(&texture);

        BloodSword::Free(&tactics);
    }

    // select target (when using an item)
    Point SelectTarget(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party, std::string action, Asset::Type asset, Controls::Type target_type)
    {
        // save a copy of the map
        auto save_map = battle.Map;

        auto selected = Point(-1, -1);

        auto location = Point(battle.Map.DrawX, battle.Map.DrawY + BloodSword::TileSize + BloodSword::Pad);

        auto id = int(battle.Map.ViewX * battle.Map.ViewY);

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        Scene::Elements assets = {Scene::Element(Asset::Get("BACK"), location.X + BloodSword::TileSize + BloodSword::Pad, location.Y + map_h)};

        Controls::Collection controls = {Controls::Base(Controls::Type::BACK, id, id, id, id - battle.Map.ViewX, id, location.X, location.Y + map_h, battle.Map.TileSize, battle.Map.TileSize, Color::Active)};

        auto input = Controls::User();

        auto done = false;

        auto map = Interface::BattleScene(battle, party, assets, controls, location);

        auto pad = BloodSword::SmallPad;

        auto action_x = battle.Map.DrawX + (battle.Map.ViewX * 2 + 1) * battle.Map.TileSize / 2 + pad;

        auto action_y = battle.Map.DrawY - pad;

        auto target_x = battle.Map.DrawX;

        auto target_y = battle.Map.DrawY - (BloodSword::TileSize + BloodSword::HalfTile);

        auto target = Graphics::CreateText(graphics, "SELECT TARGET", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto texture = Graphics::CreateText(graphics, action.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto combatant = (target_type == Controls::Type::ENEMY || target_type == Controls::Type::PLAYER);

        auto stats_w = battle.Map.TileSize * 5;

        auto enemy_stats = Interface::GenerateStats(graphics, battle.Opponents, stats_w);

        auto enemy_status = Interface::GenerateStatus(graphics, battle.Opponents, stats_w);

        auto party_stats = Interface::GenerateStats(graphics, party, stats_w);

        auto party_status = Interface::GenerateStatus(graphics, party, stats_w);

        while (!done)
        {
            auto regenerate_scene = false;

            auto overlay = Scene::Base();

            if (!input.Selected && Input::IsValid(overlay, input))
            {
                auto &control = overlay.Controls[input.Current];

                // show character stats
                if (battle.Map[control.Map].Occupant == Map::Object::PLAYER)
                {
                    auto stats_id = battle.Map[control.Map].Id;

                    if (stats_id >= 0 && stats_id < party.Count())
                    {
                        // stats
                        overlay.VerifyAndAdd(Scene::Element(party_stats[stats_id], action_x, action_y, Color::Background, Color::Active, BloodSword::Border));

                        auto &stats = overlay.Elements.back();

                        // status
                        overlay.VerifyAndAdd(Scene::Element(party_status[stats_id], action_x, action_y + stats.H + pad * 4, Color::Background, Color::Active, BloodSword::Border));
                    }
                }
                else if (battle.Map[control.Map].Occupant == Map::Object::ENEMY)
                {
                    auto stats_id = battle.Map[control.Map].Id;

                    if (stats_id >= 0 && stats_id < battle.Opponents.Count())
                    {
                        // enemy stats
                        overlay.VerifyAndAdd(Scene::Element(enemy_stats[stats_id], action_x, action_y, Color::Background, Color::Active, BloodSword::Border));

                        auto &stats = overlay.Elements.back();

                        // status
                        overlay.VerifyAndAdd(Scene::Element(enemy_status[stats_id], action_x, action_y + stats.H + pad * 4, Color::Background, Color::Active, BloodSword::Border));
                    }
                }
                else if (battle.Map[control.Map].IsExit())
                {
                    BloodSword::Free(&texture);

                    texture = Graphics::CreateText(graphics, "EXIT", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

                    overlay.VerifyAndAdd(Scene::Element(texture, action_x, action_y, Color::Background, Color::Inactive, 4));
                }
                else if (battle.Map[control.Map].IsTemporarilyBlocked())
                {
                    BloodSword::Free(&texture);

                    std::string text = " OBSTACLE (" + std::to_string(battle.Map[control.Map].Lifetime) + ") ";

                    texture = Graphics::CreateText(graphics, text.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

                    overlay.VerifyAndAdd(Scene::Element(texture, action_x, action_y, Color::Background, Color::Inactive, 4));
                }
            }
            else
            {
                BloodSword::Free(&texture);

                texture = Graphics::CreateText(graphics, action.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

                overlay.VerifyAndAdd(Scene::Element(target, Point(target_x, target_y)));

                overlay.VerifyAndAdd(Scene::Element(texture, Point(action_x, action_y)));

                overlay.VerifyAndAdd(Scene::Element(Asset::Get(asset), Point(action_x, action_y + pad * 8)));
            }

            input = Input::WaitForInput(graphics, {map, overlay}, map.Controls, input, false);

            if (Input::Check(input))
            {
                if (Input::IsValid(map, input))
                {
                    auto &control = map.Controls[input.Current];

                    if (control.OnMap && battle.Map[control.Map].IsOccupied() && control.Type == target_type && combatant)
                    {
                        selected = control.Map;
                    }
                    else if (control.OnMap && battle.Map[control.Map].IsPassable() && !battle.Map[control.Map].IsExit() && !combatant)
                    {
                        selected = control.Map;
                    }
                    else if (input.Type == Controls::Type::BACK)
                    {
                        input.Selected = false;

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

                    if (!selected.IsNone())
                    {
                        if (!Interface::Confirm(graphics, map, "PROCEED?", Color::Background, Color::Active, BloodSword::Border, Color::Highlight, true))
                        {
                            selected = Point(-1, -1);

                            input.Selected = false;

                            regenerate_scene = true;
                        }
                        else
                        {
                            done = true;
                        }
                    }
                }
            }

            if (regenerate_scene)
            {
                map = Interface::BattleScene(battle, party, assets, controls, location);

                if (input.Type == Controls::Type::MAP_DOWN || input.Type == Controls::Type::MAP_UP || input.Type == Controls::Type::MAP_LEFT || input.Type == Controls::Type::MAP_RIGHT)
                {
                    input.Current = Controls::Find(map.Controls, input.Type);
                }
            }
        }

        // restore map
        battle.Map = save_map;

        // free textures
        BloodSword::Free(party_status);

        BloodSword::Free(party_stats);

        BloodSword::Free(enemy_status);

        BloodSword::Free(enemy_stats);

        BloodSword::Free(&texture);

        BloodSword::Free(&target);

        return selected;
    }
}

#endif
