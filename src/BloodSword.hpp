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
    // initialize all subsytems
    void Initialize(Graphics::Base &graphics)
    {
        Engine::InitializeRNG();

        Fonts::Load("settings/font-settings.json");

        Palette::Switch(0);

        Interface::LoadTextures(graphics);
    }

    // shutdown all subsystems
    void Shutdown(Graphics::Base &graphics)
    {
        Fonts::Free();

        Interface::UnloadTextures();

        Graphics::Quit(graphics);
    }
}

#endif
