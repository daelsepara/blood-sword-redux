#ifndef __BATTLE_BOARD_HPP__
#define __BATTLE_BOARD_HPP__

#include "Fonts.hpp"
#include "Graphics.hpp"
#include "Interface.hpp"
#include "InterfaceBattle.hpp"

// framework for Battle Board
namespace BloodSword::BattleBoard
{
    void Menu(Graphics::Base &graphics, const char *battle_file)
    {
        auto party = Interface::CreateParty(graphics, Book::Number::BOOK1, false);

        auto battle = Battle::Load(battle_file);

        auto scene = Scene::Base();

        if (battle.IsDefined())
        {
            auto result = Interface::RenderBattle(graphics, battle, party);

            switch (result)
            {
            case Battle::Result::VICTORY:
                Interface::MessageBox(graphics, scene, Graphics::RichText("YOUR PARTY IS VICTORIOUS!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Transparent, Color::Active, 4, Color::Highlight, true);
                break;
            case Battle::Result::DEFEAT:
                Interface::MessageBox(graphics, scene, Graphics::RichText("YOUR PARTY HAS BEEN DEFEATED!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Transparent, Color::Highlight, 4, Color::Highlight, true);
                break;
            case Battle::Result::ENTHRALLED:
                Interface::MessageBox(graphics, scene, Graphics::RichText("ENEMIES HAVE BEEN SUBDUED!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Transparent, Color::Active, 4, Color::Highlight, true);
                break;
            case Battle::Result::FLEE:
                Interface::MessageBox(graphics, scene, Graphics::RichText("YOUR PARTY FLEES!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Transparent, Color::Active, 4, Color::Highlight, true);
                break;
            default:
                Interface::MessageBox(graphics, scene, Graphics::RichText("THE OUTCOME WAS INCONCLUSIVE!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Transparent, Color::Inactive, 4, Color::Highlight, true);
                break;
            }
        }
        else
        {
            Interface::MessageBox(graphics, scene, Graphics::RichText("BATTLE NOT LOADED!", Fonts::Normal, Color::Active, TTF_STYLE_NORMAL, 0), Color::Transparent, Color::Highlight, 4, Color::Highlight, true);
        }
    }
}

#endif
