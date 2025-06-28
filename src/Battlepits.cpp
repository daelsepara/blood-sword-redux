#include "Rogue.hpp"

// render map to a png file
namespace BloodSword::BattlepitsRenderer
{
    bool Empty(Map::Base &map, Point point)
    {
        return (map[point].Type == Map::Object::NONE);
    }

    bool Blocked(Map::Base &map, Point point)
    {
        auto &tile = map[point];

        return (tile.Asset == Asset::Type::NONE && (tile.Type == Map::Object::NONE || (tile.Type != Map::Object::PASSABLE && tile.Type != Map::Object::ENEMY_PASSABLE)));
    }

    void RenderCorners(BloodSword::Map::Base &map, BloodSword::Map::Tile &tile, int x, int y)
    {
        auto tl = Empty(map, {x - 1, y - 1});

        auto tr = Empty(map, {x + 1, y - 1});

        auto bl = Empty(map, {x - 1, y + 1});

        auto br = Empty(map, {x + 1, y + 1});

        if (tl && tr && bl && br)
        {
            tile.Asset = Asset::Type::FOUR_CORNERS;
        }
        else if (tr && tl && bl)
        {
            tile.Asset = Asset::Type::TL_CORNERS;
        }
        else if (tl && tr && br)
        {
            tile.Asset = Asset::Type::TR_CORNERS;
        }
        else if (tl && bl && br)
        {
            tile.Asset = Asset::Type::BL_CORNERS;
        }
        else if (bl && br && tr)
        {
            tile.Asset = Asset::Type::BR_CORNERS;
        }
        else if (tl && tr)
        {
            tile.Asset = Asset::Type::TOP_CORNERS;
        }
        else if (bl && br)
        {
            tile.Asset = Asset::Type::BOTTOM_CORNERS;
        }
        else if (tl && bl)
        {
            tile.Asset = Asset::Type::LEFT_CORNERS;
        }
        else if (tr && br)
        {
            tile.Asset = Asset::Type::RIGHT_CORNERS;
        }
        else if (tl)
        {
            tile.Asset = Asset::Type::CORNER_TL;
        }
        else if (tr)
        {
            tile.Asset = Asset::Type::CORNER_TR;
        }
        else if (bl)
        {
            tile.Asset = Asset::Type::CORNER_BL;
        }
        else if (br)
        {
            tile.Asset = Asset::Type::CORNER_BR;
        }
    }

    void RenderTunnel(BloodSword::Map::Base &map, BloodSword::Map::Tile &tile, int x, int y)
    {
        auto top = (y == 0 || Blocked(map, {x, y - 1}));

        auto left = (x == 0 || Blocked(map, {x - 1, y}));

        auto right = (x >= map.Width - 1 || Blocked(map, {x + 1, y}));

        auto bottom = (y >= map.Height - 1 || Blocked(map, {x, y + 1}));

        if (top && left && right)
        {
            tile.Asset = Asset::Type::BORDER_BON;
        }
        else if (top && right && bottom)
        {
            tile.Asset = (Asset::Type::BORDER_LON);
        }
        else if (left && right && bottom)
        {
            tile.Asset = (Asset::Type::BORDER_TON);
        }
        else if (top && left && bottom)
        {
            tile.Asset = (Asset::Type::BORDER_RON);
        }
        else if (top && left)
        {
            tile.Asset = (Asset::Type::BORDER_TLT);

            if (Empty(map, {x + 1, y + 1}))
            {
                tile.Asset = Asset::Type::BORDER_TLT_E;
            }
        }
        else if (top && right)
        {
            tile.Asset = (Asset::Type::BORDER_TRT);

            if (Empty(map, {x - 1, y + 1}))
            {
                tile.Asset = Asset::Type::BORDER_TRT_E;
            }
        }
        else if (bottom && right)
        {
            tile.Asset = (Asset::Type::BORDER_BRT);

            if (Empty(map, {x - 1, y - 1}))
            {
                tile.Asset = Asset::Type::BORDER_BRT_E;
            }
        }
        else if (bottom && left)
        {
            tile.Asset = (Asset::Type::BORDER_BLT);

            if (Empty(map, {x + 1, y - 1}))
            {
                tile.Asset = Asset::Type::BORDER_BLT_E;
            }
        }
        else if (top && bottom)
        {
            tile.Asset = (Asset::Type::BORDER_TBT);
        }
        else if (left && right)
        {
            tile.Asset = (Asset::Type::BORDER_LRT);
        }
        else if (top)
        {
            tile.Asset = (Asset::Type::BORDER_TOP);
        }
        else if (bottom)
        {
            tile.Asset = (Asset::Type::BORDER_BOT);
        }
        else if (left)
        {
            tile.Asset = (Asset::Type::BORDER_LFT);
        }
        else if (right)
        {
            tile.Asset = (Asset::Type::BORDER_RHT);
        }
        else
        {
            RenderCorners(map, tile, x, y);
        }
    }

    void Render(const char *module, int width, int height, int max_rooms, int min_size, int max_size, const char *image_file)
    {
        // get all available modules
        Interface::LoadModules();

        // load selected module
        Interface::LoadModule(module);

        // load settings
        Interface::Initialize(Interface::SettingsFile.c_str());

        // load asset location mappings
        Asset::Load(std::string(Interface::Settings["assets"]).c_str());

        // generate battlepits
        auto map = Battlepits::Generate(width, height, max_rooms, min_size, max_size);

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

                    if (tile.Type == Map::Object::PASSABLE)
                    {
                        RenderTunnel(map, tile, x, y);
                    }
                    else if (tile.Type == Map::Object::EXIT)
                    {
                        tile.Asset = Asset::Type::SELECT;
                    }

                    if (tile.Asset != Asset::Type::NONE)
                    {
                        surface_asset = BloodSword::Asset::Surface(tile.Asset);
                    }

                    if (surface_asset == nullptr)
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
    int Main(const char *module, int width, int height, int max_rooms, int min_size, int max_size, const char *image_file)
    {
        auto return_code = 0;

        try
        {
            BattlepitsRenderer::Render(module, width, height, max_rooms, min_size, max_size, image_file);
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
    if (argc < 8)
    {
        std::cerr << "To run:" << std::endl
                  << std::endl
                  << argv[0] << " [module] [map width] [map height] [max rooms] [min size] [max size] [image.png]"
                  << std::endl;

        return 1;
    }

    int width = std::stoi(argv[2], nullptr, 10);

    int height = std::stoi(argv[3], nullptr, 10);

    int max_rooms = std::stoi(argv[4], nullptr, 10);

    int min_size = std::stoi(argv[5], nullptr, 10);

    int max_size = std::stoi(argv[6], nullptr, 10);

    return BloodSword::BattlepitsRenderer::Main(argv[1], width, height, max_rooms, min_size, max_size, argv[7]);
}
