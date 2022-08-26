#include "BloodSword.hpp"

namespace BloodSword
{
    int Main(int argc, char **argv)
    {
        auto graphics = Graphics::Initialize("Blood Sword");

        BloodSword::Initialize(graphics);

        Test::Controls(graphics);

        BloodSword::Shutdown(graphics);

        return 0;
    }
}

int main(int argc, char **argv)
{
    return BloodSword::Main(argc, argv);
}
