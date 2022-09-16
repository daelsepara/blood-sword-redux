#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <vector>
#include <SDL.h>

#include "Controls.hpp"

namespace BloodSword::Scene
{
    class Element
    {
    public:
        SDL_Texture *Texture;

        Uint32 Background = 0;

        Uint32 Border = 0;

        int X = 0;

        int Y = 0;

        int Bounds = 0;

        int Offset = 0;

        int W = 0;

        int H = 0;

        int BorderSize = 0;

        void Initialize(SDL_Texture *texture, int x, int y, int bounds, int offset, int w, int h)
        {
            this->Texture = texture;

            this->Bounds = bounds;

            this->Offset = offset;

            this->X = x;

            this->Y = y;

            this->W = w;

            this->H = h;
        }

        void Initialize(SDL_Texture *texture, int x, int y, int bounds, int offset)
        {
            auto w = 0;

            auto h = 0;

            if (texture)
            {
                SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            }

            this->Initialize(texture, x, y, bounds, offset, w, h);
        }

        Element(SDL_Texture *texture, int x, int y, int bounds, int offset, Uint32 background, Uint32 border, int borderSize) : Background(background), Border(border), BorderSize(borderSize)
        {
            this->Initialize(texture, x, y, bounds, offset);
        }

        Element(SDL_Texture *texture, int x, int y, int bounds, int offset, Uint32 border, int borderSize) : Border(border), BorderSize(borderSize)
        {
            this->Initialize(texture, x, y, bounds, offset);
        }

        Element(SDL_Texture *texture, int x, int y, int bounds, int offset, Uint32 background) : Background(background)
        {
            this->Initialize(texture, x, y, bounds, offset);
        }

        Element(SDL_Texture *texture, int x, int y, int bounds, int offset)
        {
            this->Initialize(texture, x, y, bounds, offset);
        }

        Element(SDL_Texture *texture, int x, int y)
        {
            auto texture_h = 0;

            if (texture)
            {
                SDL_QueryTexture(texture, NULL, NULL, NULL, &texture_h);
            }

            this->Initialize(texture, x, y, texture_h, 0);
        }

        Element(int x, int y, int w, int h, Uint32 background, Uint32 border, int borderSize) : Background(background), Border(border), BorderSize(borderSize)
        {
            this->Initialize(NULL, x, y, h, 0, w, h);
        }

        Element(int x, int y, int w, int h, Uint32 background) : Background(background)
        {
            this->Initialize(NULL, x, y, h, 0, w, h);
        }
    };

    class Base
    {
    public:
        std::vector<Scene::Element> Elements = {};

        std::vector<Controls::Base> Controls = {};

        Uint32 Background = 0;

        void Clear()
        {
            this->Background = 0;

            this->Elements.clear();

            this->Controls.clear();
        }

        void Add(Scene::Element element)
        {
            this->Elements.push_back(element);
        }

        void Add(Controls::Base control)
        {
            this->Controls.push_back(control);
        }

        Base(std::vector<Scene::Element> elements, std::vector<Controls::Base> controls, Uint32 background) : Elements(elements), Controls(controls), Background(background)
        {
        }

        Base(std::vector<Scene::Element> elements, std::vector<Controls::Base> controls) : Elements(elements), Controls(controls)
        {
        }

        Base(std::vector<Scene::Element> elements, Uint32 background) : Elements(elements), Background(background)
        {
        }

        Base(Uint32 background) : Background(background)
        {
        }

        Base()
        {
        }
    };
}

#endif
