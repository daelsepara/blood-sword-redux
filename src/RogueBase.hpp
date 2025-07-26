#ifndef __ROGUE_BASE_HPP__
#define __ROGUE_BASE_HPP__

#include "RogueClasses.hpp"

namespace BloodSword::Rogue
{
    int FindLoot(Rogue::Base &rogue, Point point)
    {
        auto found = Rogue::None;

        auto &Loot = rogue.Loot;

        for (auto loot = 0; loot < Loot.size(); loot++)
        {
            if (Loot[loot].Location() == point)
            {
                found = loot;

                break;
            }
        }

        return found;
    }

    int FindOpponents(Rogue::Base &rogue, Point point)
    {
        auto found = Rogue::None;

        for (auto opponent = 0; opponent < rogue.Opponents.size(); opponent++)
        {
            if (rogue.Opponents[opponent].Origin() == point)
            {
                found = opponent;

                break;
            }
        }

        return found;
    }

    int FindOpponents(Rogue::Base &rogue, int room)
    {
        auto found = Rogue::None;

        for (auto opponent = 0; opponent < rogue.Opponents.size(); opponent++)
        {
            if (rogue.Opponents[opponent].Room == room)
            {
                found = opponent;

                break;
            }
        }

        return found;
    }

    int FindTrigger(Rogue::Base &rogue, Point point)
    {
        auto found = Rogue::None;

        auto &triggers = rogue.Triggers;

        for (auto trigger = 0; trigger < triggers.size(); trigger++)
        {
            if (triggers[trigger].Location() == point)
            {
                found = trigger;

                break;
            }
        }

        return found;
    }

    // attribute labels
    std::string StatsLabels()
    {
        // attribute labels
        auto label_1 = std::string("FPR      END      ");

        auto label_2 = std::string("AWR      DMG      ");

        auto label_3 = std::string("PSY      ARM      ");

        auto labels = label_1 + '\n' + label_2 + '\n' + label_3;

        return labels;
    }

    SDL_Texture *Stats(Graphics::Base &graphics, Character::Base &character, int w)
    {
        auto surface_labels = TTF_RenderUTF8_Blended_Wrapped(Fonts::Normal, Rogue::StatsLabels().c_str(), Color::S(Color::Active), 0);

        // texture
        SDL_Texture *texture = nullptr;

        if (surface_labels)
        {
            auto labels_h = std::max(BloodSword::TileSize, surface_labels->h + BloodSword::Pad);

            auto surface = Graphics::CreateSurface(w, BloodSword::TileSize + BloodSword::Pad + labels_h);

            if (surface)
            {
                // retrieve attribute values
                auto fpr = Interface::ScoreString(character, Attribute::Type::FIGHTING_PROWESS);

                auto awr = Interface::ScoreString(character, Attribute::Type::AWARENESS);

                auto psy = Interface::ScoreString(character, Attribute::Type::PSYCHIC_ABILITY);

                auto end = Interface::ScoreString(character, Attribute::Type::ENDURANCE);

                auto dmg = Interface::ScoreString(character, Attribute::Type::DAMAGE);

                auto arm = Interface::ScoreString(character, Attribute::Type::ARMOUR);

                auto four = std::string(4, ' ');

                // format attribute values
                auto stats_1 = four + fpr + std::string(5 - fpr.size(), ' ') + four + end + '\n';

                auto stats_2 = four + awr + std::string(5 - awr.size(), ' ') + four + dmg + '\n';

                auto stats_3 = four + psy + std::string(5 - psy.size(), ' ') + four + arm;

                auto stats = stats_1 + stats_2 + stats_3;

                // create and convert SDL surface to appropriate format
                auto surface_stats = TTF_RenderUTF8_Blended_Wrapped(Fonts::Normal, stats.c_str(), Color::S(Color::Highlight), 0);

                SDL_Rect stats_rect;

                stats_rect.w = surface->w;

                stats_rect.h = surface->h;

                stats_rect.x = BloodSword::Pad;

                stats_rect.y = BloodSword::TileSize + BloodSword::Pad;

                Graphics::RenderAsset(surface, surface_labels, stats_rect);

                Graphics::RenderAsset(surface, surface_stats, stats_rect);

                // add icon (blur if dead)
                auto surface_asset = Engine::Score(character, Attribute::Type::ENDURANCE) > 0 ? Asset::Surface(character.Asset) : Asset::Surface(Asset::Type::DEAD);

                stats_rect.x = (surface->w - BloodSword::TileSize) / 2;

                stats_rect.y = 0;

                Graphics::RenderAsset(surface, surface_asset, stats_rect);

                stats_rect.x = BloodSword::Pad;

                stats_rect.y = BloodSword::Pad;

                if (character.Has(Character::Status::IN_BATTLE) && character.Has(Character::Status::MELEE) && Engine::IsAlive(character))
                {
                    auto melee = Asset::Surface(Asset::Type::FIGHT);

                    if (melee)
                    {
                        Graphics::RenderAsset(surface, melee, stats_rect);

                        BloodSword::Free(&melee);
                    }
                }
                else if (character.Has(Character::Status::IN_BATTLE) && character.Has(Character::Status::RANGED) && Engine::IsAlive(character))
                {
                    SDL_Surface *ranged = nullptr;

                    if (character.Has(Skills::Type::ARCHERY))
                    {
                        ranged = Asset::Surface(Asset::Type::ARCHERY);
                    }
                    else if (character.Has(Skills::Type::SHURIKEN))
                    {
                        ranged = Asset::Surface(Asset::Type::SHURIKEN);
                    }
                    else
                    {
                        ranged = Asset::Surface(Asset::Type::SHOOT);
                    }

                    if (ranged)
                    {
                        Graphics::RenderAsset(surface, ranged, stats_rect);

                        BloodSword::Free(&ranged);
                    }
                }

                if (character.Has(Character::Status::DEFENDING) && Engine::IsAlive(character))
                {
                    stats_rect.x = surface->w - (BloodSword::TileSize + BloodSword::Pad);

                    auto defend = Asset::Surface(Asset::Type::DEFEND);

                    if (defend)
                    {
                        Graphics::RenderAsset(surface, defend, stats_rect);

                        BloodSword::Free(&defend);
                    }
                }

                // cleanup
                BloodSword::Free(&surface_labels);

                BloodSword::Free(&surface_stats);

                BloodSword::Free(&surface_asset);

                texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

                BloodSword::Free(&surface);
            }

            // cleanup
            BloodSword::Free(&surface_labels);
        }

        return texture;
    }

    // generate textures of party stats
    BloodSword::Textures Stats(Graphics::Base &graphics, Party::Base &party, int w)
    {
        auto textures = std::vector<SDL_Texture *>();

        for (auto character = 0; character < party.Count(); character++)
        {
            auto texture = Rogue::Stats(graphics, party[character], w);

            textures.push_back(texture);
        }

        return textures;
    }

    // check if point is a good spot
    bool GoodSpot(Rogue::Base &rogue, Room::Base &room, Point point)
    {
        auto &map = rogue.Battlepits;

        auto empty = (map[point].Type == Map::Object::PASSABLE && !map[point].IsOccupied());

        auto inside = room.Inside(point) && empty;

        if (!inside)
        {
            return false;
        }

        auto left = ((point.X == room.X1 + 1) && map[Point(room.X1, point.Y)].IsBlocked()) || (point.X > room.X1 + 1);

        auto right = ((point.X == room.X2 - 2) && map[Point(room.X2 - 1, point.Y)].IsBlocked()) || (point.X < room.X2 - 2);

        auto top = ((point.Y == room.Y1 + 1) && map[Point(point.X, room.Y1)].IsBlocked()) || (point.Y > room.Y1 + 1);

        auto bottom = ((point.Y == room.Y2 - 2) && map[Point(point.X, room.Y2 - 1)].IsBlocked()) || (point.Y < room.Y2 - 2);

        return inside && top && bottom && left && right && empty;
    }

    Points GoodSpots(Rogue::Base &rogue, Room::Base &room)
    {
        auto available = Points();

        for (auto y = room.Y1 + 2; y <= room.Y2 - 2; y++)
        {
            for (auto x = room.X1 + 2; x <= room.X2 - 2; x++)
            {
                auto point = Point(x, y);

                if (Rogue::GoodSpot(rogue, room, point))
                {
                    available.push_back(point);
                }
            }
        }

        return available;
    }

    void RenderCombatant(Scene::Base &scene, SDL_Texture *texture, int x, int y, Uint32 color)
    {
        scene.Add(Scene::Element(x, y, BloodSword::Width(texture), BloodSword::Height(texture), Color::Background, color, BloodSword::Border));

        scene.VerifyAndAdd(Scene::Element(texture, Point(x, y)));
    }

    void RenderCombatants(Graphics::Base &graphics, Scene::Base &scene, Party::Base &party, Textures &party_stats, Party::Base &enemies, BloodSword::Textures &enemy_stats, int stats_w, bool is_player, bool is_enemy, int id, Uint32 color)
    {
        // estimate positions from window
        auto window_h = BloodSword::WindowTile - BloodSword::Pad;

        auto window_y = (graphics.Height - window_h) / 2;

        auto pad = BloodSword::SmallPad;

        auto enemy_offset = (graphics.Width - (enemies.Count() * stats_w + (enemies.Count() - 1) * pad)) / 2;

        auto enemy_h = BloodSword::Height(enemy_stats);

        auto enemy_y = window_y - (BloodSword::TileSize + enemy_h + BloodSword::LargePad);

        for (auto i = 0; i < enemies.Count(); i++)
        {
            if (!is_enemy || id != i)
            {
                auto enemy_x = enemy_offset + i * (stats_w + pad);

                Rogue::RenderCombatant(scene, enemy_stats[i], enemy_x, enemy_y, Color::Inactive);
            }
        }

        if (is_enemy)
        {
            auto enemy_x = enemy_offset + id * (stats_w + pad);

            Rogue::RenderCombatant(scene, enemy_stats[id], enemy_x, enemy_y, color);
        }

        auto party_offset = (graphics.Width - (party.Count() * stats_w + (party.Count() - 1) * pad)) / 2;

        auto party_y = window_y + window_h + BloodSword::TileSize + BloodSword::Pad;

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

            Rogue::RenderCombatant(scene, party_stats[id], party_x, party_y, color);
        }
    }

    int SelectTarget(Graphics::Base &graphics, Party::Base &party, Textures &party_stats, Party::Base &enemies, BloodSword::Textures &enemy_stats, int stats_w, bool is_player, bool is_enemy, int id)
    {
        // estimate positions from window
        auto window_h = BloodSword::WindowTile - BloodSword::Pad;

        auto window_y = (graphics.Height - window_h) / 2;

        int target = Rogue::None;

        auto texture = Graphics::CreateText(graphics, "SELECT TARGET", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto pad = BloodSword::SmallPad;

        auto max_selection = is_player ? enemies.Count() : party.Count();

        auto stats_h = is_player ? BloodSword::Height(enemy_stats) : BloodSword::Height(party_stats);

        auto offset = (graphics.Width - (max_selection * stats_w + (max_selection - 1) * pad)) / 2;

        auto target_y = is_player ? (window_y - (BloodSword::TileSize + stats_h + BloodSword::LargePad)) : (window_y + window_h + BloodSword::TileSize + BloodSword::Pad);

        auto selection = is_player ? Engine::First(enemies) : Engine::First(party);

        auto done = false;

        while (!done)
        {
            auto scene = Scene::Base();

            // render all combatants
            Rogue::RenderCombatants(graphics, scene, party, party_stats, enemies, enemy_stats, stats_w, is_player, is_enemy, id, Color::Active);

            // render label
            Interface::Boxed(graphics, scene, texture, Color::Background, Color::Active, BloodSword::Border);

            // calculate target texture x-coordinate
            auto target_x = offset + selection * (stats_w + pad);

            // render target
            Rogue::RenderCombatant(scene, is_player ? enemy_stats[selection] : party_stats[selection], target_x, target_y, Color::Highlight);

            // calculate pointer x-coordinate
            auto pointer_x = offset + selection * (stats_w + pad) + (stats_w - BloodSword::TileSize) / 2;

            // // calculate pointer y-coordinate, render pointer
            if (is_player)
            {
                auto pointer_y = window_y - (BloodSword::TileSize + BloodSword::Pad);

                scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::UP), pointer_x, pointer_y));
            }
            else
            {
                auto pointer_y = window_y + window_h + BloodSword::Pad;

                scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::DOWN), pointer_x, pointer_y));
            }

            auto input = Input::RogueInput(graphics, {scene});

            if (Input::Check(input))
            {
                if (input.Type == Controls::Type::LEFT)
                {
                    if (selection > 0)
                    {
                        selection--;
                    }
                }
                else if (input.Type == Controls::Type::RIGHT)
                {
                    if (selection < max_selection - 1)
                    {
                        selection++;
                    }
                }
                else if (input.Type == Controls::Type::UP || input.Type == Controls::Type::DOWN)
                {
                    Sound::Play(Sound::Type::FAIL);
                }
                else if (input.Type == Controls::Type::ACTION)
                {
                    if (selection >= 0 && selection < max_selection)
                    {
                        target = selection;

                        done = true;
                    }
                    else
                    {
                        Sound::Play(Sound::Type::FAIL);
                    }
                }
                else if (input.Type == Controls::Type::BACK)
                {
                    done = true;
                }

                input.Selected = false;
            }
        }

        BloodSword::Free(&texture);

        return target;
    }

    int SelectTarget(Graphics::Base &graphics, Rogue::Base &rogue, bool is_player, bool is_enemy, int id)
    {
        auto target = Rogue::None;

        if (rogue.Enemy >= 0 && rogue.Enemy < rogue.Opponents.size())
        {
            auto &enemies = rogue.Opponents[rogue.Enemy];

            auto &party = rogue.Party;

            auto enemy_stats = Rogue::Stats(graphics, enemies, rogue.StatsWidth);

            auto party_stats = Rogue::Stats(graphics, party, rogue.StatsWidth);

            target = Rogue::SelectTarget(graphics, party, party_stats, enemies, enemy_stats, rogue.StatsWidth, is_player, is_enemy, id);

            BloodSword::Free(party_stats);

            BloodSword::Free(enemy_stats);
        }

        return target;
    }

    // clears the trigger on the tile
    void RemoveTrigger(Rogue::Base &rogue, int trigger, bool remove = true)
    {
        auto &battlepits = rogue.Battlepits;

        if (trigger >= 0 && trigger < rogue.Triggers.size())
        {
            auto location = rogue.Triggers[trigger].Location();

            if (battlepits.IsValid(location))
            {
                auto &tile = battlepits[location];

                tile.Type = Map::Object::PASSABLE;

                tile.Asset = Asset::Type::NONE;

                if (remove)
                {
                    rogue.Triggers.erase(rogue.Triggers.begin() + trigger);
                }
            }
        }
    }

    // check if trigger has been activated
    bool CheckTrigger(Rogue::Base &rogue, int trigger)
    {
        auto result = false;

        return result;
    }
}

#endif
