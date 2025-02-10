#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <vector>
#include <SDL.h>

#include "Controls.hpp"
#include "Primitives.hpp"

// classes and functions to define objects that will be rendered on screen
namespace BloodSword::Scene
{
    // scene element base class
    class Element
    {
    public:
        // texture associated with the element
        SDL_Texture *Texture;

        // background color, 0 if none
        Uint32 Background = 0;

        // border color, 0 if none
        Uint32 Border = 0;

        // x location on screen
        int X = 0;

        // y location on screen
        int Y = 0;

        // size of texture portion to be rendered. Used with Offset.
        int Bounds = 0;

        // starting point in texture to be rendered. Used with Bounds.
        int Offset = 0;

        // width of the element
        int W = 0;

        // height of the element
        int H = 0;

        // thickness of the border in pixels
        int BorderSize = 0;

        Element(SDL_Texture *texture,
                int x, int y,
                int bounds, int offset,
                int w, int h,
                Uint32 background,
                Uint32 border,
                int border_size) : Texture(texture),
                                   Background(background), Border(border),
                                   X(x), Y(y),
                                   Bounds(bounds), Offset(offset),
                                   W(w), H(h),
                                   BorderSize(border_size) {}

        Element(SDL_Texture *texture,
                int x, int y,
                int bounds, int offset,
                Uint32 background,
                Uint32 border,
                int border_size) : Texture(texture),
                                   Background(background), Border(border),
                                   X(x), Y(y),
                                   Bounds(bounds), Offset(offset),
                                   BorderSize(border_size)
        {
            if (this->Texture)
            {
                BloodSword::Size(this->Texture, &this->W, &this->H);
            }
        }

        Element(SDL_Texture *texture,
                int x, int y,
                int bounds, int offset,
                Uint32 border,
                int border_size) : Element(texture, x, y, bounds, offset, 0, border, border_size) {}

        Element(SDL_Texture *texture,
                int x, int y,
                Uint32 background,
                Uint32 border,
                int border_size) : Texture(texture),
                                   Background(background),
                                   Border(border),
                                   X(x), Y(y),
                                   Offset(0),
                                   BorderSize(border_size)
        {
            if (texture)
            {
                BloodSword::Size(texture, &this->W, &this->H);

                this->Bounds = this->H;
            }
        }

        Element(SDL_Texture *texture,
                int x, int y,
                int bounds, int offset) : Element(texture, x, y, bounds, offset, 0, 0, 0) {}

        Element(SDL_Texture *texture, int x, int y) : Texture(texture), X(x), Y(y), Offset(0)
        {
            if (this->Texture)
            {
                BloodSword::Size(this->Texture, &this->W, &this->H);
            }

            this->Bounds = this->H;
        }

        Element(SDL_Texture *texture, Point point) : Element(texture, point.X, point.Y) {}

        Element(int x, int y, int w, int h,
                Uint32 background,
                Uint32 border,
                int border_size) : Element(nullptr, x, y, h, 0, w, h, background, border, border_size) {}

        Element(Point point, int w, int h,
                Uint32 background,
                Uint32 border,
                int border_size) : Element(point.X, point.Y, w, h, background, border, border_size) {}

        Element(int x, int y, int w, int h,
                Uint32 background) : Element(nullptr, x, y, h, 0, w, h, background, 0, 0) {}
    };

    // Scene base clas
    class Base
    {
    public:
        // objects to be rendered on screen
        std::vector<Scene::Element> Elements = {};

        // locations and dimensions of controls hitboxes associated with the scene lement
        std::vector<Controls::Base> Controls = {};

        // background color of the entire screen. set before rendering each element
        Uint32 Background = 0;

        // clear the scene
        void Clear()
        {
            this->Background = 0;

            this->Elements.clear();

            this->Controls.clear();
        }

        // add element to the scene
        void Add(Scene::Element element)
        {
            this->Elements.push_back(element);
        }

        void VerifyAndAdd(Scene::Element element)
        {
            if (element.Texture != nullptr)
            {
                this->Add(element);
            }
        }

        // add a control hitbox
        void Add(Controls::Base control)
        {
            this->Controls.push_back(control);
        }

        Base(std::vector<Scene::Element> elements, std::vector<Controls::Base> controls, Uint32 background) : Elements(elements), Controls(controls), Background(background) {}

        Base(std::vector<Scene::Element> elements, std::vector<Controls::Base> controls) : Elements(elements), Controls(controls) {}

        Base(std::vector<Scene::Element> elements, Uint32 background) : Elements(elements), Background(background) {}

        Base(std::vector<Scene::Element> elements) : Elements(elements) {}

        Base(Uint32 background) : Background(background) {}

        Base(SDL_Texture *texture, int x, int y) : Base(std::vector<Scene::Element>({Scene::Element(texture, x, y)})) {}

        Base(SDL_Texture *texture, Point point) : Base(texture, point.X, point.Y) {}

        Base() {}
    };
}

#endif
