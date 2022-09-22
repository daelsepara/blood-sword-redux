#ifndef __ANIMATION_HPP__
#define __ANIMATION_HPP__

#include "Primitives.hpp"
#include "Scene.hpp"
#include "Templates.hpp"

namespace BloodSword::Animate
{
    class Frame
    {
    public:
        SDL_Texture *Texture = NULL;

        int Duration = 0;

        Frame(SDL_Texture *texture, int duration) : Texture(texture), Duration(duration)
        {
        }

        Frame(SDL_Texture *texture) : Texture(texture)
        {
        }

        Frame()
        {
        }
    };

    enum class Type
    {
        MOVE,
        FRAME
    };

    class Base
    {
    public:
        std::vector<Animate::Frame> Frames = {};

        std::vector<Animate::Type> Mode = {};

        std::vector<Point> Path = {};

        Point Origin = Point(0, 0);

        Point Current = Point();

        Point Scale = Point(1, 1);

        Point Offset = Point(0, 0);

        Point Delta = Point(8, 8);

        bool Loop = false;

        Uint32 TimeStamp = 0;

        int Frame = 0;

        int Move = 0;

        int Speed = 0;

        Base(std::vector<Animate::Frame> frames,
             std::vector<Animate::Type> mode,
             std::vector<Point> path,
             int speed,
             bool loop) : Frames(frames), Mode(mode), Path(path), Loop(loop), Speed(speed)
        {
            if (!this->Frames.empty())
            {
                this->Frame = 0;
            }

            if (!this->Path.empty())
            {
                this->Current = path[0];
            }
        }

        Base(std::vector<Animate::Frame> frames,
             std::vector<Point> path,
             int speed) : Base(frames, {Type::MOVE}, path, speed, false) {}

        Base(SDL_Texture *texture,
             std::vector<Point> path,
             int speed) : Base({Animate::Frame(texture)}, {Type::MOVE}, path, speed, false) {}

        Base(std::vector<Animate::Frame> frames, bool loop) : Base(frames, {Type::FRAME}, {}, 0, loop) {}

        Base(std::vector<Animate::Frame> frames) : Base(frames, {Type::FRAME}, {}, 0, false) {}

        Base() {}

        void Set(std::vector<Animate::Frame> frames, bool loop)
        {
            this->Frames = frames;

            this->Loop = loop;

            this->Frame = 0;
        }

        void Set(std::vector<Point> path)
        {
            this->Path = path;

            this->Move = 0;
        }

        void Set(std::vector<Animate::Type> modes)
        {
            this->Mode = modes;
        }

        void Set(int speed)
        {
            this->Speed = speed;
        }

        void Set(Point point)
        {
            this->Current = point;
        }

        void Set(int x, int y)
        {
            this->Set(Point(x, y));
        }

        void Set(Point origin, Point current)
        {
            this->Origin = origin;

            this->Current = current;
        }

        void Set(int originx, int originy, int x, int y)
        {
            this->Set(Point(originx, originy), Point(x, y));
        }

        bool Is(std::vector<Animate::Type> types)
        {
            auto result = true;

            for (auto &type : types)
            {
                result &= BloodSword::Found(this->Mode, type);
            }

            return result;
        }

        bool Is(Animate::Type type)
        {
            return this->Is(std::vector<Animate::Type>{type});
        }

        void Add(Animate::Frame frame)
        {
            this->Frames.push_back(frame);
        }

        void Reset()
        {
            this->TimeStamp = 0;

            this->Frame = 0;

            this->Move = 0;

            if (!this->Path.empty())
            {
                this->Current = this->Path[0];

                this->Offset = Point(0, 0);
            }
        }
    };

    bool Show(Scene::Base &scene, Animate::Base &animation, bool delay = true)
    {
        auto done = false;

        if (!animation.Frames.empty() && animation.Frame >= 0 && animation.Frame < animation.Frames.size())
        {
            if (delay)
            {
                SDL_Delay(animation.Speed);
            }

            if (animation.Frame < animation.Frames.size() - 1)
            {
                animation.Frame++;
            }
            else if (!animation.Loop)
            {
                done = true;
            }
            else
            {
                animation.Frame = 0;
            }
        }
        else
        {
            done = true;
        }

        return done;
    }

    bool Move(Scene::Base &scene, Animate::Base &animation, bool delay = true)
    {
        auto Sign = [&](int Value)
        {
            return Value != 0 ? (Value > 0 ? 1 : -1) : 0;
        };

        auto done = false;

        if (!animation.Path.empty() && animation.Move >= 0 && animation.Move < animation.Path.size() - 1)
        {
            if (delay)
            {
                SDL_Delay(animation.Speed);
            }

            auto DeltaX = (animation.Current.X - animation.Path[animation.Move + 1].X) * animation.Scale.X + animation.Offset.X;

            auto DeltaY = (animation.Current.Y - animation.Path[animation.Move + 1].Y) * animation.Scale.Y + animation.Offset.Y;

            // check if we have reached our destination
            if (DeltaX == 0 && DeltaY == 0)
            {
                animation.Current = animation.Path[animation.Move + 1];

                animation.Offset = Point(0, 0);

                animation.Move++;
            }
            else
            {
                animation.Offset.X -= Sign(DeltaX) * animation.Delta.X;

                animation.Offset.Y -= Sign(DeltaY) * animation.Delta.Y;
            }
        }
        else
        {
            done = true;
        }

        return done;
    }

    bool Step(Scene::Base &scene, Animate::Base &animation)
    {
        auto done = false;

        auto move = animation.Move;

        auto frame = animation.Frame;

        if (animation.TimeStamp != 0)
        {
            if (animation.Is({Type::MOVE, Type::FRAME}))
            {
                SDL_Delay(animation.Speed);

                done = Animate::Move(scene, animation, false) && Animate::Show(scene, animation, false);
            }
            else if (animation.Is(Type::MOVE))
            {
                done = Animate::Move(scene, animation);
            }
            else if (animation.Is(Type::FRAME))
            {
                done = Animate::Show(scene, animation);
            }

            if (animation.Frame >= 0 && animation.Frame < animation.Frames.size())
            {
                auto x = animation.Origin.X + animation.Current.X * animation.Scale.X + animation.Offset.X;
                auto y = animation.Origin.Y + animation.Current.Y * animation.Scale.Y + animation.Offset.Y;

                // add sprite to scene
                scene.Add(Scene::Element(animation.Frames[animation.Frame].Texture, x, y));
            }

            if (move != animation.Move || frame != animation.Frame)
            {
                // update timestamp
                animation.TimeStamp = SDL_GetTicks();
            }
        }
        else
        {
            animation.TimeStamp = SDL_GetTicks();
        }

        return done;
    }
}

#endif