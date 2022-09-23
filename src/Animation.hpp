#ifndef __ANIMATION_HPP__
#define __ANIMATION_HPP__

#include "Primitives.hpp"
#include "Scene.hpp"
#include "Templates.hpp"

namespace BloodSword::Animation
{
    class Frame
    {
    public:
        SDL_Texture *Texture = NULL;

        Uint32 Duration = 0;

        Frame(SDL_Texture *texture, Uint32 duration) : Texture(texture), Duration(duration)
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
        std::vector<Animation::Frame> Frames = {};

        std::vector<Animation::Type> Mode = {};

        std::vector<Point> Path = {};

        Point Origin = Point(0, 0);

        Point Current = Point();

        Point Scale = Point(1, 1);

        Point Offset = Point(0, 0);

        Point Delta = Point(1, 1);

        int Cycles = 1;

        bool Loop = false;

        Uint32 TimeStamp = 0;

        int Frame = 0;

        int Move = 0;

        int Speed = 0;

        int Cycle = 0;

        Base(std::vector<Animation::Frame> frames,
             std::vector<Animation::Type> mode,
             std::vector<Point> path,
             int cycles,
             int speed,
             bool loop) : Frames(frames), Mode(mode), Path(path), Cycles(cycles), Loop(loop), Speed(speed)
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

        Base(std::vector<Animation::Frame> frames,
             std::vector<Point> path,
             int speed) : Base(frames, {Type::MOVE}, path, 1, speed, false) {}

        Base(SDL_Texture *texture,
             std::vector<Point> path,
             int speed) : Base({Animation::Frame(texture)}, {Type::MOVE}, path, 1, speed, false) {}

        Base(std::vector<Animation::Frame> frames, int cycles, bool loop) : Base(frames, {Type::FRAME}, {}, cycles, 0, loop) {}

        Base(std::vector<Animation::Frame> frames, bool loop) : Base(frames, 1, loop) {}

        Base(std::vector<Animation::Frame> frames) : Base(frames, {Type::FRAME}, {}, 1, 0, false) {}

        Base() {}

        void Set(std::vector<Animation::Frame> frames, bool loop)
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

        void Set(std::vector<Animation::Type> modes)
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

        bool Is(std::vector<Animation::Type> types)
        {
            auto result = true;

            for (auto &type : types)
            {
                result &= BloodSword::Found(this->Mode, type);
            }

            return result;
        }

        bool Is(Animation::Type type)
        {
            return this->Is(std::vector<Animation::Type>{type});
        }

        void Add(Animation::Frame frame)
        {
            this->Frames.push_back(frame);
        }

        void Reset()
        {
            this->TimeStamp = 0;

            this->Frame = 0;

            this->Move = 0;

            this->Cycle = 0;

            if (!this->Path.empty())
            {
                this->Current = this->Path[0];

                this->Offset = Point(0, 0);
            }
        }

        bool IsScaled()
        {
            return this->Scale != Point(1, 1);
        }
    };

    bool Show(Scene::Base &scene, Animation::Base &animation, bool delay = true)
    {
        auto done = true;

        if (!animation.Frames.empty() && animation.Frame >= 0 && animation.Frame < animation.Frames.size())
        {
            done = false;

            if (delay)
            {
                SDL_Delay(animation.Frames[animation.Frame].Duration);
            }

            if (animation.Frame < animation.Frames.size() - 1)
            {
                animation.Frame++;
            }
            else if (animation.Loop)
            {
                animation.Frame = 0;
            }
            else if (animation.Cycle < animation.Cycles)
            {
                animation.Frame = 0;

                animation.Cycle++;
            }
            else
            {
                done = true;
            }
        }

        return done;
    }

    bool Move(Scene::Base &scene, Animation::Base &animation, bool delay = true)
    {
        auto Sign = [&](int Value)
        {
            return Value != 0 ? (Value > 0 ? 1 : -1) : 0;
        };

        auto Direction = [&](Point &point)
        {
            return Point(Sign(point.X), Sign(point.Y));
        };

        auto done = true;

        if (!animation.Path.empty() && animation.Move >= 0 && animation.Move < animation.Path.size() - 1)
        {
            if (delay)
            {
                SDL_Delay(animation.Speed);
            }

            auto delta = (animation.Current - animation.Path[animation.Move + 1]) * animation.Scale + animation.Offset;

            // check if we have reached our destination
            if (delta == Point(0, 0))
            {
                animation.Current = animation.Path[animation.Move + 1];

                animation.Offset = Point(0, 0);

                animation.Move++;
            }
            else
            {
                animation.Offset -= Direction(delta) * animation.Delta;
            }

            done = false;
        }

        return done;
    }

    bool Step(Scene::Base &scene, Animation::Base &animation, bool delay = true)
    {
        auto done = false;

        auto move = animation.Move;

        auto frame = animation.Frame;

        if (animation.TimeStamp != 0)
        {
            if (animation.Is({Type::MOVE, Type::FRAME}))
            {
                if (delay)
                {
                    SDL_Delay(animation.Speed);
                }

                Animation::Show(scene, animation, false);

                // stop when move is completed
                done = Animation::Move(scene, animation, false);
            }
            else if (animation.Is(Type::MOVE))
            {
                done = Animation::Move(scene, animation, delay);
            }
            else if (animation.Is(Type::FRAME))
            {
                done = Animation::Show(scene, animation, delay);
            }

            if (animation.Frame >= 0 && animation.Frame < animation.Frames.size())
            {
                auto location = animation.Origin + animation.Current * animation.Scale + animation.Offset;

                // add sprite to scene
                scene.Add(Scene::Element(animation.Frames[animation.Frame].Texture, location));
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

namespace BloodSword::Animations
{
    class Base
    {
    public:
        std::vector<Animation::Base> List = {};

        Uint32 Delay = 0;

        void Set(std::vector<Animation::Base> list)
        {
            this->List = list;
        }

        void Set(Uint32 delay)
        {
            this->Delay = delay;
        }

        Base(std::vector<Animation::Base> list, Uint32 delay) : List(list), Delay(delay) {}

        Base(std::vector<Animation::Base> list) : List(list) {}

        Base(Animation::Base animation) : List(std::vector<Animation::Base>{animation}) {}

        Base() {}

        void Clear()
        {
            this->List.clear();

            this->Delay = 0;
        }

        void Add(Animation::Base &animation)
        {
            this->List.push_back(animation);
        }
    };

    bool Step(Scene::Base &scene, Animations::Base &animations, bool delay = true)
    {
        auto done = true;

        if (!animations.List.empty())
        {
            auto movement = true;

            auto frame = true;

            auto moves = 0;

            auto frames = 0;

            if (delay && animations.List.size() != 1)
            {
                SDL_Delay(animations.Delay);
            }

            for (auto &animation : animations.List)
            {
                if (animation.Is(Animation::Type::MOVE))
                {
                    moves++;

                    movement &= Animation::Step(scene, animation, false || (animations.List.size() == 1 && delay));
                }
                else
                {
                    frames++;

                    frame &= Animation::Step(scene, animation, false || (animations.List.size() == 1 && delay));
                }
            }

            // movement takes precendence
            if (moves >= frames)
            {
                done = movement;
            }
            else
            {
                done = frame;
            }
        }

        return done;
    }
}
#endif
