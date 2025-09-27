#include "Rogue.hpp"

// render map to a png file
namespace BloodSword::BattlepitsRenderer
{
    // main render function
    void Render(const char *module, int width, int height, int max_rooms, int min_size, int max_size, Battlepits::Connection connection, bool inner_tunnel, int gap_size, const char *image_file)
    {
        // disable logger
        Logger::Disable();

        // get all available modules
        Interface::LoadModules();

        // load selected module
        Interface::LoadModule(module);

        // load settings
        Interface::Initialize(Interface::SettingsFile.c_str());

        // load asset location mappings
        Asset::Load(std::string(Interface::Settings["assets"]).c_str());

        // generate battlepits
        auto map = Battlepits::Generate(width, height, max_rooms, min_size, max_size, connection, inner_tunnel, gap_size);

        SDL_Rect rect;

        auto offset = BloodSword::LargePad;

        rect.w = map.Width * BloodSword::TileSize + offset * 2;

        rect.h = map.Height * BloodSword::TileSize + offset * 2;

        auto surface = Graphics::CreateSurface(rect.w, rect.h);

        if (surface)
        {
            // fill entire map
            SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 255, 255, 255, 255));

            for (auto y = 0; y < map.Height; y++)
            {
                for (auto x = 0; x < map.Width; x++)
                {
                    SDL_Surface *surface_asset = nullptr;

                    auto &tile = map[Point(x, y)];

                    if (tile.Type == Map::Object::EXIT)
                    {
                        surface_asset = Asset::Surface("SELECT");
                    }
                    else if (tile.Asset != Asset::NONE)
                    {
                        surface_asset = Asset::Surface(tile.Asset);
                    }
                    else
                    {
                        // fill empty space
                        surface_asset = Graphics::CreateSurface(BloodSword::TileSize, BloodSword::TileSize);

                        SDL_FillRect(surface_asset, nullptr, SDL_MapRGBA(surface->format, 0, 0, 0, 255));
                    }

                    rect.x = x * BloodSword::TileSize + offset;

                    rect.y = y * BloodSword::TileSize + offset;

                    Graphics::RenderAssetThenFree(surface, surface_asset, rect);
                }
            }

            IMG_SavePNG(surface, image_file);

            SDL_FreeSurface(surface);
        }
    }

    // main loop
    int Main(const char *module, int width, int height, int max_rooms, int min_size, int max_size, Battlepits::Connection connection, bool inner_tunnel, int gap_size, const char *image_file)
    {
        auto return_code = 0;

        try
        {
            BattlepitsRenderer::Render(module, width, height, max_rooms, min_size, max_size, connection, inner_tunnel, gap_size, image_file);
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

            return_code = 1;
        }

        return return_code;
    }
}

int main(int argc, char **argv)
{
    if (argc < 10)
    {
        std::cerr << "To run:" << std::endl
                  << std::endl
                  << argv[0] << " [module] [map width] [map height] [max rooms] [min size] [max size] [INNER | OUTER | WALLS] [gap size] [image.png]"
                  << std::endl;

        return 1;
    }

    int width = std::stoi(argv[2], nullptr, 10);

    int height = std::stoi(argv[3], nullptr, 10);

    int max_rooms = std::stoi(argv[4], nullptr, 10);

    int min_size = std::stoi(argv[5], nullptr, 10);

    int max_size = std::stoi(argv[6], nullptr, 10);

    auto connection_type = BloodSword::Engine::ToUpper(std::string(argv[7]));

    int gap_size = std::stoi(argv[8], nullptr, 10);

    auto connection = BloodSword::Battlepits::Connection::NONE;

    auto inner_tunnel = false;

    if (connection_type == "INNER")
    {
        connection = BloodSword::Battlepits::Connection::TUNNELS;

        inner_tunnel = true;
    }
    else if (connection_type == "OUTER")
    {
        connection = BloodSword::Battlepits::Connection::TUNNELS;
    }
    else if (connection_type == "WALLS")
    {
        connection = BloodSword::Battlepits::Connection::WALLS;
    }

    if (connection != BloodSword::Battlepits::Connection::NONE)
    {
        return BloodSword::BattlepitsRenderer::Main(argv[1], width, height, max_rooms, min_size, max_size, connection, inner_tunnel, gap_size, argv[9]);
    }
}
