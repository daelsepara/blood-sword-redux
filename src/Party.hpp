#ifndef __PARTY_HPP__
#define __PARTY_HPP__

#include <stdexcept>
#include <fstream>

#include "nlohmann/json.hpp"
#include "BattleResults.hpp"
#include "Book.hpp"
#include "Character.hpp"
#include "ZipFileLibrary.hpp"

// functions for managing the party of characters
namespace BloodSword::Party
{
    // list of characters
    typedef std::vector<Character::Base> List;

    // party base class
    class Base
    {
    private:
        // members in the party
        Party::List Members = {};

    public:
        // current module loaded
        std::string Module = "DEFAULT";

        // current book and section number
        Book::Location Location = Book::Undefined;

        // previous book and section number
        Book::Location PreviousLocation = Book::Undefined;

        // save location
        Book::Location SaveLocation = Book::Undefined;

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

        // location on map: room, x, y (rogue mode)
        int Room = -1;

        // location on map: x, y (rogue mode)
        int X = -1;

        int Y = -1;

        // default field of view radius
        int FieldOfView = 3;

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
                auto missing = "CHARACTER: " + std::to_string(index) + " NOT IN PARTY!";

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

        // party has characters that are not in standard list (WARRIOR, TRICKSTER, SAGE, ENCHANTER)
        bool HasOthers()
        {
            auto found = false;

            for (auto character = 0; character < this->Members.size(); character++)
            {
                found |= Character::OtherClass(this->Members[character].Class);

                if (found)
                {
                    break;
                }
            }

            return found;
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

        // search by name
        int Find(std::string name)
        {
            auto found = -1;

            for (auto i = 0; i < this->Members.size(); i++)
            {
                if (this->Members[i].Name == name)
                {
                    found = i;

                    break;
                }
            }

            return found;
        }

        // has a character with NAME
        bool Has(std::string name)
        {
            return (this->Find(name) != -1);
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

        // check if party has all items in list
        bool HasAll(Items::List items)
        {
            auto result = true;

            for (auto &item : items)
            {
                result &= this->Has(item);

                if (!result)
                {
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
                if (character->HasCharged(item, charge, quantity < 0 ? -quantity : 0))
                {
                    character->AddCharge(item, charge, quantity);
                }
            }

            return result;
        }

        // check if anyone in the party has this status
        bool Has(Character::Status status)
        {
            return (this->Find(status) != -1);
        }

        // check if party has this card in deck
        bool Has(Item::CardType card)
        {
            return BloodSword::Found(this->Cards, card);
        }

        // access party by character class
        Character::Base &operator[](Character::Class character_class)
        {
            if (!this->Has(character_class))
            {
                auto missing = "CHARACTER: " + Character::ClassMapping[character_class] + " NOT IN PARTY!";

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

        // clear party of all members
        void Clear()
        {
            this->Members.clear();
        }

        // clear party of all status
        void ClearStatus()
        {
            for (auto i = 0; i < this->Members.size(); i++)
            {
                this->Members[i].Status.clear();
            }
        }

        // set (book) location of party, set previous location
        void Set(Book::Location location)
        {
            this->PreviousLocation = this->Location;

            this->Location = location;

            for (auto &member : this->Members)
            {
                member.Set(location);
            }
        }

        // remove character from party (based on character class)
        void Remove(Character::Class character_class)
        {
            auto found = -1;

            if (!this->Has(character_class))
            {
                auto missing = "CHARACTER: " + Character::ClassMapping[character_class] + " NOT IN PARTY!";

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

        // add these statuses to party
        void Add(std::vector<Character::Status> statuses)
        {
            for (auto i = 0; i < this->Count(); i++)
            {
                for (auto &status : statuses)
                {
                    this->Members[i].Add(status);
                }
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

        // remove list of status from party
        void Remove(std::vector<Character::Status> statuses)
        {
            for (auto i = 0; i < this->Count(); i++)
            {
                for (auto &status : statuses)
                {
                    this->Members[i].Remove(status);
                }
            }
        }

        // reset spell complexities
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

        // get current location as point
        Point Origin()
        {
            return Point(this->X, this->Y);
        }

        // load party from json data
        void Load(nlohmann::json &data)
        {
            // set module
            this->Module = !data["module"].is_null() ? std::string(data["module"]) : "DEFAULT";

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

            // get saved location
            if (!data["save_location"].is_null())
            {
                this->SaveLocation = Book::Load(data["save_location"]);
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
                this->Cards = Items::LoadHand(data["cards"]);
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

            this->Room = !data["room"].is_null() ? int(data["room"]) : -1;

            this->X = !data["x"].is_null() ? int(data["x"]) : -1;

            this->Y = !data["y"].is_null() ? int(data["y"]) : -1;

            this->FieldOfView = !data["fov"].is_null() ? int(data["fov"]) : 3;
        }

        // check if string is a number
        bool IsANumber(const std::string &variable)
        {
            // check if first character is a sign (-/+)
            auto offset = (variable.size() > 1 && (variable[0] == '-' || variable[0] == '+') ? 1 : 0);

            // SEE: https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
            auto result = !variable.empty() && std::find_if(variable.begin() + offset, variable.end(), [](unsigned char c)
                                                            { return !std::isdigit(c); }) == variable.end();

            SDL_Log("[VARIABLE %s] IS %s A NUMBER", variable.c_str(), (!result ? "NOT" : ""));

            return result;
        }

        // check if variable is in internal table
        bool IsPresent(std::string variable)
        {
            auto result = !variable.empty() && BloodSword::Has(this->Variables, variable);

            SDL_Log("[VARIABLE %s] IS %sSENT", variable.c_str(), (!result ? "PRE" : "AB"));

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

                    SDL_Log("[PARTY] ---> %s", value.c_str());
                }
                else if (variable == "CHOSEN")
                {
                    value = std::to_string(this->ChosenNumber);

                    SDL_Log("[CHOSEN NUMBER] ---> %s", value.c_str());
                }
                else if (this->IsPresent(variable))
                {
                    value = this->Variables[variable];

                    SDL_Log("[VARIABLE %s] ---> %s", variable.c_str(), value.c_str());
                }
                else
                {
                    SDL_Log("[LITERAL] %s", variable.c_str());

                    // may be a number or a string literal
                    value = variable;
                }
            }

            return value;
        }

        // set variable
        void Set(std::string variable, std::string value)
        {
            if (!variable.empty())
            {
                if (variable == "CHOSEN")
                {
                    if (this->IsANumber(value))
                    {
                        this->ChosenNumber = std::stoi(value, nullptr, 10);

                        SDL_Log("[CHOSEN NUMBER] <--- %s", value.c_str());
                    }
                }
                else if (variable == "PLAYER")
                {
                    auto index = this->Index(this->ChosenCharacter);

                    if (this->ChosenCharacter != Character::Class::NONE && index != -1)
                    {
                        value = this->Members[index].Name;

                        this->Variables[variable] = value;

                        SDL_Log("[VARIABLE PLAYER] <--- %s", value.c_str());
                    }
                }
                else if (!this->IsANumber(variable))
                {
                    this->Variables[variable] = value;

                    SDL_Log("[VARIABLE %s] <--- %s", variable.c_str(), value.c_str());
                }
            }
        }

        // set numerical value of a variable
        void Set(std::string variable, int value)
        {
            if (!variable.empty())
            {
                if (variable == "CHOSEN")
                {
                    this->ChosenNumber = value;

                    SDL_Log("[CHOSEN NUMBER] <--- %d", value);
                }
                else if (!this->IsANumber(variable))
                {
                    this->Variables[variable] = std::to_string(value);

                    SDL_Log("[VARIABLE %s] <--- %d", variable.c_str(), value);
                }
            }
        }

        // get numeric value of a variable
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

            SDL_Log("[NUMBER %s] ---> %d", variable.c_str(), value);

            return value;
        }

        // check if item is in the list
        bool IsValid(std::vector<std::string> list, std::string item)
        {
            auto result = BloodSword::Has(list, item);

            SDL_Log("[CHECK %s] IS %s", item.c_str(), (result ? "VALID" : "INVALID"));

            return result;
        }

        // math operations
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

                        SDL_Log("[MATH] %s %s %s = %d", first.c_str(), operation.c_str(), second.c_str(), value_first);

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

                    SDL_Log("[IF] %s %s %s IS %s", first.c_str(), operation.c_str(), second.c_str(), (result ? "TRUE" : "FALSE"));
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

                    SDL_Log("[IF] %s %s %s IS %s", first.c_str(), operation.c_str(), second.c_str(), (result ? "TRUE" : "FALSE"));
                }
            }

            return result;
        }

        // erase variable
        void Remove(std::string variable)
        {
            if (this->IsPresent(variable))
            {
                this->Variables.erase(variable);

                if (!this->IsPresent(variable))
                {
                    SDL_Log("[VARIABLE %s] REMOVED", variable.c_str());
                }
            }
        }
    };

    // other characters (in the book that can be added to the party)
    Party::Base Characters = Party::Base();

    // initialize party from json data
    Party::Base Initialize(nlohmann::json &data)
    {
        auto party = Party::Base();

        if (!data.is_null())
        {
            party.Load(data);
        }

        return party;
    }

    // get party data as json
    nlohmann::json Data(Party::Base &party)
    {
        nlohmann::json data;

        data["module"] = !party.Module.empty() ? party.Module : "DEFAULT";

        if (Book::IsDefined(party.Location))
        {
            data["location"] = Book::Data(party.Location);
        }

        if (Book::IsDefined(party.PreviousLocation))
        {
            data["previous_location"] = Book::Data(party.PreviousLocation);
        }

        if (Book::IsDefined(party.SaveLocation))
        {
            data["save_location"] = Book::Data(party.SaveLocation);
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
            data["cards"] = Items::HandData(party.Cards);
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

        // rogue mode
        data["room"] = party.Room;

        data["x"] = party.X;

        data["y"] = party.Y;

        data["fov"] = party.FieldOfView;

        return data;
    }

    // save party to file
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

    // load party from file
    Party::Base Load(const char *filename, const char *name)
    {
        auto party = Party::Base();

        std::ifstream ifs(filename);

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            party = Party::Initialize(data[std::string(name)]);

            ifs.close();

            SDL_Log("[LOADED] %d characters", party.Count());
        }

        return party;
    }

    // load named party from file
    Party::Base Load(std::string filename, std::string name)
    {
        return Party::Load(filename.c_str(), name.c_str());
    }

    // load party from zip archive
    Party::Base Load(const char *filename, const char *name, const char *zip_file)
    {
        auto party = Party::Base();

        if (zip_file == nullptr)
        {
            Party::Load(filename, name);
        }
        else
        {
            auto ifs = ZipFile::Read(zip_file, filename);

            if (!ifs.empty())
            {
                auto data = nlohmann::json::parse(ifs);

                party = Party::Initialize(data[std::string(name)]);

                ifs.clear();

                SDL_Log("[LOADED] %d characters", party.Count());
            }
        }

        return party;
    }

    // load named party from zip archive
    Party::Base Load(std::string filename, std::string name, std::string zip_file)
    {
        return Party::Load(filename.c_str(), name.c_str(), zip_file.empty() ? nullptr : zip_file.c_str());
    }
}

#endif
