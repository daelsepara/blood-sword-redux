#include "Interface.hpp"

// generate sprite sheet
namespace BloodSword::SpriteSheet
{
    void Generate(const char *module, const char *image_file, int width, bool reverse)
    {
        // get all available modules
        Interface::LoadModules();

        // load selected module
        Interface::LoadModule(module);

        // load settings
        Interface::Initialize(Interface::SettingsFile.c_str());

        // load asset location mappings
        Asset::Load(std::string(Interface::Settings["assets"]).c_str());

        if (Asset::Locations.size() > 0)
        {
            SDL_Rect rect;

            auto height = int(std::nearbyint((double)Asset::Locations.size() / (double)width + 0.5));

            rect.w = width * BloodSword::TileSize;

            rect.h = height * BloodSword::TileSize;

            auto surface = Graphics::CreateSurface(rect.w, rect.h);

            if (surface)
            {
                auto x = 0;

                auto y = 0;

                for (const auto &[asset, name] : Asset::TypeMapping)
                {
                    if (asset != Asset::Type::NONE)
                    {
                        auto surface_asset = Asset::Surface(asset);

                        if (surface_asset == nullptr)
                        {
                            std::cerr << "Asset: [" << name << "] not found!" << std::endl;
                        }

                        rect.x = (reverse ? width - x - 1 : x) * BloodSword::TileSize;

                        rect.y = (reverse ? height - y - 1 : y) * BloodSword::TileSize;

                        Graphics::RenderAssetThenFree(surface, surface_asset, rect);

                        x++;

                        if (x == width)
                        {
                            x = 0;

                            y++;
                        }
                    }
                }

                IMG_SavePNG(surface, image_file);

                SDL_FreeSurface(surface);
            }
        }
    }

    // main loop
    int Main(const char *module, const char *image_file, int width, bool reverse)
    {
        auto return_code = 0;

        try
        {
            auto converted_module = Engine::ToUpper(std::string(module));

            SpriteSheet::Generate(converted_module.c_str(), image_file, width, reverse);
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
