#include "SpriteSheet.hpp"

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cerr << "To run:" << std::endl
                  << std::endl
                  << argv[0] << " [module] [sprite sheet] [width] [reverse: optional]"
                  << std::endl;

        return 1;
    }

    auto width = std::stoi(argv[3], nullptr, 10);

    auto reverse = false;

    if (argc > 4)
    {
        reverse = BloodSword::Engine::ToUpper(std::string(argv[4])) == "REVERSE";
    }

    return BloodSword::SpriteSheet::Main(argv[1], argv[2], width, reverse);
}
