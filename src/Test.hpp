#ifndef __TEST_HPP__
#define __TEST_HPP__

#include <SDL.h>

#include "Asset.hpp"
#include "Engine.hpp"
#include "Fonts.hpp"
#include "Graphics.hpp"
#include "Input.hpp"

// framework for testing game subsystems
namespace BloodSword::Test
{
    void Render(Graphics::Base &graphics)
    {
        if (graphics.Renderer)
        {
            auto x = 128;
            auto y = 64;
            auto w = 64;
            auto s = 8;

            // sprites
            std::vector<Graphics::SceneElements> Scene = {
                {Graphics::SceneElements(Asset::Get(Asset::Type::WARRIOR), x, y)},
                {Graphics::SceneElements(Asset::Get(Asset::Type::TRICKSTER), x + w + s, y)},
                {Graphics::SceneElements(Asset::Get(Asset::Type::SAGE), x + 2 * (w + s), y)},
                {Graphics::SceneElements(Asset::Get(Asset::Type::ENCHANTER), x + 3 * (w + s), y)},
            };

            // text
            std::string Text = "Every thirteen lunar months the Magi of Krarth hold a desperate contest to see which of them will rule that bleak and icy land. Teams of daring adventurers are sent down into the labyrinths that lie beneath the tundra, each searching for the Emblem of Victory that will win power for their patron.\n\nOnly one team can prevail. The others must die.";

            auto text = Graphics::CreateText(Text.c_str(), Fonts::Normal, {96, 96, 96, 255}, TTF_STYLE_NORMAL, 8 * (w + s) - s);

            Scene.push_back(Graphics::SceneElements(text, x + 4 * (w + s), y));

            Graphics::Scene TestScene = Graphics::Scene(Scene, 0);

            Input::WaitForNext(graphics, TestScene);

            SDL_FreeSurface(text);

            text = NULL;
        }
    }

    void Controls(Graphics::Base &graphics)
    {
    }
}

#endif