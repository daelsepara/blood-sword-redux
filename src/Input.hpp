#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include <iostream>
#include <vector>

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

        // count how many controllers there are
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

    void WaitForNext(Graphics::Base &graphics, Scene::Base &scene)
    {
        SDL_Event result;

        while (true)
        {
            Graphics::Render(graphics, scene);

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

    Controls::User WaitForInput(Graphics::Base &graphics, Scene::Base &scene, Controls::User input)
    {
        SDL_Event result;

        auto sensitivity = 32000;

        Graphics::Render(graphics, scene, input);

        SDL_RenderPresent(graphics.Renderer);

        SDL_PollEvent(&result);

        input.Selected = false;

        input.Up = false;

        input.Down = false;

        if (result.type == SDL_QUIT)
        {
            input.Quit = true;
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
            if (result.key.keysym.sym == SDLK_PAGEUP)
            {
                input.Up = true;

                input.Current = -1;
            }
            else if (result.key.keysym.sym == SDLK_PAGEDOWN)
            {
                input.Down = true;

                input.Current = -1;
            }
            else if (input.Current < 0)
            {
                input.Current = scene.Controls[0].ID;
            }
            else if (result.key.keysym.sym == SDLK_TAB || result.key.keysym.sym == SDLK_KP_TAB || result.key.keysym.sym == SDL_SCANCODE_KP_TAB)
            {
                if (input.Current < 0)
                {
                    input.Current = scene.Controls[0].ID;
                }
                else if (input.Current == scene.Controls.size() - 1)
                {
                    input.Current = scene.Controls[0].ID;
                }
                else if (input.Current >= 0 && input.Current < scene.Controls.size() - 1)
                {
                    input.Current = scene.Controls[input.Current + 1].ID;
                }
            }
            else if (input.Current >= 0 && input.Current < scene.Controls.size())
            {
                if (result.key.keysym.sym == SDLK_LEFT)
                {
                    input.Current = scene.Controls[input.Current].Left;
                }
                else if (result.key.keysym.sym == SDLK_RIGHT)
                {
                    input.Current = scene.Controls[input.Current].Right;
                }
                else if (result.key.keysym.sym == SDLK_UP)
                {
                    input.Current = scene.Controls[input.Current].Up;
                }
                else if (result.key.keysym.sym == SDLK_DOWN)
                {
                    input.Current = scene.Controls[input.Current].Down;
                }
                else if (result.key.keysym.sym == SDLK_KP_ENTER || result.key.keysym.sym == SDLK_RETURN || result.key.keysym.sym == SDLK_RETURN2)
                {
                    input.Selected = true;
                }
            }
        }
        else if (result.type == SDL_CONTROLLERAXISMOTION)
        {
            if (result.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
            {
                if (input.Current < 0)
                {
                    input.Current = scene.Controls[0].ID;
                }
                else if (result.caxis.value < -sensitivity)
                {
                    if (input.Current >= 0 && input.Current < scene.Controls.size())
                    {
                        input.Current = scene.Controls[input.Current].Left;
                    }
                }
                else if (result.caxis.value > sensitivity)
                {
                    if (input.Current >= 0 && input.Current < scene.Controls.size())
                    {
                        input.Current = scene.Controls[input.Current].Right;
                    }
                }
            }
            else if (result.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
            {
                if (input.Current < 0)
                {
                    input.Current = scene.Controls[0].ID;
                }
                else if (result.caxis.value < -sensitivity)
                {
                    if (input.Current >= 0 && input.Current < scene.Controls.size())
                    {
                        input.Current = scene.Controls[input.Current].Up;
                    }
                }
                else if (result.caxis.value > sensitivity)
                {
                    if (input.Current >= 0 && input.Current < scene.Controls.size())
                    {
                        input.Current = scene.Controls[input.Current].Down;
                    }
                }
            }
        }
        else if (result.type == SDL_CONTROLLERBUTTONDOWN && result.cbutton.button == SDL_CONTROLLER_BUTTON_A)
        {
            input.Hold = true;

            if (input.Current >= 0 && input.Current < scene.Controls.size() && (scene.Controls[input.Current].Type == Controls::Type::SCROLL_UP || scene.Controls[input.Current].Type == Controls::Type::SCROLL_DOWN))
            {
                input.Selected = true;
            }
        }
        else if (result.type == SDL_CONTROLLERBUTTONUP)
        {
            input.Selected = false;

            input.Hold = false;

            if (input.Current < 0)
            {
                input.Current = scene.Controls[0].ID;
            }
            else if (input.Current >= 0 && input.Current < scene.Controls.size())
            {
                if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
                {
                    input.Current = scene.Controls[input.Current].Left;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                {
                    input.Current = scene.Controls[input.Current].Right;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
                {
                    input.Current = scene.Controls[input.Current].Up;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
                {
                    input.Current = scene.Controls[input.Current].Down;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_A)
                {
                    input.Selected = true;
                }
            }
        }
        else if (result.type == SDL_MOUSEMOTION)
        {
            input.Hold = false;

            input.Current = -1;

            for (auto i = 0; i < scene.Controls.size(); i++)
            {
                if (result.motion.x >= scene.Controls[i].X && result.motion.x <= scene.Controls[i].X + scene.Controls[i].W - 1 && result.motion.y >= scene.Controls[i].Y && result.motion.y <= scene.Controls[i].Y + scene.Controls[i].H - 1)
                {
                    input.Current = scene.Controls[i].ID;
                }
            }
        }
        else if (result.type == SDL_MOUSEBUTTONDOWN && result.button.button == SDL_BUTTON_LEFT)
        {
            input.Hold = true;

            if (input.Current >= 0 && input.Current < scene.Controls.size() && (scene.Controls[input.Current].Type == Controls::Type::SCROLL_UP || scene.Controls[input.Current].Type == Controls::Type::SCROLL_DOWN))
            {
                input.Selected = true;
            }
        }
        else if (result.type == SDL_MOUSEBUTTONUP && result.button.button == SDL_BUTTON_LEFT)
        {
            if (input.Hold)
            {
                input.Hold = false;

                if (input.Current >= 0 && input.Current < scene.Controls.size())
                {
                    input.Selected = true;
                }
            }
        }
        else if (result.type == SDL_MOUSEWHEEL)
        {
            input.Current = -1;

            input.Selected = false;

            if (result.wheel.y < 0 || result.wheel.x < 0)
            {
                input.Down = true;
            }
            else
            {
                input.Up = true;
            }
        }

        if (input.Current >= 0 && input.Current < scene.Controls.size() && !input.Up && !input.Down)
        {
            input.Type = scene.Controls[input.Current].Type;
        }
        else
        {
            input.Type = Controls::Type::NONE;
        }

        return input;
    }
}

#endif
