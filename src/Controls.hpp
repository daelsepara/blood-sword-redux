#ifndef __CONTROLS_HPP__
#define __CONTROLS_HPP__

#include <vector>
#include <SDL.h>

#include "Primitives.hpp"

namespace BloodSword::Controls
{
    enum class Type
    {
        NONE = -1,
        ANY = 0,
        WARRIOR,
        TRICKSTER,
        SAGE,
        ENCHANTER,
        SCROLL_UP,
        SCROLL_DOWN,
        MAP_NONE,
        MAP_UP,
        MAP_DOWN,
        MAP_LEFT,
        MAP_RIGHT,
        MAP_EXIT,
        BACK,
        CONFIRM,
        ROLL,
        PLAYER,
        ENEMY,
        DESTINATION,
        ABILITY,
        ITEMS,
        CALL_TO_MIND,
        FORGET,
        FLEE,
        MOVE,
        HEAL,
        PLUS,
        MINUS,
        ATTACK,
        DEFEND,
        SHOOT,
        CAST,
        DAMAGE,
        ARCHERY,
        DODGING,
        QUARTERSTAFF,
        EXORCISM,
        ESP,
        PARANORMAL_SIGHT,
        LEVITATION,
        TEST,
        CHOICE,
        CONTINUE,
        MAP,
        ENCYCLOPEDIA,
        NEW,
        LOAD,
        SAVE,
        EXIT,
        GAME,
        PARTY,
        INFO,
        USE,
        DROP,
        TRANSFER,
        TAKE,
        TEMPORARY_OBSTACLE
    };

    class Base
    {
    public:
        // type of control. used in distinguishing selected control in user input
        Controls::Type Type = Controls::Type::ANY;

        // ID
        int ID = -1;

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
        int Pixels = 2;

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
                              ID(id), Left(left), Right(right), Up(up), Down(down),
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
        int Current = -1;

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
    };

    // find if control is present in the list
    int Find(std::vector<Controls::Base> &controls, Controls::Type type)
    {
        auto result = -1;

        for (auto &control : controls)
        {
            if (control.Type == type)
            {
                result = control.ID;

                break;
            }
        }

        return result;
    }
}

#endif
