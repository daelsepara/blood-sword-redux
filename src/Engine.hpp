#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "Character.hpp"
#include "Random.hpp"

namespace BloodSword::Engine
{
    auto Random = Random::Base();

    void Initialize()
    {
        Random.UniformIntDistribution(1, 6);
        Random.UniformDistribution(0, 1.0);
    }
}

#endif