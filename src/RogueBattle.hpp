#ifndef __ROGUE_BATTLE_HPP__
#define __ROGUE_BATTLE_HPP__

#include "RogueBase.hpp"

namespace BloodSword::Rogue
{
    void RenderCombatantsStats(Graphics::Base &graphics, BloodSword::Scene::Base &scene, Party::Base &party, Textures &party_stats, Party::Base &enemies, BloodSword::Textures &enemy_stats, int stats_w, bool is_player, bool is_enemy, int id)
    {
        auto pad = BloodSword::SmallPad;

        auto enemy_offset = (graphics.Width - (enemies.Count() * stats_w + (enemies.Count() - 1) * pad)) / 2;

        for (auto i = 0; i < enemies.Count(); i++)
        {
            auto enemy_x = enemy_offset + i * (stats_w + pad);

            scene.Add(Scene::Element(enemy_x, BloodSword::Pad, BloodSword::Width(enemy_stats[i]), BloodSword::Height(enemy_stats[i]), Color::Background, (is_enemy && id == i ? Color::Active : Color::Inactive), BloodSword::Border));

            scene.VerifyAndAdd(Scene::Element(enemy_stats[i], Point(enemy_x, BloodSword::Pad)));
        }

        auto party_h = 0;

        auto party_offset = (graphics.Width - (party.Count() * stats_w + (party.Count() - 1) * pad)) / 2;

        for (auto i = 0; i < party.Count(); i++)
        {
            party_h = std::max(party_h, BloodSword::Height(party_stats[i]));
        }

        for (auto i = 0; i < party.Count(); i++)
        {
            auto party_x = party_offset + i * (stats_w + pad);

            auto party_y = graphics.Height - (party_h + BloodSword::Pad * 2);

            scene.Add(Scene::Element(party_x, party_y, BloodSword::Width(party_stats[i]), BloodSword::Height(party_stats[i]), Color::Background, (is_enemy && id == i ? Color::Active : Color::Inactive), BloodSword::Border));

            scene.VerifyAndAdd(Scene::Element(party_stats[i], Point(party_x, party_y)));
        }
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

        //while (Engine::Count(party) > 0 || Engine::Count(enemies) > 0)
        {
            auto scene = Scene::Base();

            auto battle_order = Engine::Build(party, enemies, Attribute::Type::AWARENESS, true, true);

            auto combatant = 0;

            //auto next_round = false;

            //while (!next_round && Engine::IsAlive(party) && Engine::IsAlive(enemies, Character::ControlType::NPC))
            {
                auto is_enemy = Engine::IsEnemy(battle_order, combatant);

                auto is_player = Engine::IsPlayer(battle_order, combatant);

                auto character_id = battle_order[combatant].Id;

                auto &character = is_player ? party[character_id] : enemies[character_id];

                if (round > 0 && Engine::CoolDown(character))
                {
                    // regenerate stats
                }

                Rogue::RenderCombatantsStats(graphics, scene, party, party_stats, enemies, enemy_stats, stats_w, is_player, is_enemy, character_id);

                Input::WaitForNext(graphics, scene);
            }
        }

        BloodSword::Free(party_stats);

        BloodSword::Free(enemy_stats);

        return update;
    }
}

#endif