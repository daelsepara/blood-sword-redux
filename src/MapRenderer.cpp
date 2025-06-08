#include "MapRenderer.hpp"

namespace BloodSword
{
    // main loop
    int Main(const char *battle_file, const char *image_file)
    {
        auto game = BloodSword::Game();

        try
        {
            game.Initialize();

            MapRenderer::Main(game.graphics, battle_file, image_file);
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
    if (argc < 3)
    {
        std::cerr << "To run:" << std::endl
                  << std::endl
                  << argv[0] << " [battle.json] [image.png]"
                  << std::endl;

        return 1;
    }

    return BloodSword::Main(argv[1], argv[2]);
}
