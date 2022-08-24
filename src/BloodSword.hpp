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
    const char *Version = "0.0.1";
    const int MajorVersion = 0;
    const int MinorVersion = 1;

    void Initialize()
    {
        Engine::InitializeRNG();

        Asset::Load("assets.json");

        Fonts::Load("font-settings.json");
    }
    
    void Shutdown()
    {
        Fonts::Free();

        Asset::Unload();
    }
}

#endif
