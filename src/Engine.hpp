#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "Graphics.hpp"
#include "Character.hpp"
#include "Random.hpp"

namespace BloodSword::Engine
{
    auto Random = Random::Base();

    void Randomize()
    {
        BloodSword::Engine::Random.UniformIntDistribution(1, 6);
        BloodSword::Engine::Random.UniformDistribution(0, 1.0);
    }
}

#endif