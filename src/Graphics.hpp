#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <iostream>
#include <vector>

#include "Color.hpp"
#include "Animation.hpp"
#include "Controls.hpp"
#include "RichText.hpp"

// classes and functions for the graphics rendering engine
namespace BloodSword::Graphics
{
    // version string texture overlay
    SDL_Texture *VersionOverlay = NULL;

    // location where version string texture is rendered
    Point VersionCoordinates = Point(0, 0);

    // horizontal scan lines toggle
    bool ScanLinesEnabled = true;

    // base class of the graphics system
    class Base
    {
    public:
        // window
        SDL_Window *Window = NULL;

        // renderer
        SDL_Renderer *Renderer = NULL;

        // screen dimension (width)
        int Width = 1280;

        // screen dimension (height)
        int Height = 800;

        Base() {}
    };

    // initialize graphics system
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
                SDL_SetRenderDrawBlendMode(graphics.Renderer, SDL_BLENDMODE_BLEND);
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

    // set window/screen icon
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

    // initialize graphics system and set window/screen title
    Graphics::Base Initialize(const char *title)
    {
        auto graphics = Graphics::Base();

        Graphics::Initialize(graphics, title);

        return graphics;
    }

    // define an rectangle and prepare color
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

    // draw rectangle outline on screen
    void DrawRect(Graphics::Base &graphics, int w, int h, int x, int y, int color)
    {
        if (graphics.Renderer)
        {
            auto rect = Graphics::CreateRect(graphics, w, h, x, y, color);

            SDL_RenderDrawRect(graphics.Renderer, &rect);
        }
    }

    // draw a filled rectangle on screen
    void FillRect(Graphics::Base &graphics, int w, int h, int x, int y, int color)
    {
        if (graphics.Renderer)
        {
            auto rect = Graphics::CreateRect(graphics, w, h, x, y, color);

            SDL_RenderFillRect(graphics.Renderer, &rect);
        }
    }

    // draw a rectangle with thich borders on screen
    void ThickRect(Graphics::Base &graphics, int w, int h, int x, int y, int color, int pts)
    {
        auto space = 2 * pts;

        for (auto size = pts * 2 - 1; size >= 0; size--)
        {
            Graphics::DrawRect(graphics, w + 2 * (space - size), h + 2 * (space - size), x - space + size, y - space + size, color);
        }
    }

    // fill entire screen with specified color
    void FillWindow(Graphics::Base &graphics, Uint32 color)
    {
        if (graphics.Renderer)
        {
            SDL_SetRenderDrawColor(graphics.Renderer, Color::R(color), Color::G(color), Color::B(color), Color::A(color));

            SDL_RenderClear(graphics.Renderer);
        }
    }

    // add scan lines to display
    void Scanlines(Base &graphics)
    {
        if (ScanLinesEnabled)
        {
            SDL_Rect scanline;

            scanline.w = graphics.Width;
            scanline.h = 1;
            scanline.x = 0;

            SDL_SetRenderDrawColor(graphics.Renderer, 0, 0, 0, 0x40);

            for (auto i = 0; i < graphics.Height; i += 2)
            {
                scanline.y = i + 1;
                SDL_RenderFillRect(graphics.Renderer, &scanline);
            }
        }
    }

    // toggle horizontal scan lines
    void ToggleScanLines()
    {
        Graphics::ScanLinesEnabled = !Graphics::ScanLinesEnabled;
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
                Graphics::Scanlines(graphics);

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

    // render texture at location
    void Render(Base &graphics, SDL_Texture *texture, int x, int y)
    {
        Graphics::Render(graphics, texture, x, y, 0);
    }

    // render texture at location
    void Render(Base &graphics, SDL_Texture *texture, Point location)
    {
        Graphics::Render(graphics, texture, location.X, location.Y);
    }

    // render overlay on screen
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

    // render scene (set backgroud color)
    void Render(Base &graphics, Scene::Base &scene)
    {
        if (graphics.Renderer)
        {
            Graphics::FillWindow(graphics, scene.Background);

            Graphics::Overlay(graphics, scene);
        }
    }

    // render version string on screen
    void DisplayVersion(Base &graphics)
    {
        if (VersionOverlay)
        {
            Graphics::Render(graphics, VersionOverlay, VersionCoordinates.X, VersionCoordinates.Y);
        }
    }

    // render scene and highlight the control currently in focus (if any)
    void Render(Base &graphics, Scene::Base &scene, Controls::User input)
    {
        if (graphics.Renderer)
        {
            Graphics::Render(graphics, scene);

            for (auto &control : scene.Controls)
            {
                if (control.Id == input.Current)
                {
                    if (!input.Blink || !control.OnMap)
                    {
                        Graphics::ThickRect(graphics, control.W - 4 * control.Pixels, control.H - 4 * control.Pixels, control.X + 2 * control.Pixels, control.Y + 2 * control.Pixels, control.Highlight, control.Pixels);
                    }
                }
            }
        }
    }

    // handle controls on pop-up dialog instead of the background
    void Dialog(Base &graphics, Scene::Base &background, Scene::Base &dialog, Controls::User input, bool blur = false)
    {
        if (graphics.Renderer)
        {
            Graphics::Render(graphics, background);

            if (blur)
            {
                auto rect = Graphics::CreateRect(graphics, graphics.Width, graphics.Height, 0, 0, 0xB0000000);

                SDL_RenderFillRect(graphics.Renderer, &rect);
            }

            Graphics::Overlay(graphics, dialog);

            for (auto &control : dialog.Controls)
            {
                if (control.Id == input.Current)
                {
                    if (!input.Blink || !control.OnMap)
                    {
                        Graphics::ThickRect(graphics, control.W - 4 * control.Pixels, control.H - 4 * control.Pixels, control.X + 2 * control.Pixels, control.Y + 2 * control.Pixels, control.Highlight, control.Pixels);
                    }
                }
            }
        }
    }

    // render scene now without waiting for user input
    // used in animation
    void RenderNow(Base &graphics, Scene::Base &scene)
    {
        if (graphics.Renderer)
        {
            Graphics::Render(graphics, scene);

            Graphics::DisplayVersion(graphics);

            Graphics::Scanlines(graphics);

            SDL_RenderPresent(graphics.Renderer);
        }
    }

    // render scene (background scene + foreground scene on top) now without waiting for user input
    // used in animation
    void RenderNow(Base &graphics, Scene::Base &background, Scene::Base &foreground)
    {
        if (graphics.Renderer)
        {
            Graphics::Render(graphics, background);

            Graphics::Overlay(graphics, foreground);

            Graphics::DisplayVersion(graphics);

            Graphics::Scanlines(graphics);

            SDL_RenderPresent(graphics.Renderer);
        }
    }

    // process through all animations
    bool Animate(Base &graphics, Scene::Base &background, Animations::Base &animations, bool trail = false, bool delay = true)
    {
        auto foreground = Scene::Base();

        auto done = Animations::Step(foreground, animations, delay, trail);

        Graphics::RenderNow(graphics, background, foreground);

        return done;
    }

    // process a single animation
    bool Animate(Base &graphics, Scene::Base &background, Animation::Base &animation, bool trail = false, bool delay = true)
    {
        auto animations = Animations::Base(animation);

        auto done = Graphics::Animate(graphics, background, animations, delay, trail);

        animation = *(animations.List.begin());

        return done;
    }

    // estimate texture dimensions of a string
    void Estimate(TTF_Font *font, const char *text, int *width, int *height)
    {
        TTF_SizeUTF8(font, text, width, height);

        auto pad = 8;

        if (width)
        {
            *width += pad;
        }

        if (height)
        {
            *height += pad;
        }
    }

    // create a SDL_Surface representation of a string
    SDL_Surface *CreateSurfaceText(const char *text, TTF_Font *font, SDL_Color textColor, int style, int wrap)
    {
        SDL_Surface *surface = NULL;

        if (font)
        {
            TTF_SetFontStyle(font, style);

            if (wrap == 0 && strchr(text, '\n') == NULL)
            {
                surface = TTF_RenderUTF8_Blended(font, text, textColor);
            }
            else
            {
                // TODO: Find better code to working with TTF_RenderUTF8_Blended_Wrapped on wraplength = 0
                auto estimate = wrap;

                if (wrap == 0 && strchr(text, '\n') != NULL)
                {
                    auto maxlength = 0;
                    
                    auto current = 0;

                    for (auto i = 0; i < strlen(text); i++)
                    {
                        if (text[i] != '\n')
                        {
                            current++;
                        }
                        else
                        {
                            if (current > maxlength)
                            {
                                maxlength = current;
                            }

                            current = 0;
                        }
                    }

                    if (current > maxlength)
                    {
                        maxlength = current;
                    }

                    if (maxlength > 0)
                    {
                        auto temp = std::string(maxlength, 'M') + ' ';

                        Graphics::Estimate(font, temp.c_str(), &estimate, NULL);
                    }
                }

                surface = TTF_RenderUTF8_Blended_Wrapped(font, text, textColor, estimate);
            }
        }

        return surface;
    }

    // create a SDL_Surface representation of a string (without line wrapping)
    SDL_Surface *CreateSurfaceText(const char *text, TTF_Font *font, SDL_Color textcolor, int style)
    {
        return Graphics::CreateSurfaceText(text, font, textcolor, style, 0);
    }

    // create a texture representation of a string
    SDL_Texture *CreateText(Graphics::Base &graphics, const char *text, TTF_Font *font, SDL_Color textcolor, int style, int wrap)
    {
        SDL_Texture *texture = NULL;

        auto surface = Graphics::CreateSurfaceText(text, font, textcolor, style, wrap);

        if (surface)
        {
            texture = SDL_CreateTextureFromSurface(graphics.Renderer, surface);

            BloodSword::Free(&surface);
        }

        return texture;
    }

    SDL_Texture *CreateText(Graphics::Base &graphics, const char *text, TTF_Font *font, SDL_Color textcolor, int style)
    {
        return Graphics::CreateText(graphics, text, font, textcolor, style, 0);
    }

    // create a list of texture representation of a collection of strings
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

    // create version string overlay texture
    void InitializeTextures(Graphics::Base &graphics)
    {
        auto estimate = 0;

        Graphics::Estimate(Fonts::Caption, Version().c_str(), &estimate, NULL);

        VersionOverlay = Graphics::CreateText(graphics, Version().c_str(), Fonts::Caption, Color::S(Color::Active), TTF_STYLE_NORMAL, estimate);

        if (VersionOverlay)
        {
            auto w = 0;

            auto h = 0;

            SDL_QueryTexture(VersionOverlay, NULL, NULL, &w, &h);

            VersionCoordinates = Point(graphics.Width - w, graphics.Height - h);
        }
    }

    // free all textures allocated by this module
    void FreeTextures()
    {
        Free(&VersionOverlay);
    }

    // close graphics system
    void Quit(Base &graphics)
    {
        Graphics::FreeTextures();

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
