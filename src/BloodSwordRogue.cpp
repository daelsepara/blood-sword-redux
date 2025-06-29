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

            auto party = Interface::CreateParty(system.graphics, {8, 4, 3, 2}, false);

            if (party.Count() > 0)
            {
                Rogue::Game(system.graphics, party);
            }
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
