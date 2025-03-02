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

namespace BloodSword::Conditions
{
    enum class Type
    {
        NONE = -1,
        NORMAL,
        IN_PARTY,
        CHOOSE_PLAYER,
        CHOSEN_PLAYER,
        BATTLE_VICTORY,
        BATTLE_FLEE,
        BATTLE_ENTHRALMENT,
        BATTLE_VICTORY_OR_ENTHRALMENT,
        HAS_ITEM,
        USE_ITEM,
        DROP_ITEM,
        TAKE_ITEM,
        TAKE_ITEMS,
        GAIN_ENDURANCE,
        LOSE_ENDURANCE,
        CHOOSE_NUMBER,
        CHOSEN_NUMBER,
        PARTY_WOUNDED,
        PREVIOUS_LOCATION,
        TEST_ATTRIBUTE,
        ITEM_QUANTITY,
        LOSE_ALL,
        HAVE_COLLEAGUES,
        SOLO,
        GAIN_STATUS,
        LOSE_STATUS,
        FIRST,
        TEST_GAIN_STATUS,
        COUNT_STATUS,
        SELECT_MULTIPLE,
        CALL_TO_MIND,
        FORGET_SPELL,
        SELECT_CARDS,
        SELECTED_CARDS
    };

    BloodSword::Mapping<Conditions::Type> TypeMapping = {
        {Conditions::Type::NONE, "NONE"},
        {Conditions::Type::NORMAL, "NORMAL"},
        {Conditions::Type::IN_PARTY, "IN PARTY"},
        {Conditions::Type::CHOOSE_PLAYER, "CHOOSE PLAYER"},
        {Conditions::Type::CHOSEN_PLAYER, "CHOSEN PLAYER"},
        {Conditions::Type::BATTLE_VICTORY, "BATTLE VICTORY"},
        {Conditions::Type::BATTLE_FLEE, "BATTLE FLEE"},
        {Conditions::Type::BATTLE_ENTHRALMENT, "BATTLE ENTHRALMENT"},
        {Conditions::Type::BATTLE_VICTORY_OR_ENTHRALMENT, "BATTLE VICTORY OR ENTHRALMENT"},
        {Conditions::Type::HAS_ITEM, "HAS ITEM"},
        {Conditions::Type::USE_ITEM, "USE ITEM"},
        {Conditions::Type::DROP_ITEM, "DROP ITEM"},
        {Conditions::Type::TAKE_ITEM, "TAKE ITEM"},
        {Conditions::Type::TAKE_ITEMS, "TAKE ITEMS"},
        {Conditions::Type::GAIN_ENDURANCE, "GAIN ENDURANCE"},
        {Conditions::Type::LOSE_ENDURANCE, "LOSE ENDURANCE"},
        {Conditions::Type::CHOOSE_NUMBER, "CHOOSE NUMBER"},
        {Conditions::Type::CHOSEN_NUMBER, "CHOSEN NUMBER"},
        {Conditions::Type::PARTY_WOUNDED, "PARTY WOUNDED"},
        {Conditions::Type::PREVIOUS_LOCATION, "PREVIOUS LOCATION"},
        {Conditions::Type::TEST_ATTRIBUTE, "TEST ATTRIBUTE"},
        {Conditions::Type::ITEM_QUANTITY, "ITEM QUANTITY"},
        {Conditions::Type::LOSE_ALL, "LOSE ALL"},
        {Conditions::Type::HAVE_COLLEAGUES, "HAVE COLLEAGUES"},
        {Conditions::Type::SOLO, "SOLO"},
        {Conditions::Type::GAIN_STATUS, "GAIN STATUS"},
        {Conditions::Type::FIRST, "FIRST"},
        {Conditions::Type::TEST_GAIN_STATUS, "TEST GAIN STATUS"},
        {Conditions::Type::COUNT_STATUS, "COUNT STATUS"},
        {Conditions::Type::LOSE_STATUS, "LOSE STATUS"},
        {Conditions::Type::SELECT_MULTIPLE, "SELECT MULTIPLE"},
        {Conditions::Type::CALL_TO_MIND, "CALL TO MIND"},
        {Conditions::Type::FORGET_SPELL, "FORGET SPELL"},
        {Conditions::Type::SELECT_CARDS, "SELECT CARDS"},
        {Conditions::Type::SELECTED_CARDS, "SELECTED CARDS"}};

    Conditions::Type Map(const char *Conditions)
    {
        return BloodSword::Find(Conditions::TypeMapping, Conditions);
    }

    Conditions::Type Map(std::string Conditions)
    {
        return Conditions::Map(Conditions.c_str());
    }

    class Base
    {
    public:
        // condition type
        Conditions::Type Type = Type::NONE;

        // destination (book, section)
        Book::Location Location = {Book::Number::NONE, -1};

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
            condition.Invert = (!data["invert"].is_null() && data["invert"].is_boolean()) ? bool(!data["invert"]) : false;
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
        Book::Location Location = {Book::Number::NONE, -1};

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

    std::string NotInParty(Character::Class &character)
    {
        return (std::string("YOU DO NOT HAVE THE ") + std::string(Character::ClassMapping[character]) + " IN YOUR PARTY!");
    }

    std::string IsDead(Character::Base &character)
    {
        return character.Name + " IS DEAD!";
    }

    std::string NoItem(Item::Type &item)
    {
        return (std::string("YOU DO NOT HAVE THE ") + std::string(Item::TypeMapping[item]) + "!");
    }

    std::string ToUpper(std::string str)
    {
        auto upper = str;

        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

        return upper;
    }

    // routine to validate "condition"
    Conditions::Evaluation Process(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Conditions::Base &condition)
    {
        auto result = false;

        auto failed = false;

        auto internal_error = false;

        auto text = std::string();

        Book::Location location = {Book::Number::NONE, -1};

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
                        text = Conditions::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Conditions::IsDead(party[character]);

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
            if (condition.Variables.size() > 0)
            {
                auto item = Item::Map(condition.Variables[0]);

                result = party.Has(item);

                if (!result)
                {
                    text = Conditions::NoItem(item);
                }
            }
            else
            {
                internal_error = true;
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
                        text = Conditions::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Conditions::IsDead(party[character]);
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
                        text = Conditions::NotInParty(character);
                    }
                    else
                    {
                        if (!Engine::IsAlive(party[character]))
                        {
                            text = Conditions::IsDead(party[character]);
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
                        text = Conditions::NotInParty(character);
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
                            text = Conditions::IsDead(party[character]);
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

                if (ToUpper(condition.Variables[0]) == "ALL")
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
                            text = Conditions::NotInParty(character);
                        }
                        else if (!Engine::IsAlive(party[character]))
                        {
                            text = Conditions::IsDead(party[character]);

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

                if (ToUpper(condition.Variables[0]) == "ALL")
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
                            text = Conditions::NotInParty(character);
                        }
                        else if (!Engine::IsAlive(party[character]))
                        {
                            text = Conditions::IsDead(party[character]);

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
                        text = Conditions::NotInParty(character);
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
                    if (ToUpper(condition.Variables[0]) == "ALL")
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
                                    text = Conditions::IsDead(party[character]);
                                }
                            }
                            else
                            {
                                text = Conditions::NotInParty(character);
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
            // 3 - Message to display
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
                        text = Conditions::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Conditions::IsDead(party[character]);
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
                        text = Conditions::NotInParty(character);
                    }
                    else if (!Engine::IsAlive(party[character]))
                    {
                        text = Conditions::IsDead(party[character]);
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

        // internal error
        if (internal_error)
        {
            Conditions::InternalError(graphics, background, condition.Type);
        }

        result = condition.Invert ? !result : result;

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

        return failed ? Conditions::Evaluation(result, failed, location, text) : Conditions::Evaluation(result, text);
    }
}
#endif
