#ifndef __INTERFACE_BATTLE_HPP__
#define __INTERFACE_BATTLE_HPP__

#include "Graphics.hpp"
#include "InterfaceInventory.hpp"

// battle engine
namespace BloodSword::Interface
{
    BloodSword::UnorderedMap<Skills::Type, Character::Status> SkillEffects = {
        {Skills::Type::NONE, Character::Status::NONE},
        {Skills::Type::QUARTERSTAFF, Character::Status::KNOCKED_OUT},
        {Skills::Type::PARALYZING_TOUCH, Character::Status::PARALYZED},
        {Skills::Type::POISONED_DAGGER, Character::Status::POISONED}};

    BloodSword::UnorderedMap<Skills::Type, Controls::Type> ActionControls = {
        {Skills::Type::NONE, Controls::Type::NONE},
        {Skills::Type::ARCHERY, Controls::Type::SHOOT},
        {Skills::Type::SHURIKEN, Controls::Type::SHURIKEN},
        {Skills::Type::QUARTERSTAFF, Controls::Type::QUARTERSTAFF},
        {Skills::Type::SPELLS, Controls::Type::SPELLS}};

    BloodSword::UnorderedMap<Spells::Type, Character::Status> SpellEffects = {
        {Spells::Type::NONE, Character::Status::NONE},
        {Spells::Type::NIGHTHOWL, Character::Status::NIGHTHOWL}};

    void LogSpellStrategy(Spells::Type spell, int spells_cast, bool cast)
    {
        std::cerr << "[SPELLS CAST] ";

        if (spell != Spells::Type::NONE)
        {
            std::cerr << "[" << Spells::TypeMapping[spell] << "] ";
        }

        std::cerr << std::to_string(spells_cast) << " [CAST] " << (cast ? "TRUE" : "FALSE") << std::endl;
    }

    void LogCaster(Character::Base &caster, int caster_id)
    {
        std::cerr << "["
                  << Target::Mapping[caster.Target]
                  << " " << std::to_string(caster_id)
                  << "] ";

        if (!Engine::IsAlive(caster))
        {
            std::cerr << "IS DEAD" << std::endl;
        }
        else if (!caster.Has(Skills::Type::SPELLS))
        {
            std::cerr << "CANNOT CAST SPELLS" << std::endl;
        }
        else
        {
            std::cerr << "CANNOT CAST SPELLS THIS ROUND" << std::endl;
        }
    }

    void LogCasters(Battle::Base &battle)
    {
        if (battle.Casters.size() > 0)
        {
            std::cerr << "[SPELL CASTERS] (";

            for (auto i = 0; i < battle.Casters.size(); i++)
            {
                if (i > 0)
                {
                    std::cerr << ", ";
                }

                std::cerr << "[" << Target::Mapping[battle.Opponents[battle.Casters[i]].Target] << " " << std::to_string(battle.Casters[i]) << "]";
            }

            std::cerr << ")" << std::endl;
        }
    }

    void LogTargets(const char *target_type, Target::Type attacker, int id, int count)
    {
        std::cerr << "["
                  << Target::Mapping[attacker]
                  << " "
                  << id
                  << "] ["
                  << target_type
                  << " TARGETS] "
                  << count
                  << std::endl;
    }

    void LogAction(const char *action, Target::Type attacker, int id, Target::Type target, int target_id)
    {
        std::cerr << "["
                  << Target::Mapping[attacker]
                  << " "
                  << id
                  << "] ["
                  << action
                  << "] ["
                  << Target::Mapping[target]
                  << " "
                  << target_id
                  << "]"
                  << std::endl;
    }

    void LogSpellCasting(Target::Type caster, int caster_id, Spells::Type spell)
    {
        std::cerr << "["
                  << Target::Mapping[caster]
                  << " "
                  << std::to_string(caster_id)
                  << "] [CASTS "
                  << Spells::TypeMapping[spell]
                  << "]"
                  << std::endl;
    }

    void LogCombatants(Party::Base &party, const char *group)
    {
        std::cerr << "["
                  << group
                  << "] "
                  << std::to_string(party.Count())
                  << " [LIVE] "
                  << std::to_string(Engine::Count(party))
                  << std::endl;
    }

    void LogGroup(Book::Location location, const char *first, const char *second, int count_first, int count_second)
    {
        std::cerr << Book::String(location)
                  << " ["
                  << first
                  << "] "
                  << std::to_string(count_first)
                  << " ["
                  << second
                  << "] "
                  << std::to_string(count_second)
                  << std::endl;
    }

    void LogSurvivors(Book::Location location, const char *first, const char *second, int count_first, int count_second)
    {
        std::string group_first = std::string(first) + " SURVIVORS";

        std::string group_second = std::string(second) + " SURVIVORS";

        Interface::LogGroup(location, group_first.c_str(), group_second.c_str(), count_first, count_second);
    }

    void LogGroupAction(const char *group, const char *action, Target::Type actor, int id)
    {
        std::cerr << "["
                  << group
                  << "] ["
                  << action
                  << "] ["
                  << Target::Mapping[actor]
                  << " "
                  << std::to_string(id)
                  << "]"
                  << std::endl;
    }

    void LogBattleResults(Battle::Base &battle, Party::Base &party, Battle::Result initial, Battle::Result result)
    {
        std::cerr << "[BATTLE RESULTS] " << Battle::ResultMapping[initial] << std::endl
                  << "[PARTY] " << (Engine::IsAlive(party) ? "ALIVE" : "INCAPACITATED") << std::endl
                  << "[OPPONENTS] " << (Engine::IsAlive(battle.Opponents, Character::ControlType::NPC) ? "ALIVE" : "INCAPACITATED") << std::endl
                  << "[FLEEING] " << (Engine::IsFleeing(party) ? "YES" : "NO") << std::endl
                  << "[FINAL RESULTS] " << Battle::ResultMapping[result] << std::endl;
    }

    // find map control
    int Find(Map::Base &map, Controls::List &controls, Controls::Type type, int id)
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

    Scene::Base BattleActions(Point origin, int w, int h, Battle::Base &battle, Party::Base &party, int id, Uint32 background, Uint32 border, int border_size, bool ranged = false)
    {
        Controls::Collection controls = {};

        auto overlay = Scene::Base();

        auto &character = party[id];

        auto is_player = character.IsPlayer();

        auto src = is_player ? battle.Map.Find(Map::Object::PLAYER, id) : battle.Map.Find(Map::Object::ENEMY, id);

        auto is_enthralled = character.Is(Character::Status::ENTHRALLED);

        // adjacency checks (inverted)
        auto no_enemy = !battle.Map.Adjacent(src, Map::Object::ENEMY);

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

                    // has steel sceptre
                    if (character.IsArmed(Item::Property::SECONDARY) && character.IsArmed(Item::Type::STEEL_SCEPTRE, Item::Requirements(Item::Type::STEEL_SCEPTRE)))
                    {
                        controls.push_back(Controls::Type::STEEL_SCEPTRE);
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

                if (is_player && exits && !battle.Has(Battle::Condition::CANNOT_FLEE) && Engine::CanFlee(battle.Map, party, id) && !ranged)
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

        auto popup_h = BloodSword::HalfTile * 5;

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
            scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::LEFT), Point(left_x, mid_y - BloodSword::HalfTile)));

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
            scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::UP), Point(mid_x - BloodSword::HalfTile, top_y)));

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
            scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::RIGHT), Point(right_x, mid_y - BloodSword::HalfTile)));

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
            scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::DOWN), Point(mid_x - BloodSword::HalfTile, bottom_y)));

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

    Scene::Base BattleScene(Battle::Base &battle, Party::Base &party, Scene::Elements &assets, Controls::List &controls, Point location)
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

        Controls::List controls = {};

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
        auto no_enemy = !battle.Map.Adjacent(origin, Map::Object::ENEMY);

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

        Asset::List asset_list = {Asset::Type::EXIT};

        Controls::Collection controls_list = {Controls::Type::EXIT};

        if (battle.Map.ViewX < battle.Map.Width || battle.Map.ViewY < battle.Map.Height)
        {
            asset_list.push_back(Asset::Type::CENTER);

            controls_list.push_back(Controls::Type::CENTER);
        }

        if (!origin.IsNone())
        {
            // can move
            if (Move::Available(battle.Map, origin) && !character.Is(Character::Status::ENTANGLED) && !ranged)
            {
                asset_list.push_back(Asset::Type::MOVE);

                controls_list.push_back(Controls::Type::MOVE);
            }

            // defend
            asset_list.push_back(Asset::Type::DEFEND);

            controls_list.push_back(Controls::Type::DEFEND);

            if (!battle.Has(Battle::Condition::NO_COMBAT))
            {
                auto player_fighter = is_player && adj_enemy;

                auto enemy_fighter = !is_player && adj_player;

                if ((player_fighter || enemy_fighter) && !ranged)
                {
                    // can fight
                    asset_list.push_back(Asset::Type::FIGHT);

                    controls_list.push_back(Controls::Type::FIGHT);

                    // has quarterstaff skill
                    if (character.Has(Skills::Type::QUARTERSTAFF) && character.IsArmed(Item::Type::QUARTERSTAFF))
                    {
                        asset_list.push_back(Asset::Type::QUARTERSTAFF);

                        controls_list.push_back(Interface::ActionControls[Skills::Type::QUARTERSTAFF]);
                    }

                    // has steel sceptre
                    if (character.IsArmed(Item::Property::SECONDARY) && character.IsArmed(Item::Type::STEEL_SCEPTRE, Item::Type::CHARGE))
                    {
                        controls_list.push_back(Controls::Type::STEEL_SCEPTRE);
                    }
                }

                auto player_shooter = is_player && Engine::CanShoot(character) && enemies && no_enemy;

                auto enemy_shooter = (enthralled || normal) && Engine::CanShoot(character);

                if ((player_shooter || enemy_shooter) && Interface::ActionControls[character.Shoot] != Controls::Type::NONE)
                {
                    // can shoot
                    if (character.Shoot == Skills::Type::SHURIKEN)
                    {
                        asset_list.push_back(Asset::Type::SHURIKEN);
                    }
                    else
                    {
                        asset_list.push_back(Asset::Type::SHOOT);
                    }

                    controls_list.push_back(Interface::ActionControls[character.Shoot]);
                }

                // can cast spells
                if (character.Has(Skills::Type::SPELLS) && !ranged)
                {
                    asset_list.push_back(Asset::Type::SPELLS);

                    controls_list.push_back(Interface::ActionControls[Skills::Type::SPELLS]);
                }

                if (is_player && exits && !battle.Has(Battle::Condition::CANNOT_FLEE) && Engine::CanFlee(battle.Map, party, id) && !ranged)
                {
                    asset_list.push_back(Asset::Type::FLEE);

                    controls_list.push_back(Controls::Type::FLEE);
                }

                if (is_player && character.Items.size() > 0 && !ranged)
                {
                    asset_list.push_back(Asset::Type::ITEMS);

                    controls_list.push_back(Controls::Type::ITEMS);
                }
            }
        }

        Scene::Elements assets = {};

        Controls::List controls = {};

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

    Scene::Base BattleScene(Battle::Base &battle, Party::Base &party, Character::Base &character, int id, Point origin, bool ranged = false)
    {
        return Interface::BattleScene(battle, party, Point(battle.Map.DrawX, battle.Map.DrawY + BloodSword::TileSize + BloodSword::Pad), character, id, origin, ranged);
    }

    // generate status
    BloodSword::Textures GenerateStatus(Graphics::Base &graphics, Party::Base &party, bool in_battle = true)
    {
        return Interface::Status(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, in_battle);
    }

    // fight action
    bool ResolveFight(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Character::Base &attacker, Character::Base &defender, Skills::Type skill)
    {
        auto alive = true;

        auto window_w = BloodSword::TileSize * 8 + BloodSword::HalfTile;

        auto window_h = BloodSword::QuarterTile * 18 - BloodSword::Pad;

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

        if (!attacker.Is(Character::Status::DEFENDING))
        {
            if (Engine::IsAlive(attacker))
            {
                auto roll = 2;

                roll += defender.Is(Character::Status::DEFENDING) ? 1 : 0;

                roll += knockout ? 1 : 0;

                roll += attacker.Has(Character::Status::NIGHTHOWL) ? 1 : 0;

                auto modifier = defender.Has(Skills::Type::DODGING) ? 1 : 0;

                if (Interface::Target(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, attacker, defender.Asset, Attribute::Type::FIGHTING_PROWESS, roll, modifier, asset, true, Item::Property::PRIMARY))
                {
                    auto hit = Interface::CombatDamage(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, attacker, defender, skill, asset, true, attacker.Has(Skills::Type::IGNORE_ARMOUR));

                    if (hit > 0)
                    {
                        alive &= Engine::GainEndurance(defender, -hit, true);

                        auto effect = BloodSword::Find(Interface::SkillEffects, skill);

                        // do not stack up effects
                        if (alive && effect != Character::Status::NONE && !defender.IsImmune(skill) && !defender.Has(effect))
                        {
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
                    }
                }
            }
        }

        return alive;
    }

    // Fight helper function
    void Fight(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Character::Base &attacker, int attacker_id, Character::Base &defender, int defender_id, Skills::Type melee)
    {
        auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

        auto map_w = battle.Map.ViewX * battle.Map.TileSize;

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        // initialize alive checks
        auto alive = true;

        alive &= Interface::ResolveFight(graphics, background, draw, map_w, map_h, attacker, defender, melee);

        if (!alive)
        {
            battle.Map.Remove(defender.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, defender_id);

            Interface::MessageBox(graphics, background, defender.Name + " KILLED!", defender.IsPlayer() ? Color::Highlight : Color::Active);
        }
        else if (defender.Is(Character::Status::PARALYZED))
        {
            // paralyzed combatants are removed from combat
            battle.Map.Remove(defender.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, defender_id);
        }
        else if (!defender.Is(Character::Status::KNOCKED_OUT) && !defender.Is(Character::Status::DEFENDING))
        {
            if (defender.IsPlayer())
            {
                defender.Add(Character::Status::IN_COMBAT);
            }

            // retaliate only if not knocked out, and defending

            // reset alive checks
            alive = true;

            alive &= Interface::ResolveFight(graphics, background, draw, map_w, map_h, defender, attacker, defender.Fight);

            if (!alive)
            {
                battle.Map.Remove(attacker.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, attacker_id);

                Interface::MessageBox(graphics, background, attacker.Name + " KILLED!", attacker.IsPlayer() ? Color::Highlight : Color::Active);
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

        auto window_w = BloodSword::TileSize * 8 + BloodSword::HalfTile;

        auto window_h = BloodSword::QuarterTile * 18 - BloodSword::Pad;

        auto window = origin + (Point(w, h) - Point(window_w, window_h)) / 2;

        if (!attacker.Is(Character::Status::DEFENDING))
        {
            if (Engine::IsAlive(attacker))
            {
                auto roll = defender.Is(Character::Status::DEFENDING) ? 3 : 2;

                roll += attacker.Has(Character::Status::NIGHTHOWL) ? 1 : 0;

                auto modifier = defender.Has(Skills::Type::DODGING) ? 1 : 0;

                if (Interface::Target(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, attacker, defender.Asset, Attribute::Type::FIGHTING_PROWESS, roll, modifier, asset, true, Item::Property::RANGED))
                {
                    auto hit = Interface::CombatDamage(graphics, background, window, window_w, window_h, Color::Active, BloodSword::Border, attacker, defender, shot, asset, true, attacker.Has(Skills::Type::IGNORE_ARMOUR));

                    alive &= Engine::GainEndurance(defender, -hit, true);

                    if (hit > 0 && shot == Skills::Type::POISONED_DAGGER)
                    {
                        // instant death
                        defender.Value(Attribute::Type::ENDURANCE, 0);

                        alive = false;

                        alive = false;
                    }
                }
            }
        }

        return alive;
    }

    // shoot helper
    void Shoot(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Character::Base &attacker, Character::Base &defender, int defenderid)
    {
        auto draw = Point(battle.Map.DrawX, battle.Map.DrawY);

        auto map_w = battle.Map.ViewX * battle.Map.TileSize;

        auto map_h = battle.Map.ViewY * battle.Map.TileSize;

        auto asset = Engine::CanShoot(attacker) ? Skills::Assets[attacker.Shoot] : Asset::Type::SHOOT;

        auto alive = Interface::Shoot(graphics, background, draw, map_w, map_h, attacker, defender, attacker.Shoot, asset);

        if (Engine::CanShoot(attacker) && attacker.Shoot == Skills::Type::ARCHERY)
        {
            attacker.Remove(Item::Requirements(Item::Type::BOW, true), 1);
        }

        if (!alive)
        {
            battle.Map.Remove(defender.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, defenderid);

            Interface::MessageBox(graphics, background, defender.Name + " KILLED!", defender.IsPlayer() ? Color::Highlight : Color::Active);
        }
    }

    // helper function (next character in battle order)
    bool NextCharacter(Battle::Base &battle, Scene::Base &scene, Party::Base &party, Engine::Queue &order, int &combatant, Controls::User &input, bool &endturn)
    {
        auto next = Engine::NextInQueue(order, combatant);

        auto character = order[combatant].Id;

        if (Engine::IsPlayer(order, combatant))
        {
            input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[character].Class]);
        }
        else if (Engine::IsEnemy(order, combatant) && battle.Opponents[character].Is(Character::Status::ENTHRALLED))
        {
            input.Current = Interface::Find(battle.Map, scene.Controls, Controls::Type::ENEMY, character);
        }

        endturn = true;

        return next;
    }

    // generate stats
    void RegenerateStats(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party, BloodSword::Textures &party_stats, BloodSword::Textures &party_status, BloodSword::Textures &enemy_stats, BloodSword::Textures &enemy_status)
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

    // checks if enthrallment is broken
    void CheckEnthrallment(Graphics::Base &graphics, Battle::Base &battle, Scene::Base &scene, Character::Base &character, Graphics::RichText &text)
    {
        auto is_enemy = character.IsEnemy();

        // check if enthralment is broken
        if (is_enemy && character.Is(Character::Status::ENTHRALLED))
        {
            auto roll = Engine::Roll(1, 0);

            if (roll.Sum == 6)
            {
                Interface::MessageBox(graphics, scene, text.Text, Color::Highlight);

                Engine::Cancel(character, Character::Status::ENTHRALLED);
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

        auto popup_w = BloodSword::TileSize * 8 + BloodSword::HalfTile;

        auto popup_h = BloodSword::QuarterTile * 18 - BloodSword::Pad;

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
            battle.Map.Remove(target.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, targetid);

            Interface::MessageBox(graphics, background, target.Name + " KILLED!", target.IsPlayer() ? Color::Highlight : Color::Active);
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
            Graphics::TextList tiger_eye =
                {Graphics::RichText("PLAYER FPR/DMG ROLLS +2", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
                 Graphics::RichText(" PARTY FPR/DMG ROLLS +1", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0)};

            auto popup = draw + (Point(map_w, map_h) - Point(BloodSword::TileSize * 8, (BloodSword::TileSize * 2 + BloodSword::QuarterTile - BloodSword::SmallPad))) / 2;

            auto tiger = Interface::Choice(graphics, background, tiger_eye, popup, BloodSword::TileSize * 8, BloodSword::TileSize, 2, Color::Background, Color::Inactive, Color::Active, true);

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

    void Center(Battle::Base &battle, Map::Object character, int id)
    {
        auto src = battle.Map.Find(character, id);

        battle.Map.X = src.X - (battle.Map.ViewX) / 2 + 1;

        battle.Map.Y = src.Y - (battle.Map.ViewY) / 2 + 1;

        battle.Map.CheckBounds();
    }

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

                    Interface::Shoot(graphics, scene, battle, attacker, defender, selected);
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

    int SelectSpellTargets(Battle::Base &battle, Party::Base &party, Engine::Queue &spell_targets, Spells::Type spell)
    {
        auto target = -1;

        if (spell != Spells::Type::NONE)
        {
            for (auto i = 0; i < spell_targets.size(); i++)
            {
                auto target_id = spell_targets[i].Id;

                auto &character = (spell_targets[i].Type == Character::ControlType::NPC) ? battle.Opponents[target_id] : party[target_id];

                auto status = BloodSword::Find(Interface::SpellEffects, spell);

                if (Engine::IsAlive(character) && status != Character::Status::NONE && !character.Has(status))
                {
                    target = i;

                    break;
                }
            }
        }

        return target;
    }

    bool CanCastSpells(Battle::Base &battle, Party::Base &party, Character::Base &caster, int caster_id)
    {
        auto cast = false;

        // count number of live opponents
        auto opponents = caster.ControlType == Character::ControlType::NPC ? Engine::Count(party) : Engine::Count(battle.Opponents);

        // determine if this is the caster's turn to cast spells (also check if list is being generated)
        auto my_turn = (std::find(battle.Casters.begin(), battle.Casters.end(), caster_id) != battle.Casters.end()) || (battle.Casters.size() == 0);

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

                Interface::LogSpellStrategy(strategy.AlreadyCast, spells_cast, cast);

                if (cast)
                {
                    break;
                }
            }
        }
        else if (battle.Casters.size() > 0)
        {
            Interface::LogCaster(caster, caster_id);
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

        Interface::LogCasters(battle);
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

            Interface::LogTargets("SPELL", character.Target, battle.Map[src].Id, targets.size());

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
                            Interface::LogSpellCasting(character.Target, battle.Map[src].Id, spell);

                            // resolve spell
                            Interface::ResolveSpell(graphics, battle, scene, character, party, spell);
                        }
                        else
                        {
                            if (targets.size() > 0)
                            {
                                // filter spell targets further
                                auto target = Interface::SelectSpellTargets(battle, party, targets, spell);

                                if (target >= 0 && target < targets.size())
                                {
                                    auto target_id = targets[target].Id;

                                    auto &defender = targets[target].Type == Character::ControlType::NPC ? battle.Opponents[target_id] : party[target_id];

                                    std::string spell_action = "CASTS " + std::string(Spells::TypeMapping[spell]);

                                    Interface::LogAction(spell_action.c_str(), character.Target, battle.Map[src].Id, defender.Target, target_id);

                                    Interface::ResolveSpell(graphics, battle, scene, character, defender, target_id, spell);
                                }
                            }
                        }
                    }
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
            // preferred targetting
            targets = Engine::RangedTargets(battle.Map, battle.Opponents, party, src, true);
        }
        else
        {
            // default
            targets = Engine::RangedTargets(battle.Map, party, src, true, false);
        }

        Interface::LogTargets("SHOOT", character.Target, battle.Map[src].Id, targets.size());

        // shoot only when there are no nearby player enemies
        if (targets.size() > 0 && opponents.size() == 0)
        {
            for (auto &target : targets)
            {
                auto &defender = (target.Type == Character::ControlType::PLAYER) ? party[target.Id] : battle.Opponents[target.Id];

                // shoot first available target
                if (!defender.IsImmune(character.Shoot))
                {
                    Interface::LogAction("SHOOTS", character.Target, battle.Map[src].Id, defender.Target, target.Id);

                    // shoot
                    Interface::Shoot(graphics, scene, battle, character, defender, target.Id);

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

        Interface::LogTargets("MOVE", character.Target, battle.Map[src].Id, targets.size());

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

    Engine::Queue EnemyFights(Battle::Base &battle, Party::Base &party, Character::Base &character, Point &src)
    {
        // check if enemy can move towards the player-controlled characters
        if (character.Targets.size() > 0 && character.TargetProbability > 0 && character.TargetProbability < 100)
        {
            // PREFERRED TARGET:
            return Engine::FightTargets(battle.Map, battle.Opponents, party, src, true);
        }
        else
        {
            // DEFAULT
            return Engine::FightTargets(battle.Map, party, src, true, false);
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
                if (Engine::IsAlive(party[i]))
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

        Interface::LogCombatants(party, "PLAYERS");
    }

    void SetEnemyLocations(Battle::Base &battle, Party::Base &party)
    {
        auto spawn = 0;

        auto enemy_away = 0;

        auto reinforce = 0;

        if (battle.Opponents.Count() > 0)
        {
            // set opposing party's location
            battle.Opponents.Set(party.Location);

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
                        Interface::LogGroupAction("SURVIVOR", "ADD", party.Survivors[i].Target, i);

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
                        Interface::LogGroupAction("PARTY SURVIVOR", "DELETE", party.Survivors[remove[i]].Target, remove[i]);

                        // remove from survivor list
                        party.Survivors.erase(party.Survivors.begin() + remove[i]);
                    }
                }

                Interface::LogGroup(source, "REINFORCEMENTS", "PARTY SURVIVORS", survivors.Count(), party.Survivors.size());

                if (battle.Map.Survivors.size() >= survivors.Count())
                {
                    auto id = battle.Opponents.Count();

                    for (auto i = 0; i < survivors.Count(); i++)
                    {
                        // set current location
                        survivors[i].Location = party.Location;

                        if (battle.Has(Battle::Condition::HEAL_SURVIVORS))
                        {
                            Interface::LogGroupAction("SURVIVOR", "HEAL", survivors[i].Target, i);

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

        Interface::LogCombatants(battle.Opponents, "OPPONENTS");
    }

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
            Scene::Element(Asset::Get(Asset::Type::CONFIRM), location.X, location.Y + map_h),
            Scene::Element(Asset::Get(Asset::Type::BACK), location.X + BloodSword::TileSize + BloodSword::Pad, location.Y + map_h)};

        Controls::List controls = {Controls::Base(Controls::Type::CONFIRM, id, id, id + 1, id - battle.Map.ViewX, id, location.X, location.Y + map_h, battle.Map.TileSize, battle.Map.TileSize, Color::Active),
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

            if (input.Selected && (input.Type != Controls::Type::NONE) && !input.Hold)
            {
                if (Input::IsValid(map, input))
                {
                    auto &control = map.Controls[input.Current];

                    if (control.OnMap && battle.Map[map.Controls[input.Current].Map].IsOccupied())
                    {
                        Interface::MessageBox(graphics, map, "THIS SPACE IS OCCUPIED", Color::Highlight);
                    }
                    else if (control.OnMap && battle.Map[map.Controls[input.Current].Map].IsPassable() && !battle.Map[map.Controls[input.Current].Map].IsExit())
                    {
                        auto remove = -1;

                        // check if this has been added already
                        for (auto i = 0; i < origins.size(); i++)
                        {
                            if (origins[i] == map.Controls[input.Current].Map)
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
                                origins.push_back(map.Controls[input.Current].Map);

                                battle.Map[map.Controls[input.Current].Map].Asset = Asset::Type::SELECT;

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

                            battle.Map[map.Controls[input.Current].Map].Asset = Asset::Type::NONE;

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
                                battle.Map[origins[i]].Asset = Asset::Type::NONE;
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

                            input.Selected = false;

                            regenerate_scene = true;
                        }
                    }
                    else if (input.Type == Controls::Type::MAP_UP)
                    {
                        if (battle.Map.Y > 0)
                        {
                            battle.Map.Y--;

                            input.Selected = false;

                            regenerate_scene = true;
                        }
                    }
                    else if (input.Type == Controls::Type::MAP_LEFT)
                    {
                        if (battle.Map.X > 0)
                        {
                            battle.Map.X--;

                            input.Selected = false;

                            regenerate_scene = true;
                        }
                    }
                    else if (input.Type == Controls::Type::MAP_RIGHT)
                    {
                        if (battle.Map.X < (battle.Map.Width - battle.Map.ViewX))
                        {
                            battle.Map.X++;

                            input.Current = -1;

                            input.Selected = false;

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
                    input.Current = Controls::Find(map.Controls, input.Type);
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

    void HighlightControl(Scene::Base &scene, SDL_Texture *texture, Controls::Type control)
    {
        auto index = Controls::Find(scene.Controls, control);

        if (index >= 0 && index < scene.Controls.size())
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

        if (battle.InCombatTarget != Character::Class::NONE)
        {
            std::cerr << "[IN COMBAT] [" << Character::ClassMapping[battle.InCombatTarget] << "]" << std::endl;
        }
    }

    // fight battle
    Battle::Result RenderBattle(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party)
    {
        // set initial result (DETERMINE = battle result to be determined)
        auto result = Battle::Result::DETERMINE;

        // save original battle and party
        auto copy_battle = battle;

        auto copy_party = party;

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
            if (battle.Has(Battle::Condition::TACTICS) || party.Has(Character::Status::TACTICS))
            {
                // setup player locations
                Interface::Tactics(graphics, battle, party);

                // remove TACTICS status from party
                party.Remove(Character::Status::TACTICS);
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

            // final checks on placement of all combatants
            Interface::FinalLocationChecks(battle, party);

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

            Controls::Collection caption_controls = {
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

            Controls::Collection action_controls = {
                Controls::Type::MOVE,
                Controls::Type::SHOOT,
                Controls::Type::SHURIKEN,
                Controls::Type::FIGHT,
                Controls::Type::QUARTERSTAFF,
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

            SDL_Texture *texture = nullptr;

            auto asset = Asset::Type::NONE;

            auto lifetime = -1;

            int round = 0;

            SDL_Texture *round_string = nullptr;

            // move animation
            auto movement = Animation::Base();

            // spell to cast
            auto cast = Spells::Type::NONE;

            // quarterstaff targetting
            auto knockout = Skills::Type::NONE;

            // for fighting with an item
            auto fight_item = Item::Type::NONE;

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

            // exit battle
            auto exit = false;

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

            while ((round < battle.Duration || battle.Duration == Battle::Unlimited) && Engine::IsAlive(party) && Engine::IsAlive(battle.Opponents, Character::ControlType::NPC) && !Engine::IsFleeing(party) && (Engine::InBattle(party) > 0) && !exit)
            {
                // battle order
                Engine::Queue order = {};

                // spells already cast by NPC players
                battle.AlreadyCast.clear();

                // generate spell casters this turn
                Interface::GenerateCasters(battle, party);

                auto ambush = true;

                if (battle.Has(Battle::Condition::AMBUSH_PLAYER) && round < battle.AmbushRounds)
                {
                    // players get a free initial turn
                    order = Engine::Build(party, Attribute::Type::AWARENESS, true, true);
                }
                else if (battle.Has(Battle::Condition::AMBUSH_NPC) && round < battle.AmbushRounds)
                {
                    // enemy combatants get a free initial turn
                    order = Engine::Build(battle.Opponents, Attribute::Type::AWARENESS, true, true);
                }
                else if (battle.Has(Battle::Condition::AMBUSH_PLAYER_RANGED) && round < battle.AmbushRounds)
                {
                    // ranged attackers get a free initial turn
                    order = Engine::Shooters(party, Attribute::Type::AWARENESS, true, true);
                }
                else if (battle.Has(Battle::Condition::AMBUSH_NPC_RANGED) && round < battle.AmbushRounds)
                {
                    for (auto i = 0; i < battle.AmbushRounds; i++)
                    {
                        // enemies get a free ranged attack
                        Interface::RenderAmbushRangedAttack(graphics, battle, party);

                        // move round
                        round++;
                    }

                    ambush = false;
                }
                else
                {
                    // otherwise create battle order (default)
                    order = Engine::Build(party, battle.Opponents, Attribute::Type::AWARENESS, true, true);

                    ambush = false;
                }

                // regenerate round string
                BloodSword::Free(&round_string);

                round_string = Graphics::CreateText(graphics, (std::string("ROUND ") + std::to_string(round + 1) + (ambush ? std::string(" (AMBUSH)") : "")).c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

                // start of round effects
                auto next = false;

                // reset IN COMBAT status
                Engine::ResetCombatStatus(party);

                Engine::ResetCombatStatus(battle.Opponents);

                // start with current character
                auto combatant = 0;

                // controls list
                Controls::Collection battle_actions = {
                    Controls::Type::MOVE,
                    Controls::Type::DEFEND,
                    Controls::Type::FIGHT,
                    Controls::Type::QUARTERSTAFF,
                    Controls::Type::SHOOT,
                    Controls::Type::SHURIKEN,
                    Controls::Type::FLEE,
                    Controls::Type::SPELLS,
                    Controls::Type::ITEMS};

                while (!next && Engine::IsAlive(party) && Engine::IsAlive(battle.Opponents, Character::ControlType::NPC) && !Engine::IsFleeing(party) && !exit)
                {
                    auto is_enemy = Engine::IsEnemy(order, combatant);

                    auto is_player = Engine::IsPlayer(order, combatant);

                    auto character_id = order[combatant].Id;

                    auto &character = is_player ? party[character_id] : battle.Opponents[character_id];

                    auto origin = battle.Map.Find(is_player ? Map::Object::PLAYER : Map::Object::ENEMY, character_id);

                    // flag for ranged attacks only
                    auto ranged_only = battle.Has(Battle::Condition::AMBUSH_PLAYER_RANGED) && round < battle.AmbushRounds;

                    // center map on player
                    Interface::Center(battle, is_player ? Map::Object::PLAYER : Map::Object::ENEMY, character_id);

                    // regenerate scene
                    auto scene = Interface::BattleScene(battle, party, character, character_id, origin, ranged_only);

                    // start of character turn
                    if (round > 0 && Engine::CoolDown(character))
                    {
                        // regenerate stats
                        Interface::RegenerateStats(graphics, battle, party, party_stats, party_status, enemy_stats, enemy_status);
                    }

                    auto end_turn = false;

                    Interface::SearchInCombatTargets(battle, party, order, combatant);

                    while (!end_turn && Engine::IsAlive(party) && Engine::IsAlive(battle.Opponents, Character::ControlType::NPC) && !Engine::IsFleeing(party) && !exit)
                    {
                        auto overlay = Scene::Base();

                        auto refresh_textures = false;

                        auto performed_action = false;

                        auto regenerate_scene = false;

                        // main loop
                        if (!animating)
                        {
                            auto src = battle.Map.Find(Engine::IsPlayer(order, combatant) ? Map::Object::PLAYER : Map::Object::ENEMY, character_id);

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

                                        if (Interface::CanCastSpells(battle, party, character, character_id) && !battle.Has(Battle::Condition::NO_COMBAT))
                                        {
                                            character.Add(Character::Status::IN_COMBAT);

                                            // cast or call to mind spell
                                            Interface::EnemyCastSpells(graphics, scene, battle, party, character, src);
                                        }
                                        else if (opponents.size() > 0 && !battle.Has(Battle::Condition::NO_COMBAT))
                                        {
                                            character.Add(Character::Status::IN_COMBAT);

                                            Interface::LogTargets("FIGHT", character.Target, character_id, opponents.size());

                                            Engine::ResetSpells(character);

                                            // fight
                                            auto defender_id = opponents[0].Id;

                                            auto &defender = ((opponents[0].Type == Character::ControlType::PLAYER) ? party[defender_id] : battle.Opponents[defender_id]);

                                            Interface::LogAction("FIGHTS", character.Target, character_id, defender.Target, defender_id);

                                            Interface::Fight(graphics, scene, battle, character, character_id, defender, defender_id, character.Fight);
                                        }
                                        else if (character.Has(Skills::Type::ATTACKS_ENGAGED) && !full_engage)
                                        {
                                            if (Engine::CanShoot(character) && opponents.size() == 0)
                                            {
                                                auto target_id = party.Index(battle.InCombatTarget);

                                                if (target_id >= 0 && target_id < party.Count() && Engine::CanTarget(party[target_id], true))
                                                {
                                                    // shoot at IN COMBAT target
                                                    Interface::LogAction("SHOOTS", character.Target, character_id, party[target_id].Target, target_id);

                                                    Interface::Shoot(graphics, scene, battle, character, party[target_id], target_id);

                                                    if (!Engine::CanTarget(party[target_id], true))
                                                    {
                                                        Interface::SearchInCombatTargets(battle, party, order, combatant);
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
                                        overlay = Interface::BattleActions(draw, map_w, map_h, battle, is_player ? party : battle.Opponents, character_id, Color::Background, Color::Active, BloodSword::Border, ranged_only);

                                        // round number
                                        overlay.VerifyAndAdd(Scene::Element(round_string, battle.Map.DrawX, text_y));

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
                                    if (!actions && !items && !spells)
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

                                    if (input.Selected && (input.Type != Controls::Type::NONE) && !input.Hold)
                                    {
                                        if (Input::IsValid(scene, input) && !actions && !items && !spells && (Controls::Find(battle_actions, input.Type) == -1))
                                        {
                                            auto &control = scene.Controls[input.Current];

                                            if (control.OnMap && battle.Map[scene.Controls[input.Current].Map].Id == character_id && ((is_player && battle.Map[scene.Controls[input.Current].Map].IsPlayer()) || (is_enemy && battle.Map[scene.Controls[input.Current].Map].IsEnemy())))
                                            {
                                                fight_item = Item::Type::NONE;

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
                                                        auto start = battle.Map.Find(Engine::IsPlayer(order, combatant) ? Map::Object::PLAYER : Map::Object::ENEMY, character_id);

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
                                                        if (fight_item == Item::Type::NONE)
                                                        {
                                                            Interface::Fight(graphics, scene, battle, character, character_id, battle.Opponents[fighter], fighter, knockout);
                                                        }

                                                        // checks if enthrallment is broken
                                                        Interface::CheckEnthrallment(graphics, battle, scene, character, Interface::Text[Interface::MSG_ENTHRAL]);

                                                        refresh_textures = true;

                                                        performed_action = true;
                                                    }

                                                    knockout = Skills::Type::NONE;

                                                    fight_item = Item::Type::NONE;

                                                    fight = false;
                                                }
                                                else if (shoot)
                                                {
                                                    // target chosen
                                                    auto distance = battle.Map.Distance(src, control.Map);

                                                    if (distance > 1)
                                                    {
                                                        auto shooter = battle.Map[control.Map].Id;

                                                        auto &shot_target = battle.Opponents[shooter];

                                                        if (!shot_target.IsImmune(character.Shoot))
                                                        {
                                                            character.Add(Character::Status::IN_COMBAT);

                                                            Engine::ResetStatus(character);

                                                            // shoot
                                                            Interface::Shoot(graphics, scene, battle, character, shot_target, shooter);

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
                                                                    Interface::ResolveSpell(graphics, battle, scene, character, spell_target, battle.Map[control.Map].Id, cast);

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

                                                end_turn = true;

                                                next = true;

                                                exit = true;
                                            }

                                            input.Selected = false;

                                            input.Current = -1;
                                        }
                                        else if ((actions && Input::IsValid(overlay, input)) || (!actions && !items && !spells && Input::IsValid(scene, input) && Controls::Find(battle_actions, input.Type) != -1))
                                        {
                                            // regenerate controls
                                            if (!actions)
                                            {
                                                regenerate_scene = true;
                                            }

                                            fight_item = Item::Type::NONE;

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
                                            else if (input.Type == Controls::Type::FIGHT || input.Type == Controls::Type::QUARTERSTAFF || input.Type == Controls::Type::STEEL_SCEPTRE)
                                            {
                                                auto opponents = Engine::FightTargets(battle.Map, battle.Opponents, src, true, false);

                                                if (opponents.size() == 1)
                                                {
                                                    character.Add(Character::Status::IN_COMBAT);

                                                    Engine::ResetStatusAndSpells(character);

                                                    knockout = ((input.Type == Controls::Type::QUARTERSTAFF) && character.Has(Skills::Type::QUARTERSTAFF)) ? Skills::Type::QUARTERSTAFF : character.Fight;

                                                    fight_item = ((input.Type == Controls::Type::STEEL_SCEPTRE)) && character.IsArmed(Item::Type::STEEL_SCEPTRE, Item::Type::CHARGE) ? Item::Type::STEEL_SCEPTRE : Item::Type::NONE;

                                                    // fight
                                                    if (fight_item == Item::Type::NONE)
                                                    {
                                                        Interface::Fight(graphics, scene, battle, character, character_id, battle.Opponents[opponents[0].Id], opponents[0].Id, knockout);
                                                    }
                                                    else
                                                    {
                                                        // fight with item
                                                    }

                                                    refresh_textures = true;

                                                    performed_action = true;
                                                }
                                                else if (opponents.size() > 1)
                                                {
                                                    if ((input.Type == Controls::Type::QUARTERSTAFF) && character.Has(Skills::Type::QUARTERSTAFF))
                                                    {
                                                        knockout = Skills::Type::QUARTERSTAFF;
                                                    }
                                                    else if ((input.Type == Controls::Type::STEEL_SCEPTRE) && character.IsArmed(Item::Type::STEEL_SCEPTRE, Item::Type::CHARGE))
                                                    {
                                                        fight_item = Item::Type::STEEL_SCEPTRE;
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

                                                    fight_item = Item::Type::NONE;
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
                                                    auto targets = Engine::RangedTargets(battle.Map, battle.Opponents, src, true, false);

                                                    auto target_id = targets[0].Id;

                                                    if (targets.size() == 1)
                                                    {
                                                        if (!battle.Opponents[target_id].IsImmune(character.Shoot))
                                                        {
                                                            character.Add(Character::Status::IN_COMBAT);

                                                            Engine::ResetStatusAndSpells(character);

                                                            // shoot
                                                            Interface::Shoot(graphics, scene, battle, character, battle.Opponents[target_id], target_id);

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
                                            auto ptr = Interface::ControlSpellMapping.find(input.Type);

                                            spells = false;

                                            spell = false;

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
                                                                                Interface::ResolveSpell(graphics, battle, scene, character, spell_target, battle.Map[target].Id, cast);

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
                                            auto update = Interface::ShowInventory(graphics, scene, battle, party, character);

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

                                input.Current = -1;

                                input.Selected = false;

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
                            // next character in battle order
                            next = Interface::NextCharacter(battle, scene, party, order, combatant, input, end_turn);
                        }
                        else if (regenerate_scene)
                        {
                            // regenerate scene (on map movement, movement, etc.)
                            scene = Interface::BattleScene(battle, party, character, character_id, origin, ranged_only);
                        }
                    }

                    // check if within endurance limit
                    if (battle.Endurance != Battle::Unlimited)
                    {
                        if (Engine::Min(battle.Opponents, Attribute::Type::ENDURANCE, true) <= battle.Endurance)
                        {
                            result = Battle::Result::VICTORY;

                            exit = true;

                            next = true;

                            end_turn = true;
                        }
                        else if (Engine::Min(party, Attribute::Type::ENDURANCE, true) <= battle.Endurance)
                        {
                            result = Battle::Result::DEFEAT;

                            exit = true;

                            next = true;

                            end_turn = true;
                        }
                    }
                }

                // end of round effects
                battle.Map.CoolDown();

                // move to next round
                round++;
            }

            // round limit exceeded
            if (battle.Duration != Battle::Unlimited && round >= battle.Duration)
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
            party.BattleLocation = party.Location;
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
        else if (result == Battle::Result::NONE)
        {
            party = copy_party;

            battle = copy_battle;
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
            Interface::LogSurvivors(battle.Opponents.Location, "BATTLE", "PARTY", survivors, party.Survivors.size());
        }

        if ((result == Battle::Result::VICTORY || result == Battle::Result::ENTHRALLED) && battle.Loot.size() > 0)
        {
            // regnerate final scene
            auto scene = BattleScene(battle, party);

            // pick up any loot
            Interface::ShowInventory(graphics, scene, party, battle.Loot);
        }

        // log battle results
        Interface::LogBattleResults(battle, party, initial_result, result);

        return result;
    }
}

#endif
