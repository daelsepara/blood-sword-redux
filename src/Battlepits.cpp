#include "Room.hpp"
#include "Interface.hpp"

// render map to a png file
namespace BloodSword::BattlepitsRenderer
{
    void RenderPoints(SDL_Surface *surface, Points &points, SDL_Rect &rect, int offset, Asset::Type asset, bool blur = false)
    {
        for (auto point : points)
        {
            rect.x = point.X * BloodSword::TileSize + offset;

            rect.y = point.Y * BloodSword::TileSize + offset;

            auto surface_sset = blur ? BloodSword::Asset::Surface(asset, 0x7F7F7F7F) : BloodSword::Asset::Surface(asset);

            Graphics::RenderAssetThenFree(surface, surface_sset, rect);
        }
    }

    void Render(const char *module, const char *image_file)
    {
        // get all available modules
        Interface::LoadModules();

        // load selected module
        Interface::LoadModule(module);

        // load settings
        Interface::Initialize(Interface::SettingsFile.c_str());

        // load asset location mappings
        Asset::Load(std::string(Interface::Settings["assets"]).c_str());

        // load specific map
        auto map = Map::Base(65, 45);

        // generate battlepits
        Battlepits::Generate(map);

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

                    if (!tile.IsOccupied())
                    {
                        if (tile.Asset != Asset::Type::NONE)
                        {
                            surface_asset = BloodSword::Asset::Surface(tile.Asset);
                        }
                        else if (tile.Type == Map::Object::EXIT)
                        {
                            surface_asset = BloodSword::Asset::Surface(Asset::Type::SELECT);
                        }
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

            // away players
            BattlepitsRenderer::RenderPoints(surface, map.AwayPlayers, rect, offset, Asset::Type::CIRCLE, true);

            // survivors
            BattlepitsRenderer::RenderPoints(surface, map.Survivors, rect, offset, Asset::Type::SELECT, true);

            // away opponents
            BattlepitsRenderer::RenderPoints(surface, map.AwayOpponents, rect, offset, Asset::Type::SELECT, true);

            IMG_SavePNG(surface, image_file);

            SDL_FreeSurface(surface);
        }
    }

    // main loop
    int Main(const char *module, const char *image_file)
    {
        auto return_code = 0;

        try
        {
            BattlepitsRenderer::Render(module, image_file);
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
    if (argc < 3)
    {
        std::cerr << "To run:" << std::endl
                  << std::endl
                  << argv[0] << "[module] [image.png]"
                  << std::endl;

        return 1;
    }

    return BloodSword::BattlepitsRenderer::Main(argv[1], argv[2]);
}
