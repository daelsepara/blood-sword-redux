#ifndef __PARTY_HPP__
#define __PARTY_HPP__

#include <stdexcept>
#include <fstream>

#include "nlohmann/json.hpp"
#include "BattleResults.hpp"
#include "Book.hpp"
#include "Character.hpp"

namespace BloodSword::Party
{
    typedef std::vector<Character::Base> List;

    class Base
    {
    private:
        // members in the party
        Party::List Members = {};

    public:
        // current book and section number
        Book::Location Location = Book::Undefined;

        Book::Location PreviousLocation = Book::Undefined;

        // for choices / conditions requiring a selected character
        Character::Class ChosenCharacter = Character::Class::NONE;

        // for choices / conditions requiring the result of the last battle
        Battle::Result LastBattle = Battle::Result::NONE;

        // location of last battle
        Book::Location BattleLocation = Book::Undefined;

        // for choices / conditions requiring a chosen number (1-6)
        int ChosenNumber = -1;

        // holds survivors from previous battles
        Party::List Survivors = {};

        // for the magus kalugen's card game
        Items::Deck Cards = {};

        // variables
        BloodSword::UnorderedMap<std::string, std::string> Variables = {};

        Base() {}

        Base(Party::List members) : Members(members) {}

        Base(Character::Base character) : Members(Party::List({character})) {}

        // add character to party
        void Add(Character::Base &character)
        {
            this->Members.push_back(character);
        }

        // number of characters in the party
        int Count()
        {
            return int(this->Members.size());
        }

        // access party member by index
        Character::Base &operator[](int index)
        {
            if (index < 0 || index > this->Count() - 1)
            {
                std::string missing = "CHARACTER: " + std::to_string(index) + " NOT IN PARTY!";

                throw std::invalid_argument(missing);
            }

            return this->Members[index];
        }

        // check if a specific character class is present in party
        Party::List::const_iterator Find(Character::Class character_class)
        {
            auto found = this->Members.end();

            for (auto character = this->Members.begin(); character != this->Members.end(); character++)
            {
                if (character->Class == character_class)
                {
                    found = character;

                    break;
                }
            }

            return found;
        }

        // check if party has a member of a specific character class
        bool Has(Character::Class character_class)
        {
            return this->Find(character_class) != this->Members.end();
        }

        // returns index of character in party
        int Index(Character::Class character_class)
        {
            auto index = -1;

            for (auto i = 0; i < this->Members.size(); i++)
            {
                if (this->Members[i].Class == character_class)
                {
                    index = i;

                    break;
                }
            }

            return index;
        }

        // find index of party member that is a valid target
        int Find(Target::Type target)
        {
            auto found = -1;

            for (auto i = 0; i < this->Members.size(); i++)
            {
                if (this->Members[i].Target == target)
                {
                    found = i;

                    break;
                }
            }

            return found;
        }

        // check if anyone in the party has this status
        int Find(Character::Status status)
        {
            auto found = -1;

            for (auto i = 0; i < this->Members.size(); i++)
            {
                if (this->Members[i].Has(status))
                {
                    found = i;

                    break;
                }
            }

            return found;
        }

        // check if party has valid target
        bool Has(Target::Type target)
        {
            return (this->Find(target) != -1);
        }

        // check if party has an item of type
        bool Has(Item::Type item)
        {
            auto result = false;

            for (auto character = this->Members.begin(); character != this->Members.end(); character++)
            {
                if (character->Has(item))
                {
                    result = true;

                    break;
                }
            }

            return result;
        }

        // check if party has charged item of type (charge) with enough quantity
        bool HasChargedItem(Item::Type item, Item::Type charge, int quantity)
        {
            auto result = false;

            for (auto character = this->Members.begin(); character != this->Members.end(); character++)
            {
                if (character->HasCharged(item, charge, quantity))
                {
                    result = true;

                    break;
                }
            }

            return result;
        }

        // charge / discharge item
        bool AddCharge(Item::Type item, Item::Type charge, int quantity)
        {
            auto result = false;

            for (auto character = this->Members.begin(); character != this->Members.end(); character++)
            {
                if (character->HasCharged(item, charge, quantity < 0 ? quantity : 0))
                {
                    character->AddCharge(item, charge, quantity);
                }
            }

            return result;
        }

        bool Has(Character::Status status)
        {
            return (this->Find(status) != -1);
        }

        bool Has(Item::CardType card)
        {
            return BloodSword::Found(this->Cards, card);
        }

        // access party by character class
        Character::Base &operator[](Character::Class character_class)
        {
            if (!this->Has(character_class))
            {
                std::string missing = "CHARACTER: " + std::string(Character::ClassMapping[character_class]) + " NOT IN PARTY!";

                throw std::invalid_argument(missing);
            }

            return (*this)[this->Find(character_class) - this->Members.begin()];
        }

        // remove character from party (based on index)
        void Remove(int index)
        {
            if (index >= 0 && index < this->Count())
            {
                this->Members.erase(this->Members.begin() + index);
            }
        }

        // remove item from everyone
        void Remove(Item::Type item)
        {
            if (item != Item::Type::NONE)
            {
                for (auto character = this->Members.begin(); character != this->Members.end(); character++)
                {
                    if (character->Has(item))
                    {
                        character->Remove(item);
                    }
                }
            }
        }

        void Clear()
        {
            this->Members.clear();
        }

        void Set(Book::Location location)
        {
            this->PreviousLocation = this->Location;

            this->Location = location;

            for (auto &member : this->Members)
            {
                member.Location = location;
            }
        }

        // remove character from party (based on character class)
        void Remove(Character::Class character_class)
        {
            auto found = -1;

            if (!this->Has(character_class))
            {
                std::string missing = "CHARACTER: " + std::string(Character::ClassMapping[character_class]) + " NOT IN PARTY!";

                throw std::invalid_argument(missing);
            }

            for (auto i = 0; i < this->Count(); i++)
            {
                auto member = (*this)[i];

                if (member.Class == character_class)
                {
                    found = i;

                    break;
                }
            }

            if (found >= 0 && found < this->Count())
            {
                this->Remove(found);
            }
        }

        // add status to entire party
        void Add(Character::Status status)
        {
            for (auto i = 0; i < this->Count(); i++)
            {
                this->Members[i].Add(status);
            }
        }

        // remove status from the entire party
        void Remove(Character::Status status)
        {
            for (auto i = 0; i < this->Count(); i++)
            {
                this->Members[i].Remove(status);
            }
        }

        void Remove(std::vector<Character::Status> statuses)
        {
            for (auto i = 0; i < this->Count(); i++)
            {
                for (auto status : statuses)
                {
                    this->Members[i].Remove(status);
                }
            }
        }

        void ResetSpells()
        {
            for (auto i = 0; i < this->Count(); i++)
            {
                if (this->Members[i].Has(Skills::Type::SPELLS))
                {
                    this->Members[i].ResetSpellComplexities();
                }
            }
        }

        // Load party from json data
        void Load(nlohmann::json &data)
        {
            // set party location
            if (!data["location"].is_null())
            {
                this->Location = Book::Load(data["location"]);
            }

            // set party previous location
            if (!data["previous_location"].is_null())
            {
                this->PreviousLocation = Book::Load(data["previous_location"]);
            }

            this->ChosenNumber = !data["chosen_number"].is_null() ? int(data["chosen_number"]) : -1;

            this->ChosenCharacter = !data["chosen_character"].is_null() ? Character::Map(std::string(data["chosen_character"])) : Character::Class::NONE;

            this->LastBattle = !data["last_battle"].is_null() ? Battle::MapResult(std::string(data["last_battle"])) : Battle::Result::NONE;

            if (!data["battle_location"].is_null())
            {
                this->BattleLocation = Book::Load(data["battle_location"]);
            }

            // load party members
            this->Clear();

            if (!data["members"].is_null() && data["members"].is_array() && data["members"].size() > 0)
            {
                for (auto i = 0; i < data["members"].size(); i++)
                {
                    auto character = Character::Load(data["members"][i]);

                    this->Add(character);
                }
            }

            // load survivors
            this->Survivors.clear();

            if (!data["survivors"].is_null() && data["survivors"].is_array() && data["survivors"].size() > 0)
            {
                for (auto i = 0; i < data["survivors"].size(); i++)
                {
                    auto character = Character::Load(data["survivors"][i]);

                    this->Survivors.push_back(character);
                }
            }

            // load deck of cards
            this->Cards.clear();

            if (!data["cards"].is_null() && data["cards"].is_array() && data["cards"].size() > 0)
            {
                this->Cards = Items::LoadDeck(data["cards"]);
            }

            this->Variables.clear();

            if (!data["variables"].is_null() && data["variables"].is_object())
            {
                for (auto &[key, val] : data["variables"].items())
                {
                    auto variable = std::string(key);

                    auto value = std::string(val);

                    this->Variables[key] = value;
                }
            }
        }

        // check if string is a number
        bool IsANumber(const std::string &variable)
        {
            // check if first character is a sign (-/+)
            auto offset = (variable.size() > 1 && (variable[0] == '-' || variable[0] == '+') ? 1 : 0);

            // SEE: https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
            auto result = !variable.empty() && std::find_if(variable.begin() + offset, variable.end(), [](unsigned char c)
                                                            { return !std::isdigit(c); }) == variable.end();

            std::cerr << "[VARIABLE " << variable << "] IS " << (!result ? "NOT" : "") << " A NUMBER" << std::endl;

            return result;
        }

        // check if variable is in internal table
        bool IsPresent(std::string variable)
        {
            auto result = !variable.empty() && (this->Variables.find(variable) != this->Variables.end());

            std::cerr << "[VARIABLE " << variable << "] IS " << (result ? "PRE" : "AB") << "SENT" << std::endl;

            return result;
        }

        // get value of the variable stored in internal table (and/or return literal value)
        std::string Get(std::string variable)
        {
            auto value = std::string();

            if (!variable.empty())
            {
                if (variable == "PARTY")
                {
                    value = std::to_string(this->Count());

                    std::cerr << "[PARTY] ---> " << value << std::endl;
                }
                else if (variable == "CHOSEN")
                {
                    value = std::to_string(this->ChosenNumber);

                    std::cerr << "[CHOSEN NUMBER] ---> " << value << std::endl;
                }
                else if (this->IsPresent(variable))
                {
                    value = this->Variables[variable];

                    std::cerr << "[VARIABLE " << variable << "] ---> " << value << std::endl;
                }
                else
                {
                    // may be a number of a string literal
                    std::cerr << "[LITERAL] " << variable << std::endl;

                    value = variable;
                }
            }

            return value;
        }

        void Set(std::string variable, std::string value)
        {
            if (!variable.empty())
            {
                if (variable == "CHOSEN")
                {
                    if (this->IsANumber(value))
                    {
                        this->ChosenNumber = std::stoi(value, nullptr, 10);

                        std::cerr << "[CHOSEN NUMBER] <--- " << value << std::endl;
                    }
                }
                else if (variable == "PLAYER")
                {
                    auto index = this->Index(this->ChosenCharacter);

                    if (this->ChosenCharacter != Character::Class::NONE && index != -1)
                    {
                        value = this->Members[index].Name;

                        this->Variables[variable] = value;

                        std::cerr << "[VARIABLE PLAYER] <--- " << value << std::endl;
                    }
                }
                else if (!this->IsANumber(variable))
                {
                    this->Variables[variable] = value;

                    std::cerr << "[VARIABLE " << variable << "] <--- " << value << std::endl;
                }
            }
        }

        void Set(std::string variable, int value)
        {
            if (!variable.empty())
            {
                if (variable == "CHOSEN")
                {
                    this->ChosenNumber = value;

                    std::cerr << "[CHOSEN NUMBER] <--- " << value << std::endl;
                }
                else if (!this->IsANumber(variable))
                {
                    this->Variables[variable] = std::to_string(value);

                    std::cerr << "[VARIABLE " << variable << "] <--- " << value << std::endl;
                }
            }
        }

        int Number(std::string variable)
        {
            auto value = 0;

            if (!variable.empty())
            {
                if (variable == "PARTY")
                {
                    value = this->Count();
                }
                else if (variable != "CHOSEN")
                {
                    if (this->IsANumber(variable))
                    {
                        value = std::stoi(variable, nullptr, 10);
                    }
                    else
                    {
                        auto search = this->Get(variable);

                        value = (!search.empty() && this->IsANumber(search)) ? std::stoi(search, nullptr, 10) : 0;
                    }
                }
                else
                {
                    value = this->ChosenNumber;
                }
            }

            std::cerr << "[NUMBER " << variable << "] ---> " << value << std::endl;

            return value;
        }

        bool IsValid(std::vector<std::string> list, std::string item)
        {
            auto result = BloodSword::Find(list, item) != list.end();

            std::cerr << "[CHECK " << item << "] IS " << (result ? "VALID" : "INVALID") << std::endl;

            return result;
        }

        void Math(std::string operation, std::string first, std::string second, bool clamp = true)
        {
            // first = (first) (operation) (second)
            if (!operation.empty() && !first.empty() && !this->IsANumber(first) && !second.empty())
            {
                // check if operation is valid
                if (this->IsValid({"+", "-", "*"}, operation))
                {
                    if (first != "CHOSEN" && !this->IsPresent(first) && !this->IsANumber(first))
                    {
                        // initialize first variable if not present
                        this->Set(first, 0);
                    }

                    if ((!this->IsANumber(first) || this->IsANumber(this->Get(first))) && (this->IsANumber(second) || this->IsANumber(this->Get(second))))
                    {
                        auto value_first = this->Number(first);

                        auto value_second = this->Number(second);

                        if (operation == "+")
                        {
                            value_first += value_second;
                        }
                        else if (operation == "-")
                        {
                            value_first -= value_second;
                        }
                        else if (operation == "*")
                        {
                            value_first *= value_second;
                        }

                        value_first = clamp ? std::max(0, value_first) : value_first;

                        std::cerr << "[MATH] " << first << " " << operation << " " << second << " = " << value_first << std::endl;

                        // set variable
                        this->Set(first, value_first);
                    }
                }
            }
        }

        // logical operations (on non-numeric)
        bool Is(std::string operation, std::string first, std::string second)
        {
            auto result = false;

            // (first) (logical operiation) (second)
            if (!operation.empty() && !first.empty() && !second.empty())
            {
                // check if operation is valid
                if (this->IsValid({"=", "!=", "<>"}, operation))
                {
                    auto value_first = this->Get(first);

                    auto value_second = this->Get(second);

                    if (operation == "=")
                    {
                        result = (value_first == value_second);
                    }
                    else if (operation == "!=" || operation == "<>")
                    {
                        result = (value_first != value_second);
                    }

                    std::cerr << "[IF] " << first << " " << operation << " " << second << " IS " << (result ? "TRUE" : "FALSE") << std::endl;
                }
            }

            return result;
        }

        // logical operations
        bool If(std::string operation, std::string first, std::string second)
        {
            auto result = false;

            // (first) (logical operiation) (second)
            if (!operation.empty() && !first.empty() && !second.empty())
            {
                // check if operation is valid
                if (this->IsValid({"=", "!=", "<>", "<", ">", "<=", ">="}, operation))
                {
                    // check if both are numbers and/or resolve to numbers
                    if ((!this->IsANumber(this->Get(first)) && !this->IsANumber(first)) || (!this->IsANumber(this->Get(second)) && !this->IsANumber(second)))
                    {
                        return this->Is(operation, first, second);
                    }

                    auto value_first = this->Number(first);

                    auto value_second = this->Number(second);

                    if (operation == "=")
                    {
                        result = (value_first == value_second);
                    }
                    else if (operation == "!=" || operation == "<>")
                    {
                        result = (value_first != value_second);
                    }
                    else if (operation == "<")
                    {
                        result = (value_first < value_second);
                    }
                    else if (operation == "<=")
                    {
                        result = (value_first <= value_second);
                    }
                    else if (operation == ">")
                    {
                        result = (value_first > value_second);
                    }
                    else if (operation == ">=")
                    {
                        result = (value_first > value_second);
                    }

                    std::cerr << "[IF] " << first << " " << operation << " " << second << " IS " << (result ? "TRUE" : "FALSE") << std::endl;
                }
            }

            return result;
        }
    };

    Party::Base Initialize(nlohmann::json &data)
    {
        auto party = Party::Base();

        if (!data.is_null())
        {
            party.Load(data);
        }

        return party;
    }

    nlohmann::json Data(Party::Base &party)
    {
        nlohmann::json data;

        if (Book::IsDefined(party.Location))
        {
            data["location"] = Book::Data(party.Location);
        }

        if (Book::IsDefined(party.PreviousLocation))
        {
            data["previous_location"] = Book::Data(party.PreviousLocation);
        }

        if (party.LastBattle != Battle::Result::NONE)
        {
            data["last_battle"] = Battle::ResultMapping[party.LastBattle];
        }

        if (Book::IsDefined(party.BattleLocation))
        {
            data["battle_location"] = Book::Data(party.BattleLocation);
        }

        if (party.ChosenCharacter != Character::Class::NONE)
        {
            data["chosen_character"] = Character::ClassMapping[party.ChosenCharacter];
        }

        if (party.ChosenNumber != -1)
        {
            data["chosen_number"] = party.ChosenNumber;
        }

        if (party.Count() > 0)
        {
            nlohmann::json members;

            for (auto character = 0; character < party.Count(); character++)
            {
                auto character_data = Character::Data(party[character]);

                members.push_back(character_data);
            }

            data["members"] = members;
        }

        if (party.Cards.size() > 0)
        {
            data["cards"] = Items::DeckData(party.Cards);
        }

        if (party.Variables.size() > 0)
        {
            nlohmann::json variables;

            for (auto &variable : party.Variables)
            {
                variables.emplace(variable.first, variable.second);
            }

            data["variables"] = variables;
        }

        return data;
    }

    void Save(Party::Base &party, const char *filename, const char *name)
    {
        nlohmann::json data;

        data.emplace(std::string(name), Party::Data(party));

        std::ofstream ifs(filename);

        if (ifs.is_open())
        {
            ifs << data.dump();

            ifs.close();
        }
    }

    Party::Base Load(const char *filename, const char *name)
    {
        auto party = Party::Base();

        std::ifstream ifs(filename);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            party = Party::Initialize(data[std::string(name)]);

            ifs.close();
        }

        return party;
    }
}

#endif
