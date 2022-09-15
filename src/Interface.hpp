#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include "Fonts.hpp"
#include "Graphics.hpp"
#include "Map.hpp"
#include "Party.hpp"

namespace BloodSword::Interface
{
    void Add(Map::Base &Map, Scene::Base &Scene, Party::Base &Party, Party::Base &Enemies, int BottomControls)
    {
        auto NumControls = Scene.Controls.size();

        for (auto y = Map.MapY; y < Map.MapY + Map.SizeY; y++)
        {
            auto CtrlY = (y - Map.MapY);

            auto AssetY = Map.DrawY + CtrlY * Map.TileSize;

            for (auto x = Map.MapX; x < Map.MapX + Map.SizeX; x++)
            {
                auto CtrlX = (x - Map.MapX);

                auto CtrlUp = NumControls;
                auto CtrlDn = NumControls;
                auto CtrlLt = NumControls;
                auto CtrlRt = NumControls;

                if (CtrlY > 0)
                {
                    CtrlUp = NumControls - Map.SizeX;
                }

                if (CtrlY < Map.SizeY - 1)
                {
                    CtrlDn = NumControls + Map.SizeX;
                }
                else
                {
                    if (CtrlX < BottomControls)
                    {
                        CtrlDn = NumControls + Map.SizeX;
                    }
                }

                if (CtrlX > 0)
                {
                    CtrlLt = NumControls - 1;
                }

                if (CtrlX < Map.SizeX - 1)
                {
                    CtrlRt = NumControls + 1;
                }

                Map::Tile &Tile = Map.Tiles[y][x];

                auto AssetX = Map.DrawX + (x - Map.MapX) * Map.TileSize;

                auto ControlType = Controls::Type::MAP_NONE;

                auto ControlColor = Color::Inactive;

                if (Tile.IsOccupied())
                {
                    switch (Tile.Occupant)
                    {
                    case Map::Object::PLAYER:
                        if (Tile.Id >= 0 && Tile.Id < Party.Members.size())
                        {
                            if (Party.Members[Tile.Id].Value(Attribute::Type::ENDURANCE) > 0)
                            {
                                Scene.Add(Scene::Element(Asset::Get(Party.Members[Tile.Id].Asset), AssetX, AssetY));

                                switch (Party.Members[Tile.Id].Class)
                                {
                                case Character::Class::WARRIOR:
                                    ControlType = Controls::Type::WARRIOR;
                                    break;
                                case Character::Class::TRICKSTER:
                                    ControlType = Controls::Type::TRICKSTER;
                                    break;
                                case Character::Class::SAGE:
                                    ControlType = Controls::Type::SAGE;
                                    break;
                                case Character::Class::ENCHANTER:
                                    ControlType = Controls::Type::ENCHANTER;
                                    break;
                                default:
                                    break;
                                }

                                ControlColor = Color::Highlight;
                            }
                        }
                        break;
                    case Map::Object::ENEMY:
                        if (Tile.Id >= 0 && Tile.Id < Enemies.Members.size())
                        {
                            if (Enemies.Members[Tile.Id].Value(Attribute::Type::ENDURANCE) > 0)
                            {
                                Scene.Add(Scene::Element(Asset::Get(Enemies.Members[Tile.Id].Asset), AssetX, AssetY));

                                ControlType = Controls::Type::ENEMY;

                                ControlColor = Color::Highlight;
                            }
                        }
                        break;
                    case Map::Object::TEMPORARY_OBSTACLE:
                        if (Tile.Lifetime > 0)
                        {
                            if (Tile.TemporaryAsset != Asset::Type::NONE)
                            {
                                Scene.Add(Scene::Element(Asset::Get(Tile.TemporaryAsset), AssetX, AssetY));

                                ControlType = Controls::Type::TEMPORARY_OBSTACLE;

                                ControlColor = Color::Highlight;
                            }
                        }
                        else
                        {
                            if (Tile.Asset != Asset::Type::NONE)
                            {
                                Scene.Add(Scene::Element(Asset::Get(Tile.Asset), AssetX, AssetY));
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    if (Tile.IsExit())
                    {
                        ControlType = Controls::Type::MAP_EXIT;

                        ControlColor = Color::Highlight;
                    }
                    else if (Tile.IsPassable())
                    {
                        ControlType = Controls::Type::DESTINATION;

                        ControlColor = Color::Highlight;
                    }
                    else if (Tile.IsPassableToEnemy())
                    {
                        ControlColor = Color::Inactive;
                    }

                    if (Tile.Asset != Asset::Type::NONE)
                    {
                        Scene.Add(Scene::Element(Asset::Get(Tile.Asset), AssetX, AssetY));
                    }
                }

                Scene.Add(Controls::Base(ControlType, NumControls, CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, Map.TileSize, Map.TileSize, ControlColor, x, y));

                NumControls++;
            }
        }
    }

    SDL_Texture *Attributes(Graphics::Base &graphics, Character::Base &character, TTF_Font *font, Uint32 labelColor, Uint32 statsColor, int style, int wrap)
    {
        SDL_Texture *texture = NULL;

        std::string labels = "FPR\nAWR\nPSY\nEND\nDMG\nARM";

        std::string stats = "";

        auto labelsw = 0;

        TTF_SizeUTF8(font, "ABCDE", &labelsw, NULL);

        auto surfaceLabels = Graphics::CreateSurfaceText(labels.c_str(), font, Color::S(labelColor), style, labelsw);

        for (auto i = 0; i < 6; i++)
        {
            auto attribute = static_cast<Attribute::Type>(i);

            auto value = character.Value(attribute);

            auto modifier = character.Modifier(attribute) + character.Modifiers(attribute);

            if (attribute == Attribute::Type::ENDURANCE)
            {
                stats += std::to_string(value + modifier);
            }
            else if (attribute == Attribute::Type::ARMOUR)
            {
                stats += std::to_string(modifier);
            }
            else
            {
                stats += std::to_string(value) + "D";

                if (modifier >= 0)
                {
                    stats += "+";
                }

                stats += std::to_string(modifier);
            }

            if (i < 5)
            {
                stats += "\n";
            }
        }

        auto surfaceStats = Graphics::CreateSurfaceText(stats.c_str(), font, Color::S(statsColor), style, labelsw * 3);

        if (surfaceLabels && surfaceStats)
        {
            SDL_Surface *surface = NULL;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            surface = SDL_CreateRGBSurface(0, (surfaceLabels->w + surfaceStats->w), std::max(surfaceLabels->h, surfaceStats->h), 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
#else
            surface = SDL_CreateRGBSurface(0, (surfaceLabels->w + surfaceStats->w), std::max(surfaceLabels->h, surfaceStats->h), 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
#endif

            if (surface)
            {
                SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

                SDL_Rect labelsRect, statsRect;

                labelsRect.w = surface->w;
                labelsRect.h = surface->h;
                labelsRect.x = 0;
                labelsRect.y = 0;

                statsRect.w = surface->w;
                statsRect.h = surface->h;
                statsRect.x = surfaceLabels->w + 8;
                statsRect.y = 0;

                auto convertedSurfaceLabels = SDL_ConvertSurface(surfaceLabels, surface->format, 0);
                auto convertedSurfaceStats = SDL_ConvertSurface(surfaceStats, surface->format, 0);

                SDL_SetSurfaceAlphaMod(convertedSurfaceLabels, SDL_ALPHA_OPAQUE);
                SDL_SetSurfaceAlphaMod(convertedSurfaceStats, SDL_ALPHA_OPAQUE);

                if (convertedSurfaceLabels && convertedSurfaceStats)
                {
                    SDL_BlitSurface(convertedSurfaceLabels, NULL, surface, &labelsRect);
                    SDL_BlitSurface(convertedSurfaceStats, NULL, surface, &statsRect);

                    BloodSword::Free(&convertedSurfaceLabels);
                    BloodSword::Free(&convertedSurfaceStats);

                    texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);
                }

                BloodSword::Free(&surface);
            }

            BloodSword::Free(&surfaceStats);
            BloodSword::Free(&surfaceLabels);
        }

        return texture;
    }
}

#endif
