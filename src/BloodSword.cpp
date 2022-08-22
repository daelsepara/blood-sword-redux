#include "BloodSword.hpp"

namespace BloodSword
{
    int Main(int argc, char **argv)
    {
        Engine::InitializeRNG();

        auto Screen = Graphics::Initialize("Blood Sword");

        Fonts::Load("bloodsword.json");

        Asset::Load("assets.json");

        Test::Render(Screen);

        Asset::Unload();

        Fonts::Free();

        Graphics::Quit(Screen);

        return 0;
    }
}

int main(int argc, char **argv)
{
    return BloodSword::Main(argc, argv);
}
