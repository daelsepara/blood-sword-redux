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
            // disable logger
            Logger::Disable();

            system.Initialize("BloodSword: Rogue", "default");

            Rogue::Game(system.graphics);
        }
        catch (std::exception &e)
        {
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "BLOODSWORD ROGUE (%s) EXCEPTION: %s", Version().c_str(), e.what());

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
