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
    void Initialize(Graphics::Base &graphics)
    {
        Engine::InitializeRNG();

        Asset::Load(graphics.Renderer, "assets.json");

        Fonts::Load("font-settings.json");

        Graphics::InitializeVersionOverlay(graphics);
    }

    void Shutdown(Graphics::Base &graphics)
    {
        Fonts::Free();

        Asset::Unload();

        Graphics::Quit(graphics);
    }
}

#endif
