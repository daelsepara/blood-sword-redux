#ifndef __CONDITIONS_HPP__
#define __CONDITIONS_HPP__

#include "nlohmann/json.hpp"
#include "Book.hpp"
#include "Graphics.hpp"
#include "Item.hpp"
#include "Party.hpp"
#include "Scene.hpp"
#include "Templates.hpp"
#include "Interface.hpp"

namespace BloodSword::Section::Conditions
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
        LOSE_ALL
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
        {Conditions::Type::LOSE_ALL, "LOSE ALL"}};

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

    Conditions::Base Parse(nlohmann::json &data)
    {
        auto condition = Conditions::Base();

        if (!data.is_null())
        {
            if (!data["location"].is_null())
            {
                // set location
                auto book = !data["location"]["book"].is_null() ? Book::MapBook(std::string(data["location"]["book"])) : Book::Number::NONE;

                auto number = !data["location"]["number"].is_null() ? int(data["location"]["number"]) : -1;

                condition.Location = {book, number};
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

    std::string NoItem(Item::Type &item)
    {
        return (std::string("YOU DO NOT HAVE THE ") + std::string(Item::TypeMapping[item]) + "!");
    }

    // routine to validate "condition"
    Conditions::Evaluation Process(Graphics::Base &graphics, Scene::Base &background, Party::Base &party, Conditions::Base &condition)
    {
        auto result = false;

        auto failed = false;

        auto text = std::string();

        Book::Location location = {Book::Number::NONE, -1};

        if (condition.Type == Conditions::Type::NORMAL)
        {
            result = true;
        }
        else if (condition.Type == Conditions::Type::IN_PARTY)
        {
            auto character = Character::Map(condition.Variables[0]);

            result = party.Has(character);

            if (!result)
            {
                text = Conditions::NotInParty(character);
            }
        }
        else if (condition.Type == Conditions::Type::CHOSEN_PLAYER)
        {
            auto character = Character::Map(condition.Variables[0]);

            result = (party.ChosenCharacter == character);

            if (!result)
            {
                text = std::string("YOU HAVE CHOSEN A DIFFERENT PLAYER!");
            }
        }
        else if (condition.Type == Conditions::Type::CHOSEN_NUMBER)
        {
            auto number = std::stoi(condition.Variables[0], nullptr, 10);

            result = (party.ChosenNumber == number);

            if (!result)
            {
                text = std::string("YOU HAVE CHOSEN A DIFFERENT NUMBER!");
            }
        }
        else if (condition.Type == Conditions::Type::HAS_ITEM)
        {
            auto item = Item::Map(condition.Variables[0]);

            result = party.Has(item);

            if (!result)
            {
                text = Conditions::NoItem(item);
            }
        }
        else if (condition.Type == Conditions::Type::TEST_ATTRIBUTE)
        {
            // variables
            // 0 - player
            // 1 - attribute
            // 2, 3 - bestination upon failure
            // 4 - failure message
            if (condition.Variables.size() < 5)
            {
                text = std::string("UNABLE TO PERFORM THIS ACTION!");
            }
            else
            {
                auto character = Character::Map(std::string(condition.Variables[0]));

                auto attribute = Attribute::Map(condition.Variables[1]);

                auto book = Book::MapBook(condition.Variables[2]);

                auto section = std::stoi(condition.Variables[3], nullptr, 10);

                if (character != Character::Class::NONE && !party.Has(character))
                {
                    text = Conditions::NotInParty(character);
                }
                else if (attribute == Attribute::Type::NONE)
                {
                    text = std::string("UNABLE TO PERFORM THIS ACTION!");
                }
                else if (character != Character::Class::NONE && party.Has(character))
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
                else
                {
                    Conditions::InternalError(graphics, background, condition.Type);
                }
            }
        }
        else if (condition.Type == Conditions::Type::ITEM_QUANTITY)
        {
            // variables
            // 0 - player
            // 1 - item
            // 2 - quantity
            if (condition.Variables.size() < 3)
            {
                text = std::string("UNABLE TO PERFORM THIS ACTION!");
            }
            else
            {
                auto character = Character::Map(std::string(condition.Variables[0]));

                auto item = Item::Map(condition.Variables[1]);

                auto quantity = std::stoi(condition.Variables[2], nullptr, 10);

                if (character != Character::Class::NONE && !party.Has(character))
                {
                    text = Conditions::NotInParty(character);
                }
                else if (item == Item::Type::NONE)
                {
                    Conditions::InternalError(graphics, background, condition.Type);
                }
                else if (quantity != 0)
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
                else
                {
                    Conditions::InternalError(graphics, background, condition.Type);
                }
            }
        }
        else if (condition.Type == Conditions::Type::LOSE_ALL)
        {
            // variables
            // 0 - player
            // 1 - item
            if (condition.Variables.size() < 2)
            {
                text = std::string("UNABLE TO PERFORM THIS ACTION!");
            }
            else
            {
                auto character = Character::Map(std::string(condition.Variables[0]));

                auto item = Item::Map(condition.Variables[1]);

                if (character != Character::Class::NONE && !party.Has(character))
                {
                    text = Conditions::NotInParty(character);
                }
                else if (item == Item::Type::NONE)
                {
                    Conditions::InternalError(graphics, background, condition.Type);
                }
                else if (party[character].Quantity(item) > 0)
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

        result = condition.Invert ? !result : result;

        // debug info
        std::cerr << "Condition: "
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
            std::cerr << ", LOCATION: [" << Book::Mapping[location.first]
                      << ": " << std::to_string(location.second) << "]";
        }

        std::cerr << ")" << std::endl;

        return failed ? Conditions::Evaluation(result, failed, location, text) : Conditions::Evaluation(result, text);
    }
}
#endif
