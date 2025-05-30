#include "BloodSwordTest.hpp"

namespace BloodSword
{
    // main loop
    int Main(int argc, char **argv)
    {
        auto game = BloodSword::Game();

        game.Initialize();

        try
        {
            Test::Menu(game.graphics);
        }
        catch (std::exception &e)
        {
            std::cerr << std::endl
                      << "BLOODSWORD ("
                      << Version()
                      << ") EXCEPTION: "
                      << e.what()
                      << std::endl
                      << std::endl;
        }

        game.Shutdown();

        return 0;
    }
}

int main(int argc, char **argv)
{
    return BloodSword::Main(argc, argv);
}
