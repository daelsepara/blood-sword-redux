#ifndef __MESSAGES_HPP__
#define __MESSAGES_HPP__

#include "Graphics.hpp"
#include "RichText.hpp"

namespace BloodSword::Interface
{
    Graphics::TextList Text = {
        Graphics::RichText("SELECT OPPONENT", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("SELECT TARGET", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("SELECT DESTINATION", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("ENTHRALMENT BROKEN!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("THERE ARE NEARBY ENEMIES!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("CANNOT MOVE THERE!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("CASTER MUST BE ADJACENT TO TARGET!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("CASTING ATTEMPT WAS UNSUCCESSFUL!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("YOU CANNOT FLEE THIS BATTLE!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("TARGET IS IMMUNE TO RANGED ATTACK!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("TARGET IS IMMUNE TO THIS SPELL!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("YOU DO NOT HAVE THIS SKILL!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("NOT ENOUGH ENDURANCE!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("TARGET HEALED!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("INVALID TARGET!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("YOU CANNOT DO THAT WHILE IN BATTLE!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("YOU CANNOT PERFORM THIS ACTION!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("HEALING ATTEMPT FAILED!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("EVERYONE IS AT MAXIMUM ENDURANCE!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("THAT PLAYER IS NOT WOUNDED!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("GAME OVER!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("NOT IMPLEMENTED YET", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("THANKS FOR PLAYING!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("EVERYONE IS INCAPACITATED", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("YOU WERE INCAPACITATED", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0),
        Graphics::RichText("TARGET IS DEAD!", Fonts::ID::NORMAL, Color::S(Color::Active), TTF_STYLE_NORMAL, 0)};

    BloodSword::Textures Message = {};

    const int MSG_OPPONENT = 0;

    const int MSG_TARGET = 1;

    const int MSG_DEST = 2;

    const int MSG_ENTHRAL = 3;

    const int MSG_NEARBY = 4;

    const int MSG_MOVE = 5;

    const int MSG_ADJACENT = 6;

    const int MSG_CAST = 7;

    const int MSG_FLEE = 8;

    const int MSG_RANGED = 9;

    const int MSG_SPELL = 10;

    const int MSG_SKILL = 10;

    const int MSG_ENDURANCE = 12;

    const int MSG_HEALED = 13;

    const int MSG_INVALID = 14;

    const int MSG_BATTLE = 15;

    const int MSG_ACTION = 16;

    const int MSG_HEALING = 17;

    const int MSG_MAX = 18;

    const int MSG_WOUND = 19;

    const int MSG_OVER = 20;

    const int MSG_IMPLEMENT = 21;

    const int MSG_PLAY = 22;

    const int MSG_DEAD = 23;

    const int MSG_DIED = 24;

    const int MSG_LOST = 25;

    void InitializeMessages(Graphics::Base &graphics)
    {
        // initialize battle messages
        Graphics::SetFonts(Text);

        Free(Message);

        Message = Graphics::CreateText(graphics, Text);
    }

    void FreeMessages()
    {
        Free(Message);
    }
}
#endif
