#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include "Animation.hpp"
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

    Controls::Mapped<Asset::Type> BattleControls = {
        {Controls::Type::MOVE, Asset::Type::MOVE},
        {Controls::Type::FIGHT, Asset::Type::FIGHT},
        {Controls::Type::SHOOT, Asset::Type::SHOOT},
        {Controls::Type::SHURIKEN, Asset::Type::SHURIKEN},
        {Controls::Type::QUARTERSTAFF, Asset::Type::QUARTERSTAFF},
        {Controls::Type::SPELLS, Asset::Type::SPELLS},
        {Controls::Type::DEFEND, Asset::Type::DEFEND},
        {Controls::Type::FLEE, Asset::Type::FLEE},
        {Controls::Type::ITEMS, Asset::Type::ITEMS},
        {Controls::Type::BACK, Asset::Type::BACK}};

    Controls::Mapped<const char *> BattleControlsText = {
        {Controls::Type::MOVE, "MOVE"},
        {Controls::Type::FIGHT, "FIGHT"},
        {Controls::Type::SHOOT, "SHOOT"},
        {Controls::Type::QUARTERSTAFF, "QUARTERSTAFF"},
        {Controls::Type::SPELLS, "SPELLS"},
        {Controls::Type::DEFEND, "DEFEND"},
        {Controls::Type::FLEE, "FLEE"},
        {Controls::Type::ITEMS, "ITEMS"},
        {Controls::Type::SHURIKEN, "SHOOT SHURIKEN"},
        {Controls::Type::BACK, "BACK"}};

    Controls::Mapped<SDL_Texture *> BattleControlCaptions = {};

    SDL_Texture *NoSkills = nullptr;

    SDL_Texture *NoSpells = nullptr;

    std::vector<Asset::Type> DICE = {
        Asset::Type::DICE1,
        Asset::Type::DICE2,
        Asset::Type::DICE3,
        Asset::Type::DICE4,
        Asset::Type::DICE5,
        Asset::Type::DICE6};

    // create textures
    void InitializeTextures(Graphics::Base &graphics)
    {
        for (auto &skill : Skills::TypeMapping)
        {
            auto active = Graphics::CreateText(graphics, skill.second, Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

            auto inactive = Graphics::CreateText(graphics, skill.second, Fonts::Caption, Color::S(Color::Inactive), TTF_STYLE_NORMAL, 0);

            SkillCaptionsActive[skill.first] = active;

            SkillCaptionsInactive[skill.first] = inactive;

            SkillsTexturesInactive[skill.first] = Asset::Copy(graphics.Renderer, Skills::Assets[skill.first], Color::Inactive);
        }

        for (auto &spell : Spells::TypeMapping)
        {
            auto active = Graphics::CreateText(graphics, spell.second, Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

            auto inactive = Graphics::CreateText(graphics, spell.second, Fonts::Caption, Color::S(Color::Inactive), TTF_STYLE_NORMAL, 0);

            SpellCaptionsActive[spell.first] = active;

            SpellCaptionsInactive[spell.first] = inactive;

            SpellsTexturesInactive[spell.first] = Asset::Copy(graphics.Renderer, Spells::Assets[spell.first], Color::Inactive);
        }

        NoSkills = Graphics::CreateText(graphics, "No special skills", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        NoSpells = Graphics::CreateText(graphics, "No spells", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        for (auto &control : BattleControlsText)
        {
            BattleControlCaptions[control.first] = Graphics::CreateText(graphics, control.second, Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);
        }
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

        for (auto &spell : SpellCaptionsActive)
        {
            Free(&spell.second);
        }

        for (auto &spell : SpellCaptionsInactive)
        {
            Free(&spell.second);
        }

        for (auto &spell : SpellsTexturesInactive)
        {
            Free(&spell.second);
        }

        for (auto &control : BattleControlCaptions)
        {
            Free(&control.second);
        }

        SkillCaptionsActive.clear();

        SkillCaptionsInactive.clear();

        SkillsTexturesInactive.clear();

        SpellCaptionsActive.clear();

        SpellCaptionsInactive.clear();

        SpellsTexturesInactive.clear();

        BattleControlCaptions.clear();

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
    Scene::Base Map(Map::Base &map, Party::Base &party, Party::Base &enemies, int num_bottom_buttons)
    {
        auto scene = Scene::Base();

        auto num_controls = 0;

        for (auto y = map.Y; y < map.Y + map.ViewY; y++)
        {
            auto ctrl_y = y - map.Y;

            for (auto x = map.X; x < map.X + map.ViewX; x++)
            {
                auto ctrl_x = x - map.X;
                auto up = num_controls;
                auto dn = num_controls;
                auto lt = num_controls;
                auto rt = num_controls;

                if (ctrl_y > 0)
                {
                    up = num_controls - map.ViewX;
                }

                if (ctrl_y < map.ViewY - 1)
                {
                    dn = num_controls + map.ViewX;
                }
                else
                {
                    // check if there is a button below to refer to
                    if (ctrl_x < num_bottom_buttons)
                    {
                        dn = num_controls + map.ViewX;
                    }
                }

                if (ctrl_x > 0)
                {
                    lt = num_controls - 1;
                }

                if (ctrl_x < map.ViewX - 1)
                {
                    rt = num_controls + 1;
                }

                auto location = Point(x, y);

                auto &tile = map[location];

                auto screen = Point(map.DrawX, map.DrawY) + Point(ctrl_x, ctrl_y) * map.TileSize;

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
                                scene.VerifyAndAdd(Scene::Element(Asset::Get(party[tile.Id].Asset), screen));

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
                                scene.VerifyAndAdd(Scene::Element(Asset::Get(enemies[tile.Id].Asset), screen));

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
                                scene.VerifyAndAdd(Scene::Element(Asset::Get(tile.TemporaryAsset), screen));

                                type = Controls::Type::TEMPORARY_OBSTACLE;

                                color = Color::Highlight;
                            }
                        }
                        else
                        {
                            if (tile.Asset != Asset::Type::NONE)
                            {
                                scene.VerifyAndAdd(Scene::Element(Asset::Get(tile.Asset), screen));
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
                        type = Controls::Type::LOCATION;

                        color = Color::Highlight;
                    }
                    else if (tile.IsPassableToEnemy())
                    {
                        color = Color::Inactive;
                    }

                    if (tile.Asset != Asset::Type::NONE)
                    {
                        scene.VerifyAndAdd(Scene::Element(Asset::Get(tile.Asset), screen));
                    }
                }

                scene.Add(Controls::Base(type, num_controls, lt, rt, up, dn, screen, map.TileSize, map.TileSize, color, location));

                num_controls++;
            }
        }

        return scene;
    }

    // create map display
    Scene::Base Map(Map::Base &map, Party::Base &party, int num_bottom_buttons)
    {
        auto enemies = Party::Base();

        // num_bottom_buttons is the number of buttons below the map
        return Interface::Map(map, party, enemies, num_bottom_buttons);
    }

    // form attribute score string
    std::string ScoreString(Character::Base &character, Attribute::Type attribute, bool in_battle = false)
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
            if (in_battle && character.IsPlayer())
            {
                if (!character.IsArmed())
                {
                    modifier -= 2;
                }

                if (character.Has(Character::Status::FPR_PLUS2))
                {
                    modifier += 2;
                }

                if (character.Has(Character::Status::FPR_PLUS1))
                {
                    modifier += 1;
                }
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
            if (in_battle && character.IsPlayer() && attribute == Attribute::Type::FIGHTING_PROWESS)
            {
                if (!character.IsArmed())
                {
                    modifier -= 2;
                }

                if (character.Has(Character::Status::FPR_PLUS2))
                {
                    modifier += 2;
                }

                if (character.Has(Character::Status::FPR_PLUS1))
                {
                    modifier += 1;
                }
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
    SDL_Texture *Attributes(Graphics::Base &graphics, Character::Base &character, TTF_Font *font, Uint32 labelcolor, Uint32 stats_color, int style, int wrap, bool add_name = false, bool in_battle = false)
    {
        SDL_Texture *texture = nullptr;

        std::string labels, stats;

        if (character.ControlType == Character::ControlType::PLAYER)
        {
            if (add_name)
            {
                labels = '\n';
            }

            labels += "RNK\nFPR\nAWR\nPSY\nEND\nDMG\nARM";

            stats += std::to_string(character.Rank);
        }
        else if (character.ControlType == Character::ControlType::NPC)
        {
            if (add_name)
            {
                labels = '\n';
            }

            labels += "FPR\nAWR\nPSY\nEND\nDMG\nARM";
        }

        auto labels_w = 0;

        auto stats_w = 0;

        // estimate maximum line width
        Graphics::Estimate(font, "DMGG", &labels_w, nullptr);

        Graphics::Estimate(font, "999D+D999", &stats_w, nullptr);

        auto surface_labels = Graphics::CreateSurfaceText(labels.c_str(), font, Color::S(labelcolor), style, labels_w);

        for (auto &attribute : Attribute::All)
        {
            if (stats.length() > 0)
            {
                stats += '\n';
            }

            stats += Interface::ScoreString(character, attribute, in_battle);
        }

        if (add_name)
        {
            stats = '\n' + stats;
        }

        auto surface_stats = Graphics::CreateSurfaceText(stats.c_str(), font, Color::S(stats_color), style, stats_w);

        if (surface_labels && surface_stats)
        {
            SDL_Surface *surface = nullptr;

            auto surface_width = surface_labels->w + surface_stats->w + 12;

            if (add_name)
            {
                auto name_width = 0;

                Graphics::Estimate(font, character.Name.c_str(), &name_width, nullptr);

                if (name_width > surface_width)
                {
                    surface_width = name_width;
                }
            }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            surface = SDL_CreateRGBSurface(0, surface_width, std::max(surface_labels->h, surface_stats->h), 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
#else
            surface = SDL_CreateRGBSurface(0, surface_width, std::max(surface_labels->h, surface_stats->h), 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
#endif

            if (surface)
            {
                SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

                SDL_Rect labels_rect, stats_rect;

                labels_rect.w = surface->w;
                labels_rect.h = surface->h;
                labels_rect.x = 0;
                labels_rect.y = 0;

                auto convertedlabels = SDL_ConvertSurface(surface_labels, surface->format, 0);

                if (convertedlabels)
                {
                    SDL_SetSurfaceAlphaMod(convertedlabels, SDL_ALPHA_OPAQUE);

                    SDL_BlitSurface(convertedlabels, nullptr, surface, &labels_rect);

                    BloodSword::Free(&convertedlabels);
                }

                stats_rect.w = surface->w;
                stats_rect.h = surface->h;
                stats_rect.x = surface_labels->w + 8;
                stats_rect.y = 0;

                auto converted_stats = SDL_ConvertSurface(surface_stats, surface->format, 0);

                if (converted_stats)
                {
                    SDL_SetSurfaceAlphaMod(converted_stats, SDL_ALPHA_OPAQUE);

                    SDL_BlitSurface(converted_stats, nullptr, surface, &stats_rect);

                    BloodSword::Free(&converted_stats);
                }

                // add character class if player character
                if (add_name)
                {
                    auto surface_name = Graphics::CreateSurfaceText(character.Name.c_str(), font, Color::S(stats_color), style | TTF_STYLE_UNDERLINE, surface_width);

                    if (surface_name)
                    {
                        labels_rect.w = surface->w;
                        labels_rect.h = surface->h;
                        labels_rect.x = 0;
                        labels_rect.y = 0;

                        auto converted_name = SDL_ConvertSurface(surface_name, surface->format, 0);

                        if (converted_name)
                        {
                            SDL_SetSurfaceAlphaMod(converted_name, SDL_ALPHA_OPAQUE);

                            SDL_BlitSurface(converted_name, nullptr, surface, &labels_rect);

                            BloodSword::Free(&converted_name);
                        }

                        BloodSword::Free(&surface_name);
                    }
                }

                texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

                BloodSword::Free(&surface);
            }

            BloodSword::Free(&surface_stats);

            BloodSword::Free(&surface_labels);
        }

        return texture;
    }

    // create party attributes text box collection
    std::vector<SDL_Texture *> Attributes(Graphics::Base &graphics, Party::Base &party, TTF_Font *font, Uint32 label_color, Uint32 stats_color, int style, int wrap, bool add_name = false, bool in_battle = false)
    {
        std::vector<SDL_Texture *> textures = {};

        for (auto i = 0; i < party.Count(); i++)
        {
            auto &character = party[i];

            auto texture = Interface::Attributes(graphics, character, font, label_color, stats_color, style, wrap, add_name, in_battle);

            if (texture)
            {
                textures.push_back(texture);
            }
        }

        return textures;
    }

    // generate stats
    std::vector<SDL_Texture *> GenerateStats(Graphics::Base &graphics, Party::Base &party, int width, bool names = true, bool in_battle = true)
    {
        return Interface::Attributes(graphics, party, Fonts::Normal, Color::Active, Color::Highlight, TTF_STYLE_NORMAL, width, names, in_battle);
    }

    // generate character skills
    SDL_Texture *Skills(Graphics::Base &graphics, Character::Base &character, TTF_Font *font, Uint32 skills_color, int style, int wrap)
    {
        std::string skills;

        if (character.Skills.size() > 0)
        {
            for (auto &skill : character.Skills)
            {
                if (skills.length() > 0)
                {
                    skills += "\n";
                }

                skills += Skills::TypeMapping[skill];
            }
        }
        else
        {
            skills = Skills::TypeMapping[Skills::Type::NONE];
        }

        auto texture = Graphics::CreateText(graphics, skills.c_str(), font, Color::S(skills_color), style, wrap);

        return texture;
    }

    // create party skills text box collection
    std::vector<SDL_Texture *> Skills(Graphics::Base &graphics, Party::Base &party, TTF_Font *font, Uint32 skills_color, int style, int wrap)
    {
        std::vector<SDL_Texture *> textures = {};

        for (auto i = 0; i < party.Count(); i++)
        {
            auto &character = party[i];

            auto texture = Interface::Skills(graphics, character, font, skills_color, style, wrap);

            if (texture)
            {
                textures.push_back(texture);
            }
        }

        return textures;
    }

    // generate texture of character status
    SDL_Texture *Status(Graphics::Base &graphics, Character::Base &character, TTF_Font *font, Uint32 color, int style, bool in_battle = false)
    {
        SDL_Texture *texture = nullptr;

        std::string list;

        auto labels_w = 0;

        auto stats_w = 0;

        // estimate maximum line width
        Graphics::Estimate(font, "DMGG ", &labels_w, nullptr);

        Graphics::Estimate(font, "999D+D999", &stats_w, nullptr);

        for (auto &status : character.Status)
        {
            if (status.second != 0)
            {
                if ((in_battle && status.first != Character::Status::IN_BATTLE && status.first != Character::Status::AWAY) || !in_battle)
                {
                    if (list.length() > 0)
                    {
                        list += '\n';
                    }

                    list += Character::StatusMapping[status.first];

                    if (status.second > 0)
                    {
                        list += " (" + std::to_string(status.second) + ")";
                    }
                }
            }
        }

        for (auto &skill : character.Skills)
        {
            auto show = (in_battle && Skills::IsBattleSkill(skill)) || (!in_battle && Skills::IsStorySkill(skill));

            if (show)
            {
                if (list.length() > 0)
                {
                    list += '\n';
                }

                list += Skills::TypeMapping[skill];

                if (skill == Skills::Type::SPELLS && character.CalledToMind.size() > 0)
                {
                    list += " (" + std::to_string(character.CalledToMind.size()) + ')';
                }
            }
        }

        if (in_battle && character.Has(Skills::Type::ARCHERY))
        {
            if (list.length() > 0)
            {
                list += '\n';
            }

            list += "ARROWS: " + std::to_string(character.Quantity(Item::Type::ARROW));
        }

        if (!in_battle)
        {
            if (list.length() > 0)
            {
                list += '\n';
            }

            list += "GOLD: " + std::to_string(character.Quantity(Item::Type::GOLD));
        }

        if (list.length() == 0)
        {
            list = "NORMAL";
        }

        list += "\n\n";

        texture = Graphics::CreateText(graphics, list.c_str(), font, Color::S(color), style, labels_w + stats_w);

        return texture;
    }

    // create party status text box collection
    std::vector<SDL_Texture *> Status(Graphics::Base &graphics, Party::Base &party, TTF_Font *font, Uint32 label_color, int style, bool in_battle = false)
    {
        std::vector<SDL_Texture *> textures = {};

        for (auto i = 0; i < party.Count(); i++)
        {
            auto &character = party[i];

            auto texture = Interface::Status(graphics, character, font, label_color, style, in_battle);

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
            SDL_RenderSetClipRect(graphics.Renderer, nullptr);
        }
    }

    // add vertical text menu to the scene
    Scene::Base Menu(std::vector<SDL_Texture *> &choices, int x, int y, int w, int h, int start, int last, int limit, Uint32 background, Uint32 border, Uint32 highlight, bool others = false)
    {
        auto scene = Scene::Base();

        if (!choices.empty())
        {
            auto start_id = 0;

            auto end = last - start;

            auto options = int(choices.size());

            auto more = options - last > 0;

            auto scroll = start_id + (options < limit ? options : limit);

            auto pixels = 2;

            auto offset = pixels * 2;

            auto adjust = pixels * 4;

            auto pad = pixels * 6;

            auto bars = options > limit;

            auto scroll_up = bars && start > 0;

            auto scroll_dn = bars && more;

            auto dim = 64;

            auto dim_adjust = dim + adjust;

            auto y_adjust = (limit - 1) * (h + pad) + (h - dim);

            auto w_adjust = w + adjust;

            auto x_offset = x + w_adjust;

            auto y_offset = y - offset;

            auto x_pad = x + w + pad;

            for (auto item = 0; item < end; item++)
            {
                auto id = start_id + item;

                auto rt = scroll_up || scroll_dn ? (item == end - 1 && scroll_dn ? (scroll_up ? scroll + 1 : scroll) : scroll) : id;

                auto up = (item > 0 ? id - 1 : id);

                auto dn = item < end - 1 ? id + 1 : (others ? (scroll_up || scroll_dn ? (scroll_up && scroll_dn ? scroll + 2 : scroll + 1) : id + 1) : (scroll_up && scroll_dn ? scroll + 2 : (scroll_dn ? scroll : id)));

                auto item_y = y + item * (h + pad);

                scene.Add(Scene::Element(x, item_y, w, h, background, border, pixels));

                scene.VerifyAndAdd(Scene::Element(choices[start + item], x, item_y));

                scene.Add(Controls::Base(Controls::Type::CHOICE, id, id, rt, up, dn, x - offset, item_y - offset, w_adjust, h + adjust, highlight));
            }

            if (options > limit)
            {
                if (scroll_up)
                {
                    scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::UP), x_pad, y));

                    scene.Add(Controls::Base(
                        Controls::Type::SCROLL_UP,
                        scroll, start_id, scroll, scroll, (more ? scroll + 1 : start_id),
                        x_offset, y_offset,
                        dim_adjust, dim_adjust,
                        highlight));
                }

                if (scroll_dn)
                {
                    scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::DOWN), x_pad, y + y_adjust));

                    scene.Add(Controls::Base(
                        Controls::Type::SCROLL_DOWN,
                        start > 0 ? scroll + 1 : scroll,
                        end - 1, start > 0 ? scroll + 1 : scroll,
                        (start > 0 ? scroll : start_id),
                        (others ? (scroll_up ? scroll + 2 : scroll + 1) : (scroll_up ? scroll + 1 : scroll)),
                        x_offset, y_offset + y_adjust,
                        dim_adjust, dim_adjust,
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

            auto items = int(choices.size());

            auto start_id = int(overlay.Controls.size());

            for (auto item = 0; item < items; item++)
            {
                auto current_w = 0;

                auto current_h = 0;

                if (choices[item])
                {
                    SDL_QueryTexture(choices[item], nullptr, nullptr, &current_w, &current_h);
                }

                auto previous_w = 0;

                if (item > 0 && choices[item - 1])
                {
                    SDL_QueryTexture(choices[item - 1], nullptr, nullptr, &previous_w, nullptr);
                }

                auto item_x = x + item * (previous_w + pad);

                auto id = start_id + item;

                auto lt = id > 0 ? id - 1 : id;

                auto rt = id < items - 1 ? id + 1 : id;

                overlay.Add(Scene::Element(item_x, y, current_w, current_h, background, border, pixels));

                overlay.VerifyAndAdd(Scene::Element(choices[item], item_x, y));

                overlay.Add(Controls::Base(controls[item], id, lt, rt, id, id, item_x - offset, y - offset, current_w + adjust, current_h + adjust, highlight));
            }
        }
    }

    // skills overlay menu
    Scene::Base Skills(Point origin, int w, int h, Character::Base &character, Uint32 background, Uint32 border, int border_size, bool in_battle = false)
    {
        auto overlay = Scene::Base();

        auto popup_w = (std::max(int(character.Skills.size()), 2) + 2) * 64;

        auto popup_h = 160;

        auto screen = origin + Point(w - popup_w, h - popup_h) / 2;

        overlay.Add(Scene::Element(screen, popup_w, popup_h, background, border, border_size));

        auto pad = 16;

        if (character.Skills.size() > 0)
        {
            for (auto i = 0; i < character.Skills.size(); i++)
            {
                SDL_Texture *texture = nullptr;

                if (in_battle)
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
                    auto texture_w = 0;

                    SDL_QueryTexture(texture, nullptr, nullptr, &texture_w, nullptr);

                    auto lt = i > 0 ? i - 1 : i;

                    auto rt = i < character.Skills.size() ? i + 1 : i;

                    overlay.Add(Controls::Base(Interface::SkillControls[character.Skills[i]], i, lt, rt, i, i, screen.X + i * texture_w + pad, screen.Y + pad + 32, 64, 64, Color::Highlight));

                    overlay.VerifyAndAdd(Scene::Element(texture, screen.X + i * texture_w + pad, screen.Y + pad + 32));
                }
            }
        }
        else
        {
            overlay.VerifyAndAdd(Scene::Element(NoSkills, screen + Point(pad, pad / 2)));
        }

        auto id = int(character.Skills.size());

        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), screen.X + character.Skills.size() * 64 + pad, screen.Y + pad + 32));

        overlay.Add(Controls::Base(Controls::Type::BACK, id, id > 0 ? id - 1 : id, id, id, id, screen.X + id * 64 + pad, screen.Y + pad + 32, 64, 64, Color::Highlight));

        return overlay;
    }

    // spells overlay menu
    Scene::Base Spells(Point origin, int w, int h, Character::Base &character, Uint32 background, Uint32 border, int border_size, bool in_battle = false)
    {
        auto spells = int(character.Spells.size());

        auto overlay = Scene::Base();

        auto rows = std::max(spells / 6, 1);

        auto popup_w = 608;

        auto popup_h = (rows + 1) * (96) + 64;

        auto screen = origin + Point(w - popup_w, h - popup_h) / 2;

        overlay.Add(Scene::Element(screen, popup_w, popup_h, background, border, border_size));

        auto pad = 16;

        auto row = 0;

        auto col = 0;

        if (spells > 0)
        {
            for (auto i = 0; i < character.Spells.size(); i++)
            {
                auto &spell = character.Spells[i];

                SDL_Texture *texture = nullptr;

                if ((!in_battle && spell.IsBasic()) || character.HasCalledToMind(character.Spells[i].Type))
                {
                    texture = Asset::Get(Spells::Assets[character.Spells[i].Type]);
                }
                else
                {
                    texture = SpellsTexturesInactive[character.Spells[i].Type];
                }

                if (texture)
                {
                    auto texture_w = 0;

                    auto texture_h = 0;

                    SDL_QueryTexture(texture, nullptr, nullptr, &texture_w, &texture_h);

                    auto lt = col > 0 ? i - 1 : i;

                    auto rt = col < 5 ? i + 1 : i;

                    auto up = i - 6 >= 0 ? i - 6 : i;

                    auto dn = i + 6 < (spells + 1) ? i + 6 : i;

                    auto x = screen.X + col * texture_w + pad;

                    auto y = screen.Y + row * (texture_h + 32) + pad + 32;

                    overlay.Add(Controls::Base(Interface::SpellControls[character.Spells[i].Type], i, lt, rt, up, dn, x, y, 64, 64, Color::Highlight));

                    overlay.VerifyAndAdd(Scene::Element(texture, x, y));
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
            overlay.VerifyAndAdd(Scene::Element(NoSpells, screen + Point(pad, pad / 2)));
        }

        auto id = int(character.Spells.size());

        auto x = screen.X + col * 64 + pad;

        auto y = screen.Y + row * 96 + pad + 32;

        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), x, y));

        overlay.Add(Controls::Base(Controls::Type::BACK, id, id - 1, id, col < 6 ? id - 6 : id, id, x, y, 64, 64, Color::Highlight));

        return overlay;
    }

    // choose character from a party
    Scene::Base SelectCharacter(Point origin, int w, int h, Party::Base &party, int popup_w, int popup_h, Uint32 background, Uint32 border, int border_size, bool back = true)
    {
        auto overlay = Scene::Base();

        auto pad = 16;

        auto screen = origin + Point(w - popup_w, h - popup_h) / 2;

        overlay.Add(Scene::Element(screen, popup_w, popup_h, background, border, border_size));

        for (auto i = 0; i < party.Count(); i++)
        {
            auto texture = Asset::Get(party[i].Asset);

            if (texture)
            {
                auto texture_w = 0;

                SDL_QueryTexture(texture, nullptr, nullptr, &texture_w, nullptr);

                auto lt = i > 0 ? i - 1 : i;

                auto rt = 0;

                if (back)
                {
                    rt = i < party.Count() ? i + 1 : i;
                }
                else
                {
                    rt = i < party.Count() - 1 ? i + 1 : i;
                }

                overlay.Add(Controls::Base(Interface::CharacterControls[party[i].Class], i, lt, rt, i, i, screen.X + i * texture_w + pad, screen.Y + pad + 32, 64, 64, Color::Highlight));

                overlay.VerifyAndAdd(Scene::Element(texture, screen.X + i * texture_w + pad, screen.Y + pad + 32));
            }
        }

        if (back)
        {
            auto id = party.Count();

            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), screen.X + party.Count() * 64 + pad, screen.Y + pad + 32));

            overlay.Add(Controls::Base(Controls::Type::BACK, id, id > 0 ? id - 1 : id, id, id, id, screen.X + id * 64 + pad, screen.Y + pad + 32, 64, 64, Color::Highlight));
        }

        return overlay;
    }

    // choose character from a party
    Scene::Base SelectCharacter(Point origin, int w, int h, Party::Base &party, Uint32 background, Uint32 border, int border_size, bool back = true)
    {
        auto pad = 16;

        auto popup_w = (party.Count() + 1) * 64 + pad * 2;

        auto popup_h = 128 + pad * 2;

        return Interface::SelectCharacter(origin, w, h, party, popup_w, popup_h, background, border, border_size, back);
    }

    // setup movement animation
    Animation::Base Movement(Map::Base &map, Character::Base &character, std::vector<Point> path, Point start)
    {
        auto movement = Animation::Base(
            {Animation::Frame(Asset::Get(character.Asset))},
            {Animation::Type::MOVE},
            path,
            1,
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

    // path overlay from src to dst
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
    bool Test(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Uint32 border, int border_size, Character::Base &character, Attribute::Type attribute, int roll, int modifier, bool in_battle = false, Asset::Type asset = Asset::Type::NONE)
    {
        auto result = false;

        std::string attribute_string = Attribute::TypeMapping[attribute] + std::string(": ") + Interface::ScoreString(character, attribute, in_battle);

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

        auto passed = Graphics::CreateText(graphics, "PASSED", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto failed = Graphics::CreateText(graphics, "FAILED", Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL);

        auto attribute_w = 0;

        SDL_QueryTexture(attribute_texture, nullptr, nullptr, &attribute_w, nullptr);

        auto start = Graphics::CreateText(graphics, {Graphics::RichText(" ROLL ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto end = Graphics::CreateText(graphics, {Graphics::RichText(" DONE ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto score = Engine::Score(character, attribute, in_battle);

        auto stage = Engine::RollStage::START;

        auto input = Controls::User();

        auto done = false;

        auto pad = 16;

        auto rolled = false;

        auto rolls = Engine::RollResult();

        // Unarmed/Eye of the Tiger effects
        if (attribute == Attribute::Type::FIGHTING_PROWESS)
        {
            if (in_battle && !character.IsArmed())
            {
                score -= 2;
            }

            if (character.Has(Character::Status::FPR_PLUS2))
            {
                score += 2;
            }
            else if (character.Has(Character::Status::FPR_PLUS1))
            {
                score += 1;
            }
        }

        score = std::max(0, score);

        while (!done)
        {
            auto overlay = Scene::Base();

            // draw border
            overlay.Add(Scene::Element(origin, w, h, Color::Background, border, border_size));

            // add character icon
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(character.Asset), origin + Point(w - pad - 64, pad)));

            // set up icon
            if (in_battle && attribute == Attribute::Type::FIGHTING_PROWESS)
            {
                if (asset == Asset::Type::NONE)
                {
                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::FIGHT), origin + Point(w - pad * 2 - 128, pad)));
                }
                else
                {
                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(asset), origin + Point(w - pad * 2 - 128, pad)));
                }
            }
            else if (attribute == Attribute::Type::PSYCHIC_ABILITY && asset != Asset::Type::NONE)
            {
                overlay.VerifyAndAdd(Scene::Element(Asset::Get(asset), origin + Point(w - pad * 2 - 128, pad)));
            }

            // attribute label
            overlay.VerifyAndAdd(Scene::Element(attribute_texture, origin + Point(pad + 4, pad)));

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
                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Interface::DICE[rolls.Rolls[dice] - 1]), origin + Point(dice * (pad + 64) + pad, pad + 64)));
                }

                if (result)
                {
                    // passed
                    overlay.VerifyAndAdd(Scene::Element(passed, origin + Point(w - pad - 64, pad + 64)));
                }
                else
                {
                    // failed
                    overlay.VerifyAndAdd(Scene::Element(failed, origin + Point(w - pad - 64, pad + 64)));
                }
            }
            else
            {
                for (auto dice = 0; dice < roll; dice++)
                {
                    // random dice/rolling
                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Interface::DICE[Engine::Random.NextInt() - 1]), origin + Point(dice * (pad + 64) + pad, pad + 64)));
                }
            }

            input = Input::WaitForInput(graphics, background, overlay, input, true, true, 0);

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
                            result = rolls.Sum <= score;
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
    int Damage(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Uint32 border, int border_size, Character::Base &attacker, Character::Base &defender, int roll, int modifier, bool in_battle = false, bool ignore_armor = false, Asset::Type asset = Asset::Type::NONE)
    {
        SDL_Texture *damage_value = nullptr;

        int damage = 0;

        // Eye of the Tiger effects
        if (attacker.Has(Character::Status::FPR_PLUS2))
        {
            modifier += 2;
        }
        else if (attacker.Has(Character::Status::FPR_PLUS1))
        {
            modifier += 1;
        }

        std::string damage_string = "END: " + Interface::ScoreString(attacker, Attribute::Type::ENDURANCE, in_battle) + "\n" + std::string("DMG: ") + std::to_string(roll) + 'D';

        if (modifier != 0)
        {
            if (modifier > 0)
            {
                damage_string += '+';
            }

            damage_string += std::to_string(modifier);
        }

        std::string armour_string = "END: " + Interface::ScoreString(defender, Attribute::Type::ENDURANCE, in_battle) + "\n" + std::string("ARM: ") + Interface::ScoreString(defender, Attribute::Type::ARMOUR, in_battle);

        auto damage_texture = Graphics::CreateText(graphics, damage_string.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto armour_texture = Graphics::CreateText(graphics, armour_string.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto passed = Graphics::CreateText(graphics, "DAMAGED", Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL);

        auto failed = Graphics::CreateText(graphics, "UNHARMED", Fonts::Normal, Color::S(Color::Inactive), TTF_STYLE_NORMAL);

        auto start = Graphics::CreateText(graphics, {Graphics::RichText(" DAMAGE ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto end = Graphics::CreateText(graphics, {Graphics::RichText(" DONE ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto stage = Engine::RollStage::START;

        auto rolls = Engine::RollResult();

        auto input = Controls::User();

        auto done = false;

        auto pad = 16;

        auto rolled = false;

        auto text_w = 0;

        auto text_h = 0;

        SDL_QueryTexture(failed, nullptr, nullptr, &text_w, &text_h);

        while (!done)
        {
            auto overlay = Scene::Base();

            // draw border
            overlay.Add(Scene::Element(origin, w, h, Color::Background, border, border_size));

            // add damage icon
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(asset), origin + Point((w - 64) / 2, pad)));

            // add attacker icon and stats
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(attacker.Asset), origin + pad));

            overlay.VerifyAndAdd(Scene::Element(damage_texture, origin + Point(pad, pad + 64)));

            // add defender icon and stats
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(defender.Asset), origin + Point(w - text_w - pad, pad)));

            // add armour stats
            overlay.VerifyAndAdd(Scene::Element(armour_texture, origin + Point(w - text_w - pad, pad + 64)));

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
                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Interface::DICE[rolls.Rolls[dice] - 1]), origin + Point(dice * (pad + 64) + pad, pad + 128)));
                }

                if (damage > 0)
                {
                    // damaged
                    overlay.VerifyAndAdd(Scene::Element(passed, origin + Point(w - text_w - pad, text_h * 2 + pad + 64)));

                    overlay.VerifyAndAdd(Scene::Element(damage_value, origin + Point(w - text_w - pad, text_h * 3 + pad + 64)));
                }
                else
                {
                    // unharmed
                    overlay.VerifyAndAdd(Scene::Element(failed, origin + Point(w - text_w - pad, text_h * 2 + pad + 64)));
                }
            }
            else
            {
                for (auto dice = 0; dice < roll; dice++)
                {
                    // random dice/rolling
                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Interface::DICE[Engine::Random.NextInt() - 1]), origin + Point(dice * (pad + 64) + pad, pad + 128)));
                }
            }

            input = Input::WaitForInput(graphics, background, overlay, input, true, true, 0);

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
                            damage = std::max(0, rolls.Sum - (!ignore_armor ? 0 : Engine::Score(defender, Attribute::Type::ARMOUR, in_battle)));

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

    // roll for damage
    int Damage(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Uint32 border, int border_size, Character::Base &attacker, Character::Base &defender, bool in_battle = false, bool shooting = false, bool knockout = false, bool ignore_armor = false, Asset::Type asset = Asset::Type::NONE)
    {
        auto roll = !shooting ? attacker.Value(Attribute::Type::DAMAGE) : 1;

        roll += knockout ? 1 : 0;

        auto modifier = !shooting ? attacker.Modifier(Attribute::Type::DAMAGE) : 0;

        if (in_battle && attacker.IsPlayer() && !attacker.IsArmed())
        {
            modifier -= 2;
        }

        if (!shooting && !knockout)
        {
            asset = Asset::Type::FIGHT;
        }

        return Interface::Damage(graphics, background, origin, w, h, border, border_size, attacker, defender, roll, modifier, in_battle, ignore_armor, asset);
    }

    // draws a message box on screen
    void MessageBox(Graphics::Base &graphics, Scene::Base &scene, Point offset, int width, int height, SDL_Texture *message, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        auto box = Scene::Base();

        auto pad = 16;

        if (message)
        {
            auto texture_w = 0;

            auto texture_h = 0;

            SDL_QueryTexture(message, nullptr, nullptr, &texture_w, &texture_h);

            auto box_w = texture_w + pad * 2;

            auto box_h = texture_h + pad * 3 + 64;

            auto location = offset + (Point(width, height) - Point(box_w, box_h)) / 2;

            auto confirm = location + Point(pad + texture_w / 2 - 32, texture_h + pad * 2);

            auto input = Controls::User();

            box.Add(Scene::Element(location, box_w, box_h, background, border, border_size));

            box.VerifyAndAdd(Scene::Element(message, location + Point(pad, pad)));

            box.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CONFIRM), confirm));

            box.Add(Controls::Base(Controls::Type::CONFIRM, 0, 0, 0, 0, 0, confirm.X, confirm.Y, 64, 64, highlight));

            while (true)
            {
                input = Input::WaitForInput(graphics, scene, box, input, true, blur);

                if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
                {
                    if (input.Type == Controls::Type::CONFIRM)
                    {
                        break;
                    }
                }
            }
        }
    }

    // draws a message box over a scene
    void MessageBox(Graphics::Base &graphics, Scene::Base &scene, Point origin, int width, int height, Graphics::RichText message, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        auto texture = Graphics::CreateText(graphics, message.Text.c_str(), message.Font, message.Color, message.Style);

        if (texture)
        {
            Interface::MessageBox(graphics, scene, origin, width, height, texture, background, border, border_size, highlight, blur);

            Free(&texture);
        }
    }

    // draws a message box over a scene
    void MessageBox(Graphics::Base &graphics, Scene::Base &scene, SDL_Texture *message, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        Interface::MessageBox(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, message, background, border, border_size, highlight, blur);
    }

    // draws a message box over a scene
    void MessageBox(Graphics::Base &graphics, Scene::Base &scene, Graphics::RichText message, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        auto texture = Graphics::CreateText(graphics, message.Text.c_str(), message.Font, message.Color, message.Style);

        if (texture)
        {
            Interface::MessageBox(graphics, scene, texture, background, border, border_size, highlight, blur);

            Free(&texture);
        }
    }

    // draws a message box
    void MessageBox(Graphics::Base &graphics, Graphics::RichText message, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        auto texture = Graphics::CreateText(graphics, message.Text.c_str(), message.Font, message.Color, message.Style);

        if (texture)
        {
            auto scene = Scene::Base();

            Interface::MessageBox(graphics, scene, texture, background, border, border_size, highlight, blur);

            Free(&texture);
        }
    }

    // draws a confirmation message box on screen
    bool Confirm(Graphics::Base &graphics, Scene::Base &scene, Point offset, int width, int height, SDL_Texture *message, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        auto result = false;

        auto box = Scene::Base();

        auto pad = 16;

        if (message)
        {
            auto texture_w = 0;

            auto texture_h = 0;

            SDL_QueryTexture(message, nullptr, nullptr, &texture_w, &texture_h);

            auto box_w = texture_w + pad * 2;

            auto box_h = texture_h + pad * 3 + 64;

            auto location = offset + (Point(width, height) - Point(box_w, box_h)) / 2;

            auto confirm = location + Point(texture_w / 2 - 64, texture_h + pad * 2);

            auto input = Controls::User();

            box.Add(Scene::Element(location, box_w, box_h, background, border, border_size));

            box.VerifyAndAdd(Scene::Element(message, location + Point(pad, pad)));

            box.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CONFIRM), confirm));

            box.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CANCEL), confirm + Point(64 + pad * 2, 0)));

            box.Add(Controls::Base(Controls::Type::CONFIRM, 0, 0, 1, 0, 0, confirm.X, confirm.Y, 64, 64, highlight));

            box.Add(Controls::Base(Controls::Type::CANCEL, 1, 0, 1, 1, 1, confirm.X + 64 + pad * 2, confirm.Y, 64, 64, highlight));

            while (true)
            {
                input = Input::WaitForInput(graphics, scene, box, input, true, blur);

                if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
                {
                    if (input.Type == Controls::Type::CONFIRM)
                    {
                        result = true;

                        break;
                    }
                    else if (input.Type == Controls::Type::CANCEL)
                    {
                        result = false;

                        break;
                    }
                }
            }
        }

        return result;
    }

    // draws a confirm message box over a scene
    bool Confirm(Graphics::Base &graphics, Scene::Base &scene, SDL_Texture *message, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        return Interface::Confirm(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, message, background, border, border_size, highlight, blur);
    }

    // draws a confirm message box over a scene
    bool Confirm(Graphics::Base &graphics, Scene::Base &scene, Graphics::RichText message, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        auto result = false;

        auto texture = Graphics::CreateText(graphics, message.Text.c_str(), message.Font, message.Color, message.Style);

        if (texture)
        {
            result = Interface::Confirm(graphics, scene, texture, background, border, border_size, highlight, blur);

            Free(&texture);
        }

        return result;
    }

    // focus cursor on character on the map
    void Focus(Map::Base &map, Engine::Queue &order, int &character, Scene::Base &overlay)
    {
        auto draw = Point(map.DrawX, map.DrawY);

        auto focus = map.Find(Engine::IsPlayer(order, character) ? Map::Object::PLAYER : Map::Object::ENEMY, order[character].Id);

        if (map.IsVisible(focus))
        {
            auto screen = (draw + focus * map.TileSize) + 4;

            overlay.Add(Scene::Element(screen.X, screen.Y, map.TileSize - 8, map.TileSize - 8, 0, Color::Active, 2));
        }
    }

    // cast spell
    bool Cast(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Character::Base &caster, Spells::Type spell, bool in_battle)
    {
        auto result = false;

        auto cast_w = 512;

        auto cast_h = 208;

        auto cast = origin + (Point(w, h) - Point(cast_w, cast_h)) / 2;

        if (!caster.Is(Character::Status::DEFENDING))
        {
            if (Engine::IsAlive(caster))
            {
                auto casting = caster.Find(spell);

                if (caster.HasCalledToMind(spell) && casting != caster.Spells.end())
                {
                    auto roll = 2;

                    auto modifier = casting->CurrentComplexity;

                    result = Interface::Test(graphics, background, cast, cast_w, cast_h, Color::Active, 4, caster, Attribute::Type::PSYCHIC_ABILITY, roll, modifier, in_battle, Spells::Assets[spell]);

                    if (!result)
                    {
                        casting->CurrentComplexity--;
                    }
                    else
                    {
                        casting->CurrentComplexity = casting->Complexity;

                        caster.Forget(spell);
                    }
                }
            }
        }

        return result;
    }

    // select from a list of options
    int Choice(Graphics::Base &graphics, Scene::Base &background, std::vector<Graphics::RichText> &choices, Point origin, int w, int h, int limit, Uint32 bg_color, Uint32 border, Uint32 highlight, bool blur = true)
    {
        auto menu = Graphics::CreateText(graphics, choices);

        auto options = int(choices.size());

        auto input = Controls::User();

        auto choice = -1;

        auto start = 0;

        auto last = start + limit;

        auto done = false;

        while (!done)
        {
            auto overlay = Interface::Menu(menu, origin.X, origin.Y, w, h, start, last, limit, bg_color, border, highlight, false);

            if (input.Up)
            {
                input.Current = Controls::Find(overlay.Controls, Controls::Type::SCROLL_UP);

                input.Up = false;
            }
            else if (input.Down)
            {
                input.Current = Controls::Find(overlay.Controls, Controls::Type::SCROLL_DOWN);

                input.Down = false;
            }

            input = Input::WaitForInput(graphics, background, overlay, input, true, blur);

            if ((input.Selected && input.Type != Controls::Type::NONE && !input.Hold) || input.Up || input.Down)
            {
                if (input.Type == Controls::Type::CHOICE)
                {
                    choice = start + input.Current;

                    if (choice >= 0 && choice < options)
                    {
                        done = true;
                    }
                }
                else if (input.Type == Controls::Type::SCROLL_UP || input.Up)
                {
                    if (start > 0)
                    {
                        start -= 1;

                        if (start < 0)
                        {
                            start = 0;
                        }

                        last = start + limit;

                        if (last > options)
                        {
                            last = options;
                        }

                        input.Up = true;
                    }
                }
                else if (input.Type == Controls::Type::SCROLL_DOWN || input.Down)
                {
                    if (options - last > 0)
                    {
                        if (start < options - limit)
                        {
                            start += 1;
                        }

                        if (start > options - limit)
                        {
                            start = options - limit;
                        }

                        last = start + limit;

                        if (last > options)
                        {
                            last = options;
                        }

                        input.Down = true;
                    }
                }
            }
        }

        Free(menu);

        return choice;
    }

    std::vector<SDL_Texture *> GenerateCharacterClassCaptions(Graphics::Base &graphics, Party::Base &party)
    {
        std::vector<Graphics::RichText> characters = {};

        for (auto character = 0; character < party.Count(); character++)
        {
            auto alive = Engine::Score(party[character], Attribute::Type::ENDURANCE) > 0;

            characters.push_back(Graphics::RichText(Character::ClassMapping[party[character].Class], Fonts::Caption, alive ? Color::Active : Color::Inactive, TTF_STYLE_NORMAL, 160));
        }

        return Graphics::CreateText(graphics, characters);
    }

    std::vector<SDL_Texture *> GenerateCharacterClassCaptions(Graphics::Base &graphics, int captionw)
    {
        auto characters = Graphics::CreateText(
            graphics,
            {Graphics::RichText("WARRIOR", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, captionw),
             Graphics::RichText("TRICKSTER", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, captionw),
             Graphics::RichText("SAGE", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, captionw),
             Graphics::RichText("ENCHANTER", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, captionw)});

        return characters;
    }

    // choose a character
    Character::Class SelectCharacter(Graphics::Base &graphics, int rank, Party::Base &current_party)
    {
        auto character_class = Character::Class::NONE;

        // create party
        auto party = Party::Base({Generate::Character(Character::Class::WARRIOR, rank),
                                  Generate::Character(Character::Class::TRICKSTER, rank),
                                  Generate::Character(Character::Class::SAGE, rank),
                                  Generate::Character(Character::Class::ENCHANTER, rank)});

        auto stats = Interface::GenerateStats(graphics, party, 320, false, true);

        auto skills = Interface::Skills(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 320);

        auto select = Graphics::CreateText(graphics, "CHOOSE A CHARACTER TO ADD", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto current = Graphics::CreateText(graphics, "CURRENT PARTY", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto captions = Interface::GenerateCharacterClassCaptions(graphics, party);

        auto pad = 10;

        auto input = Controls::User();

        auto done = false;

        std::vector<Attribute::Type> attributes = {
            Attribute::Type::FIGHTING_PROWESS,
            Attribute::Type::AWARENESS,
            Attribute::Type::PSYCHIC_ABILITY};

        auto popup_pad = 16;

        auto popup_w = (party.Count() + 1) * 64 + popup_pad * 2;

        auto popup_h = 0;

        if (stats.size() > 0)
        {
            SDL_QueryTexture(stats[0], nullptr, nullptr, nullptr, &popup_h);
        }

        while (!done)
        {
            auto scene = Scene::Base();

            auto overlay = Scene::Base();

            if (popup_h > 0)
            {
                overlay = Interface::SelectCharacter(Point(0, 0), graphics.Width, graphics.Height, party, popup_w, popup_h, 0, Color::Active, 4, false);
            }
            else
            {
                overlay = Interface::SelectCharacter(Point(0, 0), graphics.Width, graphics.Height, party, 0, Color::Active, 4, false);
            }

            auto &popup = overlay.Elements[0];

            overlay.VerifyAndAdd(Scene::Element(select, popup.X + 16, popup.Y + 8));

            if (Input::IsValid(overlay, input))
            {
                // party popup captions
                if (input.Type != Controls::Type::BACK && input.Current >= 0 && input.Current < party.Count())
                {
                    auto &control = overlay.Controls[input.Current];

                    overlay.VerifyAndAdd(Scene::Element(captions[input.Current], control.X, control.Y + control.H + pad));

                    if (stats[input.Current])
                    {
                        auto stats_w = 0;

                        SDL_QueryTexture(stats[input.Current], nullptr, nullptr, &stats_w, nullptr);

                        overlay.VerifyAndAdd(Scene::Element(stats[input.Current], popup.X - (stats_w + pad * 2), popup.Y, 0, Color::Active, 4));
                    }

                    if (skills[input.Current])
                    {
                        auto skills_x = popup.X + (popup.W + pad * 2);

                        overlay.VerifyAndAdd(Scene::Element(skills[input.Current], skills_x, popup.Y));

                        overlay.Add(Scene::Element(skills_x, popup.Y, 320, popup.H, 0, Color::Active, 4));
                    }
                }
            }

            if (current_party.Count() > 0)
            {
                auto &origin = overlay.Elements[0];

                auto screen = Point(origin.X, origin.Y - 160);

                overlay.Add(Scene::Element(screen, popup.W, 128, 0, Color::Active, 4));

                for (auto i = 0; i < current_party.Count(); i++)
                {
                    auto texture = Asset::Get(current_party[i].Asset);

                    if (texture)
                    {
                        auto texture_w = 0;

                        SDL_QueryTexture(texture, nullptr, nullptr, &texture_w, nullptr);

                        overlay.VerifyAndAdd(Scene::Element(texture, screen.X + i * texture_w + pad, screen.Y + pad + 32));
                    }
                }

                overlay.VerifyAndAdd(Scene::Element(current, screen.X + 16, screen.Y + pad));
            }

            input = Input::WaitForInput(graphics, scene, overlay, input, true);

            if ((input.Selected && input.Type != Controls::Type::NONE && !input.Hold) || input.Up || input.Down)
            {
                if (input.Type == Controls::Type::BACK)
                {
                    done = true;
                }
                else if (Input::IsPlayer(input) && input.Current >= 0 && input.Current < party.Count())
                {
                    character_class = party[input.Current].Class;

                    done = true;
                }
            }
        }

        Free(&current);

        Free(&select);

        Free(skills);

        Free(stats);

        Free(captions);

        return character_class;
    }

    // create a party
    Party::Base CreateParty(Graphics::Base &graphics, bool blur = true)
    {
        auto scene = Scene::Base();

        auto bg_scene = Scene::Base();

        auto width = 254;

        auto base_height = 32;

        auto pad = 8;

        auto items = 4;

        auto height = (base_height + pad * 2) * items - pad * 7 / 2;

        auto origin = Point(graphics.Width - width, graphics.Height - height) / 2;

        auto menu_w = 0;

        auto menu_title = Graphics::CreateText(graphics, "CHOOSE NUMBER OF PARTY MEMBERS", Fonts::Caption, Color::S(Color::Highlight), TTF_STYLE_NORMAL);

        SDL_QueryTexture(menu_title, nullptr, nullptr, &menu_w, nullptr);

        scene.Add(Scene::Element(menu_title, Point((graphics.Width - menu_w) / 2, origin.Y - pad * 6)));

        scene.Add(Scene::Element(origin - Point(pad, pad), width + pad * 2, height + pad * 2, Color::Background, Color::Active, 4));

        auto party = Party::Base();

        std::vector<Graphics::RichText> party_sizes = {
            Graphics::RichText(" 1 Starting rank(s): 8", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
            Graphics::RichText(" 2 Starting rank(s): 4", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
            Graphics::RichText(" 3 Starting rank(s): 3", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
            Graphics::RichText(" 4 Starting rank(s): 2", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        };

        auto current = Graphics::CreateText(graphics, "CURRENT PARTY", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto current_w = 0;

        if (current)
        {
            SDL_QueryTexture(current, nullptr, nullptr, &current_w, nullptr);
        }

        auto party_size = Interface::Choice(graphics, scene, party_sizes, origin, width, base_height, 4, Color::Background, Color::Background, Color::Highlight, blur) + 1;

        if (party_size > 0 && party_size <= 4)
        {
            auto rank = 8;

            if (party_size == 2)
            {
                rank = 4;
            }
            else if (party_size == 3)
            {
                rank = 3;
            }
            else if (party_size == 4)
            {
                rank = 2;

                for (auto &character_class : Character::All)
                {
                    auto character = Generate::Character(character_class, rank);

                    party.Add(character);
                }
            }

            while (party.Count() != party_size)
            {
                bg_scene = Scene::Base();

                auto character_class = Interface::SelectCharacter(graphics, rank, party);

                if (character_class != Character::Class::NONE)
                {
                    if (!party.Has(character_class))
                    {
                        auto character = Generate::Character(character_class, rank);

                        party.Add(character);

                        Interface::MessageBox(graphics, bg_scene, Graphics::RichText(std::string(Character::ClassMapping[character_class]) + " added to the party!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), 0, Color::Active, 4, Color::Highlight, false);
                    }
                    else
                    {
                        party.Remove(character_class);

                        Interface::MessageBox(graphics, bg_scene, Graphics::RichText(std::string(Character::ClassMapping[character_class]) + " removed from the party!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), 0, Color::Highlight, 4, Color::Active, false);
                    }
                }

                if (party.Count() == party_size)
                {
                    auto party_x = (graphics.Width - (party_size * 64)) / 2;

                    auto party_y = (graphics.Height - 432) / 2;

                    bg_scene = Scene::Base();

                    for (auto i = 0; i < party_size; i++)
                    {
                        auto texture = Asset::Get(party[i].Asset);

                        if (texture)
                        {
                            auto texture_w = 0;

                            SDL_QueryTexture(texture, nullptr, nullptr, &texture_w, nullptr);

                            bg_scene.VerifyAndAdd(Scene::Element(texture, party_x + i * texture_w, party_y + pad + 32));
                        }
                    }

                    bg_scene.VerifyAndAdd(Scene::Element(current, (graphics.Width - current_w) / 2, party_y + pad));

                    if (!Interface::Confirm(graphics, bg_scene, Graphics::RichText("Proceed with this party?", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), 0, Color::Active, 4, Color::Inactive, false))
                    {
                        party.Clear();
                    }
                }

                bg_scene = Scene::Base();
            }

            Interface::MessageBox(graphics, bg_scene, Graphics::RichText("Party Complete!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), 0, Color::Active, 4, Color::Highlight, false);
        }

        Free(&current);

        Free(&menu_title);

        return party;
    }

    std::string TextInput(Graphics::Base &graphics, Scene::Base &background, Point location, std::string question, std::string start_text, Uint32 question_color, Uint32 input_color, int input_limit, int box_w, int box_h, Uint32 border = Color::Active, Uint32 box_bg = Color::Background, int border_size = 4, bool blur = true)
    {
        auto message = Graphics::CreateText(graphics, question.c_str(), Fonts::Normal, Color::S(question_color), TTF_STYLE_NORMAL, 0);

        auto cursor = Graphics::CreateText(graphics, "_", Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL, 0);

        std::string input_text = std::string();

        if (message)
        {
            SDL_Texture *input_texture = nullptr;

            auto pad = 16;

            // setup text input mode
            auto input = Controls::User();

            input.TextLimit = input_limit;

            input.SetText(start_text);

            input.RefreshText = (input.TextInput.size() > 0);

            if (input.TextInput.size() > 0)
            {
                input_texture = Graphics::CreateText(graphics, input.TextInput.c_str(), Fonts::Normal, Color::S(input_color), TTF_STYLE_NORMAL, 0);
            }

            input.Text = true;

            // cursor blink
            auto blink = false;

            // enable text input events
            SDL_StartTextInput();

            while (true)
            {
                auto box = Scene::Base();

                box.Add(Scene::Element(location, box_w, box_h, box_bg, border, border_size));

                box.VerifyAndAdd(Scene::Element(message, location + Point(pad, pad)));

                if (input_texture)
                {
                    box.VerifyAndAdd(Scene::Element(input_texture, location + Point(pad, pad * 4)));
                }

                // add cursor blink
                if (blink && input.TextInput.size() < input.TextLimit)
                {
                    auto texture_w = 0;

                    if (input_texture)
                    {
                        SDL_QueryTexture(input_texture, nullptr, nullptr, &texture_w, nullptr);
                    }

                    box.VerifyAndAdd(Scene::Element(cursor, location + Point(pad + texture_w, pad * 4)));
                }

                blink = !blink;

                input = Input::WaitForInput(graphics, background, box, input, true, blur, 100);

                if (input.RefreshText)
                {
                    Free(&input_texture);

                    if (input.TextInput.size() > 0)
                    {
                        input_texture = Graphics::CreateText(graphics, input.TextInput.c_str(), Fonts::Normal, Color::S(input_color), TTF_STYLE_NORMAL, 0);
                    }
                }
                else if (input.Selected)
                {
                    break;
                }
            }

            // disable text input events
            SDL_StopTextInput();

            Free(&input_texture);

            Free(&cursor);

            Free(&message);

            input_text = input.TextInput;
        }

        return input_text;
    }

    std::string TextInput(Graphics::Base &graphics, Scene::Base &background, std::string question, int input_limit, int box_w, int box_h, bool blur = true)
    {
        auto location = (Point(graphics.Width, graphics.Height) - Point(box_w, box_h)) / 2;

        return Interface::TextInput(graphics, background, location, question, "", Color::Inactive, Color::Active, input_limit, box_w, box_h, Color::Active, Color::Background, 4, blur);
    }

    std::string TextInput(Graphics::Base &graphics, Scene::Base &background, std::string question, int box_w, int box_h, bool blur = true)
    {
        return Interface::TextInput(graphics, background, question, 20, box_w, box_h, blur);
    }

    std::string TextInput(Graphics::Base &graphics, Scene::Base &background, std::string question, bool blur = true)
    {
        auto box_w = (question.size() > 16) ? int(question.size() * 16) : 320;

        return Interface::TextInput(graphics, background, question, box_w, 120, blur);
    }

    std::string TextInput(Graphics::Base &graphics, std::string question, bool blur = true)
    {
        auto background = Scene::Base();

        return Interface::TextInput(graphics, background, question, blur);
    }
}

#endif
