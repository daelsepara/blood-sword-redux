#include "BloodSword.hpp"

namespace BloodSword
{
    int Main(int argc, char **argv)
    {
        auto graphics = Graphics::Initialize("Blood Sword");

        Engine::InitializeRNG();

        Fonts::Load("font-settings.json");

        Asset::Load("assets.json");

        Test::Render(graphics);

        Asset::Unload();

        Fonts::Free();

        Graphics::Quit(graphics);

        return 0;
    }
}

int main(int argc, char **argv)
{
    return BloodSword::Main(argc, argv);
}
