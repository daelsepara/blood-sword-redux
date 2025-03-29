#ifndef __CONDITIONS_EVALUATE_HPP__
#define __CONDITIONS_EVALUATE_HPP__

#include <algorithm>
#include <vector>

#include "Book.hpp"
#include "Graphics.hpp"
#include "Item.hpp"
#include "Party.hpp"
#include "Templates.hpp"
#include "ItemEffects.hpp"
#include "Conditions.hpp"

namespace BloodSword::Conditions
{
    // results of evaluating a condition
    class Evaluation
    {
    public:
        bool Result = false;

        bool Failed = false;

        // text (usually on failure)
        std::string Text = std::string();

        Evaluation() {}

        Evaluation(bool result, bool failed, std::string text) : Result(result), Failed(failed), Text(text) {}

        Evaluation(bool result, std::string text) : Result(result), Text(text) {}

        Evaluation(bool result) : Result(result) {}
    };

    void InternalError(Graphics::Base &graphics, Scene::Base &background, Conditions::Type condition)
    {
        std::string message = "Internal Error: " + std::string(Conditions::TypeMapping[condition]) + "!";

        Interface::InternalError(graphics, background, message);
    }

    void Log(Conditions::Base &condition, bool result, bool failed, std::string text)
    {
        // debug info
        std::cerr << "[CONDITION] "
                  << std::string(Conditions::TypeMapping[condition.Type])
                  << " ([RESULT] "
                  << (result ? "TRUE" : "FALSE")
                  << ", [FAILED] "
                  << (failed ? "TRUE" : "FALSE");

        if (text.size() > 0)
        {
            std::cerr << ", [TEXT] " << text;
        }

        std::cerr << ")";

        if (condition.Invert)
        {
            std::cerr << " [INVERTED]";
        }

        std::cerr << std::endl;
    }

    // routine to validate "condition"
    Conditions::Evaluation Process(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Conditions::Base &condition)
    {
        auto result = false;

        auto failed = false;

        auto internal_error = false;

        auto text = std::string();

        if (condition.Type == Conditions::Type::NORMAL)
        {
            result = true;

            // variables
            // 0 - text to display (when used in an event)
            if (condition.Variables.size() > 0)
            {
                text = condition.Variables[0];
            }
        }
        else if (condition.Type == Conditions::Type::IN_PARTY)
        {
            internal_error = true;

            // variables
            // 0 - player
            if (condition.Variables.size() > 0)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    result = party.Has(character);

                    if (!result)
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);

                        result = false;
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::IS_DEAD)
        {
            internal_error = true;

            // variables
            // 0 - player
            if (condition.Variables.size() > 0)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    if (party.Has(character))
                    {
                        if (!Engine::IsAlive(party[character]))
                        {
                            result = true;
                        }
                        else
                        {
                            text = party[character].Name + " IS NOT DEAD";
                        }
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::NotInParty(character);
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::CHOSEN_PLAYER)
        {
            // variables
            // 0 - player to check if they were chosen
            if (condition.Variables.size() > 0)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                result = (party.ChosenCharacter == character);

                if (!result)
                {
                    text = std::string("YOU HAVE CHOSEN A DIFFERENT PLAYER!");
                }
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::CHOSEN_NUMBER)
        {
            // variables
            // 0 - number
            if (condition.Variables.size() > 0)
            {
                auto number = party.Number(condition.Variables[0]);

                result = (party.ChosenNumber == number);

                if (!result)
                {
                    text = std::string("YOU HAVE CHOSEN A DIFFERENT NUMBER!");
                }
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::HAS_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - item
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto item = Item::Map(condition.Variables[0]);

                if (item != Item::Type::NONE)
                {
                    result = party.Has(item);

                    if (!result)
                    {
                        text = Engine::NoItem(item);
                    }

                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::DROP_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - player / ALL
            // 1 - item
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto item = Item::Map(condition.Variables[1]);

                if (item != Item::Type::NONE && (is_party || (character != Character::Class::NONE && party.Has(character) && Engine::IsAlive(party[character]))))
                {
                    result = is_party ? party.Has(item) : party[character].Has(item);

                    if (!result)
                    {
                        text = Engine::NoItem(item);
                    }
                    else
                    {
                        if (is_party)
                        {
                            party.Remove(item);
                        }
                        else
                        {
                            party[character].Remove(item);
                        }
                    }

                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::REVEAL_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - item
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto item = Item::Map(condition.Variables[0]);

                if (item != Item::Type::NONE)
                {
                    result = party.Has(item);

                    if (!result)
                    {
                        text = Engine::NoItem(item);
                    }
                    else
                    {
                        for (auto i = 0; i < party.Count(); i++)
                        {
                            if (Engine::IsAlive(party[i]))
                            {
                                for (auto &this_item : party[i].Items)
                                {
                                    if (this_item.Is(item))
                                    {
                                        this_item.Reveal();
                                    }
                                }
                            }
                        }

                        result = true;
                    }

                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::IN_PARTY_WITH_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - item
            if (condition.Variables.size() > 1)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto item = Item::Map(condition.Variables[1]);

                if (character != Character::Class::NONE && item != Item::Type::NONE)
                {
                    result = party.Has(character) && Engine::IsAlive(party[character]) && party.Has(item);

                    if (result)
                    {
                        text = std::string(Character::ClassMapping[character]) + " HAS THE " + Item::TypeMapping[item];
                    }
                    else if (!(party.Has(character)))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);
                    }
                    else
                    {
                        text = Engine::NoItem(item);
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::TEST_ATTRIBUTE)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - attribute
            // 2 - failure message
            if (condition.Variables.size() > 2)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto attribute = Attribute::Map(condition.Variables[1]);

                if (character != Character::Class::NONE && attribute != Attribute::Type::NONE && !Book::IsUndefined(condition.Failure))
                {
                    if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);
                    }
                    else
                    {
                        auto test = Interface::Test(graphics, background, party[character], attribute);

                        if (!test)
                        {
                            failed = true;

                            text = condition.Variables[2];
                        }
                        else
                        {
                            result = true;
                        }
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::LOSE_ALL)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - item
            if (condition.Variables.size() > 1)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto item = Item::Map(condition.Variables[1]);

                if (item != Item::Type::NONE && character != Character::Class::NONE)
                {
                    if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (party.Has(character))
                    {
                        if (!Engine::IsAlive(party[character]))
                        {
                            if (party[character].Quantity(item) > 0)
                            {
                                result = true;

                                party[character].Remove(item, party[character].Quantity(item));

                                text = std::string("YOU LOSE ALL ") + std::string(Item::TypeMapping[item]) + "!";
                            }
                            else
                            {
                                text = std::string("YOU DO NOT HAVE ANY ") + Item::TypeMapping[item] + "!";
                            }
                        }
                        else
                        {
                            text = Engine::IsDead(party[character]);
                        }
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::BATTLE_VICTORY)
        {
            result = (party.LastBattle == Battle::Result::VICTORY);

            if (!result)
            {
                text = "YOU WERE NOT VICTORIOUS IN THE LAST BATTLE!";
            }
        }
        else if (condition.Type == Conditions::Type::BATTLE_FLEE)
        {
            result = (party.LastBattle == Battle::Result::FLEE);

            if (!result)
            {
                text = "YOU DID NOT FLEE IN YOUR LAST BATTLE!";
            }
        }
        else if (condition.Type == Conditions::Type::BATTLE_ENTHRALMENT)
        {
            result = (party.LastBattle == Battle::Result::ENTHRALLED);

            if (!result)
            {
                text = "YOU HAVE NOT SUBDUED YOUR OPPONENTS!";
            }
        }
        else if (condition.Type == Conditions::Type::BATTLE_VICTORY_OR_ENTHRALMENT)
        {
            result = (party.LastBattle == Battle::Result::ENTHRALLED || party.LastBattle == Battle::Result::VICTORY);

            if (!result)
            {
                text = "YOU WERE NOT VICTORIOUS IN THE LAST BATTLE!";
            }
        }
        else if (condition.Type == Conditions::Type::HAVE_COLLEAGUES)
        {
            result = (Engine::Count(party) > 1);

            if (result && condition.Variables.size() > 0)
            {
                // variables (when used in an event):
                // 0 - text to display on success
                text = condition.Variables[0];
            }
            else if (!result && condition.Variables.empty())
            {
                // when used in a choice
                text = "YOU ARE ALONE!";
            }
        }
        else if (condition.Type == Conditions::Type::SOLO)
        {
            result = (Engine::Count(party) == 1);

            if (result && condition.Variables.size() > 0)
            {
                // variables (when used in an event):
                // 0 - text to display on success
                text = condition.Variables[0];
            }
            else if (!result && condition.Variables.empty())
            {
                text = "YOU ARE NOT ALONE!";
            }
        }
        else if (condition.Type == Conditions::Type::GAIN_STATUS)
        {
            internal_error = true;

            // variables
            // 0 - player (or ALL)
            // 1 - status
            // 2 - TRUE/FALSE hide message (optional)
            if (condition.Variables.size() > 1)
            {
                auto status = Character::MapStatus(condition.Variables[1]);

                if (Engine::ToUpper(condition.Variables[0]) == "ALL")
                {
                    if (status != Character::Status::NONE && Engine::IsAlive(party))
                    {
                        // entire party gains status
                        party.Add(status);

                        result = true;

                        if (condition.Variables.size() < 2 || (condition.Variables.size() > 2 && Engine::ToUpper(condition.Variables[2]) != "TRUE"))
                        {
                            if (Engine::Count(party) > 1)
                            {
                                text = "EVERYONE GAINS [" + std::string(Character::StatusMapping[status]) + "]";
                            }
                            else
                            {
                                text = "YOU GAIN [" + std::string(Character::StatusMapping[status]) + "]";
                            }
                        }

                        internal_error = false;
                    }
                }
                else
                {
                    auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                    if (character != Character::Class::NONE && status != Character::Status::NONE)
                    {
                        result = party.Has(character);

                        if (!result)
                        {
                            text = Engine::NotInParty(character);
                        }
                        else if (!Engine::IsAlive(party[character]))
                        {
                            text = Engine::IsDead(party[character]);

                            result = false;
                        }
                        else
                        {
                            party[character].Add(status);

                            if (condition.Variables.size() < 2 || (condition.Variables.size() > 2 && Engine::ToUpper(condition.Variables[2]) != "TRUE"))
                            {
                                text = party[character].Name + " GAINS [" + Character::StatusMapping[status] + "]";
                            }
                        }

                        internal_error = false;
                    }
                }
            }
        }
        else if (condition.Type == Conditions::Type::LOSE_STATUS)
        {
            internal_error = true;

            // variables
            // 0 - player (or ALL)
            // 1 - status
            // 2 - TRUE/FALSE hide message (optional)
            if (condition.Variables.size() > 1)
            {
                auto status = Character::MapStatus(condition.Variables[1]);

                if (Engine::ToUpper(condition.Variables[0]) == "ALL")
                {
                    if (status != Character::Status::NONE && Engine::IsAlive(party))
                    {
                        if (party.Has(status))
                        {
                            // remove status from entire party
                            party.Remove(status);

                            result = true;

                            if (condition.Variables.size() < 2 || (condition.Variables.size() > 2 && Engine::ToUpper(condition.Variables[2]) != "TRUE"))
                            {
                                if (Engine::Count(party) > 1)
                                {
                                    text = "EVERYONE LOSES [" + std::string(Character::StatusMapping[status]) + "]";
                                }
                                else
                                {
                                    text = "YOU LOSE [" + std::string(Character::StatusMapping[status]) + "]";
                                }
                            }
                        }

                        internal_error = false;
                    }
                }
                else
                {
                    auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                    if (character != Character::Class::NONE && status != Character::Status::NONE)
                    {
                        result = party.Has(character);

                        if (!result)
                        {
                            text = Engine::NotInParty(character);
                        }
                        else if (!Engine::IsAlive(party[character]))
                        {
                            text = Engine::IsDead(party[character]);

                            result = false;
                        }
                        else if (party[character].Has(status))
                        {
                            party[character].Remove(status);

                            if (condition.Variables.size() < 2 || (condition.Variables.size() > 2 && Engine::ToUpper(condition.Variables[2]) != "TRUE"))
                            {
                                text = party[character].Name + " LOSES [" + Character::StatusMapping[status] + "]";
                            }
                        }

                        internal_error = false;
                    }
                }
            }
        }
        else if (condition.Type == Conditions::Type::FIRST)
        {
            internal_error = true;

            // variables
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else
                    {
                        for (auto i = 0; i < party.Count(); i++)
                        {
                            if (Engine::IsAlive(party[i]))
                            {
                                result = (party[i].Class == character);

                                break;
                            }
                        }

                        if (!result)
                        {
                            text = std::string(Character::ClassMapping[character]) + " NOT THE FIRST IN BATTLE ORDER!";
                        }
                    }

                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::LAST)
        {
            internal_error = true;

            // variables
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else
                    {
                        for (auto i = (party.Count() - 1); i >= 0; i--)
                        {
                            if (Engine::IsAlive(party[i]))
                            {
                                result = (party[i].Class == character);

                                break;
                            }
                        }

                        if (!result)
                        {
                            text = std::string(Character::ClassMapping[character]) + " NOT THE LAST IN BATTLE ORDER!";
                        }
                    }

                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::TEST_GAIN_STATUS)
        {
            internal_error = true;

            // variables
            // 0 - player (or ALL)
            // 1 - attribute
            // 2 - status gain on success
            // 3 - status gain on failure
            if (condition.Variables.size() > 3)
            {
                auto attribute = Attribute::Map(condition.Variables[1]);

                auto status_success = Character::MapStatus(condition.Variables[2]);

                auto status_fail = Character::MapStatus(condition.Variables[3]);

                if (attribute != Attribute::Type::NONE && status_fail != Character::Status::NONE)
                {
                    if (Engine::ToUpper(condition.Variables[0]) == "ALL")
                    {
                        if (Engine::IsAlive(party))
                        {
                            result = true;

                            for (auto character = 0; character < party.Count(); character++)
                            {
                                if (Engine::IsAlive(party[character]))
                                {
                                    if (Interface::Test(graphics, background, party[character], attribute))
                                    {
                                        party[character].Add(status_success);
                                    }
                                    else
                                    {
                                        party[character].Add(status_fail);
                                    }
                                }
                            }

                            internal_error = false;
                        }
                    }
                    else
                    {
                        auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                        if (character != Character::Class::NONE)
                        {
                            if (party.Has(character))
                            {
                                if (Engine::IsAlive(party[character]))
                                {
                                    result = true;

                                    if (Interface::Test(graphics, background, party[character], attribute))
                                    {
                                        party[character].Add(status_success);
                                    }
                                    else
                                    {
                                        party[character].Add(status_fail);
                                    }
                                }
                                else
                                {
                                    text = Engine::IsDead(party[character]);
                                }
                            }
                            else
                            {
                                text = Engine::NotInParty(character);
                            }

                            internal_error = false;
                        }
                    }
                }
            }
        }
        else if (condition.Type == Conditions::Type::COUNT_STATUS)
        {
            internal_error = true;

            // variables
            // 0 - status
            // 1 - min threshold
            // 2 - max threshold
            if (condition.Variables.size() > 2)
            {
                auto status = Character::MapStatus(condition.Variables[0]);

                auto min_count = party.Number(condition.Variables[1]);

                auto max_count = party.Number(condition.Variables[2]);

                if (Engine::IsAlive(party) && status != Character::Status::NONE && min_count >= 0 && max_count >= 0 && min_count <= max_count)
                {
                    auto count = Engine::Count(party, Character::ControlType::PLAYER, status);

                    result = count >= min_count && count <= max_count;

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::SELECT_MULTIPLE)
        {
            internal_error = true;

            // variables:
            // 0 - criteria for preselection (status)
            // 1 - status when selected
            // 2 - status when not selected
            // 3 - message to display
            if (Engine::IsAlive(party) && condition.Variables.size() > 3)
            {
                auto preselect = Character::MapStatus(condition.Variables[0]);

                auto selected = Character::MapStatus(condition.Variables[1]);

                auto excluded = Character::MapStatus(condition.Variables[2]);

                if (selected != Character::Status::NONE || excluded != Character::Status::NONE)
                {
                    Interface::SelectMultiple(graphics, background, party, condition.Variables[3].c_str(), preselect, selected, excluded, true);

                    result = true;

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::CALL_TO_MIND)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - spell
            if (condition.Variables.size() > 1)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto spell = Spells::Map(condition.Variables[1]);

                if (character != Character::Class::NONE && spell != Spells::Type::NONE)
                {
                    if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);
                    }
                    else if (!party[character].HasCalledToMind(spell))
                    {
                        party[character].CallToMind(spell);

                        text = "[" + std::string(Spells::TypeMapping[spell]) + "] CALLED TO MIND.";

                        result = true;
                    }
                    else
                    {
                        text = "[" + std::string(Spells::TypeMapping[spell]) + "] ALREADY CALLED TO MIND.";

                        result = true;
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::FORGET_SPELL)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - spell
            if (condition.Variables.size() > 1)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto spell = Spells::Map(condition.Variables[1]);

                if (character != Character::Class::NONE && spell != Spells::Type::NONE)
                {
                    if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);
                    }
                    else if (party[character].HasCalledToMind(spell))
                    {
                        party[character].Forget(spell);

                        text = "[" + std::string(Spells::TypeMapping[spell]) + "] UN-CALLED FROM MIND.";

                        result = true;
                    }
                    else
                    {
                        text = "[" + std::string(Spells::TypeMapping[spell]) + "] WAS NOT CALLED TO MIND.";

                        result = true;
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::KALUGEN_SELECT)
        {
            internal_error = true;

            // variables
            // 0 - message (to display)
            // 1 - number of options
            // 2 - minimum number of items to be selected
            // 3 - maximum number of items to be selected
            // 4 - hidden (true/false)
            // 5 to 5+(1) - assets
            // last - asset (hidden)
            if (condition.Variables.size() > 5)
            {
                auto options = party.Number(condition.Variables[1]);

                auto min_select = party.Number(condition.Variables[2]);

                auto max_select = party.Number(condition.Variables[3]);

                auto hidden = Engine::ToUpper(condition.Variables[4]) == "TRUE";

                auto expected = 4 + options + (hidden ? 1 : 0);

                if (options > 0 && min_select > 0 && min_select <= max_select && condition.Variables.size() > expected)
                {
                    // contains assets
                    auto assets = Asset::List();

                    // contains card ids
                    auto values = std::vector<int>();

                    // set hidden asset
                    auto asset_hidden = hidden ? Asset::Map(condition.Variables[5 + options]) : Asset::Type::NONE;

                    // collect assets and set values
                    for (auto i = 0; i < options; i++)
                    {
                        auto asset = Asset::Map(condition.Variables[5 + i]);

                        if (asset != Asset::Type::NONE)
                        {
                            assets.push_back(asset);
                        }

                        values.push_back(i);
                    }

                    if (assets.size() == values.size())
                    {
                        // clear current selection
                        party.Cards.clear();

                        // select cards
                        auto selection = Interface::SelectIcons(graphics, background, condition.Variables[0].c_str(), assets, values, {}, min_select, max_select, asset_hidden, hidden);

                        for (auto i = 0; i < selection.size(); i++)
                        {
                            if (i >= 0 && i < Items::KalugenDeck.size())
                            {
                                party.Cards.push_back(Items::KalugenDeck[selection[i]]);
                            }
                        }

                        if (party.Cards.size() >= min_select && party.Cards.size() <= max_select)
                        {
                            result = true;

                            internal_error = false;
                        }
                    }
                }
            }
        }
        else if (condition.Type == Conditions::Type::KALUGEN_LIST)
        {
            if (party.Cards.size() > 0)
            {
                text = "You have chosen " + std::string(party.Cards.size() > 1 ? "these cards" : "this card") + ": ";

                for (auto i = 0; i < party.Cards.size(); i++)
                {
                    if (i > 0)
                    {
                        text += ", ";
                    }

                    text += Item::CardMapping[party.Cards[i]];
                }

                text += ".";

                result = true;
            }
            else
            {
                text = "YOU HAVE NOT SELECTED ANY CARDS!";
            }
        }
        else if (condition.Type == Conditions::Type::KALUGEN_SCORE)
        {
            // show score
            auto score = Engine::ScoreKalugenGame(party);

            text = "You scored: " + std::to_string(score) + ".";

            result = true;
        }
        else if (condition.Type == Conditions::Type::KALUGEN_PLAY)
        {
            internal_error = true;

            // variables
            // 0 - score to beat
            if (condition.Variables.size() > 0)
            {
                auto beat = party.Number(condition.Variables[0]);

                auto score = Engine::ScoreKalugenGame(party);

                if (beat > 0 && score >= 0)
                {
                    result = (score >= beat);

                    if (result)
                    {
                        text = "YOU HAVE " + std::string(score > beat ? "BEATEN" : "MATCHED");
                    }
                    else
                    {
                        text = "YOU HAVE LOST TO";
                    }

                    text += " THE SCORE OF " + std::to_string(beat);

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::DISCARD_ITEMS)
        {
            // variables:
            // 0 - player / ALL
            // 1 .. N - items to discard (type)
            if (condition.Variables.size() > 1)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto characters = Character::Classes();

                auto items = Items::List();

                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                if (is_party || character != Character::Class::NONE)
                {
                    if ((is_party && Engine::IsAlive(party)) || (party.Has(character) && Engine::IsAlive(party[character])))
                    {
                        for (auto i = 1; i < condition.Variables.size(); i++)
                        {
                            auto item = Item::Map(condition.Variables[i]);

                            if (item != Item::Type::NONE)
                            {
                                items.push_back(item);
                            }
                        }

                        if (items.size() > 0)
                        {
                            if (Engine::ToUpper(condition.Variables[0]) == "ALL")
                            {
                                for (auto i = 0; i < party.Count(); i++)
                                {
                                    if (Engine::IsAlive(party[i]))
                                    {
                                        characters.push_back(party[i].Class);
                                    }
                                }
                            }
                            else
                            {
                                characters.push_back(character);
                            }

                            auto discarded = std::vector<std::string>();

                            for (auto i = 0; i < items.size(); i++)
                            {
                                auto item = items[i];

                                if ((is_party && party.Has(item)) || (!is_party && party[character].Has(item)))
                                {
                                    discarded.push_back(Item::TypeMapping[item]);

                                    for (auto player : characters)
                                    {
                                        party[player].Remove(item);
                                    }
                                }
                            }

                            if (!discarded.empty())
                            {
                                text = (is_party ? "You" : party[character].Name) + " discard" + (is_party ? "" : "s") + " the ";

                                for (auto i = 0; i < discarded.size(); i++)
                                {
                                    if (i > 0 && discarded.size() > 2)
                                    {
                                        text += ", ";
                                    }

                                    if ((discarded.size() > 1) && (i == discarded.size() - 1))
                                    {
                                        text += " and the ";
                                    }

                                    text += discarded[i];
                                }

                                text += ".";

                                result = true;
                            }
                        }
                        else
                        {
                            // nothing to discard
                            internal_error = true;
                        }
                    }
                    else if (is_party)
                    {
                        text = Interface::DeathMessage(party);
                    }
                    else if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else
                    {
                        text = Engine::IsDead(party[character]);
                    }
                }
                else
                {
                    // party or player not specified
                    internal_error = true;
                }
            }
        }
        else if (condition.Type == Conditions::Type::RECEIVE_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - ALL/FIRST/LAST/SELECT/CHOSEN or player
            // 1 - FALL-BACK recipient (NONE/FIRST/LAST/SELECT)
            // 2 - item
            if (condition.Variables.size() > 2)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                character = (Engine::ToUpper(condition.Variables[0]) == "CHOSEN") ? party.ChosenCharacter : character;

                auto is_character = (character != Character::Class::NONE);

                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto is_first = (Engine::ToUpper(condition.Variables[0]) == "FIRST");

                auto is_last = (Engine::ToUpper(condition.Variables[0]) == "LAST");

                auto first_or_last = (is_first || is_last);

                auto selected = (Engine::ToUpper(condition.Variables[0]) == "SELECT");

                auto next_first = (Engine::ToUpper(condition.Variables[1]) == "FIRST");

                auto next_last = (Engine::ToUpper(condition.Variables[1]) == "LAST");

                auto next = is_character && (next_first || next_last);

                auto select_next = is_character && (Engine::ToUpper(condition.Variables[1]) == "SELECT");

                auto item = Item::Map(condition.Variables[2]);

                if ((is_party || first_or_last || selected || is_character) && Engine::IsAlive(party) && item != Item::Type::NONE && Items::Found(item))
                {
                    auto characters = Character::Classes();

                    if (is_party)
                    {
                        for (auto i = 0; i < party.Count(); i++)
                        {
                            if (Engine::IsAlive(party[i]))
                            {
                                characters.push_back(party[i].Class);
                            }
                        }
                    }
                    else if (first_or_last || (is_character && (!party.Has(character) || !Engine::IsAlive(party[character])) && next))
                    {
                        if (!next)
                        {
                            if (is_first)
                            {
                                characters.push_back(Engine::FirstClass(party));
                            }
                            else if (is_last)
                            {
                                characters.push_back(Engine::LastClass(party));
                            }
                        }
                        else
                        {
                            if (next_first)
                            {
                                characters.push_back(Engine::FirstClass(party));
                            }
                            else if (next_last)
                            {
                                characters.push_back(Engine::LastClass(party));
                            }
                        }
                    }
                    else if (selected || (is_character && (!party.Has(character) || !Engine::IsAlive(party[character])) && select_next))
                    {
                        std::string message = "WHO RECEIVES THE " + Items::Defaults[item].Name + "?";

                        character = Interface::SelectCharacter(graphics, background, party, message.c_str(), true, false, false, false, true);

                        characters.push_back(character);
                    }
                    else if (is_character && party.Has(character) && Engine::IsAlive(party[character]))
                    {
                        characters.push_back(party[character].Class);
                    }

                    if (characters.size() > 0)
                    {
                        for (auto i = 0; i < characters.size(); i++)
                        {
                            party[characters[i]].Add(Items::Defaults[item]);
                        }

                        text = (is_party ? "You" : party[characters[0]].Name) + " receive" + (is_party ? "" : "s") + " the " + Items::Defaults[item].Name + ".";

                        result = true;

                        internal_error = false;
                    }
                }
                else if (!Engine::IsAlive(party))
                {
                    text = Interface::DeathMessage(party);

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::LOSE_WEAPONS)
        {
            // variables
            // 0 - message to display
            // 1 - N items to be excluded (optional)
            if (condition.Variables.size() > 0 && !condition.Variables[0].empty())
            {
                auto excluded = Items::List();

                for (auto i = 1; i < condition.Variables.size(); i++)
                {
                    auto item = Item::Map(condition.Variables[i]);

                    if (item != Item::Type::NONE)
                    {
                        excluded.push_back(item);
                    }
                }

                if (Engine::IsAlive(party))
                {
                    for (auto i = 0; i < party.Count(); i++)
                    {
                        if (Engine::IsAlive(party[i]))
                        {
                            auto inventory = Items::Inventory();

                            for (auto item = 0; item < party[i].Items.size(); item++)
                            {
                                if (!party[i].Items[item].Is(Item::Property::WEAPON) || Items::Included(excluded, party[i].Items[item].Type))
                                {
                                    inventory.push_back(party[i].Items[item]);
                                }
                            }

                            party[i].Items = inventory;
                        }
                    }

                    text = condition.Variables[0];

                    result = true;
                }
                else
                {
                    text = Interface::DeathMessage(party);
                }
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::LOSE_ARMOUR)
        {
            // variables
            // 0 - message to display
            // 1 - N items to be excluded (optional)
            if (condition.Variables.size() > 0 && !condition.Variables[0].empty())
            {
                auto excluded = Items::List();

                for (auto i = 1; i < condition.Variables.size(); i++)
                {
                    auto item = Item::Map(condition.Variables[i]);

                    if (item != Item::Type::NONE)
                    {
                        excluded.push_back(item);
                    }
                }

                if (Engine::IsAlive(party))
                {
                    for (auto i = 0; i < party.Count(); i++)
                    {
                        if (Engine::IsAlive(party[i]))
                        {
                            auto inventory = Items::Inventory();

                            for (auto item = 0; item < party[i].Items.size(); item++)
                            {
                                if (!party[i].Items[item].Is(Item::Property::ARMOUR) || Items::Included(excluded, party[i].Items[item].Type))
                                {
                                    inventory.push_back(party[i].Items[item]);
                                }
                            }

                            party[i].Items = inventory;
                        }
                    }

                    text = condition.Variables[0];

                    result = true;
                }
                else
                {
                    text = Interface::DeathMessage(party);
                }
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::SELECT_PLAYER || condition.Type == Conditions::Type::CHOOSE_PLAYER)
        {
            // variables:
            // 0 - message to display
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                if (Engine::Count(party) > 1)
                {
                    auto message = condition.Variables[0].size() > 0 ? condition.Variables[0] : std::string(Conditions::TypeMapping[condition.Type]);

                    party.ChosenCharacter = Interface::SelectCharacter(graphics, background, party, message.c_str(), true, false, false, false, true);
                }
                else
                {
                    party.ChosenCharacter = Engine::FirstClass(party);
                }

                result = true;
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::SET_CHOSEN)
        {
            // variables:
            // 0 - player to set as CHOSEN
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    if (party.Has(character) && Engine::IsAlive(party[character]))
                    {
                        party.ChosenCharacter = character;

                        result = true;
                    }
                    else if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);
                    }
                }
                else
                {
                    internal_error = true;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::STAKE)
        {
            // variables
            // 0 - item to stake
            // 1 - asset (to item in #0)
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto item = Item::Map(condition.Variables[0]);

                auto asset = Asset::Map(condition.Variables[1]);

                result = Interface::StakeQuantity(graphics, background, party, item, asset);
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::COPY || condition.Type == Conditions::Type::SET)
        {
            // variables
            // 0 - destination variable
            // 1 - source variable / value
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto dst = condition.Variables[0];

                auto src = condition.Variables[1];

                if (!dst.empty() && !src.empty())
                {
                    if (party.IsPresent(src))
                    {
                        party.Set(dst, party.Get(src));
                    }
                    else
                    {
                        party.Set(dst, src);
                    }

                    result = true;
                }
                else
                {
                    internal_error = true;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::MATH)
        {
            // variables
            // 0 - operation (+, -, *)
            // 1 - first variable (destination)
            // 2 - second variable / value (source)
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto ops = condition.Variables[0];

                auto dst = condition.Variables[1];

                auto src = condition.Variables[2];

                if (!ops.empty() && !dst.empty() && !src.empty())
                {
                    party.Math(ops, dst, src);

                    result = true;
                }
                else
                {
                    internal_error = true;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::CHOOSE_NUMBER)
        {
            // variables
            // 0 - CHOOSE/VALUE
            // 1 - message to display if CHOOSE in #0
            // 2 - min if CHOOSE in #0
            // 3 - max if CHOOSE in #0
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto number = (Engine::ToUpper(condition.Variables[0]) != "CHOOSE");

                if (number)
                {
                    party.ChosenNumber = party.Number(condition.Variables[0]);

                    result = true;
                }
                else if (condition.Variables.size() > 3)
                {
                    auto min_number = party.Number(condition.Variables[2]);

                    auto max_number = party.Number(condition.Variables[3]);

                    party.ChosenNumber = Interface::GetNumber(graphics, background, condition.Variables[1].c_str(), min_number, max_number, Asset::Type::DICE1, Asset::Type::UP, Asset::Type::DOWN, false);

                    result = true;
                }
                else
                {
                    internal_error = true;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::ROLL)
        {
            internal_error = true;

            // variables
            // 0 - number of dice to roll
            // 1 - modifiers
            // 2 - variable to save results to
            // 3 - asset (CHOSEN / asset name)
            // 4 - asset (action)
            if (Engine::IsAlive(party) && condition.Variables.size() > 4)
            {
                auto roll = party.Number(condition.Variables[0]);

                auto mods = party.Number(condition.Variables[1]);

                auto variable = condition.Variables[2];

                auto is_character = (Engine::ToUpper(condition.Variables[3]) == "CHOSEN");

                if (!is_character || (is_character && party.ChosenCharacter != Character::Class::NONE && party.Has(party.ChosenCharacter) && Engine::IsAlive(party[party.ChosenCharacter])))
                {
                    auto asset = is_character ? party[party.ChosenCharacter].Asset : Asset::Map(condition.Variables[3]);

                    auto action = Asset::Map(condition.Variables[4]);

                    if (asset != Asset::Type::NONE && action != Asset::Type::NONE && !variable.empty())
                    {
                        auto rolls = Interface::Roll(graphics, background, asset, action, roll, mods);

                        party.Set(variable, rolls);

                        result = true;

                        internal_error = false;
                    }
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::IF)
        {
            // variables
            // 0 - operation (=, !=, <>, >, <, >=. <=)
            // 1 - first variable / value
            // 2 - second variable / value
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto ops = condition.Variables[0];

                auto first = condition.Variables[1];

                auto second = condition.Variables[2];

                if (!ops.empty() && !first.empty() && !second.empty())
                {
                    result = party.If(ops, first, second);
                }
                else
                {
                    internal_error = true;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::IF_MATH)
        {
            // variables
            // 0 - logical operation (=, !=, <>, >, <, >=. <=)
            // 1 - first variable
            // 2 - second variable
            // 3 - math operation (*, +, -)
            // 4 - first variable (destination)
            // 5 - second variable / value (source)
            if (Engine::IsAlive(party) && condition.Variables.size() > 4)
            {
                auto ops = condition.Variables[0];

                auto first = condition.Variables[1];

                auto second = condition.Variables[2];

                if (!ops.empty() && !first.empty() && !second.empty())
                {
                    result = party.If(ops, first, second);

                    if (result)
                    {
                        auto ops = condition.Variables[3];

                        auto first = condition.Variables[4];

                        auto second = condition.Variables[5];

                        party.Math(ops, first, second);
                    }
                }
                else
                {
                    internal_error = true;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::AND || condition.Type == Conditions::Type::OR)
        {
            // variables
            // 0 - 1st operation (=, !=, <>, >, <, >=. <=)
            // 1 - 1st variable (first)
            // 2 - 1st variable (second)
            // 3 - 2nd operation (=, !=, <>, >, <, >=. <=)
            // 4 - 2nd variable (first)
            // 5 - 2nd variable (second)
            if (Engine::IsAlive(party) && condition.Variables.size() > 5)
            {
                auto empty = 0;

                for (auto i = 0; i < condition.Variables.size(); i++)
                {
                    empty += (condition.Variables[i].empty() ? 1 : 0);
                }

                if (empty == 0)
                {
                    auto first = party.If(condition.Variables[0], condition.Variables[1], condition.Variables[2]);

                    auto second = party.If(condition.Variables[3], condition.Variables[4], condition.Variables[5]);

                    if (condition.Type == Conditions::Type::AND)
                    {
                        result = (first && second);
                    }
                    else if (condition.Type == Conditions::Type::OR)
                    {
                        result = (first || second);
                    }
                }
                else
                {
                    internal_error = true;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::ADD_TO_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - item
            // 2 - quantity or variable
            if (condition.Variables.size() > 2)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto item = Item::Map(condition.Variables[1]);

                auto quantity = party.Number(condition.Variables[2]);

                if (character != Character::Class::NONE && item != Item::Type::NONE && quantity != 0)
                {
                    if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else
                    {
                        if (!Engine::IsAlive(party[character]))
                        {
                            text = Engine::IsDead(party[character]);
                        }
                        else
                        {
                            if ((party[character].Quantity(item) + quantity) >= 0)
                            {
                                result = true;

                                party[character].Add(item, quantity);

                                if (quantity > 0)
                                {
                                    text = std::string("YOU GAINED ") + std::to_string(quantity) + " " + std::string(Item::TypeMapping[item]) + "!";
                                }
                                else
                                {
                                    text = std::string("YOU LOST ") + std::to_string(quantity) + " " + std::string(Item::TypeMapping[item]) + "!";
                                }
                            }
                            else
                            {
                                text = Engine::NotEnough(item);
                            }
                        }
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::SHOW_VARIABLES)
        {
            // variables:
            // 0 - border color
            // 1 - N variables to show
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto wrap = BloodSword::TileSize * 8;

                auto border = Engine::Color(condition.Variables[0]);

                auto message = std::string();

                for (auto i = 1; i < condition.Variables.size(); i++)
                {
                    auto value = party.Get(condition.Variables[i]);

                    if (!condition.Variables[i].empty())
                    {
                        if (!value.empty() && condition.Variables[i] != value)
                        {
                            if (!message.empty())
                            {
                                message += "\n";
                            }

                            if (Engine::ToUpper(condition.Variables[i]) != "PLAYER")
                            {
                                auto n = condition.Variables[i].find(" (");

                                if (n != std::string::npos)
                                {
                                    message += condition.Variables[i].substr(0, n);
                                }
                                else
                                {
                                    // show value
                                    message += condition.Variables[i];
                                }

                                message += ": " + value;
                            }
                            else
                            {
                                message += value;
                            }
                        }
                        else
                        {
                            // assume variable is a literal
                            message += condition.Variables[i];
                        }
                    }
                    else
                    {
                        // assume a blank is for spacing
                        message += "\n";
                    }
                }

                if (message.size() > 0)
                {
                    Interface::TextBox(graphics, background, message, border, wrap);
                }

                result = true;
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::TAKE_ITEMS)
        {
            // variables:
            // 0 - item (type)
            // 1 - limit
            // 2 - asset
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto item = Item::Map(condition.Variables[0]);

                auto limit = (Engine::ToUpper(condition.Variables[1]) == "UNLIMITED") ? Items::Unlimited : party.Number(condition.Variables[1]);

                auto asset = Asset::Map(condition.Variables[2]);

                if (item != Item::Type::NONE && asset != Asset::Type::NONE && (limit == Item::Unlimited || limit > 0))
                {
                    Interface::TakeItems(graphics, background, party, item, asset, limit);

                    result = true;
                }
                else
                {
                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::EAT_FOOD)
        {
            // variables:
            // 0 - asset
            // 1 - gain (ENDURANCE)
            // 2 - limit
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto asset = Asset::Map(condition.Variables[0]);

                auto gain = party.Number(condition.Variables[1]);

                auto limit = (Engine::ToUpper(condition.Variables[2]) == "UNLIMITED") ? Items::Unlimited : party.Number(condition.Variables[2]);

                if (asset != Asset::Type::NONE && (limit == Item::Unlimited || limit > 0) && gain > 0)
                {
                    Interface::EatFood(graphics, background, party, asset, gain, limit);

                    result = true;
                }
                else
                {
                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::SHOW_MESSAGE || condition.Type == Conditions::Type::TEXTBOX)
        {
            // variables:
            // 0 - message
            // 1 - border color (ACTIVE, HIGHLIGHT, INACTIVE, BACKGROUND, TRANSPARENT)
            if (condition.Variables.size() > 1 && !condition.Variables[0].empty())
            {
                auto border = Engine::Color(condition.Variables[1]);

                if (condition.Type == Conditions::Type::SHOW_MESSAGE)
                {
                    Interface::MessageBox(graphics, background, condition.Variables[0], border);
                }
                else if (condition.Type == Conditions::Type::TEXTBOX)
                {
                    Interface::TextBox(graphics, background, condition.Variables[0], border, BloodSword::TileSize * 8);
                }

                result = true;
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::COUNT_ITEMS)
        {
            // variables
            // 0 - ALL / player
            // 1 - item
            // 2 - quantity
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto item = Item::Map(condition.Variables[1]);

                auto required = party.Number(condition.Variables[2]);

                if ((is_party || (character != Character::Class::NONE && party.Has(character) && Engine::IsAlive(party[character]))) && item != Item::Type::NONE)
                {
                    auto quantity = is_party ? Engine::Quantity(party, item) : party[character].Quantity(item);

                    result = (quantity >= required);

                    if (!result)
                    {
                        text = Engine::NotEnough(item);
                    }
                }
                else if (character != Character::Class::NONE && item != Item::Type::NONE)
                {
                    if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else
                    {
                        text = Engine::IsDead(party[character]);
                    }
                }
                else
                {
                    internal_error = true;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::PSYCHIC_SPELL)
        {
            // variables
            // 0 - player
            // 1 - spell
            // 2 - target asset
            // 3 - difficulty (when sucessfully cast)
            // 4 - damage action asset
            // 5 - damage roll (on failure)
            // 6 - damage modifier (on failure)
            // 7 - ignore armour (TRUE/FALSE)
            // 8 - message (on failure)
            if (Engine::IsAlive(party) && condition.Variables.size() > 8)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto spell = Spells::Map(condition.Variables[1]);

                auto target = Asset::Map(condition.Variables[2]);

                auto difficulty = party.Number(condition.Variables[3]);

                auto dmg_act = Asset::Map(condition.Variables[4]);

                auto dmg_rol = party.Number(condition.Variables[5]);

                auto dmg_mod = party.Number(condition.Variables[6]);

                auto ignore_armour = (Engine::ToUpper(condition.Variables[7]) == "TRUE");

                if (character != Character::Class::NONE && party.Has(character) && Engine::IsAlive(party[character]) && spell != Spells::Type::NONE && target != Asset::Type::NONE && dmg_act != Asset::Type::NONE)
                {
                    party[character].CallToMind(spell);

                    if (Interface::Cast(graphics, background, Point(0, 0), graphics.Width, graphics.Height, party[character], spell, false))
                    {
                        auto rolls = Interface::Roll(graphics, background, target, Spells::Assets[spell], 2, 0);

                        if (rolls <= difficulty)
                        {
                            result = true;
                        }
                    }

                    if (!result)
                    {
                        // spellcasting unsuccessful!
                        auto message = condition.Variables[8].empty() ? Interface::GetText(Interface::MSG_CAST) : condition.Variables[8];

                        Interface::MessageBox(graphics, background, message, Color::Highlight);

                        if (dmg_rol > 0)
                        {
                            auto damage = Interface::Roll(graphics, background, party[character].Asset, dmg_act, dmg_rol, dmg_mod);

                            damage -= ignore_armour ? 0 : Engine::Score(party[character], Attribute::Type::ARMOUR);

                            if (damage > 0)
                            {
                                Engine::GainEndurance(party[character], -damage);
                            }

                            if (!Engine::IsAlive(party[character]))
                            {
                                std::string message = party[character].Name + " KILLED!";

                                Interface::MessageBox(graphics, background, message, Color::Highlight);
                            }
                        }

                        failed = true;
                    }
                }
                else if (!party.Has(character))
                {
                    text = Engine::NotInParty(character);
                }
                else if (!Engine::IsAlive(party[character]))
                {
                    text = Engine::IsDead(party[character]);
                }
                else
                {
                    internal_error = true;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::COLLECT)
        {
            // variables
            // 0 - item to stake
            // 1 - asset (to item in #0)
            // 2 - min collect
            // 3 - max collect
            if (Engine::IsAlive(party) && condition.Variables.size() > 3)
            {
                auto item = Item::Map(condition.Variables[0]);

                auto asset = Asset::Map(condition.Variables[1]);

                auto min_collect = party.Number(condition.Variables[2]);

                auto max_collect = party.Number(condition.Variables[3]);

                if (item != Item::Type::NONE && asset != Asset::Type::NONE)
                {
                    if (Engine::Quantity(party, item) >= min_collect)
                    {
                        result = Interface::Collect(graphics, background, party, item, asset, min_collect, max_collect);
                    }
                    else
                    {
                        text = Engine::NotEnough(item);
                    }
                }
                else
                {
                    internal_error = true;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::PERMANENT_ATTRIBUTE_GAIN)
        {
            // variables
            // 0 - number of attributes to update
            // 1 - gain (+/-)
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto attributes = party.Number(condition.Variables[0]);

                auto gain = party.Number(condition.Variables[1]);

                if (attributes > 0 && gain != 0)
                {
                    Interface::PermanentAttributeGain(graphics, background, party, attributes, gain);

                    result = true;
                }
                else
                {
                    internal_error = true;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::HAS_CHARGED_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - item
            // 1 - charge (item)
            // 2 - charges (minimum)
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto item = Item::Map(condition.Variables[0]);

                auto charge = Item::Map(condition.Variables[1]);

                auto charges = party.Number(condition.Variables[2]);

                if (item != Item::Type::NONE && charge != Item::Type::NONE)
                {
                    result = party.Has(item);

                    if (result)
                    {
                        if (party.HasChargedItem(item, charge, charges))
                        {
                            result = true;
                        }
                        else
                        {
                            text = Engine::NotEnough(charge);
                        }
                    }
                    else
                    {
                        text = Engine::NoItem(item);
                    }

                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::CHARGE || condition.Type == Conditions::Type::DISCHARGE)
        {
            internal_error = true;

            // variables
            // 0 - item
            // 1 - charge (item)
            // 2 - charges (minimum)
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto item = Item::Map(condition.Variables[0]);

                auto charge = Item::Map(condition.Variables[1]);

                auto charges = party.Number(condition.Variables[2]);

                if (item != Item::Type::NONE && charge != Item::Type::NONE && charges > 0)
                {
                    result = party.Has(item);

                    if (result)
                    {
                        if (condition.Type == Conditions::Type::DISCHARGE)
                        {
                            if (party.HasChargedItem(item, charge, charges))
                            {
                                party.AddCharge(item, charge, -charges);

                                result = true;
                            }
                            else
                            {
                                text = Engine::NotEnough(charge);
                            }
                        }
                        else if (condition.Type == Conditions::Type::CHARGE)
                        {
                            party.AddCharge(item, charge, charges);

                            result = true;
                        }
                    }
                    else
                    {
                        text = Engine::NoItem(item);
                    }

                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::KILL_PLAYER)
        {
            internal_error = true;

            // variables
            // 0 - player
            if (condition.Variables.size() > 0)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    result = party.Has(character);

                    if (!result)
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);

                        result = false;
                    }
                    else
                    {
                        // unalive player
                        party[character].Value(Attribute::Type::ENDURANCE, 0);

                        text = party[character].Name + " DIES.";
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::REMOVE_PLAYER)
        {
            internal_error = true;

            // variables
            // 0 - player
            if (condition.Variables.size() > 0)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    result = party.Has(character);

                    if (!result)
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);

                        result = false;
                    }
                    else
                    {
                        // remove player from party
                        party.Remove(character);

                        text = condition.Variables[1];
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::TAKE_ITEM || condition.Type == Conditions::Type::GET_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - item
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto item = Item::Map(condition.Variables[0]);

                if (item != Item::Type::NONE && Items::Found(item))
                {
                    if (Interface::CanReceive(party))
                    {
                        auto character = Engine::FirstClass(party);

                        if (Engine::Count(party) > 1)
                        {
                            std::string message = "WHO GETS THE " + Items::Defaults[item].Name + "?";

                            character = Interface::SelectCharacter(graphics, background, party, message.c_str(), true, false, false, false, true);
                        }

                        if (Interface::CanReceive(party[character]))
                        {
                            // preserve choice
                            party.ChosenCharacter = character;

                            party[character].Add(Items::Defaults[item]);

                            text = party[character].Name + " TAKES THE " + Item::TypeMapping[item] + ".";

                            result = true;
                        }
                        else
                        {
                            text = Interface::Text[Interface::MSG_FULL].Text;
                        }
                    }
                    else
                    {
                        text = Interface::Text[Interface::MSG_FULL].Text;
                    }

                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::GAIN_ATTRIBUTE)
        {
            internal_error = true;

            // variables
            // 0 - player / ALL
            // 1 - attribute
            // 2 - points to gain/lose
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto attribute = Attribute::Map(condition.Variables[1]);

                auto gain = party.Number(condition.Variables[2]);

                if ((is_party || (character != Character::Class::NONE && party.Has(character) && Engine::IsAlive(party[character]))) && attribute != Attribute::Type::NONE)
                {
                    if (is_party)
                    {
                        for (auto i = 0; i < party.Count(); i++)
                        {
                            if (Engine::IsAlive(party[i]))
                            {
                                auto value = party[i].Value(attribute) + gain;

                                party[i].Value(attribute, value);
                            }
                        }

                        auto plural = (Engine::Count(party) > 1);

                        text = (plural ? std::string("EVERYONE ") : "YOU ") + (gain < 0 ? "LOSE" : "GAIN") + (plural ? "S" : "") + " " + std::to_string(gain) + " " + Attribute::TypeMapping[attribute] + ".";
                    }
                    else
                    {
                        auto value = party[character].Value(attribute) + gain;

                        party[character].Value(attribute, value);

                        text = party[character].Name + " " + (gain < 0 ? "LOSE" : "GAIN") + " " + std::to_string(gain) + " " + Attribute::TypeMapping[attribute] + ".";
                    }

                    result = true;

                    internal_error = false;
                }
                else if (!party.Has(character))
                {
                    text = Engine::NotInParty(character);
                }
                else if (!Engine::IsAlive(party[character]))
                {
                    text = Engine::IsDead(party[character]);
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::ITEM_EFFECT)
        {
            // variables
            // 0 - item (type)
            if (condition.Variables.size() > 0)
            {
                auto item = Item::Map(condition.Variables[0]);

                if (item != Item::Type::NONE && party.ChosenCharacter == Character::Class::NONE)
                {
                    for (auto i = 0; i < party.Count(); i++)
                    {
                        if (Engine::IsAlive(party[i]) && party[i].Has(item))
                        {
                            party.ChosenCharacter = party[i].Class;

                            break;
                        }
                    }
                }

                if (party.ChosenCharacter != Character::Class::NONE && party.Has(party.ChosenCharacter) && Engine::IsAlive(party[party.ChosenCharacter]) && item != Item::Type::NONE)
                {
                    auto &chosen = party[party.ChosenCharacter];

                    if (chosen.Has(item))
                    {
                        Interface::ItemEffects(graphics, background, chosen, item);

                        result = true;
                    }
                    else
                    {
                        text = Engine::NoItem(item);
                    }
                }
                else if (party.ChosenCharacter == Character::Class::NONE || item == Item::Type::NONE)
                {
                    Conditions::InternalError(graphics, background, condition.Type);
                }
                else if (!party.Has(party.ChosenCharacter))
                {
                    text = Engine::NotInParty(party.ChosenCharacter);
                }
                else if (party.Has(party.ChosenCharacter) && !Engine::IsAlive(party[party.ChosenCharacter]))
                {
                    text = Engine::IsDead(party[party.ChosenCharacter]);
                }
            }
        }
        else if (condition.Type == Conditions::Type::DAMAGE_PLAYER)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - roll
            // 2 - modifier
            // 3 - ignore armour
            // 4 - display
            if (Engine::IsAlive(party) && condition.Variables.size() > 4)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto roll = party.Number(condition.Variables[1]);

                auto modifier = party.Number(condition.Variables[2]);

                auto ignore_armour = (Engine::ToUpper(condition.Variables[3]) == "TRUE");

                auto display = (Engine::ToUpper(condition.Variables[4]) == "TRUE");

                if (character != Character::Class::NONE)
                {
                    if (party.Has(character) && Engine::IsAlive(party[character]))
                    {
                        auto damage = Interface::DamagePlayer(graphics, background, party[character], roll, modifier, ignore_armour, false, display);

                        auto color = Color::Inactive;

                        auto message = std::string();

                        if (damage > 0)
                        {
                            message = party[character].Name + " LOSES " + std::to_string(damage) + " ENDURANCE";

                            color = Color::Highlight;
                        }
                        else
                        {
                            message = party[character].Name + " IS UNHARMED";
                        }

                        if (display)
                        {
                            Interface::MessageBox(graphics, background, text, color);
                        }
                        else
                        {
                            text = message;
                        }

                        result = true;
                    }
                    else if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);

                        result = false;
                    }

                    internal_error = false;
                }
            }
            else
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::LOSE_ENDURANCE || condition.Type == Conditions::Type::GAIN_ENDURANCE)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - endurance
            // 2 - ignore armour
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto endurance = party.Number(condition.Variables[1]);

                auto ignore_armour = (Engine::ToUpper(condition.Variables[2]) == "TRUE");

                if (character != Character::Class::NONE && endurance > 0)
                {
                    if (party.Has(character) && Engine::IsAlive(party[character]))
                    {
                        if (condition.Type == Conditions::Type::LOSE_ENDURANCE)
                        {
                            endurance -= ignore_armour ? 0 : Engine::Score(party[character], Attribute::Type::ARMOUR, false);

                            endurance = -endurance;
                        }

                        Engine::GainEndurance(party[character], endurance, false);

                        if (endurance < 0)
                        {
                            text = party[character].Name + " LOSES " + std::to_string(-endurance) + " ENDURANCE";
                        }
                        else if (endurance > 0)
                        {
                            text = party[character].Name + " GAINS " + std::to_string(endurance) + " ENDURANCE";
                        }
                        else
                        {
                            text = party[character].Name + " IS UNHARMED";
                        }

                        result = true;
                    }
                    else if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);
                    }

                    internal_error = false;
                }
            }
            else
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::FAIL_THEN_DIE)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - attribute
            // 2 - failure message
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto attribute = Attribute::Map(condition.Variables[1]);

                if (is_party || (character != Character::Class::NONE && attribute != Attribute::Type::NONE && !Book::IsUndefined(condition.Failure)))
                {
                    if (!is_party && !party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!is_party && !Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);
                    }
                    else
                    {
                        std::vector<Character::Class> characters = {};

                        if (!is_party)
                        {
                            characters = {character};
                        }
                        else
                        {
                            for (auto i = 0; i < party.Count(); i++)
                            {
                                if (Engine::IsAlive(party[i]))
                                {
                                    characters.push_back(party[i].Class);
                                }
                            }
                        }

                        for (auto i = 0; i < characters.size(); i++)
                        {
                            auto test = Interface::Test(graphics, background, party[characters[i]], attribute);

                            if (!test)
                            {
                                std::string message = party[characters[i]].Name + " " + condition.Variables[2];

                                Interface::MessageBox(graphics, background, message, Color::Highlight);

                                // unalive player
                                party[characters[i]].Value(Attribute::Type::ENDURANCE, 0);
                            }
                        }

                        result = true;
                    }

                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::SELECT_DICE)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - message
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    if (party.Has(character) && Engine::IsAlive(party[character]))
                    {
                        party.ChosenNumber = Interface::SelectDice(graphics, background, condition.Variables[1]);

                        result = true;
                    }
                    else if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);
                    }

                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::MULTIPLE_DICE)
        {
            // variables
            // 0 - player
            // 1 - message
            // 2 - number of dice to select
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto message = condition.Variables[1];

                auto number = party.Number(condition.Variables[2]);

                if (character != Character::Class::NONE)
                {
                    if (party.Has(character) && Engine::IsAlive(party[character]) && number >= 1 && number <= 6)
                    {
                        auto choice = Interface::SelectDice(graphics, background, message, number);

                        party.ChosenNumber = choice.Sum;

                        result = true;
                    }
                    else if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);
                    }
                }
                else
                {
                    internal_error = true;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::IS_ALIVE)
        {
            internal_error = true;

            // variables
            // 0 - player / ALL
            // 1 - message / text
            // 2 - display in textbox (TRUE / FALSE)
            // 3 - border color on display
            if (condition.Variables.size() > 2)
            {
                auto wrap = BloodSword::TileSize * 8;

                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto display = (Engine::ToUpper(condition.Variables[2]) == "TRUE");

                if (is_party || (character != Character::Class::NONE))
                {
                    if (is_party || party.Has(character))
                    {
                        result = is_party ? Engine::IsAlive(party) : Engine::IsAlive(party[character]);

                        if (result && condition.Variables.size() > 1 && !condition.Variables[1].empty())
                        {
                            if (display)
                            {
                                auto border = Engine::Color(condition.Variables[3]);

                                Interface::TextBox(graphics, background, condition.Variables[1], border, wrap);
                            }
                            else
                            {
                                text = condition.Variables[1];
                            }
                        }
                    }
                    else if (!is_party)
                    {
                        text = Engine::NotInParty(character);
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::GAIN_EXPERIENCE)
        {
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto experience = std::stoi(condition.Variables[0], nullptr, 10);

                if (experience > 0 && Engine::Count(party) != 0)
                {
                    auto share = experience / Engine::Count(party);

                    Engine::GainExperience(party, share);
                }

                result = true;
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::TEST_DISCHARGE_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - character (in party able to test)
            // 1 - item (in current section)
            // 2 - amount to discharge (on failure)
            // 3 - message on success (not discharged)
            // 4 - message on failure
            if (Engine::IsAlive(party) && condition.Variables.size() > 4)
            {
                auto current = Story::CurrentBook.Find(party.Location);

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto item = Item::Map(condition.Variables[1]);

                auto charge = Item::Requirements(item);

                auto discharge = party.Number(condition.Variables[2]);

                if (item != Item::Type::NONE && charge != Item::Type::NONE && discharge > 0)
                {
                    if (current >= 0 && current < Story::CurrentBook.Sections.size() && Story::CurrentBook.Sections[current].Items.size() > 0)
                    {
                        auto &section = Story::CurrentBook.Sections[current];

                        auto found = Items::Find(section.Items, item, charge);

                        auto available = (found != section.Items.end() && (*found).Quantity >= discharge);

                        if (available)
                        {
                            if (!party.Has(character) || !Engine::IsAlive(party[character]))
                            {
                                (*found).Quantity -= discharge;

                                (*found).Quantity = std::max(0, (*found).Quantity);

                                text = condition.Variables[4];
                            }
                            else
                            {
                                text = condition.Variables[3];
                            }

                            result = true;
                        }
                        else if (found == section.Items.end())
                        {
                            text = "NOTHING TO TEST HERE";
                        }
                        else
                        {
                            text = std::string(Item::TypeMapping[charge]) + " EXHAUSTED";
                        }
                    }
                    else if (current >= 0 && current < Story::CurrentBook.Sections.size())
                    {
                        text = "NOTHING TO TEST HERE";
                    }

                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::TASK)
        {
            internal_error = true;
            // variables
            // 0 - player / ALL
            // 1 - task
            // 2 - CHECK/SET
            // 3 - status
            // 4 - message on failure
            if (Engine::IsAlive(party) && condition.Variables.size() > 4)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto task = Engine::ToUpper(condition.Variables[1]);

                auto check = (Engine::ToUpper(condition.Variables[2]) == "CHECK");

                auto set = (Engine::ToUpper(condition.Variables[2]) == "SET");

                auto status = Task::Map(condition.Variables[3]);

                if ((is_party || (character != Character::Class::NONE)) && (check || set) && !task.empty() && status != Task::Status::NONE)
                {
                    if (is_party || party.Has(character))
                    {
                        if (set)
                        {
                            if (is_party)
                            {
                                Engine::TaskStatus(party, task, status);

                                result = true;
                            }
                            else if (Engine::IsAlive(party[character]))
                            {
                                Engine::TaskStatus(party, character, task, status);

                                result = true;
                            }
                            else
                            {
                                text = Engine::IsDead(party[character]);
                            }
                        }
                        else if (check)
                        {
                            result = is_party ? Engine::CheckTask(party, task, status) : Engine::CheckTask(party, character, task, status);

                            if (!result)
                            {
                                if (!is_party && !Engine::IsAlive(party[character]))
                                {
                                    text = Engine::IsDead(party[character]);
                                }
                                else
                                {
                                    text = condition.Variables[4];
                                }
                            }
                        }
                    }
                    else if (!is_party && !party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }

                    internal_error = false;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::LOSE_UNEQUIPPED)
        {
            // variables
            // 0 - player / ALL
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (is_party || character != Character::Class::NONE)
                {
                    std::vector<Character::Class> characters = {};

                    if (is_party || (party.Has(character) && Engine::IsAlive(party[character])))
                    {
                        if (is_party)
                        {
                            for (auto i = 0; i < party.Count(); i++)
                            {
                                if (Engine::IsAlive(party[i]))
                                {
                                    characters.push_back(party[i].Class);
                                }
                            }
                        }
                        else
                        {
                            characters = {character};
                        }

                        for (auto character_class : characters)
                        {
                            auto items = Items::Inventory();

                            auto &current = party[character_class];

                            // drop items that are either not equipped or can be dropped
                            for (auto i = 0; i < current.Items.size(); i++)
                            {
                                auto cannot_drop = current.Items[i].Is(Item::Property::CANNOT_DROP);

                                auto weapon = current.Items[i].HasAll({Item::Property::WEAPON, Item::Property::PRIMARY, Item::Property::EQUIPPED});

                                auto armour = current.Items[i].HasAll({Item::Property::ARMOUR, Item::Property::EQUIPPED});

                                if (cannot_drop || armour || weapon)
                                {
                                    items.push_back(current.Items[i]);
                                }
                            }

                            current.Items = items;
                        }

                        result = true;
                    }
                    else if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else
                    {
                        text = Engine::IsDead(party[character]);
                    }
                }
                else
                {
                    text = Interface::DeathMessage(party);
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::LOSE_EVERYTHING)
        {
            // variables
            // 0 - player / ALL
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (is_party || character != Character::Class::NONE)
                {
                    std::vector<Character::Class> characters = {};

                    if (is_party || (party.Has(character) && Engine::IsAlive(party[character])))
                    {
                        if (is_party)
                        {
                            for (auto i = 0; i < party.Count(); i++)
                            {
                                if (Engine::IsAlive(party[i]))
                                {
                                    characters.push_back(party[i].Class);
                                }
                            }
                        }
                        else
                        {
                            characters = {character};
                        }

                        for (auto character_class : characters)
                        {
                            auto items = Items::Inventory();

                            auto &current = party[character_class];

                            // drop items that are either not equipped or can be dropped
                            for (auto i = 0; i < current.Items.size(); i++)
                            {
                                auto cannot_drop = current.Items[i].Is(Item::Property::CANNOT_DROP) && current.Items[i].Contains == Item::Type::GOLD;

                                auto weapon = current.Items[i].HasAll({Item::Property::WEAPON, Item::Property::PRIMARY, Item::Property::EQUIPPED});

                                auto armour = current.Items[i].HasAll({Item::Property::ARMOUR, Item::Property::EQUIPPED});

                                if (cannot_drop || armour || weapon)
                                {
                                    items.push_back(current.Items[i]);
                                }
                            }

                            current.Items = items;
                        }

                        result = true;
                    }
                    else if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else
                    {
                        text = Engine::IsDead(party[character]);
                    }
                }
                else
                {
                    text = Interface::DeathMessage(party);
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::TAKE_FROM_LIST)
        {
            // variables
            // 0 - player
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    if (party.Has(character) && Engine::IsAlive(party[character]))
                    {
                        Items::List items = {};

                        for (auto i = 1; i < condition.Variables.size(); i++)
                        {
                            auto item = Item::Map(condition.Variables[i]);

                            if (item != Item::Type::NONE)
                            {
                                items.push_back(item);
                            }
                        }

                        if (items.size() > 0)
                        {
                            Interface::TakeFromInfiniteList(graphics, background, party, character, items, true);

                            result = true;
                        }
                    }
                    else if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);
                    }
                }
                else
                {
                    internal_error = true;
                }
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::CONFIRM)
        {
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                result = Interface::Confirm(graphics, background, condition.Variables[0], Color::Background, Color::Active, BloodSword::Border, Color::Active, true);
            }
            else if (!Engine::IsAlive(party))
            {
                text = Interface::DeathMessage(party);
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::PREVIOUS_LOCATION)
        {
            condition.Location = party.PreviousLocation;

            result = true;
        }

        // internal error
        if (internal_error)
        {
            Conditions::InternalError(graphics, background, condition.Type);
        }

        result = condition.Invert ? !result : result;

        Conditions::Log(condition, result, failed, text);

        return failed ? Conditions::Evaluation(result, failed, text) : Conditions::Evaluation(result, text);
    }
}

#endif