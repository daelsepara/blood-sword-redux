#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <vector>

// Using SDL
#include <SDL.h>

#include "Controls.hpp"

namespace BloodSword::Scene
{
    class Element
    {
    public:
        SDL_Texture *Texture;

        int X = 0;

        int Y = 0;

        int Bounds = 0;

        int Offset = 0;

        int W = 0;

        int H = 0;

        Uint32 Background = 0;

        Uint32 Border = 0;

        int BorderSize = 0;

        void Set(Uint32 background, Uint32 border, int borderSize)
        {
            this->Background = background;

            this->Border = border;

            this->BorderSize = borderSize;
        }

        void Initialize(SDL_Texture *texture, int x, int y, int bounds, int offset, int w, int h, Uint32 background, Uint32 border, int borderSize)
        {
            if (texture)
            {
                this->Set(background, border, borderSize);

                this->Texture = texture;

                this->Bounds = bounds;

                this->Offset = offset;

                this->X = x;

                this->Y = y;

                this->W = w;

                this->H = h;
            }
        }

        void Initialize(SDL_Texture *texture, int x, int y, int bounds, int offset, Uint32 background, Uint32 border, int borderSize)
        {
            if (texture)
            {
                auto w = 0;

                auto h = 0;

                SDL_QueryTexture(texture, NULL, NULL, &w, &h);

                this->Initialize(texture, x, y, bounds, offset, w, h, background, border, borderSize);
            }
        }

        void Initialize(SDL_Texture *texture, int x, int y, int bounds, int offset)
        {
            this->Initialize(texture, x, y, bounds, offset, 0, 0, 0);
        }

        Element(SDL_Texture *texture, int x, int y, int bounds, int offset, Uint32 background, Uint32 border, int borderSize)
        {
            if (texture)
            {
                this->Initialize(texture, x, y, bounds, offset, background, border, borderSize);
            }
        }

        Element(SDL_Texture *texture, int x, int y, int bounds, int offset, Uint32 border, int borderSize)
        {
            if (texture)
            {
                this->Initialize(texture, x, y, bounds, offset, 0, border, borderSize);
            }
        }

        Element(SDL_Texture *texture, int x, int y, int bounds, int offset, Uint32 background)
        {
            if (texture)
            {
                this->Initialize(texture, x, y, bounds, offset, background, 0, 0);
            }
        }

        Element(SDL_Texture *texture, int x, int y, int bounds, int offset)
        {
            if (texture)
            {
                this->Initialize(texture, x, y, bounds, offset);
            }
        }

        Element(SDL_Texture *texture, int x, int y)
        {
            if (texture)
            {
                auto texture_h = 0;

                SDL_QueryTexture(texture, NULL, NULL, NULL, &texture_h);

                this->Initialize(texture, x, y, texture_h, 0);
            }
        }
    };

    class Base
    {
    public:
        std::vector<Scene::Element> Elements = {};

        std::vector<Controls::Base> Controls = {};

        Uint32 Background = 0;

        void Set(std::vector<Scene::Element> elements)
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

        void Add(Scene::Element element)
        {
            this->Elements.push_back(element);
        }

        void Add(Controls::Base control)
        {
            this->Controls.push_back(control);
        }

        Base(std::vector<Scene::Element> elements, std::vector<Controls::Base> controls, Uint32 background)
        {
            this->Set(elements);

            this->Set(controls);

            this->Set(background);
        }

        Base(std::vector<Scene::Element> elements, std::vector<Controls::Base> controls)
        {
            this->Set(elements);

            this->Set(controls);
        }

        Base(std::vector<Scene::Element> elements, Uint32 background)
        {
            this->Set(elements);

            this->Set(background);
        }

        Base(Uint32 background)
        {
            this->Set(background);
        }

        Base()
        {
        }
    };
}

#endif
