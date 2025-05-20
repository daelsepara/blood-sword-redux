#ifndef __BLOODSWORD_HPP__
#define __BLOODSWORD_HPP__

#include "nlohmann/json.hpp"
#include "Interface.hpp"

namespace BloodSword
{
    class Game
    {
    public:
        Graphics::Base graphics;

        // initialize all subsytems
        void Initialize(const char *session_name)
        {
            this->graphics = Graphics::Initialize(session_name);

            Engine::InitializeRNG();

            // game settings
            Interface::Initialize("settings/bloodsword.json");

            // load sound assets
            Sound::Load(Interface::Settings["sounds"]);

            // load fonts
            Fonts::Load(Interface::Settings["fonts"]);

            // palette definitions
            Palette::Load(Interface::Settings["palettes"]);

            // set default palette
            Palette::Switch(int(Interface::Settings["palette"]));

            // load textures
            Interface::LoadTextures(this->graphics);

            // initialize gamepads
            Input::InitializeGamePads();

            // load item defaults
            Items::LoadDefaults(Interface::Settings["items"]);

            // load item descriptions
            Items::LoadDescriptions(Interface::Settings["item_descriptions"]);

            // load characters
            Party::Characters = Party::Load(Interface::Settings["characters"], "characters");

            // flush inputs
            Input::Flush();
        }

        void Initialize()
        {
            this->Initialize("Blood Sword");
        }

        // shutdown all subsystems
        void Shutdown()
        {
            Sound::Free();

            Fonts::Free();

            Interface::UnloadTextures();

            Graphics::Quit(this->graphics);
        }

        Game() {}

        Game(const char *session_name)
        {
            this->Initialize(session_name);
        }
    };
}

#endif
