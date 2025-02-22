#include <algorithm>
#include <string>
#include <iostream>

#include "nlohmann/json.hpp"
#include "Map.hpp"

bool compare(std::string s1, std::string s2)
{
    //convert s1 and s2 into lower case strings
    transform(s1.begin(), s1.end(), s1.begin(), ::tolower);

    transform(s2.begin(), s2.end(), s2.begin(), ::tolower);

    return (s1.compare(s2) == 0);
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cerr << "To run:" << std::endl << std::endl
                  << argv[0] << "[map.json] [width] [height]"
                  << std::endl;

        return 1;
    }

    auto width = std::stoi(argv[2], nullptr, 10);

    auto height = std::stoi(argv[3], nullptr, 10);

    auto map = BloodSword::Map::Base(width, height);

    map.Save(argv[1]);

    return 0;
}
