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

        std::vector<Point> Path = {};

        std::vector<Animate::Type> Mode = {};

        Point Current = Point();

        bool Loop = false;

        int TimeStamp = 0;

        int Frame = 0;

        int Move = 0;

        int Speed = 0;

        Base(std::vector<Animate::Frame> frames,
             std::vector<Animate::Type> mode,
             std::vector<Point> path,
             int speed,
             bool loop) : Frames(frames), Path(path), Mode(mode), Loop(loop), Speed(speed)
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
            }
        }
    };

    bool Show(Scene::Base &scene, Animate::Base &animation, int timestamp)
    {
        auto done = false;

        if (!animation.Frames.empty() && animation.Frame >= 0 && animation.Frame < animation.Frames.size())
        {
            if ((timestamp - animation.TimeStamp) > animation.Speed)
            {
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

                // update timestamp
                animation.TimeStamp = timestamp;
            }
        }
        else
        {
            done = true;
        }

        return done;
    }

    bool Move(Scene::Base &scene, Animate::Base &animation, int timestamp)
    {
        auto Sign = [&](int Value)
        {
            return Value >= 0 ? 1 : -1;
        };

        auto done = false;

        if (!animation.Path.empty() && animation.Move >= 0 && animation.Move < animation.Path.size())
        {
            if ((timestamp - animation.TimeStamp) > animation.Speed)
            {
                if (animation.Move < animation.Path.size() - 1)
                {
                    auto DeltaX = animation.Current.X - animation.Path[animation.Move + 1].X;

                    auto DeltaY = animation.Current.Y - animation.Path[animation.Move + 1].Y;

                    // check if we have reached our destination
                    if (DeltaX == 0 && DeltaY == 0)
                    {
                        animation.Move++;
                    }
                    else
                    {
                        if (DeltaX != 0)
                        {
                            animation.Current.X -= Sign(DeltaX);
                        }
                        else if (DeltaY != 0)
                        {
                            animation.Current.Y -= Sign(DeltaY);
                        }
                    }
                }
                else
                {
                    done = true;
                }

                // update timestamp
                animation.TimeStamp = timestamp;
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
        auto now = SDL_GetTicks();

        auto done = false;

        if (animation.TimeStamp != 0)
        {
            if (animation.Is({Type::MOVE, Type::FRAME}))
            {
                done = Animate::Move(scene, animation, now) && Animate::Show(scene, animation, now);
            }
            else if (animation.Is(Type::MOVE))
            {
                done = Animate::Move(scene, animation, now);
            }
            else if (animation.Is(Type::FRAME))
            {
                done = Animate::Show(scene, animation, now);
            }
        }
        else
        {
            animation.TimeStamp = now;
        }

        if (animation.Frame >= 0 && animation.Frame < animation.Frames.size())
        {
            // add sprite to scene
            scene.Add(Scene::Element(animation.Frames[animation.Frame].Texture, animation.Current.X, animation.Current.Y));
        }

        return done;
    }
}

#endif