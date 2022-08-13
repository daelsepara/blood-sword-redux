#include "BloodSword.hpp"

namespace BloodSword
{
    int Main(int argc, char **argv)
    {
        auto Screen = Graphics::Base();

        Engine::InitializeRNG();

        Graphics::Initialize(Screen, "Blood Sword");

        Asset::Load("assets.json");

        Test::RenderPlayers(Screen);

        Asset::Unload();

        Graphics::Quit(Screen);

        return 0;
    }
}

int main(int argc, char **argv)
{
    return BloodSword::Main(argc, argv);
}
