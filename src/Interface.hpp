#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include "Animation.hpp"
#include "Battle.hpp"
#include "Engine.hpp"
#include "Graphics.hpp"
#include "Input.hpp"
#include "Maze.hpp"
#include "Palette.hpp"

namespace BloodSword::Interface
{
    Skills::Mapped<SDL_Texture *> SkillsTexturesInactive = {};
    Skills::Mapped<SDL_Texture *> SkillCaptionsActive = {};
    Skills::Mapped<SDL_Texture *> SkillCaptionsInactive = {};
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

    Spells::Mapped<SDL_Texture *> SpellsTexturesInactive = {};
    Spells::Mapped<SDL_Texture *> SpellCaptionsActive = {};
    Spells::Mapped<SDL_Texture *> SpellCaptionsInactive = {};
    Spells::Mapped<Controls::Type> SpellControls = {
        {Spells::Type::VOLCANO_SPRAY, Controls::Type::VOLCANO_SPRAY},
        {Spells::Type::NIGHTHOWL, Controls::Type::NIGHTHOWL},
        {Spells::Type::WHITE_FIRE, Controls::Type::WHITE_FIRE},
        {Spells::Type::SWORDTHRUST, Controls::Type::SWORDTHRUST},
        {Spells::Type::EYE_OF_THE_TIGER, Controls::Type::EYE_OF_THE_TIGER},
        {Spells::Type::IMMEDIATE_DELIVERANCE, Controls::Type::IMMEDIATE_DELIVERANCE},
        {Spells::Type::MISTS_OF_DEATH, Controls::Type::MISTS_OF_DEATH},
        {Spells::Type::THE_VAMPIRE_SPELL, Controls::Type::THE_VAMPIRE_SPELL},
        {Spells::Type::PILLAR_OF_SALT, Controls::Type::PILLAR_OF_SALT},
        {Spells::Type::SHEET_LIGHTNING, Controls::Type::SHEET_LIGHTNING},
        {Spells::Type::GHASTLY_TOUCH, Controls::Type::GHASTLY_TOUCH},
        {Spells::Type::NEMESIS_BOLT, Controls::Type::NEMESIS_BOLT},
        {Spells::Type::SERVILE_ENTHRALMENT, Controls::Type::SERVILE_ENTHRALMENT},
        {Spells::Type::SUMMON_FALTYN, Controls::Type::SUMMON_FALTYN},
        {Spells::Type::PREDICTION, Controls::Type::PREDICTION},
        {Spells::Type::DETECT_ENCHANTMENT, Controls::Type::DETECT_ENCHANTMENT}};
    Controls::Mapped<Spells::Type> ControlSpellMapping = {
        {Controls::Type::VOLCANO_SPRAY, Spells::Type::VOLCANO_SPRAY},
        {Controls::Type::NIGHTHOWL, Spells::Type::NIGHTHOWL},
        {Controls::Type::WHITE_FIRE, Spells::Type::WHITE_FIRE},
        {Controls::Type::SWORDTHRUST, Spells::Type::SWORDTHRUST},
        {Controls::Type::EYE_OF_THE_TIGER, Spells::Type::EYE_OF_THE_TIGER},
        {Controls::Type::IMMEDIATE_DELIVERANCE, Spells::Type::IMMEDIATE_DELIVERANCE},
        {Controls::Type::MISTS_OF_DEATH, Spells::Type::MISTS_OF_DEATH},
        {Controls::Type::THE_VAMPIRE_SPELL, Spells::Type::THE_VAMPIRE_SPELL},
        {Controls::Type::PILLAR_OF_SALT, Spells::Type::PILLAR_OF_SALT},
        {Controls::Type::SHEET_LIGHTNING, Spells::Type::SHEET_LIGHTNING},
        {Controls::Type::GHASTLY_TOUCH, Spells::Type::GHASTLY_TOUCH},
        {Controls::Type::NEMESIS_BOLT, Spells::Type::NEMESIS_BOLT},
        {Controls::Type::SERVILE_ENTHRALMENT, Spells::Type::SERVILE_ENTHRALMENT},
        {Controls::Type::SUMMON_FALTYN, Spells::Type::SUMMON_FALTYN},
        {Controls::Type::PREDICTION, Spells::Type::PREDICTION},
        {Controls::Type::DETECT_ENCHANTMENT, Spells::Type::DETECT_ENCHANTMENT}};

    Character::Mapped<Controls::Type> CharacterControls = {
        {Character::Class::WARRIOR, Controls::Type::WARRIOR},
        {Character::Class::TRICKSTER, Controls::Type::TRICKSTER},
        {Character::Class::SAGE, Controls::Type::SAGE},
        {Character::Class::ENCHANTER, Controls::Type::ENCHANTER}};

    SDL_Texture *NoSkills = NULL;
    SDL_Texture *NoSpells = NULL;

    std::vector<Asset::Type> DICE = {
        Asset::Type::DICE1,
        Asset::Type::DICE2,
        Asset::Type::DICE3,
        Asset::Type::DICE4,
        Asset::Type::DICE5,
        Asset::Type::DICE6};

    // create textures
    void
    InitializeTextures(Graphics::Base &graphics)
    {
        auto estimate = 0;

        for (auto &skill : Skills::TypeMapping)
        {
            Graphics::Estimate(Fonts::Caption, skill.second, &estimate, NULL);

            auto active = Graphics::CreateText(graphics, skill.second, Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, estimate);
            auto inactive = Graphics::CreateText(graphics, skill.second, Fonts::Caption, Color::S(Color::Inactive), TTF_STYLE_NORMAL, estimate);

            SkillCaptionsActive[skill.first] = active;
            SkillCaptionsInactive[skill.first] = inactive;
            SkillsTexturesInactive[skill.first] = Asset::Copy(graphics.Renderer, Skills::Assets[skill.first], Color::Inactive);
        }

        for (auto &spell : Spells::TypeMapping)
        {
            Graphics::Estimate(Fonts::Caption, spell.second, &estimate, NULL);

            auto active = Graphics::CreateText(graphics, spell.second, Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, estimate);
            auto inactive = Graphics::CreateText(graphics, spell.second, Fonts::Caption, Color::S(Color::Inactive), TTF_STYLE_NORMAL, estimate);

            SpellCaptionsActive[spell.first] = active;
            SpellCaptionsInactive[spell.first] = inactive;
            SpellsTexturesInactive[spell.first] = Asset::Copy(graphics.Renderer, Spells::Assets[spell.first], Color::Inactive);
        }

        Graphics::Estimate(Fonts::Caption, "No special skills", &estimate, NULL);
        NoSkills = Graphics::CreateText(graphics, "No special skills", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, estimate);

        Graphics::Estimate(Fonts::Caption, "No spells", &estimate, NULL);
        NoSpells = Graphics::CreateText(graphics, "No spells", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, estimate);
    }

    // unload all textures allocated by this module
    void FreeTextures()
    {
        for (auto &skill : SkillCaptionsActive)
        {
            Free(&skill.second);
        }

        for (auto &skill : SkillCaptionsInactive)
        {
            Free(&skill.second);
        }

        for (auto &skill : SkillsTexturesInactive)
        {
            Free(&skill.second);
        }

        SkillCaptionsActive.clear();
        SkillCaptionsInactive.clear();
        SkillsTexturesInactive.clear();

        Free(&NoSkills);
        Free(&NoSpells);
    }

    // unload all textures and assets
    void UnloadTextures()
    {
        Interface::FreeTextures();

        Graphics::FreeTextures();

        Asset::Unload();
    }

    // load all textures
    void LoadTextures(Graphics::Base &graphics)
    {
        Asset::Load(graphics.Renderer, "settings/assets.json");

        Graphics::InitializeTextures(graphics);

        Interface::InitializeTextures(graphics);
    }

    // switch texture and reload all textures
    void ReloadTextures(Graphics::Base &graphics, int palette = 0, bool override = true)
    {
        Interface::UnloadTextures();

        Palette::Switch(palette, override);

        Interface::LoadTextures(graphics);
    }

    // add map to the scene
    Scene::Base Map(Map::Base &map, Party::Base &party, Party::Base &enemies, int bottom)
    {
        auto scene = Scene::Base();

        auto numcontrols = 0;

        for (auto y = map.Y; y < map.Y + map.SizeY; y++)
        {
            auto ctrly = y - map.Y;

            for (auto x = map.X; x < map.X + map.SizeX; x++)
            {
                auto ctrlx = x - map.X;

                auto up = numcontrols;
                auto dn = numcontrols;
                auto lt = numcontrols;
                auto rt = numcontrols;

                if (ctrly > 0)
                {
                    up = numcontrols - map.SizeX;
                }

                if (ctrly < map.SizeY - 1)
                {
                    dn = numcontrols + map.SizeX;
                }
                else
                {
                    if (ctrlx < bottom)
                    {
                        dn = numcontrols + map.SizeX;
                    }
                }

                if (ctrlx > 0)
                {
                    lt = numcontrols - 1;
                }

                if (ctrlx < map.SizeX - 1)
                {
                    rt = numcontrols + 1;
                }

                auto location = Point(x, y);

                auto &tile = map[location];

                auto screen = Point(map.DrawX, map.DrawY) + Point(ctrlx, ctrly) * map.TileSize;

                auto type = Controls::Type::MAP_NONE;

                auto color = Color::Inactive;

                if (tile.IsOccupied())
                {
                    switch (tile.Occupant)
                    {
                    case Map::Object::PLAYER:
                        if (tile.Id >= 0 && tile.Id < party.Count())
                        {
                            if (Engine::IsAlive(party[tile.Id]))
                            {
                                scene.Add(Scene::Element(Asset::Get(party[tile.Id].Asset), screen));

                                switch (party[tile.Id].Class)
                                {
                                case Character::Class::WARRIOR:
                                    type = Controls::Type::WARRIOR;
                                    break;
                                case Character::Class::TRICKSTER:
                                    type = Controls::Type::TRICKSTER;
                                    break;
                                case Character::Class::SAGE:
                                    type = Controls::Type::SAGE;
                                    break;
                                case Character::Class::ENCHANTER:
                                    type = Controls::Type::ENCHANTER;
                                    break;
                                default:
                                    break;
                                }

                                color = Color::Highlight;
                            }
                        }
                        break;
                    case Map::Object::ENEMY:
                        if (tile.Id >= 0 && tile.Id < enemies.Count())
                        {
                            if (Engine::IsAlive(enemies[tile.Id]))
                            {
                                scene.Add(Scene::Element(Asset::Get(enemies[tile.Id].Asset), screen));

                                type = Controls::Type::ENEMY;

                                color = Color::Highlight;
                            }
                        }
                        break;
                    case Map::Object::TEMPORARY_OBSTACLE:
                        if (tile.Lifetime > 0)
                        {
                            if (tile.TemporaryAsset != Asset::Type::NONE)
                            {
                                scene.Add(Scene::Element(Asset::Get(tile.TemporaryAsset), screen));

                                type = Controls::Type::TEMPORARY_OBSTACLE;

                                color = Color::Highlight;
                            }
                        }
                        else
                        {
                            if (tile.Asset != Asset::Type::NONE)
                            {
                                scene.Add(Scene::Element(Asset::Get(tile.Asset), screen));
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    if (tile.IsExit())
                    {
                        type = Controls::Type::MAP_EXIT;

                        color = Color::Highlight;
                    }
                    else if (tile.IsPassable())
                    {
                        type = Controls::Type::DESTINATION;

                        color = Color::Highlight;
                    }
                    else if (tile.IsPassableToEnemy())
                    {
                        color = Color::Inactive;
                    }

                    if (tile.Asset != Asset::Type::NONE)
                    {
                        scene.Add(Scene::Element(Asset::Get(tile.Asset), screen));
                    }
                }

                scene.Add(Controls::Base(type, numcontrols, lt, rt, up, dn, screen, map.TileSize, map.TileSize, color, location));

                numcontrols++;
            }
        }

        return scene;
    }

    Scene::Base Map(Map::Base &map, Party::Base &party, int bottom)
    {
        auto enemies = Party::Base();

        return Interface::Map(map, party, enemies, bottom);
    }

    std::string ScoreString(Character::Base &character, Attribute::Type attribute, bool inbattle = false)
    {
        std::string stats;

        auto value = std::min(character.Value(attribute), character.Maximum(attribute));

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
            if (inbattle && character.IsPlayer() && !character.IsArmed())
            {
                modifier -= 2;
            }

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
            if (inbattle && character.IsPlayer() && !character.IsArmed() && attribute == Attribute::Type::FIGHTING_PROWESS)
            {
                modifier -= 2;
            }

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

        return stats;
    }

    // create character attributes text box
    SDL_Texture *Attributes(Graphics::Base &graphics, Character::Base &character, TTF_Font *font, Uint32 labelcolor, Uint32 statscolor, int style, int wrap, bool addname = false, bool inbattle = false)
    {
        SDL_Texture *texture = NULL;

        std::string labels, stats;

        if (character.ControlType == Character::ControlType::PLAYER)
        {
            if (addname)
            {
                labels = '\n';
            }

            labels += "RNK\nFPR\nAWR\nPSY\nEND\nDMG\nARM";

            stats += std::to_string(character.Rank);
        }
        else if (character.ControlType == Character::ControlType::NPC)
        {
            if (addname)
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

        auto surfacelabels = Graphics::CreateSurfaceText(labels.c_str(), font, Color::S(labelcolor), style, labelsw);

        for (auto &attribute : Attribute::All)
        {
            if (stats.length() > 0)
            {
                stats += '\n';
            }

            stats += Interface::ScoreString(character, attribute, inbattle);
        }

        if (addname)
        {
            stats = '\n' + stats;
        }

        auto surfacestats = Graphics::CreateSurfaceText(stats.c_str(), font, Color::S(statscolor), style, statsw);

        if (surfacelabels && surfacestats)
        {
            SDL_Surface *surface = NULL;

            auto surfacewidth = surfacelabels->w + surfacestats->w;

            if (addname)
            {
                auto namewidth = 0;

                Graphics::Estimate(font, character.Name.c_str(), &namewidth, NULL);

                if (namewidth > surfacewidth)
                {
                    surfacewidth = namewidth;
                }
            }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            surface = SDL_CreateRGBSurface(0, surfacewidth, std::max(surfacelabels->h, surfacestats->h), 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
#else
            surface = SDL_CreateRGBSurface(0, surfacewidth, std::max(surfacelabels->h, surfacestats->h), 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
#endif

            if (surface)
            {
                SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

                SDL_Rect labelsrect, statsrect;

                labelsrect.w = surface->w;
                labelsrect.h = surface->h;
                labelsrect.x = 0;
                labelsrect.y = 0;

                auto convertedlabels = SDL_ConvertSurface(surfacelabels, surface->format, 0);

                if (convertedlabels)
                {
                    SDL_SetSurfaceAlphaMod(convertedlabels, SDL_ALPHA_OPAQUE);
                    SDL_BlitSurface(convertedlabels, NULL, surface, &labelsrect);
                    BloodSword::Free(&convertedlabels);
                }

                statsrect.w = surface->w;
                statsrect.h = surface->h;
                statsrect.x = surfacelabels->w + 8;
                statsrect.y = 0;

                auto convertedstats = SDL_ConvertSurface(surfacestats, surface->format, 0);

                if (convertedstats)
                {
                    SDL_SetSurfaceAlphaMod(convertedstats, SDL_ALPHA_OPAQUE);
                    SDL_BlitSurface(convertedstats, NULL, surface, &statsrect);
                    BloodSword::Free(&convertedstats);
                }

                // add character class if player character
                if (addname)
                {
                    auto surfacename = Graphics::CreateSurfaceText(character.Name.c_str(), font, Color::S(statscolor), style | TTF_STYLE_UNDERLINE, surfacewidth);

                    if (surfacename)
                    {
                        labelsrect.w = surface->w;
                        labelsrect.h = surface->h;
                        labelsrect.x = 0;
                        labelsrect.y = 0;

                        auto convertedname = SDL_ConvertSurface(surfacename, surface->format, 0);

                        if (convertedname)
                        {
                            SDL_SetSurfaceAlphaMod(convertedname, SDL_ALPHA_OPAQUE);
                            SDL_BlitSurface(convertedname, NULL, surface, &labelsrect);
                            BloodSword::Free(&convertedname);
                        }

                        BloodSword::Free(&surfacename);
                    }
                }

                texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

                BloodSword::Free(&surface);
            }

            BloodSword::Free(&surfacestats);
            BloodSword::Free(&surfacelabels);
        }

        return texture;
    }

    std::vector<SDL_Texture *> Attributes(Graphics::Base &graphics, Party::Base &party, TTF_Font *font, Uint32 labelcolor, Uint32 statscolor, int style, int wrap, bool addname = false, bool inbattle = false)
    {
        std::vector<SDL_Texture *> textures = {};

        for (auto i = 0; i < party.Count(); i++)
        {
            auto &character = party[i];

            auto texture = Interface::Attributes(graphics, character, font, labelcolor, statscolor, style, wrap, addname, inbattle);

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
    Scene::Base Menu(std::vector<SDL_Texture *> &choices, int x, int y, int w, int h, int start, int last, int limit, Uint32 border, Uint32 highlight, bool others = false)
    {
        auto scene = Scene::Base();

        if (!choices.empty())
        {
            auto startid = 0;
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

        return scene;
    }

    // add horizontal text menu to existing overlay
    void HorizontalMenu(Scene::Base &overlay, std::vector<SDL_Texture *> &choices, std::vector<Controls::Type> controls, int x, int y, Uint32 background, Uint32 border, Uint32 highlight)
    {
        if (!choices.empty() && !controls.empty() && choices.size() == controls.size())
        {
            auto pixels = 2;
            auto offset = pixels * 2;
            auto adjust = pixels * 4;
            auto pad = pixels * 6;
            auto items = (int)(choices.size());
            auto startid = (int)(overlay.Controls.size());

            for (auto item = 0; item < items; item++)
            {
                auto currentw = 0;
                auto currenth = 0;

                if (choices[item])
                {
                    SDL_QueryTexture(choices[item], NULL, NULL, &currentw, &currenth);
                }

                auto previousw = 0;

                if (item > 0 && choices[item - 1])
                {
                    SDL_QueryTexture(choices[item - 1], NULL, NULL, &previousw, NULL);
                }

                auto itemx = x + item * (previousw + pad);

                auto id = startid + item;
                auto lt = id > 0 ? id - 1 : id;
                auto rt = id < items - 1 ? id + 1 : id;

                overlay.Add(Scene::Element(itemx, y, currentw, currenth, background, border, pixels));
                overlay.Add(Scene::Element(choices[item], itemx, y));
                overlay.Add(Controls::Base(controls[item], id, lt, rt, id, id, itemx - offset, y - offset, currentw + adjust, currenth + adjust, highlight));
            }
        }
    }

    // skills overlay menu
    Scene::Base Skills(Point origin, int w, int h, Character::Base &character, Uint32 background, Uint32 border, int bordersize, bool inbattle = false)
    {
        auto overlay = Scene::Base();

        auto popupw = (std::max((int)(character.Skills.size()), 2) + 2) * 64;

        auto popuph = 160;

        auto screen = origin + Point(w - popupw, h - popuph) / 2;

        overlay.Add(Scene::Element(screen, popupw, popuph, background, border, bordersize));

        auto pad = 16;

        if (character.Skills.size() > 0)
        {
            for (auto i = 0; i < character.Skills.size(); i++)
            {
                SDL_Texture *texture = NULL;

                if (inbattle)
                {
                    if (Skills::IsBattleSkill(character.Skills[i]))
                    {
                        texture = Asset::Get(Skills::Assets[character.Skills[i]]);
                    }
                    else
                    {
                        texture = SkillsTexturesInactive[character.Skills[i]];
                    }
                }
                else
                {
                    if (Skills::IsStorySkill(character.Skills[i]))
                    {
                        texture = Asset::Get(Skills::Assets[character.Skills[i]]);
                    }
                    else
                    {
                        texture = SkillsTexturesInactive[character.Skills[i]];
                    }
                }

                if (texture)
                {
                    auto texturew = 0;
                    auto textureh = 0;

                    SDL_QueryTexture(texture, NULL, NULL, &texturew, &textureh);

                    auto lt = i > 0 ? i - 1 : i;
                    auto rt = i < character.Skills.size() ? i + 1 : i;

                    overlay.Add(Controls::Base(Interface::SkillControls[character.Skills[i]], i, lt, rt, i, i, screen.X + i * texturew + pad, screen.Y + pad + 32, 64, 64, Color::Highlight));
                    overlay.Add(Scene::Element(texture, screen.X + i * texturew + pad, screen.Y + pad + 32));
                }
            }
        }
        else
        {
            overlay.Add(Scene::Element(NoSkills, screen + Point(pad, pad / 2)));
        }

        auto id = (int)(character.Skills.size());

        overlay.Add(Scene::Element(Asset::Get(Asset::Type::BACK), screen.X + character.Skills.size() * 64 + pad, screen.Y + pad + 32));

        overlay.Add(Controls::Base(Controls::Type::BACK, id, id > 0 ? id - 1 : id, id, id, id, screen.X + id * 64 + pad, screen.Y + pad + 32, 64, 64, Color::Highlight));

        return overlay;
    }

    // spells overlay menu
    Scene::Base Spells(Point origin, int w, int h, Character::Base &character, Uint32 background, Uint32 border, int bordersize, bool inbattle = false)
    {
        auto spells = (int)(character.Spells.size());

        auto overlay = Scene::Base();

        auto rows = std::max(spells / 6, 1);

        auto popupw = 608;

        auto popuph = (rows + 1) * (96) + 64;

        auto screen = origin + Point(w - popupw, h - popuph) / 2;

        overlay.Add(Scene::Element(screen, popupw, popuph, background, border, bordersize));

        auto pad = 16;

        auto row = 0;

        auto col = 0;

        if (spells > 0)
        {
            for (auto i = 0; i < character.Spells.size(); i++)
            {
                auto &spell = character.Spells[i];

                SDL_Texture *texture = NULL;

                if ((!inbattle && spell.IsBasic()) || character.HasCalledToMind(character.Spells[i].Type))
                {
                    texture = Asset::Get(Spells::Assets[character.Spells[i].Type]);
                }
                else
                {
                    texture = SpellsTexturesInactive[character.Spells[i].Type];
                }

                if (texture)
                {
                    auto texturew = 0;
                    auto textureh = 0;

                    SDL_QueryTexture(texture, NULL, NULL, &texturew, &textureh);

                    auto lt = col > 0 ? i - 1 : i;
                    auto rt = col < 5 ? i + 1 : i;
                    auto up = i - 6 >= 0 ? i - 6 : i;
                    auto dn = i + 6 < (spells + 1) ? i + 6 : i;

                    auto x = screen.X + col * texturew + pad;
                    auto y = screen.Y + row * (textureh + 32) + pad + 32;

                    overlay.Add(Controls::Base(Interface::SpellControls[character.Spells[i].Type], i, lt, rt, up, dn, x, y, 64, 64, Color::Highlight));
                    overlay.Add(Scene::Element(texture, x, y));
                }

                if (col < 5)
                {
                    col++;
                }
                else
                {
                    col = 0;

                    row++;
                }
            }
        }
        else
        {
            overlay.Add(Scene::Element(NoSpells, screen + Point(pad, pad / 2)));
        }

        auto id = (int)(character.Spells.size());

        auto x = screen.X + col * 64 + pad;
        auto y = screen.Y + row * 96 + pad + 32;

        overlay.Add(Scene::Element(Asset::Get(Asset::Type::BACK), x, y));
        overlay.Add(Controls::Base(Controls::Type::BACK, id, id - 1, id, col < 6 ? id - 6 : id, id, x, y, 64, 64, Color::Highlight));

        return overlay;
    }

    // choose character from a party
    Scene::Base Party(Point origin, int w, int h, Party::Base &party, Uint32 background, Uint32 border, int bordersize)
    {
        auto overlay = Scene::Base();

        auto pad = 16;

        auto popupw = (party.Count() + 1) * 64 + pad * 2;

        auto popuph = 128 + pad * 2;

        auto screen = origin + Point(w - popupw, h - popuph) / 2;

        overlay.Add(Scene::Element(screen, popupw, popuph, background, border, bordersize));

        for (auto i = 0; i < party.Count(); i++)
        {
            auto texture = Asset::Get(party[i].Asset);

            if (texture)
            {
                auto texturew = 0;
                auto textureh = 0;

                SDL_QueryTexture(texture, NULL, NULL, &texturew, &textureh);

                auto lt = i > 0 ? i - 1 : i;
                auto rt = i < party.Count() ? i + 1 : i;

                overlay.Add(Controls::Base(Interface::CharacterControls[party[i].Class], i, lt, rt, i, i, screen.X + i * texturew + pad, screen.Y + pad + 32, 64, 64, Color::Highlight));
                overlay.Add(Scene::Element(texture, screen.X + i * texturew + pad, screen.Y + pad + 32));
            }
        }

        auto id = party.Count();

        overlay.Add(Scene::Element(Asset::Get(Asset::Type::BACK), screen.X + party.Count() * 64 + pad, screen.Y + pad + 32));

        overlay.Add(Controls::Base(Controls::Type::BACK, id, id > 0 ? id - 1 : id, id, id, id, screen.X + id * 64 + pad, screen.Y + pad + 32, 64, 64, Color::Highlight));

        return overlay;
    }

    Animation::Base Movement(Map::Base &map, Character::Base &character, std::vector<Point> path, Point start)
    {
        auto movement = Animation::Base(
            {Animation::Frame(Asset::Get(character.Asset))},
            {Animation::Type::MOVE},
            path,
            1,
            Graphics::ScanLinesEnabled ? 14 : 16,
            false);

        // scale movement scale to map dimensions
        movement.Scale = Point(map.TileSize, map.TileSize);

        movement.Delta = Point(8, 8);

        movement.Set(Point(map.DrawX, map.DrawY), start);

        movement.Reset();

        return movement;
    }

    // setup movement animation
    bool Move(Map::Base &map, Character::Base &character, Animation::Base &movement, Point &start, Point &end)
    {
        auto moving = false;

        // find a path to the destination
        auto path = Move::FindPath(map, start, end);

        auto valid = Move::Count(map, path, false);

        if (valid > 0)
        {
            map.Put(start, Map::Object::NONE, -1);

            if (character.IsPlayer())
            {
                map.Put(end, Map::Object::NONE, -1);
            }

            auto first = path.Points.begin();

            // add destination to the count
            auto moves = std::min(valid + (character.IsPlayer() ? 1 : 0), character.Moves);

            // setup animation
            movement = Interface::Movement(map, character, std::vector<Point>(first, first + moves), start);

            moving = true;
        }

        return moving;
    }

    Scene::Base Path(Map::Base &map, Character::Base &character, Point &src, Point &dst)
    {
        auto overlay = Scene::Base();

        if (map.IsValid(src))
        {
            auto path = Move::FindPath(map, src, dst);

            auto valid = Move::Count(map, path, false);

            if (valid > 0)
            {
                auto first = path.Points.begin();

                auto moves = std::min(valid + 1, character.Moves);

                auto trajectory = std::vector<Point>(first, first + moves);

                for (auto i = 0; i < path.Points.size() - 1; i++)
                {
                    auto &point = path.Points[i];

                    auto x = map.DrawX + point.X * map.TileSize;

                    auto y = map.DrawY + point.Y * map.TileSize;

                    if (i == 0)
                    {
                        overlay.Add(Scene::Element(x + 4, y + 4, map.TileSize - 8, map.TileSize - 8, 0, Color::Inactive, 2));
                    }
                    else
                    {
                        overlay.Add(Scene::Element(x, y, map.TileSize, map.TileSize, Color::Inactive));
                    }
                }
            }
        }

        return overlay;
    }

    // attribute difficulty check
    bool Test(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Uint32 border, int borderSize, Character::Base &character, Attribute::Type attribute, int roll, int modifier, bool inbattle = false)
    {
        auto result = false;

        std::string attribute_string = Attribute::TypeMapping[attribute] + std::string(": ") + Interface::ScoreString(character, attribute, inbattle);

        attribute_string += "\nDIFFICULTY: " + std::to_string(roll) + "D";

        if (modifier != 0)
        {
            if (modifier > 0)
            {
                attribute_string += "+";
            }

            attribute_string += std::to_string(modifier);
        }

        auto attribute_texture = Graphics::CreateText(graphics, attribute_string.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto attributew = 0;

        auto passed = Graphics::CreateText(graphics, "PASSED", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto failed = Graphics::CreateText(graphics, "FAILED", Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL);

        SDL_QueryTexture(attribute_texture, NULL, NULL, &attributew, NULL);

        auto start = Graphics::CreateText(graphics, {Graphics::RichText(" ROLL ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto end = Graphics::CreateText(graphics, {Graphics::RichText(" DONE ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto stage = Engine::RollStage::START;

        auto done = false;

        auto pad = 16;

        auto input = Controls::User();

        auto rolled = false;

        auto rolls = Engine::RollResult();

        while (!done)
        {
            auto overlay = Scene::Base();

            // draw border
            overlay.Add(Scene::Element(origin, w, h, Color::Background, border, borderSize));

            // add character icon
            overlay.Add(Scene::Element(Asset::Get(character.Asset), origin + Point(w - pad - 64, pad)));

            // add fight icon if in battle and attribute is FIGHTING PROWESS
            if (inbattle && attribute == Attribute::Type::FIGHTING_PROWESS)
            {
                overlay.Add(Scene::Element(Asset::Get(Asset::Type::FIGHT), origin + Point(w - pad * 2 - 128, pad)));
            }

            // attribute label
            overlay.Add(Scene::Element(attribute_texture, origin + Point(pad + 4, pad)));

            if (stage == Engine::RollStage::START)
            {
                // initial
                Interface::HorizontalMenu(overlay, start, {Controls::Type::START}, origin.X + pad + 8, origin.Y + h - pad * 3, Color::Active, Color::Active, Color::Highlight);
            }
            else if (stage == Engine::RollStage::RESULT)
            {
                // result
                Interface::HorizontalMenu(overlay, end, {Controls::Type::EXIT}, origin.X + pad + 8, origin.Y + h - pad * 3, Color::Active, Color::Active, Color::Highlight);
            }

            if (rolled)
            {
                for (auto dice = 0; dice < roll; dice++)
                {
                    // dice rolled
                    overlay.Add(Scene::Element(Asset::Get(Interface::DICE[rolls.Rolls[dice] - 1]), origin + Point(dice * (pad + 64) + pad, pad + 64)));
                }

                if (result)
                {
                    // passed
                    overlay.Add(Scene::Element(passed, origin + Point(w - pad - 64, pad + 64)));
                }
                else
                {
                    // failed
                    overlay.Add(Scene::Element(failed, origin + Point(w - pad - 64, pad + 64)));
                }
            }
            else
            {
                for (auto dice = 0; dice < roll; dice++)
                {
                    // random dice/rolling
                    overlay.Add(Scene::Element(Asset::Get(Interface::DICE[Engine::Random.NextInt() - 1]), origin + Point(dice * (pad + 64) + pad, pad + 64)));
                }
            }

            input = Input::WaitForInput(graphics, background, overlay, input, true, true);

            if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
            {
                if (input.Type == Controls::Type::START)
                {
                    if (stage == Engine::RollStage::START)
                    {
                        stage = Engine::RollStage::RESULT;

                        // roll dice
                        if (!rolled)
                        {
                            rolls = Engine::Roll(roll, modifier);

                            rolled = true;

                            // check result
                            result = rolls.Sum <= Engine::Score(character, attribute, inbattle);
                        }
                    }
                }
                else if (input.Type == Controls::Type::EXIT)
                {
                    if (stage == Engine::RollStage::RESULT)
                    {
                        done = true;
                    }
                }
            }
        }

        Free(start);
        Free(end);
        Free(&passed);
        Free(&failed);
        Free(&attribute_texture);

        return result;
    }

    // roll for damage
    int Damage(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Uint32 border, int borderSize, Character::Base &attacker, Character::Base &defender, bool inbattle = false)
    {
        SDL_Texture *damage_value = NULL;

        int damage = 0;

        std::string damage_string = "END: " + Interface::ScoreString(attacker, Attribute::Type::ENDURANCE, inbattle) + "\n" + std::string("DMG: ") + Interface::ScoreString(attacker, Attribute::Type::DAMAGE, inbattle);

        std::string armour_string = "END: " + Interface::ScoreString(defender, Attribute::Type::ENDURANCE, inbattle) + "\n" + std::string("ARM: ") + Interface::ScoreString(defender, Attribute::Type::ARMOUR, inbattle);

        auto damage_texture = Graphics::CreateText(graphics, damage_string.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto armour_texture = Graphics::CreateText(graphics, armour_string.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto passed = Graphics::CreateText(graphics, "DAMAGED", Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL);

        auto failed = Graphics::CreateText(graphics, "UNHARMED", Fonts::Normal, Color::S(Color::Inactive), TTF_STYLE_NORMAL);

        auto start = Graphics::CreateText(graphics, {Graphics::RichText(" DAMAGE ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto end = Graphics::CreateText(graphics, {Graphics::RichText(" DONE ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto stage = Engine::RollStage::START;

        auto done = false;

        auto pad = 16;

        auto input = Controls::User();

        auto rolled = false;

        auto rolls = Engine::RollResult();

        auto roll = attacker.Value(Attribute::Type::DAMAGE);

        auto modifier = attacker.Modifier(Attribute::Type::DAMAGE);

        if (inbattle && attacker.IsPlayer() && !attacker.IsArmed())
        {
            modifier -= 2;
        }

        auto textw = 0;

        auto texth = 0;

        SDL_QueryTexture(failed, NULL, NULL, &textw, &texth);

        while (!done)
        {
            auto overlay = Scene::Base();

            // draw border
            overlay.Add(Scene::Element(origin, w, h, Color::Background, border, borderSize));

            // add fight icon
            overlay.Add(Scene::Element(Asset::Get(Asset::Type::FIGHT), origin + Point((w - 64) / 2, pad)));

            // add attacker icon and stats
            overlay.Add(Scene::Element(Asset::Get(attacker.Asset), origin + pad));

            overlay.Add(Scene::Element(damage_texture, origin + Point(pad, pad + 64)));

            // add defender icon and stats
            overlay.Add(Scene::Element(Asset::Get(defender.Asset), origin + Point(w - textw - pad, pad)));

            overlay.Add(Scene::Element(armour_texture, origin + Point(w - textw - pad, pad + 64)));

            if (stage == Engine::RollStage::START)
            {
                // initial
                Interface::HorizontalMenu(overlay, start, {Controls::Type::START}, origin.X + pad + 8, origin.Y + h - pad * 3, Color::Active, Color::Active, Color::Highlight);
            }
            else if (stage == Engine::RollStage::RESULT)
            {
                // result
                Interface::HorizontalMenu(overlay, end, {Controls::Type::EXIT}, origin.X + pad + 8, origin.Y + h - pad * 3, Color::Active, Color::Active, Color::Highlight);
            }

            if (rolled)
            {
                for (auto dice = 0; dice < roll; dice++)
                {
                    // dice rolled
                    overlay.Add(Scene::Element(Asset::Get(Interface::DICE[rolls.Rolls[dice] - 1]), origin + Point(dice * (pad + 64) + pad, pad + 128)));
                }

                if (damage > 0)
                {
                    // damaged
                    overlay.Add(Scene::Element(passed, origin + Point(w - textw - pad, texth * 2 + pad + 64)));
                    overlay.Add(Scene::Element(damage_value, origin + Point(w - textw - pad, texth * 3 + pad + 64)));
                }
                else
                {
                    // unharmed
                    overlay.Add(Scene::Element(failed, origin + Point(w - textw - pad, texth * 2 + pad + 64)));
                }
            }
            else
            {
                for (auto dice = 0; dice < roll; dice++)
                {
                    // random dice/rolling
                    overlay.Add(Scene::Element(Asset::Get(Interface::DICE[Engine::Random.NextInt() - 1]), origin + Point(dice * (pad + 64) + pad, pad + 128)));
                }
            }

            input = Input::WaitForInput(graphics, background, overlay, input, true, true);

            if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
            {
                if (input.Type == Controls::Type::START)
                {
                    if (stage == Engine::RollStage::START)
                    {
                        stage = Engine::RollStage::RESULT;

                        // roll dice
                        if (!rolled)
                        {
                            rolls = Engine::Roll(roll, modifier);

                            rolled = true;

                            // check damage
                            damage = std::max(0, rolls.Sum - Engine::Score(defender, Attribute::Type::ARMOUR, inbattle));

                            if (damage > 0)
                            {
                                damage_value = Graphics::CreateText(graphics, ("-" + std::to_string(damage) + " END").c_str(), Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL);
                            }
                        }
                    }
                }
                else if (input.Type == Controls::Type::EXIT)
                {
                    if (stage == Engine::RollStage::RESULT)
                    {
                        done = true;
                    }
                }
            }
        }

        Free(end);
        Free(start);
        Free(&failed);
        Free(&passed);
        Free(&armour_texture);
        Free(&damage_texture);
        Free(&damage_value);

        return damage;
    }

    // Fight action
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

                if (Interface::Test(graphics, background, fight, fightw, fighth, Color::Active, 4, attacker, Attribute::Type::FIGHTING_PROWESS, roll, 0, true))
                {
                    auto hit = Interface::Damage(graphics, background, damage, damagew, damageh, Color::Active, 4, attacker, defender, true);

                    alive &= Engine::Damage(defender, hit, true);
                }
            }

            if (Engine::IsAlive(defender) || (Engine::Score(defender, Attribute::Type::AWARENESS, true) >= Engine::Score(attacker, Attribute::Type::AWARENESS, true) && !defender.Is(Character::Status::DEFENDING)))
            {
                if (Interface::Test(graphics, background, fight, fightw, fighth, Color::Active, 4, defender, Attribute::Type::FIGHTING_PROWESS, 2, 0, true))
                {
                    auto hit = Interface::Damage(graphics, background, damage, damagew, damageh, Color::Active, 4, defender, attacker, true);

                    alive &= Engine::Damage(attacker, hit, true);
                }
            }
        }

        return alive;
    }
}

#endif
