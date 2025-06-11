#include "BattleBoard.hpp"

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

    return BloodSword::BattleBoard::Main(argv[1]);
}
