#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <iostream>
#include <vector>

#include "Primitives.hpp"
#include "Color.hpp"
#include "Controls.hpp"
#include "RichText.hpp"
#include "Scene.hpp"
#include "Animation.hpp"

namespace BloodSword::Graphics
{
    SDL_Texture *VersionOverlay = NULL;

    Point VersionCoordinates = Point(0, 0);

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

            graphics.Width = mode.w;

            graphics.Height = mode.h;

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

    void SetWindowIcon(Base &graphics, const char *icon)
    {
        auto surface = BloodSword::Load(icon);

        if (graphics.Window && surface)
        {
            SDL_SetWindowIcon(graphics.Window, surface);

            BloodSword::Free(&surface);
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
            SDL_SetRenderDrawColor(graphics.Renderer, Color::R(color), Color::G(color), Color::B(color), Color::A(color));
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
            SDL_SetRenderDrawColor(graphics.Renderer, Color::R(color), Color::G(color), Color::B(color), Color::A(color));

            SDL_RenderClear(graphics.Renderer);
        }
    }

    // respond to window resizing/in focus/out of focus events
    void Refresh(Graphics::Base &graphics)
    {
        SDL_Event result;

        SDL_PollEvent(&result);

        if (result.type == SDL_WINDOWEVENT)
        {
            if (result.window.event == SDL_WINDOWEVENT_RESTORED || result.window.event == SDL_WINDOWEVENT_MAXIMIZED || result.window.event == SDL_WINDOWEVENT_SHOWN)
            {
                SDL_RenderPresent(graphics.Renderer);
            }
        }
    }

    // base render texture function
    void Render(Base &graphics, SDL_Texture *texture, int texture_w, int texture_h, int x, int y, int bounds, int offset, int w, int h, Uint32 background)
    {
        if (graphics.Renderer)
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

            if (background != 0)
            {
                SDL_SetRenderDrawColor(graphics.Renderer, Color::R(background), Color::G(background), Color::B(background), Color::A(background));

                SDL_RenderFillRect(graphics.Renderer, &dst);
            }

            if (texture)
            {
                SDL_RenderCopy(graphics.Renderer, texture, &src, &dst);
            }
        }
    }

    // render texture, background and borders
    void Render(Base &graphics, SDL_Texture *texture, int texture_w, int texture_h, int x, int y, int bounds, int offset, int w, int h, Uint32 background, Uint32 border, int borderSize)
    {
        if (graphics.Renderer)
        {
            Graphics::Render(graphics, texture, texture_w, texture_h, x, y, bounds, offset, w, h, background);

            if (border != 0)
            {
                Graphics::ThickRect(graphics, w, h, x, y, border, borderSize);
            }
        }
    }

    // stretch a portion of the texture
    void Render(Base &graphics, SDL_Texture *texture, int x, int y, int bounds, int offset, int w, int h, Uint32 background)
    {
        if (graphics.Renderer && texture)
        {
            auto texture_w = 0;

            auto texture_h = 0;

            SDL_QueryTexture(texture, NULL, NULL, &texture_w, &texture_h);

            Graphics::Render(graphics, texture, texture_w, texture_h, x, y, bounds, offset, w, h, background);
        }
    }

    // render a portion of the texture
    void Render(Base &graphics, SDL_Texture *texture, int x, int y, int bounds, int offset, Uint32 background)
    {
        if (graphics.Renderer && texture)
        {
            auto texture_w = 0;

            auto texture_h = 0;

            SDL_QueryTexture(texture, NULL, NULL, &texture_w, &texture_h);

            Graphics::Render(graphics, texture, texture_w, texture_h, x, y, std::min(texture_h, bounds), offset, texture_w, std::min(texture_h, bounds), background);
        }
    }

    // render texture at location
    void Render(Base &graphics, SDL_Texture *texture, int x, int y, Uint32 background)
    {
        if (graphics.Renderer && texture)
        {
            auto texture_h = 0;

            SDL_QueryTexture(texture, NULL, NULL, NULL, &texture_h);

            Graphics::Render(graphics, texture, x, y, texture_h, 0, background);
        }
    }

    void Render(Base &graphics, SDL_Texture *texture, int x, int y)
    {
        if (graphics.Renderer && texture)
        {
            Graphics::Render(graphics, texture, x, y, 0);
        }
    }

    void Render(Base &graphics, SDL_Texture *texture, Point location)
    {
        Graphics::Render(graphics, texture, location.X, location.Y);
    }

    void Overlay(Base &graphics, Scene::Base &scene)
    {
        if (graphics.Renderer)
        {
            for (auto &element : scene.Elements)
            {
                Graphics::Render(graphics, element.Texture, element.W, element.H, element.X, element.Y, element.Bounds, element.Offset, element.W, std::min(element.Bounds, element.H), element.Background, element.Border, element.BorderSize);
            }
        }
    }

    void Render(Base &graphics, Scene::Base &scene)
    {
        if (graphics.Renderer)
        {
            Graphics::FillWindow(graphics, scene.Background);

            Graphics::Overlay(graphics, scene);
        }
    }

    void DisplayVersion(Base &graphics)
    {
        if (VersionOverlay)
        {
            Graphics::Render(graphics, VersionOverlay, VersionCoordinates.X, VersionCoordinates.Y);
        }
    }

    void Render(Base &graphics, Scene::Base &scene, Controls::User input)
    {
        if (graphics.Renderer)
        {
            Graphics::Render(graphics, scene);

            for (auto &control : scene.Controls)
            {
                if (control.ID == input.Current)
                {
                    Graphics::ThickRect(graphics, control.W - 4 * control.Pixels, control.H - 4 * control.Pixels, control.X + 2 * control.Pixels, control.Y + 2 * control.Pixels, control.Highlight, control.Pixels);
                }
            }
        }
    }

    void RenderNow(Base &graphics, Scene::Base &scene)
    {
        if (graphics.Renderer)
        {
            Graphics::Render(graphics, scene);

            SDL_RenderPresent(graphics.Renderer);
        }
    }

    void RenderNow(Base &graphics, Scene::Base &background, Scene::Base &foreground)
    {
        if (graphics.Renderer)
        {
            Graphics::Render(graphics, background);

            Graphics::Overlay(graphics, foreground);

            SDL_RenderPresent(graphics.Renderer);
        }
    }

    bool Animate(Base &graphics, Scene::Base &background, Animations::Base &animations, bool delay = true)
    {
        auto foreground = Scene::Base();

        auto done = Animations::Step(foreground, animations, delay);

        Graphics::RenderNow(graphics, background, foreground);

        return done;
    }

    bool Animate(Base &graphics, Scene::Base &background, Animation::Base &animation, bool delay = true)
    {
        auto animations = Animations::Base(animation);

        auto done = Graphics::Animate(graphics, background, animations, delay);

        animation = *(animations.List.begin());

        return done;
    }

    // estimate texture dimensions of string
    void Estimate(TTF_Font *font, const char *text, int *width, int *height)
    {
        TTF_SizeUTF8(font, text, width, height);

        if (width)
        {
            *width += 8;
        }

        if (height)
        {
            *height += 8;
        }
    }

    SDL_Surface *CreateSurfaceText(const char *text, TTF_Font *font, SDL_Color textColor, int style, int wrap)
    {
        SDL_Surface *surface = NULL;

        if (font)
        {
            TTF_SetFontStyle(font, style);

            surface = TTF_RenderUTF8_Blended_Wrapped(font, text, textColor, wrap);
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

            BloodSword::Free(&surface);
        }

        return texture;
    }

    std::vector<SDL_Texture *> CreateText(Graphics::Base &graphics, std::vector<Graphics::RichText> collection)
    {
        std::vector<SDL_Texture *> textures = {};

        for (auto &text : collection)
        {
            auto texture = Graphics::CreateText(graphics, text.Text.c_str(), text.Font, text.Color, text.Style, text.Wrap);

            if (texture)
            {
                textures.push_back(texture);
            }
        };

        return textures;
    }

    void InitializeVersionOverlay(Graphics::Base &graphics)
    {
        auto estimate = 0;

        Graphics::Estimate(Fonts::Caption, VersionString, &estimate, NULL);

        VersionOverlay = Graphics::CreateText(graphics, VersionString, Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, estimate);

        if (VersionOverlay)
        {
            auto w = 0;

            auto h = 0;

            SDL_QueryTexture(VersionOverlay, NULL, NULL, &w, &h);

            VersionCoordinates = Point(graphics.Width - w, graphics.Height - h);
        }
    }

    // close graphics subsystem
    void Quit(Base &graphics)
    {
        Free(&VersionOverlay);

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
