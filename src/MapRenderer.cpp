#include "InterfaceBattle.hpp"

// render map to a png file
namespace BloodSword::MapRenderer
{
    // render points
    void RenderPoints(SDL_Surface *surface, Points &points, SDL_Rect &rect, int offset, Asset::Type asset, bool blur = false)
    {
        for (auto point : points)
        {
            rect.x = point.X * BloodSword::TileSize + offset;

            rect.y = point.Y * BloodSword::TileSize + offset;

            auto surface_sset = blur ? Asset::Surface(asset, 0x7F7F7F7F) : Asset::Surface(asset);

            Graphics::RenderAssetThenFree(surface, surface_sset, rect);
        }
    }

    // render map to a png file
    void Render(const char *module, const char *battle_file, const char *image_file)
    {
        // get all available modules
        Interface::LoadModules();

        // load selected module
        Interface::LoadModule(module);

        // load settings
        Interface::Initialize(Interface::SettingsFile.c_str());

        // load asset location mappings
        Asset::Load(std::string(Interface::Settings["assets"]).c_str());

        // load specific battle
        auto battle = Battle::Load(battle_file);

        auto scene = Scene::Base();

        auto party = Party::Base();

        for (auto &character_class : Character::All)
        {
            auto character = Generate::Character(character_class, 2);

            party.Add(character);
        }

        if (battle.IsDefined())
        {
            Interface::CloneCharacters(battle, party);

            if (battle.Has(Battle::Condition::REPLICATE) && battle.Opponents.Count() == 1)
            {
                // opponents to be replicated
                auto replicated = battle.Opponents[0];

                // replicate opponents to the number of players
                for (auto i = 0; i < Engine::Combatants(party) - 1; i++)
                {
                    battle.Opponents.Add(replicated);
                }
            }

            Interface::SetEnemyLocations(battle, party);

            SDL_Rect rect;

            auto offset = BloodSword::LargePad;

            rect.w = battle.Map.Width * BloodSword::TileSize + offset * 2;

            rect.h = battle.Map.Height * BloodSword::TileSize + offset * 2;

            auto surface = Graphics::CreateSurface(rect.w, rect.h);

            if (surface)
            {
                // fill entire map
                SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 255, 255, 255, 255));

                for (auto y = 0; y < battle.Map.Height; y++)
                {
                    for (auto x = 0; x < battle.Map.Width; x++)
                    {
                        SDL_Surface *surface_asset = nullptr;

                        auto &tile = battle.Map[Point(x, y)];

                        if (!tile.IsOccupied())
                        {
                            if (tile.Asset == Asset::Map("EMPTY SPACE"))
                            {
                                surface_asset = Asset::Surface(Asset::Map("SELECT"));
                            }
                            else if (tile.Asset != Asset::NONE)
                            {
                                surface_asset = Asset::Surface(tile.Asset);
                            }
                            else if (tile.Type == Map::Object::EXIT)
                            {
                                surface_asset = Asset::Surface(Asset::Map("SELECT"));
                            }
                        }
                        else
                        {
                            switch (tile.Occupant)
                            {
                            case Map::Object::PLAYER:

                                if (tile.Id >= 0 && tile.Id < party.Count() && party.Count() > 0)
                                {
                                    surface_asset = Asset::Surface(Asset::Map("SELECT"));
                                }

                                break;

                            case Map::Object::ENEMY:

                                if (tile.Id >= 0 && tile.Id < battle.Opponents.Count() && battle.Opponents.Count() > 0)
                                {
                                    surface_asset = Asset::Surface(battle.Opponents[tile.Id].Asset);
                                }

                                break;

                            case Map::Object::TEMPORARY_OBSTACLE:

                                if (tile.Lifetime > 0)
                                {
                                    if (tile.TemporaryAsset != Asset::NONE)
                                    {
                                        surface_asset = Asset::Surface(tile.TemporaryAsset);
                                    }
                                }
                                else if (tile.Asset != Asset::NONE)
                                {
                                    surface_asset = Asset::Surface(tile.Asset);
                                }

                                break;

                            default:

                                break;
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
                MapRenderer::RenderPoints(surface, battle.Map.AwayPlayers, rect, offset, Asset::Map("CIRCLE"), true);

                // survivors
                MapRenderer::RenderPoints(surface, battle.Map.Survivors, rect, offset, Asset::Map("SELECT"), true);

                // away opponents
                MapRenderer::RenderPoints(surface, battle.Map.AwayOpponents, rect, offset, Asset::Map("SELECT"), true);

                // player locations
                auto number = 1;

                for (auto &origin : battle.Map.Origins)
                {
                    rect.x = origin.X * BloodSword::TileSize + offset;

                    rect.y = origin.Y * BloodSword::TileSize + offset;

                    auto character_asset = Asset::Surface(Interface::Numbers[number]);

                    Graphics::RenderAssetThenFree(surface, character_asset, rect);

                    number++;
                }

                IMG_SavePNG(surface, image_file);

                SDL_FreeSurface(surface);
            }
        }
    }

    // main loop
    int Main(const char *module, const char *battle_file, const char *image_file)
    {
        auto return_code = 0;

        try
        {
            MapRenderer::Render(module, battle_file, image_file);
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
                  << argv[0] << "[module] [battle.json] [image.png]"
                  << std::endl;

        return 1;
    }

    return BloodSword::MapRenderer::Main(argv[1], argv[2], argv[3]);
}
