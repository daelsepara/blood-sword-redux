#ifndef __INTERFACE_BATTLE_ITEMS_HPP__
#define __INTERFACE_BATTLE_ITEMS_HPP__

#include "InterfaceBattleScene.hpp"

namespace BloodSword::Interface
{
    void ShowBattleDescription(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Base &character, int id)
    {
        auto &item = character.Items[id];

        if (item.BattleDescriptions.size() > 0)
        {
            for (auto description = 0; description < item.BattleDescriptions.size(); description++)
            {
                if (Book::IsDefined(item.BattleDescriptions[description]) && Book::Equal(item.BattleDescriptions[description], party.Location))
                {
                    Interface::ShowBookDescription(graphics, background, item.BattleDescriptions[description]);

                    break;
                }
            }
        }
    }

    bool TargetAction(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Party::Base &party, Character::Base &character, int id, Point target)
    {
        auto used = false;

        auto target_player = battle.Map.IsValid(target) && battle.Map[target].IsPlayer() && Engine::CanTarget(party[battle.Map[target].Id], true);

        auto target_enemy = battle.Map.IsValid(target) && battle.Map[target].IsEnemy() && Engine::CanTarget(battle.Opponents[battle.Map[target].Id], true);

        auto valid = (target_enemy || target_player);

        auto &item = character.Items[id];

        if (!target.IsNone() && valid)
        {
            auto target_id = battle.Map[target].Id;

            auto &defender = character.IsPlayer() ? battle.Opponents[target_id] : party[target_id];

            auto target_type = character.Items[id].HasEffect(defender.Target) ? defender.Target : (character.Items[id].HasEffect(Target::Type::ENEMY) ? Target::Type::ENEMY : Target::Type::NONE);

            // check if it is a blasting / weapon
            if (target_type != Target::Type::NONE && item.TargetEffects[target_type] == Item::TargetEffect::DAMAGE_TARGET)
            {
                if ((character.IsPlayer() && target_enemy) || (character.IsEnemy() && target_player))
                {
                    auto charged = item.Contains == Item::Type::CHARGE;

                    auto has_charges = item.IsCharged(Item::Type::CHARGE, 1);

                    auto usable = (charged && has_charges) || !charged;

                    if (usable)
                    {
                        auto rolls = item.DamageTypes[target_type].Value;

                        auto modifier = item.DamageTypes[target_type].Modifier;

                        auto ignore_armour = item.DamageTypes[target_type].IgnoreArmour;

                        Interface::DamagePlayer(graphics, background, defender, rolls, modifier, ignore_armour, true, true);

                        if (!Engine::IsAlive(defender))
                        {
                            battle.Map.Remove(defender.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, target_id);

                            Interface::MessageBox(graphics, background, defender.Name + " KILLED!", defender.IsPlayer() ? Color::Highlight : Color::Active);

                            if (charged && has_charges)
                            {
                                item.Remove(Item::Type::CHARGE, 1);
                            }
                            else if (item.Drops)
                            {
                                battle.Loot.push_back(item);

                                Interface::ConsumeItem(character, id);
                            }
                        }

                        used = true;
                    }
                    else if (!has_charges)
                    {
                        Interface::MessageBox(graphics, background, "NO CHARGES LEFT!", Color::Highlight);
                    }
                }
                else
                {
                    Interface::MessageBox(graphics, background, "INVALID TARGET", Color::Highlight);
                }
            }
            else if (target_type != Target::Type::NONE && item.TargetEffects[target_type] == Item::TargetEffect::THROW)
            {
                if ((character.IsPlayer() && target_enemy) || (character.IsEnemy() && target_player))
                {
                    if (Interface::Test(graphics, background, character, Attribute::Type::FIGHTING_PROWESS))
                    {
                        Interface::MessageBox(graphics, background, item.Name + " HITS " + defender.Name, character.IsPlayer() ? Color::Active : Color::Highlight);

                        auto rolls = item.DamageTypes[target_type].Value;

                        auto modifier = item.DamageTypes[target_type].Modifier;

                        auto ignore_armour = item.DamageTypes[target_type].IgnoreArmour;

                        Interface::DamagePlayer(graphics, background, defender, rolls, modifier, ignore_armour, true, true);

                        if (!Engine::IsAlive(defender))
                        {
                            battle.Map.Remove(defender.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, target_id);

                            Interface::MessageBox(graphics, background, defender.Name + " KILLED!", defender.IsPlayer() ? Color::Highlight : Color::Active);
                        }
                    }
                    else
                    {
                        Interface::MessageBox(graphics, background, character.Name + " MISSED!", character.IsPlayer() ? Color::Highlight : Color::Active);
                    }

                    if (item.Drops)
                    {
                        battle.Loot.push_back(item);

                        Interface::ConsumeItem(character, id);
                    }

                    used = true;
                }
                else
                {
                    Interface::MessageBox(graphics, background, "INVALID TARGET", Color::Highlight);
                }
            }
        }

        return used;
    }

    // (character) manage item while in battle
    bool ManageItem(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Party::Base &party, Character::Base &character, Point src, int id)
    {
        auto update = false;

        auto assets = Asset::List();

        auto controls = Controls::Collection();

        auto captions = std::vector<std::string>();

        // take item
        assets.push_back(Asset::Type::USE);

        controls.push_back(Controls::Type::USE);

        captions.push_back("USE");

        // go back
        assets.push_back(Asset::Type::BACK);

        controls.push_back(Controls::Type::BACK);

        auto values = std::vector<int>();

        for (auto i = 0; i < controls.size(); i++)
        {
            values.push_back(i);
        }

        auto done = false;

        auto &item = character.Items[id];

        while (!done)
        {
            auto selection = Interface::SelectIcons(graphics, background, character.Items[id].Name.c_str(), assets, values, captions, 1, 1, Asset::Type::NONE, false);

            if (selection.size() == 1)
            {
                auto input = controls[selection[0]];

                if (input == Controls::Type::BACK)
                {
                    done = true;
                }
                else if (input == Controls::Type::USE)
                {
                    if (item.Is(Item::Property::WEAPON) || item.Is(Item::Property::ARMOUR))
                    {
                        if (item.Is(Item::Property::EQUIPPED))
                        {
                            Interface::MessageBox(graphics, background, "EQUIPMENT IS ALREADY IN USE", Color::Active);
                        }
                        else
                        {
                            Interface::MessageBox(graphics, background, "YOU CANNOT CHANGE EQUIPMENT WHILE IN BATTLE!", Color::Highlight);
                        }
                    }
                    else if (!item.Has(Item::Property::COMBAT))
                    {
                        Interface::MessageBox(graphics, background, "YOU CANNOT USE THIS IN BATTLE!", Color::Highlight);
                    }
                    else if (!item.Has(Item::Property::REQUIRES_TARGET))
                    {
                        Interface::ItemEffects(graphics, background, character, item.Type);
                    }
                    else
                    {
                        auto src = battle.Map.Find(Map::Object::PLAYER, Engine::Find(party, character));

                        auto opponents = Engine::FightTargets(battle.Map, battle.Opponents, src, true, false);

                        auto ranged = item.Has(Item::Property::RANGED);

                        if (ranged)
                        {
                            auto targets = Engine::RangedTargets(battle.Map, battle.Opponents, src, true, false);

                            if (opponents.size() > 0)
                            {
                                Interface::MessageBox(graphics, background, Interface::GetText(Interface::MSG_NEARBY), Color::Highlight);
                            }
                            else if (targets.size() > 0)
                            {
                                auto asset = Asset::Type::ARCHERY;

                                if (item.Type == Item::Type::DAGGER_OF_VISLET)
                                {
                                    asset = Asset::Type::POISONED_DAGGER;
                                }

                                auto target = Point(-1, -1);

                                // do something
                                if (targets.size() > 1)
                                {
                                    target = Interface::SelectTarget(graphics, battle, party, item.Name, asset, Controls::Type::ENEMY);
                                }
                                else
                                {
                                    target = battle.Map.Find(opponents[0].Type == Character::ControlType::PLAYER ? Map::Object::PLAYER : Map::Object::ENEMY, opponents[0].Id);
                                }

                                if (target.IsNone() || battle.Map.Distance(src, target) < 2)
                                {
                                    Interface::MessageBox(graphics, background, "INVALID TARGET", Color::Highlight);
                                }
                                else
                                {
                                    update = Interface::TargetAction(graphics, background, battle, party, character, id, target);
                                }
                            }
                        }
                        else
                        {
                            if (opponents.size() == 0)
                            {
                                Interface::MessageBox(graphics, background, Interface::GetText(Interface::MSG_ENEMIES), Color::Highlight);
                            }
                            else
                            {
                                auto asset = Asset::Type::FIGHT;

                                if (item.Type == Item::Type::STEEL_SCEPTRE)
                                {
                                    asset = Asset::Type::STEEL_SCEPTRE;
                                }

                                auto target = Point(-1, -1);

                                if (opponents.size() > 1)
                                {
                                    target = Interface::SelectTarget(graphics, battle, party, item.Name, asset, Controls::Type::ENEMY);
                                }
                                else
                                {
                                    target = battle.Map.Find(opponents[0].Type == Character::ControlType::PLAYER ? Map::Object::PLAYER : Map::Object::ENEMY, opponents[0].Id);
                                }

                                if (target.IsNone() || battle.Map.Distance(src, target) > 1)
                                {
                                    Interface::MessageBox(graphics, background, "INVALID TARGET", Color::Highlight);
                                }
                                else
                                {
                                    update = Interface::TargetAction(graphics, background, battle, party, character, id, target);
                                }
                            }
                        }
                    }

                    if (update)
                    {
                        done = true;
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

    // (character) show inventory while in battle
    bool ShowInventory(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Party::Base &party, Character::Base &character, Point src)
    {
        auto update = false;

        auto exit = false;

        while (!exit)
        {
            auto limit = std::min(4, int(character.Items.size()));

            auto start = 0;

            auto last = start + limit;

            auto options = int(character.Items.size());

            // wrap length
            auto wrap = BloodSword::TileSize * 6;

            auto text_list = Graphics::TextList();

            for (auto &item : character.Items)
            {
                text_list.push_back(Graphics::RichText(item.String(true), Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, wrap));
            }

            auto menu = Graphics::CreateText(graphics, text_list);

            // default width
            auto w = wrap;

            // default height
            auto h = BloodSword::TileSize;

            // padding
            auto pads = BloodSword::Pad * 2;

            for (auto &item : menu)
            {
                w = std::max(BloodSword::Width(item) + pads, wrap);

                h = std::max(BloodSword::Height(item) + pads, h);
            }

            auto x = (graphics.Width - w) / 2 - (character.Items.size() > limit ? (BloodSword::HalfTile + 1) : 0);

            auto y = (graphics.Height - h * (limit + 1)) / 2 - BloodSword::HalfTile + BloodSword::Pad;

            auto input = Controls::User();

            auto done = false;

            auto frame_x = x - BloodSword::HalfTile;

            auto frame_y = y - BloodSword::HalfTile + BloodSword::Pad;

            auto frame_w = w + BloodSword::HalfTile * (options > limit ? 4 : 2);

            auto frame_h = (limit * h) + (BloodSword::HalfTile * 5) + BloodSword::OddPad;

            while (!done)
            {
                auto overlay = Interface::Menu(menu, x, y, w, h, start, last, limit, Color::Background, Color::Background, Color::Active, true);

                // add frame at the back
                overlay.Elements.insert(overlay.Elements.begin(), Scene::Element(frame_x, frame_y, frame_w, frame_h, Color::Background, Color::Active, BloodSword::Border));

                auto &lastControl = overlay.Controls.back();

                auto id = lastControl.Id + 1;

                auto first = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                auto bottom = overlay.Controls[first + limit - 1].Y + h + BloodSword::LargePad;

                overlay.VerifyAndAdd(Scene::Element(Asset::Get(Asset::Type::BACK), x - BloodSword::SmallPad, bottom));

                overlay.Add(Controls::Base(Controls::Type::BACK, id, id, id, first + limit - 1, id, x - BloodSword::SmallPad, bottom, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

                if (input.Up)
                {
                    input.Current = Controls::Find(overlay.Controls, Controls::Type::SCROLL_UP);

                    input.Up = false;
                }
                else if (input.Down)
                {
                    input.Current = Controls::Find(overlay.Controls, Controls::Type::SCROLL_DOWN);

                    input.Down = false;
                }

                input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

                if ((input.Selected && input.Type != Controls::Type::NONE && !input.Hold) || input.Up || input.Down)
                {
                    if (input.Type == Controls::Type::BACK)
                    {
                        done = true;

                        exit = true;
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
                    else if (input.Type == Controls::Type::CHOICE)
                    {
                        auto list = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                        auto choice = start + (input.Current - list);

                        if (choice >= 0 && choice < character.Items.size())
                        {
                            update = Interface::ManageItem(graphics, background, battle, party, character, src, choice);
                        }

                        // check if item list is unchanged
                        if (character.Items.size() == 0 || character.Items.size() != options || update)
                        {
                            done = true;

                            exit = true;
                        }
                    }
                }
            }

            BloodSword::Free(menu);
        }

        return update;
    }
}

#endif
