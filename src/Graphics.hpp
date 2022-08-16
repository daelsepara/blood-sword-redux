#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <iostream>
#include <vector>

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

        int W = 0;

        int H = 0;

        void Initialize(SDL_Surface *surface, int x, int y, int bounds, int offset, int w, int h)
        {
            if (surface)
            {
                Surface = surface;

                X = x;

                Y = y;

                Bounds = bounds;

                Offset = offset;

                W = w;

                H = h;
            }
        }

        SceneElements(SDL_Surface *surface, int x, int y, int bounds, int offset, int w, int h)
        {
            if (surface)
            {
                this->Initialize(surface, x, y, bounds, offset, w, h);
            }
        }

        SceneElements(SDL_Surface *surface, int x, int y, int bounds, int offset)
        {
            if (surface)
            {
                this->Initialize(surface, x, y, bounds, offset, surface->w, bounds);
            }
        }

        SceneElements(SDL_Surface *surface, int x, int y)
        {
            if (surface)
            {
                this->Initialize(surface, x, y, surface->h, 0, surface->w, surface->h);
            }
        }
    };

    class Scene
    {
    public:
        std::vector<Graphics::SceneElements> Elements = {};

        Uint32 Background = 0;

        void Set(Uint32 background)
        {
            this->Background = background;
        }

        void Set(std::vector<Graphics::SceneElements> elements)
        {
            this->Elements = elements;
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
    }

    SDL_Rect CreateRect(Graphics::Base &graphics, int w, int h, int x, int y, int color)
    {
        SDL_Rect rect;

        rect.w = w;
        rect.h = h;
        rect.x = x;
        rect.y = y;

        if (graphics.Renderer)
        {
            SDL_SetRenderDrawColor(graphics.Renderer, R(color), G(color), B(color), A(color));
        }

        return rect;
    }

    void DrawRect(Graphics::Base &graphics, int w, int h, int x, int y, int color)
    {
        if (graphics.Renderer)
        {
            auto rect = Graphics::CreateRect(graphics, w, h, x, y, color);

            SDL_RenderDrawRect(graphics.Renderer, &rect);
        }
    }

    void FillRect(Graphics::Base &graphics, int w, int h, int x, int y, int color)
    {
        if (graphics.Renderer)
        {
            auto rect = Graphics::CreateRect(graphics, w, h, x, y, color);

            SDL_RenderFillRect(graphics.Renderer, &rect);
        }
    }

    void ThickRect(Graphics::Base &graphics, int w, int h, int x, int y, int color, int pts)
    {
        auto space = 2 * pts;

        for (auto size = pts; size >= 0; size--)
        {
            Graphics::DrawRect(graphics, w + 2 * (space - size), h + 2 * (space - size), x - space + size, y - space + size, color);
        }
    }

    void FillWindow(Graphics::Base &graphics, Uint32 color)
    {
        if (graphics.Renderer)
        {
            SDL_SetRenderDrawColor(graphics.Renderer, R(color), G(color), B(color), A(color));

            SDL_RenderClear(graphics.Renderer);
        }
    }

    // base render function
    void Render(Base &graphics, SDL_Surface *image, int x, int y, int bounds, int offset, int w, int h)
    {
        if (graphics.Renderer && image)
        {
            SDL_Rect src;
            src.w = image->w;
            src.h = std::min(image->h, bounds);
            src.y = offset;
            src.x = 0;

            SDL_Rect dst;
            dst.w = w;
            dst.h = h;
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

    // render a portion of the image
    void Render(Base &graphics, SDL_Surface *image, int x, int y, int bounds, int offset)
    {
        if (graphics.Renderer && image)
        {
            Graphics::Render(graphics, image, x, y, std::min(image->h, bounds), offset, image->w, std::min(image->h, bounds));
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

                Graphics::Render(graphics, element.Surface, element.X, element.Y, element.Bounds, element.Offset, element.W, element.H);
            }
        }
    }

    SDL_Surface *CreateText(const char *text, TTF_Font *font, SDL_Color textColor, int style, int wrap)
    {
        SDL_Surface *surface = NULL;

        if (font)
        {
            TTF_SetFontStyle(font, style);

            surface = TTF_RenderText_Blended_Wrapped(font, text, textColor, wrap);
        }

        return surface;
    }

    SDL_Surface *CreateText(const char *text, TTF_Font *font, SDL_Color textColor, int style)
    {
        return Graphics::CreateText(text, font, textColor, style, 0);
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