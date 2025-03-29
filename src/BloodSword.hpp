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

            Interface::Initialize("settings/bloodsword.json");

            Fonts::Load(Interface::Settings["fonts"]);

            Palette::Load(Interface::Settings["palettes"]);

            Palette::Switch(int(Interface::Settings["palette"]));

            Interface::LoadTextures(this->graphics);

            Input::InitializeGamePads();

            // load item defaults
            Items::LoadDefaults(Interface::Settings["items"]);

            // load item descriptions
            Items::LoadDescriptions(Interface::Settings["item_descriptions"]);

            Input::Flush();
        }

        void Initialize()
        {
            this->Initialize("Blood Sword");
        }

        // shutdown all subsystems
        void Shutdown()
        {
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
