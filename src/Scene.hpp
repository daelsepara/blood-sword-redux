#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <vector>
#include <SDL.h>

#include "Controls.hpp"
#include "Primitives.hpp"

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

        Element(SDL_Texture *texture,
                int x, int y,
                int bounds, int offset,
                int w, int h,
                Uint32 background,
                Uint32 border,
                int borderSize) : Texture(texture),
                                  Background(background), Border(border),
                                  X(x), Y(y),
                                  Bounds(bounds), Offset(offset),
                                  W(w), H(h),
                                  BorderSize(borderSize) {}

        Element(SDL_Texture *texture,
                int x, int y,
                int bounds, int offset,
                Uint32 background,
                Uint32 border,
                int borderSize) : Texture(texture),
                                  Background(background), Border(border),
                                  X(x), Y(y),
                                  Bounds(bounds), Offset(offset),
                                  BorderSize(borderSize)
        {
            if (this->Texture)
            {
                SDL_QueryTexture(this->Texture, NULL, NULL, &this->W, &this->H);
            }
        }

        Element(SDL_Texture *texture,
                int x, int y,
                int bounds, int offset,
                Uint32 border,
                int borderSize) : Element(texture, x, y, bounds, offset, 0, border, borderSize) {}

        Element(SDL_Texture *texture,
                int x, int y,
                int bounds, int offset,
                Uint32 background) : Element(texture, x, y, bounds, offset, background, 0, 0) {}

        Element(SDL_Texture *texture,
                int x, int y,
                int bounds, int offset) : Element(texture, x, y, bounds, offset, 0, 0, 0) {}

        Element(SDL_Texture *texture, int x, int y) : Texture(texture), X(x), Y(y), Offset(0)
        {
            if (this->Texture)
            {
                SDL_QueryTexture(this->Texture, NULL, NULL, &this->W, &this->H);
            }

            this->Bounds = this->H;
        }

        Element(SDL_Texture *texture, Point &point) : Element(texture, point.X, point.Y) {}

        Element(int x, int y, int w, int h,
                Uint32 background,
                Uint32 border,
                int borderSize) : Element(NULL, x, y, h, 0, w, h, background, border, borderSize) {}

        Element(int x, int y, int w, int h,
                Uint32 background) : Element(NULL, x, y, h, 0, w, h, background, 0, 0) {}
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

        Base(std::vector<Scene::Element> elements, std::vector<Controls::Base> controls, Uint32 background) : Elements(elements), Controls(controls), Background(background) {}

        Base(std::vector<Scene::Element> elements, std::vector<Controls::Base> controls) : Elements(elements), Controls(controls) {}

        Base(std::vector<Scene::Element> elements, Uint32 background) : Elements(elements), Background(background) {}

        Base(Uint32 background) : Background(background) {}

        Base(SDL_Texture *texture, int x, int y) : Elements(std::vector<Scene::Element>({Scene::Element(texture, x, y)})) {}

        Base(SDL_Texture *texture, Point &point) : Base(texture, point.X, point.Y) {}

        Base() {}
    };
}

#endif
