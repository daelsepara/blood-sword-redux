#ifndef __ANIMATION_HPP__
#define __ANIMATION_HPP__

#include <vector>

#include "Primitives.hpp"
#include "Color.hpp"
#include "Scene.hpp"
#include "Templates.hpp"

// functions and classes for animating objects on screen
namespace BloodSword::Animation
{
    // animation frame
    class Frame
    {
    public:
        // texture associated with this frame
        SDL_Texture *Texture = nullptr;

        Frame(SDL_Texture *texture) : Texture(texture) {}

        Frame() {}
    };

    // animation type: MOVE (movement), FRAME (cycle through a sequence of frames)
    enum class Type
    {
        MOVE,
        FRAME
    };

    typedef std::vector<Animation::Frame> Frames;

    typedef std::vector<Animation::Type> Types;

    // animation base class
    class Base
    {
    public:
        // frame (texture) to render
        Animation::Frames Frames = {};

        // type of animation(s)
        Animation::Types Mode = {};

        // path on-screen (if MOVE type)
        Points Path = {};

        // origin of graphics cooordinate system, other locations are relative to this
        Point Origin = Point(0, 0);

        // current location (if MOVE type)
        Point Current = Point();

        // movement scaling (X, Y): (1, 1) if on screen, (DX, DY) if on the map
        Point Scale = Point(1, 1);

        // Offset from Current (location), used in MOVE type animation
        Point Offset = Point(0, 0);

        // Number of pixels (X, Y) to move from one location to another
        Point Delta = Point(1, 1);

        // number of cycles Frames is to be shown
        int Cycles = 1;

        // loop through the Frames
        bool Loop = false;

        // frame currently being rendered
        int Frame = 0;

        // Current step (move number) in the path
        int Move = 0;

        // current frames cycle
        int Cycle = 0;

        // default frame dimensions
        int FrameW = BloodSword::TileSize;

        int FrameH = BloodSword::TileSize;

        // trail properties
        int TrailSize = 2;

        // Clipping
        Point Clip = Point(-1, -1);

        int ClipW = 0;

        int ClipH = 0;

        Base(Animation::Frames frames,
             Animation::Types mode,
             Points path,
             int cycles,
             bool loop) : Frames(frames), Mode(mode), Path(path), Cycles(cycles), Loop(loop)
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

        Base(Animation::Frames frames,
             Points path) : Base(frames, {Type::MOVE}, path, 1, false) {}

        Base(SDL_Texture *texture,
             Points path) : Base({Animation::Frame(texture)}, {Type::MOVE}, path, 1, false) {}

        Base(Animation::Frames frames, int cycles, bool loop) : Base(frames, {Type::FRAME}, {}, cycles, loop) {}

        Base(Animation::Frames frames, bool loop) : Base(frames, 1, loop) {}

        Base(Animation::Frames frames) : Base(frames, {Type::FRAME}, {}, 1, false) {}

        Base() {}

        // set animation frames and whether they are looping
        void Set(Animation::Frames frames, bool loop)
        {
            this->Frames = frames;

            this->Loop = loop;

            this->Frame = 0;
        }

        // set path of the object
        void Set(Points path)
        {
            this->Path = path;

            this->Move = 0;
        }

        // set animation types
        void Set(Animation::Types modes)
        {
            this->Mode = modes;
        }

        // set current location
        void Set(Point point)
        {
            this->Current = point;
        }

        // set current location
        void Set(int x, int y)
        {
            this->Set(Point(x, y));
        }

        // set graphics system origin and current location
        void Set(Point origin, Point current)
        {
            this->Origin = origin;

            this->Current = current;
        }

        // set graphics system origin and current location
        void Set(int originx, int originy, int x, int y)
        {
            this->Set(Point(originx, originy), Point(x, y));
        }

        void SetFrameSize(int width, int height)
        {
            this->FrameW = width;

            this->FrameH = height;
        }

        void SetTrailSize(int size)
        {
            this->TrailSize = size;
        }

        // check if animation is of type(s)
        bool Is(Animation::Types types)
        {
            auto result = true;

            for (auto &type : types)
            {
                result &= BloodSword::Found(this->Mode, type);
            }

            return result;
        }

        // check if animation is of type
        bool Is(Animation::Type type)
        {
            return this->Is(Animation::Types{type});
        }

        // add a frame to animation
        void Add(Animation::Frame frame)
        {
            this->Frames.push_back(frame);
        }

        // reset animation
        void Reset()
        {
            this->Frame = 0;

            this->Move = 0;

            this->Cycle = 0;

            if (!this->Path.empty())
            {
                this->Current = this->Path[0];

                this->Offset = Point(0, 0);
            }
        }

        // check if motion is scaled
        bool IsScaled()
        {
            return this->Scale != Point(1, 1);
        }
    };

    // show a frame of the animation on screen and cycle (if possible)
    bool Show(Scene::Base &scene, Animation::Base &animation, bool update)
    {
        auto done = true;

        if (update)
        {
            if (!animation.Frames.empty() && animation.Frame >= 0 && animation.Frame < animation.Frames.size())
            {
                done = false;

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
        }

        return done;
    }

    // move the object towards destination and render it on screen
    bool Move(Scene::Base &scene, Animation::Base &animation, bool update)
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
            auto delta = (animation.Current - animation.Path[animation.Move + 1]) * animation.Scale + animation.Offset;

            // check if we have reached our destination
            if (delta == Point(0, 0))
            {
                animation.Current = animation.Path[animation.Move + 1];

                animation.Offset = Point(0, 0);

                animation.Move++;
            }
            else if (update)
            {
                animation.Offset -= Direction(delta) * animation.Delta;
            }

            done = false;
        }

        return done;
    }

    // update the animation on screen (MOVE, FRAME, both)
    bool Step(Scene::Base &scene, Animation::Base &animation, bool update, bool trail = false)
    {
        auto done = false;

        if (animation.Is({Type::MOVE, Type::FRAME}))
        {
            Animation::Show(scene, animation, update);

            // stop when move is completed
            done = Animation::Move(scene, animation, update);
        }
        else if (animation.Is(Type::MOVE))
        {
            done = Animation::Move(scene, animation, update);
        }
        else if (animation.Is(Type::FRAME))
        {
            done = Animation::Show(scene, animation, update);
        }

        if (animation.Frame >= 0 && animation.Frame < animation.Frames.size())
        {
            // add trail to movement
            if (trail && animation.Is(Type::MOVE))
            {
                for (auto i = animation.Move + 1; i < animation.Path.size() - 1; i++)
                {
                    auto trails = animation.Origin + animation.Path[i] * animation.Scale;

                    scene.Add(Scene::Element(trails, animation.FrameW, animation.FrameH, Color::Inactive, 0, 0));
                }
            }

            // add sprite to scene
            auto location = animation.Origin + animation.Current * animation.Scale + animation.Offset;

            if (trail)
            {
                auto dst = animation.Origin + animation.Path.back() * animation.Scale;

                scene.Add(Scene::Element(dst + (2 * animation.TrailSize), animation.FrameW - (4 * animation.TrailSize), animation.FrameH - (4 * animation.TrailSize), Color::Transparent, Color::Inactive, animation.TrailSize));
            }

            scene.VerifyAndAdd(Scene::Element(animation.Frames[animation.Frame].Texture, location));

            if (trail)
            {
                scene.Add(Scene::Element(location + (2 * animation.TrailSize), animation.FrameW - (4 * animation.TrailSize), animation.FrameW - (4 * animation.TrailSize), Color::Transparent, Color::Inactive, animation.TrailSize));
            }
        }

        return done;
    }
}

namespace BloodSword::Animations
{
    typedef std::vector<Animation::Base> List;

    // collection of objects to animate on screen
    class Base
    {
    public:
        // objects to animate
        Animations::List List = {};

        // global delay (speed)
        Uint64 Delay = 0;

        // global timestamp
        Uint64 TimeStamp = 0;

        // set objects to animate
        void Set(Animations::List list)
        {
            this->List = list;
        }

        // set global delay
        void Set(Uint32 delay)
        {
            this->Delay = delay;
        }

        // Clip Area
        Point Clip = Point(-1, -1);

        int ClipW = 0;

        int ClipH = 0;

        void SetupClipping()
        {
            for (auto i = 0; i < this->List.size(); i++)
            {
                if (!this->List[i].Clip.IsNone())
                {
                    // get max clipping area
                    this->Clip.X = (i == 0) ? this->List[i].Clip.X : std::min(this->Clip.X, this->List[i].Clip.X);

                    this->Clip.Y = (i == 0) ? this->List[i].Clip.Y : std::min(this->Clip.Y, this->List[i].Clip.Y);

                    this->ClipW = std::max(this->ClipW, this->List[i].ClipW);

                    this->ClipH = std::max(this->ClipH, this->List[i].ClipH);
                }
            }
        }

        Base(Animations::List list, Uint32 delay) : List(list), Delay(delay) {}

        Base(Animations::List list) : List(list)
        {
            this->SetupClipping();
        }

        Base(Animation::Base animation) : Base(Animations::List{animation}) {}

        Base() {}

        // clear all animations
        void Clear()
        {
            this->List.clear();

            this->Delay = 0;
        }

        // add animation to queue
        void Add(Animation::Base &animation)
        {
            this->List.push_back(animation);
        }
    };

    // process all animations in the list
    bool Step(Scene::Base &scene, Animations::Base &animations, bool trail = false)
    {
        auto clip = Point(-1, -1);

        auto clip_w = 0;

        auto clip_h = 0;

        auto done = false;

        if (!animations.List.empty())
        {
            auto movement = true;

            auto frame = true;

            auto moves = 0;

            auto frames = 0;

            if (animations.TimeStamp == 0)
            {
                animations.TimeStamp = SDL_GetTicks64();
            }

            // check if frame / sprite needs to be updated / moved
            auto update = (SDL_GetTicks64() - animations.TimeStamp > animations.Delay);

            for (auto &animation : animations.List)
            {
                if (animation.Is(Animation::Type::MOVE))
                {
                    moves++;

                    movement &= Animation::Step(scene, animation, update, trail);
                }
                else
                {
                    frames++;

                    frame &= Animation::Step(scene, animation, update, trail);
                }

                if (!animation.Clip.IsNone())
                {
                    clip.X = (clip.X == -1) ? animation.Clip.X : std::min(clip.X, animation.Clip.X);

                    clip.Y = (clip.Y == -1) ? animation.Clip.Y : std::min(clip.Y, animation.Clip.Y);

                    clip_w = std::max(clip_w, animation.ClipW);

                    clip_h = std::max(clip_h, animation.ClipH);
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

            // reset animation delay
            if (update)
            {
                animations.TimeStamp = SDL_GetTicks64();
            }
        }

        if (!clip.IsNone())
        {
            scene.Clip = clip;

            scene.ClipW = clip_w;

            scene.ClipH = clip_h;
        }

        if (done)
        {
            // reset time stamp
            animations.TimeStamp = 0;
        }

        return done;
    }
}
#endif
