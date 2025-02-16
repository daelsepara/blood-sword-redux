#ifndef __BATTLE_BOARD_HPP__
#define __BATTLE_BOARD_HPP__

#include "Interface.hpp"
#include "InterfaceBattle.hpp"

// framework for Battle Board
namespace BloodSword::BattleBoard
{
    void Menu(Graphics::Base &graphics)
    {
        auto party = Interface::CreateParty(graphics, Book::Number::BOOK1, false);
    }
}

#endif