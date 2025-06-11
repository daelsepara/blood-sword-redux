#include "MapRenderer.hpp"

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

    return BloodSword::MapRenderer::Main(argv[1], argv[2]);
}
