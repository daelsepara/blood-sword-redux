#ifndef __TEST_HPP__
#define __TEST_HPP__

#include "Input.hpp"
#include "Interface.hpp"

// framework for testing game subsystems
namespace BloodSword::Test
{
    // user input test
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
            auto warrior_sprite = Scene::Element(warrior_text, x + ws * 4, y);
            auto trickster_text = Graphics::CreateText(graphics, "TRICKSTER\n\nSome adventurers are honourable and prefer to face their foes in a straight fight. You live by your wits. If you can win by trickery or by shooting someone in the back, you will. You know how to wield a sword if you have to, but your main weapon is cunning.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * ws - s);
            auto trickster_sprite = Scene::Element(trickster_text, x + ws * 4, y);
            auto sage_text = Graphics::CreateText(graphics, "SAGE\n\nYour upbringing has been in the spartan Monastery of Illumination on the barren island of Kaxos. There, you have studied the Mystic Way, a series of demanding spiritual disciplines combined with rigorous physical training.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * ws - s);
            auto sage_sprite = Scene::Element(sage_text, x + ws * 4, y);
            auto enchanter_text = Graphics::CreateText(graphics, "ENCHANTER\n\nForget the mundane arts of swordplay. You know that true power lies in the manipulation of occult powers of sorcery.", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, 8 * ws - s);
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

                if (Input::IsValid(controls, input))
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

    // map rendering test
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
                 Graphics::RichText("ENEMY PASSABLE", Fonts::Normal, Color::S(Color::Inactive), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("OBSTACLE", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("TEMPORARY OBSTACLE", Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("EXIT", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("WARRIOR", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("TRICKSTER", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("SAGE", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, objectw),
                 Graphics::RichText("ENCHANTER", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, objectw)});

            auto backgrounds = Graphics::CreateText(
                graphics,
                {Graphics::RichText("THE BATTLEPITS OF KRARTH\n\nEvery thirteen lunar months the Magi of Krarth hold a desperate contest to see which of them will rule that bleak and icy land. Teams of daring adventurers are sent down into the labyrinths that lie beneath the tundra, each searching for the Emblem of Victory that will win power for their patron.\n\nOnly one team can prevail. The others must die.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, textw),
                 Graphics::RichText("WARRIOR\n\nYou are a master of the fighting arts. You have better Fighting Prowess than any other character type, and when you strike a blow, you inflict more damage. You also have chainmail armour which provides an Armour rating of 3, which is better than the armour available to other characters.\n\nThese advantages give you a real edge in any fight, but you do not get things all your own way. You have none of the other characters' special skills -- the Sage's ESP, for instance, or the Trickster's low devious cunning. Also, because you follow the honourable traditions of your class, you must be careful to stay true to the code of chivalry.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, textw),
                 Graphics::RichText("TRICKSTER\n\nSome adventurers are honourable and prefer to face their foes in a straight fight. You live by your wits. If you can win by trickery or by shooting someone in the back, you will. You know how to wield a sword if you have to, but your main weapon is cunning.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, textw),
                 Graphics::RichText("SAGE\n\nYour upbringing has been in the spartan Monastery of Illumination on the barren island of Kaxos. There, you have studied the Mystic Way, a series of demanding spiritual disciplines combined with rigorous physical training.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, textw),
                 Graphics::RichText("ENCHANTER\n\nForget the mundane arts of swordplay. You know that true power lies in the manipulation of occult powers of sorcery.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, textw)});

            auto stats = Interface::Attributes(graphics, party, Fonts::Fixed, Color::Active, Color::Highlight, TTF_STYLE_NORMAL, objectw);

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
                auto scene = Interface::Map(map, party, enemies, 1);

                auto id = (int)(scene.Controls.size());
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

                if (Input::IsValid(scene, input))
                {
                    auto &control = scene.Controls[input.Current];

                    if (control.OnMap)
                    {
                        auto &tile = map[control.Map];
                        auto background = Color::Highlight;
                        auto border = Color::Active;
                        auto object = -1;

                        if (tile.IsOccupied() && tile.Occupant == Map::Object::PLAYER)
                        {
                            // stats
                            scene.Add(Scene::Element(stats[tile.Id], objectx + objectw + pad * 3, objecty, Color::Background, Color::Active, 4));

                            // character class
                            object = (int)party[tile.Id].Class + 7;
                        }
                        else if (tile.IsPassable())
                        {
                            object = (int)tile.Type;
                            background = Color::Background;
                            border = Color::Active;
                        }
                        else if (tile.IsPassableToEnemy())
                        {
                            object = (int)tile.Type;
                            background = Color::Background;
                            border = Color::Highlight;
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
                            background = Color::Highlight;
                            border = Color::Inactive;
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
                    scene.Add(Scene::Element(backgroundx, backgroundy, backgroundw, backgroundh, Color::Highlight, Color::Active, 4));
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

    // animation test
    void Animation(Graphics::Base &graphics)
    {
        auto Class = Character::Class::TRICKSTER;
        auto party = Party::Base(Generate::Character(Class, 8));
        auto enemies = Party::Base();

        auto map = Map::Base();
        auto mazew = (graphics.Width / map.TileSize) - 2;
        auto mazeh = (graphics.Height / map.TileSize) - 3;

        if ((mazew & 1) == 0)
        {
            mazew--;
        }

        if ((mazeh & 1) == 0)
        {
            mazeh--;
        }

        Maze::Generate(map, mazew, mazeh);
        map.Viewable(mazew, mazeh);

        auto random = Random::Base();

        auto start = Point(1, map.Height - 1);
        auto exit = (random.NextInt() % (mazeh / 2)) * 2 + 1;
        auto end = Point(map.Width - 1, exit);
        auto draw = Point(map.DrawX, map.DrawY);

        auto RegenerateScene = [&](Map::Base &map)
        {
            auto scene = Interface::Map(map, party, enemies, 3);

            auto id = (int)(scene.Controls.size());

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

            exit = (random.NextInt() % (mazeh / 2)) * 2 + 1;

            end = Point(map.Width - 1, exit);

            map.Put(end, Map::Object::PASSABLE, Asset::Type::RIGHT);
        };

        auto input = Controls::User();

        // initialize animation
        auto animations = Animations::Base();

        // set frame, type, and delay
        auto movement = Interface::Movement(map, party[Class], {}, start);

        ResetObjects(map);

        auto background = RegenerateScene(map);

        auto done = true;

        Uint32 framestart = 0;

        auto frames = 0;

        auto fps = 0.0;

        SDL_Texture *ftptexture = NULL;

        while (true)
        {
            if (done)
            {
                if (ftptexture)
                {
                    auto overlay = Scene::Base(ftptexture, map.DrawX, map.TileSize / 2);

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

                        auto valid = Move::Count(map, path, false);

                        if (valid > 0)
                        {
                            map.Put(start, Map::Object::NONE, -1);

                            background = RegenerateScene(map);

                            Interface::Clip(graphics, map);

                            // add destination to the count
                            auto first = path.Points.begin();

                            auto moves = std::min(valid + 1, party[Class].Moves);

                            movement = Interface::Movement(map, party[Class], std::vector<Point>(first, first + moves), start);

                            // add to list
                            animations.Clear();

                            animations.Add(movement);

                            done = false;

                            framestart = SDL_GetTicks();

                            frames = 0;

                            fps = 0.0;
                        }
                    }
                    else if (input.Type == Controls::Type::BACK && done)
                    {
                        Maze::Generate(map, mazew, mazeh);

                        map.Viewable(mazew, mazeh);

                        ResetObjects(map);

                        background = RegenerateScene(map);
                    }
                }
            }
            else
            {
                done = Graphics::Animate(graphics, background, animations, true);

                Graphics::Refresh(graphics);

                frames++;

                if (done)
                {
                    // compute FPS
                    auto msec = SDL_GetTicks() - framestart;

                    if (msec > 0)
                    {
                        Free(&ftptexture);

                        fps = frames * 1000.0 / (double)msec;

                        std::string fpsString = "FPS: " + std::to_string(fps) + "/sec";

                        auto fpswidth = 0;

                        Graphics::Estimate(Fonts::Normal, fpsString.c_str(), &fpswidth, NULL);

                        ftptexture = Graphics::CreateText(graphics, fpsString.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, fpswidth);
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

        Free(&ftptexture);
    }

    // battle order
    void Queue(Graphics::Base &graphics)
    {
        auto xdim = 14;

        auto ydim = 9;

        auto map = Map::Base(xdim, ydim);

        map.Viewable(xdim, ydim);

        // generate map
        for (auto y = 0; y < ydim; y++)
        {
            for (auto x = 0; x < xdim; x++)
            {
                map.Put(x, y, Map::Object::PASSABLE, Asset::Type::NONE);
            }
        }

        for (auto i = 0; i < ydim; i++)
        {
            map.Put(0, i, Map::Object::OBSTACLE, Asset::Type::WALL);
            map.Put(map.Width - 1, i, Map::Object::OBSTACLE, Asset::Type::WALL);
        }

        for (auto i = 0; i < xdim; i++)
        {
            map.Put(i, 0, Map::Object::OBSTACLE, Asset::Type::WALL);
            map.Put(i, map.Height - 1, Map::Object::OBSTACLE, Asset::Type::WALL);
        }

        // create party
        auto party = Party::Base({Generate::Character(Character::Class::WARRIOR, 2),
                                  Generate::Character(Character::Class::TRICKSTER, 2),
                                  Generate::Character(Character::Class::SAGE, 2),
                                  Generate::Character(Character::Class::ENCHANTER, 2)});

        auto enemies = Party::Base(
            {Generate::NPC("BARBARIAN", {}, 8, 5, 7, 12, 1, 1, 2, 100, Asset::Type::BARBARIAN),
             Generate::NPC("ASSASSIN", {}, 7, 6, 7, 5, 0, 1, 0, 100, Asset::Type::ASSASSIN),
             Generate::NPC("CORPSE", {}, 5, 4, 2, 4, 0, 1, 1, 3, Asset::Type::CORPSE)});

        std::vector<Point> origins = {
            Point(1, 1),
            Point(map.Width - 2, 1),
            Point(1, map.Height - 2),
            Point(map.Width - 2, map.Height - 2),
        };

        auto center = Point(xdim, ydim) / 2;

        std::vector<Point> spawn = {
            center,
            center + 1,
            center - 1};

        auto stats = Interface::Attributes(graphics, party, Fonts::Normal, Color::Active, Color::Highlight, TTF_STYLE_NORMAL, map.TileSize * 5, true);

        auto enemyStats = Interface::Attributes(graphics, enemies, Fonts::Normal, Color::Active, Color::Highlight, TTF_STYLE_NORMAL, map.TileSize * 5, true);

        auto draw = Point(map.DrawX, map.DrawY);

        auto captionw = 320;

        // initialize captions
        auto characters = Graphics::CreateText(
            graphics,
            {Graphics::RichText("WARRIOR", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, captionw),
             Graphics::RichText("TRICKSTER", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, captionw),
             Graphics::RichText("SAGE", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, captionw),
             Graphics::RichText("ENCHANTER", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, captionw)});

        auto captions = Graphics::CreateText(
            graphics,
            {Graphics::RichText("Move character", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, captionw),
             Graphics::RichText("Reset starting locations", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, captionw),
             Graphics::RichText("Back to test suite", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, captionw)});

        auto RegenerateScene = [&](Map::Base &map)
        {
            auto scene = Interface::Map(map, party, enemies, 3);

            auto id = scene.Controls.size();

            scene.Add(Scene::Element(Asset::Get(Asset::Type::MOVE), map.DrawX, map.DrawY + map.SizeY * map.TileSize));
            scene.Add(Scene::Element(Asset::Get(Asset::Type::BACK), map.DrawX + map.TileSize, map.DrawY + map.SizeY * map.TileSize));
            scene.Add(Scene::Element(Asset::Get(Asset::Type::EXIT), map.DrawX + map.TileSize * 2, map.DrawY + map.SizeY * map.TileSize));
            scene.Add(Controls::Base(Controls::Type::MOVE, id, id, id + 1, id - map.SizeX, id, map.DrawX, map.DrawY + map.SizeY * map.TileSize, map.TileSize, map.TileSize, Color::Active));
            scene.Add(Controls::Base(Controls::Type::RESET, id + 1, id, id + 2, id - map.SizeX + 1, id + 1, map.DrawX + map.TileSize, map.DrawY + map.SizeY * map.TileSize, map.TileSize, map.TileSize, Color::Active));
            scene.Add(Controls::Base(Controls::Type::EXIT, id + 2, id + 1, id + 2, id - map.SizeX + 2, id + 2, map.DrawX + map.TileSize * 2, map.DrawY + map.SizeY * map.TileSize, map.TileSize, map.TileSize, Color::Active));

            return scene;
        };

        auto ResetLocations = [&](Map::Base &map, Party::Base &party, std::vector<Point> origins)
        {
            for (auto i = 0; i < party.Count(); i++)
            {
                auto location = map.Find(Map::Object::PLAYER, i);

                if (!location.IsNone())
                {
                    map.Put(location, Map::Object::NONE, -1);
                }

                map.Put(origins[i], Map::Object::PLAYER, i);
            }

            for (auto i = 0; i < enemies.Count(); i++)
            {
                auto enemy = map.Find(Map::Object::ENEMY, i);

                if (!enemy.IsNone())
                {
                    map.Put(enemy, Map::Object::NONE, -1);
                }

                map.Put(spawn[i], Map::Object::ENEMY, i);
            }
        };

        auto Blink = [&](Map::Base &map, Engine::Queue &order, int &character, Scene::Base &overlay)
        {
            // blink cursor
            if (Engine::IsPlayer(order, character))
            {
                auto blink = map.Find(Map::Object::PLAYER, order[character].ID);

                if (map.IsValid(blink))
                {
                    auto screen = (draw + blink * map.TileSize) + 4;

                    overlay.Add(Scene::Element(screen.X, screen.Y, map.TileSize - 8, map.TileSize - 8, 0, Color::Active, 2));
                }
            }
        };

        // set starting locations
        ResetLocations(map, party, origins);

        // generate a queue based on AWARENESS score
        auto order = Engine::Build(party, enemies, Attribute::Type::AWARENESS);

        auto character = 0;

        auto blinking = false;

        auto input = Controls::User();

        auto previous = Controls::User();

        auto animating = false;

        auto move = false;

        auto done = false;

        auto scene = RegenerateScene(map);

        auto movement = Animation::Base();

        if (Engine::IsPlayer(order, character))
        {
            movement = Interface::Movement(map, party[order[character].ID], {}, origins[order[character].ID]);
        }
        else
        {
            movement = Interface::Movement(map, enemies[order[character].ID], {}, spawn[order[character].ID]);
        }

        auto pad = 10;
        auto objectx = map.DrawX + (map.SizeX * 2 + 1) * map.TileSize / 2 + pad;
        auto objecty = map.DrawY + pad;
        auto skill = false;
        auto spell = false;
        auto popupid = 0;
        auto inbattle = true;

        while (!done)
        {
            auto overlay = Scene::Base();

            // check if this character can move
            if (!animating)
            {
                auto src = map.Find(Engine::IsPlayer(order, character) ? Map::Object::PLAYER : Map::Object::ENEMY, order[character].ID);

                if (!src.IsNone())
                {
                    if (!Move::Available(map, src))
                    {
                        Engine::Next(order, character);
                    }
                    else
                    {
                        // if enemy, move to nearest target
                        if (!Engine::IsPlayer(order, character))
                        {
                            auto targets = Engine::Targets(map, party, src);

                            if (targets.size() > 0)
                            {
                                auto validtarget = false;

                                for (auto &target : targets)
                                {
                                    auto end = map.Find(Map::Object::PLAYER, target.ID);

                                    if (!end.IsNone())
                                    {
                                        validtarget = Interface::Move(map, enemies[order[character].ID], movement, src, end);

                                        if (validtarget)
                                        {
                                            scene = RegenerateScene(map);

                                            animating = true;

                                            break;
                                        }
                                    }
                                }

                                if (!validtarget)
                                {
                                    Engine::Next(order, character);
                                }
                            }
                            else
                            {
                                Engine::Next(order, character);
                            }
                        }
                    }
                }
                else
                {
                    Engine::Next(order, character);
                }
            }

            if (skill)
            {
                if (popupid >= 0 && popupid < party.Count())
                {
                    overlay = Interface::Skills(draw, map.SizeX * map.TileSize, map.SizeY * map.TileSize, party[popupid], Color::Background, Color::Inactive, 4, inbattle);
                }
                else
                {
                    skill = false;

                    input = previous;
                }
            }
            else if (spell)
            {
                if (popupid >= 0 && popupid < party.Count())
                {
                    overlay = Interface::Spells(draw, map.SizeX * map.TileSize, map.SizeY * map.TileSize, party[popupid], Color::Background, Color::Inactive, 4, inbattle);
                }
                else
                {
                    spell = false;

                    skill = true;

                    input = previous;
                }
            }

            if (!animating && !skill && !spell && Input::IsValid(scene, input))
            {
                auto &control = scene.Controls[input.Current];

                if (control.OnMap)
                {
                    // draw path to destination
                    if (move && Engine::IsPlayer(order, character))
                    {
                        auto src = map.Find(Map::Object::PLAYER, order[character].ID);

                        auto dst = control.Map;

                        overlay = Interface::Path(map, party[order[character].ID], src, dst);
                    }

                    if (map[control.Map].Occupant == Map::Object::PLAYER)
                    {
                        if (map[control.Map].Id >= 0 && map[control.Map].Id < party.Count())
                        {
                            // stats
                            overlay.Add(Scene::Element(stats[map[control.Map].Id], objectx, objecty, Color::Background, Color::Active, 4));
                        }
                    }
                    else if (map[control.Map].Occupant == Map::Object::ENEMY)
                    {
                        if (map[control.Map].Id >= 0 && map[control.Map].Id < enemies.Count())
                        {
                            // enemy stats
                            overlay.Add(Scene::Element(enemyStats[map[control.Map].Id], objectx, objecty, Color::Background, Color::Active, 4));
                        }
                    }
                }
                else
                {
                    auto moveid = Controls::Find(scene.Controls, Controls::Type::MOVE);

                    auto caption = control.ID - moveid;

                    if (caption >= 0 && caption < captions.size())
                    {
                        auto control = moveid + caption;

                        overlay.Add(Scene::Element(captions[caption], scene.Controls[control].X, scene.Controls[control].Y + scene.Controls[control].H + 8));
                    }
                }
            }
            else if (skill && Input::IsValid(overlay, input))
            {
                // skill popup captions
                auto &control = overlay.Controls[input.Current];

                if (control.Type != Controls::Type::BACK && popupid >= 0 && popupid < party.Count())
                {
                    auto skill = party[popupid].Skills[control.ID];

                    if ((inbattle && Skills::IsBattleSkill(skill)) || (!inbattle && Skills::IsStorySkill(skill)))
                    {
                        overlay.Add(Scene::Element(Interface::SkillCaptionsActive[skill], control.X, control.Y + control.H + pad));
                    }
                    else
                    {
                        overlay.Add(Scene::Element(Interface::SkillCaptionsInactive[skill], control.X, control.Y + control.H + pad));
                    }
                }
            }
            else if (spell && Input::IsValid(overlay, input))
            {
                // spell popup captions
                auto &control = overlay.Controls[input.Current];

                if (control.Type != Controls::Type::BACK && popupid >= 0 && popupid < party.Count())
                {
                    auto &spell = party[popupid].Spells[control.ID];

                    auto &popup = overlay.Elements[0];

                    if (!inbattle)
                    {
                        if (spell.IsBasic() || party[popupid].HasCalledToMind(spell.Type))
                        {
                            overlay.Add(Scene::Element(Interface::SpellCaptionsActive[spell.Type], control.X, control.Y + control.H + pad));
                        }

                        if (spell.IsBasic() || (party[popupid].HasCalledToMind(spell.Type) && !spell.IsBattle))
                        {
                            overlay.Add(Scene::Element(Asset::Get(Asset::Type::CAST_SPELL), popup.X + popup.W - 72, popup.Y + 8));
                            overlay.Add(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CAST_SPELL], popup.X + 16, popup.Y + 8));
                        }
                        else if (!party[popupid].HasCalledToMind(spell.Type))
                        {
                            overlay.Add(Scene::Element(Asset::Get(Asset::Type::CALL_TO_MIND), popup.X + popup.W - 72, popup.Y + 8));
                            overlay.Add(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CALL_TO_MIND], popup.X + 16, popup.Y + 8));
                        }
                    }
                    else
                    {
                        if (party[popupid].HasCalledToMind(spell.Type) && spell.IsBattle && !spell.IsBasic())
                        {
                            overlay.Add(Scene::Element(Asset::Get(Asset::Type::CAST_SPELL), popup.X + popup.W - 72, popup.Y + 8));
                            overlay.Add(Scene::Element(Interface::SpellCaptionsActive[spell.Type], control.X, control.Y + control.H + pad));
                            overlay.Add(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CAST_SPELL], popup.X + 16, popup.Y + 8));
                        }
                        else if (!spell.IsBasic() && spell.IsBattle)
                        {
                            overlay.Add(Scene::Element(Asset::Get(Asset::Type::CALL_TO_MIND), popup.X + popup.W - 72, popup.Y + 8));
                            overlay.Add(Scene::Element(Interface::SpellCaptionsInactive[spell.Type], control.X, control.Y + control.H + pad));
                            overlay.Add(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CALL_TO_MIND], popup.X + 16, popup.Y + 8));
                        }
                        else
                        {
                            overlay.Add(Scene::Element(Interface::SpellCaptionsInactive[spell.Type], control.X, control.Y + control.H + pad));
                        }
                    }
                }
            }

            if (animating)
            {
                animating = !Graphics::Animate(graphics, scene, movement);

                Graphics::Refresh(graphics);

                if (!animating)
                {
                    if (Engine::IsPlayer(order, character))
                    {
                        map.Put(movement.Current, Map::Object::PLAYER, order[character].ID);
                    }
                    else
                    {
                        map.Put(movement.Current, Map::Object::ENEMY, order[character].ID);
                    }

                    Input::Flush();

                    input.Current = -1;

                    input.Selected = false;

                    scene = RegenerateScene(map);

                    Engine::Next(order, character);
                }
            }
            else
            {
                if (!skill && !spell && Engine::IsPlayer(order, character))
                {
                    if (!move)
                    {
                        if (blinking)
                        {
                            // blink cursor
                            Blink(map, order, character, overlay);
                        }

                        blinking = !blinking;

                        input.Blink = false;
                    }
                    else
                    {
                        input.Blink = !input.Blink;
                    }
                }

                input = Input::WaitForInput(graphics, scene, overlay, input, skill || spell);

                if (!skill && !spell)
                {
                    if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
                    {
                        if (input.Type == Controls::Type::MOVE)
                        {
                            // toggles between move/hover mode
                            move = !move;
                        }
                        else if (input.Type == Controls::Type::DESTINATION)
                        {
                            // setup animation
                            if (move)
                            {
                                if (Input::IsValid(scene, input))
                                {
                                    auto &control = scene.Controls[input.Current];

                                    if (control.OnMap && map.IsValid(control.Map) && Engine::IsPlayer(order, character))
                                    {
                                        auto start = map.Find(Map::Object::PLAYER, order[character].ID);
                                        auto end = control.Map;

                                        // find a path to the destination
                                        animating = Interface::Move(map, party[order[character].ID], movement, start, end);

                                        if (animating)
                                        {
                                            scene = RegenerateScene(map);
                                        }
                                    }
                                }

                                move = !move;
                            }
                        }
                        else if (input.Type == Controls::Type::RESET)
                        {
                            // reset starting locations
                            ResetLocations(map, party, origins);

                            scene = RegenerateScene(map);

                            character = 0;
                        }
                        else if (Input::IsPlayer(input) && Input::IsValid(scene, input) && map[scene.Controls[input.Current].Map].Id == order[character].ID)
                        {
                            previous = input;

                            skill = true;

                            if (scene.Controls[input.Current].OnMap)
                            {
                                popupid = map[scene.Controls[input.Current].Map].Id;
                            }

                            input.Current = -1;
                        }
                        else if (input.Type == Controls::Type::EXIT)
                        {
                            done = true;
                        }
                    }
                }
                else if (skill)
                {
                    if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
                    {
                        if (input.Type == Controls::Type::BACK)
                        {
                            skill = false;

                            spell = false;

                            input = previous;
                        }
                        else if (input.Type == Controls::Type::SPELLS && party[popupid].Spells.size() > 0)
                        {
                            skill = false;

                            spell = true;
                        }
                    }
                }
                else if (spell)
                {
                    if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
                    {
                        if (input.Type == Controls::Type::BACK)
                        {
                            spell = false;

                            skill = true;

                            input.Current = -1;
                        }
                    }
                }
            }
        }

        Free(enemyStats);

        Free(stats);

        Free(captions);

        Free(characters);
    }

    void Palette(Graphics::Base &graphics)
    {
        Interface::ReloadTextures(graphics, 0, false);

        std::vector<Graphics::RichText> collection = {};

        Uint32 fixed = 0xFFFFFFFF;

        Uint32 highlight = 0xFFFF0000;

        auto width = 640;

        auto height = 128;

        for (auto name : Palette::Names)
        {
            collection.push_back(Graphics::RichText(name, Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, width));
        }

        std::vector<Graphics::RichText> colors = {
            Graphics::RichText("ACTIVE", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, width),
            Graphics::RichText("INACTIVE", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, width),
            Graphics::RichText("HIGHLIGHT", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, width),
            Graphics::RichText("BACKGROUND", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, width)};

        auto title = Graphics::CreateText(graphics, "Palette Test", Fonts::Fixed, Color::S(fixed), TTF_STYLE_UNDERLINE, width);

        auto menu = Graphics::CreateText(graphics, collection);
        auto labels = Graphics::CreateText(graphics, colors);

        auto start = 0;
        auto limit = 4;
        auto last = start + limit;
        auto options = (int)(menu.size());
        auto xadjust = 60;
        auto origin = 64;
        auto pad = 16;
        auto dim = 64;

        auto input = Controls::User();

        auto done = false;

        auto palette = Palette::Current;

        auto boxx = origin + width + dim + pad / 2;

        while (!done)
        {
            auto scene = Interface::Menu(menu, origin, origin, width, height, start, last, limit, fixed, highlight, true);

            scene.Add(Scene::Element(title, xadjust, 28));

            auto &lastControl = scene.Controls.back();
            auto id = lastControl.ID + 1;
            auto first = Controls::Find(scene.Controls, Controls::Type::CHOICE);
            auto bottomy = scene.Controls[first + limit - 1].Y + height + pad;

            scene.Add(Scene::Element(Asset::Get(Asset::Type::EXIT), xadjust, bottomy));
            scene.Add(Controls::Base(Controls::Type::EXIT, id, id, id, first + limit - 1, id, xadjust, bottomy, dim, dim, highlight));

            if (Input::IsValid(scene, input) && input.Type == Controls::Type::CHOICE)
            {
                auto choices = Controls::Find(scene.Controls, Controls::Type::CHOICE);

                auto color = start + (input.Current - choices);

                if (color >= 0 && color < Palette::List.size())
                {
                    palette = color;
                }
            }

            // add colors
            scene.Add(Scene::Element(boxx + pad, origin, 192, 128, Palette::List[palette][0], Palette::List[palette][0], 2));
            scene.Add(Scene::Element(boxx + pad * 2 + 192, origin, 192, 128, Palette::List[palette][1], Palette::List[palette][1], 2));
            scene.Add(Scene::Element(boxx + pad, origin + 192, 192, 128, Palette::List[palette][2], Palette::List[palette][2], 2));
            scene.Add(Scene::Element(boxx + pad * 2 + 192, origin + 192, 192, 128, Palette::List[palette][3], fixed, 2));
            scene.Add(Scene::Element(labels[0], Point(boxx + pad, origin + 136)));
            scene.Add(Scene::Element(labels[1], Point(boxx + pad * 2 + 192, origin + 136)));
            scene.Add(Scene::Element(labels[2], Point(boxx + pad, origin + 328)));
            scene.Add(Scene::Element(labels[3], Point(boxx + pad * 2 + 192, origin + 328)));
            scene.Add(Scene::Element(menu[palette], boxx + pad, origin + 392));

            if (input.Up)
            {
                input.Current = Controls::Find(scene.Controls, Controls::Type::SCROLL_UP);

                input.Up = false;
            }
            else if (input.Down)
            {
                input.Current = Controls::Find(scene.Controls, Controls::Type::SCROLL_DOWN);

                input.Down = false;
            }

            input = Input::WaitForInput(graphics, scene, input);

            if ((input.Selected && input.Type != Controls::Type::NONE && !input.Hold) || input.Up || input.Down)
            {
                if (input.Type == Controls::Type::EXIT)
                {
                    palette = Palette::Current;

                    done = true;
                }
                else if (input.Type == Controls::Type::CHOICE)
                {
                    auto choices = Controls::Find(scene.Controls, Controls::Type::CHOICE);

                    auto choice = start + (input.Current - choices);

                    palette = choice;

                    done = true;
                }
                else if (input.Type == Controls::Type::SCROLL_UP || input.Up)
                {
                    if (start > 0)
                    {
                        start -= 1;

                        if (start < 0)
                        {
                            start = 0;
                        }

                        last = start + limit;

                        if (last > options)
                        {
                            last = options;
                        }

                        input.Up = true;
                    }
                }
                else if (input.Type == Controls::Type::SCROLL_DOWN || input.Down)
                {
                    if (options - last > 0)
                    {
                        if (start < options - limit)
                        {
                            start += 1;
                        }

                        if (start > options - limit)
                        {
                            start = options - limit;
                        }

                        last = start + limit;

                        if (last > options)
                        {
                            last = options;
                        }

                        input.Down = true;
                    }
                }
            }
        }

        Free(labels);

        Free(menu);

        Free(&title);

        Interface::ReloadTextures(graphics, palette);
    }

    void Menu(Graphics::Base &graphics)
    {
        auto width = 640;

        auto height = 128;

        auto RegenerateTitle = [&]()
        {
            return Graphics::CreateText(graphics, "Blood Sword Test Suite", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_UNDERLINE, width);
        };

        auto RegenerateMenu = [&]()
        {
            auto menu = Graphics::CreateText(
                graphics,
                {Graphics::RichText("00 MENU TEST\n\nDemonstrates the font engine, menu rendering and list box scrolling", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
                 Graphics::RichText("01 CONTROLS TEST\n\nDemonstrates the graphics rendering engine and mouse/keyboard/gamepad controls", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
                 Graphics::RichText("02 MAP TEST\n\nDemonstrates map rendering, object info box display, text scrolling, and context-sensitive controls", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
                 Graphics::RichText("03 ANIMATION TEST\n\nDemonstrates A* path-finding and animation engine and window clipping", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
                 Graphics::RichText("04 BATTLE ORDER\n\nDemonstrates battle order/action/turn sequence and pop-up windows", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
                 Graphics::RichText("05 PALETTES\n\n\nDemonstrates palette and color-switching", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
                 Graphics::RichText("06 SCANLINES\n\n\nToggle horizontal scanlines on/off", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width)});
            return menu;
        };

        auto title = RegenerateTitle();
        auto menu = RegenerateMenu();

        auto FreeTextures = [&]()
        {
            Free(menu);

            Free(&title);
        };

        auto start = 0;
        auto limit = 4;
        auto last = start + limit;
        auto options = (int)(menu.size());
        auto xadjust = 60;
        auto origin = 64;
        auto pad = 16;
        auto dim = 64;
        auto input = Controls::User();

        auto done = false;

        while (!done)
        {
            auto scene = Interface::Menu(menu, origin, origin, width, height, start, last, limit, Color::Inactive, Color::Highlight, true);

            scene.Add(Scene::Element(title, xadjust, 28));

            auto &lastControl = scene.Controls.back();
            auto id = lastControl.ID + 1;
            auto first = Controls::Find(scene.Controls, Controls::Type::CHOICE);
            auto bottomy = scene.Controls[first + limit - 1].Y + height + pad;

            scene.Add(Scene::Element(Asset::Get(Asset::Type::EXIT), xadjust, bottomy));
            scene.Add(Controls::Base(Controls::Type::EXIT, id, id, id, first + limit - 1, id, xadjust, bottomy, dim, dim, Color::Highlight));

            if (input.Up)
            {
                input.Current = Controls::Find(scene.Controls, Controls::Type::SCROLL_UP);

                input.Up = false;
            }
            else if (input.Down)
            {
                input.Current = Controls::Find(scene.Controls, Controls::Type::SCROLL_DOWN);

                input.Down = false;
            }

            input = Input::WaitForInput(graphics, scene, input);

            if ((input.Selected && input.Type != Controls::Type::NONE && !input.Hold) || input.Up || input.Down)
            {
                if (input.Type == Controls::Type::EXIT)
                {
                    done = true;
                }
                else if (input.Type == Controls::Type::CHOICE)
                {
                    auto choices = Controls::Find(scene.Controls, Controls::Type::CHOICE);

                    auto choice = start + (input.Current - choices);

                    switch (choice)
                    {
                    case 1:
                        Test::Controls(graphics);
                        break;
                    case 2:
                        Test::Map(graphics);
                        break;
                    case 3:
                        Test::Animation(graphics);
                        break;
                    case 4:
                        Test::Queue(graphics);
                        break;
                    case 5:
                        // reset textures and menu
                        Test::Palette(graphics);
                        FreeTextures();
                        title = RegenerateTitle();
                        menu = RegenerateMenu();
                        break;
                    case 6:
                        Graphics::ToggleScanLines();
                        break;
                    default:
                        // do nothing - menu test
                        break;
                    }
                }
                else if (input.Type == Controls::Type::SCROLL_UP || input.Up)
                {
                    if (start > 0)
                    {
                        start -= 1;

                        if (start < 0)
                        {
                            start = 0;
                        }

                        last = start + limit;

                        if (last > options)
                        {
                            last = options;
                        }

                        input.Up = true;
                    }
                }
                else if (input.Type == Controls::Type::SCROLL_DOWN || input.Down)
                {
                    if (options - last > 0)
                    {
                        if (start < options - limit)
                        {
                            start += 1;
                        }

                        if (start > options - limit)
                        {
                            start = options - limit;
                        }

                        last = start + limit;

                        if (last > options)
                        {
                            last = options;
                        }

                        input.Down = true;
                    }
                }
            }
        }

        FreeTextures();
    }
}

#endif
