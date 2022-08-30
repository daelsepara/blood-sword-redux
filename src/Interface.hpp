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

        auto StartMap = NumControls;

        Scene.Controls.erase(Scene.Controls.begin() + StartMap, Scene.Controls.end());

        // map scroll controls
        if (Scene.Controls.size() > 3)
        {
            Scene.Controls[0].Highlight = Map.MapY > 0 ? Color::Highlight : Color::Inactive;
            Scene.Controls[1].Highlight = Map.MapX > 0 ? Color::Highlight : Color::Inactive;
            Scene.Controls[2].Highlight = (Map.MapX < Map.Width - Map.SizeX) ? Color::Highlight : Color::Inactive;
            Scene.Controls[3].Highlight = (Map.MapY < Map.Height - Map.SizeY) ? Color::Highlight : Color::Inactive;
        }

        for (auto y = Map.MapY; y < Map.MapY + Map.SizeY; y++)
        {
            auto AssetY = Map.DrawY + (y - Map.MapY) * Map.TileSize;

            auto CtrlY = (y - Map.MapY);

            for (auto x = Map.MapX; x < Map.MapX + Map.SizeX; x++)
            {
                auto CtrlUp = NumControls;
                auto CtrlDn = NumControls;
                auto CtrlLt = NumControls;
                auto CtrlRt = NumControls;
                auto CtrlX = (x - Map.MapX);

                if (CtrlY > 0)
                {
                    CtrlUp = NumControls - Map.SizeX;
                }

                if (CtrlY < Map.SizeY - 1)
                {
                    CtrlDn = NumControls + Map.SizeX;
                }
                else
                {
                    if (CtrlX < (StartMap - 4))
                    {
                        CtrlDn = CtrlX + 4;
                    }
                    else
                    {
                        CtrlDn = 4;
                    }
                }

                if (CtrlX > 0)
                {
                    CtrlLt = NumControls - 1;
                }
                else
                {
                    if (CtrlY < Map.SizeY / 2)
                    {
                        if (CtrlY == 0)
                        {
                            CtrlLt = 0;
                        }
                        else
                        {
                            CtrlLt = 1;
                        }
                    }
                    else
                    {
                        if (CtrlY == Map.SizeY - 1)
                        {
                            CtrlLt = 3;
                        }
                        else
                        {
                            CtrlLt = 2;
                        }
                    }
                }

                if (CtrlX < Map.SizeX - 1)
                {
                    CtrlRt = NumControls + 1;
                }

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
                                Scene.Add(Graphics::SceneElements(Asset::Get(Party.Members[Tile.Id].Asset), AssetX, AssetY));

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
                                Scene.Add(Graphics::SceneElements(Asset::Get(Enemies.Members[Tile.Id].Asset), AssetX, AssetY));

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
                                Scene.Add(Graphics::SceneElements(Asset::Get(Tile.TemporaryAsset), AssetX, AssetY));

                                ControlType = Controls::Type::TEMPORARY_OBSTACLE;

                                ControlColor = Color::Highlight;
                            }
                        }
                        else
                        {
                            if (Tile.Asset != Asset::Type::NONE)
                            {
                                Scene.Add(Graphics::SceneElements(Asset::Get(Tile.Asset), AssetX, AssetY));
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
                        Scene.Add(Graphics::SceneElements(Asset::Get(Tile.Asset), AssetX, AssetY));
                    }
                }

                Scene.Add(Controls::Base(ControlType, NumControls, CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, Map.TileSize, Map.TileSize, ControlColor));

                NumControls++;
            }
        }
    }
}

#endif
