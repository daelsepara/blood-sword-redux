#ifndef __CONTROLS_HPP__
#define __CONTROLS_HPP__

#include <SDL.h>

#include "ControlTypes.hpp"
#include "Primitives.hpp"

namespace BloodSword::Controls
{
    const int NotFound = -1;

    template <typename T>
    using Mapped = std::unordered_map<Controls::Type, T>;

    class Base
    {
    public:
        // type of control. used in distinguishing selected control in user input
        Controls::Type Type = Controls::Type::ANY;

        // ID
        int Id = -1;

        // ID of control to the left of this control, equal to ID if none
        int Left = -1;

        // ID of control to the right of this control, equal to ID if none
        int Right = -1;

        // ID of control above this control, equal to ID if none
        int Up = -1;

        // ID of control below this control, equal to ID if none
        int Down = -1;

        // X location on the screen (part of this control's hitbox definition)
        int X = 0;

        // Y location on the screen (part of this control's hitbox definition)
        int Y = 0;

        // width of this control's hitbox
        int W = 0;

        // height of this control's hitbox
        int H = 0;

        // border size (when highligted)
        int Pixels = BloodSword::Pixel;

        // control is on the map
        bool OnMap = false;

        // location on the map
        Point Map = Point(-1, -1);

        // color of border when hightlighted
        Uint32 Highlight;

        Base(Controls::Type type,
             int id, int left, int right, int up, int down,
             int x, int y, int w, int h,
             int highlight) : Type(type),
                              Id(id), Left(left), Right(right), Up(up), Down(down),
                              X(x), Y(y), W(w), H(h),
                              Highlight(highlight) {}

        Base(Controls::Type type,
             int id, int left, int right, int up, int down,
             int x, int y, int w, int h, int highlight,
             int mapx, int mapy) : Base(type, id, left, right, up, down, x, y, w, h, highlight)
        {
            this->OnMap = true;

            this->Map = Point(mapx, mapy);
        }

        Base(Controls::Type type,
             int id, int left, int right, int up, int down,
             Point p, int w, int h, int highlight,
             Point map) : Base(type,
                               id, left, right, up, down,
                               p.X, p.Y, w, h, highlight,
                               map.X, map.Y) {}
    };

    // user input base class
    class User
    {
    public:
        // type of the control currently in focus
        Controls::Type Type = Controls::Type::NONE;

        // ID of the control currently in focus, -1 if none
        int Current = 0;

        // the control has been explicitly selected
        // i.e. through a mouse button click, pressing a button on the gamepad,
        // or via the RETURN key (on the keyboard)
        bool Selected = false;

        // a scroll up event
        bool Up = false;

        // a scroll down event
        bool Down = false;

        // control is currently being held down (e.g. mouse left button held over the control, etc.)
        bool Hold = false;

        // quit/exit/terminate event
        bool Quit = false;

        // blink curser
        bool Blink = false;

        // text event
        bool Text = false;

        // flag to indicate a re-render text
        bool RefreshText = false;

        // input text
        std::string TextInput = std::string();

        // Character limit
        int TextLimit = 20;

        User(Controls::Type type,
             int current,
             bool selected,
             bool up,
             bool down,
             bool hold) : Type(type),
                          Current(current),
                          Selected(selected),
                          Up(up),
                          Down(down),
                          Hold(hold) {}

        User(int current,
             bool selected,
             bool up,
             bool down) : Current(current), Selected(selected), Up(up), Down(down) {}

        User(int current, bool selected) : Current(current), Selected(selected) {}

        User() {}

        void SetText(const char *text)
        {
            this->TextInput = std::string(text);
        }

        void SetText(std::string text)
        {
            this->TextInput = text;
        }

        void ClearText()
        {
            this->TextInput.clear();
        }

        void Clear()
        {
            this->Type = Controls::Type::NONE;

            this->Selected = false;

            this->Current = -1;
        }
    };

    typedef std::vector<Controls::Base> List;

    // find if control is present in the list
    int Find(Controls::List &controls, Controls::Type type)
    {
        auto result = -1;

        for (auto &control : controls)
        {
            if (control.Type == type)
            {
                result = control.Id;

                break;
            }
        }

        return result;
    }

    int Find(Controls::Collection &controls, Controls::Type type)
    {
        auto result = -1;

        for (auto id = 0; id < controls.size(); id++)
        {
            if (controls[id] == type)
            {
                result = id;

                break;
            }
        }

        return result;
    }

    void Select(Controls::User &input, Controls::List &controls, Controls::Type control)
    {
        input.Current = Controls::Find(controls, control);

        if (input.Current != -1)
        {
            input.Type = control;
        }

        input.Selected = false;
    }

    void Select(Controls::User &input, Controls::Collection &controls, Controls::Type control)
    {
        input.Current = Controls::Find(controls, control);

        if (input.Current != -1)
        {
            input.Type = control;
        }

        input.Selected = false;
    }
}

#endif
