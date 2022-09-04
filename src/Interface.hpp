#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include "Fonts.hpp"
#include "Graphics.hpp"
#include "Map.hpp"
#include "Party.hpp"

namespace BloodSword::Interface
{
    void AddMap(Map::Base &Map, Graphics::Scene &Scene, Party::Base &Party, Party::Base &Enemies)
    {
        auto NumControls = Scene.Controls.size();

        // map scroll up
        for (auto i = 0; i < Map.SizeX; i++)
        {
            auto id = NumControls;
            auto lt = i > 0 ? id - 1 : id;
            auto rt = i < Map.SizeX - 1 ? id + 1 : id;
            auto up = id;
            auto dn = id + Map.SizeX + 1;

            auto MapX = Map.DrawX + i * Map.TileSize;
            auto MapY = Map.DrawY - Map.TileSize;

            Scene.Add(Controls::Base(Controls::Type::MAP_UP, id, lt, rt, up, dn, MapX, MapY, Map.TileSize, Map.TileSize, Color::Background));

            NumControls++;
        }

        for (auto y = Map.MapY; y < Map.MapY + Map.SizeY; y++)
        {
            auto CtrlY = (y - Map.MapY);

            auto AssetY = Map.DrawY + CtrlY * Map.TileSize;

            for (auto x = Map.MapX; x < Map.MapX + Map.SizeX; x++)
            {
                auto CtrlX = (x - Map.MapX);

                // map scroll left
                if (CtrlX == 0)
                {
                    auto id = NumControls;
                    auto lt = id;
                    auto rt = id + 1;
                    auto up = CtrlY > 0 ? id - Map.SizeX - 2 : id;
                    auto dn = CtrlY < Map.SizeY - 1 ? id + Map.SizeX + 2 : id;

                    auto MapX = Map.DrawX - Map.TileSize;
                    auto MapY = AssetY;

                    Scene.Add(Controls::Base(Controls::Type::MAP_LEFT, id, lt, rt, up, dn, MapX, MapY, Map.TileSize, Map.TileSize, Color::Background));

                    NumControls++;
                }

                auto CtrlUp = NumControls;
                auto CtrlDn = NumControls;
                auto CtrlLt = NumControls;
                auto CtrlRt = NumControls;

                if (CtrlY > 0)
                {
                    CtrlUp = NumControls - Map.SizeX - 2;
                }
                else
                {
                    CtrlUp = NumControls - Map.SizeX - 1;
                }

                if (CtrlY < Map.SizeY - 1)
                {
                    CtrlDn = NumControls + Map.SizeX + 2;
                }
                else
                {
                    CtrlDn = NumControls + Map.SizeX + 1;
                }

                CtrlLt = NumControls - 1;
                CtrlRt = NumControls + 1;

                Map::Tile &Tile = Map.Tiles[y][x];

                auto AssetX = Map.DrawX + (x - Map.MapX) * Map.TileSize;

                auto ControlType = Controls::Type::MAP_NONE;

                auto ControlColor = Color::Inactive;

                if (Tile.IsOccupied())
                {
                    switch (Tile.Occupant)
                    {
                    case Map::Object::PLAYER:
                        if (Tile.Id >= 0 && Tile.Id < Party.Members.size())
                        {
                            if (Party.Members[Tile.Id].Value(Attribute::Type::Endurance) > 0)
                            {
                                Scene.Add(Graphics::SceneElement(Asset::Get(Party.Members[Tile.Id].Asset), AssetX, AssetY));

                                switch (Party.Members[Tile.Id].Type)
                                {
                                case Character::Type::Warrior:
                                    ControlType = Controls::Type::WARRIOR;
                                    break;
                                case Character::Type::Trickster:
                                    ControlType = Controls::Type::TRICKSTER;
                                    break;
                                case Character::Type::Sage:
                                    ControlType = Controls::Type::SAGE;
                                    break;
                                case Character::Type::Enchanter:
                                    ControlType = Controls::Type::ENCHANTER;
                                    break;
                                default:
                                    break;
                                }

                                ControlColor = Color::Highlight;
                            }
                        }
                        break;
                    case Map::Object::ENEMY:
                        if (Tile.Id >= 0 && Tile.Id < Enemies.Members.size())
                        {
                            if (Enemies.Members[Tile.Id].Value(Attribute::Type::Endurance) > 0)
                            {
                                Scene.Add(Graphics::SceneElement(Asset::Get(Enemies.Members[Tile.Id].Asset), AssetX, AssetY));

                                ControlType = Controls::Type::ENEMY;

                                ControlColor = Color::Highlight;
                            }
                        }
                        break;
                    case Map::Object::TEMPORARY_OBSTACLE:
                        if (Tile.Lifetime > 0)
                        {
                            if (Tile.TemporaryAsset != Asset::Type::NONE)
                            {
                                Scene.Add(Graphics::SceneElement(Asset::Get(Tile.TemporaryAsset), AssetX, AssetY));

                                ControlType = Controls::Type::TEMPORARY_OBSTACLE;

                                ControlColor = Color::Highlight;
                            }
                        }
                        else
                        {
                            if (Tile.Asset != Asset::Type::NONE)
                            {
                                Scene.Add(Graphics::SceneElement(Asset::Get(Tile.Asset), AssetX, AssetY));
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    if (Tile.IsExit())
                    {
                        ControlType = Controls::Type::MAP_EXIT;

                        ControlColor = Color::Highlight;
                    }
                    else if (Tile.IsPassable())
                    {
                        ControlType = Controls::Type::DESTINATION;

                        ControlColor = Color::Highlight;
                    }
                    else if (Tile.IsPassableToEnemy())
                    {
                        ControlColor = Color::Highlight;
                    }

                    if (Tile.Asset != Asset::Type::NONE)
                    {
                        Scene.Add(Graphics::SceneElement(Asset::Get(Tile.Asset), AssetX, AssetY));
                    }
                }

                Scene.Add(Controls::Base(ControlType, NumControls, CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, Map.TileSize, Map.TileSize, ControlColor));

                NumControls++;

                // map scroll right
                if (CtrlX == Map.SizeX - 1)
                {
                    auto id = NumControls;
                    auto lt = id - 1;
                    auto rt = id;
                    auto up = CtrlY > 0 ? id - Map.SizeX - 2 : id;
                    auto dn = CtrlY < Map.SizeY - 1 ? id + Map.SizeX + 2 : id;

                    auto MapX = Map.DrawX + Map.SizeX * Map.TileSize;

                    auto MapY = AssetY;

                    Scene.Add(Controls::Base(Controls::Type::MAP_RIGHT, id, lt, rt, up, dn, MapX, MapY, Map.TileSize, Map.TileSize, Color::Background));

                    NumControls++;
                }
            }
        }

        // map scroll down
        for (auto i = 0; i < Map.SizeX; i++)
        {
            auto id = NumControls;
            auto lt = i > 0 ? id - 1 : id;
            auto rt = i < Map.SizeX - 1 ? id + 1 : id;
            auto up = id - Map.SizeX - 1;
            auto dn = id;

            auto MapX = Map.DrawX + i * Map.TileSize;
            auto MapY = Map.DrawY + Map.SizeY * Map.TileSize;

            Scene.Add(Controls::Base(Controls::Type::MAP_DOWN, id, lt, rt, up, dn, MapX, MapY, Map.TileSize, Map.TileSize, Color::Background));

            NumControls++;
        }
    }
}

#endif
