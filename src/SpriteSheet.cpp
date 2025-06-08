#include "SpriteSheet.hpp"

namespace BloodSword
{
    // main loop
    int Main(const char *image_file, int width)
    {
        try
        {
            SpriteSheet::Main(image_file, width);
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
    if (argc < 3)
    {
        std::cerr << "To run:" << std::endl
                  << std::endl
                  << argv[0] << " [sprite sheet] [width]"
                  << std::endl;

        return 1;
    }

    auto width = std::stoi(argv[2], nullptr, 10);

    return BloodSword::Main(argv[1], width);
}
