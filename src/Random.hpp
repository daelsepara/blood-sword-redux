#ifndef __RANDOM_HPP__
#define __RANDOM_HPP__

#include <chrono>
#include <random>

namespace Random
{
	class Base
	{
	private:
		std::mt19937_64 generator;

		std::normal_distribution<double> normalDistribution;

		std::uniform_real_distribution<double> uniformDistribution;

		std::uniform_int_distribution<int> uniformIntDistribution;

	public:
		Base() : generator(std::mt19937_64(std::chrono::system_clock::now().time_since_epoch().count())) {}

		Base(int seed) : generator(std::mt19937_64(seed)) {}

		std::mt19937_64 Generator()
		{
			return this->generator;
		}

		void UniformDistribution(double a, double b)
		{
			uniformDistribution = std::uniform_real_distribution<double>(a, b);
		}

		void UniformIntDistribution(int a, int b)
		{
			uniformIntDistribution = std::uniform_int_distribution<int>(a, b);
		}

		void UniformDistribution()
		{
			UniformDistribution(0.0, 1.0);
		}

		double NextDouble()
		{
			return uniformDistribution(generator);
		}

		int NextInt()
		{
			return uniformIntDistribution(generator);
		}

		int NextInt(int a, int b)
		{
			uniformIntDistribution = std::uniform_int_distribution<int>(a, b);

			return uniformIntDistribution(generator);
		}

		void NormalDistribution(double mean, double std)
		{
			normalDistribution = std::normal_distribution<double>(mean, std);
		}

		void NormalDistribution()
		{
			NormalDistribution(0.0, 1.0);
		}

		double NextNormal()
		{
			return normalDistribution(generator);
		}
	};
}

#endif
