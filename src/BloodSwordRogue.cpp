#include "Rogue.hpp"
#include "System.hpp"

// BloodSword Rogue
namespace BloodSword::BloodSwordRogue
{
    // main loop
    int Main()
    {
        auto return_code = 0;

        auto system = BloodSword::System();

        try
        {
            system.Initialize("BloodSword: Rogue");

            Rogue::Game(system.graphics);
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

            return_code = 1;
        }

        system.Shutdown();

        return return_code;
    }
}

int main(int argc, char **argv)
{
    return BloodSword::BloodSwordRogue::Main();
}
