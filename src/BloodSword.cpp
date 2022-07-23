#include "BloodSword.hpp"

namespace BloodSword
{
    int Main(int argc, char **argv)
    {
        auto Screen = Graphics::Base();

        Engine::Initialize();

        Graphics::Initialize(Screen, "Blood Sword");

        Graphics::Quit(Screen);

        return 0;
    }
}

int main(int argc, char **argv)
{
    return BloodSword::Main(argc, argv);
}
