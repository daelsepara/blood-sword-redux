#ifndef __ROGUE_HPP__
#define __ROGUE_HPP__

#include "RogueBase.hpp"
#include "RogueGenerator.hpp"
#include "RogueItems.hpp"
#include "RogueBattle.hpp"

// classes and functions to enable rogue-like game mode
namespace BloodSword::Rogue
{
    bool Blocked(Rogue::Base &rogue, Point point)
    {
        auto &tile = rogue.Battlepits[point];

        auto items = (tile.IsOccupied() && tile.Type == Map::Object::ITEMS);

        auto blockers = (tile.IsOccupied() && tile.Type != Map::Object::ITEMS);

        if (items)
        {
            auto loot = Rogue::FindLoot(rogue, point);

            if (loot >= 0 && loot < rogue.Loot.size())
            {
                items &= (rogue.Loot[loot].Items.size() > 0);
            }
        }

        if (items)
        {
            return true;
        }

        return (blockers || tile.IsBlocked() || !tile.IsPassable());
    }

    // setup movement animation for enemy parties
    bool Move(Rogue::Base &rogue, int enemy, Animation::Base &movement, Point start, Point end)
    {
        auto &map = rogue.Battlepits;

        auto moving = false;

        // find a direct path to the destination
        auto path = Move::FindPath(map, start, end);

        auto closer = false;

        if (path.Points.size() == 0)
        {
            // move closer to target
            path = Move::FindPath(map, start, path.Closest);

            closer = true;
        }

        // add extra move if enemy is trying to close distance
        auto valid = Move::Count(map, path) + (closer ? 1 : 0);

        if (valid > 0)
        {
            map.Put(start, Map::Object::NONE, -1);

            auto first = path.Points.begin();

            // add destination to the count
            auto moves = std::min(valid, 1000);

            if (enemy >= 0 && enemy < rogue.Opponents.size())
            {
                auto character = Engine::First(rogue.Opponents[enemy]);

                if (character >= 0 && character < rogue.Opponents[enemy].Count())
                {
                    // setup animation
                    movement = Interface::Movement(map, Points(first, first + moves), start, rogue.Opponents[enemy][character].Asset);

                    moving = true;
                }
            }
        }

        return moving;
    }

    bool Move(Rogue::Base &rogue, Point point)
    {
        auto moved = !Rogue::Blocked(rogue, point);

        auto &party = rogue.Party;

        if (moved)
        {
            auto from = party.Origin();

            auto &origin = rogue.Battlepits[from];

            auto &destination = rogue.Battlepits[point];

            origin.Occupant = Map::Object::NONE;

            origin.Id = Map::NotFound;

            destination.Occupant = Map::Object::PARTY;

            destination.Id = Map::Party;

            party.Room = destination.Room;

            party.X = point.X;

            party.Y = point.Y;
        }

        return moved;
    }

    bool BattleResults(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, int &enemy)
    {
        auto done = false;

        if (!rogue.IsAlive())
        {
            Interface::MessageBox(graphics, background, "YOUR ADVENTURE HAS COME TO AN END", Color::Highlight);

            done = true;
        }
        else if (!Engine::IsAlive(rogue.Opponents[enemy]))
        {
            Interface::FlashMessage(graphics, background, "YOUR PARTY IS VICTORIUS", Color::Active);

            auto &tile = rogue.Battlepits[rogue.Opponents[enemy].Origin()];

            tile.Occupant = Map::Object::NONE;

            tile.Id = Map::NotFound;

            rogue.Opponents.erase(rogue.Opponents.begin() + enemy);

            enemy = Map::NotFound;
        }

        return done;
    }

    Rogue::Update Handle(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Point point)
    {
        Rogue::Update update = {false, false, false};

        auto &tile = rogue.Battlepits[point];

        if (tile.IsOccupied())
        {
            if (tile.Occupant == Map::Object::ITEMS)
            {
                Rogue::ShowLoot(graphics, background, rogue, point);

                update.Scene = true;

                update.Party = true;
            }
            else if (tile.Occupant == Map::Object::ENEMIES)
            {
                auto enemy = Rogue::FindOpponents(rogue, point);

                if (enemy >= 0 && enemy < rogue.Opponents.size())
                {
                    Rogue::Battle(graphics, background, rogue, enemy);

                    // check results
                    Rogue::BattleResults(graphics, background, rogue, enemy);

                    update.Scene = true;

                    update.Party = true;

                    Input::Clear();
                }
            }
            else
            {
                Interface::FlashMessage(graphics, background, "TILE OCCUPIED", Color::Inactive);

                Sound::Play(Sound::Type::ERROR);
            }
        }
        else
        {
            Sound::Play(Sound::Type::ERROR);
        }

        return update;
    }

    Rogue::Update Actions(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, Point point)
    {
        Rogue::Update result = {false, false};

        if (Battlepits::Empty(rogue.Battlepits, point))
        {
            Sound::Play(Sound::Type::ERROR);
        }
        else if (Rogue::Blocked(rogue, point))
        {
            result = Rogue::Handle(graphics, background, rogue, point);
        }
        else if (Rogue::Move(rogue, point))
        {
            result.Scene = true;
        }

        return result;
    }

    // render battlepits
    void RenderBattlepits(Scene::Base &scene, Rogue::Base &rogue, FieldOfView::Method method, bool sight = true)
    {
        auto &map = rogue.Battlepits;

        auto &party = rogue.Party;

        // set fog color
        auto fog = Color::O(Color::Active, 0x14);

        // get leading character's awareness
        auto first = Engine::First(party);

        // set field of view radius
        auto radius = Engine::IsAlive(party) ? party[first].Value(Attribute::Type::AWARENESS) / 2 : 0;

        // calculate field of view
        auto view = FieldOfView::Compute(map, party.Origin(), radius, method);

        // offset for FoV illumination
        auto fov_offset = BloodSword::Pad;

        // size of FoV illumination
        auto fov_size = BloodSword::TileSize - fov_offset * 2;

        for (auto y = map.Y; y < map.Y + map.ViewY; y++)
        {
            for (auto x = map.X; x < map.X + map.ViewX; x++)
            {
                auto offset = Point(x - map.X, y - map.Y);

                auto &tile = map[Point(x, y)];

                auto screen = Point(map.DrawX, map.DrawY) + offset * map.TileSize;

                auto visible = BloodSword::In(view, x, y);

                auto loot_id = Map::NotFound;

                auto opponent_id = Map::NotFound;

                if (tile.IsOccupied() && (visible || tile.Explored))
                {
                    switch (tile.Occupant)
                    {
                    case Map::Object::PARTY:

                        if (Engine::IsAlive(party) && tile.Id == Map::Party)
                        {
                            auto first = Engine::First(party);

                            auto &player = party[first];

                            if (Engine::IsAlive(player))
                            {
                                scene.VerifyAndAdd(Scene::Element(Asset::Get(player.Asset), screen));
                            }
                        }

                        break;

                    case Map::Object::ENEMIES:

                        opponent_id = Rogue::FindOpponents(rogue, Point(x, y));

                        if (opponent_id >= 0 && opponent_id < rogue.Opponents.size() && rogue.Opponents.size() > 0)
                        {
                            if (Engine::IsAlive(rogue.Opponents[opponent_id]))
                            {
                                auto first = Engine::First(rogue.Opponents[opponent_id]);

                                auto &enemy = rogue.Opponents[opponent_id][first];

                                if (Engine::IsAlive(enemy))
                                {
                                    scene.VerifyAndAdd(Scene::Element(Asset::Get(enemy.Asset), screen));
                                }
                            }
                        }

                        break;

                    case Map::Object::TEMPORARY_OBSTACLE:

                        if (tile.Lifetime > 0 && tile.TemporaryAsset != Asset::Type::NONE)
                        {
                            scene.VerifyAndAdd(Scene::Element(Asset::Get(tile.TemporaryAsset), screen));
                        }
                        else if (tile.Asset != Asset::Type::NONE)
                        {
                            scene.VerifyAndAdd(Scene::Element(Asset::Get(tile.Asset), screen));
                        }

                        break;

                    case Map::Object::ITEMS:

                        loot_id = Rogue::FindLoot(rogue, Point(x, y));

                        if (loot_id >= 0 && loot_id < rogue.Loot.size() && rogue.Loot.size() > 0)
                        {
                            auto &loot = rogue.Loot[loot_id];

                            if (loot.Items.size() > 0)
                            {
                                auto first = Engine::FirstAsset(loot.Items);

                                if (first != Item::NotFound)
                                {
                                    auto &item = loot.Items[first];

                                    scene.VerifyAndAdd(Scene::Element(Asset::Get(item.Asset), screen));
                                }
                                else
                                {
                                    scene.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::ITEMS), screen));
                                }
                            }
                        }

                        break;

                    default:

                        break;
                    }
                }
                else if (!tile.IsOccupied() && tile.Asset != Asset::Type::NONE && (visible || tile.Explored))
                {
                    scene.VerifyAndAdd(Scene::Element(Asset::Get(tile.Asset), screen));
                }
                else if (tile.Asset == Asset::Type::NONE && visible && sight)
                {
                    scene.Add(Scene::Element(screen.X + fov_offset, screen.Y + fov_offset, fov_size, fov_size, Color::O(Color::Highlight, 0x20)));
                }

                if (visible)
                {
                    tile.Explored = true;
                }
                else if (tile.Explored)
                {
                    scene.Add(Scene::Element(screen.X, screen.Y, BloodSword::TileSize, BloodSword::TileSize, Color::Blur));
                }
                else
                {
                    scene.Add(Scene::Element(screen.X, screen.Y, BloodSword::TileSize, BloodSword::TileSize, fog));
                }
            }
        }
    }

    // center battlepits on src (point)
    void Center(Rogue::Base &rogue, Point src)
    {
        rogue.Battlepits.X = src.X - (rogue.Battlepits.ViewX) / 2 + 1;

        rogue.Battlepits.Y = src.Y - (rogue.Battlepits.ViewY) / 2 + 1;

        rogue.Battlepits.CheckBounds();
    }

    // center battlepits on entity (map object type, id)
    void Center(Rogue::Base &rogue, Map::Object entity, int id)
    {
        auto src = rogue.Battlepits.Find(entity, id);

        Rogue::Center(rogue, src);
    }

    Scene::Base UpdateScene(Rogue::Base &rogue, SDL_Texture *image, Point image_location, int panel_w, int panel_h, FieldOfView::Method method, bool animating)
    {
        auto scene = Scene::Base();

        // left panel border
        scene.Add(Scene::Element(BloodSword::TileSize, BloodSword::TileSize, panel_w, panel_h, Color::Background, Color::Active, BloodSword::Border));

        if (image)
        {
            // add left panel
            scene.VerifyAndAdd(Scene::Element(image, image_location));
        }

        // map battlepits panel border
        scene.Add(Scene::Element(BloodSword::TileSize + panel_w + BloodSword::TileSize, BloodSword::TileSize, panel_w, panel_h, Color::Background, Color::Active, BloodSword::Border));

        // center battlepits to party's location
        Rogue::Center(rogue, Map::Object::PARTY, Map::Party);

        // add battlepits to scene
        Rogue::RenderBattlepits(scene, rogue, method, !animating);

        return scene;
    }

    void ShowMap(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue)
    {
        auto &map = rogue.Battlepits;

        auto &party = rogue.Party;

        auto scale = Point(8, 8);

        auto surface = Graphics::CreateSurface(map.Width * scale.X, map.Height * scale.Y);

        if (surface)
        {
            SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 0, 0, 0, 255));

            SDL_Rect rect;

            rect.w = scale.X;

            rect.h = scale.Y;

            for (auto y = 0; y < map.Height; y++)
            {
                for (auto x = 0; x < map.Width; x++)
                {
                    SDL_Surface *surface_asset = nullptr;

                    auto &tile = map[Point(x, y)];

                    rect.x = x * scale.X;

                    rect.y = y * scale.Y;

                    if (party.Origin() == Point(x, y))
                    {
                        surface_asset = Asset::Surface(Asset::Type::WHITE_SPACE, Color::Highlight);
                    }
                    else if (tile.Explored && tile.Asset != Asset::Type::NONE)
                    {
                        surface_asset = Asset::Surface(tile.Asset);
                    }
                    else if (!tile.Explored)
                    {
                        surface_asset = Graphics::CreateSurface(scale.X, scale.Y);

                        SDL_FillRect(surface_asset, nullptr, SDL_MapRGBA(surface_asset->format, Color::R(Color::Inactive), Color::G(Color::Inactive), Color::B(Color::Inactive), 255));
                    }

                    if (surface_asset)
                    {
                        Graphics::RenderAssetScaled(surface, surface_asset, rect);

                        BloodSword::Free(&surface_asset);
                    }
                }
            }

            auto texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

            if (texture)
            {
                auto width = BloodSword::Width(texture) + BloodSword::LargePad;

                auto height = BloodSword::TileSize * 6 + BloodSword::LargePad;

                auto x = (graphics.Width - width) / 2;

                auto y = (graphics.Height - height) / 2;

                // calculate offset to center current location
                auto text_h = height - (BloodSword::TileSize + BloodSword::Pad * 3);

                auto loc = party.Y * scale.Y + scale.Y / 2;

                auto offset = 0;

                if (loc > text_h / 2)
                {
                    offset = (loc - text_h / 2);
                }

                offset = std::min(std::max(0, offset), BloodSword::Height(texture) - text_h);

                Interface::ScrollableImageBox(graphics, background, texture, width, height, x, y, Color::Background, Color::Active, BloodSword::Border, Color::Active, Asset::Type::SWORDTHRUST, Asset::Type::UP, Asset::Type::DOWN, true, offset);

                BloodSword::Free(&texture);
            }

            BloodSword::Free(&surface);
        }
    }

    void PartyInformation(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue, int character = 0)
    {
        auto &party = rogue.Party;

        auto infow = BloodSword::OctaTile;

        auto panelw = BloodSword::OctaTile;

        auto panelh = BloodSword::PanelSize;

        auto names = Interface::GenerateNameCaptions(graphics, party);

        auto stats = Interface::PartyStats(graphics, party, infow);

        auto items = Interface::Items(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, infow);

        auto skills = Interface::Skills(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, infow);

        auto status = Interface::BasicStatus(graphics, party, Fonts::Normal, Color::Active, TTF_STYLE_NORMAL);

        auto backgrounds = Interface::PartyBackgrounds(graphics, party, infow);

        auto spacing = BloodSword::TileSize + BloodSword::Pad;

        auto controlsy = (graphics.Height + panelh) / 2 - spacing - BloodSword::HugePad;

        auto controlsx = (graphics.Width - panelw) / 2 + BloodSword::Pad;

        auto panelx = (graphics.Width - panelw) / 2;

        auto panely = (graphics.Height - panelh) / 2;

        Asset::List assets = {
            Asset::Type::LEFT,
            Asset::Type::RIGHT,
            Asset::Type::CHARACTER,
            Asset::Type::INVENTORY,
            Asset::Type::SKILLS,
            Asset::Type::STATUS,
            Asset::Type::BACK};

        Controls::List controls = {
            Controls::Type::LEFT,
            Controls::Type::RIGHT,
            Controls::Type::ABOUT,
            Controls::Type::ITEMS,
            Controls::Type::SKILLS,
            Controls::Type::STATUS,
            Controls::Type::BACK,
        };

        std::vector<std::string> labels = {
            "PREVIOUS CHARACTER",
            "NEXT CHARACTER",
            "BACKGROUND",
            "INVENTORY",
            "SKILLS",
            "STATUS"};

        if (party.Count() == 1)
        {
            assets.erase(assets.begin(), assets.begin() + 2);

            controls.erase(controls.begin(), controls.begin() + 2);

            labels.erase(labels.begin(), labels.begin() + 2);
        }

        auto captions = Graphics::CreateText(graphics, Graphics::GenerateTextList(labels, Fonts::Caption, Color::Active, 0));

        auto display = Controls::Type::ABOUT;

        auto display_y = panely + BloodSword::TileSize + BloodSword::HalfTile + BloodSword::OddPad * 4;

        auto boxh = controlsy - display_y - BloodSword::LargePad;

        auto input = Controls::User();

        auto done = false;

        while (!done)
        {
            auto overlay = Scene::Base();

            // render panel
            overlay.Add(Scene::Element(panelx, panely, panelw, panelh, Color::Background, Color::Active, BloodSword::Border));

            // render subpanel
            overlay.Add(Scene::Element(panelx, display_y - BloodSword::Pad, panelw, boxh, Color::Background, Color::Active, BloodSword::Border));

            // render names
            overlay.VerifyAndAdd(Scene::Element(names[character], Point(panelx + BloodSword::Pad, panely + BloodSword::Pad)));

            // render stats
            overlay.VerifyAndAdd(Scene::Element(stats[character], Point(panelx + BloodSword::Pad, panely + BloodSword::Pad * 5)));

            // render information
            if (character >= 0 && character < party.Count())
            {
                switch (display)
                {
                case Controls::Type::ABOUT:

                    if (backgrounds[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(backgrounds[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;

                case Controls::Type::ITEMS:

                    if (items[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(items[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;

                case Controls::Type::SKILLS:

                    if (skills[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(skills[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;

                case Controls::Type::STATUS:

                    if (status[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(status[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;

                default:

                    if (items[character])
                    {
                        overlay.VerifyAndAdd(Scene::Element(items[character], Point(panelx + BloodSword::Pad, display_y)));
                    }

                    break;
                }
            }

            // generate panel controls
            auto id = 0;

            for (auto control = 0; control < controls.size(); control++)
            {
                auto lt = id > 0 ? id - 1 : id;

                auto rt = controls[control] != Controls::Type::BACK ? id + 1 : id;

                overlay.VerifyAndAdd(Scene::Element(Asset::Get(assets[control]), Point(controlsx + control * spacing, controlsy)));

                overlay.Add(Controls::Base(controls[control], id, lt, rt, id, id, controlsx + control * spacing, controlsy, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

                if ((controls[control] == Controls::Type::LEFT && character == 0) || (controls[control] == Controls::Type::RIGHT && character == party.Count() - 1))
                {
                    // blur button
                    overlay.Add(Scene::Element(controlsx + control * spacing, controlsy, BloodSword::TileSize, BloodSword::TileSize, Color::Blur));
                }

                id++;
            }

            // render button captions
            if (input.Type != Controls::Type::BACK && Input::IsValid(overlay, input))
            {
                auto &control = overlay.Controls[input.Current];

                // center caption
                auto center = (control.W - BloodSword::Width(captions[input.Current])) / 2;

                if ((control.X + center < (panelx + BloodSword::QuarterTile)) && input.Current == 0)
                {
                    center = 0;
                }

                overlay.VerifyAndAdd(Scene::Element(captions[input.Current], control.X + center, control.Y + control.H + BloodSword::Pad));
            }

            input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

            if ((input.Selected && input.Type != Controls::Type::NONE && !input.Hold) || input.Up || input.Down)
            {
                if (input.Type == Controls::Type::BACK)
                {
                    done = true;
                }
                else if (input.Type == Controls::Type::LEFT)
                {
                    if (character > 0)
                    {
                        character--;
                    }
                    else
                    {
                        Sound::Play(Sound::Type::ERROR);
                    }
                }
                else if (input.Type == Controls::Type::RIGHT)
                {
                    if (character < party.Count() - 1)
                    {
                        character++;
                    }
                    else
                    {
                        Sound::Play(Sound::Type::ERROR);
                    }
                }
                else if (input.Type == Controls::Type::SKILLS)
                {
                    // trigger default character-specific skills
                    if (display == Controls::Type::SKILLS)
                    {
                        if (Engine::IsAlive(party[character]))
                        {
                            if (party[character].Class == Character::Class::SAGE)
                            {
                                Interface::Heal(graphics, background, party, party[character], true);
                            }
                            else if (party[character].Class == Character::Class::ENCHANTER)
                            {
                                Interface::RenderGrimoire(graphics, background, party[character]);
                            }

                            done = true;
                        }
                    }
                    else
                    {
                        display = Controls::Type::SKILLS;
                    }
                }
                else if (input.Type == Controls::Type::ITEMS)
                {
                    if (display == Controls::Type::ITEMS)
                    {
                        Rogue::ManageInventory(graphics, background, rogue, party[character], true);

                        done = true;
                    }
                    else
                    {
                        display = Controls::Type::ITEMS;
                    }
                }
                else if (input.Type == Controls::Type::ABOUT)
                {
                    display = Controls::Type::ABOUT;
                }
                else if (input.Type == Controls::Type::STATUS)
                {
                    display = Controls::Type::STATUS;
                }

                input.Selected = false;
            }
        }

        BloodSword::Free(backgrounds);

        BloodSword::Free(captions);

        BloodSword::Free(status);

        BloodSword::Free(items);

        BloodSword::Free(skills);

        BloodSword::Free(stats);

        BloodSword::Free(names);
    }

    bool SetBattleOrder(Graphics::Base &graphics, Scene::Base &background, Party::Base &party)
    {
        auto update = false;

        if (Engine::IsAlive(party))
        {
            if (party.Count() > 1)
            {
                Interface::BattleOrder(graphics, background, party);

                update = true;
            }
            else
            {
                Interface::MessageBox(graphics, background, "YOU DO NOT HAVE ANY COMPANIONS", Color::Inactive);
            }
        }
        else
        {
            Interface::ErrorMessage(graphics, background, Interface::MSG_OVER);
        }

        return update;
    }

    Rogue::Update Menu(Graphics::Base &graphics, Scene::Base &background, Rogue::Base &rogue)
    {
        Rogue::Update update = {false, false, false};

        auto &party = rogue.Party;

        Asset::List assets = {
            Asset::Type::CHARACTER,
            Asset::Type::MAP,
            Asset::Type::BATTLE_ORDER,
            Asset::Type::LOAD,
            Asset::Type::SAVE,
            Asset::Type::ABOUT,
            Asset::Type::EXIT,
            Asset::Type::BACK};

        Controls::List controls = {
            Controls::Type::PARTY,
            Controls::Type::MAP,
            Controls::Type::BATTLE_ORDER,
            Controls::Type::LOAD,
            Controls::Type::SAVE,
            Controls::Type::ABOUT,
            Controls::Type::EXIT,
            Controls::Type::BACK};

        std::vector<std::string> captions = {
            "PARTY",
            "MAP",
            "BATTLE ORDER",
            "LOAD GAME",
            "SAVE GAME",
            "HELP",
            "QUIT",
            "BACK"};

        auto values = std::vector<int>(controls.size());

        std::iota(values.begin(), values.end(), 0);

        auto message = "BloodSword: Rogue";

        auto done = false;

        while (!done)
        {
            auto selection = Interface::SelectIcons(graphics, background, message, assets, values, captions, 1, 1, Asset::Type::NONE, false, true);

            if (selection.size() == 1)
            {
                auto input = controls[selection[0]];

                if (input == Controls::Type::BACK)
                {
                    done = true;
                }
                else if (input == Controls::Type::PARTY)
                {
                    Rogue::PartyInformation(graphics, background, rogue, 0);

                    update.Party = true;
                }
                else if (input == Controls::Type::MAP)
                {
                    Rogue::ShowMap(graphics, background, rogue);
                }
                else if (input == Controls::Type::BATTLE_ORDER)
                {
                    done = Rogue::SetBattleOrder(graphics, background, party);

                    update.Party = true;
                }
                else if (input == Controls::Type::EXIT)
                {
                    if (Interface::Confirm(graphics, background, "ARE YOU SURE?", Color::Background, Color::Active, BloodSword::Border, Color::Active, true))
                    {
                        done = true;

                        update.Quit = true;
                    }
                }
                else
                {
                    Interface::NotImplemented(graphics, background);
                }
            }
        }

        return update;
    }

    // main game loop
    void Main(Graphics::Base &graphics, Rogue::Base &rogue)
    {
        // set FOV algorithm
        auto method = FieldOfView::Map(Engine::ToUpper(Interface::Settings["fov"]));

        // determine panel dimensions and locations
        auto panel_w = (graphics.Width - BloodSword::TileSize * 3) / 2;

        auto panel_h = (graphics.Height - BloodSword::TileSize * 2);

        rogue.Battlepits.ViewX = panel_w / rogue.Battlepits.TileSize;

        rogue.Battlepits.ViewY = panel_h / rogue.Battlepits.TileSize;

        auto offset_x = (panel_w - rogue.Battlepits.ViewX * rogue.Battlepits.TileSize) / 2;

        auto offset_y = (panel_h - rogue.Battlepits.ViewY * rogue.Battlepits.TileSize) / 2;

        rogue.Battlepits.DrawX = (BloodSword::TileSize * 2 + panel_w + offset_x);

        rogue.Battlepits.DrawY = (BloodSword::TileSize + offset_y);

        // generate initial party stats image
        auto image = Interface::GeneratePartyStats(graphics, rogue.Party, panel_w - BloodSword::LargePad);

        auto image_location = Point(BloodSword::TileSize + BloodSword::Pad, BloodSword::TileSize + BloodSword::Pad);

        if (image)
        {
            image_location = Point(BloodSword::TileSize + BloodSword::Pad, BloodSword::TileSize + BloodSword::Pad);
        }

        // start with a blank scene
        Rogue::Update update = {true, false, false};

        auto scene = Scene::Base();

        // animation
        auto movement = Animation::Base();

        auto animating = false;

        auto done = false;

        auto events = true;

        auto enemy = Map::NotFound;

        while (!done)
        {
            if (update.Party)
            {
                BloodSword::Free(&image);

                image = Interface::GeneratePartyStats(graphics, rogue.Party, panel_w - BloodSword::LargePad);

                update.Scene = true;

                update.Party = false;
            }

            if (update.Scene || animating)
            {
                scene = Rogue::UpdateScene(rogue, image, image_location, panel_w, panel_h, method, animating);

                update.Scene = false;
            }

            if (animating)
            {
                animating = !Graphics::Animate(graphics, scene, movement, BloodSword::FrameDelay);

                if (!animating)
                {
                    // place enemy in final destination
                    rogue.Opponents[enemy].X = movement.Current.X;

                    rogue.Opponents[enemy].Y = movement.Current.Y;

                    rogue.Battlepits.Put(movement.Current, Map::Object::ENEMIES, -1);

                    // trigger event if distance closed with party
                    if (rogue.Battlepits.Distance(movement.Current, rogue.Origin()) <= 1)
                    {
                        events = true;
                    }

                    // reset animation
                    movement = Animation::Base();

                    // reset enemy
                    enemy = Map::NotFound;

                    // update battlepits
                    update.Scene = true;

                    // skip input and events processing
                    Input::Clear();

                    continue;
                }
            }

            // enemy movement, ranged and magic attacks
            if (rogue.IsAlive() && events && !animating && rogue.InsideRoom())
            {
                enemy = Rogue::FindOpponents(rogue, rogue.Room());

                if (enemy >= 0 && enemy < rogue.Opponents.size())
                {
                    auto distance = rogue.Battlepits.Distance(rogue.Origin(), rogue.Opponents[enemy].Origin());

                    if (distance > 1)
                    {
                        // move or shoot at party
                        animating = Rogue::Move(rogue, enemy, movement, rogue.Opponents[enemy].Origin(), rogue.Origin());
                    }
                    else
                    {
                        // update scene
                        Rogue::UpdateScene(rogue, image, image_location, panel_w, panel_h, method, true);

                        // flash a message
                        Interface::FlashMessage(graphics, scene, "PARTY ATTACKED!", Color::Background, Color::Highlight, BloodSword::Border, BloodSword::OneSecond);

                        // commence battle
                        Rogue::Battle(graphics, scene, rogue, enemy);

                        // check results
                        done = Rogue::BattleResults(graphics, scene, rogue, enemy);

                        update.Scene = true;

                        update.Party = true;

                        Input::Clear();
                    }
                }

                events = false;
            }

            if (!animating)
            {
                auto input = Input::RogueInput(graphics, {scene});

                auto prev = rogue.Origin();

                if (input.Selected && input.Type != Controls::Type::NONE && !input.Hold)
                {
                    auto point = rogue.Origin();

                    if (input.Type == Controls::Type::MENU)
                    {
                        update = Rogue::Menu(graphics, scene, rogue);

                        done = update.Quit;
                    }
                    else if (input.Type == Controls::Type::UP)
                    {
                        if (point.Y > 0)
                        {
                            point.Y--;

                            update = Rogue::Actions(graphics, scene, rogue, point);
                        }
                    }
                    else if (input.Type == Controls::Type::DOWN)
                    {
                        if (point.Y < rogue.Battlepits.Height - 1)
                        {
                            point.Y++;

                            update = Rogue::Actions(graphics, scene, rogue, point);
                        }
                    }
                    else if (input.Type == Controls::Type::LEFT)
                    {
                        if (point.X > 0)
                        {
                            point.X--;

                            update = Rogue::Actions(graphics, scene, rogue, point);
                        }
                    }
                    else if (input.Type == Controls::Type::RIGHT)
                    {
                        if (point.X < rogue.Battlepits.Width - 1)
                        {
                            point.X++;

                            update = Rogue::Actions(graphics, scene, rogue, point);
                        }
                    }
                    else if (input.Type == Controls::Type::MAP)
                    {
                        Rogue::ShowMap(graphics, scene, rogue);
                    }
                    else if (input.Type == Controls::Type::PARTY)
                    {
                        auto first = std::max(0, Engine::First(rogue.Party));

                        Rogue::PartyInformation(graphics, scene, rogue, first);

                        update.Party = true;
                    }
                    else if (input.Type == Controls::Type::BATTLE_ORDER)
                    {
                        update.Party = Rogue::SetBattleOrder(graphics, scene, rogue.Party);
                    }
                    else if (input.Type == Controls::Type::EXIT)
                    {
                        done = Interface::Confirm(graphics, scene, "ARE YOU SURE?", Color::Background, Color::Active, BloodSword::Border, Color::Active, true);
                    }

                    // trigger event on movement
                    if (prev != rogue.Origin())
                    {
                        events = true;
                    }

                    input.Selected = false;
                }
            }
        }

        BloodSword::Free(&image);
    }

    void Game(Graphics::Base &graphics)
    {
        // set default control to the first
        Controls::Default = 0;

        auto rogue = Rogue::GenerateBattlepits(50, 100, 100, 5, 7, false);

        // create party
        rogue.Party = Interface::CreateParty(graphics, {8, 4, 3, 2}, false);

        if (rogue.Rooms.size() > 0 && rogue.Count() > 0)
        {
            // 50% rooms has monsters
            Rogue::PlaceMonsters(rogue, rogue.Rooms.size() / 2);

            // place boss
            Rogue::PlaceBoss(rogue);

            // place loot
            Rogue::PlaceLoot(rogue, rogue.Rooms.size() / 4, 10, 50);

            // place party at the center of the starting room
            auto center = rogue.Rooms[0].Center();

            rogue.Party.X = center.X;

            rogue.Party.Y = center.Y;

            // run main game loop
            Rogue::Main(graphics, rogue);
        }
    }
}

#endif
