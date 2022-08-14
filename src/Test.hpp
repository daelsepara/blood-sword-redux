#ifndef __TEST_HPP__
#define __TEST_HPP__

#include <SDL.h>

#include "Asset.hpp"
#include "Engine.hpp"
#include "Graphics.hpp"
#include "Input.hpp"

// framework for testing game subsystems
namespace BloodSword::Test
{
    void RenderPlayers(Graphics::Base &graphics)
    {
        if (graphics.Renderer)
        {
            auto x = 128;
            auto y = 64;
            auto w = 64;
            auto s = 8;

            std::vector<Graphics::SceneElements> Players = {
                {Graphics::SceneElements(Asset::Get(Asset::Type::WARRIOR), x, y)},
                {Graphics::SceneElements(Asset::Get(Asset::Type::TRICKSTER), x + w + s, y)},
                {Graphics::SceneElements(Asset::Get(Asset::Type::SAGE), x + 2 * (w + s), y)},
                {Graphics::SceneElements(Asset::Get(Asset::Type::ENCHANTER), x + 3 * (w + s), y)},
            };

            Graphics::Scene TestScene = Graphics::Scene(Players, 0);

            Input::WaitForNext(graphics, TestScene);
        }
    }
}

#endif