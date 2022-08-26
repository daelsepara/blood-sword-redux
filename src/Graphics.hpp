#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <iostream>
#include <vector>

// Using SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Controls.hpp"

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
        SDL_Texture *Texture;

        int X = 0;

        int Y = 0;

        int Bounds = 0;

        int Offset = 0;

        int W = 0;

        int H = 0;

        void Initialize(SDL_Texture *surface, int x, int y, int bounds, int offset, int w, int h)
        {
            if (surface)
            {
                Texture = surface;

                X = x;

                Y = y;

                Bounds = bounds;

                Offset = offset;

                W = w;

                H = h;
            }
        }

        SceneElements(SDL_Texture *texture, int x, int y, int bounds, int offset, int w, int h)
        {
            if (texture)
            {
                this->Initialize(texture, x, y, bounds, offset, w, h);
            }
        }

        SceneElements(SDL_Texture *texture, int x, int y, int bounds, int offset)
        {
            if (texture)
            {
                auto texture_w = 0;
                auto texture_h = 0;

                SDL_QueryTexture(texture, NULL, NULL, &texture_w, &texture_h);

                this->Initialize(texture, x, y, bounds, offset, texture_w, bounds);
            }
        }

        SceneElements(SDL_Texture *texture, int x, int y)
        {
            if (texture)
            {
                auto texture_w = 0;
                auto texture_h = 0;

                SDL_QueryTexture(texture, NULL, NULL, &texture_w, &texture_h);

                this->Initialize(texture, x, y, texture_h, 0, texture_w, texture_h);
            }
        }
    };

    class Scene
    {
    public:
        std::vector<Graphics::SceneElements> Elements = {};

        std::vector<Controls::Base> Controls = {};

        Uint32 Background = 0;

        void Set(std::vector<Graphics::SceneElements> elements)
        {
            this->Elements = elements;
        }

        void Set(std::vector<Controls::Base> controls)
        {
            this->Controls = controls;
        }

        void Set(Uint32 background)
        {
            this->Background = background;
        }

        void Clear()
        {
            this->Set(0);

            this->Elements.clear();

            this->Controls.clear();
        }

        void Add(Graphics::SceneElements element)
        {
            this->Elements.push_back(element);
        }

        void Add(Controls::Base control)
        {
            this->Controls.push_back(control);
        }

        Scene(std::vector<Graphics::SceneElements> elements, std::vector<Controls::Base> controls, Uint32 background)
        {
            this->Set(elements);

            this->Set(controls);

            this->Set(background);
        }

        Scene(std::vector<Graphics::SceneElements> elements, std::vector<Controls::Base> controls)
        {
            this->Set(elements);

            this->Set(controls);
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

    Graphics::Base Initialize(const char *title)
    {
        auto graphics = Graphics::Base();

        Graphics::Initialize(graphics, title);

        return graphics;
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

    // free texture
    void Free(SDL_Surface **surface)
    {
        SDL_FreeSurface(*surface);

        *surface = NULL;
    }

    // free texture
    void Free(SDL_Texture **texture)
    {
        SDL_DestroyTexture(*texture);

        *texture = NULL;
    }

    // base render texture function
    void Render(Base &graphics, SDL_Texture *texture, int texture_w, int texture_h, int x, int y, int bounds, int offset, int w, int h)
    {
        if (graphics.Renderer && texture)
        {
            SDL_Rect src;
            src.w = texture_w;
            src.h = std::min(texture_h, bounds);
            src.y = offset;
            src.x = 0;

            SDL_Rect dst;
            dst.w = w;
            dst.h = h;
            dst.x = x;
            dst.y = y;

            SDL_RenderCopy(graphics.Renderer, texture, &src, &dst);
        }
    }

    void Render(Base &graphics, SDL_Texture *texture, int x, int y, int bounds, int offset, int w, int h)
    {
        if (graphics.Renderer && texture)
        {
            auto texture_w = 0;

            auto texture_h = 0;

            SDL_QueryTexture(texture, NULL, NULL, &texture_w, &texture_h);

            Graphics::Render(graphics, texture, texture_w, texture_h, x, y, bounds, offset, w, h);
        }
    }

    // render a portion of the texture
    void Render(Base &graphics, SDL_Texture *texture, int x, int y, int bounds, int offset)
    {
        if (graphics.Renderer && texture)
        {
            auto texture_w = 0;

            auto texture_h = 0;

            SDL_QueryTexture(texture, NULL, NULL, &texture_w, &texture_h);

            Graphics::Render(graphics, texture, x, y, std::min(texture_h, bounds), offset, texture_w, std::min(texture_h, bounds));
        }
    }

    // render texture at location
    void Render(Base &graphics, SDL_Texture *texture, int x, int y)
    {
        if (texture && graphics.Renderer)
        {
            auto texture_h = 0;

            SDL_QueryTexture(texture, NULL, NULL, NULL, &texture_h);

            Graphics::Render(graphics, texture, x, y, texture_h, 0);
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

                Graphics::Render(graphics, element.Texture, element.X, element.Y, element.Bounds, element.Offset, element.W, element.H);
            }
        }
    }

    void RenderScene(Base &graphics, Graphics::Scene &scene, Controls::User input)
    {
        if (graphics.Renderer)
        {
            Graphics::RenderScene(graphics, scene);

            for (auto i = 0; i < scene.Controls.size(); i++)
            {
                auto control = scene.Controls.at(i);

                if (i == input.Current)
                {
                    Graphics::ThickRect(graphics, control.W, control.H, control.X, control.Y, control.Highlight, control.Pixels);
                }
            }
        }
    }

    SDL_Surface *CreateSurfaceText(const char *text, TTF_Font *font, SDL_Color textColor, int style, int wrap)
    {
        SDL_Surface *surface = NULL;

        if (font)
        {
            TTF_SetFontStyle(font, style);

            surface = TTF_RenderText_Blended_Wrapped(font, text, textColor, wrap);
        }

        return surface;
    }

    SDL_Surface *CreateSurfaceText(const char *text, TTF_Font *font, SDL_Color textColor, int style)
    {
        return Graphics::CreateSurfaceText(text, font, textColor, style, 0);
    }

    SDL_Texture *CreateText(Graphics::Base &graphics, const char *text, TTF_Font *font, SDL_Color textColor, int style, int wrap)
    {
        SDL_Texture *texture = NULL;

        auto surface = Graphics::CreateSurfaceText(text, font, textColor, style, wrap);

        if (surface)
        {
            texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

            Graphics::Free(&surface);
        }

        return texture;
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
