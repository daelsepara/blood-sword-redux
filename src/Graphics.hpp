#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <iostream>

// Using SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

namespace BloodSword::Graphics
{
    class Base
    {
    public:
        // window and renderer
        SDL_Window *Window = NULL;

        SDL_Renderer *Renderer = NULL;

        // pixel dimensions
        int Width = 1280;
        int Height = 800;

        Base()
        {
        }
    };

    void CreateWindow(Uint32 flags, const char *title, Base &graphics)
    {
        // The window and renderer we'll be rendering to
        graphics.Window = NULL;
        graphics.Renderer = NULL;

        if (SDL_Init(flags) < 0)
        {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            SDL_DisplayMode mode;

            SDL_GetCurrentDisplayMode(0, &mode);

            graphics.Width = (mode.w);

            graphics.Height = (mode.h);

            SDL_CreateWindowAndRenderer(graphics.Width, graphics.Height, (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), &graphics.Window, &graphics.Renderer);

            if (graphics.Renderer)
            {
                SDL_SetRenderDrawBlendMode(graphics.Renderer, SDL_BLENDMODE_NONE);
            }

            if (!graphics.Window || !graphics.Renderer)
            {
                std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            }
            else if (graphics.Window)
            {
                SDL_SetWindowTitle(graphics.Window, title);
            }
        }
    }

    void Initialize(Base &graphics, const char *title)
    {
        IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

        TTF_Init();

        CreateWindow(SDL_INIT_VIDEO | SDL_INIT_AUDIO, title, graphics);
    }

    void Quit(Base &graphics)
    {
        if (graphics.Renderer)
        {
            SDL_DestroyRenderer(graphics.Renderer);

            graphics.Renderer = NULL;
        }

        if (graphics.Window)
        {
            SDL_DestroyWindow(graphics.Window);

            graphics.Window = NULL;
        }

        TTF_Quit();

        IMG_Quit();

        if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) & SDL_INIT_GAMECONTROLLER)
        {
            SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
        }

        if (SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO)
        {
            SDL_QuitSubSystem(SDL_INIT_AUDIO);
        }

        if (SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO)
        {
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }

        SDL_Quit();
    }
}

#endif