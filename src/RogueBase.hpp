#ifndef __ROGUE_BASE_HPP__
#define __ROGUE_BASE_HPP__

#include "Battlepits.hpp"
#include "InterfaceInventory.hpp"

namespace BloodSword::Rogue
{
    class Loot
    {
    public:
        // location in battlepits
        int X = -1;

        int Y = -1;

        // items in this location
        Items::Inventory Items = {};

        Point Location()
        {
            return Point(X, Y);
        }

        Loot() {}

        Loot(int x, int y) : X(x), Y(y) {}

        Loot(Point point) : X(point.X), Y(point.Y) {}
    };

    class Base
    {
    public:
        // battlepits
        Map::Base Battlepits;

        // rooms generated in battlepits map
        std::vector<Room::Base> Rooms = {};

        // groups of enemies in battlepits
        std::vector<Party::Base> Opponents = {};

        // groups of items in battlepits
        std::vector<Rogue::Loot> Loot = {};

        // player party adventuring in battlepits
        Party::Base Party;

        Base() {}
    };

    int FindLoot(Rogue::Base &rogue, Point point)
    {
        auto &Loot = rogue.Loot;

        auto found = -1;

        for (auto loot = 0; loot < Loot.size(); loot++)
        {
            if (Loot[loot].Location() == point)
            {
                found = loot;

                break;
            }
        }

        return found;
    }

    int FindOpponents(Rogue::Base &rogue, Point point)
    {
        auto found = -1;

        for (auto opponent = 0; opponent < rogue.Opponents.size(); opponent++)
        {
            if (rogue.Opponents[opponent].Origin() == point)
            {
                found = opponent;

                break;
            }
        }

        return found;
    }

    int FindOpponents(Rogue::Base &rogue, int room)
    {
        auto found = -1;

        for (auto opponent = 0; opponent < rogue.Opponents.size(); opponent++)
        {
            if (rogue.Opponents[opponent].Room == room)
            {
                found = opponent;

                break;
            }
        }

        return found;
    }

    struct Update
    {
        bool Scene = false;

        bool Party = false;

        bool Quit = false;
    };

    // attribute labels
    std::string StatsLabels()
    {
        // attribute labels
        auto label_1 = std::string("FPR      END      ");

        auto label_2 = std::string("AWR      DMG      ");

        auto label_3 = std::string("PSY      ARM      ");

        auto labels = label_1 + '\n' + label_2 + '\n' + label_3;

        return labels;
    }

    SDL_Texture *Stats(Graphics::Base &graphics, Character::Base &character, int w)
    {
        auto surface_labels = TTF_RenderUTF8_Blended_Wrapped(Fonts::Normal, Rogue::StatsLabels().c_str(), Color::S(Color::Active), 0);

        // texture
        SDL_Texture *texture = nullptr;

        if (surface_labels)
        {
            auto labels_h = std::max(BloodSword::TileSize, surface_labels->h + BloodSword::Pad);

            auto surface = Graphics::CreateSurface(w, BloodSword::TileSize + BloodSword::Pad + labels_h);

            if (surface)
            {
                // retrieve attribute values
                auto fpr = Interface::ScoreString(character, Attribute::Type::FIGHTING_PROWESS);

                auto awr = Interface::ScoreString(character, Attribute::Type::AWARENESS);

                auto psy = Interface::ScoreString(character, Attribute::Type::PSYCHIC_ABILITY);

                auto end = Interface::ScoreString(character, Attribute::Type::ENDURANCE);

                auto dmg = Interface::ScoreString(character, Attribute::Type::DAMAGE);

                auto arm = Interface::ScoreString(character, Attribute::Type::ARMOUR);

                auto four = std::string(4, ' ');

                // format attribute values
                auto stats_1 = four + fpr + std::string(5 - fpr.size(), ' ') + four + end + '\n';

                auto stats_2 = four + awr + std::string(5 - awr.size(), ' ') + four + dmg + '\n';

                auto stats_3 = four + psy + std::string(5 - psy.size(), ' ') + four + arm;

                auto stats = stats_1 + stats_2 + stats_3;

                // create and convert SDL surface to appropriate format
                auto surface_stats = TTF_RenderUTF8_Blended_Wrapped(Fonts::Normal, stats.c_str(), Color::S(Color::Highlight), 0);

                SDL_Rect stats_rect;

                stats_rect.w = surface->w;

                stats_rect.h = surface->h;

                stats_rect.x = BloodSword::Pad;

                stats_rect.y = BloodSword::TileSize + BloodSword::Pad;

                Graphics::RenderAsset(surface, surface_labels, stats_rect);

                Graphics::RenderAsset(surface, surface_stats, stats_rect);

                // add icon (blur if dead)
                auto surface_asset = Engine::Score(character, Attribute::Type::ENDURANCE) > 0 ? BloodSword::Asset::Surface(character.Asset) : BloodSword::Asset::Surface(character.Asset, Color::Inactive);

                stats_rect.x = (surface->w - BloodSword::TileSize) / 2;

                stats_rect.y = 0;

                Graphics::RenderAsset(surface, surface_asset, stats_rect);

                // cleanup
                BloodSword::Free(&surface_labels);

                BloodSword::Free(&surface_stats);

                BloodSword::Free(&surface_asset);

                texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

                BloodSword::Free(&surface);
            }

            // cleanup
            BloodSword::Free(&surface_labels);
        }

        return texture;
    }

    // generate textures of party stats
    BloodSword::Textures Stats(Graphics::Base &graphics, Party::Base &party, int w)
    {
        auto textures = std::vector<SDL_Texture *>();

        for (auto character = 0; character < party.Count(); character++)
        {
            auto texture = Rogue::Stats(graphics, party[character], w);

            textures.push_back(texture);
        }

        return textures;
    }
}

#endif
