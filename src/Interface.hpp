#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include "loggers/Interface.hpp"

namespace fs = std::filesystem;

namespace BloodSword::Interface
{
    std::string SettingsFile = std::string();

    nlohmann::json Settings;

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

    Controls::Mapped<SDL_Texture *> BattleControlCaptions = {};

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
        {Character::Class::ENCHANTER, Controls::Type::ENCHANTER},
        {Character::Class::IMRAGARN, Controls::Type::IMRAGARN}};

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

    BloodSword::UnorderedMap<Character::Class, Asset::Type> ClassAssets = {
        {Character::Class::NONE, Asset::Type::NONE},
        {Character::Class::WARRIOR, Asset::Type::WARRIOR},
        {Character::Class::TRICKSTER, Asset::Type::TRICKSTER},
        {Character::Class::SAGE, Asset::Type::SAGE},
        {Character::Class::ENCHANTER, Asset::Type::ENCHANTER},
        {Character::Class::IMRAGARN, Asset::Type::PERSON}};

    // SKILL to STATUS mapping
    BloodSword::UnorderedMap<Skills::Type, Character::Status> SkillEffects = {
        {Skills::Type::NONE, Character::Status::NONE},
        {Skills::Type::QUARTERSTAFF, Character::Status::KNOCKED_OUT},
        {Skills::Type::PARALYZING_TOUCH, Character::Status::PARALYZED},
        {Skills::Type::POISONED_DAGGER, Character::Status::POISONED}};

    // SKILL to CONTROL mapping
    BloodSword::UnorderedMap<Skills::Type, Controls::Type> ActionControls = {
        {Skills::Type::NONE, Controls::Type::NONE},
        {Skills::Type::ARCHERY, Controls::Type::SHOOT},
        {Skills::Type::SHURIKEN, Controls::Type::SHURIKEN},
        {Skills::Type::QUARTERSTAFF, Controls::Type::QUARTERSTAFF},
        {Skills::Type::SPELLS, Controls::Type::SPELLS}};

    // SPELL to STATUS mapping
    BloodSword::UnorderedMap<Spells::Type, Character::Status> SpellEffects = {
        {Spells::Type::NONE, Character::Status::NONE},
        {Spells::Type::NIGHTHOWL, Character::Status::NIGHTHOWL}};

    SDL_Texture *NoSkills = nullptr;

    SDL_Texture *NoSpells = nullptr;

    // dice assets
    Asset::List Dice = {
        Asset::Type::DICE1,
        Asset::Type::DICE2,
        Asset::Type::DICE3,
        Asset::Type::DICE4,
        Asset::Type::DICE5,
        Asset::Type::DICE6};

    Asset::List Numbers = {
        Asset::Type::ZERO,
        Asset::Type::ONE,
        Asset::Type::TWO,
        Asset::Type::THREE,
        Asset::Type::FOUR,
        Asset::Type::FIVE,
        Asset::Type::SIX,
        Asset::Type::SEVEN,
        Asset::Type::EIGHT,
        Asset::Type::NINE};

    const int BoxSize = 16;

    const int MaxBoxRow = 16;

    // struct to hold results of using/consuming an item
    struct ItemResult
    {
        bool Update = false;

        Book::Location Next = Book::Undefined;
    };

    // struct to specify one damage gauntlet round
    struct Gauntlet
    {
        int Rounds = 1;

        int Rolls = 2;

        int RollModifier = 0;

        int Difficulty = 2;

        int Damage = 2;

        int DamageModifier = 0;

        int IgnoreArmour = false;
    };

    // class to represent saved game on save game menu
    class SavedGame
    {
    public:
        int Number = 0;

        std::string Filename;

        std::string TimeStamp = std::string();

        std::vector<Character::Class> Players = {};

        Book::Location Location = Book::Undefined;

        bool Completed = false;

        SavedGame() {};
    };

    // save game slots
    const int MaxGameFiles = 4;

    std::vector<Interface::SavedGame> GamesList = {};

    class Module
    {
    public:
        std::string Id = std::string();

        std::string Title = std::string();

        std::string SettingsFile = std::string();
    };

    std::vector<Interface::Module> Modules = {};

    // initialize settings from file
    void Initialize(const char *settings)
    {
        std::ifstream ifs(settings);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            if (!data["settings"].is_null() && data["settings"].is_object() && data["settings"].size() > 0)
            {
                Interface::Settings = data["settings"];
            }

            ifs.close();
        }
        else
        {
            throw std::invalid_argument("Unable to load settings!");
        }
    }

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

        // initialize messages
        Interface::InitializeMessages(graphics);
    }

    // unload all textures allocated by this module
    void FreeTextures()
    {
        BloodSword::Free(SkillCaptionsActive);

        BloodSword::Free(SkillCaptionsInactive);

        BloodSword::Free(SkillsTexturesInactive);

        BloodSword::Free(SpellCaptionsActive);

        BloodSword::Free(SpellCaptionsInactive);

        BloodSword::Free(SpellsTexturesInactive);

        BloodSword::Free(BattleControlCaptions);

        BloodSword::Free(&NoSkills);

        BloodSword::Free(&NoSpells);

        Interface::FreeMessages();
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
        Asset::Load(graphics.Renderer, Interface::Settings["assets"]);

        Graphics::InitializeTextures(graphics);

        Interface::InitializeTextures(graphics);
    }

    // switch texture and reload all textures
    void ReloadTextures(Graphics::Base &graphics, int palette = 0, bool override = true)
    {
        Interface::UnloadTextures();

        Palette::Switch(palette, override);

        Interface::LoadTextures(graphics);

        InitializeMessages(graphics);
    }

    // load settings from file
    void LoadSettings(Graphics::Base &graphics, std::string settings_file)
    {
        // game settings
        Interface::Initialize(settings_file.c_str());

        // load sound assets
        Sound::Load(Interface::Settings["sounds"]);

        // load fonts
        Fonts::Load(Interface::Settings["fonts"]);

        // palette definitions
        Palette::Load(Interface::Settings["palettes"]);

        // set default palette
        Palette::Switch(int(Interface::Settings["palette"]));

        // load textures
        Interface::LoadTextures(graphics);

        // initialize gamepads
        Input::InitializeGamePads();

        // load item defaults
        Items::LoadDefaults(Interface::Settings["items"]);

        // load item descriptions
        Items::LoadDescriptions(Interface::Settings["item_descriptions"]);

        // load characters
        Party::Characters = Party::Load(Interface::Settings["characters"], "characters");
    }

    // unload texture assets
    void UnloadAssets()
    {
        Sound::Free();

        Fonts::Free();

        Interface::UnloadTextures();
    }

    // reload settings
    void ReloadSettings(Graphics::Base &graphics, std::string settings_file)
    {
        Interface::UnloadAssets();

        Interface::LoadSettings(graphics, settings_file);
    }

    void Boxed(Graphics::Base &graphics, Scene::Base &scene, SDL_Texture *texture, Point box_location, Uint32 background, Uint32 border, int border_size)
    {
        auto texture_width = 0;

        auto texture_height = 0;

        BloodSword::Size(texture, &texture_width, &texture_height);

        auto box_width = std::max(BloodSword::QuadTile, texture_width) + BloodSword::LargePad;

        auto box_height = std::max(BloodSword::DoubleTile, texture_height + BloodSword::TriplePad);

        auto texture_location = box_location + (Point(box_width, box_height) - Point(texture_width, texture_height)) / 2;

        scene.Add(Scene::Element(box_location, box_width, box_height, background, border, border_size));

        scene.VerifyAndAdd(Scene::Element(texture, texture_location));
    }

    Scene::Base Boxed(Graphics::Base &graphics, SDL_Texture *texture, Point box_location, Uint32 background, Uint32 border, int border_size)
    {
        auto boxed = Scene::Base();

        Interface::Boxed(graphics, boxed, texture, box_location, background, border, border_size);

        return boxed;
    }

    Scene::Base Boxed(Graphics::Base &graphics, SDL_Texture *texture, Uint32 background, Uint32 border, int border_size)
    {
        auto texture_width = 0;

        auto texture_height = 0;

        BloodSword::Size(texture, &texture_width, &texture_height);

        auto box_width = std::max(BloodSword::QuadTile, texture_width) + BloodSword::LargePad;

        auto box_height = std::max(BloodSword::DoubleTile, texture_height + BloodSword::TriplePad);

        auto box_location = (Point(graphics.Width, graphics.Height) - Point(box_width, box_height)) / 2;

        return Interface::Boxed(graphics, texture, box_location, background, border, border_size);
    }

    void Boxed(Graphics::Base &graphics, Scene::Base &scene, SDL_Texture *texture, Uint32 background, Uint32 border, int border_size)
    {
        auto texture_width = 0;

        auto texture_height = 0;

        BloodSword::Size(texture, &texture_width, &texture_height);

        auto box_width = std::max(BloodSword::QuadTile, texture_width) + BloodSword::LargePad;

        auto box_height = std::max(BloodSword::DoubleTile, texture_height + BloodSword::TriplePad);

        auto box_location = (Point(graphics.Width, graphics.Height) - Point(box_width, box_height)) / 2;

        Interface::Boxed(graphics, scene, texture, box_location, background, border, border_size);
    }

    void FlashTexture(Graphics::Base &graphics, Scene::Base &scene, SDL_Texture *texture, Uint32 background, Uint32 border, int border_size, int delay = BloodSword::StandardDelay)
    {
        if (texture)
        {
            auto message = Interface::Boxed(graphics, texture, background, border, border_size);

            Input::RenderWhileWaiting(graphics, {scene, message});

            SDL_Delay(delay);
        }
    }

    void FlashMessage(Graphics::Base &graphics, Scene::Base &scene, std::string message, Uint32 background, Uint32 border, int border_size, int delay = BloodSword::StandardDelay)
    {
        auto texture = Graphics::CreateText(graphics, message.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        Interface::FlashTexture(graphics, scene, texture, background, border, border_size, delay);

        BloodSword::Free(&texture);
    }

    void FlashMessage(Graphics::Base &graphics, Scene::Base &scene, std::string message, Uint32 border)
    {
        return Interface::FlashMessage(graphics, scene, message, Color::Background, border, BloodSword::Border, BloodSword::TwoSeconds);
    }

    // draws a message box on screen
    void MessageBox(Graphics::Base &graphics, Scene::Base &scene, Point offset, int width, int height, SDL_Texture *message, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        auto box = Scene::Base();

        auto pad = BloodSword::QuarterTile;

        if (message)
        {
            auto texture_w = 0;

            auto texture_h = 0;

            BloodSword::Size(message, &texture_w, &texture_h);

            auto box_w = std::max(BloodSword::QuadTile, texture_w) + pad * 2;

            auto box_h = texture_h + pad * 3 + BloodSword::TileSize;

            auto location_box = offset + (Point(width, height) - Point(box_w, box_h)) / 2;

            auto location_txt = offset + (Point(width, height) - Point(texture_w, box_h)) / 2;

            auto confirm = location_box + Point(box_w / 2 - BloodSword::HalfTile, texture_h + pad * 2);

            auto input = Controls::User();

            box.Add(Scene::Element(location_box, box_w, box_h, background, border, border_size));

            box.VerifyAndAdd(Scene::Element(message, location_txt + Point(0, pad)));

            box.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CONFIRM), confirm));

            box.Add(Controls::Base(Controls::Type::CONFIRM, 0, 0, 0, 0, 0, confirm.X, confirm.Y, BloodSword::TileSize, BloodSword::TileSize, highlight));

            while (true)
            {
                input = Input::WaitForInput(graphics, scene, box, input, true, blur);

                if (Input::Check(input))
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

            BloodSword::Free(&texture);
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

            BloodSword::Free(&texture);
        }
    }

    // generic message box
    void MessageBox(Graphics::Base &graphics, Scene::Base &scene, std::string message, Uint32 border)
    {
        auto texture = Graphics::CreateText(graphics, message.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        if (texture)
        {
            Interface::MessageBox(graphics, scene, texture, Color::Background, border, BloodSword::Border, border == Color::Active ? Color::Highlight : Color::Active, true);

            BloodSword::Free(&texture);
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

            BloodSword::Free(&texture);
        }
    }

    // draws a message box
    void MessageBox(Graphics::Base &graphics, Scene::Base &scene, Point offset, int width, int height, const char *message, TTF_Font *font, SDL_Color color, int style, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        Interface::MessageBox(graphics, scene, offset, width, height, Graphics::RichText(std::string(message), font, color, style, 0), background, border, border_size, highlight, blur);
    }

    void ScrollUp(Scene::Base &overlay, Controls::User &input, Controls::Type control, int options, int limit, int &start, int &last)
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

            Controls::Select(input, overlay.Controls, control);
        }
    }

    void ScrollDown(Scene::Base &overlay, Controls::User &input, Controls::Type control, int options, int limit, int &start, int &last)
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

            Controls::Select(input, overlay.Controls, control);
        }
    }

    void ClearScrolling(Scene::Base &overlay, Controls::User &input, bool &scroll_up, bool &scroll_dn, Controls::Type up, Controls::Type down)
    {
        if (scroll_up)
        {
            Controls::Select(input, overlay.Controls, up);

            scroll_up = false;
        }
        else if (scroll_dn)
        {
            Controls::Select(input, overlay.Controls, down);

            scroll_dn = false;
        }
    }

    void ClearScrolling(Scene::Base &overlay, Controls::User &input, Controls::Type up, Controls::Type down)
    {
        Interface::ClearScrolling(overlay, input, input.Up, input.Down, up, down);
    }

    void TextUp(Scene::Base &overlay, Controls::User &input, Controls::Type control, bool &up, int &offset, int texture_h, int text_h, int speed)
    {
        if (text_h < texture_h)
        {
            offset -= speed;

            if (offset < 0)
            {
                offset = 0;
            }

            up = true;
        }

        Controls::Select(input, overlay.Controls, control);
    }

    void TextDown(Scene::Base &overlay, Controls::User &input, Controls::Type control, bool &down, int &offset, int texture_h, int text_h, int speed)
    {
        if (text_h < texture_h)
        {
            offset += speed;

            if (offset > (texture_h - text_h))
            {
                offset = texture_h - text_h;
            }

            down = true;
        }

        Controls::Select(input, overlay.Controls, control);
    }

    void AddScrollableTextureBox(Scene::Base &scene, int x, int y, int width, int height, Uint32 bg_color, Uint32 border, int border_size, SDL_Texture *texture, int texture_h, int text_x, int text_y, int text_h, int offset, int controls_x, int controls_y, Asset::Type asset, Asset::Type left, Asset::Type right, int scroll_speed)
    {
        auto id = int(scene.Controls.size());

        // texture box panel
        scene.Add(Scene::Element(Point(x, y), width, height, bg_color, border, border_size));

        // texture
        scene.VerifyAndAdd(Scene::Element(texture, text_x, text_y, text_h, offset));

        // scroll up (left)
        scene.VerifyAndAdd(Scene::Element(Asset::Get(left), controls_x, controls_y));

        scene.Add(Controls::Base(Controls::Type::LEFT, id, id, id + 1, id, id, controls_x, controls_y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

        if (offset <= 0)
        {
            // blur button
            scene.Add(Scene::Element(controls_x, controls_y, BloodSword::TileSize, BloodSword::TileSize, Color::Blur));
        }

        id++;

        // close texturebox
        scene.VerifyAndAdd(Scene::Element(Asset::Get(asset), controls_x + BloodSword::TileSize + BloodSword::Pad, controls_y));

        scene.Add(Controls::Base(Controls::Type::CONFIRM, id, id - 1, id + 1, id, id, controls_x + BloodSword::TileSize + BloodSword::Pad, controls_y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

        id++;

        // scroll down (right)
        scene.VerifyAndAdd(Scene::Element(Asset::Get(right), controls_x + BloodSword::DoubleTile + BloodSword::LargePad, controls_y));

        scene.Add(Controls::Base(Controls::Type::RIGHT, id, id - 1, id, id, id, controls_x + BloodSword::DoubleTile + BloodSword::LargePad, controls_y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

        if (text_h >= texture_h || (offset + scroll_speed) > (texture_h - text_h))
        {
            // blur button
            scene.Add(Scene::Element(controls_x + BloodSword::DoubleTile + BloodSword::LargePad, controls_y, BloodSword::TileSize, BloodSword::TileSize, Color::Blur));
        }
    }

    // draws a scrollable image box
    void ScrollableImageBox(Graphics::Base &graphics, Scene::Base &background, SDL_Texture *texture, int width, int height, int x, int y, Uint32 bg_color, Uint32 border, int border_size, Uint32 highlight, Asset::Type asset, Asset::Type left, Asset::Type right, bool blur = true, int offset = 0)
    {
        if (texture)
        {
            auto text_h = height - (BloodSword::TileSize + BloodSword::TriplePad);

            auto texture_h = BloodSword::Height(texture);

            auto text_x = x + BloodSword::Pad;

            auto text_y = y + BloodSword::Pad;

            auto input = Controls::User();

            auto controls_x = x + (width - (BloodSword::TripleTile + BloodSword::LargePad)) / 2;

            auto controls_y = y + text_h + BloodSword::LargePad;

            auto scroll_speed = BloodSword::ScrollSpeed;

            auto done = false;

            while (!done)
            {
                auto scene = Scene::Base();

                Interface::AddScrollableTextureBox(scene, x, y, width, height, bg_color, border, border_size, texture, texture_h, text_x, text_y, text_h, offset, controls_x, controls_y, asset, left, right, scroll_speed);

                input = Input::WaitForInput(graphics, {background, scene}, scene.Controls, input, blur);

                if (Input::Validate(input))
                {
                    if (input.Type == Controls::Type::LEFT || input.Up)
                    {
                        Interface::TextUp(scene, input, Controls::Type::LEFT, input.Up, offset, texture_h, text_h, scroll_speed);
                    }
                    else if (input.Type == Controls::Type::RIGHT || input.Down)
                    {
                        Interface::TextDown(scene, input, Controls::Type::RIGHT, input.Down, offset, texture_h, text_h, scroll_speed);
                    }
                    else if (input.Type == Controls::Type::CONFIRM)
                    {
                        done = true;
                    }

                    input.Selected = false;
                }
            }
        }
    }

    // draws a scrollable text box (multi-line)
    void ScrollableTextBox(Graphics::Base &graphics, Scene::Base &background, TTF_Font *font, std::string text, int width, int height, int x, int y, SDL_Color color, int style, Uint32 bg_color, Uint32 border, int border_size, Uint32 highlight, Asset::Type asset, bool blur = true)
    {
        auto texture = Graphics::CreateText(graphics, text.c_str(), font, color, style, width - BloodSword::LargePad);

        if (texture)
        {
            Interface::ScrollableImageBox(graphics, background, texture, width, height, x, y, bg_color, border, border_size, highlight, asset, Asset::Type::LEFT, Asset::Type::RIGHT, blur);

            BloodSword::Free(&texture);
        }
    }

    // draws a scrollable text box (multi-line)
    void ScrollableTextBox(Graphics::Base &graphics, Scene::Base &background, TTF_Font *font, std::string text, int width, int height, SDL_Color color, int style, Uint32 bg_color, Uint32 border, int border_size, Uint32 highlight, Asset::Type asset, bool blur = true)
    {
        auto x = (graphics.Width - width) / 2;

        auto y = (graphics.Height - height) / 2;

        Interface::ScrollableTextBox(graphics, background, font, text, width, height, x, y, color, style, bg_color, border, border_size, highlight, asset, blur);
    }

    // draws a text box (multi-line)
    void TextBox(Graphics::Base &graphics, Scene::Base &scene, TTF_Font *font, const char *message, int wrap, SDL_Color color, int style, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        auto texture = Graphics::CreateText(graphics, message, font, color, style, wrap);

        if (texture)
        {
            auto text_w = 0;

            auto text_h = 0;

            BloodSword::Size(texture, &text_w, &text_h);

            text_w += BloodSword::LargePad;

            text_h += BloodSword::TriplePad + BloodSword::TileSize;

            auto origin = Point(graphics.Width - text_w, graphics.Height - text_h) / 2;

            Interface::MessageBox(graphics, scene, origin, text_w, text_h, texture, background, border, border_size, highlight, blur);

            BloodSword::Free(&texture);
        }
    }

    // generate message textbox
    void TextBox(Graphics::Base &graphics, Scene::Base &scene, const char *message, int wrap, bool blur = true)
    {
        Interface::TextBox(graphics, scene, Fonts::Normal, message, wrap, Color::S(Color::Active), TTF_STYLE_NORMAL, Color::Background, Color::Active, BloodSword::Border, Color::Active, blur);
    }

    // generate message textbox
    void TextBox(Graphics::Base &graphics, Scene::Base &scene, const char *message, Uint32 border, int wrap, bool blur = true)
    {
        Interface::TextBox(graphics, scene, Fonts::Normal, message, wrap, Color::S(Color::Active), TTF_STYLE_NORMAL, Color::Background, border, BloodSword::Border, Color::Active, blur);
    }

    // generate message textbox
    void TextBox(Graphics::Base &graphics, Scene::Base &scene, std::string message, Uint32 border, int wrap, bool blur = true)
    {
        Interface::TextBox(graphics, scene, message.c_str(), border, wrap, blur);
    }

    // generate message textbox
    void TextBox(Graphics::Base &graphics, Scene::Base &scene, std::string message, int wrap, bool blur = true)
    {
        Interface::TextBox(graphics, scene, message, Color::Active, wrap, blur);
    }

    // add map to the scene
    Scene::Base Map(Map::Base &map, Party::Base &party, Party::Base &enemies, int num_bottom_buttons)
    {
        auto scene = Scene::Base();

        auto num_controls = 0;

        // add background frame
        scene.Add(Scene::Element(map.DrawX, map.DrawY, map.ViewX * map.TileSize, map.ViewY * map.TileSize, Color::Background, Color::Active, 1));

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
                        if (tile.Id >= 0 && tile.Id < party.Count() && party.Count() > 0)
                        {
                            if (Engine::IsAlive(party[tile.Id]))
                            {
                                scene.VerifyAndAdd(Scene::Element(Asset::Get(party[tile.Id].Asset), screen));

                                if (BloodSword::Has(Interface::CharacterControls, party[tile.Id].Class))
                                {
                                    type = Interface::CharacterControls[party[tile.Id].Class];
                                }

                                color = Color::Active;
                            }
                        }
                        break;
                    case Map::Object::ENEMY:
                        if (tile.Id >= 0 && tile.Id < enemies.Count() && enemies.Count() > 0)
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

                                color = Color::Inactive;
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

                        color = Color::Active;
                    }
                    else if (tile.IsPassable())
                    {
                        type = Controls::Type::LOCATION;

                        color = Color::Active;
                    }
                    else if (tile.IsPassableToEnemy())
                    {
                        color = Color::Highlight;
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

    // generate map overlay and reserve a number of buttons at the bottom
    Scene::Base Map(Map::Base &map, int num_bottom_buttons)
    {
        auto party = Party::Base();

        auto enemies = Party::Base();

        // num_bottom_buttons is the number of buttons below the map
        return Interface::Map(map, party, enemies, num_bottom_buttons);
    }

    // form attribute score string
    std::string ScoreString(Character::Base &character, Attribute::Type attribute, bool in_battle = false, Item::Property weapon = Item::Property::NONE)
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
            if (in_battle)
            {
                if (character.IsPlayer())
                {
                    modifier += !character.IsArmed() ? -2 : 0;

                    modifier += character.Has(Character::Status::FPR_PLUS2) ? 2 : 0;

                    modifier += character.Has(Character::Status::FPR_PLUS1) ? 1 : 0;
                }
                else if (character.IsEnemy() && character.Fight == Skills::Type::BROKEN_WEAPON)
                {
                    modifier -= 2;
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
            if (attribute == Attribute::Type::FIGHTING_PROWESS && in_battle)
            {
                if (character.IsPlayer())
                {
                    if (character.Fight == Skills::Type::NONE)
                    {
                        modifier += !character.IsArmed() ? -2 : 0;
                    }

                    modifier += character.Has(Character::Status::FPR_PLUS2) ? 2 : 0;

                    modifier += character.Has(Character::Status::FPR_PLUS1) ? 1 : 0;

                    modifier -= character.Has(Character::Status::BURNED) ? 1 : 0;
                }
                else if (character.IsEnemy() && character.Fight == Skills::Type::BROKEN_WEAPON)
                {
                    modifier -= 2;
                }

                if (character.Has(Character::Status::FIGHTING_BLIND))
                {
                    value = 1;

                    modifier = 0;
                }
            }

            if (attribute == Attribute::Type::FIGHTING_PROWESS || attribute == Attribute::Type::AWARENESS)
            {
                if (character.Has(Character::Status::OSTEAL_WEAKNESS))
                {
                    modifier -= 1;
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
        Graphics::Estimate(font, "DMGG ", &labels_w, nullptr);

        Graphics::Estimate(font, "9999D+D9999", &stats_w, nullptr);

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

            auto surface_width = labels_w + stats_w;

            if (add_name)
            {
                auto name_width = 0;

                Graphics::Estimate(font, character.Name.c_str(), &name_width, nullptr);

                if (name_width > surface_width)
                {
                    surface_width = name_width;
                }
            }

            // create surface to combine labels and stats
            surface = Graphics::CreateSurface(surface_width, std::max(surface_labels->h, surface_stats->h));

            if (surface)
            {
                SDL_Rect labels_rect, stats_rect;

                labels_rect.w = surface->w;

                labels_rect.h = surface->h;

                labels_rect.x = 0;

                labels_rect.y = 0;

                stats_rect.w = surface->w;

                stats_rect.h = surface->h;

                stats_rect.x = surface_labels->w + BloodSword::Pad;

                stats_rect.y = 0;

                Graphics::RenderAsset(surface, surface_labels, labels_rect);

                Graphics::RenderAsset(surface, surface_stats, stats_rect);

                // add character class if player character
                if (add_name)
                {
                    auto surface_name = Graphics::CreateSurfaceText(character.Name.c_str(), font, Color::S(stats_color), style | TTF_STYLE_UNDERLINE, surface_width);

                    if (surface_name)
                    {
                        Graphics::RenderAsset(surface, surface_name, labels_rect);

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
    BloodSword::Textures Attributes(Graphics::Base &graphics, Party::Base &party, TTF_Font *font, Uint32 label_color, Uint32 stats_color, int style, int wrap, bool add_name = false, bool in_battle = false)
    {
        BloodSword::Textures textures = {};

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
    BloodSword::Textures GenerateStats(Graphics::Base &graphics, Party::Base &party, int width, bool names = true, bool in_battle = true)
    {
        return Interface::Attributes(graphics, party, Fonts::Normal, Color::Active, Color::Highlight, TTF_STYLE_NORMAL, width, names, in_battle);
    }

    // generate textures of character's class'
    BloodSword::Textures GenerateCharacterClassCaptions(Graphics::Base &graphics, Party::Base &party)
    {
        Graphics::TextList characters = {};

        for (auto character = 0; character < party.Count(); character++)
        {
            auto alive = Engine::Score(party[character], Attribute::Type::ENDURANCE) > 0;

            characters.push_back(Graphics::RichText(Character::ClassMapping[party[character].Class], Fonts::Caption, alive ? Color::Active : Color::Inactive, TTF_STYLE_NORMAL, BloodSword::FrameHeight));
        }

        return Graphics::CreateText(graphics, characters);
    }

    // generate textures of character's names
    BloodSword::Textures GenerateNameCaptions(Graphics::Base &graphics, Party::Base &party)
    {
        Graphics::TextList characters = {};

        for (auto character = 0; character < party.Count(); character++)
        {
            auto alive = Engine::Score(party[character], Attribute::Type::ENDURANCE) > 0;

            characters.push_back(Graphics::RichText(party[character].Name.c_str(), Fonts::Caption, alive ? Color::Active : Color::Inactive, TTF_STYLE_NORMAL, BloodSword::FrameHeight));
        }

        return Graphics::CreateText(graphics, characters);
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
    BloodSword::Textures Skills(Graphics::Base &graphics, Party::Base &party, TTF_Font *font, Uint32 skills_color, int style, int wrap)
    {
        BloodSword::Textures textures = {};

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

        Graphics::Estimate(font, "9999D+D9999", &stats_w, nullptr);

        for (auto &status : character.Status)
        {
            if (status.second != 0)
            {
                auto current_status = status.first;

                auto duration = status.second;

                if (((in_battle && current_status != Character::Status::IN_BATTLE) || !in_battle) && !BloodSword::In(Character::GlobalStatus, current_status) && !BloodSword::In(Character::InvisibleStatus, current_status))
                {
                    if (list.length() > 0)
                    {
                        list += '\n';
                    }

                    list += Character::StatusMapping[current_status];

                    if (duration > 0)
                    {
                        list += " (" + std::to_string(duration) + ")";
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
    BloodSword::Textures Status(Graphics::Base &graphics, Party::Base &party, TTF_Font *font, Uint32 label_color, int style, bool in_battle = false)
    {
        BloodSword::Textures textures = {};

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

    // generate texture of character status (basic)
    SDL_Texture *BasicStatus(Graphics::Base &graphics, Character::Base &character, TTF_Font *font, Uint32 color, int style)
    {
        SDL_Texture *texture = nullptr;

        std::string list;

        auto labels_w = 0;

        auto stats_w = 0;

        // estimate maximum line width
        Graphics::Estimate(font, "DMGG ", &labels_w, nullptr);

        Graphics::Estimate(font, "9999D+D9999", &stats_w, nullptr);

        for (auto &status : character.Status)
        {
            if (status.second != 0)
            {
                auto current_status = status.first;

                auto duration = status.second;

                if (!BloodSword::In(Character::GlobalStatus, current_status) && !BloodSword::In(Character::InvisibleStatus, current_status))
                {
                    if (list.length() > 0)
                    {
                        list += '\n';
                    }

                    list += Character::StatusMapping[current_status];

                    if (duration > 0)
                    {
                        list += " (" + std::to_string(duration) + ")";
                    }
                }
            }
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
    BloodSword::Textures BasicStatus(Graphics::Base &graphics, Party::Base &party, TTF_Font *font, Uint32 label_color, int style)
    {
        BloodSword::Textures textures = {};

        for (auto i = 0; i < party.Count(); i++)
        {
            auto &character = party[i];

            auto texture = Interface::BasicStatus(graphics, character, font, label_color, style);

            if (texture)
            {
                textures.push_back(texture);
            }
        }

        return textures;
    }

    // attribute labels
    std::string StatsLabels()
    {
        // attribute labels
        auto label_1 = std::string("RNK    FPR        END        EXP ");

        auto label_2 = std::string("GOLD   AWR        DMG            ");

        auto label_3 = std::string("       PSY        ARM            ");

        auto labels = label_1 + '\n' + label_2 + '\n' + label_3;

        return labels;
    }

    // generate texture of the character's stats
    SDL_Surface *GenerateCharacterStats(Graphics::Base &graphics, Character::Base &character, int w)
    {
        auto surface_labels = TTF_RenderUTF8_Blended_Wrapped(Fonts::Normal, StatsLabels().c_str(), Color::S(Color::Active), 0);

        // surface
        SDL_Surface *surface = nullptr;

        if (surface_labels)
        {
            auto labels_h = std::max(BloodSword::TileSize, surface_labels->h + BloodSword::Pad);

            surface = Graphics::CreateSurface(w, labels_h);

            if (surface)
            {
                // retrieve attribute values
                auto rnk = std::to_string(character.Rank);

                auto fpr = Interface::ScoreString(character, Attribute::Type::FIGHTING_PROWESS);

                auto awr = Interface::ScoreString(character, Attribute::Type::AWARENESS);

                auto psy = Interface::ScoreString(character, Attribute::Type::PSYCHIC_ABILITY);

                auto end = Interface::ScoreString(character, Attribute::Type::ENDURANCE);

                auto dmg = Interface::ScoreString(character, Attribute::Type::DAMAGE);

                auto arm = Interface::ScoreString(character, Attribute::Type::ARMOUR);

                auto exp = std::to_string(character.Experience);

                auto gold = std::to_string(character.Quantity(Item::Type::GOLD));

                auto four = std::string(4, ' ');

                // format attribute values
                auto stats_1 = four + rnk + std::string(3 - rnk.size(), ' ') + four + fpr + std::string(7 - fpr.size(), ' ') + four + end + '\n';

                auto stats_2 = std::string(11, ' ') + awr + std::string(7 - awr.size(), ' ') + four + dmg + std::string(7 - dmg.size(), ' ') + exp + '\n';

                auto stats_3 = gold + std::string(11 - gold.size(), ' ') + psy + std::string(7 - psy.size(), ' ') + four + arm;

                auto stats = stats_1 + stats_2 + stats_3;

                // create and convert SDL surface to appropriate format
                auto surface_stats = TTF_RenderUTF8_Blended_Wrapped(Fonts::Normal, stats.c_str(), Color::S(Color::Highlight), 0);

                SDL_Rect stats_rect;

                stats_rect.w = surface->w;

                stats_rect.h = surface->h;

                stats_rect.x = BloodSword::TileSize + BloodSword::Pad;

                stats_rect.y = 0;

                Graphics::RenderAsset(surface, surface_labels, stats_rect);

                Graphics::RenderAsset(surface, surface_stats, stats_rect);

                // add icon (blur if dead)
                auto surface_asset = Engine::Score(character, Attribute::Type::ENDURANCE) > 0 ? Asset::Surface(character.Asset) : Asset::Surface(character.Asset, Color::Inactive);

                stats_rect.x = 0;

                Graphics::RenderAsset(surface, surface_asset, stats_rect);

                // cleanup
                BloodSword::Free(&surface_labels);

                BloodSword::Free(&surface_stats);

                BloodSword::Free(&surface_asset);
            }

            // cleanup
            BloodSword::Free(&surface_labels);
        }

        return surface;
    }

    // generate texture of the entire party's stats
    SDL_Texture *GeneratePartyStats(Graphics::Base &graphics, Party::Base &party, int w)
    {
        // final texture
        SDL_Texture *texture = nullptr;

        auto surface_labels = TTF_RenderUTF8_Blended_Wrapped(Fonts::Normal, StatsLabels().c_str(), Color::S(Color::Active), 0);

        if (surface_labels)
        {
            auto labels_h = std::max(BloodSword::TileSize, surface_labels->h + BloodSword::Pad);

            // final surface
            auto surface = Graphics::CreateSurface(w, labels_h * party.Count());

            if (surface)
            {
                SDL_Rect stats_rect;

                stats_rect.w = surface->w;

                stats_rect.h = surface->h;

                for (auto i = 0; i < party.Count(); i++)
                {
                    auto character_surface = Interface::GenerateCharacterStats(graphics, party[i], w);

                    if (character_surface)
                    {
                        stats_rect.x = 0;

                        stats_rect.y = i * labels_h;

                        Graphics::RenderAsset(surface, character_surface, stats_rect);

                        // cleanup
                        BloodSword::Free(&character_surface);
                    }
                }

                // create final texture
                texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

                // cleanup
                BloodSword::Free(&surface);
            }

            // cleanup
            BloodSword::Free(&surface_labels);
        }

        return texture;
    }

    // generate texture of character stats
    SDL_Texture *CharacterStats(Graphics::Base &graphics, Character::Base &character, int w)
    {
        SDL_Texture *texture = nullptr;

        auto surface = Interface::GenerateCharacterStats(graphics, character, w);

        if (surface)
        {
            texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

            BloodSword::Free(&surface);
        }

        return texture;
    }

    // generate textures of party stats
    BloodSword::Textures PartyStats(Graphics::Base &graphics, Party::Base &party, int w)
    {
        auto textures = std::vector<SDL_Texture *>();

        for (auto character = 0; character < party.Count(); character++)
        {
            auto texture = Interface::CharacterStats(graphics, party[character], w);

            textures.push_back(texture);
        }

        return textures;
    }

    // generate texture of character
    SDL_Texture *CharacterBackgrounds(Graphics::Base &graphics, Character::Base &character, int w)
    {
        Graphics::RichText text;

        switch (character.Class)
        {
        case Character::Class::WARRIOR:
            text = Graphics::RichText("You are a master of the fighting arts. You have better Fighting Prowess than any other character type, and when you strike a blow, you inflict more damage.\n\nThese advantages give you a real edge in any fight but you have none of the others' special skills. Also, because you follow the honourable traditions of your class, you must be careful to stay true to the code of chivalry.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, w);
            break;
        case Character::Class::TRICKSTER:
            text = Graphics::RichText("Some adventurers are honourable and prefer to face their foes in a straight fight. You live by your wits. If you can win by trickery or by shooting someone in the back, you will. You know how to wield a sword if you have to, but your main weapon is cunning.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, w);
            break;
        case Character::Class::SAGE:
            text = Graphics::RichText("Your upbringing has been in the spartan Monastery of Illumination on the barren island of Kaxos. There, you have studied the Mystic Way, a series of demanding spiritual disciplines combined with rigorous physical training.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, w);
            break;
        case Character::Class::ENCHANTER:
            text = Graphics::RichText("Forget the mundane arts of swordplay. You know that true power lies in the manipulation of occult powers of sorcery.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, w);
            break;
        case Character::Class::IMRAGARN:
            text = Graphics::RichText("You have been frozen in ice for almost a decade, since you entered the Battlepits with several companions as the champion of Magus Laglor.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, w);
            break;
        default:
            text = Graphics::RichText("You have only one goal going into the Battlepits: emerge with the Emblem of Victory or die trying ...", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, w);
            break;
        }

        return Graphics::CreateText(graphics, text);
    }

    // generate textures of character backgrounds
    BloodSword::Textures PartyBackgrounds(Graphics::Base &graphics, Party::Base &party, int w)
    {
        auto textures = std::vector<SDL_Texture *>();

        for (auto character = 0; character < party.Count(); character++)
        {
            auto texture = Interface::CharacterBackgrounds(graphics, party[character], w);

            textures.push_back(texture);
        }

        return textures;
    }

    // generate character items list
    SDL_Texture *Items(Graphics::Base &graphics, Character::Base &character, TTF_Font *font, Uint32 items_color, int style, int wrap)
    {
        std::string items;

        if (character.Items.size() > 0)
        {
            for (auto &item : character.Items)
            {
                if (items.length() > 0)
                {
                    items += "\n";
                }

                items += item.Name;
            }
        }
        else
        {
            items = "NONE";
        }

        auto texture = Graphics::CreateText(graphics, items.c_str(), font, Color::S(items_color), style, wrap);

        return texture;
    }

    // create party items text box collection
    BloodSword::Textures Items(Graphics::Base &graphics, Party::Base &party, TTF_Font *font, Uint32 items_color, int style, int wrap)
    {
        BloodSword::Textures textures = {};

        for (auto i = 0; i < party.Count(); i++)
        {
            auto &character = party[i];

            auto texture = Interface::Items(graphics, character, font, items_color, style, wrap);

            if (texture)
            {
                textures.push_back(texture);
            }
        }

        return textures;
    }

    // add vertical text menu to the scene
    Scene::Base Menu(BloodSword::Textures &choices, int x, int y, int w, int h, int start, int last, int limit, Uint32 background, Uint32 border, Uint32 highlight, bool others = false, bool center = false)
    {
        auto scene = Scene::Base();

        if (!choices.empty())
        {
            auto start_id = 0;

            auto end = last - start;

            auto options = int(choices.size());

            auto more = options - last > 0;

            auto scroll = start_id + (options < limit ? options : limit);

            auto pixels = BloodSword::Pixel;

            auto offset = pixels * 2;

            auto adjust = pixels * 4;

            auto pad = pixels * 6;

            auto bars = options > limit;

            auto scroll_up = bars && start > 0;

            auto scroll_dn = bars && more;

            auto dim = BloodSword::TileSize;

            auto dim_adjust = dim + adjust;

            auto y_adjust = (limit - 1) * (h + pad) + (h - dim);

            auto w_adjust = w + adjust;

            auto x_offset = x + w_adjust;

            auto y_offset = y - offset;

            auto x_pad = x + w + pad;

            auto max_item_w = BloodSword::Width(choices[start]);

            for (auto item = 0; item < end; item++)
            {
                max_item_w = std::max(max_item_w, BloodSword::Width(choices[start + item]) + adjust);
            }

            for (auto item = 0; item < end; item++)
            {
                auto id = start_id + item;

                auto rt = scroll_up || scroll_dn ? (item == end - 1 && scroll_dn ? (scroll_up ? scroll + 1 : scroll) : scroll) : id;

                auto up = (item > 0 ? id - 1 : id);

                auto dn = item < end - 1 ? id + 1 : (others ? (scroll_up || scroll_dn ? (scroll_up && scroll_dn ? scroll + 2 : scroll + 1) : id + 1) : (scroll_up && scroll_dn ? scroll + 2 : (scroll_dn ? scroll : id)));

                auto item_y = y + item * (h + pad);

                // background
                scene.Add(Scene::Element(x, item_y, w, h, background, border, pixels));

                auto center_x = center ? ((w + (scroll ? (BloodSword::TileSize + pad) : 0)) - (BloodSword::Width(choices[start + item]))) / 2 : 0;

                auto center_y = center ? (h - BloodSword::Height(choices[start + item])) / 2 : 0;

                auto item_x = x - offset;

                if (center && scroll)
                {
                    w_adjust = max_item_w + adjust;

                    item_x = x + ((w + (scroll ? (BloodSword::TileSize + pad) : 0)) - max_item_w) / 2;
                }

                scene.VerifyAndAdd(Scene::Element(choices[start + item], x + center_x, item_y + center_y));

                scene.Add(Controls::Base(Controls::Type::CHOICE, id, id, rt, up, dn, center ? item_x - offset : item_x, item_y - offset, w_adjust, h + adjust, highlight));
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
    void HorizontalMenu(Scene::Base &overlay, BloodSword::Textures &choices, Controls::List controls, int x, int y, Uint32 background, Uint32 border, Uint32 highlight)
    {
        if (!choices.empty() && !controls.empty() && choices.size() == controls.size())
        {
            auto pixels = BloodSword::Pixel;

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
                    BloodSword::Size(choices[item], &current_w, &current_h);
                }

                auto previous_w = (item > 0 && choices[item - 1]) ? BloodSword::Width(choices[item - 1]) : 0;

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

        auto popup_w = (std::max(int(character.Skills.size()), 2) + (BloodSword::SmallPad / 2)) * BloodSword::TileSize;

        auto popup_h = BloodSword::FrameHeight;

        auto screen = origin + Point(w - popup_w, h - popup_h) / 2;

        overlay.Add(Scene::Element(screen, popup_w, popup_h, background, border, border_size));

        auto pad = BloodSword::QuarterTile;

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
                    auto texture_w = BloodSword::Width(texture);

                    auto lt = i > 0 ? i - 1 : i;

                    auto rt = i < character.Skills.size() ? i + 1 : i;

                    overlay.Add(Controls::Base(Interface::SkillControls[character.Skills[i]], i, lt, rt, i, i, screen.X + i * texture_w + pad, screen.Y + pad + BloodSword::HalfTile, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

                    overlay.VerifyAndAdd(Scene::Element(texture, screen.X + i * texture_w + pad, screen.Y + pad + BloodSword::HalfTile));
                }
            }
        }
        else
        {
            overlay.VerifyAndAdd(Scene::Element(NoSkills, screen + Point(pad, pad / 2)));
        }

        auto id = int(character.Skills.size());

        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), screen.X + character.Skills.size() * BloodSword::TileSize + pad, screen.Y + pad + BloodSword::HalfTile));

        overlay.Add(Controls::Base(Controls::Type::BACK, id, id > 0 ? id - 1 : id, id, id, id, screen.X + id * BloodSword::TileSize + pad, screen.Y + pad + BloodSword::HalfTile, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

        return overlay;
    }

    // spells overlay menu
    Scene::Base Spells(Point origin, int w, int h, Character::Base &character, Uint32 background, Uint32 border, int border_size, bool in_battle = false)
    {
        auto spells = int(character.Spells.size());

        auto overlay = Scene::Base();

        auto rows = std::max(spells / 6, 1);

        auto popup_w = BloodSword::HalfTile * 19;

        auto popup_h = (rows + 1) * (BloodSword::TileSize + BloodSword::HalfTile) + BloodSword::TileSize;

        auto screen = origin + Point(w - popup_w, h - popup_h) / 2;

        overlay.Add(Scene::Element(screen, popup_w, popup_h, background, border, border_size));

        auto pad = BloodSword::QuarterTile;

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
                    auto lt = col > 0 ? i - 1 : i;

                    auto rt = col < 5 ? i + 1 : i;

                    auto up = i - 6 >= 0 ? i - 6 : i;

                    auto dn = i + 6 < (spells + 1) ? i + 6 : i;

                    auto x = screen.X + col * BloodSword::Width(texture) + pad;

                    auto y = screen.Y + row * (BloodSword::Width(texture) + BloodSword::HalfTile) + pad + BloodSword::HalfTile;

                    overlay.Add(Controls::Base(Interface::SpellControls[character.Spells[i].Type], i, lt, rt, up, dn, x, y, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

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

        auto x = screen.X + col * BloodSword::TileSize + pad;

        auto y = screen.Y + row * (BloodSword::TileSize + BloodSword::HalfTile) + pad + BloodSword::HalfTile;

        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), x, y));

        overlay.Add(Controls::Base(Controls::Type::BACK, id, id - 1, id, col < 6 ? id - 6 : id, id, x, y, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

        return overlay;
    }

    // spells overlay menu
    Scene::Base Spells(Graphics::Base &graphics, Character::Base &character, Uint32 background, Uint32 border, int border_size, bool in_battle = false)
    {
        return Interface::Spells(Point(0, 0), graphics.Width, graphics.Height, character, background, border, border_size, in_battle);
    }

    Spells::Type GetSpell(Graphics::Base &graphics, Scene::Base &scene, Character::Base &character, Uint32 background, Uint32 border, int border_size, bool in_battle = false)
    {
        auto selected = Spells::Type::NONE;

        auto pad = BloodSword::OddPad;

        auto input = Controls::User();

        auto done = false;

        while (!done)
        {
            // spells popup
            auto overlay = Interface::Spells(graphics, character, Color::Background, Color::Active, BloodSword::Border, true);

            if (Input::IsValid(overlay, input))
            {
                if (input.Type != Controls::Type::BACK)
                {
                    auto &control = overlay.Controls[input.Current];

                    auto &spell = character.Spells[control.Id];

                    auto &popup = overlay.Elements[0];

                    if (character.HasCalledToMind(spell.Type) && spell.IsBattle && !spell.IsBasic())
                    {
                        auto asset = in_battle ? Asset::Type::CAST_SPELL : Asset::Type::CALL_TO_MIND;

                        overlay.VerifyAndAdd(Scene::Element(Asset::Get(asset), popup.X + popup.W - (BloodSword::TileSize + BloodSword::Pad), popup.Y + BloodSword::Pad));

                        overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsActive[spell.Type], control.X, control.Y + control.H + pad));

                        auto caption = in_battle ? Skills::Type::CAST_SPELL : Skills::Type::FORGET_SPELL;

                        overlay.VerifyAndAdd(Scene::Element(Interface::SkillCaptionsActive[caption], popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));
                    }
                    else if (spell.IsBattle && !spell.IsBasic())
                    {
                        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CALL_TO_MIND), popup.X + popup.W - (BloodSword::TileSize + BloodSword::Pad), popup.Y + BloodSword::Pad));

                        overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsInactive[spell.Type], control.X, control.Y + control.H + pad));

                        overlay.VerifyAndAdd(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CALL_TO_MIND], popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));
                    }
                    else
                    {
                        auto caption = in_battle ? Interface::SpellCaptionsInactive[spell.Type] : Interface::SpellCaptionsActive[spell.Type];

                        overlay.VerifyAndAdd(Scene::Element(caption, control.X, control.Y + control.H + pad));
                    }
                }
            }

            input = Input::WaitForInput(graphics, {scene, overlay}, overlay.Controls, input, true);

            if (Input::Check(input))
            {
                if (Engine::IsSpell(input.Type) && BloodSword::Has(Interface::ControlSpellMapping, input.Type))
                {
                    selected = Interface::ControlSpellMapping[input.Type];
                }

                done = true;
            }
        }

        return selected;
    }

    // choose character from a party
    Scene::Base CharacterList(Point origin, int w, int h, Party::Base &party, int popup_w, int popup_h, Uint32 background, Uint32 border, int border_size, Controls::Type button, Asset::Type asset)
    {
        auto overlay = Scene::Base();

        auto pad = BloodSword::QuarterTile;

        auto screen = origin + Point(w - popup_w, h - popup_h) / 2;

        auto buttons = party.Count();

        overlay.Add(Scene::Element(screen, popup_w, popup_h, background, border, border_size));

        for (auto i = 0; i < party.Count(); i++)
        {
            auto texture = Asset::Get(party[i].Asset);

            if (texture)
            {
                auto texture_w = BloodSword::Width(texture);

                auto lt = i > 0 ? i - 1 : i;

                auto rt = 0;

                if (button != Controls::Type::NONE && asset != Asset::Type::NONE)
                {
                    rt = i < party.Count() ? i + 1 : i;
                }
                else
                {
                    rt = i < party.Count() - 1 ? i + 1 : i;
                }

                overlay.Add(Controls::Base(Interface::CharacterControls[party[i].Class], i, lt, rt, i, i, screen.X + i * texture_w + pad, screen.Y + pad + BloodSword::HalfTile, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

                overlay.VerifyAndAdd(Scene::Element(texture, screen.X + i * texture_w + pad, screen.Y + pad + BloodSword::HalfTile));

                if (!Engine::IsAlive(party[i]))
                {
                    // blur dead characters
                    overlay.Add(Scene::Element(screen.X + i * texture_w + pad, screen.Y + pad + BloodSword::HalfTile, BloodSword::TileSize, BloodSword::TileSize, Color::Blur));
                }
            }
        }

        // add last button if requested
        if (button != Controls::Type::NONE && asset != Asset::Type::NONE)
        {
            auto id = party.Count();

            overlay.VerifyAndAdd(Scene::Element(Asset::Get(asset), screen.X + party.Count() * BloodSword::TileSize + pad, screen.Y + pad + BloodSword::HalfTile));

            overlay.Add(Controls::Base(button, id, id > 0 ? id - 1 : id, id, id, id, screen.X + id * BloodSword::TileSize + pad, screen.Y + pad + BloodSword::HalfTile, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

            buttons++;
        }

        // center buttons
        auto center = (popup_w - buttons * (BloodSword::TileSize + pad)) / 2;

        // skip the popup window border
        for (auto i = 0; i < overlay.Elements.size(); i++)
        {
            if (i > 0)
            {
                overlay.Elements[i].X += center;
            }

            if (i < overlay.Controls.size())
            {
                overlay.Controls[i].X += center;
            }
        }

        return overlay;
    }

    // generate list of characters
    Scene::Base CharacterList(Point origin, int w, int h, Party::Base &party, Uint32 background, Uint32 border, int border_size, Controls::Type button, Asset::Type asset)
    {
        auto pad = BloodSword::QuarterTile;

        auto popup_w = (party.Count() + 1) * (BloodSword::TileSize + pad);

        auto popup_h = (BloodSword::TileSize + pad) * 2;

        return Interface::CharacterList(origin, w, h, party, popup_w, popup_h, background, border, border_size, button, asset);
    }

    // choose character from a party
    Scene::Base CharacterList(Point origin, int w, int h, Party::Base &party, int popup_w, int popup_h, Uint32 background, Uint32 border, int border_size, bool back = true)
    {
        auto asset = back ? Asset::Type::BACK : Asset::Type::NONE;

        auto button = back ? Controls::Type::BACK : Controls::Type::NONE;

        return Interface::CharacterList(origin, w, h, party, popup_w, popup_h, background, border, border_size, button, asset);
    }

    // choose character from a party
    Scene::Base CharacterList(Point origin, int w, int h, Party::Base &party, Uint32 background, Uint32 border, int border_size, bool back = true)
    {
        auto pad = BloodSword::QuarterTile;

        auto num_buttons = party.Count() + (back ? 1 : 0);

        auto popup_w = num_buttons * (BloodSword::TileSize + pad);

        auto popup_h = (BloodSword::TileSize + pad) * 2;

        return Interface::CharacterList(origin, w, h, party, popup_w, popup_h, background, border, border_size, back);
    }

    Animation::Base Movement(Map::Base &map, Points path, Point start, Asset::Type asset)
    {
        auto movement = Animation::Base(
            {Animation::Frame(Asset::Get(asset))},
            {Animation::Type::MOVE},
            path,
            1,
            false);

        // scale movement scale to map dimensions
        movement.Scale = Point(map.TileSize, map.TileSize);

        movement.Delta = Point(8, 8);

        // set offsets
        movement.Set(Point(map.DrawX - map.X * map.TileSize, map.DrawY - map.Y * map.TileSize), start);

        // clip area outside of animation
        movement.Clip = Point(map.DrawX, map.DrawY);

        movement.ClipW = map.ViewX * map.TileSize;

        movement.ClipH = map.ViewY * map.TileSize;

        return movement;
    }

    // setup movement animation
    Animation::Base Movement(Map::Base &map, Character::Base &character, Points path, Point start)
    {
        return Interface::Movement(map, path, start, character.Asset);
    }

    // setup movement animation
    bool Move(Map::Base &map, Character::Base &character, Animation::Base &movement, Point start, Point end)
    {
        auto moving = false;

        auto enemy = character.IsEnemy();

        auto enemy_target = (enemy && map.IsValid(end) && map[end].IsEnemy()) ? map[end].Id : -1;

        // find a direct path to the destination
        auto path = Move::FindPath(map, start, end, enemy_target);

        auto closer = false;

        if (path.Points.size() == 0 && enemy)
        {
            auto target = path.Closest;

            // move closer to target
            path = Move::FindPath(map, start, path.Closest);

            InterfaceLogger::LogPathToTarget(target, path.Points.size(), map.Distance(start, target));

            closer = true;
        }

        // add extra move if enemy is trying to close distance
        auto valid = Move::Count(map, path, enemy, enemy_target) + (closer ? 1 : 0);

        if (enemy && !closer)
        {
            if (map.IsValid(end))
            {
                InterfaceLogger::LogMoveTargets((map[end].IsEnemy() ? "ENEMY" : "PLAYER"), character.Target, map[start].Id, map[end].Id, path.Points.size(), valid, map.Free(end));
            }
        }

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
            movement = Interface::Movement(map, character, Points(first, first + moves), start);

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

                auto trajectory = Points(first, first + moves);

                for (auto i = 0; i < path.Points.size() - 1; i++)
                {
                    auto &point = path.Points[i];

                    auto adjust_x = point.X - map.X;

                    auto adjust_y = point.Y - map.Y;

                    if (adjust_x >= 0 && adjust_x < map.ViewX && adjust_y >= 0 && adjust_y < map.ViewY)
                    {
                        auto x = map.DrawX + adjust_x * map.TileSize;

                        auto y = map.DrawY + adjust_y * map.TileSize;

                        if (i == 0)
                        {
                            overlay.Add(Scene::Element(x + BloodSword::SmallPad, y + BloodSword::SmallPad, map.TileSize - BloodSword::Pad, map.TileSize - BloodSword::Pad, Color::Transparent, Color::Inactive, BloodSword::Pixel));
                        }
                        else
                        {
                            overlay.Add(Scene::Element(x, y, map.TileSize, map.TileSize, Color::Inactive));
                        }
                    }
                }
            }
        }

        return overlay;
    }

    // generic dice roller
    Engine::RollResult Roll(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Uint32 border, int border_size, Asset::Type actor, Asset::Type action, int roll, int modifier)
    {
        Engine::RollResult result;

        std::string roll_string = "ROLL: " + std::to_string(roll) + 'D';

        if (modifier != 0)
        {
            if (modifier > 0)
            {
                roll_string += '+';
            }

            roll_string += std::to_string(modifier);
        }

        auto roll_texture = Graphics::CreateText(graphics, roll_string.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto start = Graphics::CreateText(graphics, {Graphics::RichText(" ROLL ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto end = Graphics::CreateText(graphics, {Graphics::RichText(" DONE ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto stage = Engine::RollStage::START;

        auto rolls = Engine::RollResult();

        auto input = Controls::User();

        auto done = false;

        auto pad = BloodSword::QuarterTile;

        auto rolled = false;

        // adjust width depending on the number of dice to roll
        w += (roll > 6 ? BloodSword::TileSize * (roll - 6) : 0);

        origin.X -= (roll > 6) ? BloodSword::HalfTile * (roll - 6) : 0;

        auto tile_pad = BloodSword::TileSize + pad;

        auto origin_x = origin.X + pad + BloodSword::Pad;

        auto origin_y = origin.Y + h - pad * 3;

        auto dice_y = BloodSword::DoubleTile + pad;

        auto origin_pad = origin + pad;

        while (!done)
        {
            auto overlay = Scene::Base();

            // draw border
            overlay.Add(Scene::Element(origin, w, h, Color::Background, border, border_size));

            // add actor icon
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(actor), origin_pad));

            // add action icon
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(action), origin_pad + Point(tile_pad, 0)));

            // add roll string
            overlay.VerifyAndAdd(Scene::Element(roll_texture, origin_pad + Point(0, tile_pad)));

            if (stage == Engine::RollStage::START)
            {
                // initial
                Interface::HorizontalMenu(overlay, start, {Controls::Type::START}, origin_x, origin_y, Color::Active, Color::Active, Color::Highlight);
            }
            else if (stage == Engine::RollStage::RESULT)
            {
                // result
                Interface::HorizontalMenu(overlay, end, {Controls::Type::EXIT}, origin_x, origin_y, Color::Active, Color::Active, Color::Highlight);
            }

            if (rolled)
            {
                for (auto dice = 0; dice < roll; dice++)
                {
                    // dice rolled
                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Interface::Dice[rolls.Rolls[dice] - 1]), origin + Point(dice * tile_pad + pad, dice_y)));
                }
            }
            else
            {
                for (auto dice = 0; dice < roll; dice++)
                {
                    // random dice/rolling
                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Interface::Dice[Engine::Random.NextInt() - 1]), origin + Point(dice * (BloodSword::TileSize + pad) + pad, dice_y)));
                }
            }

            input = Input::WaitForInput(graphics, background, overlay, input, true, true, 0);

            if (Input::Check(input))
            {
                if (input.Type == Controls::Type::START)
                {
                    if (stage == Engine::RollStage::START)
                    {
                        stage = Engine::RollStage::RESULT;

                        // roll dice
                        if (!rolled)
                        {
                            Sound::Play(Sound::Type::DICE_ROLL);

                            rolls = Engine::Roll(roll, modifier);

                            rolled = true;

                            result = rolls;

                            // check roll
                            result.Sum = std::max(0, rolls.Sum);
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

        BloodSword::Free(end);

        BloodSword::Free(start);

        BloodSword::Free(&roll_texture);

        return result;
    }

    // default roller
    Engine::RollResult Roll(Graphics::Base &graphics, Scene::Base &background, Asset::Type actor, Asset::Type action, int roll, int modifier)
    {
        auto window_w = BloodSword::OctaTile + BloodSword::HalfTile;

        auto window_h = BloodSword::WindowTile - BloodSword::Pad;

        auto origin = Point(graphics.Width - window_w, graphics.Height - window_h) / 2;

        return Interface::Roll(graphics, background, origin, window_w, window_h, Color::Active, BloodSword::Border, actor, action, roll, modifier);
    }

    // play sound effect (attribute check)
    void PlayAttributeCheckSound(BloodSword::Character::Base &attacker, BloodSword::Attribute::Type attribute, BloodSword::Item::Property weapon, bool result, bool in_battle)
    {
        if (result && in_battle && attribute == Attribute::Type::FIGHTING_PROWESS && weapon != Item::Property::NONE)
        {
            if (attacker.IsPlayer())
            {
                auto weapon_type = attacker.EquippedWeapon(weapon);

                if (weapon_type != Item::NotFound && !attacker.Items[weapon_type].Has(Item::Property::BROKEN))
                {
                    if (weapon == Item::Property::PRIMARY)
                    {
                        if (attacker.Items[weapon_type].Type == Item::Type::QUARTERSTAFF)
                        {
                            Sound::Play(Sound::Type::STAFF_HIT);
                        }
                        else
                        {
                            Sound::Play(Sound::Type::SWORD_HIT);
                        }
                    }
                    else if (weapon == Item::Property::RANGED)
                    {
                        Sound::Play(Sound::Type::BOW_RELEASE);
                    }
                    else
                    {
                        Sound::Play(Sound::Type::DICE_ROLL);
                    }
                }
                else
                {
                    Sound::Play(Sound::Type::DICE_ROLL);
                }
            }
            else
            {
                if (weapon == Item::Property::PRIMARY && attacker.Fight != Skills::Type::BROKEN_WEAPON)
                {
                    Sound::Play(Sound::Type::SWORD_HIT);
                }
                else if (weapon == Item::Property::RANGED)
                {
                    if (attacker.Shoot == Skills::Type::SHURIKEN)
                    {
                        Sound::Play(Sound::Type::WEAPON_THROW);
                    }
                    else
                    {
                        Sound::Play(Sound::Type::BOW_RELEASE);
                    }
                }
                else
                {
                    Sound::Play(Sound::Type::DICE_ROLL);
                }
            }
        }
        else
        {
            Sound::Play(Sound::Type::DICE_ROLL);
        }
    }

    // attribute level check (with target)
    bool Target(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Uint32 border, int border_size, Character::Base &attacker, Asset::Type target, Attribute::Type attribute, int roll, int modifier, Asset::Type asset, bool in_battle, Item::Property weapon = Item::Property::NONE, bool blur = true)
    {
        auto result = false;

        std::string attribute_string = Attribute::TypeMapping[attribute] + std::string(": ") + Interface::ScoreString(attacker, attribute, in_battle);

        attribute_string += "\nDIFFICULTY: " + std::to_string(roll) + "D";

        if (modifier != 0)
        {
            if (modifier > 0)
            {
                attribute_string += "+";
            }

            attribute_string += std::to_string(modifier);
        }

        auto texture = Graphics::CreateText(graphics, attribute_string.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto passed = Graphics::CreateText(graphics, "PASSED", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto failed = Graphics::CreateText(graphics, "FAILED", Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL);

        auto start = Graphics::CreateText(graphics, {Graphics::RichText(" ROLL ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto end = Graphics::CreateText(graphics, {Graphics::RichText(" DONE ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto score = Engine::Score(attacker, attribute, in_battle, weapon);

        auto stage = Engine::RollStage::START;

        auto input = Controls::User();

        auto done = false;

        auto pad = BloodSword::QuarterTile;

        auto rolled = false;

        auto rolls = Engine::RollResult();

        // Eye of the Tiger effects
        if (attribute == Attribute::Type::FIGHTING_PROWESS)
        {
            if (attacker.Has(Character::Status::FPR_PLUS2))
            {
                score += 2;
            }
            else if (attacker.Has(Character::Status::FPR_PLUS1))
            {
                score += 1;
            }
        }

        score = std::max(0, score);

        auto character_offset = (target != Asset::Type::NONE) ? 3 : 1;

        auto origin_character = origin + Point(w - pad * character_offset - (character_offset * BloodSword::TileSize), pad);

        auto origin_type = origin + Point(w - (BloodSword::TileSize + pad) * 2, pad);

        auto origin_x = origin.X + pad + BloodSword::Pad;

        auto origin_y = origin.Y + h - pad * 3;

        auto origin_result = origin + Point(w - pad * character_offset - (character_offset * BloodSword::TileSize), BloodSword::TileSize + pad);

        auto origin_target = origin + Point(w - pad - BloodSword::TileSize, pad);

        auto origin_attribute = origin + Point(pad + BloodSword::SmallPad, pad);

        while (!done)
        {
            auto overlay = Scene::Base();

            // draw border
            overlay.Add(Scene::Element(origin, w, h, Color::Background, border, border_size));

            if (target != Asset::Type::NONE)
            {
                // add target icon
                overlay.VerifyAndAdd(Scene::Element(Asset::Get(target), origin_target));
            }

            // add character icon
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(attacker.Asset), origin_character));

            // set up icon
            if (asset == Asset::Type::NONE)
            {
                overlay.VerifyAndAdd(Scene::Element(Asset::Get(Attribute::Assets[attribute]), origin_type));
            }
            else
            {
                overlay.VerifyAndAdd(Scene::Element(Asset::Get(asset), origin_type));
            }

            // attribute label
            overlay.VerifyAndAdd(Scene::Element(texture, origin_attribute));

            if (stage == Engine::RollStage::START)
            {
                // initial
                Interface::HorizontalMenu(overlay, start, {Controls::Type::START}, origin_x, origin_y, Color::Active, Color::Active, Color::Highlight);
            }
            else if (stage == Engine::RollStage::RESULT)
            {
                // result
                Interface::HorizontalMenu(overlay, end, {Controls::Type::EXIT}, origin_x, origin_y, Color::Active, Color::Active, Color::Highlight);
            }

            if (rolled)
            {
                for (auto dice = 0; dice < roll; dice++)
                {
                    // dice rolled
                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Interface::Dice[rolls.Rolls[dice] - 1]), origin + Point(dice * (BloodSword::TileSize + pad) + pad, BloodSword::TileSize + pad)));
                }

                if (result)
                {
                    // passed
                    overlay.VerifyAndAdd(Scene::Element(passed, origin_result));
                }
                else
                {
                    // failed
                    overlay.VerifyAndAdd(Scene::Element(failed, origin_result));
                }
            }
            else
            {
                for (auto dice = 0; dice < roll; dice++)
                {
                    // random dice/rolling
                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Interface::Dice[Engine::Random.NextInt() - 1]), origin + Point(dice * (BloodSword::TileSize + pad) + pad, BloodSword::TileSize + pad)));
                }
            }

            input = Input::WaitForInput(graphics, background, overlay, input, true, blur, 0);

            if (Input::Check(input))
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

                            Interface::PlayAttributeCheckSound(attacker, attribute, weapon, result, in_battle);
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

        // check if RUSTY weapon breaks
        if (weapon != Item::Property::NONE)
        {
            auto check = false;

            auto current = attacker.EquippedWeapon(weapon);

            if (weapon == Item::Property::PRIMARY && attacker.Fight == Skills::Type::RUSTY_WEAPON)
            {
                check = true;
            }
            else if (weapon != Item::Property::NONE)
            {
                check = (current >= 0 && current < attacker.Items.size() && attacker.Items[current].Has(Item::Property::RUSTY));
            }

            if (check)
            {
                auto sum = 0;

                for (auto i = 0; i < rolls.Rolls.size(); i++)
                {
                    sum += (rolls.Rolls[i] == 6 ? rolls.Rolls[i] : 0);
                }

                if (sum >= (Engine::Dice * roll))
                {
                    auto message = std::string();

                    if (weapon == Item::Property::PRIMARY && attacker.Fight == Skills::Type::RUSTY_WEAPON)
                    {
                        attacker.Fight = Skills::Type::BROKEN_WEAPON;

                        message = attacker.Name + ": WEAPON BREAKS!";
                    }
                    else if (weapon != Item::Property::NONE)
                    {
                        attacker.Items[current].Add(Item::Property::BROKEN);

                        message = attacker.Items[current].Name + " BREAKS!";
                    }

                    auto color = attacker.IsPlayer() ? Color::Highlight : Color::Active;

                    Interface::MessageBox(graphics, background, message, color);
                }
            }
        }

        BloodSword::Free(start);

        BloodSword::Free(end);

        BloodSword::Free(&passed);

        BloodSword::Free(&failed);

        BloodSword::Free(&texture);

        return result;
    }

    // attribute difficulty check (no targets / self-targetting)
    bool Test(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Uint32 border, int border_size, Character::Base &character, Attribute::Type attribute, int roll, int modifier, Asset::Type asset, bool in_battle, bool blur = true)
    {
        if (character.Has(Character::Status::SERENITY) && attribute == Attribute::Type::PSYCHIC_ABILITY)
        {
            modifier -= 2;
        }

        return Interface::Target(graphics, background, origin, w, h, border, border_size, character, Asset::Type::NONE, attribute, roll, modifier, asset, in_battle, Item::Property::NONE, blur);
    }

    // test character [ATTRIBUTE]
    bool Test(Graphics::Base &graphics, Scene::Base &background, Character::Base &character, Attribute::Type attribute)
    {
        auto w = BloodSword::OctaTile + BloodSword::HalfTile;

        auto h = BloodSword::WindowTile - BloodSword::Pad;

        auto origin = Point(graphics.Width - w, graphics.Height - h) / 2;

        return Interface::Test(graphics, background, origin, w, h, Color::Active, BloodSword::Border, character, attribute, 2, 0, Asset::Type::NONE, false);
    }

    // roll for damage
    int CombatDamage(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Uint32 border, int border_size, Character::Base &attacker, Character::Base &defender, int roll, int modifier, Asset::Type asset, bool in_battle, bool ignore_armour = false, bool blur = true)
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

        auto damaged = Graphics::CreateText(graphics, "DAMAGED", Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL);

        auto unharmed = Graphics::CreateText(graphics, "UNHARMED", Fonts::Normal, Color::S(Color::Inactive), TTF_STYLE_NORMAL);

        auto start = Graphics::CreateText(graphics, {Graphics::RichText(" DAMAGE ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto end = Graphics::CreateText(graphics, {Graphics::RichText(" DONE ", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 0)});

        auto stage = Engine::RollStage::START;

        auto rolls = Engine::RollResult();

        auto input = Controls::User();

        auto done = false;

        auto pad = BloodSword::QuarterTile;

        auto rolled = false;

        auto text_h = BloodSword::Height(unharmed);

        // adjust width depending on the number of dice to roll
        w += (roll > 6 ? BloodSword::TileSize * (roll - 6) : 0);

        origin.X -= (roll > 6) ? BloodSword::HalfTile * (roll - 6) : 0;

        auto tile_pad = BloodSword::TileSize + pad;

        auto origin_x = origin.X + pad + BloodSword::Pad;

        auto origin_y = origin.Y + h - pad * 3;

        auto edge_x = w - BloodSword::Width(unharmed) - pad;

        auto dice_y = BloodSword::DoubleTile + pad;

        auto origin_unharmed = origin + Point(edge_x, text_h * 2 + tile_pad);

        auto origin_icon = origin + Point((w - BloodSword::TileSize) / 2, pad);

        auto origin_damaged = origin + Point((w - BloodSword::Width(damaged)) / 2, tile_pad + text_h);

        auto origin_stats = origin + Point(pad, tile_pad);

        auto origin_defender = origin + Point(edge_x, pad);

        auto origin_armour = origin + Point(edge_x, tile_pad);

        auto origin_pad = origin + pad;

        // width of damage value string
        auto value_w = 0;

        while (!done)
        {
            auto overlay = Scene::Base();

            // draw border
            overlay.Add(Scene::Element(origin, w, h, Color::Background, border, border_size));

            // add damage icon
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(asset), origin_icon));

            // add attacker icon and stats
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(attacker.Asset), origin_pad));

            overlay.VerifyAndAdd(Scene::Element(damage_texture, origin_stats));

            // add defender icon and stats
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(defender.Asset), origin_defender));

            // add armour stats
            overlay.VerifyAndAdd(Scene::Element(armour_texture, origin_armour));

            if (stage == Engine::RollStage::START)
            {
                // initial
                Interface::HorizontalMenu(overlay, start, {Controls::Type::START}, origin_x, origin_y, Color::Active, Color::Active, Color::Highlight);
            }
            else if (stage == Engine::RollStage::RESULT)
            {
                // result
                Interface::HorizontalMenu(overlay, end, {Controls::Type::EXIT}, origin_x, origin_y, Color::Active, Color::Active, Color::Highlight);
            }

            if (rolled)
            {
                for (auto dice = 0; dice < roll; dice++)
                {
                    // dice rolled
                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Interface::Dice[rolls.Rolls[dice] - 1]), origin + Point(dice * tile_pad + pad, dice_y)));
                }

                if (damage > 0)
                {
                    // damaged
                    overlay.VerifyAndAdd(Scene::Element(damaged, origin_damaged));

                    auto origin_value = origin + Point((w - value_w) / 2, BloodSword::TileSize + pad);

                    overlay.VerifyAndAdd(Scene::Element(damage_value, origin_value));
                }
                else
                {
                    // unharmed
                    overlay.VerifyAndAdd(Scene::Element(unharmed, origin_unharmed));
                }
            }
            else
            {
                for (auto dice = 0; dice < roll; dice++)
                {
                    // random dice/rolling
                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Interface::Dice[Engine::Random.NextInt() - 1]), origin + Point(dice * (BloodSword::TileSize + pad) + pad, dice_y)));
                }
            }

            input = Input::WaitForInput(graphics, background, overlay, input, true, blur, 0);

            if (Input::Check(input))
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
                            damage = std::max(0, rolls.Sum - (ignore_armour ? 0 : Engine::Score(defender, Attribute::Type::ARMOUR, in_battle)));

                            if (damage > 0)
                            {
                                if (damage_value)
                                {
                                    BloodSword::Free(&damage_value);
                                }

                                damage_value = Graphics::CreateText(graphics, ("-" + std::to_string(damage) + " END").c_str(), Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL);

                                value_w = BloodSword::Width(damage_value);

                                Sound::Play(Sound::Type::COMBAT_DAMAGE);
                            }
                            else
                            {
                                Sound::Play(Sound::Type::DICE_ROLL);
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

        BloodSword::Free(end);

        BloodSword::Free(start);

        BloodSword::Free(&unharmed);

        BloodSword::Free(&damaged);

        BloodSword::Free(&armour_texture);

        BloodSword::Free(&damage_texture);

        BloodSword::Free(&damage_value);

        return damage;
    }

    // roll for damage
    int CombatDamage(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Uint32 border, int border_size, Character::Base &attacker, Character::Base &defender, Skills::Type skill, Asset::Type asset, bool in_battle, bool ignore_armour = false, bool blur = true)
    {
        auto shooting = Engine::CanShoot(attacker, skill);

        auto knockout = (skill == Skills::Type::QUARTERSTAFF) && attacker.Has(Skills::Type::QUARTERSTAFF);

        auto roll = !shooting ? attacker.Value(Attribute::Type::DAMAGE) : 1;

        roll += knockout ? 1 : 0;

        auto modifier = !shooting ? attacker.Modifier(Attribute::Type::DAMAGE) : 0;

        if (attacker.IsPlayer() && !attacker.IsArmed())
        {
            modifier -= 2;
        }
        else if (attacker.IsEnemy() && skill == Skills::Type::BROKEN_WEAPON)
        {
            modifier -= 2;
        }

        // roguelike specific modifiers
        if (!shooting)
        {
            if (attacker.Has(Character::Status::MELEE) && defender.Has(Character::Status::RANGED))
            {
                modifier += 1;
            }
            else if (attacker.Has(Character::Status::RANGED) && defender.Has(Character::Status::MELEE))
            {
                modifier -= 1;
            }
        }
        else if (attacker.Has(Character::Status::RANGED) && defender.Has(Character::Status::RANGED))
        {
            modifier += 1;
        }

        // check for bonus damage to specific target types
        if (attacker.IsPlayer() && attacker.IsArmed() && !shooting && !knockout)
        {
            auto weapon = attacker.EquippedWeapon(Item::Property::PRIMARY);

            if (weapon >= 0 && weapon < attacker.Items.size() && attacker.Items[weapon].HasDamageModifier(defender.Target))
            {
                roll += attacker.Items[weapon].DamageModifiers[defender.Target].Value;

                modifier += attacker.Items[weapon].DamageModifiers[defender.Target].Modifier;

                ignore_armour |= attacker.Items[weapon].DamageModifiers[defender.Target].IgnoreArmour;
            }
        }

        modifier -= (shooting && (skill == Skills::Type::SHURIKEN)) ? 1 : 0;

        return Interface::CombatDamage(graphics, background, origin, w, h, border, border_size, attacker, defender, roll, modifier, asset, in_battle, ignore_armour, blur);
    }

    // generic character class selector
    Character::Class SelectCharacter(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, const char *message, bool names = false, bool back = false, bool show_skills = true, bool show_stats = true, bool blur = true)
    {
        auto character_class = Character::Class::NONE;

        auto stats = Interface::GenerateStats(graphics, party, BloodSword::PentaTile, false, true);

        auto skills = Interface::Skills(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, BloodSword::PentaTile);

        auto select = Graphics::CreateText(graphics, message, Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto captions = names ? Interface::GenerateNameCaptions(graphics, party) : Interface::GenerateCharacterClassCaptions(graphics, party);

        auto pad = BloodSword::OddPad;

        auto input = Controls::User();

        auto done = false;

        auto popup_pad = BloodSword::QuarterTile;

        auto popup_w = std::max((party.Count() + 1) * (BloodSword::TileSize + popup_pad), BloodSword::Width(select) + popup_pad * 2);

        auto popup_h = stats.size() > 0 ? BloodSword::Height(stats[0]) : 0;

        auto popup = Point(graphics.Width - popup_w, graphics.Height - popup_h) / 2;

        while (!done)
        {
            auto overlay = Scene::Base();

            if (popup_h > 0)
            {
                overlay = Interface::CharacterList(Point(0, 0), graphics.Width, graphics.Height, party, popup_w, popup_h, Color::Background, Color::Active, BloodSword::Border, back);
            }
            else
            {
                overlay = Interface::CharacterList(Point(0, 0), graphics.Width, graphics.Height, party, Color::Background, Color::Active, BloodSword::Border, back);
            }

            overlay.VerifyAndAdd(Scene::Element(select, popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));

            if (Input::IsValid(overlay, input))
            {
                // party popup captions
                if (input.Type != Controls::Type::BACK && Input::IsValid(overlay, input, party.Count()))
                {
                    auto &control = overlay.Controls[input.Current];

                    if (show_stats)
                    {
                        overlay.VerifyAndAdd(Scene::Element(captions[input.Current], control.X, control.Y + control.H + pad));

                        overlay.VerifyAndAdd(Scene::Element(stats[input.Current], popup.X - (BloodSword::Width(stats[input.Current]) + pad * 2), popup.Y, Color::Background, Color::Active, 4));
                    }

                    if (show_skills)
                    {
                        if (skills[input.Current])
                        {
                            auto skills_x = popup.X + (popup_w + pad * 2);

                            overlay.Add(Scene::Element(skills_x, popup.Y, BloodSword::PentaTile, popup_h, Color::Background, Color::Active, BloodSword::Border));

                            overlay.VerifyAndAdd(Scene::Element(skills[input.Current], skills_x, popup.Y));
                        }
                    }
                }
            }

            input = Input::WaitForInput(graphics, background, overlay, input, true, blur);

            if (Input::Check(input))
            {
                if (input.Type == Controls::Type::BACK)
                {
                    character_class = Character::Class::NONE;

                    done = true;
                }
                else if (Input::IsPlayer(input) && input.Current >= 0 && input.Current < party.Count())
                {
                    if (Engine::IsAlive(party[input.Current]))
                    {
                        character_class = party[input.Current].Class;

                        done = true;
                    }
                    else
                    {
                        Interface::MessageBox(graphics, background, Engine::IsDead(party[input.Current]), Color::Highlight);
                    }
                }
            }
        }

        BloodSword::Free(&select);

        BloodSword::Free(skills);

        BloodSword::Free(stats);

        BloodSword::Free(captions);

        return character_class;
    }

    // enums for character selection mode
    enum class Mode
    {
        NONE = -1,
        ATTRIBUTES,
        STATUS,
        SKILLS,
        ITEMS
    };

    // select charcter (modal)
    Character::Class SelectCharacter(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, const char *message, Interface::Mode mode, bool back = true, bool blur = true)
    {
        auto character = Character::Class::NONE;

        auto infow = BloodSword::PentaTile;

        BloodSword::Textures textures = {};

        switch (mode)
        {
        case Mode::ATTRIBUTES:

            textures = Interface::GenerateStats(graphics, party, infow, true, false);

            break;

        case Mode::STATUS:

            textures = Interface::BasicStatus(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL);

            break;

        case Mode::SKILLS:

            textures = Interface::Skills(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, infow);

            break;

        case Mode::ITEMS:

            textures = Interface::Items(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, infow);

            break;

        default:

            textures = Interface::GenerateStats(graphics, party, infow, true, false);

            break;
        }

        auto select = Graphics::CreateText(graphics, message, Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto captions = Interface::GenerateNameCaptions(graphics, party);

        auto pad = BloodSword::OddPad;

        auto input = Controls::User();

        auto done = false;

        auto popup_pad = BloodSword::QuarterTile;

        auto popup_w = std::max((party.Count() + 1) * (BloodSword::TileSize + popup_pad), BloodSword::Width(select) + popup_pad * 2);

        auto popup_h = BloodSword::DoubleTile + BloodSword::HalfTile;

        for (auto i = 0; i < textures.size(); i++)
        {
            if (BloodSword::Height(textures[i]) > popup_h)
            {
                popup_h = BloodSword::Height(textures[i]);
            }
        }

        auto popup = Point(graphics.Width - popup_w, graphics.Height - popup_h) / 2;

        while (!done)
        {
            auto overlay = Scene::Base();

            overlay = Interface::CharacterList(Point(0, 0), graphics.Width, graphics.Height, party, popup_w, popup_h, Color::Background, Color::Active, BloodSword::Border, back);

            overlay.VerifyAndAdd(Scene::Element(select, popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));

            if (Input::IsValid(overlay, input))
            {
                // party popup captions
                if (input.Type != Controls::Type::BACK && Input::IsValid(overlay, input, party.Count()))
                {
                    auto &control = overlay.Controls[input.Current];

                    // box background
                    overlay.Add(Scene::Element(popup.X - (infow + pad * 2), popup.Y, infow, popup_h, Color::Background, Color::Active, BloodSword::Border));

                    // center caption
                    auto center = (control.W - BloodSword::Width(captions[input.Current])) / 2;

                    if ((control.X + center < (popup.X + BloodSword::QuarterTile)) && input.Current == 0)
                    {
                        center = 0;
                    }

                    // name captions
                    overlay.VerifyAndAdd(Scene::Element(captions[input.Current], control.X + center, control.Y + control.H + pad));

                    // info box
                    overlay.VerifyAndAdd(Scene::Element(textures[input.Current], popup.X - (BloodSword::Width(textures[input.Current]) + pad * 2), popup.Y));
                }
            }

            input = Input::WaitForInput(graphics, background, overlay, input, true, blur);

            if (Input::Check(input))
            {
                if (input.Type == Controls::Type::BACK)
                {
                    character = Character::Class::NONE;

                    done = true;
                }
                else if (Input::IsPlayer(input) && input.Current >= 0 && input.Current < party.Count())
                {
                    if (Engine::IsAlive(party[input.Current]))
                    {
                        character = party[input.Current].Class;

                        done = true;
                    }
                    else
                    {
                        Interface::MessageBox(graphics, background, Engine::IsDead(party[input.Current]), Color::Highlight);
                    }
                }
            }
        }

        BloodSword::Free(captions);

        BloodSword::Free(&select);

        BloodSword::Free(textures);

        return character;
    }

    // displays text from a section of the current book
    void ShowBookDescription(Graphics::Base &graphics, Scene::Base &background, Book::Location location)
    {
        auto description = Story::CurrentBook.Find(location);

        if (description >= 0 && description < Story::CurrentBook.Sections.size() && !Story::CurrentBook.Sections[description].Text.empty())
        {
            auto text_description = Story::CurrentBook.Sections[description].Text;

            Interface::TextBox(graphics, background, text_description, Color::Active, BloodSword::DescriptionSize);
        }
    }

    // show [ITEM]'s book description
    void ShowBookDescription(Graphics::Base &graphics, Scene::Base &background, Item::Type item)
    {
        if (item != Item::Type::NONE && Items::FoundDescription(item))
        {
            Interface::TextBox(graphics, background, Items::Descriptions[item], Color::Active, BloodSword::DescriptionSize);
        }
    }

    // draws a confirmation message box on screen
    bool Confirm(Graphics::Base &graphics, Scene::Base &scene, Point offset, int width, int height, SDL_Texture *message, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        auto result = false;

        auto box = Scene::Base();

        auto pad = BloodSword::QuarterTile;

        if (message)
        {
            auto texture_w = 0;

            auto texture_h = 0;

            BloodSword::Size(message, &texture_w, &texture_h);

            auto box_w = std::max(texture_w + pad * 2, BloodSword::Width(message) + ((BloodSword::TileSize + BloodSword::Pad) * 2));

            auto box_h = texture_h + pad * 3 + BloodSword::TileSize;

            auto location = offset + (Point(width, height) - Point(box_w, box_h)) / 2;

            auto message_x = offset.X + (width - texture_w) / 2;

            auto confirm = location + Point(box_w / 2 - BloodSword::TileSize - pad, texture_h + pad * 2);

            auto input = Controls::User();

            box.Add(Scene::Element(location, box_w, box_h, background, border, border_size));

            box.VerifyAndAdd(Scene::Element(message, Point(message_x, location.Y + pad)));

            box.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CONFIRM), confirm));

            box.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CANCEL), confirm + Point(TileSize + pad * 2, 0)));

            box.Add(Controls::Base(Controls::Type::CONFIRM, 0, 0, 1, 0, 0, confirm.X, confirm.Y, BloodSword::TileSize, BloodSword::TileSize, highlight));

            box.Add(Controls::Base(Controls::Type::CANCEL, 1, 0, 1, 1, 1, confirm.X + BloodSword::TileSize + pad * 2, confirm.Y, BloodSword::TileSize, BloodSword::TileSize, highlight));

            while (true)
            {
                input = Input::WaitForInput(graphics, {scene, box}, box.Controls, input, blur);

                if (Input::Check(input))
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

            BloodSword::Free(&texture);
        }

        return result;
    }

    // show confirm dialog window
    bool Confirm(Graphics::Base &graphics, Scene::Base &scene, std::string message, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        return Interface::Confirm(graphics, scene, Graphics::RichText(message.c_str(), Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), background, border, border_size, highlight, blur);
    }

    // draws a confirm text box message over a scene
    bool TextBoxConfirm(Graphics::Base &graphics, Scene::Base &scene, TTF_Font *font, const char *message, int wrap, SDL_Color color, int style, Uint32 background, Uint32 border, int border_size, Uint32 highlight, bool blur = true)
    {
        auto result = false;

        auto texture = Graphics::CreateText(graphics, message, font, color, style, wrap);

        if (texture)
        {
            auto text_w = 0;

            auto text_h = 0;

            BloodSword::Size(texture, &text_w, &text_h);

            text_w += BloodSword::LargePad;

            text_h += BloodSword::TriplePad + BloodSword::TileSize;

            auto origin = Point(graphics.Width - text_w, graphics.Height - text_h) / 2;

            result = Interface::Confirm(graphics, scene, origin, text_w, text_h, texture, background, border, border_size, highlight, blur);

            BloodSword::Free(&texture);
        }

        return result;
    }

    // show confirm textbox dialog window
    bool TextBoxConfirm(Graphics::Base &graphics, Scene::Base &scene, const char *message, int wrap)
    {
        return Interface::TextBoxConfirm(graphics, scene, Fonts::Normal, message, wrap, Color::S(Color::Active), TTF_STYLE_NORMAL, Color::Background, Color::Active, BloodSword::Border, Color::Active, true);
    }

    // show confirm textbox dialog window
    bool TextBoxConfirm(Graphics::Base &graphics, Scene::Base &scene, std::string message, int wrap)
    {
        return Interface::TextBoxConfirm(graphics, scene, message.c_str(), wrap);
    }

    // show confirm textbox dialog window
    bool TextBoxConfirm(Graphics::Base &graphics, Scene::Base &scene, const char *message, Uint32 border, int wrap)
    {
        return Interface::TextBoxConfirm(graphics, scene, Fonts::Normal, message, wrap, Color::S(Color::Active), TTF_STYLE_NORMAL, Color::Background, border, BloodSword::Border, Color::Active, true);
    }

    // show confirm textbox dialog window
    bool TextBoxConfirm(Graphics::Base &graphics, Scene::Base &scene, std::string message, Uint32 border, int wrap)
    {
        return Interface::TextBoxConfirm(graphics, scene, message.c_str(), border, wrap);
    }

    // focus cursor on character on the map
    void Focus(Map::Base &map, Engine::Queue &order, int &character, Scene::Base &overlay)
    {
        auto draw = Point(map.DrawX, map.DrawY);

        auto focus = map.Find(Engine::IsPlayer(order, character) ? Map::Object::PLAYER : Map::Object::ENEMY, order[character].Id);

        if (map.IsVisible(focus))
        {
            auto screen = (draw + (focus - Point(map.X, map.Y)) * map.TileSize) + BloodSword::SmallPad;

            overlay.Add(Scene::Element(screen.X, screen.Y, map.TileSize - BloodSword::Pad, map.TileSize - BloodSword::Pad, Color::Transparent, Color::Active, BloodSword::Pixel));
        }
    }

    // cast spell (has target)
    bool Cast(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Character::Base &caster, Asset::Type target, Spells::Type spell, bool in_battle, bool blur = true)
    {
        auto result = false;

        auto cast_w = BloodSword::OctaTile + BloodSword::HalfTile;

        auto cast_h = BloodSword::WindowTile - BloodSword::Pad;

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

                    result = Interface::Target(graphics, background, cast, cast_w, cast_h, Color::Active, BloodSword::Border, caster, target, Attribute::Type::PSYCHIC_ABILITY, roll, modifier, Spells::Assets[spell], in_battle, Item::Property::NONE, blur);

                    if (!result)
                    {
                        casting->CurrentComplexity--;
                    }
                    else
                    {
                        Sound::Play(Sound::Type::CAST_SPELL);

                        casting->CurrentComplexity = casting->Complexity;

                        caster.Forget(spell);
                    }
                }
                else
                {
                    InterfaceLogger::LogSpellFailure(caster, spell);
                }
            }
        }

        return result;
    }

    // cast spell (no targets)
    bool Cast(Graphics::Base &graphics, Scene::Base &background, Point origin, int w, int h, Character::Base &caster, Spells::Type spell, bool in_battle, bool blur = true)
    {
        return Interface::Cast(graphics, background, origin, w, h, caster, Asset::Type::NONE, spell, in_battle, blur);
    }

    // cast spell (no targets)
    bool Cast(Graphics::Base &graphics, Scene::Base &background, Character::Base &caster, Spells::Type spell, bool in_battle, bool blur = true)
    {
        return Interface::Cast(graphics, background, Point(0, 0), graphics.Width, graphics.Height, caster, Asset::Type::NONE, spell, in_battle, blur);
    }

    bool Cast(Graphics::Base &graphics, Scene::Base &background, Character::Base &caster, Asset::Type target, Spells::Type spell, bool in_battle, bool blur = true)
    {
        return Interface::Cast(graphics, background, Point(0, 0), graphics.Width, graphics.Height, caster, target, spell, in_battle, blur);
    }

    // select from a list of options
    int Choice(Graphics::Base &graphics, Scene::Base &background, Graphics::TextList &choices, Point origin, int w, int h, int limit, Uint32 bg_color, Uint32 border, Uint32 highlight, bool blur = true)
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

            Interface::ClearScrolling(overlay, input, Controls::Type::SCROLL_UP, Controls::Type::SCROLL_DOWN);

            input = Input::WaitForInput(graphics, background, overlay, input, true, blur);

            if ((Input::Check(input)) || input.Up || input.Down)
            {
                if (input.Type == Controls::Type::CHOICE)
                {
                    choice = start + input.Current;

                    if (choice >= 0 && choice < options)
                    {
                        done = true;
                    }
                }
                else if (Input::IsUp(input))
                {
                    Interface::ScrollUp(overlay, input, Controls::Type::SCROLL_UP, options, limit, start, last);
                }
                else if (Input::IsDown(input))
                {
                    Interface::ScrollDown(overlay, input, Controls::Type::SCROLL_DOWN, options, limit, start, last);
                }
            }
        }

        BloodSword::Free(menu);

        return choice;
    }

    // choose a character (with set rank)
    Character::Class SelectRankedCharacter(Graphics::Base &graphics, int rank, Party::Base &current_party)
    {
        auto character_class = Character::Class::NONE;

        // create party
        auto party = Party::Base({Generate::Character(Character::Class::WARRIOR, rank),
                                  Generate::Character(Character::Class::TRICKSTER, rank),
                                  Generate::Character(Character::Class::SAGE, rank),
                                  Generate::Character(Character::Class::ENCHANTER, rank)});

        auto stats = Interface::GenerateStats(graphics, party, BloodSword::PentaTile, false, true);

        auto skills = Interface::Skills(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, BloodSword::PentaTile);

        auto select = Graphics::CreateText(graphics, "CHOOSE A PLAYER TO ADD", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto current = Graphics::CreateText(graphics, "CURRENT PARTY", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto captions = Interface::GenerateCharacterClassCaptions(graphics, party);

        auto pad = BloodSword::OddPad;

        auto input = Controls::User();

        auto done = false;

        auto popup_pad = BloodSword::QuarterTile;

        auto popup_w = (party.Count() + 1) * (BloodSword::TileSize + popup_pad);

        auto popup_h = stats.size() > 0 ? BloodSword::Height(stats[0]) : 0;

        auto popup = Point(graphics.Width - popup_w, graphics.Height - popup_h) / 2;

        while (!done)
        {
            auto scene = Scene::Base();

            auto overlay = Scene::Base();

            if (popup_h > 0)
            {
                overlay = Interface::CharacterList(Point(0, 0), graphics.Width, graphics.Height, party, popup_w, popup_h, Color::Transparent, Color::Active, BloodSword::Border, false);
            }
            else
            {
                overlay = Interface::CharacterList(Point(0, 0), graphics.Width, graphics.Height, party, Color::Transparent, Color::Active, BloodSword::Border, false);
            }

            overlay.VerifyAndAdd(Scene::Element(select, popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));

            if (Input::IsValid(overlay, input))
            {
                // party popup captions
                if (input.Type != Controls::Type::BACK && Input::IsValid(overlay, input, party.Count()))
                {
                    auto &control = overlay.Controls[input.Current];

                    overlay.VerifyAndAdd(Scene::Element(captions[input.Current], control.X, control.Y + control.H + pad));

                    if (stats[input.Current])
                    {
                        overlay.VerifyAndAdd(Scene::Element(stats[input.Current], popup.X - (BloodSword::Width(stats[input.Current]) + pad * 2), popup.Y, Color::Transparent, Color::Active, BloodSword::Border));
                    }

                    if (skills[input.Current])
                    {
                        auto skills_x = popup.X + (popup_w + pad * 2);

                        overlay.Add(Scene::Element(skills_x, popup.Y, BloodSword::PentaTile, popup_h, Color::Background, Color::Active, BloodSword::Border));

                        overlay.VerifyAndAdd(Scene::Element(skills[input.Current], skills_x, popup.Y));
                    }
                }
            }

            if (current_party.Count() > 0)
            {
                auto screen = Point(popup.X, popup.Y - BloodSword::FrameHeight);

                overlay.Add(Scene::Element(screen, popup_w, BloodSword::DoubleTile, Color::Transparent, Color::Active, BloodSword::Border));

                for (auto i = 0; i < current_party.Count(); i++)
                {
                    auto texture = Asset::Get(current_party[i].Asset);

                    if (texture)
                    {
                        overlay.VerifyAndAdd(Scene::Element(texture, screen.X + i * BloodSword::Width(texture) + pad, screen.Y + pad + BloodSword::HalfTile));
                    }
                }

                overlay.VerifyAndAdd(Scene::Element(current, screen.X + BloodSword::QuarterTile, screen.Y + pad));
            }

            input = Input::WaitForInput(graphics, scene, overlay, input, true, true);

            if (Input::Check(input))
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

        BloodSword::Free(&current);

        BloodSword::Free(&select);

        BloodSword::Free(skills);

        BloodSword::Free(stats);

        BloodSword::Free(captions);

        return character_class;
    }

    // (new) create party
    Party::Base CreateParty(Graphics::Base &graphics, Scene::Base &background, int rank, int party_size)
    {
        auto current_party = Party::Base();

        // create party
        auto party = Party::Base({Generate::Character(Character::Class::WARRIOR, rank),
                                  Generate::Character(Character::Class::TRICKSTER, rank),
                                  Generate::Character(Character::Class::SAGE, rank),
                                  Generate::Character(Character::Class::ENCHANTER, rank)});

        auto character = 0;

        auto infow = BloodSword::OctaTile;

        auto panelw = BloodSword::OctaTile;

        auto panelh = BloodSword::PanelSize;

        auto names = Interface::GenerateNameCaptions(graphics, party);

        auto stats = Interface::PartyStats(graphics, party, infow);

        auto items = Interface::Items(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, infow);

        auto skills = Interface::Skills(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, infow);

        auto backgrounds = Interface::PartyBackgrounds(graphics, party, infow);

        auto spacing = BloodSword::TileSize + BloodSword::Pad;

        auto currenth = (BloodSword::TileSize + BloodSword::Pad) * 2;

        auto controlsy = (graphics.Height + panelh + currenth) / 2 - spacing - BloodSword::HugePad;

        auto controlsx = (graphics.Width - panelw) / 2 + BloodSword::Pad;

        auto panelx = (graphics.Width - panelw) / 2;

        auto partyy = (graphics.Height - (panelh + currenth)) / 2;

        auto panely = partyy + currenth;

        auto current = Graphics::CreateText(graphics, "CURRENT PARTY", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto currentx = (graphics.Width - BloodSword::Width(current)) / 2;

        auto partyx = (graphics.Width - (BloodSword::TileSize + BloodSword::Pad) * party_size + BloodSword::Pad) / 2;

        Asset::List assets = {
            Asset::Type::LEFT,
            Asset::Type::RIGHT,
            Asset::Type::CHARACTER,
            Asset::Type::INVENTORY,
            Asset::Type::SKILLS,
            Asset::Type::CONFIRM,
            Asset::Type::BACK};

        Controls::List controls = {
            Controls::Type::LEFT,
            Controls::Type::RIGHT,
            Controls::Type::ABOUT,
            Controls::Type::ITEMS,
            Controls::Type::SKILLS,
            Controls::Type::CONFIRM,
            Controls::Type::BACK,
        };

        std::vector<std::string> labels = {
            "PREVIOUS CHARACTER",
            "NEXT CHARACTER",
            "BACKGROUND",
            "INVENTORY",
            "SKILLS",
            "ADD/REMOVE"};

        auto captions = Graphics::CreateText(graphics, Graphics::GenerateTextList(labels, Fonts::Caption, Color::Active, 0));

        auto display = Controls::Type::ABOUT;

        auto display_y = panely + BloodSword::TileSize + BloodSword::HalfTile + BloodSword::OddPad * 4;

        auto boxh = controlsy - display_y - BloodSword::LargePad;

        auto input = Controls::User();

        auto done = false;

        while (!done)
        {
            auto overlay = Scene::Base();

            overlay.Add(Scene::Element(panelx, partyy, panelw, BloodSword::DoubleTile, Color::Background, Color::Active, BloodSword::Border));

            overlay.VerifyAndAdd(Scene::Element(current, currentx, partyy + BloodSword::SmallPad));

            // current party
            if (current_party.Count() > 0)
            {
                for (auto i = 0; i < current_party.Count(); i++)
                {
                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(current_party[i].Asset), partyx + i * (BloodSword::TileSize + BloodSword::Pad), partyy + BloodSword::Pad + BloodSword::HalfTile));
                }
            }

            // render panel
            overlay.Add(Scene::Element(panelx, panely, panelw, panelh, Color::Background, Color::Active, BloodSword::Border));

            // render subpanel
            overlay.Add(Scene::Element(panelx, display_y - BloodSword::Pad, panelw, boxh, Color::Background, Color::Active, BloodSword::Border));

            // render names
            overlay.VerifyAndAdd(Scene::Element(names[character], Point(panelx + BloodSword::Pad, panely + BloodSword::Pad)));

            // render stats
            overlay.VerifyAndAdd(Scene::Element(stats[character], Point(panelx + BloodSword::Pad, panely + BloodSword::EpicPad)));

            // render information
            if (character >= 0 && character < party.Count())
            {
                switch (display)
                {
                case Controls::Type::ABOUT:

                    if (backgrounds[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(backgrounds[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;

                case Controls::Type::ITEMS:

                    if (items[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(items[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;

                case Controls::Type::SKILLS:

                    if (skills[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(skills[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;

                default:

                    if (backgrounds[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(backgrounds[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;
                }
            }

            // generate panel controls
            auto id = 0;

            for (auto control = 0; control < controls.size(); control++)
            {
                auto lt = id > 0 ? id - 1 : id;

                auto rt = controls[control] != Controls::Type::BACK ? id + 1 : id;

                overlay.VerifyAndAdd(Scene::Element(Asset::Get(assets[control]), Point(controlsx + control * spacing, controlsy)));

                overlay.Add(Controls::Base(controls[control], id, lt, rt, id, id, controlsx + control * spacing, controlsy, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

                if ((controls[control] == Controls::Type::LEFT && character == 0) || (controls[control] == Controls::Type::RIGHT && character == party.Count() - 1))
                {
                    // blur button
                    overlay.Add(Scene::Element(controlsx + control * spacing, controlsy, BloodSword::TileSize, BloodSword::TileSize, Color::Blur));
                }

                id++;
            }

            // render button captions
            if (input.Type != Controls::Type::BACK && Input::IsValid(overlay, input))
            {
                auto &control = overlay.Controls[input.Current];

                // center caption
                auto center = (control.W - BloodSword::Width(captions[input.Current])) / 2;

                if ((control.X + center < (panelx + BloodSword::QuarterTile)) && input.Current == 0)
                {
                    center = 0;
                }

                overlay.VerifyAndAdd(Scene::Element(captions[input.Current], control.X + center, control.Y + control.H + BloodSword::Pad));
            }

            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

            if (Input::Validate(input))
            {
                if (input.Type == Controls::Type::CONFIRM)
                {
                    auto character_class = party[character].Class;

                    if (!current_party.Has(character_class))
                    {
                        current_party.Add(party[character]);

                        Interface::MessageBox(graphics, overlay, Graphics::RichText(std::string(Character::ClassMapping[character_class]) + " added to the party!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Active, BloodSword::Border, Color::Highlight, true);
                    }
                    else
                    {
                        current_party.Remove(character_class);

                        Interface::MessageBox(graphics, overlay, Graphics::RichText(std::string(Character::ClassMapping[character_class]) + " removed from the party!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Highlight, BloodSword::Border, Color::Active, true);
                    }

                    if (current_party.Count() == party_size)
                    {
                        auto last = current_party.Count() - 1;

                        // add latest character
                        overlay.VerifyAndAdd(Scene::Element(Asset::Get(current_party[last].Asset), partyx + last * (BloodSword::TileSize + BloodSword::Pad), partyy + BloodSword::Pad + BloodSword::HalfTile));

                        if (!Interface::Confirm(graphics, overlay, "PROCEED?", Color::Background, Color::Active, BloodSword::Border, Color::Highlight, false))
                        {
                            current_party.Clear();
                        }
                        else
                        {
                            done = true;
                        }
                    }
                }
                else if (input.Type == Controls::Type::BACK)
                {
                    done = true;
                }
                else if (input.Type == Controls::Type::LEFT)
                {
                    if (character > 0)
                    {
                        character--;
                    }
                    else
                    {
                        Sound::Play(Sound::Type::FAIL);
                    }
                }
                else if (input.Type == Controls::Type::RIGHT)
                {
                    if (character < party.Count() - 1)
                    {
                        character++;
                    }
                    else
                    {
                        Sound::Play(Sound::Type::FAIL);
                    }
                }
                else if (input.Type == Controls::Type::SKILLS)
                {
                    display = Controls::Type::SKILLS;
                }
                else if (input.Type == Controls::Type::ITEMS)
                {
                    display = Controls::Type::ITEMS;
                }
                else if (input.Type == Controls::Type::ABOUT)
                {
                    display = Controls::Type::ABOUT;
                }

                input.Selected = false;
            }
        }

        BloodSword::Free(&current);

        BloodSword::Free(backgrounds);

        BloodSword::Free(captions);

        BloodSword::Free(items);

        BloodSword::Free(skills);

        BloodSword::Free(stats);

        BloodSword::Free(names);

        return current_party;
    }

    // create party with custom starting ranks depending on size of party
    Party::Base CreateParty(Graphics::Base &graphics, std::vector<int> ranks, bool blur = true)
    {
        if (ranks.size() != 4)
        {
            throw std::invalid_argument("Invalid starting ranks definition");
        }

        auto menu = Scene::Base();

        auto overlay = Scene::Base();

        auto width = BloodSword::QuadTile;

        auto base_height = BloodSword::HalfTile;

        auto pad = BloodSword::Pad;

        auto items = 4;

        auto height = (base_height + pad * 2) * items - pad * 7 / 2;

        auto menu_title = Graphics::CreateText(graphics, "CHOOSE NUMBER OF PARTY MEMBERS", Fonts::Caption, Color::S(Color::Highlight), TTF_STYLE_NORMAL);

        auto party = Party::Base();

        auto starting_ranks = std::string(" Starting ranks(s): ");

        auto rank1 = " 1" + starting_ranks + std::to_string(ranks[0]) + " ";

        auto rank2 = " 2" + starting_ranks + std::to_string(ranks[1]) + " ";

        auto rank3 = " 3" + starting_ranks + std::to_string(ranks[2]) + " ";

        auto rank4 = " 4" + starting_ranks + std::to_string(ranks[3]) + " ";

        // adjust width
        auto item_w = 0;

        Graphics::Estimate(Fonts::Caption, rank1.c_str(), &item_w, nullptr);

        width = std::max(item_w, width);

        auto origin = Point(graphics.Width - width, graphics.Height - height) / 2;

        menu.Add(Scene::Element(menu_title, Point((graphics.Width - BloodSword::Width(menu_title)) / 2, origin.Y - pad * 6)));

        menu.Add(Scene::Element(origin - Point(pad, pad), width + pad * 2, height + pad * 2, Color::Background, Color::Active, BloodSword::Border));

        Graphics::TextList party_sizes = {
            Graphics::RichText(rank1.c_str(), Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
            Graphics::RichText(rank2.c_str(), Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
            Graphics::RichText(rank3.c_str(), Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
            Graphics::RichText(rank4.c_str(), Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, 0)};

        auto current = Graphics::CreateText(graphics, "CURRENT PARTY", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto party_size = Interface::Choice(graphics, menu, party_sizes, origin, width, base_height, int(party_sizes.size()), Color::Background, Color::Background, Color::Highlight, false) + 1;

        if (party_size > 0 && party_size <= 4)
        {
            auto rank = ranks[0];

            if (party_size == 2)
            {
                rank = ranks[1];
            }
            else if (party_size == 3)
            {
                rank = ranks[2];
            }
            else if (party_size == 4)
            {
                rank = ranks[3];

                for (auto &character_class : Character::All)
                {
                    auto character = Generate::Character(character_class, rank);

                    party.Add(character);
                }
            }

            while (party.Count() != party_size)
            {
                party = Interface::CreateParty(graphics, overlay, rank, party_size);

                if (party.Count() == 0)
                {
                    break;
                }
            }

            if (party.Count() > 1)
            {
                Interface::MessageBox(graphics, overlay, Graphics::RichText("PARTY COMPLETE!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Active, BloodSword::Border, Color::Highlight, blur);
            }
        }

        BloodSword::Free(&current);

        BloodSword::Free(&menu_title);

        return party;
    }

    // create a party (using ranks from a book)
    Party::Base CreateParty(Graphics::Base &graphics, Book::Number book, bool blur = true)
    {
        return Interface::CreateParty(graphics, Book::Ranks[book], blur);
    }

    // get text from user input (popup interface)
    std::string TextInput(Graphics::Base &graphics, Scene::Base &background, Point location, std::string question, std::string start_text, Uint32 question_color, Uint32 input_color, int input_limit, int box_w, int box_h, Uint32 border = Color::Active, Uint32 box_bg = Color::Background, int border_size = BloodSword::Border, bool blur = true)
    {
        auto message = Graphics::CreateText(graphics, question.c_str(), Fonts::Normal, Color::S(question_color), TTF_STYLE_NORMAL, 0);

        auto input_text = std::string();

        if (message)
        {
            auto cursor = Graphics::CreateText(graphics, "_", Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL, 0);

            SDL_Texture *texture = nullptr;

            auto input = Controls::User();

            auto pad = BloodSword::QuarterTile;

            input.TextLimit = input_limit;

            input.SetText(start_text);

            input.RefreshText = (input.TextInput.size() > 0);

            if (input.TextInput.size() > 0)
            {
                texture = Graphics::CreateText(graphics, input.TextInput.c_str(), Fonts::Normal, Color::S(input_color), TTF_STYLE_NORMAL, 0);
            }

            // setup text input mode
            input.Text = true;

            // cursor blink
            auto blink = false;

            // enable text input events
            Input::StartTextInput();

            while (true)
            {
                auto box = Scene::Base();

                box.Add(Scene::Element(location, box_w, box_h, box_bg, border, border_size));

                box.VerifyAndAdd(Scene::Element(message, location + Point(pad, pad)));

                if (texture)
                {
                    box.VerifyAndAdd(Scene::Element(texture, location + Point(pad, BloodSword::TileSize)));
                }

                // add blinking cursor
                if (blink && input.TextInput.size() < input.TextLimit)
                {
                    auto pad_cursor = texture ? BloodSword::Width(texture) : 0;

                    box.VerifyAndAdd(Scene::Element(cursor, location + Point(pad + pad_cursor, BloodSword::TileSize)));
                }

                blink = !blink;

                input = Input::WaitForText(graphics, background, box, input, true, blur, BloodSword::StandardDelay);

                if (input.RefreshText)
                {
                    BloodSword::Free(&texture);

                    if (input.TextInput.size() > 0)
                    {
                        texture = Graphics::CreateText(graphics, input.TextInput.c_str(), Fonts::Normal, Color::S(input_color), TTF_STYLE_NORMAL, 0);
                    }
                }
                else if (input.Selected && input.TextInput.size() > 0)
                {
                    break;
                }
            }

            // disable text input events
            Input::StopTextInput();

            BloodSword::Free(&texture);

            BloodSword::Free(&cursor);

            BloodSword::Free(&message);

            input_text = input.TextInput;
        }

        return input_text;
    }

    // get text from user input (popup interface)
    std::string TextInput(Graphics::Base &graphics, Scene::Base &background, std::string question, int input_limit, int box_w, int box_h, bool blur = true)
    {
        auto location = (Point(graphics.Width, graphics.Height) - Point(box_w, box_h)) / 2;

        return Interface::TextInput(graphics, background, location, question, "", Color::Inactive, Color::Active, input_limit, box_w, box_h, Color::Active, Color::Background, BloodSword::Border, blur);
    }

    // get text from user input (popup interface)
    std::string TextInput(Graphics::Base &graphics, Scene::Base &background, std::string question, int box_w, int box_h, bool blur = true)
    {
        return Interface::TextInput(graphics, background, question, (BloodSword::QuarterTile + BloodSword::SmallPad), box_w, box_h, blur);
    }

    // get text from user input (popup interface)
    std::string TextInput(Graphics::Base &graphics, Scene::Base &background, std::string question, bool blur = true)
    {
        auto box_w = (int(question.size()) > BloodSword::QuarterTile) ? (int(question.size()) * BloodSword::QuarterTile) : (BloodSword::PentaTile);

        return Interface::TextInput(graphics, background, question, box_w, (BloodSword::DoubleTile - BloodSword::Pad), blur);
    }

    // get text from user input (popup interface)
    std::string TextInput(Graphics::Base &graphics, std::string question, bool blur = true)
    {
        auto background = Scene::Base();

        return Interface::TextInput(graphics, background, question, blur);
    }

    // get text from list of internal string messages
    std::string GetText(const int message)
    {
        auto text = std::string();

        if (message >= 0 && message < Interface::Text.size())
        {
            text = Interface::Text[message].Text;
        }

        return text;
    }

    // generic error message box
    void ErrorMessage(Graphics::Base &graphics, Scene::Base &background, int message)
    {
        Interface::MessageBox(graphics, background, Point(0, 0), graphics.Width, graphics.Height, Interface::Text[message], Color::Background, Color::Highlight, BloodSword::Border, Color::Active, true);
    }

    // generic notify message box
    void Notify(Graphics::Base &graphics, Scene::Base &background, int message)
    {
        Interface::MessageBox(graphics, background, Point(0, 0), graphics.Width, graphics.Height, Interface::Text[message], Color::Background, Color::Active, BloodSword::Border, Color::Highlight, true);
    }

    // internal error
    void InternalError(Graphics::Base &graphics, Scene::Base &background, std::string message)
    {
        // do not blur on internal error
        Interface::MessageBox(graphics, background, Graphics::RichText(message.c_str(), Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Highlight, 4, Color::Highlight, false);
    }

    // not implemented yet error message
    void NotImplemented(Graphics::Base &graphics, Scene::Base &overlay)
    {
        Interface::Notify(graphics, overlay, Interface::MSG_IMPLEMENT);
    }

    // generic get number popup (arbitrary location)
    int GetNumber(Graphics::Base &graphics, Scene::Base &background, const char *message, Point origin, int popup_w, int popup_h, int minimum, int maximum, Asset::Type target, Asset::Type increase, Asset::Type decrease, bool cancel = true)
    {
        if (maximum < minimum)
        {
            std::swap(minimum, maximum);
        }

        auto number = std::max(0, minimum);

        std::string string_current = "NOW: ";

        std::string string_min = "MIN: " + std::to_string(minimum);

        std::string string_max = "MAX: " + std::to_string(maximum);

        auto texture = Graphics::CreateText(graphics, message, Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto texture_min = Graphics::CreateText(graphics, string_min.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto texture_max = Graphics::CreateText(graphics, string_max.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto current = Graphics::CreateText(graphics, (string_current + std::to_string(number)).c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

        auto texture_height = BloodSword::Height(texture_min);

        auto texture_width = BloodSword::Width(texture_max);

        auto pad = BloodSword::Pad;

        auto asset = origin + Point(popup_w - BloodSword::TileSize - pad, pad);

        auto button = origin + Point(pad, popup_h - BloodSword::TileSize - pad * 2);

        auto texture_x = popup_w - texture_width - pad;

        auto input = Controls::User();

        auto done = false;

        auto update = false;

        // set up overlay controls
        auto controls = Controls::Collection();

        // button offset
        auto offset = BloodSword::TileSize + pad;

        // starting id
        auto id = 0;

        // increase number
        controls.push_back(Controls::Base(Controls::Type::PLUS, id, id, id + 1, id, id, button.X, button.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

        // decrease number
        controls.push_back(Controls::Base(Controls::Type::MINUS, id + 1, id, id + 2, id + 1, id + 1, button.X + (offset), button.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

        // confirm
        controls.push_back(Controls::Base(Controls::Type::CONFIRM, id + 2, id + 1, id + 3, id + 2, id + 2, button.X + offset * 2, button.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

        if (cancel)
        {
            // cancel
            controls.push_back(Controls::Base(Controls::Type::CANCEL, id + 3, id + 2, id + 3, id + 3, id + 3, button.X + offset * 3, button.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));
        }

        while (!done)
        {
            // initialize overlay
            auto overlay = Scene::Base();

            // draw border
            overlay.Add(Scene::Element(origin, popup_w, popup_h, Color::Background, Color::Active, BloodSword::Border));

            // add message
            overlay.VerifyAndAdd(Scene::Element(texture, origin + pad));

            // stats location
            auto line = pad;

            if (target != Asset::Type::NONE)
            {
                overlay.VerifyAndAdd(Scene::Element(Asset::Get(target), asset));

                line += BloodSword::TileSize + pad;
            }

            // add minimum
            overlay.VerifyAndAdd(Scene::Element(texture_min, origin + Point(texture_x, line)));

            // add maximum
            overlay.VerifyAndAdd(Scene::Element(texture_max, origin + Point(texture_x, line + texture_height)));

            // update texture
            if (update)
            {
                BloodSword::Free(&current);

                current = Graphics::CreateText(graphics, (string_current + std::to_string(number)).c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

                update = false;
            }

            // add current number
            overlay.VerifyAndAdd(Scene::Element(current, origin + Point(texture_x, line + texture_height * 2)));

            // increase number (icon)
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(increase), button));

            // decrease number (icon)
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(decrease), Point(button.X + offset, button.Y)));

            // confirm (icon)
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CONFIRM), Point(button.X + offset * 2, button.Y)));

            if (cancel)
            {
                // cancel (icon)
                overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CANCEL), Point(button.X + offset * 3, button.Y)));
            }

            // add boxes
            auto row = 0;

            auto col = 0;

            for (auto box = 0; box < number; box++)
            {
                // break long lines into the next row
                if (box % Interface::MaxBoxRow == 0)
                {
                    // reset column
                    col = 0;

                    row++;
                }
                else
                {
                    // fill the line
                    col++;
                }

                overlay.Add(Scene::Element(origin + Point(col * (Interface::BoxSize + pad) + pad * 2, row * (Interface::BoxSize + BloodSword::SmallPad * 2) + pad * 3), Interface::BoxSize, Interface::BoxSize, Color::Highlight, Color::Active, BloodSword::Border));
            }

            // set overlay controls
            overlay.Controls = controls;

            // get input
            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

            if (Input::Check(input))
            {
                if (input.Type == Controls::Type::CONFIRM)
                {
                    done = true;
                }
                else if (input.Type == Controls::Type::CANCEL)
                {
                    number = 0;

                    done = true;
                }
                else if (input.Type == Controls::Type::PLUS)
                {
                    if (number < maximum)
                    {
                        number++;

                        update = true;
                    }
                }
                else if (input.Type == Controls::Type::MINUS)
                {
                    if (number > minimum)
                    {
                        number--;

                        update = true;
                    }
                }
            }
        }

        BloodSword::Free(&current);

        BloodSword::Free(&texture_max);

        BloodSword::Free(&texture_min);

        BloodSword::Free(&texture);

        return number;
    }

    // generic get number popup (centered)
    int GetNumber(Graphics::Base &graphics, Scene::Base &background, const char *message, int minimum, int maximum, Asset::Type target, Asset::Type increase, Asset::Type decrease, bool cancel = true)
    {
        auto popup_w = BloodSword::OctaTile + BloodSword::HalfTile;

        auto popup_h = ((maximum / Interface::MaxBoxRow) + 2) * BloodSword::HalfTile + BloodSword::TileSize + BloodSword::HugePad;

        auto origin = Point(graphics.Width - popup_w, graphics.Height - popup_h) / 2;

        return Interface::GetNumber(graphics, background, message, origin, popup_w, popup_h, minimum, maximum, target, increase, decrease, cancel);
    }

    // heal party interface
    void Heal(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Base &character, bool blur = true)
    {
        if (character.Has(Skills::Type::HEALING))
        {
            if (Engine::IsAlive(party) && Engine::IsAlive(character))
            {
                if (!Engine::Healed(party))
                {
                    if (!character.Has(Character::Status::IN_BATTLE))
                    {
                        if (Engine::Score(character, Attribute::Type::ENDURANCE) > 1)
                        {
                            auto done = false;

                            while (!done)
                            {
                                auto endurance = Engine::Score(character, Attribute::Type::ENDURANCE);

                                if (endurance > 1)
                                {
                                    auto cost = Interface::GetNumber(graphics, background, "HOW MANY ENDURANCE TO SPEND?", 0, endurance - 1, character.Asset, Asset::Type::HEAL, Asset::Type::DAMAGE);

                                    if (cost > 0)
                                    {
                                        Engine::GainEndurance(character, -cost, false);

                                        auto score = cost * Interface::Roll(graphics, background, character.Asset, Asset::Type::HEALING, 1, -2).Sum;

                                        done = !(score > 0);

                                        if (score == 0)
                                        {
                                            // healing attempt failed
                                            Interface::ErrorMessage(graphics, background, Interface::MSG_HEALING);
                                        }
                                        else
                                        {
                                            // check if healing is doubled
                                            if (character.Has(Character::Status::DOUBLE_HEALING))
                                            {
                                                score *= 2;
                                            }

                                            while (!done)
                                            {
                                                // distribute healing
                                                auto target = Interface::SelectCharacter(graphics, background, party, "SELECT PLAYER TO HEAL", true, false, true, true, blur);

                                                if (party[target].Value(Attribute::Type::ENDURANCE) > 0 && party[target].Value(Attribute::Type::ENDURANCE) < party[target].Maximum(Attribute::Type::ENDURANCE))
                                                {
                                                    if (!character.Has(Character::Status::TASK) || target == character.Class)
                                                    {
                                                        std::string heal_string = "HEAL " + party[target].Name;

                                                        auto max_healing = std::min(score, party[target].Maximum(Attribute::Type::ENDURANCE) - party[target].Value(Attribute::Type::ENDURANCE));

                                                        // heal
                                                        auto heal = Interface::GetNumber(graphics, background, heal_string.c_str(), 0, max_healing, party[target].Asset, Asset::Type::HEAL, Asset::Type::DAMAGE);

                                                        if (heal > 0)
                                                        {
                                                            // heal selected character
                                                            Engine::GainEndurance(party[target], heal, false);

                                                            Interface::Notify(graphics, background, Interface::MSG_HEALED);
                                                        }

                                                        score -= heal;
                                                    }
                                                    else
                                                    {
                                                        std::string message = party[target].Name + " IS AWAY";

                                                        Interface::MessageBox(graphics, background, message, Color::Highlight);
                                                    }
                                                }
                                                else if (!Engine::IsAlive(party[target]))
                                                {
                                                    // target is beyond healing
                                                    Interface::ErrorMessage(graphics, background, Interface::MSG_LOST);
                                                }
                                                else
                                                {
                                                    // target is not wounded
                                                    Interface::ErrorMessage(graphics, background, Interface::MSG_WOUND);
                                                }

                                                done = !(score > 0) || Engine::Healed(party);
                                            }

                                            if (Engine::Healed(party))
                                            {
                                                // everyone is at maximum endurance
                                                Interface::Notify(graphics, background, Interface::MSG_MAX);
                                            }
                                        }
                                    }

                                    done = !(cost > 0) || done;
                                }

                                done = !(endurance > 1) || done;
                            }
                        }
                        else
                        {
                            Interface::ErrorMessage(graphics, background, Interface::MSG_ENDURANCE);
                        }
                    }
                    else
                    {
                        Interface::ErrorMessage(graphics, background, Interface::MSG_BATTLE);
                    }
                }
                else if (Engine::Healed(party))
                {
                    // everyone is at maximum endurance
                    Interface::Notify(graphics, background, Interface::MSG_MAX);
                }
            }
            else
            {
                Interface::ErrorMessage(graphics, background, Interface::MSG_ACTION);
            }
        }
        else
        {
            Interface::ErrorMessage(graphics, background, Interface::MSG_SKILL);
        }
    }

    // select multiple characters (apply STATUS to selected and EXCLUDE status otherwise)
    void SelectMultiple(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, const char *message, Character::Status preselect, Character::Status status_selected, Character::Status status_excluded, bool names = false)
    {
        auto selection = std::vector<bool>(party.Count());

        auto select = Graphics::CreateText(graphics, message, Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto captions = names ? Interface::GenerateNameCaptions(graphics, party) : Interface::GenerateCharacterClassCaptions(graphics, party);

        // pre-select characters with specific status
        if (preselect != Character::Status::NONE)
        {
            for (auto i = 0; i < party.Count(); i++)
            {
                if (Engine::IsAlive(party[i]) && party[i].Has(preselect))
                {
                    selection[i] = true;
                }
            }
        }

        auto pad = BloodSword::OddPad;

        auto input = Controls::User();

        auto done = false;

        auto popup_pad = BloodSword::QuarterTile;

        auto popup_w = std::max((party.Count() + 1) * (BloodSword::TileSize + popup_pad), BloodSword::Width(select) + popup_pad * 2);

        auto popup_h = (BloodSword::TileSize + BloodSword::QuarterTile) * 2;

        auto popup = Point(graphics.Width - popup_w, graphics.Height - popup_h) / 2;

        while (!done)
        {
            auto overlay = Scene::Base();

            overlay = Interface::CharacterList(Point(0, 0), graphics.Width, graphics.Height, party, popup_w, popup_h, Color::Background, Color::Active, BloodSword::Border, Controls::Type::CONFIRM, Asset::Type::CONFIRM);

            // title
            overlay.VerifyAndAdd(Scene::Element(select, popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));

            for (auto i = 0; i < selection.size(); i++)
            {
                if (selection[i] && Input::IsValid(overlay, i, selection.size()))
                {
                    auto &control = overlay.Controls[i];

                    // highlight current selection
                    overlay.Add(Scene::Element(Point(control.X + 2 * control.Pixels, control.Y + 2 * control.Pixels), control.W - 4 * control.Pixels, control.H - 4 * control.Pixels, Color::Transparent, Color::Active, BloodSword::Pixel));
                }
            }

            if (Input::IsValid(overlay, input))
            {
                // party popup captions
                if (input.Type != Controls::Type::CONFIRM && Input::IsValid(overlay, input, party.Count()))
                {
                    auto &control = overlay.Controls[input.Current];

                    // center caption
                    auto center = (control.W - BloodSword::Width(captions[input.Current])) / 2;

                    if ((control.X + center < (popup.X + BloodSword::QuarterTile)) && input.Current == 0)
                    {
                        center = 0;
                    }

                    overlay.VerifyAndAdd(Scene::Element(captions[input.Current], control.X + center, control.Y + control.H + pad));
                }
            }

            input = Input::WaitForInput(graphics, background, overlay, input, true, true);

            if (Input::Check(input))
            {
                if (input.Type == Controls::Type::CONFIRM)
                {
                    done = Interface::Confirm(graphics, background, Graphics::RichText("PROCEED?", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Active, BloodSword::Border, Color::Highlight, true);
                }
                else if (Input::IsPlayer(input) && input.Current >= 0 && input.Current < party.Count())
                {
                    if (preselect == Character::Status::NONE || !party[input.Current].Has(preselect))
                    {
                        selection[input.Current] = !selection[input.Current];
                    }
                }

                input.Selected = false;
            }
        }

        // update party status
        for (auto i = 0; i < selection.size(); i++)
        {
            if (selection[i])
            {
                if (status_selected != Character::Status::NONE)
                {
                    party[i].Add(status_selected);
                }
            }
            else
            {
                if (status_excluded != Character::Status::NONE)
                {
                    party[i].Add(status_excluded);
                }
            }
        }

        BloodSword::Free(captions);

        BloodSword::Free(&select);
    }

    // select multiple characters then apply status
    void SelectMultiple(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, const char *message, Character::Status status_selected, int limit, bool names = false)
    {
        if (status_selected != Character::Status::NONE)
        {
            auto selection = std::vector<bool>(party.Count());

            auto select = Graphics::CreateText(graphics, message, Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

            auto captions = names ? Interface::GenerateNameCaptions(graphics, party) : Interface::GenerateCharacterClassCaptions(graphics, party);

            auto pad = BloodSword::OddPad;

            auto input = Controls::User();

            auto done = false;

            auto popup_pad = BloodSword::QuarterTile;

            auto popup_w = std::max((party.Count() + 1) * (BloodSword::TileSize + popup_pad), BloodSword::Width(select) + popup_pad * 2);

            auto popup_h = (BloodSword::TileSize + BloodSword::QuarterTile) * 2;

            auto popup = Point(graphics.Width - popup_w, graphics.Height - popup_h) / 2;

            while (!done)
            {
                auto overlay = Scene::Base();

                overlay = Interface::CharacterList(Point(0, 0), graphics.Width, graphics.Height, party, popup_w, popup_h, Color::Background, Color::Active, BloodSword::Border, Controls::Type::CONFIRM, Asset::Type::CONFIRM);

                // title
                overlay.VerifyAndAdd(Scene::Element(select, popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));

                for (auto i = 0; i < selection.size(); i++)
                {
                    if (selection[i] && Input::IsValid(overlay, i, selection.size()))
                    {
                        auto &control = overlay.Controls[i];

                        // highlight current selection
                        overlay.Add(Scene::Element(Point(control.X + 2 * control.Pixels, control.Y + 2 * control.Pixels), control.W - 4 * control.Pixels, control.H - 4 * control.Pixels, Color::Transparent, Color::Active, BloodSword::Pixel));
                    }
                }

                if (Input::IsValid(overlay, input))
                {
                    // party popup captions
                    if (input.Type != Controls::Type::CONFIRM && Input::IsValid(overlay, input, party.Count()))
                    {
                        auto &control = overlay.Controls[input.Current];

                        // center caption
                        auto center = (control.W - BloodSword::Width(captions[input.Current])) / 2;

                        if ((control.X + center < (popup.X + BloodSword::QuarterTile)) && input.Current == 0)
                        {
                            center = 0;
                        }

                        overlay.VerifyAndAdd(Scene::Element(captions[input.Current], control.X + center, control.Y + control.H + pad));
                    }
                }

                input = Input::WaitForInput(graphics, background, overlay, input, true, true);

                if (Input::Check(input))
                {
                    if (input.Type == Controls::Type::CONFIRM)
                    {
                        done = Interface::Confirm(graphics, background, Graphics::RichText("PROCEED?", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Active, BloodSword::Border, Color::Highlight, true);
                    }
                    else if (Input::IsPlayer(input) && input.Current >= 0 && input.Current < party.Count())
                    {
                        if (std::count(selection.begin(), selection.end(), true) < limit || selection[input.Current])
                        {
                            selection[input.Current] = !selection[input.Current];
                        }
                    }

                    input.Selected = false;
                }
            }

            // update party status
            for (auto i = 0; i < selection.size(); i++)
            {
                if (Engine::IsAlive(party[i]) && selection[i])
                {
                    party[i].Add(status_selected);
                }
            }

            BloodSword::Free(captions);

            BloodSword::Free(&select);
        }
    }

    // generate horizontal icon menu
    Scene::Base IconList(Point origin, int w, int h, Asset::List assets, Controls::List controls, int popup_w, int popup_h, Uint32 background, Uint32 border, int border_size, Controls::Type button, Asset::Type asset)
    {
        auto overlay = Scene::Base();

        auto pad = BloodSword::Pad;

        auto screen = origin + Point(w - popup_w, h - popup_h) / 2;

        overlay.Add(Scene::Element(screen, popup_w, popup_h, background, border, border_size));

        auto size_icons = (assets.size() + ((button != Controls::Type::NONE && asset != Asset::Type::NONE) ? 1 : 0)) * (BloodSword::TileSize + pad) - pad;

        auto offset = popup_w > size_icons ? (popup_w - size_icons) / 2 : 0;

        for (auto i = 0; i < assets.size(); i++)
        {
            auto texture = Asset::Get(assets[i]);

            if (texture)
            {
                auto texture_w = BloodSword::Width(texture);

                auto lt = i > 0 ? i - 1 : i;

                auto rt = 0;

                if (button != Controls::Type::NONE && asset != Asset::Type::NONE)
                {
                    rt = i < assets.size() ? i + 1 : i;
                }
                else
                {
                    rt = i < assets.size() - 1 ? i + 1 : i;
                }

                auto icon_x = screen.X + i * (texture_w + pad) + offset;

                overlay.Add(Controls::Base(controls[i], i, lt, rt, i, i, icon_x, screen.Y + pad + BloodSword::HalfTile, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

                overlay.VerifyAndAdd(Scene::Element(texture, icon_x, screen.Y + pad + BloodSword::HalfTile));
            }
        }

        // add last button if requested
        if (button != Controls::Type::NONE && asset != Asset::Type::NONE)
        {
            auto id = assets.size();

            overlay.VerifyAndAdd(Scene::Element(Asset::Get(asset), screen.X + id * (BloodSword::TileSize + pad) + offset, screen.Y + pad + BloodSword::HalfTile));

            overlay.Add(Controls::Base(button, id, id > 0 ? id - 1 : id, id, id, id, screen.X + id * (BloodSword::TileSize + pad) + offset, screen.Y + pad + BloodSword::HalfTile, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));
        }

        return overlay;
    }

    // icons select popup interface
    std::vector<int> SelectIcons(Graphics::Base &graphics, Scene::Base &background, const char *message, Asset::List assets, std::vector<int> values, std::vector<std::string> captions, int min_select, int max_select, Asset::Type asset_hidden, bool hidden = false, bool centered = true, bool blur = true)
    {
        auto selected_symbols = std::vector<int>();

        auto selection = std::vector<bool>(assets.size());

        auto controls = Controls::List(selection.size(), Controls::Type::SELECT);

        auto texture = Graphics::CreateText(graphics, message, Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        auto has_captions = (captions.size() > 0 && captions.size() <= assets.size());

        auto texture_captions = has_captions ? Graphics::CreateText(graphics, Graphics::GenerateTextList(captions, Fonts::Caption, Color::Active, 0)) : BloodSword::Textures();

        auto ordering_captions = std::vector<std::string>();

        for (auto i = 0; i < assets.size(); i++)
        {
            ordering_captions.push_back(std::to_string(i + 1));
        }

        auto texture_ordering = Graphics::CreateText(graphics, Graphics::GenerateTextList(ordering_captions, Fonts::Caption, Color::Active, 0));

        auto input = Controls::User();

        auto done = false;

        auto popup_pad = BloodSword::QuarterTile;

        auto num_icons = int(assets.size()) + ((min_select == 1 && max_select == 1) ? 0 : 1);

        auto min_icons = std::max(4, num_icons);

        auto popup_w = std::max(min_icons * BloodSword::TileSize + (min_icons - 1) * BloodSword::Pad, BloodSword::Width(texture) + popup_pad * 2);

        auto popup_h = (BloodSword::TileSize + popup_pad) * 2;

        auto popup = Point(graphics.Width - popup_w, graphics.Height - popup_h) / 2;

        auto final_assets = assets;

        InterfaceLogger::LogOptions(assets, values, "[ORDER]");

        if (hidden)
        {
            // shuffle values
            for (auto i = 0; i < assets.size(); i++)
            {
                final_assets[i] = asset_hidden;

                std::shuffle(values.begin(), values.end(), Engine::Random.Generator());
            }

            InterfaceLogger::LogOptions(assets, values, "[SHUFFLE]");
        }

        auto last_control = (min_select == 1 && max_select == 1) ? Controls::Type::NONE : Controls::Type::CONFIRM;

        auto last_asset = (min_select == 1 && max_select == 1) ? Asset::Type::NONE : Asset::Type::CONFIRM;

        while (!done)
        {
            auto overlay = Interface::IconList(Point(0, 0), graphics.Width, graphics.Height, final_assets, controls, popup_w, popup_h, Color::Background, Color::Active, BloodSword::Border, last_control, last_asset);

            // title
            auto center_title = centered ? ((popup_w - BloodSword::Width(texture)) / 2) : BloodSword::Pad;

            overlay.VerifyAndAdd(Scene::Element(texture, popup.X + center_title, popup.Y + BloodSword::Pad));

            for (auto i = 0; i < selection.size(); i++)
            {
                if (selection[i] && Input::IsValid(overlay, i, selection.size()))
                {
                    auto &control = overlay.Controls[i];

                    // highlight current selection
                    overlay.Add(Scene::Element(Point(control.X + 2 * control.Pixels, control.Y + 2 * control.Pixels), control.W - 4 * control.Pixels, control.H - 4 * control.Pixels, Color::Transparent, Color::Active, BloodSword::Pixel));

                    if (!has_captions && max_select > 1)
                    {
                        for (auto j = 0; j < selected_symbols.size(); j++)
                        {
                            if (values[i] == selected_symbols[j])
                            {
                                auto center = 0;

                                if (centered)
                                {
                                    // center texture
                                    center = (control.W - BloodSword::Width(texture_ordering[j])) / 2;

                                    if ((control.X + center < (popup.X + BloodSword::QuarterTile)) && input.Current == 0)
                                    {
                                        center = 0;
                                    }
                                }

                                overlay.VerifyAndAdd(Scene::Element(texture_ordering[j], control.X + center, control.Y + control.H + BloodSword::Pad));
                            }
                        }
                    }
                }
            }

            if (Input::IsValid(overlay, input) && !hidden)
            {
                // captions
                if (input.Type != Controls::Type::CONFIRM && Input::IsValid(overlay, input, texture_captions.size()))
                {
                    auto &control = overlay.Controls[input.Current];

                    if (texture_captions[input.Current])
                    {
                        auto center = 0;

                        // do not center caption on first icon
                        if (centered)
                        {
                            // center texture
                            center = (control.W - BloodSword::Width(texture_captions[input.Current])) / 2;

                            if ((control.X + center < (popup.X + BloodSword::QuarterTile)) && input.Current == 0)
                            {
                                center = 0;
                            }
                        }

                        overlay.VerifyAndAdd(Scene::Element(texture_captions[input.Current], control.X + center, control.Y + control.H + BloodSword::Pad));
                    }
                }
            }

            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, blur);

            if (Input::Check(input))
            {
                if (input.Type == Controls::Type::CONFIRM)
                {
                    if (selected_symbols.size() >= min_select && selected_symbols.size() <= max_select)
                    {
                        done = Interface::Confirm(graphics, background, "PROCEED?", Color::Background, Color::Active, BloodSword::Border, Color::Highlight, true);
                    }
                }
                else if (input.Type == Controls::Type::SELECT && input.Current >= 0 && input.Current < assets.size())
                {
                    if (min_select == 1 && max_select == 1)
                    {
                        // if min / max selection is 1, skip confirmation message box
                        selected_symbols = {values[input.Current]};

                        done = true;
                    }
                    else
                    {
                        if ((!selection[input.Current] && selected_symbols.size() < max_select) || selection[input.Current])
                        {
                            selection[input.Current] = !selection[input.Current];

                            if (selection[input.Current])
                            {
                                selected_symbols.push_back(values[input.Current]);

                                InterfaceLogger::LogChoice("SELECTED", assets[values[input.Current]], input.Current, selected_symbols.size());
                            }
                            else
                            {
                                selected_symbols.erase(std::find(selected_symbols.begin(), selected_symbols.end(), values[input.Current]));

                                InterfaceLogger::LogChoice("DESELECTED", assets[values[input.Current]], input.Current, selected_symbols.size());
                            }
                        }
                    }
                }

                input.Selected = false;
            }
        }

        BloodSword::Free(texture_ordering);

        BloodSword::Free(texture_captions);

        BloodSword::Free(&texture);

        return selected_symbols;
    }

    // helper string function (party/character is dead)
    std::string DeathMessage(Party::Base &party)
    {
        auto death = std::string();

        if (party.Count() > 1)
        {
            death = Interface::GetText(Interface::MSG_DEAD);
        }
        else
        {
            death = Interface::GetText(Interface::MSG_DIED);
        }

        return death;
    }

    // stake a [QUANTITY] of an [ITEM]
    bool StakeQuantity(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Item::Type to_stake, Asset::Type stake_asset)
    {
        auto proceed = false;

        if (Engine::IsAlive(party) && party.ChosenCharacter != Character::Class::NONE && to_stake != Item::Type::NONE)
        {
            auto player = party.ChosenCharacter;

            auto total_staked = 0;

            auto done = false;

            while (!done)
            {
                auto stake = player;

                if (Engine::Count(party) > 1)
                {
                    stake = Interface::SelectCharacter(graphics, background, party, "WHO IS ADDING TO THE STAKE?", true, true, false, false, true);
                }

                std::string stake_message = "HOW MUCH " + std::string(Item::TypeMapping[to_stake]) + " TO STAKE?";

                if (stake != Character::Class::NONE)
                {
                    auto staked = Interface::GetNumber(graphics, background, stake_message.c_str(), 0, party[stake].Quantity(to_stake), stake_asset, Asset::Type::UP, Asset::Type::DOWN);

                    if (staked > 0)
                    {
                        party[stake].Remove(to_stake, staked);

                        total_staked += staked;
                    }
                }

                if (Engine::Count(party) == 1 || stake == Character::Class::NONE)
                {
                    std::string message = "STAKED: " + std::to_string(total_staked) + " " + std::string(Item::TypeMapping[to_stake]) + ", PROCEED?";

                    if (Interface::Confirm(graphics, background, message.c_str(), Color::Background, Color::Active, BloodSword::Border, Color::Active, true))
                    {
                        done = true;
                    }
                }
            }

            if (total_staked > 0)
            {
                std::string stake_variable = "STAKED " + std::string(Item::TypeMapping[to_stake]);

                party.Set(stake_variable, std::to_string(total_staked));
            }

            proceed = (total_staked > 0);

            if (!proceed)
            {
                auto character = Engine::FirstClass(party);

                if (character != Character::Class::NONE)
                {
                    // refund to first character
                    party[character].Add(to_stake, total_staked);
                }
            }
        }
        else if (party.ChosenCharacter == Character::Class::NONE)
        {
            Interface::InternalError(graphics, background, "Internal Error: STAKE QUANTITY (NO PLAYER SELECTED)");
        }
        else if (to_stake == Item::Type::NONE)
        {
            Interface::InternalError(graphics, background, "Internal Error: STAKE QUANTITY (NONE STAKED)");
        }
        else
        {
            Interface::MessageBox(graphics, background, Interface::DeathMessage(party), Color::Highlight);
        }

        return proceed;
    }

    // collect money into a pool
    bool Collect(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Item::Type to_collect, Asset::Type collect_asset, int min_collect, int max_collect)
    {
        auto proceed = false;

        if (Engine::IsAlive(party) && to_collect != Item::Type::NONE)
        {
            auto player = party.ChosenCharacter;

            auto total_collected = 0;

            auto done = false;

            while (!done)
            {
                auto from = player;

                if (Engine::Count(party) > 1)
                {
                    from = Interface::SelectCharacter(graphics, background, party, "WHO IS ADDING TO THE POOL?", true, true, false, false, true);
                }

                std::string stake_message = "HOW MUCH " + std::string(Item::TypeMapping[to_collect]) + " TO ADD?";

                if (from != Character::Class::NONE)
                {
                    auto collected = Interface::GetNumber(graphics, background, stake_message.c_str(), 0, party[from].Quantity(to_collect), collect_asset, Asset::Type::UP, Asset::Type::DOWN);

                    if (collected > 0)
                    {
                        party[from].Remove(to_collect, collected);

                        total_collected += collected;
                    }
                }

                if ((Engine::Count(party) == 1 || from == Character::Class::NONE))
                {
                    std::string message = "COLLECTED: " + std::to_string(total_collected) + " " + std::string(Item::TypeMapping[to_collect]) + ", PROCEED?";

                    if (Interface::Confirm(graphics, background, message.c_str(), Color::Background, Color::Active, BloodSword::Border, Color::Active, true))
                    {
                        done = true;
                    }
                }
            }

            proceed = (total_collected >= min_collect && total_collected <= max_collect);

            if (!proceed)
            {
                auto character = Engine::FirstClass(party);

                if (character != Character::Class::NONE)
                {
                    // refund to first character
                    party[character].Add(to_collect, total_collected);
                }
            }
        }
        else if (to_collect == Item::Type::NONE)
        {
            Interface::InternalError(graphics, background, "Internal Error: COLLECT QUANTITY (NOTHING TO COLLECT)");
        }
        else
        {
            Interface::MessageBox(graphics, background, Interface::DeathMessage(party), Color::Highlight);
        }

        return proceed;
    }

    // eat food
    void EatFood(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Asset::Type food, int gain = 1, int limit = Item::Unlimited)
    {
        if (Engine::IsAlive(party))
        {
            auto done = false;

            auto food_left = limit;

            while (!done || (food_left != Item::Unlimited && food_left > 0))
            {
                auto eater = Engine::FirstClass(party);

                if (Engine::Count(party) > 1)
                {
                    eater = Interface::SelectCharacter(graphics, background, party, "WHO IS EATING?", true, true, false, false, true);
                }

                if (eater != Character::Class::NONE)
                {
                    auto max_heal = party[eater].Maximum(Attribute::Type::ENDURANCE) - party[eater].Value(Attribute::Type::ENDURANCE);

                    auto max_food = (limit != Item::Unlimited) ? food_left : max_heal;

                    auto eaten = Interface::GetNumber(graphics, background, "HOW MANY PORTIONS TO EAT?", 0, std::min(max_food, max_heal), food, Asset::Type::UP, Asset::Type::DOWN);

                    if (eaten > 0)
                    {
                        Engine::GainEndurance(party[eater], eaten * gain);

                        if (limit != Item::Unlimited)
                        {
                            food_left -= eaten;
                        }
                    }
                }

                if (Engine::Count(party) == 1 || eater == Character::Class::NONE || food_left == 0 || Engine::Healed(party))
                {
                    if (Interface::Confirm(graphics, background, "PROCEED?", Color::Background, Color::Active, BloodSword::Border, Color::Active, true))
                    {
                        done = true;
                    }
                }
            }
        }
        else
        {
            Interface::MessageBox(graphics, background, Interface::DeathMessage(party), Color::Highlight);
        }
    }

    // change battle order
    bool BattleOrder(Graphics::Base &graphics, Scene::Base &background, Party::Base &party)
    {
        auto assets = Asset::List();

        auto values = std::vector<int>(party.Count());

        std::iota(values.begin(), values.end(), 0);

        for (auto i = 0; i < party.Count(); i++)
        {
            assets.push_back(party[i].Asset);
        }

        auto done = false;

        while (!done)
        {
            auto selection = Interface::SelectIcons(graphics, background, "SET BATTLE ORDER", assets, values, {}, party.Count(), party.Count(), Asset::Type::NONE, false, true);

            if (selection.size() == party.Count())
            {
                auto characters = Party::List();

                for (auto i = 0; i < selection.size(); i++)
                {
                    characters.push_back(party[selection[i]]);
                }

                party.Clear();

                for (auto i = 0; i < characters.size(); i++)
                {
                    party.Add(characters[i]);
                }

                done = true;
            }
        }

        return true;
    }

    // permanetly gain or lose attribute points
    bool PermanentAttributeGain(Graphics::Base &graphics, Scene::Base &background, Character::Base &character, Attribute::Type attribute, int gain)
    {
        auto done = false;

        if ((gain > 0 || (gain < 0 && character.Value(attribute) > (1 - gain) && character.Maximum(attribute) > (1 - gain))) && attribute != Attribute::Type::NONE)
        {
            Engine::ModifyAttribute(character, attribute, gain);

            done = true;
        }
        else
        {
            std::string error = std::string("CANNOT ADJUST ") + Attribute::TypeMapping[attribute] + " CANNOT BE ADJUSTED FURTHER!";

            Interface::MessageBox(graphics, background, error, Color::Highlight);
        }

        return done;
    }

    // permanetly gain or lose attribute points (PLAYER)
    void PermanentAttributeGain(Graphics::Base &graphics, Scene::Base &background, Character::Base &character, int attributes, int gain)
    {
        std::vector<Attribute::Type> attribute_list = {
            Attribute::Type::FIGHTING_PROWESS,
            Attribute::Type::AWARENESS,
            Attribute::Type::PSYCHIC_ABILITY,
            Attribute::Type::ENDURANCE};

        auto captions = std::vector<std::string>();

        auto values = std::vector<int>(attribute_list.size());

        std::iota(values.begin(), values.end(), 0);

        auto assets = Asset::List();

        for (auto i = 0; i < attribute_list.size(); i++)
        {
            auto attribute = attribute_list[i];

            assets.push_back(Attribute::Assets[attribute]);

            captions.push_back(Attribute::TypeMapping[attribute]);
        }

        if (Engine::IsAlive(character))
        {
            for (auto i = 0; i < attributes; i++)
            {
                auto done = false;

                while (!done)
                {
                    std::string message = character.Name + ":" + " " + std::string(gain > 0 ? "GAIN" : "LOSE") + " " + std::to_string(std::abs(gain)) + " POINT" + (gain > 1 ? "S" : "") + " TO ONE ATTRIBUTE (PERMANENT)";

                    auto selection = Interface::SelectIcons(graphics, background, message.c_str(), assets, values, captions, 1, 1, Asset::Type::NONE, false, true);

                    if (selection.size() == 1)
                    {
                        auto attribute = attribute_list[selection[0]];

                        done = Interface::PermanentAttributeGain(graphics, background, character, attribute, gain);
                    }
                }
            }
        }
    }

    // permanetly gain or lose attribute points (PARTY)
    void PermanentAttributeGain(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, int attributes, int gain)
    {
        for (auto character = 0; character < party.Count(); character++)
        {
            if (Engine::IsAlive(party[character]))
            {
                Interface::PermanentAttributeGain(graphics, background, party[character], attributes, gain);
            }
        }
    }

    // select character interface (with specific mode)
    Character::Class SelectCharacter(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, std::string variable, std::string message, Interface::Mode mode = Interface::Mode::NONE)
    {
        auto character_string = Engine::ToUpper(variable);

        auto character = Character::Class::NONE;

        if (Engine::IsAlive(party))
        {
            if (character_string == "FIRST")
            {
                character = Engine::FirstClass(party);
            }
            else if (character_string == "LAST")
            {
                character = Engine::LastClass(party);
            }
            else if (character_string == "CHOSEN")
            {
                character = party.ChosenCharacter;

                if (character == Character::Class::NONE || Engine::Count(party) == 1)
                {
                    character = Engine::FirstClass(party);
                }
            }
            else if (character_string == "SELECT" || character_string == "CHOOSE")
            {
                if (Engine::Count(party) == 1)
                {
                    character = Engine::FirstClass(party);
                }
                else if (mode == Mode::NONE)
                {
                    character = Interface::SelectCharacter(graphics, background, party, message.c_str(), true, false, false, false, true);
                }
                else
                {
                    character = Interface::SelectCharacter(graphics, background, party, message.c_str(), mode, false, true);
                }
            }
            else if (party.IsPresent(character_string))
            {
                // check if variable is present in internal variables
                character = Character::Map(party.Get(character_string));
            }
            else if (party.Has(character_string))
            {
                // check if there is named character in the party
                auto named = party.Find(character_string);

                if (named >= 0 && named < party.Count())
                {
                    character = party[named].Class;
                }
            }
            else
            {
                // default: try to map character class
                character = Character::Map(character_string);
            }
        }

        if (character != Character::Class::NONE && party.Has(character))
        {
            party.ChosenCharacter = character;
        }

        return character;
    }

    // select character interface
    Character::Class SelectCharacter(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, std::string variable, Interface::Mode mode = Interface::Mode::NONE)
    {
        return Interface::SelectCharacter(graphics, background, party, variable, "SELECT PLAYER", mode);
    }

    // get character class mapped to control type
    Character::Class GetCharacter(Controls::Type control)
    {
        auto character = Character::Class::NONE;

        for (auto &keys : Interface::CharacterControls)
        {
            if (keys.second == control)
            {
                character = keys.first;

                break;
            }
        }

        return character;
    }

    // check if control type is a character control
    bool IsCharacter(Controls::Type control)
    {
        return Interface::GetCharacter(control) != Character::Class::NONE;
    }

    // check character's encumbrance limit
    bool CheckEncumbranceLimit(Character::Base &character)
    {
        return !Engine::IsAlive(character) || (character.EncumbranceLimit == Items::Unlimited) || (character.TotalEncumbrance() <= character.EncumbranceLimit);
    }

    // check if character can receive
    bool CanReceive(Character::Base &character)
    {
        return Engine::IsAlive(character) && ((character.EncumbranceLimit == Items::Unlimited) || (character.TotalEncumbrance() < character.EncumbranceLimit));
    }

    // check party item limit
    bool CheckEncumbranceLimit(Party::Base &party)
    {
        auto ok = true;

        for (auto i = 0; i < party.Count(); i++)
        {
            ok &= Interface::CheckEncumbranceLimit(party[i]);
        }

        return ok;
    }

    // check if anyone in the party can receive an item
    bool CanReceive(Party::Base &party)
    {
        auto ok = false;

        for (auto i = 0; i < party.Count(); i++)
        {
            ok |= Interface::CanReceive(party[i]);
        }

        return ok;
    }

    // take (default) items
    void TakeItems(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Item::Type item, Asset::Type asset, int limit = Item::Unlimited)
    {
        if (Engine::IsAlive(party))
        {
            auto done = false;

            auto items_left = limit;

            if (Items::Found(item))
            {
                while (!done || (limit != Item::Unlimited && items_left > 0))
                {
                    auto taker = Engine::FirstClass(party);

                    if (Engine::Count(party) > 1)
                    {
                        std::string message = "WHO IS TAKING THE " + Items::Defaults[item].Name + ((limit == Items::Unlimited || items_left > 1) ? "S?" : "?");

                        taker = Interface::SelectCharacter(graphics, background, party, message.c_str(), true, true, false, false, true);
                    }

                    if (taker != Character::Class::NONE)
                    {
                        auto max_take = (limit != Item::Unlimited) ? items_left : party[taker].SpaceLeft();

                        std::string message = "HOW MANY " + Items::Defaults[item].Name + (items_left > 1 ? "S" : "") + " TO TAKE?";

                        auto taken = Interface::GetNumber(graphics, background, message.c_str(), 0, std::min(max_take, party[taker].SpaceLeft()), asset, Asset::Type::UP, Asset::Type::DOWN);

                        if (taken > 0)
                        {
                            for (auto i = 0; i < taken; i++)
                            {
                                party[taker].Add(Items::Defaults[item]);
                            }

                            if (limit != Item::Unlimited)
                            {
                                items_left -= taken;
                            }
                        }
                    }

                    if (Engine::Count(party) == 1 || taker == Character::Class::NONE || items_left == 0)
                    {
                        if (Interface::Confirm(graphics, background, "PROCEED?", Color::Background, Color::Active, BloodSword::Border, Color::Active, true))
                        {
                            done = true;
                        }
                    }
                }
            }
            else
            {
                Interface::InternalError(graphics, background, "Internal Error: ITEM NOT FOUND");
            }
        }
        else
        {
            Interface::MessageBox(graphics, background, Interface::DeathMessage(party), Color::Highlight);
        }
    }

    // take / read items. each item has an infinite amount of supply, similar to take food, take item
    void TakeFromInfiniteList(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Class character, std::vector<Item::Type> items, bool blur = true)
    {
        if (character == Character::Class::NONE || items.size() == 0 || !party.Has(character) || !Engine::IsAlive(party[character]))
        {
            auto message = std::string();

            if (character == Character::Class::NONE || items.size() == 0)
            {
                Interface::InternalError(graphics, background, std::string("Internal Error: TakeFromInfiniteList"));
            }
            else if (!party.Has(character))
            {
                message = Engine::NotInParty(character);
            }
            else if (!Engine::IsAlive(party[character]))
            {
                message = Engine::IsDead(party[character]);
            }

            Interface::MessageBox(graphics, background, message, Color::Highlight);

            return;
        }

        auto limit = std::min(4, int(items.size()));

        auto start = 0;

        auto last = start + limit;

        auto options = int(items.size());

        auto text_list = Graphics::TextList();

        for (auto &item : items)
        {
            std::string item_string = Item::TypeMapping[item];

            text_list.push_back(Graphics::RichText(item_string, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, BloodSword::Wrap));
        }

        auto menu = Graphics::CreateText(graphics, text_list);

        // padding
        auto pads = BloodSword::LargePad;

        // default width
        auto w = std::max(BloodSword::Width(menu) + pads, BloodSword::Wrap);

        // default height
        auto h = std::max(BloodSword::Height(menu) + pads, BloodSword::TileSize);

        auto x = (graphics.Width - w) / 2 - (items.size() > limit ? (BloodSword::HalfTile + 1) : 0);

        auto y = (graphics.Height - h * (limit + 1)) / 2 - BloodSword::HalfTile + BloodSword::Pad;

        auto input = Controls::User();

        auto done = false;

        auto frame_x = x - BloodSword::HalfTile;

        auto frame_y = y - BloodSword::HalfTile + BloodSword::Pad;

        auto frame_w = w + (options > limit ? BloodSword::DoubleTile : BloodSword::TileSize);

        auto frame_h = (limit * h) + (BloodSword::FrameHeight) + BloodSword::OddPad;

        while (!done)
        {
            auto overlay = Interface::Menu(menu, x, y, w, h, start, last, limit, Color::Background, Color::Background, Color::Active, true);

            // add frame at the back
            overlay.Elements.insert(overlay.Elements.begin(), Scene::Element(frame_x, frame_y, frame_w, frame_h, Color::Background, Color::Active, BloodSword::Border));

            auto &lastControl = overlay.Controls.back();

            auto id = lastControl.Id + 1;

            auto first = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

            auto bottom = overlay.Controls[first + limit - 1].Y + h + BloodSword::LargePad;

            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), x - BloodSword::SmallPad, bottom));

            overlay.Add(Controls::Base(Controls::Type::BACK, id, id, id, first + limit - 1, id, x - BloodSword::SmallPad, bottom, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            Interface::ClearScrolling(overlay, input, Controls::Type::SCROLL_UP, Controls::Type::SCROLL_DOWN);

            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

            if (Input::Validate(input))
            {
                if (input.Type == Controls::Type::BACK)
                {
                    done = true;
                }
                else if (Input::IsUp(input))
                {
                    Interface::ScrollUp(overlay, input, Controls::Type::SCROLL_UP, options, limit, start, last);
                }
                else if (Input::IsDown(input))
                {
                    Interface::ScrollDown(overlay, input, Controls::Type::SCROLL_DOWN, options, limit, start, last);
                }
                else if (input.Type == Controls::Type::CHOICE)
                {
                    auto list = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                    auto choice = start + (input.Current - list);

                    if (choice >= 0 && choice < items.size())
                    {
                        // Take / Info
                        if (Items::FoundDescription(items[choice]) && Items::Found(items[choice]))
                        {
                            auto text = Items::Defaults[items[choice]].Name + "\n\n" + Items::Descriptions[items[choice]];

                            if (Interface::TextBoxConfirm(graphics, background, text, Color::Active, BloodSword::OctaTile))
                            {
                                if (Interface::CanReceive(party[character]))
                                {
                                    party[character].Add(Items::Defaults[items[choice]]);

                                    auto message = party[character].Name + " TAKES THE " + Item::TypeMapping[items[choice]] + ".";

                                    Interface::MessageBox(graphics, background, message, Color::Active);
                                }
                                else
                                {
                                    Interface::ErrorMessage(graphics, background, MSG_FULL);
                                }
                            }
                        }
                        else
                        {
                            Interface::InternalError(graphics, background, std::string("Internal Error: TakeFromInfiniteList"));
                        }
                    }
                }
            }
        }

        BloodSword::Free(menu);
    }

    // resurrect character
    void Resurrect(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Base &character)
    {
        if (Engine::IsAlive(character) && character.IsPlayer() && character.Has(Item::Property::RESURRECTION) && !party.Is("=", std::string(Character::ClassMapping[character.Class]) + " RESURRECTED", "TRUE"))
        {
            auto item = character.Find(Item::Property::RESURRECTION);

            if (item != character.Items.end())
            {
                if (Book::IsDefined((*item).Effects))
                {
                    Interface::ShowBookDescription(graphics, background, (*item).Effects);
                }

                character.Value(Attribute::Type::ENDURANCE, character.Maximum(Attribute::Type::ENDURANCE));

                party.Set(character.Name + " RESURRECTED", "TRUE");
            }
        }
    }

    // deal random damage to character (display)
    int DamagePlayer(Graphics::Base &graphics, Scene::Base &background, Character::Base &character, int roll, int modifier, bool ignore_armour = false, bool in_battle = false, bool display = true)
    {
        auto damage = 0;

        if (display)
        {
            damage = Interface::Roll(graphics, background, character.Asset, Asset::Type::DAMAGE, roll, modifier).Sum;
        }
        else
        {
            auto roll_result = Engine::Roll(roll, modifier);

            damage = roll_result.Sum;
        }

        damage -= ignore_armour ? 0 : Engine::Score(character, Attribute::Type::ARMOUR, in_battle);

        if (display)
        {
            if (damage > 0)
            {
                Sound::Play(Sound::Type::COMBAT_DAMAGE);

                Engine::GainEndurance(character, -damage, in_battle);

                std::string message = character.Name + " LOSES " + std::to_string(damage) + " ENDURANCE";

                Interface::MessageBox(graphics, background, message, Color::Highlight);
            }
            else
            {
                std::string message = character.Name + " UNHARMED";

                Interface::MessageBox(graphics, background, message, Color::Inactive);
            }
        }

        return damage;
    }

    // damage num_targets in party
    void DamageParty(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, int num_targets, int rounds, int roll, int modifier, bool ignore_armour = false, bool in_battle = false, bool display = true)
    {
        auto targets = std::vector<int>();

        for (auto i = 0; i < party.Count(); i++)
        {
            if (Engine::CanTarget(party[i], in_battle))
            {
                targets.push_back(i);
            }
        }

        num_targets = std::min(num_targets, int(targets.size()));

        for (auto i = 0; i < (num_targets * num_targets); i++)
        {
            std::shuffle(targets.begin(), targets.end(), Engine::Random.Generator());
        }

        for (auto i = 0; i < rounds; i++)
        {
            if (targets.size() > 0)
            {
                for (auto target = 0; target < num_targets; target++)
                {
                    Interface::DamagePlayer(graphics, background, party[targets[target]], roll, modifier, ignore_armour, in_battle, display);

                    Interface::Resurrect(graphics, background, party, party[targets[target]]);
                }
            }

            // remove from list of targets if incapacitated
            for (auto target = targets.begin(); target != targets.end();)
            {
                if (!Engine::IsAlive(party[*target]))
                {
                    target = targets.erase(target);
                }
                else
                {
                    ++target;
                }
            }

            num_targets = std::min(num_targets, int(targets.size()));

            // shuffle
            std::shuffle(targets.begin(), targets.end(), Engine::Random.Generator());
        }
    }

    // deal damage to members of party (selected at random) for several rounds (display)
    void DamageParty(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, int rounds, int roll, int modifier, bool ignore_armour = false, bool in_battle = false, bool display = true)
    {
        return Interface::DamageParty(graphics, background, party, rounds, 1, roll, modifier, ignore_armour, in_battle, display);
    }

    // select dice/die interface
    Engine::RollResult SelectDice(Graphics::Base &graphics, Scene::Base &background, std::string message, int number)
    {
        auto result = Engine::RollResult();

        Asset::List assets = {
            Asset::Type::DICE1,
            Asset::Type::DICE2,
            Asset::Type::DICE3,
            Asset::Type::DICE4,
            Asset::Type::DICE5,
            Asset::Type::DICE6};

        std::vector<int> values = {0, 1, 2, 3, 4, 5};

        auto done = false;

        while (!done)
        {
            auto selection = Interface::SelectIcons(graphics, background, message.c_str(), assets, values, {}, number, number, Asset::Type::NONE, false, false);

            if (selection.size() == number)
            {
                for (auto i = 0; i < selection.size(); i++)
                {
                    if (selection[i] >= 0 && selection[i] < values.size())
                    {
                        result.Sum += (values[selection[i]] + 1);

                        result.Rolls.push_back(values[selection[i]] + 1);
                    }
                }

                done = true;
            }
        }

        return result;
    }

    // select dice interface
    int SelectDice(Graphics::Base &graphics, Scene::Base &background, std::string message)
    {
        auto result = Interface::SelectDice(graphics, background, message, 1);

        return result.Sum;
    }

    // run one round of damage gauntlet
    void GauntletRound(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Gauntlet gauntlet, Asset::Type asset, std::string prefix)
    {
        if (Engine::IsAlive(party))
        {
            for (auto character = 0; character < party.Count(); character++)
            {
                if (Engine::IsAlive(party[character]))
                {
                    if (Interface::Roll(graphics, background, party[character].Asset, asset, gauntlet.Rolls, gauntlet.RollModifier).Sum < gauntlet.Difficulty)
                    {
                        std::string message = prefix + " " + party[character].Name + "!";

                        Interface::TextBox(graphics, background, message, Color::Highlight, BloodSword::Wrap, true);

                        Interface::DamagePlayer(graphics, background, party[character], gauntlet.Damage, gauntlet.DamageModifier, gauntlet.IgnoreArmour, false, true);

                        Interface::Resurrect(graphics, background, party, party[character]);
                    }
                }
            }
        }
    }

    // run through a gauntlet of damage
    bool RunGauntlet(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Gauntlet gauntlet, Asset::Type asset, std::string prefix)
    {
        auto result = false;

        if (Engine::IsAlive(party))
        {
            auto passed = 0;

            for (auto round = 0; round < gauntlet.Rounds; round++)
            {
                Interface::GauntletRound(graphics, background, party, gauntlet, asset, prefix);

                passed = round + 1;

                if (!Engine::IsAlive(party) || !Interface::Confirm(graphics, background, "CONTINUE?", Color::Background, Color::Active, BloodSword::Border, Color::Active, true))
                {
                    break;
                }
            }

            if (Engine::IsAlive(party) && passed != gauntlet.Rounds)
            {
                for (auto round = 0; round < passed; round++)
                {
                    Interface::GauntletRound(graphics, background, party, gauntlet, asset, prefix);
                }
            }
            else
            {
                result = true;
            }
        }
        else
        {
            Interface::MessageBox(graphics, background, Interface::DeathMessage(party), Color::Highlight);
        }

        return result;
    }

    // use multiple instances of an item that are spread across the party
    bool UseItems(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Item::Type item, int count)
    {
        auto used = false;

        if (Engine::Count(party, item) >= count)
        {
            auto select = Graphics::CreateText(graphics, "SELECT PLAYER", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

            auto selection = std::vector<bool>(party.Count());

            // count of items used by a player
            auto counts = std::vector<int>(party.Count());

            auto items = Interface::Items(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, BloodSword::PentaTile);

            auto captions = Interface::GenerateNameCaptions(graphics, party);

            auto pad = BloodSword::OddPad;

            auto input = Controls::User();

            auto done = false;

            auto popup_pad = BloodSword::QuarterTile;

            auto popup_w = std::max((party.Count() + 1) * (BloodSword::TileSize + popup_pad), BloodSword::Width(select) + popup_pad * 2);

            auto popup_h = (BloodSword::TileSize + BloodSword::QuarterTile) * 2;

            for (auto i = 0; i < items.size(); i++)
            {
                popup_h = std::max(popup_h, BloodSword::Height(items[i]));
            }

            auto popup = Point(graphics.Width - popup_w, graphics.Height - popup_h) / 2;

            while (!done)
            {
                auto overlay = Scene::Base();

                overlay = Interface::CharacterList(Point(0, 0), graphics.Width, graphics.Height, party, popup_w, popup_h, Color::Background, Color::Active, BloodSword::Border, Controls::Type::CONFIRM, Asset::Type::CONFIRM);

                // title
                overlay.VerifyAndAdd(Scene::Element(select, popup.X + BloodSword::QuarterTile, popup.Y + BloodSword::Pad));

                for (auto i = 0; i < selection.size(); i++)
                {
                    if (selection[i] && Input::IsValid(overlay, i, selection.size()))
                    {
                        auto &control = overlay.Controls[i];

                        // highlight current selection
                        overlay.Add(Scene::Element(Point(control.X + 2 * control.Pixels, control.Y + 2 * control.Pixels), control.W - 4 * control.Pixels, control.H - 4 * control.Pixels, Color::Transparent, Color::Active, BloodSword::Pixel));
                    }
                }

                if (Input::IsValid(overlay, input))
                {
                    // party popup captions
                    if (input.Type != Controls::Type::CONFIRM && Input::IsValid(overlay, input, party.Count()))
                    {
                        auto &control = overlay.Controls[input.Current];

                        // center caption
                        auto center = (control.W - BloodSword::Width(captions[input.Current])) / 2;

                        if ((control.X + center < (popup.X + BloodSword::QuarterTile)) && input.Current == 0)
                        {
                            center = 0;
                        }

                        overlay.VerifyAndAdd(Scene::Element(captions[input.Current], control.X + center, control.Y + control.H + pad));

                        auto items_x = popup.X + (popup_w + pad * 2);

                        overlay.Add(Scene::Element(items_x, popup.Y, BloodSword::PentaTile, popup_h, Color::Background, Color::Active, BloodSword::Border));

                        overlay.VerifyAndAdd(Scene::Element(items[input.Current], items_x, popup.Y));
                    }
                }

                input = Input::WaitForInput(graphics, background, overlay, input, true, true);

                if (Input::Check(input))
                {
                    if (input.Type == Controls::Type::CONFIRM)
                    {
                        done = Interface::Confirm(graphics, background, Graphics::RichText("PROCEED?", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Active, BloodSword::Border, Color::Highlight, true);
                    }
                    else if (Input::IsPlayer(input) && input.Current >= 0 && input.Current < party.Count())
                    {
                        if (Engine::IsAlive(party[input.Current]))
                        {
                            if (party[input.Current].Has(item))
                            {
                                selection[input.Current] = !selection[input.Current];

                                if (selection[input.Current])
                                {
                                    if (party[input.Current].Count(item) > 1)
                                    {
                                        auto item_count = party[input.Current].Count(item);

                                        std::string message = "HOW MANY " + Items::Defaults[item].Name + (item_count > 1 ? "S" : "") + " TO USE?";

                                        counts[input.Current] = Interface::GetNumber(graphics, background, message.c_str(), 1, item_count, Asset::Type::ITEMS, Asset::Type::UP, Asset::Type::DOWN);
                                    }
                                    else
                                    {
                                        counts[input.Current] = 1;
                                    }
                                }
                                else
                                {
                                    counts[input.Current] = 0;
                                }
                            }
                            else
                            {
                                std::string text = party[input.Current].Name + " DOES NOT HAVE THE " + std::string(Item::TypeMapping[item]) + "!";

                                Interface::MessageBox(graphics, background, text, Color::Highlight);
                            }
                        }
                        else
                        {
                            Interface::MessageBox(graphics, background, Engine::IsDead(party[input.Current]), Color::Highlight);
                        }
                    }

                    input.Selected = false;
                }
            }

            if (std::accumulate(counts.begin(), counts.end(), 0) >= count)
            {
                auto items_used = 0;

                for (auto character = 0; character < party.Count(); character++)
                {
                    if (counts[character] > 0)
                    {
                        for (auto to_use = 0; to_use < counts[character]; to_use++)
                        {
                            items_used++;

                            party[character].Remove(item);

                            if (items_used >= count)
                            {
                                break;
                            }
                        }
                    }

                    if (items_used >= count)
                    {
                        used = true;

                        break;
                    }
                }
            }

            BloodSword::Free(captions);

            BloodSword::Free(items);

            BloodSword::Free(&select);
        }

        return used;
    }

    // (character) show inventory while in story mode
    void DropItem(Graphics::Base &graphics, Scene::Base &background, Character::Base &character)
    {
        if (character.Items.size() == 0)
        {
            return;
        }

        auto limit = std::min(4, int(character.Items.size()));

        auto start = 0;

        auto last = start + limit;

        auto options = int(character.Items.size());

        auto text_list = Graphics::TextList();

        for (auto &item : character.Items)
        {
            text_list.push_back(Graphics::RichText(item.String(true), Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, BloodSword::Wrap));
        }

        auto menu = Graphics::CreateText(graphics, text_list);

        // padding
        auto pads = BloodSword::LargePad;

        // default width
        auto w = std::max(BloodSword::Width(menu) + pads, BloodSword::Wrap);

        // default height
        auto h = std::max(BloodSword::Height(menu) + pads, BloodSword::TileSize);

        auto x = (graphics.Width - w) / 2 - (character.Items.size() > limit ? (BloodSword::HalfTile + 1) : 0);

        auto y = (graphics.Height - h * (limit + 1)) / 2 - BloodSword::HalfTile + BloodSword::Pad;

        auto input = Controls::User();

        auto done = false;

        auto frame_x = x - BloodSword::HalfTile;

        auto frame_y = y - BloodSword::HalfTile + BloodSword::Pad;

        auto frame_w = w + (options > limit ? BloodSword::DoubleTile : BloodSword::TileSize);

        auto frame_h = (limit * h) + (BloodSword::FrameHeight) + BloodSword::OddPad;

        while (!done)
        {
            auto overlay = Interface::Menu(menu, x, y, w, h, start, last, limit, Color::Background, Color::Background, Color::Active, true);

            // add frame at the back
            overlay.Elements.insert(overlay.Elements.begin(), Scene::Element(frame_x, frame_y, frame_w, frame_h, Color::Background, Color::Active, BloodSword::Border));

            Interface::ClearScrolling(overlay, input, Controls::Type::SCROLL_UP, Controls::Type::SCROLL_DOWN);

            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

            if (Input::Validate(input))
            {
                if (Input::IsUp(input))
                {
                    Interface::ScrollUp(overlay, input, Controls::Type::SCROLL_UP, options, limit, start, last);
                }
                else if (Input::IsDown(input))
                {
                    Interface::ScrollDown(overlay, input, Controls::Type::SCROLL_DOWN, options, limit, start, last);
                }
                else if (input.Type == Controls::Type::CHOICE)
                {
                    auto list = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                    auto choice = start + (input.Current - list);

                    if (choice >= 0 && choice < character.Items.size())
                    {
                        if (!character.Items[choice].Has(Item::Property::CANNOT_DROP))
                        {
                            std::string drop_message = std::string("DROP THE ") + character.Items[choice].Name + "?";

                            if (Interface::Confirm(graphics, overlay, drop_message, Color::Background, Color::Active, BloodSword::Border, Color::Highlight, true))
                            {
                                // drop item
                                character.Items.erase(character.Items.begin() + choice);
                            }
                        }
                        else
                        {
                            std::string drop_message = character.Items[choice].Name + " CANNOT BE DROPPED";

                            Interface::MessageBox(graphics, background, drop_message, Color::Highlight);
                        }
                    }

                    if (character.Items.size() == 0 || character.Items.size() != options)
                    {
                        done = true;
                    }
                }
            }
        }

        BloodSword::Free(menu);
    }

    // implement chaos (random) potion effect
    void ChaosEffect(Graphics::Base &graphics, Scene::Base &background, Character::Base &character)
    {
        if (Engine::IsAlive(character))
        {
            auto effect = Interface::Roll(graphics, background, character.Asset, Asset::Type::DRINK, 1, 0).Sum;

            switch (effect)
            {
            case 1:
                Interface::MessageBox(graphics, background, "NOTHING HAPPENS", Color::Inactive);

                break;
            case 2:
                Interface::MessageBox(graphics, background, "+1 FIGHTING PROWESS", Color::Active);

                Interface::PermanentAttributeGain(graphics, background, character, Attribute::Type::FIGHTING_PROWESS, 1);

                break;
            case 3:
                Interface::MessageBox(graphics, background, "YOU GAIN TEMPORARY INVULNERABILITY", Color::Active);

                character.Add(Character::Status::TEMPORARY_INVULNERABILITY);

                break;
            case 4:
                Interface::MessageBox(graphics, background, "YOU ARE WEAKENED", Color::Highlight);

                character.Add(Character::Status::WEAKENED);

                break;
            case 5:
                Interface::MessageBox(graphics, background, "-1 AWARENESS", Color::Highlight);

                Interface::PermanentAttributeGain(graphics, background, character, Attribute::Type::AWARENESS, -1);

                break;
            case 6:
                Interface::MessageBox(graphics, background, "YOU GAIN TEMPORARY STRENGTH", Color::Active);

                character.Add(Character::Status::STRONG);

                break;
            default:
                break;
            }
        }
    }

    // Lose endurance or drop item
    void LoseEnduranceOrDropItem(Graphics::Base &graphics, Scene::Base &background, Character::Base &character)
    {
        auto limit = 2;

        auto start = 0;

        auto last = start + limit;

        auto options = 2;

        Graphics::TextList text_list = {
            Graphics::RichText("LOSE 1 ENDURANCE", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, BloodSword::Wrap),
            Graphics::RichText("DROP 1 ITEM", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, BloodSword::Wrap)};

        auto menu = Graphics::CreateText(graphics, text_list);

        // padding
        auto pads = BloodSword::LargePad;

        // default width
        auto w = std::max(BloodSword::Width(menu) + pads, BloodSword::Wrap);

        // default height
        auto h = std::max(BloodSword::Height(menu) + pads, BloodSword::TileSize);

        auto x = (graphics.Width - w) / 2 - (character.Items.size() > limit ? (BloodSword::HalfTile + 1) : 0);

        auto y = (graphics.Height - h * (limit + 1)) / 2 - BloodSword::HalfTile + BloodSword::Pad;

        auto input = Controls::User();

        auto done = false;

        auto frame_x = x - BloodSword::HalfTile;

        auto frame_y = y - BloodSword::HalfTile + BloodSword::Pad;

        auto frame_w = w + (options > limit ? BloodSword::DoubleTile : BloodSword::TileSize);

        auto frame_h = (limit * h) + (BloodSword::FrameHeight) + BloodSword::OddPad;

        while (!done)
        {
            auto overlay = Interface::Menu(menu, x, y, w, h, start, last, limit, Color::Background, Color::Background, Color::Active, true);

            // add frame at the back
            overlay.Elements.insert(overlay.Elements.begin(), Scene::Element(frame_x, frame_y, frame_w, frame_h, Color::Background, Color::Active, BloodSword::Border));

            Interface::ClearScrolling(overlay, input, Controls::Type::SCROLL_UP, Controls::Type::SCROLL_DOWN);

            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

            if (Input::Validate(input))
            {
                if (Input::IsUp(input))
                {
                    Interface::ScrollUp(overlay, input, Controls::Type::SCROLL_UP, options, limit, start, last);
                }
                else if (Input::IsDown(input))
                {
                    Interface::ScrollDown(overlay, input, Controls::Type::SCROLL_DOWN, options, limit, start, last);
                }
                else if (input.Type == Controls::Type::CHOICE)
                {
                    auto list = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                    auto choice = start + (input.Current - list);

                    if (choice >= 0 && choice < text_list.size())
                    {
                        if (choice == 0)
                        {
                            Interface::MessageBox(graphics, background, character.Name + " LOSES 1 ENDURANCE", Color::Highlight);

                            Engine::GainEndurance(character, -1, false);

                            done = true;
                        }
                        else if (choice == 1)
                        {
                            if (Engine::CanDrop(character))
                            {
                                Interface::MessageBox(graphics, background, character.Name + " NEEDS TO DROP 1 ITEM", Color::Inactive);

                                Interface::DropItem(graphics, background, character);

                                done = true;
                            }
                            else
                            {
                                Interface::MessageBox(graphics, background, character.Name + " DOES NOT HAVE ITEMS THAT CAN BE DROPPED", Color::Highlight);
                            }
                        }
                    }
                }
            }
        }

        BloodSword::Free(menu);
    }

    // generate base game filename
    std::string GameFile(int game)
    {
        std::string game_file = "game" + std::to_string(game) + ".json";

        return game_file;
    }

    // load module settings
    void LoadModule(std::string load)
    {
        Interface::SettingsFile = std::string();

        if (Interface::Modules.size() == 0 || load.empty())
        {
            throw std::invalid_argument("No modules loaded!");
        }
        else
        {
            load = Engine::ToUpper(load);

            // search for the settings file
            for (auto &module : Interface::Modules)
            {
                if (module.Id == load)
                {
                    Interface::SettingsFile = module.SettingsFile;

                    break;
                }
            }
        }

        if (Interface::SettingsFile.empty())
        {
            std::string error_message = "MODULE: " + load + " NOT LOADED!";

            throw std::invalid_argument(error_message.c_str());
        }
#if defined(DEBUG)
        else
        {
            std::cerr << "[LOADED] [MODULE: " << load << "]" << std::endl;
        }
#endif
    }

    // load modules list and select default
    void LoadModules()
    {
        Interface::Modules.clear();

        auto DefaultModule = std::string();

        std::ifstream ifs("modules/index.json", std::ios::in);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            DefaultModule = !data["default"].is_null() ? std::string(data["default"]) : std::string();

            if (!data["modules"].is_null() && data["modules"].is_array())
            {
                for (auto i = 0; i < data["modules"].size(); i++)
                {
                    auto module = Interface::Module();

                    if (!data["modules"][i].is_null() && data["modules"][i].is_object())
                    {
                        module.Id = !data["modules"][i]["id"].is_null() ? std::string(data["modules"][i]["id"]) : std::string();

                        module.Title = !data["modules"][i]["title"].is_null() ? std::string(data["modules"][i]["title"]) : std::string();

                        module.SettingsFile = !data["modules"][i]["settings"].is_null() ? std::string(data["modules"][i]["settings"]) : std::string();

                        if (!module.Id.empty() && !module.Title.empty() && !module.SettingsFile.empty())
                        {
                            Interface::Modules.push_back(module);
                        }
                    }
                }
            }

            ifs.close();
        }

        if (Interface::Modules.size() == 0)
        {
            throw std::invalid_argument("No modules loaded!");
        }
        else if (DefaultModule.empty())
        {
            DefaultModule = Interface::Modules[0].Id;
        }

        Interface::LoadModule(DefaultModule);
    }

    // return current module
    std::string CurrentModule()
    {
        auto current = std::string();

        if (Interface::Modules.size() == 0 || Interface::SettingsFile.empty())
        {
            Interface::LoadModules();
        }

        for (auto &module : Interface::Modules)
        {
            if (module.SettingsFile == Interface::SettingsFile)
            {
                current = module.Id;

                break;
            }
        }

        if (Interface::Modules.size() == 0 || Interface::SettingsFile.empty() || current.empty())
        {
            throw std::invalid_argument("No modules loaded!");
        }

        return current;
    }

    // initialize games list
    void InitializeGamesList()
    {
        Interface::CreateDirectories();

        auto GamesPath = Interface::GetGamesPath();

        if (Interface::GamesList.size() != Interface::MaxGameFiles)
        {
            Interface::GamesList.clear();

            for (auto game = 0; game < Interface::MaxGameFiles; game++)
            {
                std::string GameFile = Interface::GameFile(game);

                std::string Filename = GamesPath + "/" + GameFile;

                std::ifstream ifs(Filename, std::ios::in);

                auto saveGame = Interface::SavedGame();

                saveGame.Number = game;

                saveGame.Filename = GameFile;

                if (ifs.good())
                {
                    auto data = nlohmann::json::parse(ifs);

                    if (!data["party"].is_null() && data["party"].is_object())
                    {
                        auto party = Party::Initialize(data["party"]);

                        for (auto character = 0; character < party.Count(); character++)
                        {
                            saveGame.Players.push_back(party[character].Class);
                        }

                        saveGame.Location = party.SaveLocation;

                        auto file_time = fs::last_write_time(Filename.c_str());

                        // get last modified time
                        saveGame.TimeStamp = Engine::UtcTime(Engine::ConvertTime(file_time));

                        // check if adventure has been completed
                        if (party.Is("=", "COMPLETED", "TRUE"))
                        {
                            saveGame.Completed = true;
                        }
                    }

                    ifs.close();
                }

                Interface::GamesList.push_back(saveGame);
            }
        }
#if defined(DEBUG)
        std::cerr << "[ INIT ] [MAX " << Interface::GamesList.size() << " GAMES]" << std::endl;
#endif
    }

    // generate textures of book locations from games list
    BloodSword::Textures GameLocations(Graphics::Base &graphics, std::vector<Book::Location> &locations, std::string undefined)
    {
        BloodSword::Textures textures = {};

        for (auto &location : locations)
        {
            SDL_Texture *texture = nullptr;

            if (Book::IsUndefined(location))
            {
                texture = Graphics::CreateText(graphics, undefined.c_str(), Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);
            }
            else
            {
                texture = Graphics::CreateText(graphics, Book::String(location).c_str(), Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);
            }

            textures.push_back(texture);
        }

        return textures;
    }

    // get book locations from games list
    std::vector<Book::Location> GetGameLocations()
    {
        auto locations = std::vector<Book::Location>();

        Interface::InitializeGamesList();

        for (auto game = 0; game < Interface::GamesList.size(); game++)
        {
            locations.push_back(Interface::GamesList[game].Location);
        }

        return locations;
    }

    // generate game (file) timestamp textures from list of games
    BloodSword::Textures TimeStamps(Graphics::Base &graphics)
    {
        BloodSword::Textures timestamps = {};

        for (auto time = 0; time < Interface::GamesList.size(); time++)
        {
            SDL_Texture *texture = nullptr;

            if (!Interface::GamesList[time].TimeStamp.empty())
            {
                texture = Graphics::CreateText(graphics, Interface::GamesList[time].TimeStamp.c_str(), Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);
            }
            else
            {
                texture = Graphics::CreateText(graphics, "UNDEFINED", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);
            }

            timestamps.push_back(texture);
        }

        return timestamps;
    }

    // show completion message in a completed game
    void LoadCompletedGame(Graphics::Base &graphics, Scene::Base &background, Party::Base &party)
    {
        auto completion = party.Get("COMPLETION MESSAGE");

        auto texture = Graphics::CreateText(graphics, completion.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, BloodSword::DecaTile);

        auto texturex = (graphics.Width - BloodSword::Width(texture)) / 2 + BloodSword::Pad;

        auto panelh = BloodSword::DoubleTile + BloodSword::LargePad * 5 + BloodSword::Height(texture);

        auto panelw = BloodSword::DecaTile + BloodSword::LargePad;

        auto panelx = (graphics.Width - panelw) / 2;

        auto panely = (graphics.Height - panelh) / 2;

        auto iconx = (graphics.Width - BloodSword::TileSize) / 2;

        auto confirmy = panely + panelh - (BloodSword::TileSize + BloodSword::LargePad);

        auto input = Controls::User();

        auto done = false;

        while (!done)
        {
            auto overlay = Scene::Base();

            // render panel
            overlay.Add(Scene::Element(panelx, panely, panelw, panelh, Color::Background, Color::Active, BloodSword::Pixel));

            // render trophy icon
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::TROPHY), Point(iconx, panely + BloodSword::LargePad)));

            // render completion text
            overlay.VerifyAndAdd(Scene::Element(texture, Point(texturex, panely + BloodSword::TileSize + BloodSword::TriplePad)));

            // render confirmation button
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CONFIRM), Point(iconx, confirmy)));

            overlay.Add(Controls::Base(Controls::Type::CONFIRM, 0, 0, 0, 0, 0, iconx, confirmy, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

            if (Input::Validate(input))
            {
                if (input.Type == Controls::Type::CONFIRM)
                {
                    done = true;
                }

                input.Selected = false;
            }
        }

        BloodSword::Free(&texture);
    }

    // load or save a game (party information)
    bool LoadSaveGame(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Controls::Type mode, Asset::Type asset)
    {
        auto update = false;

        Interface::InitializeGamesList();

        auto max_games = Interface::GamesList.size();

        auto GamesPath = Interface::GetGamesPath();

        auto boxw = BloodSword::OctaTile;

        auto boxh = BloodSword::DoubleTile;

        auto boxx = (graphics.Width - boxw) / 2;

        auto input = Controls::User();

        auto done = false;

        auto panelh = boxh * (Interface::MaxGameFiles + 1) + BloodSword::Pad * Interface::MaxGameFiles;

        auto panelw = boxw + BloodSword::TileSize;

        auto panelx = (graphics.Width - panelw) / 2;

        auto panely = (graphics.Height - panelh) / 2;

        auto space = BloodSword::TileSize + BloodSword::Pad;

        auto controlsy = panely + panelh - space - BloodSword::SmallPad;

        auto game_locations = Interface::GetGameLocations();

        auto locations = Interface::GameLocations(graphics, game_locations, "NO GAME FOUND");

        auto timestamps = Interface::TimeStamps(graphics);

        std::string mode_string = std::string();

        switch (mode)
        {
        case Controls::Type::LOAD:

            mode_string = "LOAD GAME";

            break;

        case Controls::Type::SAVE:

            mode_string = "SAVE GAME";

            break;

        default:

            mode_string = "SELECT GAME";

            break;
        }

        auto GameMode = Graphics::CreateText(graphics, mode_string.c_str(), Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, 0);

        while (!done)
        {
            auto overlay = Scene::Base();

            // render panel
            overlay.Add(Scene::Element(panelx, panely, panelw, panelh, Color::Background, Color::Active, BloodSword::Pixel));

            auto id = 0;

            for (auto game = 0; game < max_games; game++)
            {
                auto currenty = panely + game * (boxh + BloodSword::LargePad) + BloodSword::TriplePad;

                // render subpanel
                auto has_game = (Book::IsDefined(Interface::GamesList[game].Location) && Interface::GamesList[game].Players.size() > 0);

                auto color = has_game ? Color::Active : Color::Inactive;

                overlay.Add(Scene::Element(boxx, currenty, boxw, boxh, Color::Background, color, BloodSword::Pixel));

                // render location
                overlay.VerifyAndAdd(Scene::Element(locations[game], Point(boxx + BloodSword::Pad, currenty + BloodSword::Pad)));

                auto charactery = currenty + BloodSword::EpicPad;

                if (has_game)
                {
                    // timestamp
                    overlay.VerifyAndAdd(Scene::Element(timestamps[game], Point(boxx + boxw - (BloodSword::Width(timestamps[game]) + BloodSword::Pad), currenty + BloodSword::Pad)));

                    // character icons
                    for (auto character = 0; character < Interface::GamesList[game].Players.size(); character++)
                    {
                        if (character < 5)
                        {
                            auto characterx = boxx + BloodSword::Pad + character * space;

                            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Interface::ClassAssets[Interface::GamesList[game].Players[character]]), Point(characterx, charactery)));
                        }
                    }

                    if (Interface::GamesList[game].Players.size() > 5)
                    {
                        auto characterx = boxx + BloodSword::Pad + 5 * space;

                        overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::RIGHT), Point(characterx, charactery)));
                    }
                }
                else
                {
                    // add dummy icon
                    auto characterx = boxx + BloodSword::Pad;

                    overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CHARACTER), Point(characterx, charactery)));
                }

                // game mode button
                auto up = id > 0 ? id - 1 : id;

                auto dn = id + 1;

                // check if game has been completed
                overlay.VerifyAndAdd(Scene::Element(Asset::Get(Interface::GamesList[game].Completed ? Asset::Type::TROPHY : asset), Point(boxx + boxw - (BloodSword::TileSize + BloodSword::Pad), charactery)));

                overlay.Add(Controls::Base(Controls::Type::CHOICE, id, id, id, up, dn, boxx + boxw - (BloodSword::TileSize + BloodSword::Pad), charactery, BloodSword::TileSize, BloodSword::TileSize, Color::Highlight));

                id++;
            }

            // game mode
            overlay.VerifyAndAdd(Scene::Element(GameMode, Point(boxx + boxw - (BloodSword::Width(GameMode) - BloodSword::Pixel), controlsy + BloodSword::Pad)));

            // add back button
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), Point(boxx - BloodSword::MidPad + 1, controlsy)));

            overlay.Add(Controls::Base(Controls::Type::BACK, id, id, id, id - 1, id, boxx - BloodSword::MidPad + 1, controlsy, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

            if (Input::Validate(input))
            {
                if (input.Type == Controls::Type::BACK)
                {
                    done = true;
                }
                else if (input.Type == Controls::Type::CHOICE)
                {
                    if (input.Current >= 0 && input.Current < max_games && input.Current < Interface::MaxGameFiles)
                    {
                        auto game = input.Current;

                        if (mode == Controls::Type::SAVE)
                        {
                            auto save = Book::IsUndefined(Interface::GamesList[game].Location) || Interface::Confirm(graphics, background, "OVERWRITE THIS GAME?", Color::Background, Color::Active, BloodSword::Border, Color::Highlight, true);

                            if (save)
                            {
                                auto GameFile = Interface::GameFile(game);

                                auto SaveFile = GamesPath + "/" + GameFile;

                                // set current module
                                party.Module = Interface::CurrentModule();

                                // save game (party)
                                Party::Save(party, SaveFile.c_str(), "party");

                                // update details in the saved games list
                                Interface::GamesList[game].Location = party.SaveLocation;

                                Interface::GamesList[game].Filename = GameFile;

                                Interface::GamesList[game].Players.clear();

                                for (auto character = 0; character < party.Count(); character++)
                                {
                                    Interface::GamesList[game].Players.push_back(party[character].Class);
                                }

                                auto file_time = fs::last_write_time(SaveFile.c_str());

                                Interface::GamesList[game].TimeStamp = Engine::UtcTime(Engine::ConvertTime(file_time));

                                // mark if game is completed
                                Interface::GamesList[game].Completed = party.Is("=", "COMPLETED", "TRUE");

                                update = true;

                                done = true;
                            }
                        }
                        else if (mode == Controls::Type::LOAD)
                        {
                            if (Book::IsDefined(Interface::GamesList[game].Location))
                            {
                                auto GameFile = Interface::GamesList[game].Filename;

                                auto LoadFile = GamesPath + "/" + GameFile;

                                auto temp_party = Party::Load(LoadFile, "party");

                                if (Book::IsDefined(temp_party.SaveLocation) && Book::Equal(temp_party.SaveLocation, Interface::GamesList[game].Location))
                                {
                                    if (temp_party.Is("=", "COMPLETED", "TRUE"))
                                    {
                                        // show completion message
                                        Interface::LoadCompletedGame(graphics, background, temp_party);
                                    }
                                    else
                                    {
                                        // load game
                                        party = temp_party;

                                        update = true;

                                        done = true;
                                    }
                                }
                            }
                            else
                            {
                                Sound::Play(Sound::Type::FAIL);
                            }
                        }
                    }
                }

                input.Selected = false;
            }
        }

        BloodSword::Free(&GameMode);

        BloodSword::Free(timestamps);

        BloodSword::Free(locations);

        return update;
    }

    // adjust ranks based on experience points (should only be called on "good" endings)
    bool AdjustRanks(Graphics::Base &graphics, Scene::Base &background, Party::Base &party)
    {
        auto saved = false;

        auto base_experience = 0;

        auto total_experience = 0;

        // copy party
        auto new_party = party;

        for (auto character = 0; character < party.Count(); character++)
        {
            if (Engine::IsAlive(party[character]) && !Character::OtherClass(party[character].Class))
            {
                base_experience += Generate::CalculateExperienceFromRank(party[character].Rank);

                total_experience += party[character].Experience;
            }
            else if (!Engine::IsAlive(party[character]) || Character::OtherClass(party[character].Class))
            {
                // remove incapacitated characters from the party
                new_party.Remove(party[character].Class);
            }
        }

        auto increased_rank = 0;

        for (auto character = 0; character < new_party.Count(); character++)
        {
            new_party[character].Rank = Generate::CalculateRankFromExperience(new_party[character].Experience);

            if (new_party[character].Rank > party[new_party[character].Class].Rank)
            {
                increased_rank++;
            }

            // reset attributes
            new_party[character].Attributes.clear();

            Generate::Attributes(new_party[character]);
        }

        auto earned_experience = total_experience - base_experience;

        std::string text = Engine::Count(new_party) > 1 ? "Surviving members of the party" : "You";

        text += " have earned " + std::to_string(earned_experience) + " points in this adventure. ";

        if (increased_rank == new_party.Count() && new_party.Count() > 1)
        {
            text += "All";
        }
        else if (increased_rank > 1)
        {
            text += std::to_string(increased_rank);
        }
        else if (increased_rank == 1)
        {
            text += "You";
        }
        else
        {
            text += "None";
        }

        text += " have increased in rank.";

        auto base_width = BloodSword::TextSize;

        auto stats = Interface::GeneratePartyStats(graphics, new_party, BloodSword::Wrap);

        auto statsx = (graphics.Width - BloodSword::Width(stats)) / 2 + BloodSword::Pad;

        auto texture = Graphics::CreateText(graphics, text.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, base_width);

        auto texturex = (graphics.Width - BloodSword::Width(texture)) / 2 + BloodSword::Pad;

        auto panelh = (BloodSword::TileSize + BloodSword::Pad) * (new_party.Count() + 2) + BloodSword::Height(texture) + BloodSword::LargePad * 6;

        auto panelw = base_width + BloodSword::LargePad;

        auto panelx = (graphics.Width - panelw) / 2;

        auto panely = (graphics.Height - panelh) / 2;

        auto iconx = (graphics.Width - BloodSword::TileSize) / 2;

        auto confirmy = panely + panelh - (BloodSword::TileSize + BloodSword::LargePad);

        auto input = Controls::User();

        auto done = false;

        while (!done)
        {
            auto overlay = Scene::Base();

            // render panel
            overlay.Add(Scene::Element(panelx, panely, panelw, panelh, Color::Background, Color::Active, BloodSword::Pixel));

            // render trophy icon
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::TROPHY), Point(iconx, panely + BloodSword::LargePad)));

            // render rankings text
            overlay.VerifyAndAdd(Scene::Element(texture, Point(texturex, panely + BloodSword::TileSize + BloodSword::TriplePad)));

            // render stats
            overlay.VerifyAndAdd(Scene::Element(stats, Point(statsx, panely + BloodSword::TileSize + BloodSword::HugePad + BloodSword::Height(texture))));

            // render confirmation button
            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CONFIRM), Point(iconx, confirmy)));

            overlay.Add(Controls::Base(Controls::Type::CONFIRM, 0, 0, 0, 0, 0, iconx, confirmy, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

            if (Input::Validate(input))
            {
                if (input.Type == Controls::Type::CONFIRM)
                {
                    // confirm completed
                    if (Interface::Confirm(graphics, background, "SAVE THIS PARTY TO COMPLETE THE GAME?", Color::Background, Color::Active, BloodSword::Border, Color::Highlight, true))
                    {
                        saved = Interface::LoadSaveGame(graphics, background, new_party, Controls::Type::SAVE, Asset::Type::SAVE);
                    }

                    done = true;
                }

                input.Selected = false;
            }
        }

        BloodSword::Free(&texture);

        BloodSword::Free(&stats);

        return saved;
    }

    // reloads the story from current module
    void ReloadStory(Graphics::Base &graphics, Party::Base &party)
    {
        Interface::LoadModule(party.Module);

        Interface::ReloadSettings(graphics, Interface::SettingsFile);

        Story::Load(Interface::Settings["adventure"]);
    }

    // render topic
    void Topic(Graphics::Base &graphics, Help::Section &section, Asset::Type button, bool blur = true)
    {
        auto background = Scene::Base();

        if (section.Items.size() > 0)
        {
            auto has_image = !section.Image.empty();

            auto panel_image_w = BloodSword::OctaTile;

            auto panel_text_w = has_image ? BloodSword::OctaTile : BloodSword::TextSize;

            auto panel_h = BloodSword::Wrap;

            auto x = has_image ? (graphics.Width - (panel_image_w + panel_text_w + BloodSword::LargePad)) / 2 : (graphics.Width - panel_text_w) / 2;

            auto panel_text_x = has_image ? (x + panel_image_w + BloodSword::LargePad) : x;

            auto y = (graphics.Height - panel_h) / 2;

            auto logo = Asset::Create(graphics.Renderer, std::string(Interface::Settings["logo"]).c_str());

            auto logo_x = (graphics.Width - BloodSword::Width(logo)) / 2;

            auto logo_y = (y - BloodSword::Height(logo) - BloodSword::LargePad);

            auto texture = Help::GenerateSection(graphics, Fonts::Normal, section, panel_text_w - BloodSword::LargePad);

            auto origin = Point(x, y);

            Point image_location;

            SDL_Texture *image = nullptr;

            if (has_image)
            {
                image = Graphics::ScaledImage(graphics, section.Image, panel_image_w - BloodSword::LargePad, panel_h - BloodSword::LargePad);

                image_location = origin + Point(panel_image_w - BloodSword::Width(image), panel_h - BloodSword::Height(image)) / 2;
            }

            if (texture)
            {
                auto offset = 0;

                auto text_h = panel_h - (BloodSword::TileSize + BloodSword::TriplePad);

                auto texture_h = BloodSword::Height(texture);

                auto text_x = panel_text_x + BloodSword::Pad;

                auto text_y = y + BloodSword::Pad;

                auto input = Controls::User();

                auto controls_x = panel_text_x + (panel_text_w - (BloodSword::TripleTile + BloodSword::LargePad)) / 2;

                auto controls_y = y + panel_h - (BloodSword::TileSize + BloodSword::LargePad);

                auto scroll_speed = BloodSword::ScrollSpeed;

                auto done = false;

                while (!done)
                {
                    auto overlay = Scene::Base();

                    Interface::AddScrollableTextureBox(overlay, panel_text_x, y, panel_text_w, panel_h, Color::Background, Color::Active, BloodSword::Border, texture, texture_h, text_x, text_y, text_h, offset, controls_x, controls_y, button, Asset::Type::LEFT, Asset::Type::RIGHT, scroll_speed);

                    if (has_image)
                    {
                        // image panel border
                        overlay.Add(Scene::Element(origin.X, origin.Y, panel_image_w, panel_h, Color::Background, Color::Active, BloodSword::Border));

                        if (image)
                        {
                            // add left panel
                            overlay.VerifyAndAdd(Scene::Element(image, image_location));
                        }
                    }

                    // add logo
                    overlay.VerifyAndAdd(Scene::Element(logo, Point(logo_x, logo_y)));

                    input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, blur);

                    if (Input::Validate(input))
                    {
                        if (input.Type == Controls::Type::LEFT || input.Up)
                        {
                            Interface::TextUp(overlay, input, Controls::Type::LEFT, input.Up, offset, texture_h, text_h, scroll_speed);
                        }
                        else if (input.Type == Controls::Type::RIGHT || input.Down)
                        {
                            Interface::TextDown(overlay, input, Controls::Type::RIGHT, input.Down, offset, texture_h, text_h, scroll_speed);
                        }
                        else if (input.Type == Controls::Type::CONFIRM)
                        {
                            done = true;
                        }

                        input.Selected = false;
                    }
                }

                BloodSword::Free(&texture);

                BloodSword::Free(&logo);
            }

            BloodSword::Free(&image);
        }
    }

    // display topics menu
    void Topics(Graphics::Base &graphics, Help::Sections &topics, Asset::Type button)
    {
        auto background = Scene::Base();

        if (topics.size() > 0)
        {
            auto max_limit = 6;

            auto start = 0;

            auto limit = std::min(max_limit, int(topics.size()));

            auto last = start + limit;

            auto options = int(topics.size());

            auto wrap = BloodSword::Wrap;

            auto text_list = Graphics::TextList();

            for (auto &topic : topics)
            {
                text_list.push_back(Graphics::RichText(topic.Header, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0));
            }

            auto menu = Graphics::CreateText(graphics, text_list);

            // padding
            auto pad = BloodSword::Pad;

            auto pads = BloodSword::LargePad;

            // default width
            auto w = std::max(BloodSword::Width(menu) + pads, wrap);

            // default height
            auto h = std::max(BloodSword::Height(menu) + pad, BloodSword::HalfTile);

            auto box_w = w + pads;

            auto box_h = ((h + pad) * max_limit + BloodSword::TileSize + BloodSword::HalfTile + pad * 3);

            if (options > limit)
            {
                box_w += BloodSword::TileSize + pad;
            }

            auto x = (graphics.Width - box_w) / 2;

            auto y = (graphics.Height - box_h) / 2;

            auto logo = Asset::Create(graphics.Renderer, std::string(Interface::Settings["logo"]).c_str());

            auto logo_x = (graphics.Width - BloodSword::Width(logo)) / 2;

            auto logo_y = (y - BloodSword::Height(logo) - BloodSword::LargePad);

            auto button_x = (graphics.Width - BloodSword::TileSize) / 2;

            auto button_y = (y + box_h - BloodSword::TileSize - pad);

            auto input = Controls::User();

            auto done = false;

            while (!done)
            {
                // add menu
                auto overlay = Interface::Menu(menu, x + pad, y + pad, w, h, start, last, limit, Color::Background, Color::Background, Color::Active, true, true);

                // add frame at the back
                overlay.Elements.insert(overlay.Elements.begin(), Scene::Element(x, y, box_w, box_h, Color::Background, Color::Active, BloodSword::Border));

                auto first = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                auto &lastControl = overlay.Controls.back();

                auto id = lastControl.Id + 1;

                // add close button
                overlay.VerifyAndAdd(Scene::Element(Asset::Get(button), button_x, button_y));

                overlay.Add(Controls::Base(Controls::Type::BACK, id, id, id, first + limit - 1, id, button_x, button_y, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

                // focus on scroll button
                Interface::ClearScrolling(overlay, input, Controls::Type::SCROLL_UP, Controls::Type::SCROLL_DOWN);

                // add logo
                overlay.VerifyAndAdd(Scene::Element(logo, Point(logo_x, logo_y)));

                input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

                if (Input::Validate(input))
                {
                    if (input.Type == Controls::Type::BACK)
                    {
                        done = true;
                    }
                    else if (input.Type == Controls::Type::CHOICE)
                    {
                        auto list = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                        auto choice = start + (input.Current - list);

                        if (choice >= 0 && choice < options)
                        {
                            Interface::Topic(graphics, topics[choice], Asset::Type::SWORDTHRUST, true);
                        }
                    }
                    else if (Input::IsUp(input))
                    {
                        Interface::ScrollUp(overlay, input, Controls::Type::SCROLL_UP, options, limit, start, last);
                    }
                    else if (Input::IsDown(input))
                    {
                        Interface::ScrollDown(overlay, input, Controls::Type::SCROLL_DOWN, options, limit, start, last);
                    }

                    input.Selected = false;
                }
            }

            BloodSword::Free(&logo);

            BloodSword::Free(menu);
        }
    }

    // display topics menu
    void Topics(Graphics::Base &graphics, Asset::Type asset)
    {
        auto help = Help::Load(Interface::Settings["help"]);

        Interface::Topics(graphics, help, asset);
    }

    // show game menu (change battle order, load/save game)
    bool GameMenu(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Party::Base &saved_party, bool &reload)
    {
        auto update = false;

        reload = false;

        Asset::List assets = {
            Asset::Type::BATTLE_ORDER,
            Asset::Type::LOAD,
            Asset::Type::SAVE,
            Asset::Type::ABOUT,
            Asset::Type::BACK};

        Controls::List controls = {
            Controls::Type::BATTLE_ORDER,
            Controls::Type::LOAD,
            Controls::Type::SAVE,
            Controls::Type::ABOUT,
            Controls::Type::BACK,
        };

        std::vector<std::string> captions = {
            "BATTLE ORDER",
            "LOAD GAME",
            "SAVE GAME",
            "HELP",
            "BACK"};

        auto values = std::vector<int>(controls.size());

        std::iota(values.begin(), values.end(), 0);

        auto message = Story::CurrentBook.Title;

        auto done = false;

        while (!done)
        {
            auto selection = Interface::SelectIcons(graphics, background, message.c_str(), assets, values, captions, 1, 1, Asset::Type::NONE, false, true);

            if (selection.size() == 1)
            {
                auto input = controls[selection[0]];

                if (input == Controls::Type::BACK)
                {
                    done = true;
                }
                else if (input == Controls::Type::BATTLE_ORDER)
                {
                    if (Engine::IsAlive(party))
                    {
                        auto current = Story::CurrentBook.Find(party.Location);

                        if (current >= 0 && current < Story::CurrentBook.Sections.size())
                        {
                            if (Story::CurrentBook.Sections[current].Has(Feature::Type::BAD_ENDING))
                            {
                                Interface::ErrorMessage(graphics, background, Interface::MSG_OVER);
                            }
                            else if (!Story::CurrentBook.Sections[current].Battle.IsDefined())
                            {
                                if (party.Count() > 1)
                                {
                                    update = Interface::BattleOrder(graphics, background, party);

                                    done = true;
                                }
                                else
                                {
                                    Interface::MessageBox(graphics, background, "YOU DO NOT HAVE ANY COMPANIONS", Color::Inactive);
                                }
                            }
                            else
                            {
                                Interface::ErrorMessage(graphics, background, Interface::MSG_ORDER);
                            }
                        }
                        else
                        {
                            Interface::ErrorMessage(graphics, background, Interface::MSG_ORDER);
                        }
                    }
                    else
                    {
                        Interface::ErrorMessage(graphics, background, Interface::MSG_OVER);
                    }
                }
                else if (input == Controls::Type::SAVE)
                {
                    done = Interface::LoadSaveGame(graphics, background, saved_party, Controls::Type::SAVE, Asset::Type::SAVE);

                    if (done)
                    {
                        Interface::Notify(graphics, background, Interface::MSG_SAVED);
                    }
                }
                else if (input == Controls::Type::LOAD)
                {
                    done = Interface::LoadSaveGame(graphics, background, party, Controls::Type::LOAD, Asset::Type::LOAD);

                    if (done)
                    {
                        Interface::Notify(graphics, background, Interface::MSG_LOADED);

                        Interface::ReloadStory(graphics, party);

                        reload = true;
                    }
                }
                else if (input == Controls::Type::ABOUT)
                {
                    Interface::Topics(graphics, Asset::Type::SWORDTHRUST);
                }
                else
                {
                    Interface::NotImplemented(graphics, background);
                }
            }
        }

        return update;
    }
}

#endif
