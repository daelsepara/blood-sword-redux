#ifndef __CONTROLS_HPP__
#define __CONTROLS_HPP__

#include <SDL.h>

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
        TAKE
    };

    class Base
    {
    public:
        Controls::Type Type = Controls::Type::ANY;

        int ID = -1;

        int Left = -1;
        int Right = -1;
        int Up = -1;
        int Down = -1;

        int X = 0;
        int Y = 0;

        int W = 0;
        int H = 0;

        int Pixels = 0;

        // color of border when hightlighted
        Uint32 Highlight;
    };

    // user input
    class User
    {
    public:
        int Selected = -1;

        int Current = -1;

        bool Up = false;

        bool Down = false;

        bool Hold = false;

        User(int selected, int current, bool up, bool down, bool hold)
        {
            this->Selected = selected;

            this->Current = current;

            this->Up = up;

            this->Down = down;

            this->Hold = hold;
        }

        User(int selected, int current, bool up, bool down)
        {
            this->Selected = selected;

            this->Current = current;

            this->Up = up;

            this->Down = down;
        }

        User(int selected, int current)
        {
            this->Selected = selected;

            this->Current = current;
        }
    };
}

#endif