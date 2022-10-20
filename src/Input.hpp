#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include <iostream>
#include <vector>

#include "Controls.hpp"
#include "Graphics.hpp"

// functions for handling user input
namespace BloodSword::Input
{
    // initialize any connected gamepads
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

    // wait for button (gamepad/mouse) click or the return key
    void WaitForNext(Graphics::Base &graphics, Scene::Base &scene)
    {
        SDL_Event result;

        while (true)
        {
            Graphics::Render(graphics, scene);

            Graphics::DisplayVersion(graphics);

            Graphics::Scanlines(graphics);

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
                    Graphics::Scanlines(graphics);

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

    // render scene and overlays then wait for user input
    Controls::User WaitForInput(Graphics::Base &graphics, Scene::Base &scene, Scene::Base &overlay, Controls::User input, bool isDialog = false, bool blur = false)
    {
        SDL_Event result;

        auto sensitivity = 32000;

        if (!isDialog)
        {
            Graphics::Render(graphics, scene, input);

            Graphics::Overlay(graphics, overlay);

            Graphics::Scanlines(graphics);
        }
        else
        {
            Graphics::Dialog(graphics, scene, overlay, input, blur);
        }

        Graphics::DisplayVersion(graphics);

        Graphics::Scanlines(graphics);

        SDL_RenderPresent(graphics.Renderer);

        SDL_PollEvent(&result);

        input.Selected = false;

        input.Up = false;

        input.Down = false;

        auto &controls = !isDialog ? scene.Controls : overlay.Controls;

        if (result.type == SDL_QUIT)
        {
            input.Quit = true;
        }
        else if (result.type == SDL_WINDOWEVENT)
        {
            if (result.window.event == SDL_WINDOWEVENT_RESTORED || result.window.event == SDL_WINDOWEVENT_MAXIMIZED || result.window.event == SDL_WINDOWEVENT_SHOWN)
            {
                Graphics::Scanlines(graphics);

                SDL_RenderPresent(graphics.Renderer);
            }
        }
        else if (result.type == SDL_CONTROLLERDEVICEADDED)
        {
            Input::InitializeGamePads();
        }
        else if (result.type == SDL_KEYDOWN && !controls.empty())
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
                input.Current = controls[0].Id;
            }
            else if (result.key.keysym.sym == SDLK_TAB || result.key.keysym.sym == SDLK_KP_TAB || result.key.keysym.sym == SDL_SCANCODE_KP_TAB)
            {
                if (input.Current < 0)
                {
                    input.Current = controls[0].Id;
                }
                else if (input.Current == controls.size() - 1)
                {
                    input.Current = controls[0].Id;
                }
                else if (input.Current >= 0 && input.Current < controls.size() - 1)
                {
                    input.Current = controls[input.Current + 1].Id;
                }
            }
            else if (input.Current >= 0 && input.Current < controls.size())
            {
                if (result.key.keysym.sym == SDLK_LEFT)
                {
                    input.Current = controls[input.Current].Left;
                }
                else if (result.key.keysym.sym == SDLK_RIGHT)
                {
                    input.Current = controls[input.Current].Right;
                }
                else if (result.key.keysym.sym == SDLK_UP)
                {
                    input.Current = controls[input.Current].Up;
                }
                else if (result.key.keysym.sym == SDLK_DOWN)
                {
                    input.Current = controls[input.Current].Down;
                }
                else if (result.key.keysym.sym == SDLK_KP_ENTER || result.key.keysym.sym == SDLK_RETURN || result.key.keysym.sym == SDLK_RETURN2)
                {
                    input.Selected = true;
                }
            }
        }
        else if (result.type == SDL_CONTROLLERAXISMOTION && !controls.empty())
        {
            if (result.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
            {
                if (input.Current < 0)
                {
                    input.Current = controls[0].Id;
                }
                else if (result.caxis.value < -sensitivity)
                {
                    if (input.Current >= 0 && input.Current < controls.size())
                    {
                        input.Current = controls[input.Current].Left;
                    }
                }
                else if (result.caxis.value > sensitivity)
                {
                    if (input.Current >= 0 && input.Current < controls.size())
                    {
                        input.Current = controls[input.Current].Right;
                    }
                }
            }
            else if (result.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
            {
                if (input.Current < 0)
                {
                    input.Current = controls[0].Id;
                }
                else if (result.caxis.value < -sensitivity)
                {
                    if (input.Current >= 0 && input.Current < controls.size())
                    {
                        input.Current = controls[input.Current].Up;
                    }
                }
                else if (result.caxis.value > sensitivity)
                {
                    if (input.Current >= 0 && input.Current < controls.size())
                    {
                        input.Current = controls[input.Current].Down;
                    }
                }
            }
        }
        else if (result.type == SDL_CONTROLLERBUTTONDOWN && result.cbutton.button == SDL_CONTROLLER_BUTTON_A && !controls.empty())
        {
            input.Hold = true;

            if (input.Current >= 0 && input.Current < controls.size() && (controls[input.Current].Type == Controls::Type::SCROLL_UP || controls[input.Current].Type == Controls::Type::SCROLL_DOWN))
            {
                input.Selected = true;
            }
        }
        else if (result.type == SDL_CONTROLLERBUTTONUP && !controls.empty())
        {
            input.Selected = false;

            input.Hold = false;

            if (input.Current < 0)
            {
                input.Current = controls[0].Id;
            }
            else if (input.Current >= 0 && input.Current < controls.size())
            {
                if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
                {
                    input.Current = controls[input.Current].Left;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                {
                    input.Current = controls[input.Current].Right;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
                {
                    input.Current = controls[input.Current].Up;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
                {
                    input.Current = controls[input.Current].Down;
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

            for (auto &control : controls)
            {
                if (result.motion.x >= control.X && result.motion.x <= control.X + control.W - 1 && result.motion.y >= control.Y && result.motion.y <= control.Y + control.H - 1)
                {
                    input.Current = control.Id;
                }
            }
        }
        else if (result.type == SDL_MOUSEBUTTONDOWN && result.button.button == SDL_BUTTON_LEFT)
        {
            input.Hold = true;

            if (input.Current >= 0 && input.Current < controls.size() && (controls[input.Current].Type == Controls::Type::SCROLL_UP || controls[input.Current].Type == Controls::Type::SCROLL_DOWN))
            {
                input.Selected = true;
            }
        }
        else if (result.type == SDL_MOUSEBUTTONUP && result.button.button == SDL_BUTTON_LEFT)
        {
            if (input.Hold)
            {
                input.Hold = false;

                if (input.Current >= 0 && input.Current < controls.size())
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
        else if (input.Hold)
        {
            if (input.Current >= 0 && input.Hold < controls.size() && (controls[input.Current].Type == Controls::Type::SCROLL_UP || controls[input.Current].Type == Controls::Type::SCROLL_DOWN))
            {
                if (controls[input.Current].Type == Controls::Type::SCROLL_UP)
                {
                    input.Up = true;
                }
                else if (controls[input.Current].Type == Controls::Type::SCROLL_DOWN)
                {
                    input.Down = true;
                }

                SDL_Delay(100);
            }
        }

        if (input.Current >= 0 && input.Current < controls.size() && !input.Up && !input.Down)
        {
            input.Type = controls[input.Current].Type;
        }
        else
        {
            input.Type = Controls::Type::NONE;
        }

        return input;
    }

    // render scene then wait for user input
    Controls::User WaitForInput(Graphics::Base &graphics, Scene::Base &scene, Controls::User input)
    {
        auto overlay = Scene::Base();

        return WaitForInput(graphics, scene, overlay, input, false);
    }

    // check if user input is valid
    bool IsValid(std::vector<Controls::Base> &controls, Controls::User &input)
    {
        return (input.Current >= 0 && input.Current < controls.size());
    }

    // check if user input is valid
    bool IsValid(Scene::Base &scene, Controls::User &input)
    {
        return Input::IsValid(scene.Controls, input);
    }

    bool IsPlayer(Controls::User &input)
    {
        return (input.Type == Controls::Type::WARRIOR || input.Type == Controls::Type::TRICKSTER || input.Type == Controls::Type::SAGE || input.Type == Controls::Type::ENCHANTER);
    }

    bool IsEnemy(Controls::User &input)
    {
        return input.Type == Controls::Type::ENEMY;
    }

    // flush all events
    void Flush()
    {
        SDL_PumpEvents();

        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    }
}

#endif
