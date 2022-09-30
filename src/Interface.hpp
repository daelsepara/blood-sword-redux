#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include "Fonts.hpp"
#include "Graphics.hpp"
#include "Map.hpp"
#include "Party.hpp"

namespace BloodSword::Interface
{
    Skills::Mapped<SDL_Texture *> SkillsTexturesInActive = {};
    Skills::Mapped<SDL_Texture *> SkillCaptionsActive = {};
    Skills::Mapped<SDL_Texture *> SkillCaptionsInActive = {};
    Skills::Mapped<Controls::Type> SkillControls = {
        {Skills::Type::ARCHERY, Controls::Type::ARCHERY},
        {Skills::Type::DODGING, Controls::Type::DODGING},
        {Skills::Type::QUARTERSTAFF, Controls::Type::QUARTERSTAFF},
        {Skills::Type::HEALING, Controls::Type::HEAL},
        {Skills::Type::ESP, Controls::Type::ESP},
        {Skills::Type::PARANORMAL_SIGHT, Controls::Type::PARANORMAL_SIGHT},
        {Skills::Type::LEVITATION, Controls::Type::LEVITATION},
        {Skills::Type::EXORCISM, Controls::Type::EXORCISM},
        {Skills::Type::SPELLS, Controls::Type::SPELLS}};

    SDL_Texture *NoSkills = NULL;

    void Initialize(Graphics::Base &graphics)
    {
        for (auto &skill : Skills::TypeMapping)
        {
            auto estimate = 0;

            Graphics::Estimate(Fonts::Caption, skill.second, &estimate, NULL);

            auto active = Graphics::CreateText(graphics, skill.second, Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, estimate);
            auto inactive = Graphics::CreateText(graphics, skill.second, Fonts::Caption, Color::S(Color::Inactive), TTF_STYLE_NORMAL, estimate);

            SkillCaptionsActive[skill.first] = active;
            SkillCaptionsInActive[skill.first] = inactive;
            SkillsTexturesInActive[skill.first] = Asset::Copy(graphics.Renderer, Skills::SkillAssets[skill.first], 0x60);
        }

        auto noskills = 0;

        Graphics::Estimate(Fonts::Caption, "No special skills", &noskills, NULL);

        NoSkills = Graphics::CreateText(graphics, "No special skills", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, noskills);
    }

    void Shutdown()
    {
        for (auto &skill : SkillCaptionsActive)
        {
            Free(&skill.second);
        }

        for (auto &skill : SkillCaptionsInActive)
        {
            Free(&skill.second);
        }

        for (auto &skill : SkillsTexturesInActive)
        {
            Free(&skill.second);
        }

        Free(&NoSkills);
    }

    // add map to the scene
    void Add(Map::Base &Map, Scene::Base &Scene, Party::Base &Party, Party::Base &Enemies, int BottomControls)
    {
        auto NumControls = Scene.Controls.size();

        for (auto y = Map.Y; y < Map.Y + Map.SizeY; y++)
        {
            auto CtrlY = y - Map.Y;

            for (auto x = Map.X; x < Map.X + Map.SizeX; x++)
            {
                auto CtrlX = x - Map.X;

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

    void Add(Map::Base &Map, Scene::Base &Scene, Party::Base &Party, int BottomControls)
    {
        auto Enemies = Party::Base();

        Interface::Add(Map, Scene, Party, Enemies, BottomControls);
    }

    // create character attributes text box
    SDL_Texture *Attributes(Graphics::Base &graphics, Character::Base &character, TTF_Font *font, Uint32 labelColor, Uint32 statsColor, int style, int wrap, bool addName = false)
    {
        SDL_Texture *texture = NULL;

        std::string labels;
        std::string stats;

        if (character.ControlType == Character::ControlType::PLAYER)
        {
            if (addName)
            {
                labels = '\n';
            }

            labels += "RNK\nFPR\nAWR\nPSY\nEND\nDMG\nARM";

            stats += std::to_string(character.Rank);
        }
        else if (character.ControlType == Character::ControlType::NPC)
        {
            if (addName)
            {
                labels = '\n';
            }

            labels += "FPR\nAWR\nPSY\nEND\nDMG\nARM";
        }

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

        if (addName)
        {
            stats = '\n' + stats;
        }

        auto surfaceStats = Graphics::CreateSurfaceText(stats.c_str(), font, Color::S(statsColor), style, statsw);

        if (surfaceLabels && surfaceStats)
        {
            SDL_Surface *surface = NULL;

            auto surfaceWidth = surfaceLabels->w + surfaceStats->w;

            if (addName)
            {
                auto namewidth = 0;

                Graphics::Estimate(font, "DMGG", &namewidth, NULL);

                if (namewidth > surfaceWidth)
                {
                    surfaceWidth = namewidth;
                }
            }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            surface = SDL_CreateRGBSurface(0, surfaceWidth, std::max(surfaceLabels->h, surfaceStats->h), 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
#else
            surface = SDL_CreateRGBSurface(0, surfaceWidth, std::max(surfaceLabels->h, surfaceStats->h), 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
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

                // add character class if player character
                if (addName)
                {
                    auto surfaceName = Graphics::CreateSurfaceText(character.Name.c_str(), font, Color::S(labelColor), style | TTF_STYLE_UNDERLINE, labelsw + statsw + 8);

                    if (surfaceName)
                    {
                        labelsRect.w = surface->w;
                        labelsRect.h = surface->h;
                        labelsRect.x = 0;
                        labelsRect.y = 0;

                        auto convertedSurfaceName = SDL_ConvertSurface(surfaceName, surface->format, 0);

                        if (convertedSurfaceName)
                        {
                            SDL_SetSurfaceAlphaMod(convertedSurfaceName, SDL_ALPHA_OPAQUE);
                            SDL_BlitSurface(convertedSurfaceName, NULL, surface, &labelsRect);
                            BloodSword::Free(&convertedSurfaceName);
                        }

                        BloodSword::Free(&surfaceName);
                    }
                }

                texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

                BloodSword::Free(&surface);
            }

            BloodSword::Free(&surfaceStats);
            BloodSword::Free(&surfaceLabels);
        }

        return texture;
    }

    std::vector<SDL_Texture *> Attributes(Graphics::Base &graphics, Party::Base &party, TTF_Font *font, Uint32 labelColor, Uint32 statsColor, int style, int wrap, bool addClass = false)
    {
        std::vector<SDL_Texture *> textures = {};

        for (auto i = 0; i < party.Count(); i++)
        {
            auto &character = party[i];

            auto texture = Interface::Attributes(graphics, character, font, labelColor, statsColor, style, wrap, addClass);

            if (texture)
            {
                textures.push_back(texture);
            }
        }

        return textures;
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

    // add vertical text menu to the scene
    void Add(std::vector<SDL_Texture *> &choices, Scene::Base &scene, int x, int y, int w, int h, int start, int last, int limit, Uint32 border, Uint32 highlight, bool others = false)
    {
        if (!choices.empty())
        {
            auto startid = (int)(scene.Controls.size());
            auto end = last - start;
            auto options = (int)(choices.size());
            auto more = options - last > 0;
            auto scroll = startid + (options < limit ? options : limit);
            auto pixels = 2;
            auto offset = pixels * 2;
            auto adjust = pixels * 4;
            auto pad = pixels * 6;
            auto bars = options > limit;
            auto scrollup = bars && start > 0;
            auto scrolldn = bars && more;
            auto dim = 64;
            auto dimadjust = dim + adjust;
            auto yadjust = (limit - 1) * (h + pad) + (h - dim);
            auto wadjust = w + adjust;
            auto xoffset = x + wadjust;
            auto yoffset = y - offset;
            auto xpad = x + w + pad;

            for (auto item = 0; item < end; item++)
            {
                auto id = startid + item;
                auto rt = scrollup || scrolldn ? (item == end - 1 && scrolldn ? (scrollup ? scroll + 1 : scroll) : scroll) : id;
                auto up = (item > 0 ? id - 1 : id);
                auto dn = item < end - 1 ? id + 1 : (others ? (scrollup || scrolldn ? (scrollup && scrolldn ? scroll + 2 : scroll + 1) : id + 1) : (scrollup && scrolldn ? scroll + 2 : (scrolldn ? scroll : id)));
                auto itemy = y + item * (h + pad);

                scene.Add(Scene::Element(choices[start + item], x, itemy));
                scene.Add(Scene::Element(x, itemy, w, h, 0, border, pixels));
                scene.Add(Controls::Base(Controls::Type::CHOICE, id, id, rt, up, dn, x - offset, itemy - offset, wadjust, h + adjust, highlight));
            }

            if (options > limit)
            {
                if (scrollup)
                {
                    scene.Add(Scene::Element(Asset::Get(Asset::Type::UP), xpad, y));
                    scene.Add(Controls::Base(
                        Controls::Type::SCROLL_UP,
                        scroll, startid, scroll, scroll, (more ? scroll + 1 : startid),
                        xoffset, yoffset,
                        dimadjust, dimadjust,
                        highlight));
                }

                if (scrolldn)
                {
                    scene.Add(Scene::Element(Asset::Get(Asset::Type::DOWN), xpad, y + yadjust));
                    scene.Add(Controls::Base(
                        Controls::Type::SCROLL_DOWN,
                        start > 0 ? scroll + 1 : scroll,
                        end - 1, start > 0 ? scroll + 1 : scroll,
                        (start > 0 ? scroll : startid),
                        (others ? (scrollup ? scroll + 2 : scroll + 1) : (scrollup ? scroll + 1 : scroll)),
                        xoffset, yoffset + yadjust,
                        dimadjust, dimadjust,
                        highlight));
                }
            }
        }
    }

    // skills overlay menu
    Scene::Base Skills(Point origin, int w, int h, Character::Base &character, Uint32 background, Uint32 border, int borderSize, bool inbattle = false)
    {
        auto overlay = Scene::Base();

        auto popupw = (std::max((int)(character.Skills.size()), 2) + 2) * 64;

        auto popuph = 160;

        auto screen = origin + Point(w - popupw, h - popuph) / 2;

        overlay.Add(Scene::Element(screen, popupw, popuph, background, border, borderSize));

        auto pad = 8;

        if (character.Skills.size() > 0)
        {
            for (auto i = 0; i < character.Skills.size(); i++)
            {
                SDL_Texture *texture = NULL;

                if (inbattle)
                {
                    if (Skills::IsBattleSkill(character.Skills[i]))
                    {
                        texture = Asset::Get(Skills::SkillAssets[character.Skills[i]]);
                    }
                    else
                    {
                        texture = SkillsTexturesInActive[character.Skills[i]];
                    }
                }
                else
                {
                    if (Skills::IsStorySkill(character.Skills[i]))
                    {
                        texture = Asset::Get(Skills::SkillAssets[character.Skills[i]]);
                    }
                    else
                    {
                        texture = SkillsTexturesInActive[character.Skills[i]];
                    }
                }

                if (texture)
                {
                    auto texturew = 0;
                    auto textureh = 0;

                    SDL_QueryTexture(texture, NULL, NULL, &texturew, &textureh);

                    auto lt = i > 0 ? i - 1 : i;
                    auto rt = i < character.Skills.size() ? i + 1 : i;

                    overlay.Add(Controls::Base(Interface::SkillControls[character.Skills[0]], i, lt, rt, i, i, screen.X + i * texturew + pad * 2, screen.Y + pad * 2 + 32, 64, 64, Color::Highlight));
                    overlay.Add(Scene::Element(texture, screen.X + i * texturew + pad * 2, screen.Y + pad * 2 + 32));
                }
            }
        }
        else
        {
            overlay.Add(Scene::Element(NoSkills, screen + Point(pad * 2, pad)));
        }

        auto id = (int)(character.Skills.size());

        overlay.Add(Scene::Element(Asset::Get(Asset::Type::BACK), screen.X + character.Skills.size() * 64 + pad * 2, screen.Y + pad * 2 + 32));
        overlay.Add(Controls::Base(Controls::Type::BACK, id, id - 1, id, id, id, screen.X + id * 64 + pad * 2, screen.Y + pad * 2 + 32, 64, 64, Color::Highlight));

        return overlay;
    }
}

#endif
