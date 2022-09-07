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
        TAKE,
        TEMPORARY_OBSTACLE
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
        int Pixels = 2;
        bool IsMap = false;
        int MapX = -1;
        int MapY = -1;

        // color of border when hightlighted
        Uint32 Highlight;

        void _Initialize(Controls::Type type, int id, int left, int right, int up, int down, int x, int y, int w, int h, int highlight)
        {
            this->Type = type;

            this->ID = id;

            this->Left = left;

            this->Right = right;

            this->Up = up;

            this->Down = down;

            this->X = x;

            this->Y = y;

            this->W = w;

            this->H = h;

            this->Highlight = highlight;
        }

        Base(Controls::Type type, int id, int left, int right, int up, int down, int x, int y, int w, int h, int highlight, int mapx, int mapy)
        {
            this->_Initialize(type, id, left, right, up, down, x, y, w, h, highlight);

            this->IsMap = true;

            this->MapX = mapx;

            this->MapY = mapy;
        }

        Base(Controls::Type type, int id, int left, int right, int up, int down, int x, int y, int w, int h, int highlight)
        {
            this->_Initialize(type, id, left, right, up, down, x, y, w, h, highlight);
        }
    };

    // user input
    class User
    {
    public:
        int Current = -1;

        bool Selected = false;

        bool Up = false;

        bool Down = false;

        bool Hold = false;

        bool Quit = false;

        User(int current, bool selected, bool up, bool down, bool hold)
        {
            this->Current = current;

            this->Selected = selected;

            this->Up = up;

            this->Down = down;

            this->Hold = hold;
        }

        User(int current, bool selected, bool up, bool down)
        {
            this->Current = current;

            this->Selected = selected;

            this->Up = up;

            this->Down = down;
        }

        User(int current, bool selected)
        {
            this->Current = current;

            this->Selected = selected;
        }

        User()
        {
        }
    };
}

#endif
