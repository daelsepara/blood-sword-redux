#include "SpriteSheet.hpp"

namespace BloodSword
{
    // main loop
    int Main(const char *module, const char *image_file, int width, bool reverse)
    {
        try
        {
            auto converted_module = Engine::ToUpper(std::string(module));

            SpriteSheet::Main(converted_module.c_str(), image_file, width, reverse);
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

        return 0;
    }
}

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

    return BloodSword::Main(argv[1], argv[2], width, reverse);
}
