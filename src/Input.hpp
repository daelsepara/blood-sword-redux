#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include <iostream>
#include <vector>
#include <SDL.h>

#include "Controls.hpp"
#include "Graphics.hpp"

namespace BloodSword::Input
{
    int InitializeGamePads()
    {
        if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 1)
        {
            if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0)
            {
                std::cerr << "SDL could not initialize gamecontroller! SDL_Error: " << SDL_GetError() << std::endl;
            }
        }

        auto nJoysticks = SDL_NumJoysticks();

        auto numGamepads = 0;

        // Count how many controllers there are
        for (auto i = 0; i < nJoysticks; i++)
        {
            if (SDL_IsGameController(i))
            {
                numGamepads++;
            }
        }

        // If we have some controllers attached
        if (numGamepads > 0)
        {
            for (auto i = 0; i < numGamepads; i++)
            {
                // Open the controller and add it to our list
                auto pad = SDL_GameControllerOpen(i);

                if (SDL_GameControllerGetAttached(pad) != 1)
                {
                    std::cerr << "Game pad not attached! SDL_Error: " << SDL_GetError() << std::endl;
                }
            }

            SDL_GameControllerEventState(SDL_ENABLE);
        }

        return numGamepads;
    }

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
            else if (result.type == SDL_CONTROLLERDEVICEADDED)
            {
                Input::InitializeGamePads();
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
}

#endif