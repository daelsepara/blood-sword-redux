#include "BloodSwordTest.hpp"

namespace BloodSword
{
    // main loop
    int Main(int argc, char **argv)
    {
        auto system = BloodSword::System();

        system.Initialize("BloodSword: Test Suite");

        try
        {
            Test::Menu(system.graphics);
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

        system.Shutdown();

        return 0;
    }
}

int main(int argc, char **argv)
{
    return BloodSword::Main(argc, argv);
}
