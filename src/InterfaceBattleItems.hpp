#ifndef __INTERFACE_BATTLE_ITEMS_HPP__
#define __INTERFACE_BATTLE_ITEMS_HPP__

#include "InterfaceBattleScene.hpp"

// in-battle use of item
namespace BloodSword::Interface
{
    // show item book description
    void ShowBattleDescription(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Character::Base &character, int id)
    {
        auto &item = character.Items[id];

        if (item.BattleDescriptions.size() > 0)
        {
            for (auto &descriptions : item.BattleDescriptions)
            {
                if (Book::IsDefined(descriptions.Description) && Book::Equal(descriptions.Battle, party.Location))
                {
                    Interface::ShowBookDescription(graphics, background, descriptions.Description);

                    break;
                }
            }
        }
    }

    // process item effects in battle mode
    void BattleItemEffects(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Party::Base &party, Character::Base &character, int id)
    {
        auto &item = character.Items[id];

        if (item.Type == Item::Type::SCROLL_INVISIBILITY)
        {
            if (battle.Has(Controls::Type::FLEE))
            {
                Interface::ShowBookDescription(graphics, background, battle.ActionCancels[Controls::Type::FLEE]);
            }
            else
            {
                if (character.IsPlayer() && !battle.Map.Find(Map::Object::EXIT).IsNone() && !battle.Has(Battle::Condition::CANNOT_FLEE))
                {
                    character.Add(Character::Status::INVISIBLE);

                    if (item.Drops)
                    {
                        battle.Loot.push_back(item);

                        Interface::ConsumeItem(character, id);
                    }
                }
                else
                {
                    Interface::MessageBox(graphics, background, "YOU CANNOT FLEE FROM THIS BATTLE!", Color::Highlight);
                }
            }
        }
        else if (item.Type == Item::Type::BIRCH_BARK_SCROLL)
        {
            for (auto character = 0; character < party.Count(); character++)
            {
                if (Engine::IsAlive(party[character]))
                {
                    if (!Interface::Test(graphics, background, party[character], Attribute::Type::PSYCHIC_ABILITY))
                    {
                        Interface::MessageBox(graphics, background, party[character].Name + " SUCCUMBS TO DREADFUL PUNITION", Color::Highlight);

                        Engine::GainEndurance(party[character], -8, true);
                    }
                    else
                    {
                        Interface::MessageBox(graphics, background, party[character].Name + " RESISTS DREADFUL PUNITION", Color::Active);
                    }
                }
            }

            for (auto character = 0; character < battle.Opponents.Count(); character++)
            {
                if (Engine::IsAlive(battle.Opponents[character]))
                {
                    if (!Interface::Test(graphics, background, battle.Opponents[character], Attribute::Type::PSYCHIC_ABILITY))
                    {
                        Interface::MessageBox(graphics, background, battle.Opponents[character].Name + " SUCCUMBS TO DREADFUL PUNITION", Color::Active);

                        Engine::GainEndurance(battle.Opponents[character], -8, true);
                    }
                    else
                    {
                        Interface::MessageBox(graphics, background, battle.Opponents[character].Name + " RESISTS DREADFUL PUNITION", Color::Highlight);
                    }
                }
            }

            Interface::ConsumeItem(character, id);
        }
        else
        {
            Interface::ItemEffects(graphics, background, party, character, item.Type);
        }
    }

    // get item's target type and default to fallbacks
    Target::Type GetTargetType(Item::Base &item, Target::Type target)
    {
        auto type = Target::Type::NONE;

        if (item.HasEffect(target))
        {
            type = target;
        }
        else if (item.HasEffect(Target::Type::ENEMY))
        {
            type = Target::Type::ENEMY;
        }
        else if (item.HasEffect(Target::Type::PLAYER))
        {
            type = Target::Type::PLAYER;
        }

        return type;
    }

    // process targetted action
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

            auto target_type = Interface::GetTargetType(character.Items[id], defender.Target);

            // display any target specific description
            if (target_type != Target::Type::NONE && target_type != Target::Type::ENEMY)
            {
                Interface::ShowBattleDescription(graphics, background, party, character, id);
            }

            // check if it is a blasting / weapon
            if (target_type != Target::Type::NONE && item.HasEffect(target_type) && item.TargetEffects[target_type] == Item::TargetEffect::DAMAGE_TARGET && item.HasDamageType(target_type))
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

                        Interface::Resurrect(graphics, background, party, defender);

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
            else if (target_type != Target::Type::NONE && item.HasEffect(target_type) && item.TargetEffects[target_type] == Item::TargetEffect::THROW)
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

                        Interface::Resurrect(graphics, background, party, defender);

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
            else if (target_type != Target::Type::NONE && item.TargetEffects[target_type] == Item::TargetEffect::KILL_TARGET)
            {
                if ((character.IsPlayer() && target_enemy) || (character.IsEnemy() && target_player))
                {
                    // unalive player
                    defender.Value(Attribute::Type::ENDURANCE, 0);

                    battle.Map.Remove(defender.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, target_id);

                    Interface::MessageBox(graphics, background, defender.Name + " KILLED!", defender.IsPlayer() ? Color::Highlight : Color::Active);

                    if (item.Drops)
                    {
                        battle.Loot.push_back(item);
                    }

                    Interface::ConsumeItem(character, id);

                    used = true;
                }
                else
                {
                    Interface::MessageBox(graphics, background, "INVALID TARGET", Color::Highlight);
                }
            }
            else if (target_type != Target::Type::NONE && item.TargetEffects[target_type] == Item::TargetEffect::SELF_BURN)
            {
                if ((character.IsPlayer() && target_enemy) || (character.IsEnemy() && target_player))
                {
                    auto my_id = party.Index(character.Class);

                    // damage self
                    Engine::GainEndurance(character, -1, true);

                    Interface::Resurrect(graphics, background, party, character);

                    if (!Engine::IsAlive(character))
                    {
                        battle.Map.Remove(character.IsPlayer() ? Map::Object::PLAYER : Map::Object::ENEMY, my_id);

                        Interface::MessageBox(graphics, background, character.Name + " KILLED!", character.IsPlayer() ? Color::Highlight : Color::Active);
                    }

                    character.Add(Character::Status::BURNED);

                    if (item.Drops)
                    {
                        battle.Loot.push_back(item);
                    }

                    Interface::ConsumeItem(character, id);

                    used = true;
                }
                else
                {
                    Interface::MessageBox(graphics, background, "INVALID TARGET", Color::Highlight);
                }
            }
            else if (target_type != Target::Type::NONE && item.HasEffect(target_type) && item.TargetEffects[target_type] == Item::TargetEffect::IMMUNE)
            {
                Interface::MessageBox(graphics, background, defender.Name + " CANNOT BE HARMED BY THE " + item.Name, defender.IsPlayer() ? Color::Active : Color::Highlight);

                used = true;
            }
        }
        else
        {
            Interface::MessageBox(graphics, background, "NOTHING HAPPENS!", Color::Highlight);

            used = true;
        }

        return used;
    }

    // (character) manage item while in battle
    bool ManageItem(Graphics::Base &graphics, Scene::Base &background, Battle::Base &battle, Party::Base &party, Character::Base &character, Point src, int id)
    {
        auto update = false;

        auto assets = Asset::List();

        auto controls = Controls::List();

        auto captions = std::vector<std::string>();

        // take item
        assets.push_back(Asset::Map("USE"));

        controls.push_back(Controls::Type::USE);

        captions.push_back("USE");

        // go back
        assets.push_back(Asset::Map("BACK"));

        controls.push_back(Controls::Type::BACK);

        auto values = std::vector<int>(controls.size());

        std::iota(values.begin(), values.end(), 0);

        auto done = false;

        auto &item = character.Items[id];

        while (!done)
        {
            auto selection = Interface::SelectIcons(graphics, background, character.Items[id].Name.c_str(), assets, values, captions, 1, 1, Asset::NONE, false);

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
                        auto items = character.Items.size();

                        Interface::BattleItemEffects(graphics, background, battle, party, character, id);

                        update = character.Items.size() != items;
                    }
                    else
                    {
                        auto src = battle.Map.Find(Map::Object::PLAYER, Engine::Find(party, character));

                        auto opponents = Engine::FightTargets(battle.Map, battle.Opponents, src, true, false);

                        auto ranged = item.Has(Item::Property::RANGED);

                        auto all_ranges = item.Has(Item::Property::ALL_RANGES);

                        if (ranged)
                        {
                            auto targets = all_ranges ? Engine::AllTargets(battle.Map, battle.Opponents, src, true, false) : Engine::RangedTargets(battle.Map, battle.Opponents, src, true, false);

                            if (opponents.size() > 0 && !all_ranges)
                            {
                                Interface::MessageBox(graphics, background, Interface::GetText(Interface::MSG_NEARBY), Color::Highlight);
                            }
                            else if (targets.size() > 0)
                            {
                                auto asset = item.Asset != Asset::NONE ? item.Asset : Asset::Map("SHOOT");

                                auto target = Point(-1, -1);

                                // do something
                                if (targets.size() > 1)
                                {
                                    target = Interface::SelectTarget(graphics, battle, party, item.Name, asset, Controls::Type::ENEMY);
                                }
                                else
                                {
                                    target = battle.Map.Find(targets[0].Type == Character::ControlType::PLAYER ? Map::Object::PLAYER : Map::Object::ENEMY, targets[0].Id);
                                }

                                if (target.IsNone() || (!all_ranges && battle.Map.Distance(src, target) < 2))
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
                                auto asset = Asset::Map("FIGHT");

                                if (item.Asset != Asset::NONE)
                                {
                                    asset = item.Asset;
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
            auto wrap = BloodSword::Wrap;

            auto text_list = Graphics::TextList();

            for (auto &item : character.Items)
            {
                text_list.push_back(Graphics::RichText(item.String(true), Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, wrap));
            }

            auto menu = Graphics::CreateText(graphics, text_list);

            // padding
            auto pads = BloodSword::LargePad;

            // default width
            auto w = std::max(BloodSword::Width(menu) + pads, wrap);

            // default height
            auto h = std::max(BloodSword::Height(menu) + pads, BloodSword::TileSize);

            auto x = (graphics.Width - w) / 2 - (character.Items.size() > limit ? (BloodSword::HalfTile + 1) : 0);

            auto y = (graphics.Height - h * (limit + 1)) / 2 - BloodSword::HalfTile + BloodSword::Pad;

            auto input = Controls::User();

            auto done = false;

            auto frame_x = x - BloodSword::HalfTile;

            auto frame_y = y - BloodSword::HalfTile + BloodSword::Pad;

            auto frame_w = w + (options > limit ? BloodSword::DoubleTile : BloodSword::TileSize);

            auto frame_h = (limit * h) + (BloodSword::FrameHeight) + BloodSword::OddPad;

            auto texture_back = Asset::Get("BACK");

            while (!done)
            {
                auto overlay = Interface::Menu(menu, x, y, w, h, start, last, limit, Color::Background, Color::Background, Color::Active, true);

                // add frame at the back
                overlay.Elements.insert(overlay.Elements.begin(), Scene::Element(frame_x, frame_y, frame_w, frame_h, Color::Background, Color::Active, BloodSword::Border));

                auto &lastControl = overlay.Controls.back();

                auto id = lastControl.Id + 1;

                auto first = Controls::Find(overlay.Controls, Controls::Type::CHOICE);

                auto bottom = overlay.Controls[first + limit - 1].Y + h + BloodSword::LargePad;

                overlay.VerifyAndAdd(Scene::Element(texture_back, x - BloodSword::SmallPad, bottom));

                overlay.Add(Controls::Base(Controls::Type::BACK, id, id, id, first + limit - 1, id, x - BloodSword::SmallPad, bottom, BloodSword::TileSize, BloodSword::TileSize, Color::Active));

                Interface::ClearScrolling(overlay, input, Controls::Type::SCROLL_UP, Controls::Type::SCROLL_DOWN);

                input = Input::WaitForInput(graphics, {background, overlay}, overlay.Controls, input, true);

                if (Input::Validate(input))
                {
                    if (input.Type == Controls::Type::BACK)
                    {
                        done = true;

                        exit = true;
                    }
                    else if (Input::IsUp(input))
                    {
                        Interface::ScrollUp(overlay, input, Controls::Type::SCROLL_UP, options, limit, start, last);
                    }
                    else if (Input::IsDown(input))
                    {
                        Interface::ScrollDown(overlay, input, Controls::Type::SCROLL_DOWN, options, limit, start, last);
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
