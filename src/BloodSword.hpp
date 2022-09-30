#ifndef __BLOODSWORD_HPP__
#define __BLOODSWORD_HPP__

#include "nlohmann/json.hpp"
#include "Asset.hpp"
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

        Asset::Load(graphics.Renderer, "assets.json");

        Fonts::Load("font-settings.json");

        Graphics::InitializeVersionOverlay(graphics);

        Interface::Initialize(graphics);
    }

    // shutdown all subsystems
    void Shutdown(Graphics::Base &graphics)
    {
        Interface::Shutdown();

        Fonts::Free();

        Asset::Unload();

        Graphics::Quit(graphics);
    }
}

#endif
