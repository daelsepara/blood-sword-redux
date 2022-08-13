#ifndef __TEST_HPP__
#define __TEST_HPP__

#include <SDL.h>

#include "Asset.hpp"
#include "Engine.hpp"
#include "Graphics.hpp"

// framework for testing game subsystems
namespace BloodSword::Test
{
    void WaitForNext(Graphics::Base &graphics, Graphics::Scene &scene)
    {
        SDL_Event result;

        while (true)
        {
            Graphics::RenderScene(graphics, scene);

            SDL_RenderPresent(graphics.Renderer);

            SDL_PollEvent(&result);

            if (result.type == SDL_QUIT)
            {
                break;
            }
            else if (result.type == SDL_WINDOWEVENT)
            {
                if (result.window.event == SDL_WINDOWEVENT_RESTORED || result.window.event == SDL_WINDOWEVENT_MAXIMIZED || result.window.event == SDL_WINDOWEVENT_SHOWN)
                {
                    SDL_RenderPresent(graphics.Renderer);
                }
            }
            else if (result.type == SDL_KEYDOWN)
            {
                if (result.key.keysym.sym == SDLK_KP_ENTER || result.key.keysym.sym == SDLK_RETURN || result.key.keysym.sym == SDLK_RETURN2)
                {
                    break;
                }
            }
            else if (result.type == SDL_CONTROLLERBUTTONUP)
            {
                if (result.cbutton.button == SDL_CONTROLLER_BUTTON_A)
                {
                    break;
                }
            }
            else if (result.type == SDL_MOUSEBUTTONUP && result.button.button == SDL_BUTTON_LEFT)
            {
                break;
            }
        }
    }

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

            Test::WaitForNext(graphics, TestScene);
        }
    }
}

#endif