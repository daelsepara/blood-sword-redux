#ifndef __TEST_HPP__
#define __TEST_HPP__

#include <SDL.h>

#include "Asset.hpp"
#include "Engine.hpp"
#include "Fonts.hpp"
#include "Generator.hpp"
#include "Graphics.hpp"
#include "Input.hpp"
#include "Interface.hpp"
#include "Map.hpp"

// framework for testing game subsystems
namespace BloodSword::Test
{
    void Controls(Graphics::Base &graphics)
    {
        if (graphics.Renderer)
        {
            auto x = 128;
            auto y = 64;
            auto w = 64;
            auto h = 64;
            auto s = 16;
            auto ws = w + s;

            // sprites
            auto warrior = Scene::Element(Asset::Get(Asset::Type::WARRIOR), x, y);
            auto trickster = Scene::Element(Asset::Get(Asset::Type::TRICKSTER), x + ws, y);
            auto sage = Scene::Element(Asset::Get(Asset::Type::SAGE), x + 2 * ws, y);
            auto enchanter = Scene::Element(Asset::Get(Asset::Type::ENCHANTER), x + 3 * ws, y);
            auto up = Scene::Element(Asset::Get(Asset::Type::UP), x, y + ws);
            auto down = Scene::Element(Asset::Get(Asset::Type::DOWN), x + ws, y + ws);
            auto left = Scene::Element(Asset::Get(Asset::Type::LEFT), x + 2 * ws, y + ws);
            auto right = Scene::Element(Asset::Get(Asset::Type::RIGHT), x + 3 * ws, y + ws);

            // text sprites
            auto text = Graphics::CreateText(graphics, "Every thirteen lunar months the Magi of Krarth hold a desperate contest to see which of them will rule that bleak and icy land. Teams of daring adventurers are sent down into the labyrinths that lie beneath the tundra, each searching for the Emblem of Victory that will win power for their patron.\n\nOnly one team can prevail. The others must die.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * ws - s);
            auto text_sprite = Scene::Element(text, x + 4 * ws, y);

            auto warrior_text = Graphics::CreateText(graphics, "You are a master of the fighting arts. You have better Fighting Prowess than any other character type, and when you strike a blow, you inflict more damage. You also have chainmail armour which provides an Armour rating of 3, which is better than the armour available to other characters.\n\nThese advantages give you a real edge in any fight, but you do not get things all your own way. You have none of the characters' special skills -- the Sage's ESP, for instance, or the Trickster's low devious cunning. Also, because you follow the honourable traditions of your class, you must be careful to stay true to the code of chivalry.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * ws - s);
            auto trickster_text = Graphics::CreateText(graphics, "Some adventurers are honourable and prefer to face their foes in a straight fight. You live by your wits. If you can win by trickery or by shooting someone in the back, you will. You know how to wield a sword if you have to, but your main weapon is cunning.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * ws - s);
            auto sage_text = Graphics::CreateText(graphics, "Your upbringing has been in the spartan Monastery of Illumination on the barren island of Kaxos. There, you have studied the Mystic Way, a series of demanding spiritual disciplines combined with rigorous physical training.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * ws - s);
            auto enchanter_text = Graphics::CreateText(graphics, "Forget the mundane arts of swordplay. You know that true power lies in the manipulation of occult powers of sorcery.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * ws - s);

            auto warrior_sprite = Scene::Element(warrior_text, x + 4 * ws, y);
            auto trickster_sprite = Scene::Element(trickster_text, x + 4 * ws, y);
            auto sage_sprite = Scene::Element(sage_text, x + 4 * ws, y);
            auto enchanter_sprite = Scene::Element(enchanter_text, x + 4 * ws, y);

            // controls
            std::vector<Controls::Base> controls = {
                Controls::Base(Controls::Type::WARRIOR, 0, 0, 1, 0, 4, x, y, w, h, Color::Highlight),
                Controls::Base(Controls::Type::TRICKSTER, 1, 0, 2, 1, 5, x + w + s, y, w, h, Color::Highlight),
                Controls::Base(Controls::Type::SAGE, 2, 1, 3, 2, 6, x + 2 * ws, y, w, h, Color::Highlight),
                Controls::Base(Controls::Type::ENCHANTER, 3, 2, 3, 3, 7, x + 3 * ws, y, w, h, Color::Highlight),
                Controls::Base(Controls::Type::MAP_UP, 4, 4, 5, 0, 4, x, y + ws, w, h, Color::Highlight),
                Controls::Base(Controls::Type::MAP_DOWN, 5, 4, 6, 1, 5, x + w + s, y + ws, w, h, Color::Highlight),
                Controls::Base(Controls::Type::MAP_LEFT, 6, 5, 7, 2, 6, x + 2 * ws, y + ws, w, h, Color::Highlight),
                Controls::Base(Controls::Type::MAP_RIGHT, 7, 6, 7, 3, 7, x + 3 * ws, y + ws, w, h, Color::Highlight)};

            auto sprites = std::vector<Scene::Element>();

            auto input = Controls::User();

            while (!input.Selected)
            {
                sprites.clear();
                sprites.push_back(warrior);
                sprites.push_back(trickster);
                sprites.push_back(sage);
                sprites.push_back(enchanter);
                sprites.push_back(up);
                sprites.push_back(down);
                sprites.push_back(left);
                sprites.push_back(right);

                if (input.Current >= 0 && input.Current < controls.size())
                {
                    switch (input.Current)
                    {
                    case 0:
                        sprites.push_back(warrior_sprite);
                        break;
                    case 1:
                        sprites.push_back(trickster_sprite);
                        break;
                    case 2:
                        sprites.push_back(sage_sprite);
                        break;
                    case 3:
                        sprites.push_back(enchanter_sprite);
                        break;
                    default:
                        sprites.push_back(text_sprite);
                    }
                }
                else
                {
                    sprites.push_back(text_sprite);
                }

                auto scene = Scene::Base(sprites, controls, 0);

                input = Input::WaitForInput(graphics, scene, input);
            }

            Graphics::Free(&enchanter_text);
            Graphics::Free(&sage_text);
            Graphics::Free(&trickster_text);
            Graphics::Free(&warrior_text);
            Graphics::Free(&text);
        }
    }

    void Map(Graphics::Base &graphics)
    {
        if (graphics.Renderer)
        {
            auto map = Map::Base();

            map.Load("maps/test.json");

            auto scene = Scene::Base();

            auto party = Party::Base({Generate::Character(Character::Class::WARRIOR, 8),
                                      Generate::Character(Character::Class::TRICKSTER, 8),
                                      Generate::Character(Character::Class::SAGE, 8),
                                      Generate::Character(Character::Class::ENCHANTER, 8)});

            auto enemies = Party::Base();

            auto input = Controls::User();

            auto textures = Graphics::CreateText(
                graphics,
                {Graphics::RichText("PLAYER", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * map.TileSize),
                 Graphics::RichText("ENEMY", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 8 * map.TileSize),
                 Graphics::RichText("PASSABLE", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * map.TileSize),
                 Graphics::RichText("ENEMY PASSABLE", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 8 * map.TileSize),
                 Graphics::RichText("OBSTACLE", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, 8 * map.TileSize),
                 Graphics::RichText("TEMPORARY OBSTACLE", Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL, 8 * map.TileSize),
                 Graphics::RichText("EXIT", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * map.TileSize),
                 Graphics::RichText("WARRIOR", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * map.TileSize),
                 Graphics::RichText("TRICKSTER", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * map.TileSize),
                 Graphics::RichText("SAGE", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * map.TileSize),
                 Graphics::RichText("ENCHANTER", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * map.TileSize)});

            while (true)
            {
                auto scene = Scene::Base();

                Interface::Add(map, scene, party, enemies, 1);

                auto id = (int)scene.Controls.size();

                auto y = map.DrawY + map.SizeY * map.TileSize;

                auto x = map.DrawX;

                scene.Add(Scene::Element(Asset::Get(Asset::Type::EXIT), x, y));

                scene.Add(Controls::Base(Controls::Type::EXIT, id, id, id, id - map.SizeX, id, x, y, map.TileSize, map.TileSize, Color::Active));

                if (input.Current >= 0 && input.Current < scene.Controls.size())
                {
                    if (scene.Controls[input.Current].IsMap)
                    {
                        auto tile = &map.Tiles[scene.Controls[input.Current].MapY][scene.Controls[input.Current].MapX];

                        auto object = -1;

                        auto background = Color::Inactive;

                        auto border = Color::Active;

                        if ((tile->IsOccupied() && tile->Occupant == Map::Object::PLAYER))
                        {
                            object = (int)party.Members[tile->Id].Class + 7;
                        }
                        else if (tile->IsPassable())
                        {
                            object = (int)tile->Type;
                        }
                        else if (tile->IsOccupied())
                        {
                            object = (int)tile->Occupant;

                            background = Color::Active;

                            border = Color::Inactive;
                        }
                        else
                        {
                            object = (int)tile->Type;

                            border = Color::Highlight;

                            background = Color::Highlight;
                        }

                        if (object >= 0 && object < textures.size())
                        {
                            auto element = Scene::Element(textures[object], map.DrawX + (2 * map.SizeX + 1) * map.TileSize / 2 + 10, map.DrawY + 10);

                            element.Set(background, border, 4);

                            scene.Add(element);
                        }
                    }
                }

                input = Input::WaitForInput(graphics, scene, input);

                if (input.Selected && input.Current >= 0 && input.Current < scene.Controls.size() && scene.Controls[input.Current].Type == Controls::Type::EXIT && !input.Hold)
                {
                    break;
                }
            }

            Graphics::Free(textures);
        }
    }
}

#endif
