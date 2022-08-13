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
    Uint8 R(Uint32 c) { return (Uint8)((c & 0xFF0000) >> 16); }
    Uint8 G(Uint32 c) { return (Uint8)((c & 0x00FF00) >> 8); }
    Uint8 B(Uint32 c) { return (Uint8)(c & 0x0000FF); }
    Uint8 A(Uint32 c) { return (Uint8)(c >> 24); }
    Uint32 O(Uint32 c, Uint8 a) { return ((c & ((Uint32)0x00FFFFFF)) | ((Uint32)(a << 24))); }

    class SceneElements
    {
    public:
        SDL_Surface *Surface;

        int X = 0;

        int Y = 0;

        int Bounds = 0;

        int Offset = 0;

        SceneElements(SDL_Surface *surface, int x, int y, int bounds, int offset)
        {
            if (surface)
            {
                Surface = surface;

                X = x;

                Y = y;

                Bounds = bounds;

                Offset = offset;
            }
        }

        SceneElements(SDL_Surface *surface, int x, int y)
        {
            if (surface)
            {
                Surface = surface;

                X = x;

                Y = y;

                Bounds = surface->h;

                Offset = 0;
            }
        }
    };

    class Scene
    {
    public:
        Uint32 Background = 0;

        std::vector<Graphics::SceneElements> Elements = {};

        void Set(Uint32 background)
        {
            this->Background = background;
        }

        void Set(std::vector<Graphics::SceneElements> elements)
        {
            this->Elements = elements;
        }

        Scene(std::vector<Graphics::SceneElements> elements, Uint32 background)
        {
            this->Set(elements);

            this->Set(background);
        }

        Scene(Uint32 background)
        {
            this->Set(background);
        }

        Scene()
        {
        }

        void Clear()
        {
            this->Set(0);

            this->Elements.clear();
        }

        void Add(Graphics::SceneElements element)
        {
            this->Elements.push_back(element);
        }
    };

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
        // the window and renderer we'll be rendering to
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

    // initialize graphics subsystem
    void Initialize(Base &graphics, const char *title)
    {
        Graphics::CreateWindow(SDL_INIT_VIDEO | SDL_INIT_AUDIO, title, graphics);

        IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

        TTF_Init();
    }

    void FillWindow(Graphics::Base &graphics, Uint32 color)
    {
        SDL_SetRenderDrawColor(graphics.Renderer, R(color), G(color), B(color), A(color));

        SDL_RenderClear(graphics.Renderer);
    }

    // render a portion of the image on bounded surface within the specified window
    void Render(Base &graphics, SDL_Surface *image, int x, int y, int bounds, int offset)
    {
        if (graphics.Renderer && image)
        {
            SDL_Rect src;
            src.w = image->w;
            src.h = std::min(image->h, bounds);
            src.y = offset;
            src.x = 0;

            SDL_Rect dst;
            dst.w = image->w;
            dst.h = std::min(image->h, bounds);
            dst.x = x;
            dst.y = y;

            auto texture = SDL_CreateTextureFromSurface(graphics.Renderer, image);

            if (texture)
            {
                SDL_RenderCopy(graphics.Renderer, texture, &src, &dst);

                SDL_DestroyTexture(texture);

                texture = NULL;
            }
        }
    }

    // render image at location
    void Render(Base &graphics, SDL_Surface *image, int x, int y)
    {
        if (image && graphics.Renderer)
        {
            Graphics::Render(graphics, image, x, y, image->h, 0);
        }
    }

    void RenderScene(Base &graphics, Graphics::Scene &scene)
    {
        if (graphics.Renderer)
        {
            Graphics::FillWindow(graphics, scene.Background);

            for (auto i = 0; i < scene.Elements.size(); i++)
            {
                auto element = scene.Elements.at(i);

                Graphics::Render(graphics, element.Surface, element.X, element.Y, element.Bounds, element.Offset);
            }
        }
    }

    // close graphics subsystem
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