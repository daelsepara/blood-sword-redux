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

    void RenderCombatants(Graphics::Base &graphics, BloodSword::Scene::Base &scene, Party::Base &party, Textures &party_stats, Party::Base &enemies, BloodSword::Textures &enemy_stats, int stats_w, bool is_player, bool is_enemy, int id)
    {
        auto pad = BloodSword::SmallPad;

        auto enemy_offset = (graphics.Width - (enemies.Count() * stats_w + (enemies.Count() - 1) * pad)) / 2;

        for (auto i = 0; i < enemies.Count(); i++)
        {
            if (!is_enemy || id != i)
            {
                auto enemy_x = enemy_offset + i * (stats_w + pad);

                Rogue::RenderCombatant(scene, enemy_stats[i], enemy_x, BloodSword::Pad, Color::Inactive);
            }
        }

        if (is_enemy)
        {
            auto enemy_x = enemy_offset + id * (stats_w + pad);

            Rogue::RenderCombatant(scene, enemy_stats[id], enemy_x, BloodSword::Pad, Color::Active);
        }

        auto party_h = 0;

        auto party_offset = (graphics.Width - (party.Count() * stats_w + (party.Count() - 1) * pad)) / 2;

        for (auto i = 0; i < party.Count(); i++)
        {
            party_h = std::max(party_h, BloodSword::Height(party_stats[i]));
        }

        auto party_y = graphics.Height - (party_h + BloodSword::Pad * 2);

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

            Rogue::RenderCombatant(scene, party_stats[id], party_x, party_y, Color::Active);
        }
    }

    Controls::Type SelectAction(Graphics::Base &graphics, Scene::Base &background, Character::Base &character)
    {
        auto selected = Controls::Type::NONE;

        Asset::List assets = {
            Asset::Type::EXIT,
            Asset::Type::MOVE,
            Asset::Type::DEFEND};

        Controls::List controls = {
            Controls::Type::EXIT,
            Controls::Type::MOVE,
            Controls::Type::DEFEND};

        auto captions = std::vector<std::string>{
            "EXIT",
            "MOVE",
            "DEFEND"};

        // character not in MELEE area
        if (character.Has(Character::Status::MELEE))
        {
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

        if (character.Has(Character::Status::RANGED) && Engine::CanShoot(character))
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

        auto values = std::vector<int>();

        for (auto i = 0; i < controls.size(); i++)
        {
            values.push_back(i);
        }

        auto selection = Interface::SelectIcons(graphics, background, "SELECT ACTION", assets, values, captions, 1, 1, Asset::Type::NONE, false, true, false);

        if (selection.size() == 1)
        {
            selected = controls[selection[0]];
        }

        return selected;
    }

    Rogue::Update Battle(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, int enemy)
    {
        Rogue::Update update = {false, false, false};

        auto &party = rogue.Party;

        auto &enemies = rogue.Opponents[enemy];

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
                    BloodSword::Free(enemy_stats);

                    BloodSword::Free(party_stats);

                    enemy_stats = Rogue::Stats(graphics, enemies, stats_w);

                    party_stats = Rogue::Stats(graphics, party, stats_w);
                }

                auto scene = Scene::Base();

                Rogue::RenderCombatants(graphics, scene, party, party_stats, enemies, enemy_stats, stats_w, is_player, is_enemy, character_id);

                auto end_turn = false;

                while (!end_turn && Engine::IsAlive(party) && Engine::IsAlive(enemies, Character::ControlType::NPC) && !exit_battle)
                {
                    if (is_enemy)
                    {
                        next_round = Engine::NextInQueue(battle_order, combatant);
                    }
                    else
                    {
                        auto input = Rogue::SelectAction(graphics, scene, character);

                        if (input != Controls::Type::NONE)
                        {
                            if (input == Controls::Type::EXIT)
                            {
                                exit_battle = true;
                            }

                            next_round = Engine::NextInQueue(battle_order, combatant);
                        }
                    }

                    end_turn = true;
                }
            }
        }

        BloodSword::Free(party_stats);

        BloodSword::Free(enemy_stats);

        return update;
    }
}

#endif