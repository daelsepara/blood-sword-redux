#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include "Fonts.hpp"
#include "Graphics.hpp"
#include "Map.hpp"
#include "Party.hpp"

namespace BloodSword::Interface
{
    // add map to the scene
    void Add(Map::Base &Map, Scene::Base &Scene, Party::Base &Party, Party::Base &Enemies, int BottomControls)
    {
        auto NumControls = Scene.Controls.size();

        for (auto y = Map.MapY; y < Map.MapY + Map.SizeY; y++)
        {
            auto CtrlY = y - Map.MapY;

            for (auto x = Map.MapX; x < Map.MapX + Map.SizeX; x++)
            {
                auto CtrlX = x - Map.MapX;

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

                auto Location = Point(x, y);

                Map::Tile &Tile = Map[Location];

                auto Screen = Point(Map.DrawX, Map.DrawY) + Point(CtrlX, CtrlY) * Map.TileSize;

                auto ControlType = Controls::Type::MAP_NONE;

                auto ControlColor = Color::Inactive;

                if (Tile.IsOccupied())
                {
                    switch (Tile.Occupant)
                    {
                    case Map::Object::PLAYER:
                        if (Tile.Id >= 0 && Tile.Id < Party.Count())
                        {
                            if (Party[Tile.Id].Value(Attribute::Type::ENDURANCE) > 0)
                            {
                                Scene.Add(Scene::Element(Asset::Get(Party[Tile.Id].Asset), Screen));

                                switch (Party[Tile.Id].Class)
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
                        if (Tile.Id >= 0 && Tile.Id < Enemies.Count())
                        {
                            if (Enemies[Tile.Id].Value(Attribute::Type::ENDURANCE) > 0)
                            {
                                Scene.Add(Scene::Element(Asset::Get(Enemies[Tile.Id].Asset), Screen));

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
                                Scene.Add(Scene::Element(Asset::Get(Tile.TemporaryAsset), Screen));

                                ControlType = Controls::Type::TEMPORARY_OBSTACLE;

                                ControlColor = Color::Highlight;
                            }
                        }
                        else
                        {
                            if (Tile.Asset != Asset::Type::NONE)
                            {
                                Scene.Add(Scene::Element(Asset::Get(Tile.Asset), Screen));
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
                        Scene.Add(Scene::Element(Asset::Get(Tile.Asset), Screen));
                    }
                }

                Scene.Add(Controls::Base(ControlType, NumControls, CtrlLt, CtrlRt, CtrlUp, CtrlDn, Screen, Map.TileSize, Map.TileSize, ControlColor, Location));

                NumControls++;
            }
        }
    }

    // create character attributes text box
    SDL_Texture *Attributes(Graphics::Base &graphics, Character::Base &character, TTF_Font *font, Uint32 labelColor, Uint32 statsColor, int style, int wrap)
    {
        SDL_Texture *texture = NULL;

        std::string labels = "RNK\nFPR\nAWR\nPSY\nEND\nDMG\nARM";

        std::string stats = std::to_string(character.Rank);

        auto labelsw = 0;

        auto statsw = 0;

        // estimate maximum line width
        Graphics::Estimate(font, "DMGG", &labelsw, NULL);

        Graphics::Estimate(font, "99D+D99", &statsw, NULL);

        auto surfaceLabels = Graphics::CreateSurfaceText(labels.c_str(), font, Color::S(labelColor), style, labelsw);

        for (auto &attribute : Attribute::All)
        {
            if (stats.length() > 0)
            {
                stats += '\n';
            }

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
            else if (attribute == Attribute::Type::DAMAGE)
            {
                stats += std::to_string(value) + "D";

                if (modifier != 0)
                {
                    if (modifier > 0)
                    {
                        stats += "+";
                    }

                    stats += std::to_string(modifier);
                }
            }
            else
            {
                stats += std::to_string(value);

                if (modifier != 0)
                {
                    stats += "(";

                    if (modifier > 0)
                    {
                        stats += "+";
                    }

                    stats += std::to_string(modifier);

                    stats += ")";
                }
            }
        }

        auto surfaceStats = Graphics::CreateSurfaceText(stats.c_str(), font, Color::S(statsColor), style, statsw);

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

                auto convertedSurfaceLabels = SDL_ConvertSurface(surfaceLabels, surface->format, 0);

                if (convertedSurfaceLabels)
                {
                    SDL_SetSurfaceAlphaMod(convertedSurfaceLabels, SDL_ALPHA_OPAQUE);
                    SDL_BlitSurface(convertedSurfaceLabels, NULL, surface, &labelsRect);
                    BloodSword::Free(&convertedSurfaceLabels);
                }

                statsRect.w = surface->w;
                statsRect.h = surface->h;
                statsRect.x = surfaceLabels->w + 8;
                statsRect.y = 0;

                auto convertedSurfaceStats = SDL_ConvertSurface(surfaceStats, surface->format, 0);

                if (convertedSurfaceStats)
                {
                    SDL_SetSurfaceAlphaMod(convertedSurfaceStats, SDL_ALPHA_OPAQUE);
                    SDL_BlitSurface(convertedSurfaceStats, NULL, surface, &statsRect);
                    BloodSword::Free(&convertedSurfaceStats);
                }

                texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

                BloodSword::Free(&surface);
            }

            BloodSword::Free(&surfaceStats);
            BloodSword::Free(&surfaceLabels);
        }

        return texture;
    }

    // clip rendering outside of map area
    void Clip(Graphics::Base &graphics, Map::Base &map)
    {
        if (graphics.Renderer)
        {
            SDL_Rect view;

            view.w = map.Width * map.TileSize;

            view.h = map.Height * map.TileSize;

            view.x = map.DrawX;

            view.y = map.DrawY;

            SDL_RenderSetClipRect(graphics.Renderer, &view);
        }
    }

    // reset clipping area
    void Clip(Graphics::Base &graphics)
    {
        if (graphics.Renderer)
        {
            SDL_RenderSetClipRect(graphics.Renderer, NULL);
        }
    }

    // add menu to the scene
    void Add(std::vector<SDL_Texture *> &choices, Scene::Base &scene, int x, int y, int w, int h, int start, int last, int limit, Uint32 border, Uint32 highlight, bool others = false)
    {
        if (!choices.empty())
        {
            auto id_start = (int)scene.Controls.size();
            auto end = last - start;
            auto options = (int)choices.size();
            auto more = options - last > 0;
            auto scroll = id_start + (options < limit ? options : limit);
            auto pixels = 2;
            auto pad = pixels * 6;
            auto offset = pixels * 2;
            auto adjust = pixels * 4;

            for (auto i = 0; i < end; i++)
            {
                auto index = start + i;
                auto id = id_start + i;
                auto rt = (options > limit && (start > 0 || more)) ? (i == end - 1 && options > limit && more ? (start > 0 ? scroll + 1 : scroll) : scroll) : id;
                auto up = (i > 0 ? id - 1 : id);
                auto dn = (i < end - 1 || options > limit || others) ? id + 1 : id;

                scene.Add(Scene::Element(choices[index], x, y + i * (h + pad)));
                scene.Add(Scene::Element(x, y + i * (h + pad), w, h, 0, border, pixels));
                scene.Add(Controls::Base(Controls::Type::CHOICE, id, id, rt, up, dn, x - offset, y + i * (h + pad) - offset, w + adjust, h + adjust, highlight));
            }

            if (options > limit)
            {
                if (start > 0)
                {
                    scene.Add(Scene::Element(Asset::Get(Asset::Type::UP), x + w + pad, y));
                    scene.Add(Controls::Base(
                        Controls::Type::SCROLL_UP,
                        scroll, id_start, scroll, scroll, (others || more ? scroll + 1 : scroll),
                        x + w + pad - offset, y - offset,
                        64 + adjust, 64 + adjust,
                        highlight));
                }

                if (more)
                {
                    scene.Add(Scene::Element(Asset::Get(Asset::Type::DOWN), x + w + pad, y + (limit - 1) * (h + pad) + (h - 64)));
                    scene.Add(Controls::Base(
                        Controls::Type::SCROLL_DOWN,
                        start > 0 ? scroll + 1 : scroll,
                        end - 1, start > 0 ? scroll + 1 : scroll,
                        (start > 0 ? scroll : id_start),
                        (others ? (start > 0 ? scroll + 2 : scroll + 1) : (start > 0 ? scroll + 1 : scroll)),
                        x + w + pad - offset, y + (limit - 1) * (h + pad) - offset + (h - 64),
                        64 + adjust, 64 + adjust,
                        highlight));
                }
            }
        }
    }
}

#endif
