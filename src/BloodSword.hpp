#ifndef __BLOODSWORD_HPP__
#define __BLOODSWORD_HPP__

#include "nlohmann/json.hpp"
#include "Asset.hpp"
#include "Palette.hpp"
#include "Engine.hpp"
#include "Fonts.hpp"
#include "Graphics.hpp"
#include "Interface.hpp"
#include "Test.hpp"

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

            Palette::Switch(0);

            Interface::LoadTextures(this->graphics);
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
