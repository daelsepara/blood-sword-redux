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

            Fonts::Load("settings/font-settings.json");

            Palette::Load("settings/palettes.json");

            Palette::Switch(0);

            Interface::LoadTextures(this->graphics);

            Input::InitializeGamePads();

            // load item defaults
            Items::LoadDefaults("settings/items.json");

            // load item descriptions
            Items::LoadDescriptions("settings/item-descriptions.json");

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
