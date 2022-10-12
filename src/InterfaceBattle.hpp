#ifndef __INTERFACE_BATTLE_HPP__
#define __INTERFACE_BATTLE_HPP__

#include "Battle.hpp"
#include "Interface.hpp"

namespace BloodSword::Interface
{
    // fight battle
    Battle::Result Battle(Graphics::Base &graphics, Battle::Base &battle, Party::Base &party)
    {
        auto result = Battle::Result::NONE;

        int round = 0;

        Engine::Queue order = {};

        // start with current character
        auto combatant = 0;

        if (battle.Duration != 0 && battle.Opponents.Count() > 0 && party.Count() > 0)
        {
            while (round < battle.Duration || battle.Duration == Battle::Unlimited)
            {
                if (round == 0 && battle.Is(Battle::Condition::AMBUSH_PLAYER))
                {
                    // players get a free initial turn
                    order = Engine::Build(party, Attribute::Type::AWARENESS, true, true);
                }
                else if (round == 0 && battle.Is(Battle::Condition::AMBUSH_NPC))
                {
                    // enemy combatants get a free initial turn
                    order = Engine::Build(battle.Opponents, Attribute::Type::AWARENESS, true, true);
                }
                else
                {
                    // otherwise create battle order (default)
                    order = Engine::Build(party, battle.Opponents, Attribute::Type::AWARENESS, true, true);
                }

                // start of round effects

                auto next = false;

                while (!next)
                {
                    // start of character turn
                    if (Engine::IsPlayer(order, combatant))
                    {
                        Engine::CoolDown(party[order[combatant].ID]);
                    }
                    else
                    {
                        Engine::CoolDown(battle.Opponents[order[combatant].ID]);
                    }

                    // main loop

                    // move to next character in battle order
                    next = Engine::Next(order, combatant);
                }

                // end of round effects

                // move to next round
                round++;
            }
        }

        return result;
    }
}

#endif
