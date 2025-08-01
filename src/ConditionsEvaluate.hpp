#ifndef __CONDITIONS_EVALUATE_HPP__
#define __CONDITIONS_EVALUATE_HPP__

#include "loggers/Eval.hpp"

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
            if (condition.Variables.size() > 0 && !condition.Variables[0].empty())
            {
                text = condition.Variables[0];
            }
        }
        else if (condition.Type == Conditions::Type::IN_PARTY)
        {
            internal_error = true;

            // variables
            // 0 - player
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
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
        else if (condition.Type == Conditions::Type::ADD_CHARACTER)
        {
            internal_error = true;

            // variables
            // 0 - player
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto character = Character::Map(condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    result = Party::Characters.Has(character);

                    if (result)
                    {
                        if (!party.Has(character) && Character::OtherClass(character))
                        {
                            party.Add(Party::Characters[character]);
                        }

                        internal_error = false;
                    }
                }
            }
        }
        else if (condition.Type == Conditions::Type::IS_DEAD)
        {
            internal_error = true;

            // variables
            // 0 - player
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
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
            internal_error = true;

            // variables
            // 0 - player to check if they were chosen
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                result = (party.ChosenCharacter == character);

                if (!result)
                {
                    text = std::string("YOU HAVE CHOSEN A DIFFERENT PLAYER!");
                }

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::CHOSEN_NUMBER)
        {
            internal_error = true;

            // variables
            // 0 - number
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto number = party.Number(condition.Variables[0]);

                result = (party.ChosenNumber == number);

                if (!result)
                {
                    text = std::string("YOU HAVE CHOSEN A DIFFERENT NUMBER!");
                }

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::PARTY_HAS_ITEM)
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
        }
        else if (condition.Type == Conditions::Type::ITEM_IN_ENVIRONMENT || condition.Type == Conditions::Type::ITEM_IN_SECTION)
        {
            internal_error = true;

            // variables
            // 0 - item
            // 1 - message if found
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto current = Story::CurrentBook.Find(party.Location);

                auto item = Item::Map(condition.Variables[0]);

                if (item != Item::Type::NONE && current >= 0 && current < Story::CurrentBook.Sections.size())
                {
                    auto &section = Story::CurrentBook.Sections[current];

                    result = (section.Items.size() > 0 && Items::Find(section.Items, item) != section.Items.end());

                    if (result)
                    {
                        text = condition.Variables[1];
                    }

                    internal_error = false;
                }
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

                if (item != Item::Type::NONE && (is_party || character != Character::Class::NONE))
                {
                    if (is_party || (party.Has(character) && Engine::IsAlive(party[character])))
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
        }
        else if (condition.Type == Conditions::Type::HAS_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - player / ALL
            // 1 - item
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0], Interface::Mode::ITEMS);

                auto item = Item::Map(condition.Variables[1]);

                if (item != Item::Type::NONE && (is_party || character != Character::Class::NONE))
                {
                    if (is_party || (party.Has(character) && Engine::IsAlive(party[character])))
                    {
                        result = is_party ? party.Has(item) : party[character].Has(item);

                        if (!result)
                        {
                            text = Engine::NoItem(item);
                        }
                    }
                    else if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);
                    }
                    else if (party.Has(character) && !Engine::IsAlive(party[character]))
                    {
                        text = Engine::IsDead(party[character]);
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::HAS_ALL_ITEMS)
        {
            internal_error = true;

            // variables
            // 0 - player / ALL
            // 1 - N items
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto items = Items::List();

                for (auto i = 1; i < condition.Variables.size(); i++)
                {
                    auto item = Item::Map(condition.Variables[i]);

                    if (item != Item::Type::NONE)
                    {
                        items.push_back(item);
                    }
                }

                if ((items.size() > 0 && items.size() == (condition.Variables.size() - 1)) && (is_party || character != Character::Class::NONE))
                {
                    if (is_party || (party.Has(character) && Engine::IsAlive(party[character])))
                    {
                        result = is_party ? party.HasAll(items) : party[character].HasAll(items);

                        if (!result)
                        {
                            auto item_string = std::string();

                            for (auto item : items)
                            {
                                if (!item_string.empty())
                                {
                                    item_string += ", ";
                                }

                                item_string += Item::TypeMapping[item];
                            }

                            text = "YOU DO NOT HAVE ALL OF THE " + item_string;
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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::COUNT_ITEMS)
        {
            internal_error = true;

            // variables
            // 0 - player / ALL
            // 1 - item
            // 2 - number required
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto item = Item::Map(condition.Variables[1]);

                auto required = party.Number(condition.Variables[2]);

                if (item != Item::Type::NONE && (is_party || character != Character::Class::NONE) && required > 0)
                {
                    auto count = (is_party ? Engine::Count(party, item) : party[character].Count(item));

                    result = count >= required;

                    if (!result)
                    {
                        if (count == 0)
                        {
                            text = Engine::NoItem(item);
                        }
                        else
                        {
                            text = Engine::NotEnough(item);
                        }
                    }

                    internal_error = false;
                }
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
        }
        else if (condition.Type == Conditions::Type::IN_PARTY_WITH_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - item
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto item = Item::Map(condition.Variables[1]);

                if (character != Character::Class::NONE && item != Item::Type::NONE)
                {
                    result = party.Has(character) && Engine::IsAlive(party[character]) && party.Has(item);

                    if (!result)
                    {
                        if (!(party.Has(character)))
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
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
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
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
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
            if (Engine::IsAlive(party))
            {
                result = (party.LastBattle == Battle::Result::VICTORY);

                if (!result)
                {
                    text = "YOU WERE NOT VICTORIOUS IN THE LAST BATTLE!";
                }
            }
        }
        else if (condition.Type == Conditions::Type::BATTLE_FLEE)
        {
            if (Engine::IsAlive(party))
            {
                result = (party.LastBattle == Battle::Result::FLEE);

                if (!result)
                {
                    text = "YOU DID NOT FLEE IN YOUR LAST BATTLE!";
                }
            }
        }
        else if (condition.Type == Conditions::Type::BATTLE_ENTHRALMENT)
        {
            if (Engine::IsAlive(party))
            {
                result = (party.LastBattle == Battle::Result::ENTHRALLED);

                if (!result)
                {
                    text = "YOU HAVE NOT SUBDUED YOUR OPPONENTS!";
                }
            }
        }
        else if (condition.Type == Conditions::Type::BATTLE_VICTORY_OR_ENTHRALMENT)
        {
            if (Engine::IsAlive(party))
            {
                result = (party.LastBattle == Battle::Result::ENTHRALLED || party.LastBattle == Battle::Result::VICTORY);

                if (!result)
                {
                    text = "YOU WERE NOT VICTORIOUS IN THE LAST BATTLE!";
                }
            }
        }
        else if (condition.Type == Conditions::Type::HAVE_COLLEAGUES)
        {
            if (Engine::IsAlive(party))
            {
                result = (Engine::Count(party) > 1);

                if (result)
                {
                    // variables (when used in an event):
                    // 0 - text to display on success
                    if (condition.Variables.size() > 0)
                    {
                        text = condition.Variables[0];
                    }
                }
                else
                {
                    // when used in a choice
                    text = "YOU ARE ALONE!";
                }
            }
        }
        else if (condition.Type == Conditions::Type::SOLO)
        {
            if (Engine::IsAlive(party))
            {
                result = (Engine::Count(party) == 1);

                if (result)
                {
                    // variables (when used in an event):
                    // 0 - text to display on success
                    if (condition.Variables.size() > 0)
                    {
                        text = condition.Variables[0];
                    }
                }
                else
                {
                    text = "YOU ARE NOT ALONE!";
                }
            }
        }
        else if (condition.Type == Conditions::Type::GAIN_STATUS)
        {
            internal_error = true;

            // variables
            // 0 - player (or ALL)
            // 1 - status
            // 2 - TRUE/FALSE hide message (optional)
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
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
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
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
        }
        else if (condition.Type == Conditions::Type::LAST)
        {
            internal_error = true;

            // variables
            // 0 - player
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
        }
        else if (condition.Type == Conditions::Type::TEST_GAIN_STATUS)
        {
            internal_error = true;

            // variables
            // 0 - player (or ALL)
            // 1 - attribute
            // 2 - status gain on success
            // 3 - status gain on failure
            if (Engine::IsAlive(party) && condition.Variables.size() > 3)
            {
                auto attribute = Attribute::Map(condition.Variables[1]);

                auto status_success = Character::MapStatus(condition.Variables[2]);

                auto status_fail = Character::MapStatus(condition.Variables[3]);

                if (attribute != Attribute::Type::NONE && status_fail != Character::Status::NONE)
                {
                    if (Engine::ToUpper(condition.Variables[0]) == "ALL")
                    {
                        result = true;

                        for (auto character = 0; character < party.Count(); character++)
                        {
                            if (Engine::IsAlive(party[character]))
                            {
                                if (Interface::Test(graphics, background, party[character], attribute))
                                {
                                    if (status_success != Character::Status::NONE)
                                    {
                                        party[character].Add(status_success);
                                    }
                                }
                                else
                                {
                                    party[character].Add(status_fail);
                                }
                            }
                        }

                        internal_error = false;
                    }
                    else
                    {
                        auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                        if (character != Character::Class::NONE)
                        {
                            if (party.Has(character) && Engine::IsAlive(party[character]))
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
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
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
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
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
            if (Engine::IsAlive(party) && condition.Variables.size() > 5)
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
            if (Engine::IsAlive(party))
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
        }
        else if (condition.Type == Conditions::Type::KALUGEN_SCORE)
        {
            if (Engine::IsAlive(party))
            {
                // show score
                auto score = Engine::ScoreKalugenGame(party);

                text = "You scored: " + std::to_string(score) + ".";

                result = true;
            }
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
            internal_error = true;

            // variables:
            // 0 - player / ALL
            // 1 .. N - items to discard (type)
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
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

                            internal_error = false;
                        }
                    }
                    else if (!party.Has(character))
                    {
                        text = Engine::NotInParty(character);

                        internal_error = false;
                    }
                    else
                    {
                        text = Engine::IsDead(party[character]);

                        internal_error = false;
                    }
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
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

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
                    else if (first_or_last || (is_character && (!party.Has(character) || (party.Has(character) && !Engine::IsAlive(party[character]))) && next))
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
                    else if (selected || (is_character && (!party.Has(character) || (party.Has(character) && !Engine::IsAlive(party[character]))) && select_next))
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
                            if (party.Has(characters[i]) && Engine::IsAlive(party[characters[i]]))
                            {
                                party[characters[i]].Add(Items::Defaults[item]);
                            }
                        }

                        if (is_party)
                        {
                            text = "You receive the " + Items::Defaults[item].Name + ".";
                        }
                        else if (party.Has(characters[0]) && Engine::IsAlive(party[characters[0]]))
                        {
                            text = party[characters[0]].Name + " receives the " + Items::Defaults[item].Name + ".";
                        }

                        result = true;
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::LOSE_WEAPONS)
        {
            internal_error = true;

            // variables
            // 0 - message to display
            // 1 - N items to be excluded (optional)
            if (Engine::IsAlive(party) && condition.Variables.size() > 0 && !condition.Variables[0].empty())
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

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::LOSE_ARMOUR)
        {
            internal_error = true;

            // variables
            // 0 - message to display
            // 1 - N items to be excluded (optional)
            if (Engine::IsAlive(party) && condition.Variables.size() > 0 && !condition.Variables[0].empty())
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

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::SELECT_PLAYER || condition.Type == Conditions::Type::CHOOSE_PLAYER)
        {
            internal_error = true;

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

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::PLAYER_PAYS || condition.Type == Conditions::Type::CHARACTER_PAYS)
        {
            internal_error = true;

            // variables:
            // 0 - message to display
            // 1 - currency (currency, e.g. GOLD)
            // 2 - quantity (of the item above)
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto chosen = Character::Class::NONE;

                auto currency = Item::Map(condition.Variables[1]);

                auto quantity = party.Number(condition.Variables[2]);

                if (currency != Item::Type::NONE && quantity > 0)
                {
                    if (Engine::Count(party) > 1)
                    {
                        auto message = !condition.Variables[0].empty() ? condition.Variables[0] : ("WHO SHALL PAY " + std::to_string(quantity) + " " + std::string(Item::TypeMapping[currency]) + "?");

                        chosen = Interface::SelectCharacter(graphics, background, party, message.c_str(), true, false, false, false, true);
                    }
                    else
                    {
                        chosen = Engine::FirstClass(party);
                    }

                    if (chosen != Character::Class::NONE)
                    {
                        result = party[chosen].Quantity(currency) >= quantity;

                        if (!result)
                        {
                            text = Engine::NotEnough(currency);
                        }
                        else
                        {
                            party[chosen].Add(currency, -quantity);

                            party.ChosenCharacter = chosen;
                        }

                        internal_error = false;
                    }
                }
            }
        }
        else if (condition.Type == Conditions::Type::PLAYER_BUYS_ITEM || condition.Type == Conditions::Type::CHARACTER_BUYS_ITEM)
        {
            internal_error = true;

            // variables:
            // 0 - message to display
            // 1 - currency (currency, e.g. GOLD)
            // 2 - quantity (of the item above)
            // 3 - item to buy
            if (Engine::IsAlive(party) && condition.Variables.size() > 3)
            {
                auto chosen = Character::Class::NONE;

                auto currency = Item::Map(condition.Variables[1]);

                auto quantity = party.Number(condition.Variables[2]);

                auto item = Item::Map(condition.Variables[3]);

                if (currency != Item::Type::NONE && item != Item::Type::NONE && Items::Found(item) && quantity > 0)
                {
                    if (Engine::Count(party) > 1)
                    {
                        auto message = !condition.Variables[0].empty() ? condition.Variables[0] : (std::string("WHO BUYS THE ") + Item::TypeMapping[item] + " (" + std::to_string(quantity) + " " + Item::TypeMapping[currency] + ")?");

                        chosen = Interface::SelectCharacter(graphics, background, party, message.c_str(), true, false, false, false, true);
                    }
                    else
                    {
                        chosen = Engine::FirstClass(party);
                    }

                    if (chosen != Character::Class::NONE)
                    {
                        auto buy = party[chosen].Quantity(currency) >= quantity;

                        auto get = Interface::CanReceive(party[chosen]);

                        result = buy && get;

                        if (!result)
                        {
                            if (!buy)
                            {
                                text = Engine::NotEnough(currency);
                            }
                            else if (!get)
                            {
                                text = party[chosen].Name + "'s inventory is full.";
                            }
                            else
                            {
                                text = "You cannot buy the " + std::string(Item::TypeMapping[item]) + ".";
                            }
                        }
                        else
                        {
                            party[chosen].Add(Items::Defaults[item]);

                            party[chosen].Add(currency, -quantity);

                            party.ChosenCharacter = chosen;

                            text = party[chosen].Name + " buys the " + std::string(Item::TypeMapping[item]) + ".";
                        }

                        internal_error = false;
                    }
                }
            }
        }
        else if (condition.Type == Conditions::Type::SET_CHOSEN)
        {
            internal_error = true;

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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::STAKE)
        {
            internal_error = true;

            // variables
            // 0 - item to stake
            // 1 - asset (to item in #0)
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto item = Item::Map(condition.Variables[0]);

                auto asset = Asset::Map(condition.Variables[1]);

                result = Interface::StakeQuantity(graphics, background, party, item, asset);

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::COPY || condition.Type == Conditions::Type::SET)
        {
            internal_error = true;

            // variables
            // 0 - destination variable
            // 1 - source variable / value
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto dst = Engine::MapTokens(party, condition.Variables[0]);

                auto src = Engine::MapTokens(party, condition.Variables[1]);

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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::MATH)
        {
            internal_error = true;

            // variables
            // 0 - operation (+, -, *)
            // 1 - first variable (destination)
            // 2 - second variable / value (source)
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto ops = condition.Variables[0];

                auto dst = Engine::MapTokens(party, condition.Variables[1]);

                auto src = Engine::MapTokens(party, condition.Variables[2]);

                if (!ops.empty() && !dst.empty() && !src.empty())
                {
                    party.Math(ops, dst, src);

                    result = true;

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::CHOOSE_NUMBER)
        {
            internal_error = true;

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

                internal_error = false;
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
                        party.Set(variable, Interface::Roll(graphics, background, asset, action, roll, mods).Sum);

                        result = true;

                        internal_error = false;
                    }
                }
            }
        }
        else if (condition.Type == Conditions::Type::IF)
        {
            internal_error = true;

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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::IF_MATH)
        {
            internal_error = true;

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

                auto first = Engine::MapTokens(party, condition.Variables[1]);

                auto second = Engine::MapTokens(party, condition.Variables[2]);

                if (!ops.empty() && !first.empty() && !second.empty())
                {
                    result = party.If(ops, first, second);

                    if (result)
                    {
                        auto ops = condition.Variables[3];

                        auto first = Engine::MapTokens(party, condition.Variables[4]);

                        auto second = Engine::MapTokens(party, condition.Variables[5]);

                        party.Math(ops, first, second);
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::IF_TRUE_RETURN || condition.Type == Conditions::Type::IF_FALSE_RETURN)
        {
            internal_error = true;

            // variables
            // 0 - operation (=, !=, <>, >, <, >=. <=)
            // 1 - first variable / value
            // 2 - second variable / value
            // 3 - message on return
            // 4 - border
            if (Engine::IsAlive(party) && condition.Variables.size() > 4)
            {
                auto ops = condition.Variables[0];

                auto first = Engine::MapTokens(party, condition.Variables[1]);

                auto second = Engine::MapTokens(party, condition.Variables[2]);

                if (!ops.empty() && !first.empty() && !second.empty())
                {
                    auto check = party.If(ops, first, second);

                    if ((condition.Type == Conditions::Type::IF_TRUE_RETURN && check) || (condition.Type == Conditions::Type::IF_FALSE_RETURN && !check))
                    {
                        condition.Location = party.PreviousLocation;

                        // do not trigger previous events
                        party.Add(Character::Status::SKIP_EVENTS);

                        // get border
                        auto border = Engine::Color(condition.Variables[4]);

                        // show message
                        Interface::TextBox(graphics, background, condition.Variables[3], border, BloodSword::Wrap, true);

                        result = true;
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::AND || condition.Type == Conditions::Type::OR)
        {
            internal_error = true;

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
                    auto ops1 = condition.Variables[0];

                    auto dst1 = Engine::MapTokens(party, condition.Variables[1]);

                    auto src1 = Engine::MapTokens(party, condition.Variables[2]);

                    auto first = party.If(ops1, dst1, src1);

                    auto ops2 = condition.Variables[3];

                    auto dst2 = Engine::MapTokens(party, condition.Variables[4]);

                    auto src2 = Engine::MapTokens(party, condition.Variables[5]);

                    auto second = party.If(ops2, dst2, src2);

                    if (condition.Type == Conditions::Type::AND)
                    {
                        result = (first && second);
                    }
                    else if (condition.Type == Conditions::Type::OR)
                    {
                        result = (first || second);
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::ADD_TO_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - item
            // 2 - quantity or variable
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
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
            internal_error = true;

            // variables:
            // 0 - border color
            // 1 - N variables to show
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
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
                    Interface::TextBox(graphics, background, message, border, BloodSword::Wrap);
                }

                result = true;

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::TAKE_ITEMS)
        {
            internal_error = true;

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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::EAT_FOOD)
        {
            internal_error = true;

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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::SHOW_MESSAGE || condition.Type == Conditions::Type::TEXTBOX)
        {
            internal_error = true;

            // variables:
            // 0 - message
            // 1 - border color (ACTIVE, HIGHLIGHT, INACTIVE, BACKGROUND, TRANSPARENT)
            if (Engine::IsAlive(party) && condition.Variables.size() > 1 && !condition.Variables[0].empty())
            {
                auto border = Engine::Color(condition.Variables[1]);

                if (condition.Type == Conditions::Type::SHOW_MESSAGE)
                {
                    Interface::MessageBox(graphics, background, condition.Variables[0], border);
                }
                else if (condition.Type == Conditions::Type::TEXTBOX)
                {
                    Interface::TextBox(graphics, background, condition.Variables[0], border, BloodSword::OctaTile);
                }

                result = true;

                internal_error = false;
            }
        }
        else if (condition.Type == Conditions::Type::HAS_QUANTITY_OF_ITEM)
        {
            internal_error = true;

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

                if (item != Item::Type::NONE && (is_party || character != Character::Class::NONE))
                {
                    if (is_party || (party.Has(character) && Engine::IsAlive(party[character])))
                    {
                        auto quantity = is_party ? Engine::Quantity(party, item) : party[character].Quantity(item);

                        result = (quantity >= required);

                        if (!result)
                        {
                            text = Engine::NotEnough(item);
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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::PSYCHIC_SPELL)
        {
            internal_error = true;

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

                if (character != Character::Class::NONE && spell != Spells::Type::NONE && target != Asset::Type::NONE && dmg_act != Asset::Type::NONE && difficulty > 0 && dmg_rol > 0)
                {
                    if (party.Has(character) && Engine::IsAlive(party[character]))
                    {
                        party[character].CallToMind(spell);

                        if (Interface::Cast(graphics, background, Point(0, 0), graphics.Width, graphics.Height, party[character], spell, false))
                        {
                            if (Interface::Roll(graphics, background, target, Spells::Assets[spell], 2, 0).Sum <= difficulty)
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
                                auto damage = Interface::Roll(graphics, background, party[character].Asset, dmg_act, dmg_rol, dmg_mod).Sum;

                                damage -= ignore_armour ? 0 : Engine::Score(party[character], Attribute::Type::ARMOUR);

                                if (damage > 0)
                                {
                                    Engine::GainEndurance(party[character], -damage);
                                }

                                if (!Engine::IsAlive(party[character]))
                                {
                                    std::string message = party[character].Name + " KILLED!";

                                    Interface::MessageBox(graphics, background, message, Color::Highlight);

                                    Interface::Resurrect(graphics, background, party, party[character]);
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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::PSYCHIC_SPELL_GAIN_ATTRIBUTE)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - spell
            // 2 - target asset
            // 3 - difficulty (when sucessfully cast)
            // 4 - attribute
            // 5 - gain/loss
            // 6 - message (on failure)
            if (Engine::IsAlive(party) && condition.Variables.size() > 6)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto spell = Spells::Map(condition.Variables[1]);

                auto target = Asset::Map(condition.Variables[2]);

                auto difficulty = party.Number(condition.Variables[3]);

                auto attribute = Attribute::Map(condition.Variables[4]);

                auto gain = party.Number(condition.Variables[5]);

                if (character != Character::Class::NONE && spell != Spells::Type::NONE && target != Asset::Type::NONE && attribute != Attribute::Type::NONE && difficulty > 0 && gain != 0)
                {
                    if (party.Has(character) && Engine::IsAlive(party[character]))
                    {
                        party[character].CallToMind(spell);

                        if (Interface::Cast(graphics, background, Point(0, 0), graphics.Width, graphics.Height, party[character], spell, false))
                        {
                            result = Interface::Roll(graphics, background, target, Spells::Assets[spell], 2, 0).Sum <= difficulty;
                        }

                        if (!result)
                        {
                            // spellcasting unsuccessful!
                            auto message = condition.Variables[6].empty() ? Interface::GetText(Interface::MSG_CAST) : condition.Variables[6];

                            Interface::MessageBox(graphics, background, message, Color::Highlight);

                            auto value = party[character].Value(attribute) + gain;

                            party[character].Value(attribute, value);

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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::COLLECT)
        {
            internal_error = true;

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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::PERMANENT_ATTRIBUTE_GAIN)
        {
            internal_error = true;

            // variables
            // 0 - player / all
            // 1 - number of attributes to update
            // 2 - gain (+/-)
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto attributes = party.Number(condition.Variables[1]);

                auto gain = party.Number(condition.Variables[2]);

                if (attributes > 0 && gain != 0 && (is_party || character != Character::Class::NONE))
                {
                    if (is_party || (party.Has(character) && Engine::IsAlive(party[character])))
                    {
                        if (is_party)
                        {
                            Interface::PermanentAttributeGain(graphics, background, party, attributes, gain);
                        }
                        else
                        {
                            Interface::PermanentAttributeGain(graphics, background, party[character], attributes, gain);
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
        }
        else if (condition.Type == Conditions::Type::KILL_PLAYER)
        {
            internal_error = true;

            // variables
            // 0 - player
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
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

                        Interface::Resurrect(graphics, background, party, party[character]);

                        if (!Engine::IsAlive(party[character]))
                        {
                            text = party[character].Name + " DIES.";
                        }
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::KILL_OTHERS)
        {
            internal_error = true;

            // variables
            // 0 - player (not killed)
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    result = party.Has(character) && Engine::IsAlive(party[character]);

                    if (result)
                    {
                        for (auto others = 0; others < party.Count(); others++)
                        {
                            if (Engine::IsAlive(party[others]) && party[others].Class != character)
                            {
                                // unalive player
                                party[others].Value(Attribute::Type::ENDURANCE, 0);
                            }
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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::KILL_ALL_WITH_STATUS)
        {
            internal_error = true;

            // variables
            // 0 - status
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto status = Character::MapStatus(condition.Variables[0]);

                if (status != Character::Status::NONE)
                {
                    for (auto character = 0; character < party.Count(); character++)
                    {
                        if (Engine::IsAlive(party[character]) && party[character].Has(status))
                        {
                            // unalive player
                            party[character].Value(Attribute::Type::ENDURANCE, 0);

                            Interface::Resurrect(graphics, background, party, party[character]);
                        }
                    }

                    internal_error = false;

                    result = true;
                }
            }
        }
        else if (condition.Type == Conditions::Type::REMOVE_PLAYER)
        {
            internal_error = true;

            // variables
            // 0 - player
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
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

                if (attribute != Attribute::Type::NONE && (is_party || character != Character::Class::NONE))
                {
                    if (is_party || (party.Has(character) && Engine::IsAlive(party[character])))
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

                            text = std::string(plural ? "EVERYONE " : "YOU ") + (gain < 0 ? "LOSE" : "GAIN") + (plural ? "S" : "") + " " + std::to_string(std::abs(gain)) + " " + Attribute::TypeMapping[attribute] + ".";
                        }
                        else
                        {
                            auto value = party[character].Value(attribute) + gain;

                            party[character].Value(attribute, value);

                            text = party[character].Name + " " + (gain < 0 ? "LOSE" : "GAIN") + " " + std::to_string(std::abs(gain)) + " " + Attribute::TypeMapping[attribute] + ".";
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
        }
        else if (condition.Type == Conditions::Type::ALL_WITH_STATUS_GAIN_ATTRIBUTE)
        {
            internal_error = true;

            // variables
            // 0 - status
            // 1 - attribute
            // 2 - points to gain/lose
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto status = Character::MapStatus(condition.Variables[0]);

                auto attribute = Attribute::Map(condition.Variables[1]);

                auto gain = party.Number(condition.Variables[2]);

                if (attribute != Attribute::Type::NONE && status != Character::Status::NONE && gain != 0)
                {
                    for (auto character = 0; character < party.Count(); character++)
                    {
                        if (Engine::IsAlive(party[character]) && party[character].Has(status))
                        {
                            auto value = party[character].Value(attribute) + gain;

                            party[character].Value(attribute, value);
                        }
                    }

                    result = true;

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::ALL_WITH_STATUS_MODIFY_ATTRIBUTE)
        {
            internal_error = true;

            // variables
            // 0 - status
            // 1 - attribute
            // 2 - points to modify
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto status = Character::MapStatus(condition.Variables[0]);

                auto attribute = Attribute::Map(condition.Variables[1]);

                auto gain = party.Number(condition.Variables[2]);

                if (attribute != Attribute::Type::NONE && status != Character::Status::NONE && gain != 0)
                {
                    for (auto character = 0; character < party.Count(); character++)
                    {
                        if (Engine::IsAlive(party[character]) && party[character].Has(status))
                        {
                            Engine::ModifyAttribute(party[character], attribute, gain);
                        }
                    }

                    result = true;

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::MODIFY_ATTRIBUTE)
        {
            internal_error = true;

            // variables
            // 0 - player / ALL
            // 1 - attribute
            // 2 - points to modify
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto attribute = Attribute::Map(condition.Variables[1]);

                auto gain = party.Number(condition.Variables[2]);

                if (attribute != Attribute::Type::NONE && (is_party || character != Character::Class::NONE))
                {
                    if (is_party || (party.Has(character) && Engine::IsAlive(party[character])))
                    {
                        if (is_party)
                        {
                            Engine::ModifyAttribute(party, attribute, gain);

                            auto plural = (Engine::Count(party) > 1);

                            text = std::string(plural ? "EVERYONE " : "YOU ") + "PERMANENTLY " + (gain < 0 ? "LOSE" : "GAIN") + (plural ? "S" : "") + " " + std::to_string(std::abs(gain)) + " " + Attribute::TypeMapping[attribute] + ".";
                        }
                        else
                        {
                            Engine::ModifyAttribute(party[character], attribute, gain);

                            text = party[character].Name + " PERMANENTLY " + (gain < 0 ? "LOSE" : "GAIN") + " " + std::to_string(std::abs(gain)) + " " + Attribute::TypeMapping[attribute] + ".";
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
        }
        else if (condition.Type == Conditions::Type::MAXIMISE_ATTRIBUTE || condition.Type == Conditions::Type::MAXIMIZE_ATTRIBUTE)
        {
            internal_error = true;

            // variables
            // 0 - player / ALL
            // 1 - attribute
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto attribute = Attribute::Map(condition.Variables[1]);

                if (attribute != Attribute::Type::NONE && (is_party || character != Character::Class::NONE))
                {
                    if (is_party || (party.Has(character) && Engine::IsAlive(party[character])))
                    {
                        if (is_party)
                        {
                            if (condition.Type == Conditions::Type::MAXIMISE_ATTRIBUTE)
                            {
                                Engine::MaximiseAttribute(party, attribute);
                            }
                            else
                            {
                                Engine::MaximizeAttribute(party, attribute);
                            }

                            auto plural = (Engine::Count(party) > 1);

                            text = std::string(plural ? "EVERYONE's " : "YOUR ") + Attribute::TypeMapping[attribute] + " IS RESTORED TO MAXIMUM";
                        }
                        else
                        {
                            if (condition.Type == Conditions::Type::MAXIMISE_ATTRIBUTE)
                            {
                                Engine::MaximiseAttribute(party[character], attribute);
                            }
                            else
                            {
                                Engine::MaximizeAttribute(party[character], attribute);
                            }

                            text = party[character].Name + "'s " + Attribute::TypeMapping[attribute] + " IS RESTORED TO MAXIMUM";
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
        }
        else if (condition.Type == Conditions::Type::ITEM_EFFECT)
        {
            internal_error = true;

            // variables
            // 0 - item (type)
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto item = Item::Map(condition.Variables[0]);

                if (item != Item::Type::NONE)
                {
                    if (party.ChosenCharacter == Character::Class::NONE)
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

                    if (party.ChosenCharacter != Character::Class::NONE)
                    {
                        if (party.Has(party.ChosenCharacter) && Engine::IsAlive(party[party.ChosenCharacter]))
                        {
                            auto &chosen = party[party.ChosenCharacter];

                            if (chosen.Has(item))
                            {
                                Interface::ItemEffects(graphics, background, party, chosen, item);

                                result = true;
                            }
                            else
                            {
                                text = Engine::NoItem(item);
                            }
                        }
                        else if (!party.Has(party.ChosenCharacter))
                        {
                            text = Engine::NotInParty(party.ChosenCharacter);
                        }
                        else if (party.Has(party.ChosenCharacter) && !Engine::IsAlive(party[party.ChosenCharacter]))
                        {
                            text = Engine::IsDead(party[party.ChosenCharacter]);
                        }

                        internal_error = false;
                    }
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

                            Interface::Resurrect(graphics, background, party, party[character]);
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
        }
        else if (condition.Type == Conditions::Type::DAMAGE_PARTY)
        {
            internal_error = true;

            // variables
            // 0 - targets
            // 1 - rounds
            // 2 - roll
            // 3 - modifier
            // 4 - ignore armour
            // 5 - display
            if (Engine::IsAlive(party) && condition.Variables.size() > 5)
            {
                auto targets = party.Number(condition.Variables[0]);

                auto rounds = party.Number(condition.Variables[1]);

                auto roll = party.Number(condition.Variables[2]);

                auto modifier = party.Number(condition.Variables[3]);

                auto ignore_armour = Engine::ToUpper(condition.Variables[4]) == "TRUE";

                auto display = Engine::ToUpper(condition.Variables[5]) == "TRUE";

                if (targets > 0 && roll > 0)
                {
                    Interface::DamageParty(graphics, background, party, targets, rounds, roll, modifier, ignore_armour, false, display);

                    result = true;

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::DAMAGE_EVERYONE)
        {
            internal_error = true;

            // variables
            // 0 - roll
            // 1 - modifier
            // 2 - ignore armour
            // 3 - display
            if (Engine::IsAlive(party) && condition.Variables.size() > 3)
            {
                auto roll = party.Number(condition.Variables[0]);

                auto modifier = party.Number(condition.Variables[1]);

                auto ignore_armour = Engine::ToUpper(condition.Variables[2]) == "TRUE";

                auto display = Engine::ToUpper(condition.Variables[3]) == "TRUE";

                if (roll > 0)
                {
                    for (auto character = 0; character < party.Count(); character++)
                    {
                        if (Engine::IsAlive(party[character]))
                        {
                            Interface::DamagePlayer(graphics, background, party[character], roll, modifier, ignore_armour, false, display);
                        }
                    }

                    result = true;

                    internal_error = false;
                }
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
                            text = party[character].Name + " LOSES " + std::to_string(endurance) + " ENDURANCE";

                            if (!Engine::IsAlive(party[character]))
                            {
                                Interface::Resurrect(graphics, background, party, party[character]);
                            }
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

                if ((is_party || character != Character::Class::NONE) && attribute != Attribute::Type::NONE)
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
                        auto characters = std::vector<Character::Class>();

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

                                if (!Engine::IsAlive(party[characters[i]]))
                                {
                                    Interface::Resurrect(graphics, background, party, party[characters[i]]);
                                }
                            }
                        }
                    }

                    result = true;

                    internal_error = false;
                }
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
        }
        else if (condition.Type == Conditions::Type::MULTIPLE_DICE)
        {
            internal_error = true;

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

                    internal_error = false;
                }
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
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto display = (Engine::ToUpper(condition.Variables[2]) == "TRUE");

                if (is_party || character != Character::Class::NONE)
                {
                    if (is_party || party.Has(character))
                    {
                        result = is_party ? Engine::IsAlive(party) : Engine::IsAlive(party[character]);

                        if (result && condition.Variables.size() > 2 && !condition.Variables[1].empty())
                        {
                            if (display)
                            {
                                auto border = Engine::Color(condition.Variables[3]);

                                Interface::TextBox(graphics, background, condition.Variables[1], border, BloodSword::Wrap);
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
            internal_error = true;

            // variables
            // 0 - experience to gain
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto experience = std::stoi(condition.Variables[0], nullptr, 10);

                if (experience > 0 && Engine::Count(party) != 0)
                {
                    auto share = experience / Engine::Count(party);

                    Engine::GainExperience(party, share);
                }

                result = true;

                internal_error = false;
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
        }
        else if (condition.Type == Conditions::Type::LOSE_UNEQUIPPED)
        {
            internal_error = true;

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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::LOSE_EVERYTHING)
        {
            internal_error = true;

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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::LOSE_EVERYTHING_EXCEPT_WEAPONS)
        {
            internal_error = true;

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

                            // drop all items except weapons
                            for (auto i = 0; i < current.Items.size(); i++)
                            {
                                if (current.Items[i].HasAll({Item::Property::WEAPON, Item::Property::PRIMARY, Item::Property::EQUIPPED}))
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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::TAKE_FROM_LIST)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - N items
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

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::GAUNTLET)
        {
            internal_error = true;

            // variables
            // 0 - rounds
            // 1 - rolls
            // 2 - roll modifier
            // 3 - difficulty
            // 4 - damage
            // 5 - damage modifier
            // 6 - asset
            // 7 - damage prefix
            // 8 - success message
            // 9 - failure message
            if (Engine::IsAlive(party) && condition.Variables.size() > 9)
            {
                Interface::Gauntlet gauntlet;

                gauntlet.Rounds = party.Number(condition.Variables[0]);

                gauntlet.Rolls = party.Number(condition.Variables[1]);

                gauntlet.RollModifier = party.Number(condition.Variables[2]);

                gauntlet.Difficulty = party.Number(condition.Variables[3]);

                gauntlet.Damage = party.Number(condition.Variables[4]);

                gauntlet.DamageModifier = party.Number(condition.Variables[5]);

                auto asset = Asset::Map(condition.Variables[6]);

                auto prefix = condition.Variables[7];

                if (gauntlet.Rounds > 0 && gauntlet.Rolls > 0 && gauntlet.Difficulty > 0 && gauntlet.Damage > 0 && asset != Asset::Type::NONE && !prefix.empty())
                {
                    result = Interface::RunGauntlet(graphics, background, party, gauntlet, asset, prefix);

                    if (result)
                    {
                        if (!condition.Variables[8].empty())
                        {
                            Interface::TextBox(graphics, background, condition.Variables[8], Color::Active, BloodSword::Wrap, true);
                        }
                    }
                    else
                    {
                        if (Engine::IsAlive(party))
                        {
                            failed = true;

                            if (!condition.Variables[9].empty())
                            {
                                Interface::TextBox(graphics, background, condition.Variables[9], Color::Inactive, BloodSword::Wrap, true);
                            }
                        }
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::ORDER_FRONT || condition.Type == Conditions::Type::ORDER_FIRST)
        {
            internal_error = true;

            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    if (party.Has(character) && Engine::IsAlive(party[character]))
                    {
                        auto new_order = std::vector<Character::Base>();

                        // put selected CHARACTER at the front
                        new_order.push_back(party[character]);

                        for (auto i = 0; i < party.Count(); i++)
                        {
                            if (party[i].Class != character)
                            {
                                new_order.push_back(party[i]);
                            }
                        }

                        party.Clear();

                        for (auto i = 0; i < new_order.size(); i++)
                        {
                            party.Add(new_order[i]);
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
        }
        else if (condition.Type == Conditions::Type::ORDER_BACK || condition.Type == Conditions::Type::ORDER_LAST)
        {
            internal_error = true;

            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    if (party.Has(character) && Engine::IsAlive(party[character]))
                    {
                        auto new_order = std::vector<Character::Base>();

                        for (auto i = 0; i < party.Count(); i++)
                        {
                            if (party[i].Class != character)
                            {
                                new_order.push_back(party[i]);
                            }
                        }

                        // put selected CHARACTER at the back
                        new_order.push_back(party[character]);

                        party.Clear();

                        for (auto i = 0; i < new_order.size(); i++)
                        {
                            party.Add(new_order[i]);
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
        }
        else if (condition.Type == Conditions::Type::DAMAGE_ON_FAIL)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - attribute
            // 2 - damage
            // 3 - message on success
            // 4 - message on fail
            if (Engine::IsAlive(party) && condition.Variables.size() > 4)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto attribute = Attribute::Map(condition.Variables[1]);

                if (character != Character::Class::NONE && attribute != Attribute::Type::NONE)
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

                            auto endurance = party.Number(condition.Variables[2]);

                            Engine::GainEndurance(party[character], -endurance, false);

                            text = condition.Variables[4];
                        }
                        else
                        {
                            text = condition.Variables[3];

                            result = true;
                        }
                    }

                    if (!Engine::IsAlive(party[character]))
                    {
                        Interface::Resurrect(graphics, background, party, party[character]);
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::DAMAGE_PARTY_ON_FAIL)
        {
            internal_error = true;

            // variables
            // 0 - attribute
            // 1 - damage roll
            // 2 - damage modifier
            // 3 - ignore armour
            // 4 - message on success
            // 5 - message on fail
            // 6 - display
            if (Engine::IsAlive(party) && condition.Variables.size() > 6)
            {
                auto attribute = Attribute::Map(condition.Variables[0]);

                auto damage = party.Number(condition.Variables[1]);

                auto modifier = party.Number(condition.Variables[2]);

                auto ignore_armour = (Engine::ToUpper(condition.Variables[3]) == "TRUE");

                auto display = (Engine::ToUpper(condition.Variables[6]) == "TRUE");

                if (attribute != Attribute::Type::NONE && damage > 0)
                {
                    for (auto character = 0; character < party.Count(); character++)
                    {
                        if (Engine::IsAlive(party[character]))
                        {
                            auto test = Interface::Test(graphics, background, party[character], attribute);

                            if (!test)
                            {
                                if (!condition.Variables[5].empty())
                                {
                                    if (display)
                                    {
                                        Interface::MessageBox(graphics, background, party[character].Name + " " + condition.Variables[5], Color::Highlight);
                                    }
                                    else
                                    {
                                        text += (party[character].Name + " " + condition.Variables[5] + "\n");
                                    }
                                }

                                Interface::DamagePlayer(graphics, background, party[character], damage, modifier, ignore_armour, false, display);

                                if (!Engine::IsAlive(party[character]))
                                {
                                    Interface::Resurrect(graphics, background, party, party[character]);
                                }
                            }
                            else if (!condition.Variables[4].empty())
                            {
                                if (display)
                                {
                                    Interface::MessageBox(graphics, background, party[character].Name + " " + condition.Variables[4], Color::Active);
                                }
                                else
                                {
                                    text += (party[character].Name + " " + condition.Variables[4] + "\n");
                                }
                            }
                        }
                    }

                    result = Engine::IsAlive(party);

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::COMBAT_TASK || condition.Type == Conditions::Type::COMBAT_MISSION)
        {
            internal_error = true;

            // variables
            // 0 - status to apply (usually AWAY)
            // 1 - limit (to number of players doing the task)
            // 2 - mission rounds
            // 3 - character selection prompt (message)
            if (Engine::IsAlive(party) && condition.Variables.size() > 3)
            {
                auto status = Character::MapStatus(condition.Variables[0]);

                auto limit = party.Number(condition.Variables[1]);

                auto rounds = party.Number(condition.Variables[2]);

                auto mission = condition.Variables[3];

                if (status != Character::Status::NONE && limit > 0 && rounds > 1 && !mission.empty())
                {
                    // remove status from entire party
                    party.Remove(status);

                    if (Engine::Count(party) > 1 && (limit < Engine::Count(party) || (limit > 1 && (limit - 1) < Engine::Count(party))))
                    {
                        limit = std::min(limit, Engine::Count(party) - 1);

                        Interface::SelectMultiple(graphics, background, party, mission.c_str(), status, limit, true);

                        for (auto i = 0; i < party.Count(); i++)
                        {
                            if (Engine::IsAlive(party[i]) && party[i].Has(status))
                            {
                                party[i].Status[status] = rounds;
                            }
                        }
                    }

                    result = true;

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::GAIN_ARMOUR)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - gain
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto gain = party.Number(condition.Variables[1]);

                if ((is_party || character != Character::Class::NONE) && gain != 0)
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
                        auto characters = std::vector<Character::Class>();

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
                            characters.push_back(party[character].Class);
                        }

                        for (auto character : characters)
                        {
                            if (Engine::IsAlive(party[character]) && party[character].Items.size() > 0)
                            {
                                for (auto item = 0; item < party[character].Items.size(); item++)
                                {
                                    if (party[character].Items[item].HasAll({Item::Property::ARMOUR, Item::Property::EQUIPPED}) && party[character].Items[item].Has(Attribute::Type::ARMOUR))
                                    {
                                        auto armour = party[character].Items[item].Attributes[Attribute::Type::ARMOUR] + gain;

                                        armour = std::max(0, armour);

                                        party[character].Items[item].Attributes[Attribute::Type::ARMOUR] = armour;
                                    }
                                }
                            }
                        }

                        result = true;
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::CHAOS_EFFECT)
        {
            internal_error = true;

            // variables
            // 0 - player
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if (character != Character::Class::NONE)
                {
                    result = party.Has(character) && Engine::IsAlive(party[character]);

                    if (result)
                    {
                        Interface::ChaosEffect(graphics, background, party[character]);
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
        }
        else if (condition.Type == Conditions::Type::LOSE_ENDURANCE_OR_DROP_ITEM)
        {
            internal_error = true;

            if (Engine::IsAlive(party))
            {
                auto characters = std::vector<Character::Class>();

                for (auto character = 0; character < party.Count(); character++)
                {
                    if (Engine::IsAlive(party[character]))
                    {
                        characters.push_back(party[character].Class);
                    }
                }

                if (characters.size() > 0)
                {
                    result = true;

                    for (auto character : characters)
                    {
                        result &= Interface::Test(graphics, background, party[character], Attribute::Type::PSYCHIC_ABILITY);

                        if (!result)
                        {
                            Interface::LoseEnduranceOrDropItem(graphics, background, party[character]);
                        }
                    }

                    if (!result && Book::IsDefined(condition.Failure))
                    {
                        failed = true;
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::IS_WOUNDED)
        {
            internal_error = true;

            // variables
            // 0 - player / all
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                if ((is_party || character != Character::Class::NONE))
                {
                    if (is_party)
                    {
                        result = !Engine::Healed(party);

                        if (!result)
                        {
                            text = "NO ONE IS WOUNDED";
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
                        result = !Engine::Healed(party[character]);

                        if (!result)
                        {
                            text = party[character].Name + " IS NOT WOUNDED";
                        }
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::MUST_TAKE_ITEM || condition.Type == Conditions::Type::MUST_GET_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - item to take
            // 1 - item (if present, then must take item in #0)
            if (Engine::IsAlive(party) && condition.Variables.size() > 1)
            {
                auto item = Item::Map(condition.Variables[0]);

                auto required = Item::Map(condition.Variables[1]);

                if (item != Item::Type::NONE)
                {
                    auto take = false;

                    if (required != Item::Type::NONE)
                    {
                        if (party.Has(required))
                        {
                            take = true;
                        }
                        else
                        {
                            std::string message = "TAKE " + Items::Defaults[item].Name + "?";

                            if (Interface::Confirm(graphics, background, message, Color::Background, Color::Active, BloodSword::Border, Color::Active, true))
                            {
                                take = true;
                            }
                        }
                    }
                    else
                    {
                        std::string message = "TAKE " + Items::Defaults[item].Name + "?";

                        if (Interface::Confirm(graphics, background, message, Color::Background, Color::Active, BloodSword::Border, Color::Active, true))
                        {
                            take = true;
                        }
                    }

                    if (take)
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
                    }
                    else
                    {
                        result = true;
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::GAIN_DELAYED_EFFECT)
        {
            internal_error = true;

            // variables
            // 0 - player / all
            // 1 - status
            // 2 - item type
            if (Engine::IsAlive(party) && condition.Variables.size() > 2)
            {
                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto character = Interface::SelectCharacter(graphics, background, party, condition.Variables[0]);

                auto status = Character::MapStatus(condition.Variables[1]);

                auto item = Item::Map(condition.Variables[2]);

                if ((is_party || character != Character::Class::NONE) && status != Character::Status::NONE && item != Item::Type::NONE)
                {
                    auto characters = std::vector<Character::Class>();

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
                        characters = {character};
                    }

                    if (characters.size() > 0)
                    {
                        for (auto character : characters)
                        {
                            party[character].DelayedEffects[status] = item;
                        }

                        result = true;
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::CONFIRM)
        {
            // variables
            // 0 - message to display
            if (Engine::IsAlive(party) && condition.Variables.size() > 0)
            {
                result = Interface::Confirm(graphics, background, condition.Variables[0], Color::Background, Color::Active, BloodSword::Border, Color::Active, true);
            }
        }
        else if (condition.Type == Conditions::Type::PREVIOUS_LOCATION)
        {
            condition.Location = party.PreviousLocation;

            // variables
            // 0 - text to display (when used in an event)
            if (condition.Variables.size() > 0 && !condition.Variables[0].empty())
            {
                text = condition.Variables[0];
            }

            // do not trigger previous events upon return
            party.Add(Character::Status::SKIP_EVENTS);

            result = true;
        }

        // internal error
        if (!Engine::IsAlive(party))
        {
            text = Interface::DeathMessage(party);
        }
        else if (internal_error)
        {
            Conditions::InternalError(graphics, background, condition.Type);
        }

        result = condition.Invert ? !result : result;

        EvalLogger::Log(condition, result, failed, text);

        return failed ? Conditions::Evaluation(result, failed, text) : Conditions::Evaluation(result, text);
    }
}

#endif
