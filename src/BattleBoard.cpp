#include "BattleBoard.hpp"

namespace BloodSword
{
    // main loop
    int Main(const char *battle_file)
    {
        auto game = BloodSword::Game();

        try
        {
            game.Initialize();

            BattleBoard::Menu(game.graphics, battle_file);
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
    if (argc < 2)
    {
        std::cerr << "To run:" << std::endl
                  << std::endl
                  << argv[0] << " [battle.json]"
                  << std::endl;

        return 1;
    }

    return BloodSword::Main(argv[1]);
}
