#include <algorithm>
#include <iostream>

#include "nlohmann/json.hpp"
#include "Map.hpp"

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cerr << "To run:" << std::endl
                  << std::endl
                  << argv[0] << " [map.json] [width] [height]"
                  << std::endl;

        return 1;
    }

    auto width = std::stoi(argv[2], nullptr, 10);

    auto height = std::stoi(argv[3], nullptr, 10);

    auto map = BloodSword::Map::Base(width, height);

    map.Save(argv[1]);

    return 0;
}
