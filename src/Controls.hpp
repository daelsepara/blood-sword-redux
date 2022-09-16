#ifndef __CONTROLS_HPP__
#define __CONTROLS_HPP__

#include <vector>
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

        void Initialize(Controls::Type type, int id, int left, int right, int up, int down, int x, int y, int w, int h, int highlight)
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

        Base(Controls::Type type, int id, int left, int right, int up, int down, int x, int y, int w, int h, int highlight, int mapx, int mapy) : IsMap(true), MapX(mapx), MapY(mapy)
        {
            this->Initialize(type, id, left, right, up, down, x, y, w, h, highlight);
        }

        Base(Controls::Type type, int id, int left, int right, int up, int down, int x, int y, int w, int h, int highlight)
        {
            this->Initialize(type, id, left, right, up, down, x, y, w, h, highlight);
        }
    };

    // user input
    class User
    {
    public:
        Controls::Type Type = Controls::Type::NONE;

        int Current = -1;

        bool Selected = false;

        bool Up = false;

        bool Down = false;

        bool Hold = false;

        bool Quit = false;

        User(Controls::Type type, int current, bool selected, bool up, bool down, bool hold) : Type(type), Current(current), Selected(selected), Up(up), Down(down), Hold(hold)
        {
        }

        User(int current, bool selected, bool up, bool down) : Current(current), Selected(selected), Up(up), Down(down)
        {
        }

        User(int current, bool selected) : Current(current), Selected(selected)
        {
        }

        User()
        {
        }
    };

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
