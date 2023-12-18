#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "Attribute.hpp"
#include "Book.hpp"
#include "Character.hpp"
#include "Item.hpp"
#include "Map.hpp"
#include "Move.hpp"
#include "Party.hpp"
#include "Random.hpp"
#include "Spells.hpp"
#include "Generator.hpp"

// functions and classes for handling gameplay logic
namespace BloodSword::Engine
{
    // queue element
    class ScoreElement
    {
    public:
        // character is PLAYER or NPC
        Character::ControlType Type = Character::ControlType::NONE;

        // character ID within the party
        int Id = -1;

        // score (used in ranking)
        int Score = 0;

        ScoreElement(Character::ControlType type, int id, int score) : Type(type), Id(id), Score(score) {}
    };

    // Queue type
    typedef std::vector<Engine::ScoreElement> Queue;

    enum class RollStage
    {
        START,
        RESULT
    };

    // RNG engine
    auto Random = Random::Base();

    // initialize RNG
    void InitializeRNG()
    {
        // fix integer RNG to 1D6
        Random.UniformIntDistribution(1, 6);

        // fix floating point RNG to [0.0, 1.0)
        Random.UniformDistribution(0, 1.0);
    }

    class RollResult
    {
    public:
        std::vector<int> Rolls = {};

        int Sum = 0;

        RollResult(std::vector<int> rolls, int sum) : Rolls(rolls), Sum(sum) {}

        // empty result
        RollResult() {}
    };

    // roll number of dice and include modifiers
    Engine::RollResult Roll(int count, int modifier)
    {
        std::vector<int> rolls = {};

        auto sum = 0;

        for (auto i = 0; i < count; i++)
        {
            rolls.push_back(Engine::Random.NextInt());

            sum += rolls.back();
        }

        sum += modifier;

        return Engine::RollResult(rolls, std::max(0, sum));
    }

    // total character score
    int Score(Character::Base &character, Attribute::Type attribute, bool inbattle = false)
    {
        auto score = 0;

        auto value = std::min(character.Value(attribute), character.Maximum(attribute));

        auto modifier = character.Modifier(attribute) + character.Modifiers(attribute);

        if (attribute == Attribute::Type::ARMOUR)
        {
            score = modifier;
        }
        else if (attribute != Attribute::Type::DAMAGE)
        {
            if (inbattle && character.IsPlayer() && !character.IsArmed() && attribute == Attribute::Type::FIGHTING_PROWESS)
            {
                modifier -= 2;
            }

            score = value + modifier;
        }

        return std::max(0, score);
    }

    // check if character is still alive (and not enthralled)
    bool IsAlive(Character::Base &character)
    {
        return Engine::Score(character, Attribute::Type::ENDURANCE) > 0;
    }

    // count characters in party who are alive
    int Count(Party::Base &party)
    {
        auto live = 0;

        for (auto character = 0; character < party.Count(); character++)
        {
            live += Engine::IsAlive(party[character]) ? 1 : 0;
        }

        return live;
    }

    int Count(Party::Base &party, Character::ControlType control, Character::Status status)
    {
        auto count = 0;

        for (auto character = 0; character < party.Count(); character++)
        {
            count += party[character].ControlType == control && party[character].Is(status) ? 1 : 0;
        }

        return count;
    }

    // check if there is at least one character in the party still alive
    bool IsAlive(Party::Base &party)
    {
        auto live = Engine::Count(party);

        auto enthralled = Engine::Count(party, Character::ControlType::NPC, Character::Status::ENTHRALLED);

        return live > 0 && live > enthralled;
    }

    // check if the entire party is fleeing
    bool IsFleeing(Party::Base &party)
    {
        auto live = 0;

        auto fleeing = 0;

        for (auto character = 0; character < party.Count(); character++)
        {
            if (party[character].ControlType == Character::ControlType::PLAYER)
            {
                live += Engine::IsAlive(party[character]) ? 1 : 0;

                fleeing += Engine::IsAlive(party[character]) && party[character].Is(Character::Status::FLEEING) ? 1 : 0;
            }
        }

        return live > 0 && fleeing > 0 && fleeing >= live;
    }

    // sort queue
    void Sort(Engine::Queue &queue, bool descending = true)
    {
        // sort based on direction
        if (descending)
        {
            std::sort(queue.begin(), queue.end(), [](ScoreElement &a, ScoreElement &b) -> bool
                      { return a.Score > b.Score; });
        }
        else
        {
            std::sort(queue.begin(), queue.end(), [](ScoreElement &a, ScoreElement &b) -> bool
                      { return a.Score < b.Score; });
        }
    }

    // creates queue (order sequence of characters ranked according to attribute score)
    Engine::Queue Build(Party::Base &party, Party::Base &other, Attribute::Type attribute, bool inbattle = false, bool descending = true)
    {
        Engine::Queue queue = {};

        // add characters in party to queue
        for (auto i = 0; i < party.Count(); i++)
        {
            auto alive = Engine::IsAlive(party[i]);
            auto paralyzed = party[i].Is(Character::Status::PARALYZED);
            auto away = party[i].Is(Character::Status::AWAY);
            auto knockedout = party[i].Is(Character::Status::KNOCKED_OUT);
            auto battle = (inbattle && party[i].Is(Character::Status::IN_BATTLE)) || !inbattle;

            if (alive && !paralyzed && !away && battle)
            {
                queue.push_back(ScoreElement(party[i].ControlType, i, knockedout ? 1 : Engine::Score(party[i], attribute, inbattle)));
            }
        }

        // add characters from the other party to the queue
        for (auto i = 0; i < other.Count(); i++)
        {
            auto alive = Engine::IsAlive(other[i]);
            auto paralyzed = other[i].Is(Character::Status::PARALYZED);
            auto away = other[i].Is(Character::Status::AWAY);
            auto knockedout = other[i].Is(Character::Status::KNOCKED_OUT);
            auto battle = (inbattle && other[i].Is(Character::Status::IN_BATTLE)) || !inbattle;

            if (alive && !paralyzed && !away && battle)
            {
                queue.push_back(ScoreElement(other[i].ControlType, i, knockedout ? 1 : Engine::Score(other[i], attribute)));
            }
        }

        Engine::Sort(queue, descending);

        return queue;
    }

    // build queue of characters sorted according to attribute score
    Engine::Queue Build(Party::Base &party, Attribute::Type attribute, bool inbattle = false, bool descending = true)
    {
        auto empty = Party::Base();

        return Engine::Build(party, empty, attribute, inbattle, descending);
    }

    // search for a character in the queue
    Engine::Queue::iterator Find(Engine::Queue &queue, Character::ControlType control, int id)
    {
        auto found = queue.end();

        for (auto characters = queue.begin(); characters != queue.end(); characters++)
        {
            auto &character = *characters;

            if (character.Type == control && character.Id == id)
            {
                found = characters;

                break;
            }
        }

        return found;
    }

    // return the position of the character in the queue (-1 if character not in queue)
    int Order(Engine::Queue &queue, Character::ControlType &control, int id)
    {
        auto distance = -1;

        auto found = Engine::Find(queue, control, id);

        if (found != queue.end())
        {
            distance = found - queue.begin();
        }

        return distance;
    }

    // check if current character (in the queue) is player-controlled
    bool IsPlayer(Engine::Queue &queue, int &character)
    {
        return queue[character].Type == Character::ControlType::PLAYER;
    }

    // check if current character (in the queue) is non-player-controlled
    bool IsEnemy(Engine::Queue &queue, int &character)
    {
        return !Engine::IsPlayer(queue, character);
    }

    // move to next item in queue
    bool Next(Engine::Queue &queue, int &item)
    {
        auto first = false;

        item++;

        if (item >= queue.size())
        {
            item = 0;

            first = true;
        }

        return first;
    }

    // move to previous item in queue
    void Previous(Engine::Queue &queue, int &item)
    {
        item--;

        if (item < 0)
        {
            item = queue.size() - 1;
        }
    }

    // move character to end of the queue
    void MoveToEnd(Engine::Queue &queue, Character::ControlType control, int id)
    {
        auto search = Engine::Find(queue, control, id);

        if (search != queue.end())
        {
            std::rotate(search, std::next(search), queue.end());
        }
    }

    // move item to end of the queue
    void MoveToEnd(Engine::Queue &queue, int id)
    {
        if (id >= 0 && id < queue.size() - 1)
        {
            auto search = queue.begin() + id;

            std::rotate(search, std::next(search), queue.end());
        }
    }

    // build distance queue
    Engine::Queue Build(Map::Base &map, Party::Base &party, Point &src, bool inbattle = false, bool descending = false)
    {
        Engine::Queue queue = {};

        for (auto i = 0; i < party.Count(); i++)
        {
            auto alive = Engine::IsAlive(party[i]);
            auto away = party[i].Is(Character::Status::AWAY);
            auto battle = (inbattle && party[i].Is(Character::Status::IN_BATTLE)) || !inbattle;

            if (alive && !away && battle)
            {
                auto distance = -1;

                Point location;

                if (party[i].ControlType == Character::ControlType::PLAYER)
                {
                    location = map.Find(Map::Object::PLAYER, i);
                }
                else if (party[i].ControlType == Character::ControlType::NPC)
                {
                    location = map.Find(Map::Object::ENEMY, i);
                }

                distance = map.Distance(src, location);

                if (distance >= 0 && map.IsValid(location))
                {
                    queue.push_back(Engine::ScoreElement(party[i].ControlType, i, distance));
                }
            }
        }

        Engine::Sort(queue, descending);

        return queue;
    }

    // build queue based on path to target (alternative to the distance-based (between src and dst) approach
    Engine::Queue MoveTargets(Map::Base &map, Party::Base &party, Point &src, bool inbattle = false, bool descending = false)
    {
        Engine::Queue queue = {};

        if (map.IsValid(src))
        {
            for (auto i = 0; i < party.Count(); i++)
            {
                auto alive = Engine::IsAlive(party[i]);
                auto away = party[i].Is(Character::Status::AWAY);
                auto battle = (inbattle && party[i].Is(Character::Status::IN_BATTLE)) || !inbattle;

                if (alive && !away && battle)
                {
                    auto distance = -1;

                    Point location;

                    if (party[i].ControlType == Character::ControlType::PLAYER)
                    {
                        location = map.Find(Map::Object::PLAYER, i);
                    }
                    else if (party[i].ControlType == Character::ControlType::NPC)
                    {
                        location = map.Find(Map::Object::ENEMY, i);
                    }

                    auto path = Move::FindPath(map, src, location, map[src].IsEnemy());

                    distance = Move::Count(map, path, map[src].IsEnemy());

                    if (distance > 0 && map.IsValid(location) && location != src)
                    {
                        queue.push_back(Engine::ScoreElement(party[i].ControlType, i, distance));
                    }
                }
            }
        }

        Engine::Sort(queue, descending);

        return queue;
    }

    // build shot targets
    Engine::Queue RangedTargets(Map::Base &map, Party::Base &party, Point &src, bool inbattle = false, bool descending = false)
    {
        Engine::Queue queue = {};

        if (map.IsValid(src))
        {
            for (auto i = 0; i < party.Count(); i++)
            {
                auto alive = Engine::IsAlive(party[i]);
                auto away = party[i].Is(Character::Status::AWAY);
                auto battle = (inbattle && party[i].Is(Character::Status::IN_BATTLE)) || !inbattle;

                if (alive && !away && battle)
                {
                    auto distance = -1;

                    Point location;

                    if (party[i].ControlType == Character::ControlType::PLAYER)
                    {
                        location = map.Find(Map::Object::PLAYER, i);
                    }
                    else if (party[i].ControlType == Character::ControlType::NPC)
                    {
                        location = map.Find(Map::Object::ENEMY, i);
                    }

                    distance = map.Distance(src, location);

                    if (distance > 1 && map.IsValid(location) && location != src)
                    {
                        queue.push_back(Engine::ScoreElement(party[i].ControlType, i, distance));
                    }
                }
            }
        }

        Engine::Sort(queue, descending);

        return queue;
    }

    // build queue endurance score for adjacent opponents
    Engine::Queue FightTargets(Map::Base &map, Party::Base &party, Point &src, bool inbattle = false, bool descending = false)
    {
        Engine::Queue queue = {};

        if (map.IsValid(src))
        {
            for (auto i = 0; i < party.Count(); i++)
            {
                auto alive = Engine::IsAlive(party[i]);
                auto away = party[i].Is(Character::Status::AWAY);
                auto battle = (inbattle && party[i].Is(Character::Status::IN_BATTLE)) || !inbattle;

                if (alive && !away && battle)
                {
                    auto distance = -1;

                    Point location;

                    if (party[i].ControlType == Character::ControlType::PLAYER)
                    {
                        location = map.Find(Map::Object::PLAYER, i);
                    }
                    else if (party[i].ControlType == Character::ControlType::NPC)
                    {
                        location = map.Find(Map::Object::ENEMY, i);
                    }

                    distance = map.Distance(src, location);

                    if (distance == 1 && map.IsValid(location) && location != src)
                    {
                        queue.push_back(Engine::ScoreElement(party[i].ControlType, i, Engine::Score(party[i], Attribute::Type::ENDURANCE, inbattle)));
                    }
                }
            }
        }

        Engine::Sort(queue, descending);

        return queue;
    }

    // check if control is a spell
    bool IsSpell(Controls::Type type)
    {
        auto isspell = false;

        for (auto spell = Controls::Spells.begin(); spell != Controls::Spells.end(); spell++)
        {
            if (*spell == type)
            {
                isspell = true;

                break;
            }
        }

        return isspell;
    }

    // character gains/loses endurance
    bool GainEndurance(Character::Base &character, int gain, bool inbattle = false)
    {
        auto endurance = Engine::Score(character, Attribute::Type::ENDURANCE, inbattle);

        endurance -= gain;

        endurance = std::max(0, endurance);

        character.Set(Attribute::Type::ENDURANCE, endurance, 0);

        return Engine::IsAlive(character);
    }

    // cooldown status effects
    bool CoolDown(Character::Base &character)
    {
        auto update = false;

        // cool down status effects
        if (character.Is(Character::Status::DEFENDING))
        {
            // change DEFENDING to DEFENDED
            character.Remove(Character::Status::DEFENDING);

            character.Add(Character::Status::DEFENDED, 1);

            update = true;
        }
        else
        {
            BloodSword::IntMapping<Character::Status> Active = {};

            // cooldown other status effects
            for (auto &status : character.Status)
            {
                if (status.second < 0)
                {
                    Active[status.first] = -1;
                }
                else if (status.second > 0)
                {
                    update = true;

                    status.second--;

                    if (status.second > 0)
                    {
                        Active[status.first] = status.second;
                    }
                }
            }

            if (Active.size() != character.Status.size())
            {
                update = true;
            }

            character.Status = Active;
        }

        return update;
    }

    // check if character can flee
    bool CanFlee(Map::Base &map, Party::Base &party, int character)
    {
        auto flee = false;

        auto src = map.Find(Map::Object::PLAYER, character);

        if (!src.IsNone())
        {
            if (map[src].IsExit())
            {
                flee = true;
            }
            else
            {
                for (auto &direction : Map::Directions)
                {
                    auto neighbor = src + direction;

                    if (map.IsValid(neighbor) && map[neighbor].Type == Map::Object::EXIT)
                    {
                        flee = true;

                        break;
                    }
                }
            }

            // check if character is adjacent to a team mate who can flee
            if (!flee && party.Count() > 1)
            {
                for (auto other = 0; other < party.Count(); other++)
                {
                    if (character != other)
                    {
                        auto next = map.Find(Map::Object::PLAYER, other);

                        if (!next.IsNone() && map.Distance(src, next) == 1)
                        {
                            if (map[next].Type == Map::Object::EXIT)
                            {
                                flee = true;

                                break;
                            }
                            else
                            {
                                for (auto &direction : Map::Directions)
                                {
                                    auto neighbor = next + direction;

                                    if (map.IsValid(neighbor) && map[neighbor].Type == Map::Object::EXIT)
                                    {
                                        flee = true;

                                        break;
                                    }
                                }

                                if (flee)
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        return flee;
    }
}

#endif
