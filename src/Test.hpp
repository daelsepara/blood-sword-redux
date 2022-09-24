#ifndef __TEST_HPP__
#define __TEST_HPP__

#include "Asset.hpp"
#include "Engine.hpp"
#include "Fonts.hpp"
#include "Generator.hpp"
#include "Graphics.hpp"
#include "Input.hpp"
#include "Interface.hpp"
#include "Move.hpp"
#include "Animation.hpp"

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
            auto sage = Scene::Element(Asset::Get(Asset::Type::SAGE), x + ws * 2, y);
            auto enchanter = Scene::Element(Asset::Get(Asset::Type::ENCHANTER), x + ws * 3, y);
            auto up = Scene::Element(Asset::Get(Asset::Type::UP), x, y + ws);
            auto down = Scene::Element(Asset::Get(Asset::Type::DOWN), x + ws, y + ws);
            auto left = Scene::Element(Asset::Get(Asset::Type::LEFT), x + ws * 2, y + ws);
            auto right = Scene::Element(Asset::Get(Asset::Type::RIGHT), x + ws * 3, y + ws);

            // text sprites
            auto text = Graphics::CreateText(graphics, "THE BATTLEPITS OF KRARTH\n\nEvery thirteen lunar months the Magi of Krarth hold a desperate contest to see which of them will rule that bleak and icy land. Teams of daring adventurers are sent down into the labyrinths that lie beneath the tundra, each searching for the Emblem of Victory that will win power for their patron.\n\nOnly one team can prevail. The others must die.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * ws - s);
            auto text_sprite = Scene::Element(text, x + ws * 4, y);

            auto warrior_text = Graphics::CreateText(graphics, "WARRIOR\n\nYou are a master of the fighting arts. You have better Fighting Prowess than any other character type, and when you strike a blow, you inflict more damage. You also have chainmail armour which provides an Armour rating of 3, which is better than the armour available to other characters.\n\nThese advantages give you a real edge in any fight, but you do not get things all your own way. You have none of the other characters' special skills -- the Sage's ESP, for instance, or the Trickster's low devious cunning. Also, because you follow the honourable traditions of your class, you must be careful to stay true to the code of chivalry.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * ws - s);
            auto trickster_text = Graphics::CreateText(graphics, "TRICKSTER\n\nSome adventurers are honourable and prefer to face their foes in a straight fight. You live by your wits. If you can win by trickery or by shooting someone in the back, you will. You know how to wield a sword if you have to, but your main weapon is cunning.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * ws - s);
            auto sage_text = Graphics::CreateText(graphics, "SAGE\n\nYour upbringing has been in the spartan Monastery of Illumination on the barren island of Kaxos. There, you have studied the Mystic Way, a series of demanding spiritual disciplines combined with rigorous physical training.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * ws - s);
            auto enchanter_text = Graphics::CreateText(graphics, "ENCHANTER\n\nForget the mundane arts of swordplay. You know that true power lies in the manipulation of occult powers of sorcery.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * ws - s);

            auto warrior_sprite = Scene::Element(warrior_text, x + ws * 4, y);
            auto trickster_sprite = Scene::Element(trickster_text, x + ws * 4, y);
            auto sage_sprite = Scene::Element(sage_text, x + ws * 4, y);
            auto enchanter_sprite = Scene::Element(enchanter_text, x + ws * 4, y);

            // controls
            std::vector<Controls::Base> controls = {
                Controls::Base(Controls::Type::WARRIOR, 0, 0, 1, 0, 4, x, y, w, h, Color::Highlight),
                Controls::Base(Controls::Type::TRICKSTER, 1, 0, 2, 1, 5, x + w + s, y, w, h, Color::Highlight),
                Controls::Base(Controls::Type::SAGE, 2, 1, 3, 2, 6, x + ws * 2, y, w, h, Color::Highlight),
                Controls::Base(Controls::Type::ENCHANTER, 3, 2, 3, 3, 7, x + ws * 3, y, w, h, Color::Highlight),
                Controls::Base(Controls::Type::MAP_UP, 4, 4, 5, 0, 4, x, y + ws, w, h, Color::Highlight),
                Controls::Base(Controls::Type::MAP_DOWN, 5, 4, 6, 1, 5, x + w + s, y + ws, w, h, Color::Highlight),
                Controls::Base(Controls::Type::MAP_LEFT, 6, 5, 7, 2, 6, x + ws * 2, y + ws, w, h, Color::Highlight),
                Controls::Base(Controls::Type::MAP_RIGHT, 7, 6, 7, 3, 7, x + ws * 3, y + ws, w, h, Color::Highlight)};

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

            BloodSword::Free(&enchanter_text);
            BloodSword::Free(&sage_text);
            BloodSword::Free(&trickster_text);
            BloodSword::Free(&warrior_text);
            BloodSword::Free(&text);
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

            auto pad = 10;

            auto textw = map.TileSize * 8 + 3 * pad;

            auto objectw = map.TileSize * 5;

            auto textures = Graphics::CreateText(
                graphics,
                {Graphics::RichText("PLAYER", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("ENEMY", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("PASSABLE", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("ENEMY PASSABLE", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("OBSTACLE", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("TEMPORARY OBSTACLE", Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("EXIT", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("WARRIOR", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("TRICKSTER", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("SAGE", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("ENCHANTER", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, objectw)});

            auto backgrounds = Graphics::CreateText(
                graphics,
                {Graphics::RichText("THE BATTLEPITS OF KRARTH\n\nEvery thirteen lunar months the Magi of Krarth hold a desperate contest to see which of them will rule that bleak and icy land. Teams of daring adventurers are sent down into the labyrinths that lie beneath the tundra, each searching for the Emblem of Victory that will win power for their patron.\n\nOnly one team can prevail. The others must die.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, textw),
                 Graphics::RichText("WARRIOR\n\nYou are a master of the fighting arts. You have better Fighting Prowess than any other character type, and when you strike a blow, you inflict more damage. You also have chainmail armour which provides an Armour rating of 3, which is better than the armour available to other characters.\n\nThese advantages give you a real edge in any fight, but you do not get things all your own way. You have none of the other characters' special skills -- the Sage's ESP, for instance, or the Trickster's low devious cunning. Also, because you follow the honourable traditions of your class, you must be careful to stay true to the code of chivalry.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, textw),
                 Graphics::RichText("TRICKSTER\n\nSome adventurers are honourable and prefer to face their foes in a straight fight. You live by your wits. If you can win by trickery or by shooting someone in the back, you will. You know how to wield a sword if you have to, but your main weapon is cunning.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, textw),
                 Graphics::RichText("SAGE\n\nYour upbringing has been in the spartan Monastery of Illumination on the barren island of Kaxos. There, you have studied the Mystic Way, a series of demanding spiritual disciplines combined with rigorous physical training.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, textw),
                 Graphics::RichText("ENCHANTER\n\nForget the mundane arts of swordplay. You know that true power lies in the manipulation of occult powers of sorcery.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, textw)});

            std::vector<SDL_Texture *> stats = {
                Interface::Attributes(graphics, party[0], Fonts::Fixed, Color::Active, Color::Highlight, TTF_STYLE_NORMAL, objectw),
                Interface::Attributes(graphics, party[1], Fonts::Fixed, Color::Active, Color::Highlight, TTF_STYLE_NORMAL, objectw),
                Interface::Attributes(graphics, party[2], Fonts::Fixed, Color::Active, Color::Highlight, TTF_STYLE_NORMAL, objectw),
                Interface::Attributes(graphics, party[3], Fonts::Fixed, Color::Active, Color::Highlight, TTF_STYLE_NORMAL, objectw)};

            auto background = 0;
            auto prev_background = -1;
            auto change = false;
            auto offset = 0;
            auto objectx = map.DrawX + (map.SizeX * 2 + 1) * map.TileSize / 2 + pad;
            auto objecty = map.DrawY + map.TileSize * 7 - pad;
            auto backgroundx = objectx;
            auto backgroundy = map.DrawY + pad;
            auto backgroundw = textw;
            auto backgroundh = map.TileSize * 5;
            auto texturew = 0;
            auto textureh = 0;
            auto scrollx = objectx + backgroundw + pad * 2;
            auto scrolly = map.DrawY + backgroundh - map.TileSize + pad * 2;
            auto scrollSpeed = 20;
            auto scroll_up = false;
            auto scroll_down = false;

            SDL_QueryTexture(backgrounds[background], NULL, NULL, &texturew, &textureh);

            while (true)
            {
                auto scene = Scene::Base();

                Interface::Add(map, scene, party, enemies, 1);

                auto id = (int)scene.Controls.size();
                auto x = map.DrawX;
                auto y = map.DrawY + map.SizeY * map.TileSize;

                scene.Add(Scene::Element(Asset::Get(Asset::Type::EXIT), x, y));

                auto arrow_up = offset > 0;
                auto arrow_down = backgroundh < textureh && offset < (textureh - backgroundh);

                if (arrow_up)
                {
                    scene.Add(Scene::Element(Asset::Get(Asset::Type::UP), scrollx, map.DrawY));
                }

                if (arrow_down)
                {
                    scene.Add(Scene::Element(Asset::Get(Asset::Type::DOWN), scrollx, scrolly));
                }

                if (arrow_up || arrow_down)
                {
                    scene.Add(Controls::Base(Controls::Type::EXIT, id, id, id + 1, id - map.SizeX, id, x, y, map.TileSize, map.TileSize, Color::Active));
                }

                if (arrow_up && arrow_down)
                {
                    scene.Add(Controls::Base(Controls::Type::SCROLL_UP, id + 1, id, id + 1, id + 1, id + 2, scrollx, map.DrawY, map.TileSize, map.TileSize, Color::Inactive));
                    scene.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id + 2, id, id + 2, id + 1, id + 2, scrollx, scrolly, map.TileSize, map.TileSize, Color::Inactive));
                }
                else if (arrow_up)
                {
                    scene.Add(Controls::Base(Controls::Type::SCROLL_UP, id + 1, id, id + 1, id + 1, id + 1, scrollx, map.DrawY, map.TileSize, map.TileSize, Color::Inactive));
                }
                else if (arrow_down)
                {
                    scene.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id + 1, id, id + 1, id + 1, id + 1, scrollx, scrolly, map.TileSize, map.TileSize, Color::Inactive));
                }
                else
                {
                    scene.Add(Controls::Base(Controls::Type::EXIT, id, id, id, id - map.SizeX, id, x, y, map.TileSize, map.TileSize, Color::Active));
                }

                if (input.Current >= 0 && input.Current < scene.Controls.size())
                {
                    auto &control = scene.Controls[input.Current];

                    if (control.IsMap)
                    {
                        auto &tile = map.Tiles[control.MapY][control.MapX];
                        auto background = Color::Inactive;
                        auto border = Color::Active;
                        auto object = -1;

                        if ((tile.IsOccupied() && tile.Occupant == Map::Object::PLAYER))
                        {
                            // stats
                            auto bounds = 0;

                            SDL_QueryTexture(stats[tile.Id], NULL, NULL, NULL, &bounds);

                            scene.Add(Scene::Element(stats[tile.Id], objectx + objectw + pad * 3, objecty, bounds, 0, Color::Background, Color::Active, 4));

                            // character class
                            object = (int)party[tile.Id].Class + 7;
                        }
                        else if (tile.IsPassable())
                        {
                            object = (int)tile.Type;
                        }
                        else if (tile.IsOccupied())
                        {
                            object = (int)tile.Occupant;
                            background = Color::Active;
                            border = Color::Inactive;
                        }
                        else
                        {
                            object = (int)tile.Type;
                            border = Color::Highlight;
                            background = Color::Highlight;
                        }

                        if (object >= 0 && object < textures.size())
                        {
                            scene.Add(Scene::Element(objectx, objecty, objectw, map.TileSize, background, border, 4));
                            scene.Add(Scene::Element(textures[object], objectx, objecty));
                        }
                    }
                }

                if (background >= 0 && background < backgrounds.size())
                {
                    scene.Add(Scene::Element(backgroundx, backgroundy, backgroundw, backgroundh, Color::Inactive, Color::Active, 4));
                    scene.Add(Scene::Element(backgrounds[background], backgroundx, backgroundy, backgroundh, offset));
                }

                if (scroll_up)
                {
                    input.Current = Controls::Find(scene.Controls, Controls::Type::SCROLL_UP);

                    scroll_up = false;
                }
                else if (scroll_down)
                {
                    input.Current = Controls::Find(scene.Controls, Controls::Type::SCROLL_DOWN);

                    scroll_down = false;
                }

                input = Input::WaitForInput(graphics, scene, input);

                if ((input.Selected && input.Type != Controls::Type::NONE && !input.Hold) || input.Up || input.Down)
                {
                    if (input.Type == Controls::Type::EXIT)
                    {
                        break;
                    }
                    else if (input.Type == Controls::Type::WARRIOR)
                    {
                        prev_background = background;

                        background = 1;

                        change = true;
                    }
                    else if (input.Type == Controls::Type::TRICKSTER)
                    {
                        prev_background = background;

                        background = 2;

                        change = true;
                    }
                    else if (input.Type == Controls::Type::SAGE)
                    {
                        prev_background = background;

                        background = 3;

                        change = true;
                    }
                    else if (input.Type == Controls::Type::ENCHANTER)
                    {
                        prev_background = background;

                        background = 4;

                        change = true;
                    }
                    else if (input.Type == Controls::Type::SCROLL_UP || input.Up)
                    {
                        if (backgroundh < textureh)
                        {
                            offset -= scrollSpeed;

                            if (offset < 0)
                            {
                                offset = 0;
                            }

                            scroll_up = true;
                        }
                    }
                    else if (input.Type == Controls::Type::SCROLL_DOWN || input.Down)
                    {
                        if (backgroundh < textureh)
                        {
                            offset += scrollSpeed;

                            if (offset > (textureh - backgroundh))
                            {
                                offset = textureh - backgroundh;
                            }

                            scroll_down = true;
                        }
                    }
                    else
                    {
                        prev_background = background;

                        background = 0;

                        change = true;
                    }

                    if (background != prev_background && background >= 0 && background < backgrounds.size())
                    {
                        if (change)
                        {
                            SDL_QueryTexture(backgrounds[background], NULL, NULL, &texturew, &textureh);

                            offset = 0;

                            change = false;
                        }
                    }
                }
            }

            BloodSword::Free(stats);
            BloodSword::Free(backgrounds);
            BloodSword::Free(textures);
        }
    }

    void Animation(Graphics::Base &graphics)
    {
        auto Class = Character::Class::TRICKSTER;

        auto party = Party::Base(Generate::Character(Class, 8));

        auto enemies = Party::Base();

        auto map = Map::Base();
        
        auto mazex = (graphics.Width / map.TileSize) - 2;
        auto mazey = (graphics.Height / map.TileSize) - 3;

        if ((mazex & 1) == 0)
        {
            mazex--;
        }

        if ((mazey & 1) == 0)
        {
            mazey--;
        }

        map.Generate(mazex, mazey);
        map.Viewable(mazex, mazey);

        auto start = Point(1, map.Height - 1);
        auto end = Point(map.Width - 1, 1);
        auto draw = Point(map.DrawX, map.DrawY);

        auto RegenerateScene = [&](Map::Base &map)
        {
            auto scene = Scene::Base();

            Interface::Add(map, scene, party, enemies, 3);

            auto id = scene.Controls.size();

            scene.Add(Scene::Element(Asset::Get(Asset::Type::BACK), map.DrawX, map.DrawY + map.SizeY * map.TileSize));
            scene.Add(Scene::Element(Asset::Get(Asset::Type::MOVE), map.DrawX + map.TileSize, map.DrawY + map.SizeY * map.TileSize));
            scene.Add(Scene::Element(Asset::Get(Asset::Type::EXIT), map.DrawX + map.TileSize * 2, map.DrawY + map.SizeY * map.TileSize));
            scene.Add(Controls::Base(Controls::Type::BACK, id, id, id + 1, id - map.SizeX, id, map.DrawX, map.DrawY + map.SizeY * map.TileSize, map.TileSize, map.TileSize, Color::Active));
            scene.Add(Controls::Base(Controls::Type::MOVE, id + 1, id, id + 2, id - map.SizeX + 1, id + 1, map.DrawX + map.TileSize, map.DrawY + map.SizeY * map.TileSize, map.TileSize, map.TileSize, Color::Active));
            scene.Add(Controls::Base(Controls::Type::EXIT, id + 2, id + 1, id + 2, id - map.SizeX + 2, id + 2, map.DrawX + map.TileSize * 2, map.DrawY + map.SizeY * map.TileSize, map.TileSize, map.TileSize, Color::Active));

            return scene;
        };

        auto ResetObjects = [&](Map::Base &map)
        {
            map.Put(start, Map::Object::PASSABLE, Asset::Type::NONE);

            map.Put(start, Map::Object::PLAYER, 0);

            map.Put(end, Map::Object::PASSABLE, Asset::Type::RIGHT);
        };

        auto input = Controls::User();

        // initialize animation
        auto animations = Animations::Base();

        // set frame, type, and delay
        auto movement = Animation::Base(
            {Animation::Frame(Asset::Get(party[Class].Asset))},
            {Animation::Type::MOVE},
            {},
            1,
            16,
            false);

        // scale movement scale to map dimensions
        movement.Scale = Point(map.TileSize, map.TileSize);

        movement.Delta = Point(8, 8);

        ResetObjects(map);

        auto background = RegenerateScene(map);

        auto done = true;

        Uint32 frameStart = 0;

        auto frames = 0;

        auto fps = 0.0;

        SDL_Texture *fpsDisplay = NULL;

        while (true)
        {
            if (done)
            {
                if (fpsDisplay)
                {
                    auto overlay = Scene::Base(fpsDisplay, map.DrawX, map.TileSize / 2);

                    input = Input::WaitForInput(graphics, background, overlay, input);
                }
                else
                {
                    input = Input::WaitForInput(graphics, background, input);
                }

                if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
                {
                    if (input.Type == Controls::Type::EXIT)
                    {
                        break;
                    }
                    else if (input.Type == Controls::Type::MOVE && done)
                    {
                        // find a path to the exit
                        auto path = Move::FindPath(map, start, end);

                        auto validMoves = Move::Count(map, path, false);

                        if (validMoves > 0)
                        {
                            Interface::Clip(graphics, map);

                            map.Put(start, Map::Object::NONE, -1);

                            map.Put(end, Map::Object::NONE, -1);

                            background = RegenerateScene(map);

                            // setup animation
                            movement.Set(draw, start);

                            auto first = path.Points.begin();

                            // add destination to the count
                            auto moves = std::min(validMoves + 1, party[Class].Moves);

                            movement.Path = std::vector<Point>(first, first + moves);

                            movement.Reset();

                            // add to list
                            animations.Clear();

                            animations.Add(movement);

                            done = false;

                            Free(&fpsDisplay);

                            frameStart = SDL_GetTicks();

                            frames = 0;

                            fps = 0.0;
                        }
                    }
                    else if (input.Type == Controls::Type::BACK && done)
                    {
                        map.Generate(mazex, mazey);

                        map.Viewable(mazex, mazey);

                        ResetObjects(map);

                        background = RegenerateScene(map);
                    }
                }
            }
            else
            {
                done = Graphics::Animate(graphics, background, animations);

                Graphics::Refresh(graphics);

                frames++;

                if (done)
                {
                    // compute FPS
                    auto msec = SDL_GetTicks() - frameStart;

                    if (msec > 0)
                    {
                        Free(&fpsDisplay);

                        fps = frames * 1000.0 / (double)msec;

                        std::string fpsString = "FPS: " + std::to_string(fps) + "/sec";

                        auto fpsWidth = 0;

                        Graphics::Estimate(Fonts::Normal, fpsString.c_str(), &fpsWidth, NULL);

                        fpsDisplay = Graphics::CreateText(graphics, fpsString.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, fpsWidth);
                    }

                    for (auto &animation : animations.List)
                    {
                        if (animation.Is(Animation::Type::MOVE) && animation.IsScaled())
                        {
                            map.Put(animation.Current, Map::Object::PLAYER, 0);
                        }
                    }

                    Input::Flush();

                    input.Current = -1;

                    input.Selected = false;

                    background = RegenerateScene(map);

                    Interface::Clip(graphics);
                }
            }
        }

        Free(&fpsDisplay);
    }
}

#endif
