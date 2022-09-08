#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include "Fonts.hpp"
#include "Graphics.hpp"
#include "Map.hpp"
#include "Party.hpp"

namespace BloodSword::Interface
{
    void AddMap(Map::Base &Map, Graphics::Scene &Scene, Party::Base &Party, Party::Base &Enemies, int BottomControls)
    {
        auto NumControls = Scene.Controls.size();

        for (auto y = Map.MapY; y < Map.MapY + Map.SizeY; y++)
        {
            auto CtrlY = (y - Map.MapY);

            auto AssetY = Map.DrawY + CtrlY * Map.TileSize;

            for (auto x = Map.MapX; x < Map.MapX + Map.SizeX; x++)
            {
                auto CtrlX = (x - Map.MapX);

                auto CtrlUp = NumControls;
                auto CtrlDn = NumControls;
                auto CtrlLt = NumControls;
                auto CtrlRt = NumControls;

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
                    if (CtrlX < BottomControls)
                    {
                        CtrlDn = NumControls + Map.SizeX;
                    }
                }

                if (CtrlX > 0)
                {
                    CtrlLt = NumControls - 1;
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
                            if (Party.Members[Tile.Id].Value(Attribute::Type::ENDURANCE) > 0)
                            {
                                Scene.Add(Graphics::SceneElement(Asset::Get(Party.Members[Tile.Id].Asset), AssetX, AssetY));

                                switch (Party.Members[Tile.Id].Type)
                                {
                                case Character::Class::WARRIOR:
                                    ControlType = Controls::Type::WARRIOR;
                                    break;
                                case Character::Class::TRICKSTER:
                                    ControlType = Controls::Type::TRICKSTER;
                                    break;
                                case Character::Class::SAGE:
                                    ControlType = Controls::Type::SAGE;
                                    break;
                                case Character::Class::ENCHANTER:
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
                            if (Enemies.Members[Tile.Id].Value(Attribute::Type::ENDURANCE) > 0)
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

                Scene.Add(Controls::Base(ControlType, NumControls, CtrlLt, CtrlRt, CtrlUp, CtrlDn, AssetX, AssetY, Map.TileSize, Map.TileSize, ControlColor, x, y));

                NumControls++;
            }
        }
    }
}

#endif
