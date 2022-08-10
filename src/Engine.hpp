#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "Random.hpp"
#include "Character.hpp"
#include "Map.hpp"

namespace BloodSword::Engine
{
    auto Random = Random::Base();

    void InitializeRNG()
    {
        Random.UniformIntDistribution(1, 6);

        Random.UniformDistribution(0, 1.0);
    }
}

#endif