#ifndef __TEST_HPP__
#define __TEST_HPP__

#include "Graphics.hpp"
#include "Messages.hpp"
#include "Interface.hpp"
#include "InterfaceBattle.hpp"
#include "InterfaceStory.hpp"

// framework for testing game subsystems
namespace BloodSword::Test
{
    // user input test
    void Controls(Graphics::Base &graphics)
    {
        auto x = BloodSword::TileSize * 2;

        auto y = BloodSword::TileSize;

        auto w = BloodSword::TileSize;

        auto h = BloodSword::TileSize;

        auto s = BloodSword::QuarterTile;

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
        Controls::List controls = {
            Controls::Base(Controls::Type::WARRIOR, 0, 0, 1, 0, 4, x, y, w, h, Color::Highlight),
            Controls::Base(Controls::Type::TRICKSTER, 1, 0, 2, 1, 5, x + w + s, y, w, h, Color::Highlight),
            Controls::Base(Controls::Type::SAGE, 2, 1, 3, 2, 6, x + ws * 2, y, w, h, Color::Highlight),
            Controls::Base(Controls::Type::ENCHANTER, 3, 2, 3, 3, 7, x + ws * 3, y, w, h, Color::Highlight),
            Controls::Base(Controls::Type::MAP_UP, 4, 4, 5, 0, 4, x, y + ws, w, h, Color::Highlight),
            Controls::Base(Controls::Type::MAP_DOWN, 5, 4, 6, 1, 5, x + w + s, y + ws, w, h, Color::Highlight),
            Controls::Base(Controls::Type::MAP_LEFT, 6, 5, 7, 2, 6, x + ws * 2, y + ws, w, h, Color::Highlight),
            Controls::Base(Controls::Type::MAP_RIGHT, 7, 6, 7, 3, 7, x + ws * 3, y + ws, w, h, Color::Highlight)};

        auto sprites = Scene::Elements();

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

        Free(&enchanter_text);

        Free(&sage_text);

        Free(&trickster_text);

        Free(&warrior_text);

        Free(&text);
    }

    // map rendering test
    void Map(Graphics::Base &graphics)
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

        auto pad = BloodSword::OddPad;

        auto text_w = (map.TileSize * 8) + 3 * pad;

        auto object_w = (map.TileSize * 5);

        auto textures = Graphics::CreateText(
            graphics,
            {Graphics::RichText("PLAYER", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, object_w),
             Graphics::RichText("ENEMY", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, object_w),
             Graphics::RichText("PASSABLE", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, object_w),
             Graphics::RichText("ENEMY PASSABLE", Fonts::Normal, Color::S(Color::Inactive), TTF_STYLE_NORMAL, object_w),
             Graphics::RichText("OBSTACLE", Fonts::Normal, Color::S(Color::Background), TTF_STYLE_NORMAL, object_w),
             Graphics::RichText("TEMPORARY OBSTACLE", Fonts::Normal, Color::S(Color::Highlight), TTF_STYLE_NORMAL, object_w),
             Graphics::RichText("EXIT", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, object_w),
             Graphics::RichText("WARRIOR", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, object_w),
             Graphics::RichText("TRICKSTER", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, object_w),
             Graphics::RichText("SAGE", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, object_w),
             Graphics::RichText("ENCHANTER", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, object_w)});

        auto backgrounds = Graphics::CreateText(
            graphics,
            {Graphics::RichText("THE BATTLEPITS OF KRARTH\n\nEvery thirteen lunar months the Magi of Krarth hold a desperate contest to see which of them will rule that bleak and icy land. Teams of daring adventurers are sent down into the labyrinths that lie beneath the tundra, each searching for the Emblem of Victory that will win power for their patron.\n\nOnly one team can prevail. The others must die.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, text_w),
             Graphics::RichText("WARRIOR\n\nYou are a master of the fighting arts. You have better Fighting Prowess than any other character type, and when you strike a blow, you inflict more damage. You also have chainmail armour which provides an Armour rating of 3, which is better than the armour available to other characters.\n\nThese advantages give you a real edge in any fight, but you do not get things all your own way. You have none of the other characters' special skills -- the Sage's ESP, for instance, or the Trickster's low devious cunning. Also, because you follow the honourable traditions of your class, you must be careful to stay true to the code of chivalry.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, text_w),
             Graphics::RichText("TRICKSTER\n\nSome adventurers are honourable and prefer to face their foes in a straight fight. You live by your wits. If you can win by trickery or by shooting someone in the back, you will. You know how to wield a sword if you have to, but your main weapon is cunning.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, text_w),
             Graphics::RichText("SAGE\n\nYour upbringing has been in the spartan Monastery of Illumination on the barren island of Kaxos. There, you have studied the Mystic Way, a series of demanding spiritual disciplines combined with rigorous physical training.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, text_w),
             Graphics::RichText("ENCHANTER\n\nForget the mundane arts of swordplay. You know that true power lies in the manipulation of occult powers of sorcery.", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_NORMAL, text_w)});

        auto stats = Interface::GenerateStats(graphics, party, object_w, false, true);

        auto background = 0;

        auto prev_background = -1;

        auto change = false;

        auto offset = 0;

        auto object_x = map.DrawX + (map.ViewX * 2 + 1) * map.TileSize / 2 + pad;

        auto object_y = map.DrawY + map.TileSize * 7 - pad;

        auto background_x = object_x;

        auto background_y = map.DrawY + pad;

        auto background_w = text_w;

        auto background_h = map.TileSize * 5;

        auto texture_w = 0;

        auto texture_h = 0;

        auto scroll_x = object_x + background_w + pad * 2;

        auto scroll_y = map.DrawY + background_h - map.TileSize + pad * 2;

        auto scroll_speed = BloodSword::ScrollSpeed;

        auto scroll_up = false;

        auto scroll_down = false;

        BloodSword::Size(backgrounds[background], &texture_w, &texture_h);

        while (true)
        {
            auto scene = Interface::Map(map, party, enemies, 1);

            auto id = int(scene.Controls.size());

            auto x = map.DrawX;

            auto y = map.DrawY + map.ViewY * map.TileSize;

            auto arrow_up = offset > 0;

            auto arrow_down = background_h < texture_h && offset < (texture_h - background_h);

            scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::EXIT), x, y));

            if (arrow_up)
            {
                scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::UP), scroll_x, map.DrawY));
            }

            if (arrow_down)
            {
                scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::DOWN), scroll_x, scroll_y));
            }

            if (arrow_up || arrow_down)
            {
                scene.Add(Controls::Base(Controls::Type::EXIT, id, id, id + 1, id - map.ViewX, id, x, y, map.TileSize, map.TileSize, Color::Active));
            }

            if (arrow_up && arrow_down)
            {
                scene.Add(Controls::Base(Controls::Type::SCROLL_UP, id + 1, id, id + 1, id + 1, id + 2, scroll_x, map.DrawY, map.TileSize, map.TileSize, Color::Inactive));

                scene.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id + 2, id, id + 2, id + 1, id + 2, scroll_x, scroll_y, map.TileSize, map.TileSize, Color::Inactive));
            }
            else if (arrow_up)
            {
                scene.Add(Controls::Base(Controls::Type::SCROLL_UP, id + 1, id, id + 1, id + 1, id + 1, scroll_x, map.DrawY, map.TileSize, map.TileSize, Color::Inactive));
            }
            else if (arrow_down)
            {
                scene.Add(Controls::Base(Controls::Type::SCROLL_DOWN, id + 1, id, id + 1, id + 1, id + 1, scroll_x, scroll_y, map.TileSize, map.TileSize, Color::Inactive));
            }
            else
            {
                scene.Add(Controls::Base(Controls::Type::EXIT, id, id, id, id - map.ViewX, id, x, y, map.TileSize, map.TileSize, Color::Active));
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
                        scene.VerifyAndAdd(Scene::Element(stats[tile.Id], object_x + object_w + pad * 3, object_y, Color::Background, Color::Active, 4));

                        // character class
                        object = int(party[tile.Id].Class) + 7;
                    }
                    else if (tile.IsPassable())
                    {
                        object = int(tile.Type);

                        background = Color::Background;

                        border = Color::Active;
                    }
                    else if (tile.IsPassableToEnemy())
                    {
                        object = int(tile.Type);

                        background = Color::Background;

                        border = Color::Highlight;
                    }
                    else if (tile.IsOccupied())
                    {
                        object = int(tile.Occupant);

                        background = Color::Active;

                        border = Color::Inactive;
                    }
                    else
                    {
                        object = int(tile.Type);

                        background = Color::Highlight;

                        border = Color::Inactive;
                    }

                    if (object >= 0 && object < textures.size())
                    {
                        scene.Add(Scene::Element(object_x, object_y, object_w, map.TileSize, background, border, 4));

                        scene.VerifyAndAdd(Scene::Element(textures[object], object_x, object_y));
                    }
                }
            }

            if (background >= 0 && background < backgrounds.size())
            {
                scene.Add(Scene::Element(background_x, background_y, background_w, background_h, Color::Highlight, Color::Active, 4));

                scene.VerifyAndAdd(Scene::Element(backgrounds[background], background_x, background_y, background_h, offset));
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
                    if (background_h < texture_h)
                    {
                        offset -= scroll_speed;

                        if (offset < 0)
                        {
                            offset = 0;
                        }

                        scroll_up = true;
                    }
                }
                else if (input.Type == Controls::Type::SCROLL_DOWN || input.Down)
                {
                    if (background_h < texture_h)
                    {
                        offset += scroll_speed;

                        if (offset > (texture_h - background_h))
                        {
                            offset = texture_h - background_h;
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
                        BloodSword::Size(backgrounds[background], &texture_w, &texture_h);

                        offset = 0;

                        change = false;
                    }
                }
            }
        }

        Free(stats);

        Free(backgrounds);

        Free(textures);
    }

    // animation test
    void Animation(Graphics::Base &graphics)
    {
        auto player_class = Character::Class::TRICKSTER;

        auto party = Party::Base(Generate::Character(player_class, 8));

        auto enemies = Party::Base();

        auto map = Map::Base();

        auto maze_w = (graphics.Width / map.TileSize) - 2;

        auto maze_h = (graphics.Height / map.TileSize) - 3;

        if ((maze_w & 1) == 0)
        {
            maze_w--;
        }

        if ((maze_h & 1) == 0)
        {
            maze_h--;
        }

        Maze::Generate(map, maze_w, maze_h);

        map.Viewable(maze_w, maze_h);

        auto random = Random::Base();

        auto start = Point(1, map.Height - 1);

        auto exit = (random.NextInt() % (maze_h / 2)) * 2 + 1;

        auto end = Point(map.Width - 1, exit);

        auto draw = Point(map.DrawX, map.DrawY);

        auto pad = BloodSword::Pad;

        auto RegenerateScene = [&](Map::Base &map)
        {
            auto scene = Interface::Map(map, party, enemies, 3);

            auto id = int(scene.Controls.size());

            scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), map.DrawX, map.DrawY + map.ViewY * map.TileSize + pad));

            scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::MOVE), map.DrawX + map.TileSize, map.DrawY + map.ViewY * map.TileSize + pad));

            scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::EXIT), map.DrawX + map.TileSize * 2, map.DrawY + map.ViewY * map.TileSize + pad));

            scene.Add(Controls::Base(Controls::Type::BACK, id, id, id + 1, id - map.ViewX, id, map.DrawX, map.DrawY + map.ViewY * map.TileSize + pad, map.TileSize, map.TileSize, Color::Active));

            scene.Add(Controls::Base(Controls::Type::MOVE, id + 1, id, id + 2, id - map.ViewX + 1, id + 1, map.DrawX + map.TileSize, map.DrawY + map.ViewY * map.TileSize + pad, map.TileSize, map.TileSize, Color::Active));

            scene.Add(Controls::Base(Controls::Type::EXIT, id + 2, id + 1, id + 2, id - map.ViewX + 2, id + 2, map.DrawX + map.TileSize * 2, map.DrawY + map.ViewY * map.TileSize + pad, map.TileSize, map.TileSize, Color::Active));

            return scene;
        };

        auto ResetObjects = [&](Map::Base &map)
        {
            exit = (random.NextInt() % (maze_h / 2)) * 2 + 1;

            end = Point(map.Width - 1, exit);

            map.Put(start, Map::Object::PASSABLE, Asset::Type::NONE);

            map.Put(start, Map::Object::PLAYER, 0);

            map.Put(end, Map::Object::PASSABLE, Asset::Type::RIGHT);
        };

        // user inputs
        auto input = Controls::User();

        // animations
        auto animations = Animations::Base();

        // movement animation
        auto movement = Interface::Movement(map, party[player_class], {}, start);

        ResetObjects(map);

        auto scene = RegenerateScene(map);

        auto done = true;

        Uint64 frame_start = 0;

        Uint64 frame_end = 0;

        Uint64 frames = 0;

        auto fps = 0.0;

        SDL_Texture *fps_texture = nullptr;

        while (true)
        {
            if (done)
            {
                if (fps_texture)
                {
                    auto overlay = Scene::Base(fps_texture, map.DrawX, map.TileSize / 2);

                    input = Input::WaitForInput(graphics, scene, overlay, input);
                }
                else
                {
                    input = Input::WaitForInput(graphics, scene, input);
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

                            scene = RegenerateScene(map);

                            Interface::Clip(graphics, map);

                            // add destination to the count
                            auto first = path.Points.begin();

                            auto moves = std::min(valid + 1, party[player_class].Moves);

                            movement = Interface::Movement(map, party[player_class], Points(first, first + moves), start);

                            // add to list
                            animations.Clear();

                            animations.Add(movement);

                            animations.Delay = BloodSword::FrameDelay;

                            done = false;

                            fps = 0.0;

                            frames = 0;

                            frame_start = SDL_GetTicks64();
                        }
                    }
                    else if (input.Type == Controls::Type::BACK && done)
                    {
                        Maze::Generate(map, maze_w, maze_h);

                        map.Viewable(maze_w, maze_h);

                        ResetObjects(map);

                        scene = RegenerateScene(map);
                    }
                }
            }
            else
            {
                while (!done)
                {
                    done = Graphics::Animate(graphics, scene, animations, true);

                    frames++;
                }

                if (done)
                {
                    // compute FPS
                    frame_end = SDL_GetTicks64();

                    auto msec = frame_end - frame_start;

                    if (msec > 0)
                    {
                        Free(&fps_texture);

                        fps = (double)(frames * BloodSword::OneSecond) / (double)msec;

                        std::string fpsString = "FPS: " + std::to_string(fps) + "/sec";

                        fps_texture = Graphics::CreateText(graphics, fpsString.c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);
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

                    scene = RegenerateScene(map);

                    Interface::Clip(graphics);
                }
            }
        }

        Free(&fps_texture);
    }

    // battle order
    void Queue(Graphics::Base &graphics)
    {
        auto map = Map::Base();

        map.Load("maps/queue.json");

        auto party = Party::Load("party/rank02.json", "party");

        auto enemies = Party::Load("party/assassin-barbarian-corpse.json", "opponents");

        auto stats = Interface::GenerateStats(graphics, party, map.TileSize * 5);

        auto enemy_stats = Interface::GenerateStats(graphics, enemies, map.TileSize * 5);

        auto draw = Point(map.DrawX, map.DrawY);

        auto caption_w = BloodSword::TileSize * 5;

        // initialize captions
        auto captions = Graphics::CreateText(
            graphics,
            {Graphics::RichText("Move character", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, caption_w),
             Graphics::RichText("Reset starting locations", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, caption_w),
             Graphics::RichText("Back to test suite", Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, caption_w)});

        // regenerate map
        auto RegenerateScene = [&](Map::Base &map)
        {
            auto scene = Interface::Map(map, party, enemies, 3);

            auto id = scene.Controls.size();

            scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::MOVE), map.DrawX, map.DrawY + map.ViewY * map.TileSize));

            scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), map.DrawX + map.TileSize, map.DrawY + map.ViewY * map.TileSize));

            scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::EXIT), map.DrawX + map.TileSize * 2, map.DrawY + map.ViewY * map.TileSize));

            scene.Add(Controls::Base(Controls::Type::MOVE, id, id, id + 1, id - map.ViewX, id, map.DrawX, map.DrawY + map.ViewY * map.TileSize, map.TileSize, map.TileSize, Color::Active));

            scene.Add(Controls::Base(Controls::Type::RESET, id + 1, id, id + 2, id - map.ViewX + 1, id + 1, map.DrawX + map.TileSize, map.DrawY + map.ViewY * map.TileSize, map.TileSize, map.TileSize, Color::Active));

            scene.Add(Controls::Base(Controls::Type::EXIT, id + 2, id + 1, id + 2, id - map.ViewX + 2, id + 2, map.DrawX + map.TileSize * 2, map.DrawY + map.ViewY * map.TileSize, map.TileSize, map.TileSize, Color::Active));

            return scene;
        };

        // reset stating locations
        auto ResetLocations = [&](Map::Base &map, Party::Base &party)
        {
            map.ResetLocations(party, Map::Object::PLAYER, map.Origins);

            map.ResetLocations(enemies, Map::Object::ENEMY, map.Spawn);
        };

        // Generate character info boxes
        auto characters = Interface::GenerateCharacterClassCaptions(graphics, caption_w);

        // set starting locations
        ResetLocations(map, party);

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
            movement = Interface::Movement(map, party[order[character].Id], {}, map.Origins[order[character].Id]);
        }
        else
        {
            movement = Interface::Movement(map, enemies[order[character].Id], {}, map.Spawn[order[character].Id]);
        }

        auto pad = BloodSword::OddPad;

        auto object_x = map.DrawX + (map.ViewX * 2 + 1) * map.TileSize / 2 + pad;

        auto object_y = map.DrawY + pad;

        auto skill = false;

        auto spell = false;

        auto popup_id = 0;

        auto in_battle = true;

        if (Engine::IsPlayer(order, character))
        {
            input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[order[character].Id].Class]);
        }

        while (!done)
        {
            auto overlay = Scene::Base();

            // check if this character can move
            if (!animating)
            {
                auto src = map.Find(Engine::IsPlayer(order, character) ? Map::Object::PLAYER : Map::Object::ENEMY, order[character].Id);

                if (!src.IsNone())
                {
                    if (!Move::Available(map, src))
                    {
                        Engine::NextInQueue(order, character);

                        if (Engine::IsPlayer(order, character))
                        {
                            input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[order[character].Id].Class]);
                        }
                    }
                    else
                    {
                        // if enemy, move to nearest target
                        if (!Engine::IsPlayer(order, character))
                        {
                            auto targets = Engine::MoveTargets(map, party, src, false, true);

                            if (targets.size() > 0)
                            {
                                auto valid_target = false;

                                for (auto &target : targets)
                                {
                                    auto end = map.Find(Map::Object::PLAYER, target.Id);

                                    if (!end.IsNone())
                                    {
                                        valid_target = Interface::Move(map, enemies[order[character].Id], movement, src, end);

                                        if (valid_target)
                                        {
                                            scene = RegenerateScene(map);

                                            animating = true;

                                            break;
                                        }
                                    }
                                }

                                if (!valid_target)
                                {
                                    Engine::NextInQueue(order, character);

                                    if (Engine::IsPlayer(order, character))
                                    {
                                        input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[order[character].Id].Class]);
                                    }
                                }
                            }
                            else
                            {
                                Engine::NextInQueue(order, character);

                                if (Engine::IsPlayer(order, character))
                                {
                                    input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[order[character].Id].Class]);
                                }
                            }
                        }
                    }
                }
                else
                {
                    Engine::NextInQueue(order, character);

                    if (Engine::IsPlayer(order, character))
                    {
                        input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[order[character].Id].Class]);
                    }
                }
            }

            if (skill)
            {
                if (popup_id >= 0 && popup_id < party.Count())
                {
                    overlay = Interface::Skills(draw, map.ViewX * map.TileSize, map.ViewY * map.TileSize, party[popup_id], Color::Background, Color::Active, 4, in_battle);
                }
                else
                {
                    skill = false;

                    input = previous;
                }
            }
            else if (spell)
            {
                if (popup_id >= 0 && popup_id < party.Count())
                {
                    overlay = Interface::Spells(draw, map.ViewX * map.TileSize, map.ViewY * map.TileSize, party[popup_id], Color::Background, Color::Active, 4, in_battle);
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
                        auto src = map.Find(Map::Object::PLAYER, order[character].Id);

                        auto dst = control.Map;

                        overlay = Interface::Path(map, party[order[character].Id], src, dst);
                    }

                    if (map[control.Map].Occupant == Map::Object::PLAYER)
                    {
                        if (map[control.Map].Id >= 0 && map[control.Map].Id < party.Count())
                        {
                            // stats
                            overlay.VerifyAndAdd(Scene::Element(stats[map[control.Map].Id], object_x, object_y, Color::Background, Color::Active, 4));
                        }
                    }
                    else if (map[control.Map].Occupant == Map::Object::ENEMY)
                    {
                        if (map[control.Map].Id >= 0 && map[control.Map].Id < enemies.Count())
                        {
                            // enemy stats
                            overlay.VerifyAndAdd(Scene::Element(enemy_stats[map[control.Map].Id], object_x, object_y, Color::Background, Color::Active, 4));
                        }
                    }
                }
                else
                {
                    auto moveid = Controls::Find(scene.Controls, Controls::Type::MOVE);

                    auto caption = control.Id - moveid;

                    if (caption >= 0 && caption < captions.size())
                    {
                        auto control = moveid + caption;

                        overlay.VerifyAndAdd(Scene::Element(captions[caption], scene.Controls[control].X, scene.Controls[control].Y + scene.Controls[control].H + BloodSword::Pad));
                    }
                }
            }
            else if (skill && Input::IsValid(overlay, input))
            {
                // skill popup captions
                auto &control = overlay.Controls[input.Current];

                if (control.Type != Controls::Type::BACK && popup_id >= 0 && popup_id < party.Count())
                {
                    auto skill = party[popup_id].Skills[control.Id];

                    if ((in_battle && Skills::IsBattleSkill(skill)) || (!in_battle && Skills::IsStorySkill(skill)))
                    {
                        overlay.VerifyAndAdd(Scene::Element(Interface::SkillCaptionsActive[skill], control.X, control.Y + control.H + pad));
                    }
                    else
                    {
                        overlay.VerifyAndAdd(Scene::Element(Interface::SkillCaptionsInactive[skill], control.X, control.Y + control.H + pad));
                    }
                }
            }
            else if (spell && Input::IsValid(overlay, input))
            {
                // spell popup captions
                auto &control = overlay.Controls[input.Current];

                if (control.Type != Controls::Type::BACK && popup_id >= 0 && popup_id < party.Count())
                {
                    auto &spell = party[popup_id].Spells[control.Id];

                    auto &popup = overlay.Elements[0];

                    auto popup_x1 = popup.X + popup.W - (BloodSword::TileSize + BloodSword::HalfTile);

                    auto popup_x2 = popup.X + BloodSword::QuarterTile;

                    auto popup_y = popup.Y + BloodSword::Pad;

                    if (!in_battle)
                    {
                        if (spell.IsBasic() || party[popup_id].HasCalledToMind(spell.Type))
                        {
                            overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsActive[spell.Type], control.X, control.Y + control.H + pad));
                        }

                        if (spell.IsBasic() || (party[popup_id].HasCalledToMind(spell.Type) && !spell.IsBattle))
                        {
                            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CAST_SPELL), popup_x1, popup_y));

                            overlay.VerifyAndAdd(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CAST_SPELL], popup_x2, popup_y));
                        }
                        else if (!party[popup_id].HasCalledToMind(spell.Type))
                        {
                            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CALL_TO_MIND), popup_x1, popup_y));

                            overlay.VerifyAndAdd(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CALL_TO_MIND], popup_x2, popup_y));
                        }
                    }
                    else
                    {
                        if (party[popup_id].HasCalledToMind(spell.Type) && spell.IsBattle && !spell.IsBasic())
                        {
                            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CAST_SPELL), popup_x1, popup_y));

                            overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsActive[spell.Type], control.X, control.Y + control.H + pad));

                            overlay.VerifyAndAdd(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CAST_SPELL], popup_x2, popup_y));
                        }
                        else if (!spell.IsBasic() && spell.IsBattle)
                        {
                            overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::CALL_TO_MIND), popup_x1, popup_y));

                            overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsInactive[spell.Type], control.X, control.Y + control.H + pad));

                            overlay.VerifyAndAdd(Scene::Element(Interface::SkillCaptionsActive[Skills::Type::CALL_TO_MIND], popup_x2, popup_y));
                        }
                        else
                        {
                            overlay.VerifyAndAdd(Scene::Element(Interface::SpellCaptionsInactive[spell.Type], control.X, control.Y + control.H + pad));
                        }
                    }
                }
            }

            if (animating)
            {
                animating = !Graphics::Animate(graphics, scene, movement, BloodSword::FrameDelay);

                if (!animating)
                {
                    if (Engine::IsPlayer(order, character))
                    {
                        map.Put(movement.Current, Map::Object::PLAYER, order[character].Id);
                    }
                    else
                    {
                        map.Put(movement.Current, Map::Object::ENEMY, order[character].Id);
                    }

                    Input::Flush();

                    input.Current = -1;

                    input.Selected = false;

                    scene = RegenerateScene(map);

                    Engine::NextInQueue(order, character);

                    if (Engine::IsPlayer(order, character))
                    {
                        input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[order[character].Id].Class]);
                    }
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
                            // focus cursor
                            Interface::Focus(map, order, character, overlay);
                        }

                        blinking = !blinking;

                        input.Blink = false;
                    }
                    else
                    {
                        input.Blink = !input.Blink;
                    }
                }

                input = Input::WaitForInput(graphics, scene, overlay, input, (skill || spell), (skill || spell));

                if (!skill && !spell)
                {
                    if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
                    {
                        if (input.Type == Controls::Type::MOVE)
                        {
                            // toggles between move/hover mode
                            move = !move;
                        }
                        else if (input.Type == Controls::Type::LOCATION)
                        {
                            // setup animation
                            if (move)
                            {
                                if (Input::IsValid(scene, input))
                                {
                                    auto &control = scene.Controls[input.Current];

                                    if (control.OnMap && map.IsValid(control.Map) && Engine::IsPlayer(order, character))
                                    {
                                        auto start = map.Find(Map::Object::PLAYER, order[character].Id);

                                        auto end = control.Map;

                                        // find a path to the destination
                                        animating = Interface::Move(map, party[order[character].Id], movement, start, end);

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
                            ResetLocations(map, party);

                            scene = RegenerateScene(map);

                            character = 0;
                        }
                        else if (Input::IsPlayer(input) && Input::IsValid(scene, input) && map[scene.Controls[input.Current].Map].Id == order[character].Id)
                        {
                            previous = input;

                            skill = true;

                            if (scene.Controls[input.Current].OnMap)
                            {
                                popup_id = map[scene.Controls[input.Current].Map].Id;
                            }

                            if (popup_id >= 0 && popup_id < party.Count() && party[popup_id].Skills.size() == 1 && party[popup_id].Skills[0] == Skills::Type::SPELLS)
                            {
                                skill = false;

                                spell = true;
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
                        else if (input.Type == Controls::Type::SPELLS && party[popup_id].Spells.size() > 0)
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
                        else if (Engine::IsSpell(input.Type))
                        {
                            auto ptr = Interface::ControlSpellMapping.find(input.Type);

                            if (ptr != Interface::ControlSpellMapping.end())
                            {
                                auto &type = ptr->second;

                                auto &caster = party[popup_id];

                                auto search = caster.Find(type);

                                if (search != caster.Spells.end())
                                {
                                    auto &spellbook = *search;

                                    auto battle_cast = in_battle && !spellbook.IsBasic() && spellbook.IsBattle;

                                    auto story_cast = !in_battle && !spellbook.IsBasic() && !spellbook.IsBattle;

                                    auto basic_cast = !in_battle && spellbook.IsBasic();

                                    if (battle_cast || story_cast || basic_cast)
                                    {
                                        if (caster.HasCalledToMind(spellbook.Type) || spellbook.IsBasic())
                                        {
                                            // cast
                                            spell = false;

                                            skill = false;

                                            if (!spellbook.IsBasic())
                                            {
                                                caster.Forget(spellbook.Type);

                                                Free(characters);

                                                stats = Interface::GenerateStats(graphics, party, map.TileSize * 5);
                                            }

                                            Engine::NextInQueue(order, character);

                                            if (Engine::IsPlayer(order, character))
                                            {
                                                input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[order[character].Id].Class]);
                                            }
                                        }
                                        else if (!spellbook.IsBasic())
                                        {
                                            // call to mind
                                            spell = false;

                                            skill = false;

                                            caster.CallToMind(spellbook.Type);

                                            stats = Interface::GenerateStats(graphics, party, map.TileSize * 5);

                                            Engine::NextInQueue(order, character);

                                            if (Engine::IsPlayer(order, character))
                                            {
                                                input.Current = Controls::Find(scene.Controls, Interface::CharacterControls[party[order[character].Id].Class]);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        Free(enemy_stats);

        Free(stats);

        Free(captions);

        Free(characters);
    }

    void Palette(Graphics::Base &graphics)
    {
        Interface::ReloadTextures(graphics, 0, false);

        Graphics::TextList collection = {};

        Uint32 fixed = Color::Active;

        Uint32 highlight = Color::Highlight;

        auto width = BloodSword::TileSize * 10;

        auto height = BloodSword::TileSize * 2;

        for (auto name : Palette::Names)
        {
            collection.push_back(Graphics::RichText(name, Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, width));
        }

        Graphics::TextList colors = {
            Graphics::RichText("ACTIVE", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, width),
            Graphics::RichText("INACTIVE", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, width),
            Graphics::RichText("HIGHLIGHT", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, width),
            Graphics::RichText("BACKGROUND", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, width)};

        auto title = Graphics::CreateText(graphics, "Palette Test", Fonts::Fixed, Color::S(fixed), TTF_STYLE_UNDERLINE, width);

        auto menu = Graphics::CreateText(graphics, collection);

        auto labels = Graphics::CreateText(graphics, colors);

        auto start = 0;

        auto limit = std::min(4, int(menu.size()));

        auto last = start + limit;

        auto options = int(menu.size());

        auto x_adjust = BloodSword::TileSize - BloodSword::SmallPad;

        auto origin_x = BloodSword::TileSize;

        auto origin_y1 = BloodSword::TileSize;

        auto pad = BloodSword::QuarterTile;

        auto dim = BloodSword::TileSize;

        auto input = Controls::User();

        auto done = false;

        auto palette = Palette::Current;

        auto palette_offset_x = origin_x + width + dim + pad / 2;

        while (!done)
        {
            auto scene = Interface::Menu(menu, origin_x, origin_y1, width, height, start, last, limit, 0, fixed, highlight, true);

            scene.VerifyAndAdd(Scene::Element(title, x_adjust, (BloodSword::HalfTile - BloodSword::SmallPad)));

            auto &last_control = scene.Controls.back();

            auto id = last_control.Id + 1;

            auto first = Controls::Find(scene.Controls, Controls::Type::CHOICE);

            auto bottom_y = scene.Controls[first + limit - 1].Y + height + pad;

            scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::EXIT), x_adjust, bottom_y));

            scene.Add(Controls::Base(Controls::Type::EXIT, id, id, id, first + limit - 1, id, x_adjust, bottom_y, dim, dim, highlight));

            if (Input::IsValid(scene, input) && input.Type == Controls::Type::CHOICE)
            {
                auto choices = Controls::Find(scene.Controls, Controls::Type::CHOICE);

                auto color = start + (input.Current - choices);

                if (color >= 0 && color < Palette::List.size())
                {
                    palette = color;
                }
            }

            // add color boxes
            auto palette_w = BloodSword::TileSize * 3;

            auto palette_h = BloodSword::TileSize * 2;

            auto palette_x1 = palette_offset_x + pad;

            auto palette_x2 = palette_offset_x + palette_w + pad * 2;

            auto origin_y2 = origin_x + palette_h + BloodSword::TileSize;

            auto palette_y1 = origin_x + palette_h + BloodSword::Pad;

            auto palette_y2 = origin_y1 + BloodSword::TileSize * 5 + BloodSword::Pad;

            scene.Add(Scene::Element(palette_x1, origin_x, palette_w, palette_h, Palette::List[palette][0], Palette::List[palette][0], 2));

            scene.Add(Scene::Element(palette_x2, origin_x, palette_w, palette_h, Palette::List[palette][1], Palette::List[palette][1], 2));

            scene.Add(Scene::Element(palette_x1, origin_y2, palette_w, palette_h, Palette::List[palette][2], Palette::List[palette][2], 2));

            scene.Add(Scene::Element(palette_x2, origin_y2, palette_w, palette_h, Palette::List[palette][3], fixed, 2));

            scene.VerifyAndAdd(Scene::Element(labels[0], Point(palette_x1, palette_y1)));

            scene.VerifyAndAdd(Scene::Element(labels[1], Point(palette_x2, palette_y1)));

            scene.VerifyAndAdd(Scene::Element(labels[2], Point(palette_x1, palette_y2)));

            scene.VerifyAndAdd(Scene::Element(labels[3], Point(palette_x2, palette_y2)));

            scene.VerifyAndAdd(Scene::Element(menu[palette], palette_x1, origin_y1 + palette_h * 3 + BloodSword::Pad));

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

    void Attribute(Graphics::Base &graphics)
    {
        auto random = Random::Base();

        random.UniformIntDistribution(0, 2);

        auto difficulty = Random::Base();

        difficulty.UniformIntDistribution(-2, 2);

        // load party
        auto party = Party::Load("party/rank02.json", "party");

        auto done = false;

        Attribute::Types attributes = {
            Attribute::Type::FIGHTING_PROWESS,
            Attribute::Type::AWARENESS,
            Attribute::Type::PSYCHIC_ABILITY};

        auto popup_w = BloodSword::TileSize * 8 + BloodSword::HalfTile;

        auto popup_h = BloodSword::QuarterTile * 18 - BloodSword::Pad;

        while (!done)
        {
            auto scene = Scene::Base();

            auto character_class = Interface::SelectCharacter(graphics, scene, party, "CHOOSE A PLAYER", false, true);

            if (character_class != Character::Class::NONE)
            {
                auto &character = party[character_class];

                auto origin = (Point(graphics.Width, graphics.Height) - Point(popup_w, popup_h)) / 2;

                auto attribute = attributes[random.NextInt()];

                auto fight = attribute == Attribute::Type::FIGHTING_PROWESS;

                Interface::Test(graphics, scene, origin, popup_w, popup_h, Color::Active, 4, character, attribute, 2, difficulty.NextInt(), fight ? Asset::Type::FIGHT : Asset::Type::NONE, false);
            }

            done = (character_class == Character::Class::NONE);
        }
    }

    // fight and damage resolution
    void Fight(Graphics::Base &graphics)
    {
        auto player = Generate::Character(Character::Class::WARRIOR, 2);

        auto enemy = Generate::NPC("BARBARIAN", {}, 8, 5, 7, 12, 1, 1, 2, BloodSword::MaximumMoves, Asset::Type::BARBARIAN);

        auto alive = true;

        auto round = 0;

        auto fixed = Color::Active;

        Graphics::TextList collection = {
            Graphics::RichText("PLAYER TURN", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, 0),
            Graphics::RichText("ENEMY TURN", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, 0),
            Graphics::RichText("PLAYER RETALIATES", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, 0),
            Graphics::RichText("ENEMY RETALIATES", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, 0)};

        auto events = Graphics::CreateText(graphics, collection);

        auto scene = Scene::Base();

        while (alive)
        {
            scene = Scene::Base();

            std::string round_text = "ROUND: " + std::to_string(round + 1);

            auto texture = Graphics::CreateText(graphics, ("ROUND: " + std::to_string(round + 1)).c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

            scene.VerifyAndAdd(Scene::Element(texture, Point(0, 0)));

            if (Engine::Score(player, Attribute::Type::AWARENESS, true) >= Engine::Score(enemy, Attribute::Type::AWARENESS, true))
            {
                // player turn
                scene.VerifyAndAdd(Scene::Element(events[0], Point(0, scene.Elements[0].H)));

                alive &= Interface::ResolveFight(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, player, enemy, player.Fight);

                // enemy retaliates
                if (alive)
                {
                    scene.Elements[1] = Scene::Element(events[3], Point(0, scene.Elements[0].H));

                    alive &= Interface::ResolveFight(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, enemy, player, enemy.Fight);
                }

                // enemy turn
                if (alive)
                {
                    scene.Elements[1] = Scene::Element(events[1], Point(0, scene.Elements[0].H));

                    alive &= Interface::ResolveFight(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, enemy, player, enemy.Fight);
                }

                // player retaliates
                if (alive)
                {
                    scene.Elements[1] = Scene::Element(events[2], Point(0, scene.Elements[0].H));

                    alive &= Interface::ResolveFight(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, player, enemy, player.Fight);
                }
            }
            else
            {
                // enemy turn
                scene.VerifyAndAdd(Scene::Element(events[1], Point(0, scene.Elements[0].H)));

                alive &= Interface::ResolveFight(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, enemy, player, enemy.Fight);

                // player retaliates
                if (alive)
                {
                    scene.Elements[1] = Scene::Element(events[2], Point(0, scene.Elements[0].H));

                    alive &= Interface::ResolveFight(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, player, enemy, player.Fight);
                }

                // player turn
                if (alive)
                {
                    scene.Elements[1] = Scene::Element(events[0], Point(0, scene.Elements[0].H));

                    alive &= Interface::ResolveFight(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, player, enemy, player.Fight);
                }

                // enemy retaliates
                if (alive)
                {
                    scene.Elements[1] = Scene::Element(events[3], Point(0, scene.Elements[0].H));

                    alive &= Interface::ResolveFight(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, enemy, player, enemy.Fight);
                }
            }

            round++;

            Free(&texture);
        }

        scene.Clear();

        if (Engine::IsAlive(player))
        {
            Interface::MessageBox(graphics, scene, Graphics::RichText(player.Name + " WINS!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Transparent, Color::Active, 4, Color::Highlight, true);
        }
        else
        {
            Interface::MessageBox(graphics, scene, Graphics::RichText(enemy.Name + " WINS!", Fonts::Normal, Color::Highlight, TTF_STYLE_NORMAL, 0), Color::Transparent, Color::Highlight, 4, Color::Active, true);
        }

        Free(events);
    }

    // shoot demo
    void Shoot(Graphics::Base &graphics)
    {
        auto player = Generate::Character(Character::Class::TRICKSTER, 2);

        auto enemy = Generate::NPC("ASSASSIN", Skills::Type::NONE, Skills::Type::SHURIKEN, {Skills::Type::SHURIKEN}, 7, 6, 7, 5, 0, 1, 0, 0, Asset::Type::ASSASSIN);

        auto alive = true;

        auto round = 0;

        auto fixed = Color::Active;

        Graphics::TextList collection = {
            Graphics::RichText("PLAYER TURN", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, 0),
            Graphics::RichText("ENEMY TURN", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, 0),
            Graphics::RichText("QUIVER EMPTY!", Fonts::Normal, Color::S(fixed), TTF_STYLE_NORMAL, 0)};

        auto events = Graphics::CreateText(graphics, collection);

        auto scene = Scene::Base();

        auto empty = false;

        while (alive && !empty)
        {
            scene = Scene::Base();

            std::string round_text = "ROUND: " + std::to_string(round + 1);

            auto texture = Graphics::CreateText(graphics, ("ROUND: " + std::to_string(round + 1)).c_str(), Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL);

            scene.VerifyAndAdd(Scene::Element(texture, Point(0, 0)));

            if (Engine::Score(player, Attribute::Type::AWARENESS, true) >= Engine::Score(enemy, Attribute::Type::AWARENESS, true))
            {
                if (player.IsArmed(Item::Type::BOW, Item::Type::QUIVER, Item::Type::ARROW))
                {
                    // player turn
                    scene.VerifyAndAdd(Scene::Element(events[0], Point(0, scene.Elements[0].H)));

                    alive &= Interface::Shoot(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, player, enemy, player.Shoot, Asset::Type::ARCHERY);

                    player.Remove(Item::Type::ARROW, 1);
                }
                else
                {
                    // quiver empty
                    scene.VerifyAndAdd(Scene::Element(events[2], Point(0, scene.Elements[0].H)));

                    empty = true;
                }

                // enemy turn
                if (alive)
                {
                    scene.Elements[1] = Scene::Element(events[1], Point(0, scene.Elements[0].H));

                    alive &= Interface::Shoot(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, enemy, player, enemy.Shoot, Asset::Type::SHURIKEN);
                }
            }
            else
            {
                // enemy turn
                scene.VerifyAndAdd(Scene::Element(events[1], Point(0, scene.Elements[0].H)));

                alive &= Interface::Shoot(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, enemy, player, enemy.Shoot, Asset::Type::SHURIKEN);

                // player turn
                if (alive)
                {
                    if (player.IsArmed(Item::Type::BOW, Item::Type::QUIVER, Item::Type::ARROW))
                    {
                        scene.Elements[1] = Scene::Element(events[0], Point(0, scene.Elements[0].H));

                        alive &= Interface::Shoot(graphics, scene, Point(0, 0), graphics.Width, graphics.Height, player, enemy, player.Shoot, Asset::Type::ARCHERY);

                        player.Remove(Item::Type::ARROW, 1);
                    }
                    else
                    {
                        // quiver empty
                        scene.Elements[1] = Scene::Element(events[2], Point(0, scene.Elements[0].H));

                        empty = true;
                    }
                }
            }

            round++;

            Free(&texture);
        }

        scene.Clear();

        if (empty)
        {
            Interface::MessageBox(graphics, scene, Graphics::RichText("YOU RAN OUT OF ARROWS!", Fonts::Normal, Color::Highlight, TTF_STYLE_NORMAL, 0), Color::Transparent, Color::Highlight, 4, Color::Active, true);
        }
        else if (Engine::IsAlive(player))
        {
            Interface::MessageBox(graphics, scene, Graphics::RichText(player.Name + " WINS!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Transparent, Color::Active, 4, Color::Highlight, true);
        }
        else
        {
            Interface::MessageBox(graphics, scene, Graphics::RichText(enemy.Name + " WINS!", Fonts::Normal, Color::Highlight, TTF_STYLE_NORMAL, 0), Color::Transparent, Color::Highlight, 4, Color::Active, true);
        }

        Free(events);
    }

    // battle
    void Battle(Graphics::Base &graphics)
    {
        // Load party from configuration file
        auto party = Party::Load("party/rank02.json", "party");

        // Load battle from configuration file
        auto battle = Battle::Load("battles/test-complete.json");

        auto result = Interface::RenderBattle(graphics, battle, party);

        auto scene = Scene::Base();

        Graphics::RichText message;

        auto border = Color::Active;

        switch (result)
        {
        case Battle::Result::VICTORY:

            message = Graphics::RichText("YOUR PARTY IS VICTORIOUS!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0);

            break;

        case Battle::Result::DEFEAT:

            message = party.Count() > 1 ? Interface::Text[Interface::MSG_DEAD] : Interface::Text[Interface::MSG_DIED];

            border = Color::Highlight;

            break;

        case Battle::Result::ENTHRALLED:

            message = Graphics::RichText("ENEMIES HAVE BEEN SUBDUED!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0);

            break;

        case Battle::Result::FLEE:

            message = Graphics::RichText("YOUR PARTY FLEES!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0);

            border = Color::Inactive;

            break;

        default:

            message = Graphics::RichText("THE OUTCOME WAS INCONCLUSIVE!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0);

            border = Color::Inactive;

            break;
        }

        Interface::MessageBox(graphics, scene, message, Color::Background, border, 4, Color::Highlight, true);
    }

    void Story(Graphics::Base &graphics)
    {
        auto background = Scene::Base();

        auto party = Interface::CreateParty(graphics, Book::Number::BOOK1, false);

        auto story = Story::Load("books/book1.json");

        if (story.Sections.size() > 0 && Book::IsDefined(story.Sections.front().Location))
        {
            // render section
            Interface::ProcessStory(graphics, background, story, party);
        }
        else
        {
            Interface::MessageBox(graphics, background, Graphics::RichText("STORY NOT LOADED!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Highlight, BloodSword::Border, Color::Active, true);
        }
    }

    void TextInput(Graphics::Base &graphics, Scene::Base &background)
    {
        auto input_string = Interface::TextInput(graphics, background, "INPUT TEXT", true);

        input_string = BloodSword::LeftPad(input_string, 4);

        Interface::MessageBox(graphics, background, Graphics::RichText(input_string.c_str(), Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Background, Color::Active, BloodSword::Border, Color::Highlight, true);
    }

    void Heal(Graphics::Base &graphics, Scene::Base &background)
    {
        // load party from file
        auto party = Party::Load("party/rank08.json", "party");

        for (auto i = 0; i < party.Count(); i++)
        {
            // add sample damage
            Engine::GainEndurance(party[i], -8, false);
        }

        auto &character = party[Character::Class::SAGE];

        Interface::Heal(graphics, background, party, character, true);
    }

    BloodSword::Textures RegenerateMenu(Graphics::Base &graphics, int width)
    {
        auto menu = Graphics::CreateText(
            graphics,
            {Graphics::RichText("00 MENU TEST\n\nDemonstrates the font engine, menu rendering and list box scrolling", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
             Graphics::RichText("01 CONTROLS TEST\n\nDemonstrates the graphics rendering engine and mouse/keyboard/gamepad controls", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
             Graphics::RichText("02 MAP TEST\n\nDemonstrates map rendering, object info box display, text scrolling, and context-sensitive controls", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
             Graphics::RichText("03 ANIMATION TEST\n\nDemonstrates A* path-finding and animation engine and window clipping", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
             Graphics::RichText("04 BATTLE ORDER\n\nDemonstrates battle order/action/turn sequence and pop-up windows", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
             Graphics::RichText("05 PALETTES\n\n\nDemonstrates palette and color-switching", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
             Graphics::RichText("06 SCANLINES\n\n\nToggle horizontal scanlines on/off", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
             Graphics::RichText("07 ATTRIBUTES TEST\n\n\nAttribute difficulty checks", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
             Graphics::RichText("08 FIGHT\n\n\nFighting and damage resolution", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
             Graphics::RichText("09 SHOOT\n\n\nShooting and damage resolution", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
             Graphics::RichText("10 BATTLE\n\n\nBattles on a map (moving/fighting/shooting/spells)", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
             Graphics::RichText("11 STORY\n\n\nStory sections", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
             Graphics::RichText("12 TEXT INPUT\n\n\nInput text inside a message box", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width),
             Graphics::RichText("13 HEALING\n\n\nHealing ability", Fonts::Normal, Color::S(Color::Active), TTF_STYLE_NORMAL, width)});

        return menu;
    }

    SDL_Texture *RegenerateTitle(Graphics::Base &graphics, int width)
    {
        return Graphics::CreateText(graphics, "Blood Sword Test Suite", Fonts::Fixed, Color::S(Color::Active), TTF_STYLE_UNDERLINE, width);
    }

    void Menu(Graphics::Base &graphics)
    {
        auto width = BloodSword::TileSize * 10;

        auto height = BloodSword::TileSize * 2;

        auto menu = RegenerateMenu(graphics, width);

        auto title = RegenerateTitle(graphics, width);

        auto start = 0;

        auto limit = 4;

        auto last = start + limit;

        auto options = int(menu.size());

        auto x_adjust = BloodSword::TileSize - BloodSword::SmallPad;

        auto origin = BloodSword::TileSize;

        auto pad = BloodSword::QuarterTile;

        auto dim = BloodSword::TileSize;

        auto input = Controls::User();

        auto done = false;

        while (!done)
        {
            auto scene = Interface::Menu(menu, origin, origin, width, height, start, last, limit, Color::Transparent, Color::Inactive, Color::Highlight, true);

            scene.VerifyAndAdd(Scene::Element(title, x_adjust, (BloodSword::HalfTile - BloodSword::SmallPad)));

            auto &lastControl = scene.Controls.back();

            auto id = lastControl.Id + 1;

            auto first = Controls::Find(scene.Controls, Controls::Type::CHOICE);

            auto bottom_y = scene.Controls[first + limit - 1].Y + height + pad;

            scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::EXIT), x_adjust, bottom_y));

            scene.Add(Controls::Base(Controls::Type::EXIT, id, id, id, first + limit - 1, id, x_adjust, bottom_y, dim, dim, Color::Highlight));

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
                        Free(menu);
                        Free(&title);
                        title = RegenerateTitle(graphics, width);
                        menu = RegenerateMenu(graphics, width);
                        break;
                    case 6:
                        Graphics::ToggleScanLines();
                        break;
                    case 7:
                        Test::Attribute(graphics);
                        break;
                    case 8:
                        Test::Fight(graphics);
                        break;
                    case 9:
                        Test::Shoot(graphics);
                        break;
                    case 10:
                        Test::Battle(graphics);
                        break;
                    case 11:
                        Test::Story(graphics);
                        break;
                    case 12:
                        Test::TextInput(graphics, scene);
                        break;
                    case 13:
                        Test::Heal(graphics, scene);
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

        Free(&title);

        Free(menu);
    }
}

#endif
