#ifndef __CONDITIONS_HPP__
#define __CONDITIONS_HPP__

#include <algorithm>
#include <vector>

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "Graphics.hpp"
#include "Item.hpp"
#include "Party.hpp"
#include "Scene.hpp"
#include "Templates.hpp"
#include "Interface.hpp"
#include "ConditionTypes.hpp"

namespace BloodSword::Conditions
{
    class Base
    {
    public:
        // condition type
        Conditions::Type Type = Type::NONE;

        // destination (book, section)
        Book::Location Location = Book::Undefined;

        // internal variables used to validate "condition"
        std::vector<std::string> Variables = {};

        // flag to indicate that the result (or condition) is inverted
        bool Invert = false;

        Base() {}

        Base(Conditions::Type type,
             Book::Location location,
             std::vector<std::string> variables,
             bool invert) : Type(type),
                            Variables(variables),
                            Invert(invert) {}

        Base(Conditions::Type type,
             Book::Location location,
             std::vector<std::string> variables) : Type(type),
                                                   Variables(variables) {}

        Base(const char *type, Book::Location location, std::vector<std::string> variables, bool invert = false)
        {
            this->Type = Conditions::Map(type);

            this->Variables = variables;

            this->Invert = invert;
        }
    };

    typedef std::vector<Conditions::Base> List;

    Conditions::Base Parse(nlohmann::json &data)
    {
        auto condition = Conditions::Base();

        if (!data.is_null())
        {
            if (!data["location"].is_null())
            {
                condition.Location = Book::Load(data["location"]);
            }

            // set condition type
            condition.Type = !data["type"].is_null() ? Conditions::Map(std::string(data["type"])) : Conditions::Type::NONE;

            // set variables
            if (!data["variables"].is_null() && data["variables"].is_array() && data["variables"].size() > 0)
            {
                auto variables = std::vector<std::string>();

                for (auto i = 0; i < data["variables"].size(); i++)
                {
                    variables.push_back(std::string(data["variables"][i]));
                }

                condition.Variables = variables;
            }

            // set invert condition
            condition.Invert = (!data["invert"].is_null() && data["invert"].is_boolean()) ? data["invert"].get<bool>() : false;
        }

        return condition;
    }

    // results of evaluating a condition
    class Evaluation
    {
    public:
        bool Result = false;

        bool Failed = false;

        // destination on failure
        Book::Location Location = Book::Undefined;

        // text (usually on failure)
        std::string Text = std::string();

        Evaluation() {}

        Evaluation(bool result, bool failed, Book::Location location, std::string text) : Result(result), Failed(failed), Location(location), Text(text) {}

        Evaluation(bool result, std::string text) : Result(result), Text(text) {}

        Evaluation(bool result) : Result(result) {}
    };

    void InternalError(Graphics::Base &graphics, Scene::Base &background, Conditions::Type condition)
    {
        std::string message = "Internal Error: " + std::string(Conditions::TypeMapping[condition]) + "!";

        Interface::InternalError(graphics, background, message);
    }

    void Log(Conditions::Base &condition, bool result, bool failed, std::string text, Book::Location location)
    {
        // debug info
        std::cerr << "CONDITION: "
                  << std::string(Conditions::TypeMapping[condition.Type])
                  << " (RESULT: "
                  << (result ? "true" : "false")
                  << ", FAILED: "
                  << (failed ? "true" : "false");

        if (text.size() > 0)
        {
            std::cerr << ", TEXT: " << text;
        }

        if (Book::IsDefined(location))
        {
            std::cerr << ", LOCATION: " << Book::String(location);
        }

        std::cerr << ")" << std::endl;
    }

    // routine to validate "condition"
    Conditions::Evaluation Process(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Conditions::Base &condition)
    {
        auto result = false;

        auto failed = false;

        auto internal_error = false;

        auto text = std::string();

        Book::Location location = Book::Undefined;

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
                auto character = Character::Map(condition.Variables[0]);

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
        else if (condition.Type == Conditions::Type::CHOSEN_PLAYER)
        {
            if (condition.Variables.size() > 0)
            {
                auto character = Character::Map(condition.Variables[0]);

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
            if (condition.Variables.size() > 0)
            {
                auto number = std::stoi(condition.Variables[0], nullptr, 10);

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

            if (condition.Variables.size() > 0)
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
        else if (condition.Type == Conditions::Type::IN_PARTY_WITH_ITEM)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - item
            if (condition.Variables.size() > 1)
            {
                auto character = Character::Map(condition.Variables[0]);

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
            // 2, 3 - destination upon failure
            // 4 - failure message
            if (condition.Variables.size() > 4)
            {
                auto character = Character::Map(std::string(condition.Variables[0]));

                auto attribute = Attribute::Map(condition.Variables[1]);

                auto book = Book::MapBook(condition.Variables[2]);

                auto section = std::stoi(condition.Variables[3], nullptr, 10);

                if (character != Character::Class::NONE && attribute != Attribute::Type::NONE && book != Book::Number::NONE && section > 0)
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

                        result = true;

                        if (!test)
                        {
                            failed = true;

                            location = {book, section};

                            text = condition.Variables[4];
                        }
                    }

                    internal_error = false;
                }
            }
        }
        else if (condition.Type == Conditions::Type::ITEM_QUANTITY)
        {
            internal_error = true;

            // variables
            // 0 - player
            // 1 - item
            // 2 - quantity
            if (condition.Variables.size() > 2)
            {
                auto character = Character::Map(std::string(condition.Variables[0]));

                auto item = Item::Map(condition.Variables[1]);

                auto quantity = std::stoi(condition.Variables[2], nullptr, 10);

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
                                text = std::string("YOU DO NOT HAVE ENOUGH ") + Item::TypeMapping[item] + "!";
                            }
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
                auto character = Character::Map(std::string(condition.Variables[0]));

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
            result = (party.Count() > 1);

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
                if (condition.Variables.empty())
                {
                    text = "YOU ARE ALONE!";
                }
            }
        }
        else if (condition.Type == Conditions::Type::SOLO)
        {
            result = (party.Count() == 1);

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
                if (condition.Variables.empty())
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

                        if (party.Count() > 1)
                        {
                            text = "EVERYONE GAINS [" + std::string(Character::StatusMapping[status]) + "]";
                        }
                        else
                        {
                            text = "YOU GAIN [" + std::string(Character::StatusMapping[status]) + "]";
                        }

                        internal_error = false;
                    }
                }
                else
                {
                    auto character = Character::Map(condition.Variables[0]);

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

                            text = party[character].Name + " GAINS" + Character::StatusMapping[status];
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

                            if (party.Count() > 1)
                            {
                                text = "EVERYONE LOSES [" + std::string(Character::StatusMapping[status]) + "]";
                            }
                            else
                            {
                                text = "YOU LOSE [" + std::string(Character::StatusMapping[status]) + "]";
                            }
                        }

                        internal_error = false;
                    }
                }
                else
                {
                    auto character = Character::Map(condition.Variables[0]);

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

                            text = party[character].Name + " LOSES" + Character::StatusMapping[status];
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
            // 0 - player
            if (condition.Variables.size() > 0)
            {
                auto character = Character::Map(condition.Variables[0]);

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
                        auto character = Character::Map(condition.Variables[0]);

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

                auto min_count = std::stoi(condition.Variables[1], nullptr, 10);

                auto max_count = std::stoi(condition.Variables[2], nullptr, 10);

                std::cerr << Character::StatusMapping[status] << " MIN: " << std::to_string(min_count) << " MAX: " << std::to_string(max_count) << std::endl;

                if (Engine::IsAlive(party) && status != Character::Status::NONE && min_count >= 0 && max_count >= 0 && min_count <= max_count)
                {
                    auto count = Engine::Count(party, Character::ControlType::PLAYER, status);

                    std::cerr << Character::StatusMapping[status] << ": " << std::to_string(count) << std::endl;

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
            if (condition.Variables.size() > 3 && Engine::IsAlive(party))
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
                auto character = Character::Map(condition.Variables[0]);

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
                auto character = Character::Map(condition.Variables[0]);

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
            // 5 to 5+(0) - assets
            // last - asset (hidden)
            if (condition.Variables.size() > 5)
            {
                auto options = std::stoi(condition.Variables[1], nullptr, 10);

                auto min_select = std::stoi(condition.Variables[2], nullptr, 10);

                auto max_select = std::stoi(condition.Variables[3], nullptr, 10);

                auto hidden = Engine::ToUpper(condition.Variables[4]) == "TRUE";

                auto expected = 3 + options + (hidden ? 1 : 0);

                if (options > 0 && min_select > 0 && min_select <= max_select && condition.Variables.size() > expected)
                {
                    // contains assets
                    auto assets = std::vector<Asset::Type>();

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

            // compare scores
            if (condition.Variables.size() > 0)
            {
                auto beat = std::stoi(condition.Variables[0], nullptr, 10);

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
        else if (condition.Type == Conditions::Type::DISCARD_ITEM)
        {
            // variables:
            // 0 - player / ALL
            // 1 .. N - items to discard (type)
            if (condition.Variables.size() > 1)
            {
                auto character = Character::Map(std::string(condition.Variables[0]));

                auto characters = std::vector<Character::Class>();

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
            // 0 - ALL/FIRST/SELECT/CHOSEN or player
            // 1 - FALL-BACK recipient (NONE/FIRST/SELECT)
            // 2 - item
            if (condition.Variables.size() > 2)
            {
                auto character = Character::Map(std::string(condition.Variables[0]));

                character = (Engine::ToUpper(condition.Variables[0]) == "CHOSEN") ? party.ChosenCharacter : character;

                auto is_character = (character != Character::Class::NONE);

                auto is_party = (Engine::ToUpper(condition.Variables[0]) == "ALL");

                auto is_first = (Engine::ToUpper(condition.Variables[0]) == "FIRST");

                auto selected = (Engine::ToUpper(condition.Variables[0]) == "SELECT");

                auto next = is_character && (Engine::ToUpper(condition.Variables[1]) == "FIRST");

                auto select_next = is_character && (Engine::ToUpper(condition.Variables[1]) == "SELECT");

                auto item = Item::Map(condition.Variables[2]);

                if ((is_party || is_first || selected || is_character) && Engine::IsAlive(party) && item != Item::Type::NONE && Items::Found(item))
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
                    else if (is_first || (is_character && (!party.Has(character) || !Engine::IsAlive(party[character])) && next))
                    {
                        characters.push_back(Engine::FirstClass(party));
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
                auto excluded = std::vector<Item::Type>();

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
                    if (party.Count() > 1)
                    {
                        text = Interface::DeathMessage(party);
                    }
                    else
                    {
                        text = Engine::IsDead(party[0]);
                    }
                }
            }
            else
            {
                internal_error = true;
            }
        }
        else if (condition.Type == Conditions::Type::SELECT_PLAYER)
        {
            // variables:
            // 0 - message to display
            if (condition.Variables.size() > 0 && Engine::IsAlive(party))
            {
                if (Engine::Count(party) > 1)
                {
                    auto message = condition.Variables[0].size() > 0 ? condition.Variables[0] : std::string("SELECT PLAYER");

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
        else if (condition.Type == Conditions::Type::STAKE)
        {
            // variables
            // 0 - item to stake
            // 1 - asset (to item in #0)
            if (condition.Variables.size() > 1 && Engine::IsAlive(party))
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
            if (condition.Variables.size() > 1 && Engine::IsAlive(party))
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
            if (condition.Variables.size() > 2 && Engine::IsAlive(party))
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
            if (condition.Variables.size() > 0 && Engine::IsAlive(party))
            {
                auto number = (Engine::ToUpper(condition.Variables[0]) != "CHOOSE");

                if (number)
                {
                    party.ChosenNumber = std::stoi(condition.Variables[0], nullptr, 10);

                    result = true;
                }
                else if (condition.Variables.size() > 3)
                {
                    auto min_number = std::stoi(condition.Variables[2], nullptr, 10);

                    auto max_number = std::stoi(condition.Variables[3], nullptr, 10);

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
            if (condition.Variables.size() > 4 && Engine::IsAlive(party))
            {
                auto roll = std::stoi(condition.Variables[0], nullptr, 10);

                auto mods = std::stoi(condition.Variables[1], nullptr, 10);

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
            if (condition.Variables.size() > 2 && Engine::IsAlive(party))
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
            if (condition.Variables.size() > 4 && Engine::IsAlive(party))
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
            if (condition.Variables.size() > 5 && Engine::IsAlive(party))
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
                auto character = (Engine::ToUpper(condition.Variables[0]) == "CHOSEN") ? party.ChosenCharacter : Character::Map(std::string(condition.Variables[0]));

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
                                text = std::string("YOU DO NOT HAVE ENOUGH ") + Item::TypeMapping[item] + "!";
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
            // 0 - N variables to show
            if (condition.Variables.size() > 0 && Engine::IsAlive(party))
            {
                auto wrap = BloodSword::TileSize * 5;

                for (auto i = 0; i < condition.Variables.size(); i++)
                {
                    auto variable = party.Get(condition.Variables[i]);

                    if (!variable.empty())
                    {
                        if (!text.empty())
                        {
                            text += "\n";
                        }

                        text += condition.Variables[i] + ": " + variable;
                    }
                }

                if (text.size() > 0)
                {
                    Interface::TextBox(graphics, background, text, wrap);
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
            if (condition.Variables.size() > 2 && Engine::IsAlive(party))
            {
                auto item = Item::Map(condition.Variables[0]);

                auto limit = (Engine::ToUpper(condition.Variables[1]) == "UNLIMITED") ? Items::Unlimited : std::stoi(condition.Variables[1], nullptr, 10);

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
            if (condition.Variables.size() > 2 && Engine::IsAlive(party))
            {
                auto asset = Asset::Map(condition.Variables[0]);

                auto gain = std::stoi(condition.Variables[1], nullptr, 10);

                auto limit = (Engine::ToUpper(condition.Variables[2]) == "UNLIMITED") ? Items::Unlimited : std::stoi(condition.Variables[2], nullptr, 10);

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

        Conditions::Log(condition, result, failed, text, location);

        return failed ? Conditions::Evaluation(result, failed, location, text) : Conditions::Evaluation(result, text);
    }
}

#endif
