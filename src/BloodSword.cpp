#include "BloodSword.hpp"

namespace BloodSword
{
    // main loop
    int Main(int argc, char **argv)
    {
        auto graphics = Graphics::Initialize("Blood Sword");

        BloodSword::Initialize(graphics);

        Input::InitializeGamePads();

        try
        {
            Test::Menu(graphics);
        }
        catch (std::exception &e)
        {
            std::cerr << std::endl
                      << "BLOODSWORD (" << Version() << ") EXCEPTION: " << e.what() << std::endl
                      << std::endl;
        }

        BloodSword::Shutdown(graphics);

        return 0;
    }
}

int main(int argc, char **argv)
{
    return BloodSword::Main(argc, argv);
}
