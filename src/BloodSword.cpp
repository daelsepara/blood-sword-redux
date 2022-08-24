#include "BloodSword.hpp"

namespace BloodSword
{
    int Main(int argc, char **argv)
    {
        auto graphics = Graphics::Initialize("Blood Sword");

        BloodSword::Initialize();

        Test::Render(graphics);

        BloodSword::Shutdown();

        Graphics::Quit(graphics);

        return 0;
    }
}

int main(int argc, char **argv)
{
    return BloodSword::Main(argc, argv);
}
