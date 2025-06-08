#ifndef __MAP_RENDERER_HPP__
#define __MAP_RENDERER_HPP__

#include "InterfaceBattle.hpp"

// render map to a png file
namespace BloodSword::MapRenderer
{
    void RenderAsset(SDL_Surface *surface, SDL_Surface *surface_asset, SDL_Rect &rect)
    {
        if (surface_asset)
        {
            auto converted_asset = SDL_ConvertSurface(surface_asset, surface->format, 0);

            if (converted_asset)
            {
                // place icon in the correct position
                SDL_SetSurfaceAlphaMod(converted_asset, SDL_ALPHA_OPAQUE);

                SDL_BlitSurface(converted_asset, nullptr, surface, &rect);

                // cleanup
                BloodSword::Free(&converted_asset);
            }

            // cleanup
            BloodSword::Free(&surface_asset);
        }
    }

    void RenderPoints(SDL_Surface *surface, Points &points, SDL_Rect &rect, int offset, Asset::Type asset, bool blur = false)
    {
        for (auto point : points)
        {
            rect.x = point.X * BloodSword::TileSize + offset;

            rect.y = point.Y * BloodSword::TileSize + offset;

            auto surface_sset = blur ? BloodSword::Asset::Surface(asset, 0x7F7F7F7F) : BloodSword::Asset::Surface(asset);

            MapRenderer::RenderAsset(surface, surface_sset, rect);
        }
    }

    void Main(const char *battle_file, const char *image_file)
    {
        // load default module
        Interface::LoadModules();

        // load settings
        Interface::Initialize(Interface::SettingsFile.c_str());

        // load asset location mappings
        Asset::Load(std::string(Interface::Settings["assets"]).c_str());

        // load specific battle
        auto battle = Battle::Load(battle_file);

        auto scene = Scene::Base();

        auto party = Party::Base();

        for (auto &character_class : Character::All)
        {
            auto character = Generate::Character(character_class, 2);

            party.Add(character);
        }

        if (battle.IsDefined())
        {
            Interface::CloneCharacters(battle, party);

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

            Interface::SetEnemyLocations(battle, party);

            SDL_Rect rect;

            auto offset = BloodSword::LargePad;

            rect.w = battle.Map.Width * BloodSword::TileSize + offset * 2;

            rect.h = battle.Map.Height * BloodSword::TileSize + offset * 2;

            auto surface = Graphics::CreateSurface(rect.w, rect.h);

            if (surface)
            {
                // fill entire map
                SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 255, 255, 255, 255));

                for (auto y = 0; y < battle.Map.Height; y++)
                {
                    for (auto x = 0; x < battle.Map.Width; x++)
                    {
                        SDL_Surface *surface_asset = nullptr;

                        auto &tile = battle.Map[Point(x, y)];

                        if (!tile.IsOccupied())
                        {
                            if (tile.Asset == Asset::Type::EMPTY_SPACE)
                            {
                                surface_asset = BloodSword::Asset::Surface(Asset::Type::SELECT);
                            }
                            else if (tile.Asset != Asset::Type::NONE)
                            {
                                surface_asset = BloodSword::Asset::Surface(tile.Asset);
                            }
                            else if (tile.Type == Map::Object::EXIT)
                            {
                                surface_asset = BloodSword::Asset::Surface(Asset::Type::SELECT);
                            }
                        }
                        else
                        {
                            switch (tile.Occupant)
                            {
                            case Map::Object::PLAYER:
                                if (tile.Id >= 0 && tile.Id < party.Count() && party.Count() > 0)
                                {
                                    surface_asset = BloodSword::Asset::Surface(Asset::Type::SELECT);
                                }
                                break;
                            case Map::Object::ENEMY:
                                if (tile.Id >= 0 && tile.Id < battle.Opponents.Count() && battle.Opponents.Count() > 0)
                                {
                                    surface_asset = BloodSword::Asset::Surface(battle.Opponents[tile.Id].Asset);
                                }
                                break;
                            case Map::Object::TEMPORARY_OBSTACLE:
                                if (tile.Lifetime > 0)
                                {
                                    if (tile.TemporaryAsset != Asset::Type::NONE)
                                    {
                                        surface_asset = BloodSword::Asset::Surface(tile.TemporaryAsset);
                                    }
                                }
                                else if (tile.Asset != Asset::Type::NONE)
                                {
                                    surface_asset = BloodSword::Asset::Surface(tile.Asset);
                                }
                                break;
                            default:
                                break;
                            }
                        }

                        if (surface_asset == nullptr)
                        {
                            // fill empty space
                            surface_asset = Graphics::CreateSurface(BloodSword::TileSize, BloodSword::TileSize);

                            SDL_FillRect(surface_asset, nullptr, SDL_MapRGBA(surface->format, 0, 0, 0, 255));
                        }

                        rect.x = x * BloodSword::TileSize + offset;

                        rect.y = y * BloodSword::TileSize + offset;

                        MapRenderer::RenderAsset(surface, surface_asset, rect);
                    }
                }

                // away players
                MapRenderer::RenderPoints(surface, battle.Map.AwayPlayers, rect, offset, Asset::Type::CIRCLE, true);

                // survivors
                MapRenderer::RenderPoints(surface, battle.Map.Survivors, rect, offset, Asset::Type::SELECT, true);

                // away opponents
                MapRenderer::RenderPoints(surface, battle.Map.AwayOpponents, rect, offset, Asset::Type::SELECT, true);

                // player locations
                auto number = 1;

                for (auto &origin : battle.Map.Origins)
                {
                    rect.x = origin.X * BloodSword::TileSize + offset;

                    rect.y = origin.Y * BloodSword::TileSize + offset;

                    auto character_asset = BloodSword::Asset::Surface(Interface::Numbers[number]);

                    MapRenderer::RenderAsset(surface, character_asset, rect);

                    number++;
                }

                IMG_SavePNG(surface, image_file);

                SDL_FreeSurface(surface);
            }
        }
    }
}

#endif
