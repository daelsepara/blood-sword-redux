#include "BloodSword.hpp"
#include "BattleBoard.hpp"

bool compare(std::string s1, std::string s2)
{
    // convert s1 and s2 into lower case strings
    transform(s1.begin(), s1.end(), s1.begin(), ::tolower);

    transform(s2.begin(), s2.end(), s2.begin(), ::tolower);

    return (s1.compare(s2) == 0);
}

namespace BloodSword
{
    // main loop
    int Main(const char *battle_file)
    {
        auto game = BloodSword::Game();

        game.Initialize();

        try
        {
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
                  << argv[0] << " [map.json]"
                  << std::endl;

        return 1;
    }

    return BloodSword::Main(argv[1]);
}
