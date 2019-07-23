#include "GenericObjectPoolTests.h"
#include "GenericObjectPool.h"
#include "HighPerformanceTimer.h"
#include <stack>
#include <iostream>

namespace Core
{

class Particle
{
	public:

		float m_X, m_Y, m_Z;
		float m_R, m_G, m_B, m_A;
		std::string m_TextureId;

		Particle()
		{
			m_X = m_Y = m_Z = 0.0f;
			m_R = m_G = m_B = m_A = 1.0f;
			m_TextureId = "";
		}

		Particle(float x, float y, float z,
				 float r, float g, float b, float a,
				 const std::string& texture)
		{
			m_X = x;
			m_Y = y;
			m_Z = z;
			
			m_R = r;
			m_G = g; 
			m_B = b;
			m_A = a;

			m_TextureId = texture;
		}
};


static bool Validation_AllocationWorks()
{
	// Test if the particle pool will return the correct results when it's out of items.
	{
		GenericObjectPool<Particle> pool(2);
		Particle* particle0 = pool.Allocate();
		if (NULL == particle0)
			return false;

		Particle* particle1 = pool.Allocate();
		if (NULL == particle1)
			return false;

		Particle* particle2 = pool.Allocate();
		if (NULL != particle2)
			return false;
	}

	// Test if the particle pool can handle non-default constructors.
	{
		GenericObjectPool<Particle> pool(1);
		Particle* particle = new (pool.AllocateUnconstructed()) Particle(0.0f, 10.0f, 2.0f, 
																		 1.0f, 2.0f, 3.0f, 4.0f, 
																		 "smoke_0");
		if (particle->m_X != 0.0f ||
			particle->m_Y != 10.0f || 
			particle->m_Z != 2.0f || 
			particle->m_R != 1.0f || 
			particle->m_G != 2.0f || 
			particle->m_B != 3.0f || 
			particle->m_A != 4.0f || 
			particle->m_TextureId != "smoke_0")
			return false;
	}

	// Program should continue running, if trying to deallocate an object that doesn't belong to the pool.
	// The behaviour here is that it is ignored, but if in debug mode an assertion is triggered.
	{
		GenericObjectPool<Particle> pool(1000);

		// - UNCOMMENT THIS IF YOU WANT AN ASSERTION TRIGGERED. -
		//Particle* particle = new Particle();
		//pool.Deallocate(particle);
	}
	
	// Test if the particle pool behaves properly over many iterations of allocations/deallocations.
	{
		GenericObjectPool<Particle> pool(100);

		for (int currentIteration = 0; currentIteration < 100; currentIteration++)
		{
			std::stack<Particle*> allocatedParticles;

			// First allocate particles.
			for (int i = 0; i < 100; i++)
			{
				Particle* currentAllocation = pool.Allocate();
				if (NULL == currentAllocation)
					return false;	

				allocatedParticles.push(currentAllocation);
			}

			// Allocated all particles, the next one should fail!
			if (NULL != pool.Allocate())
				return false;

			// Then deallocate particles.
			for (int i = 0; i < 100; i++)
			{
				pool.Deallocate(allocatedParticles.top());
				allocatedParticles.pop();
			}
		}
	}

	return true;
}

bool GenericObjectPoolTests::RunValidationTests()
{
	return Validation_AllocationWorks();
}

const int amount = 1000000;
Particle* particles[amount];
void GenericObjectPoolTests::RunPerformanceTests()
{
	HighPerformanceTimer poolTimer;
	HighPerformanceTimer naiveTimer;

	{
		GenericObjectPool<Particle> pool(amount);

		int counter = 0;

		poolTimer.Start();
		for (; counter < amount; counter++)
			particles[counter] = pool.Allocate();
		counter--;

		for (; counter >= 0; counter--)
			pool.Deallocate(particles[counter]);

		poolTimer.Stop();
	}

	{
		int counter = 0;

		naiveTimer.Start();
		for (; counter < amount; counter++)
			particles[counter] = new Particle();
		counter--;

		for (; counter >= 0; counter--)
			delete particles[counter];

		naiveTimer.Stop();
	}

	long double pool = poolTimer.GetTimeMilliseconds();
	long double naive = naiveTimer.GetTimeMilliseconds();

	std::cerr << "pool: " << pool << std::endl;
	std::cerr << "naive: " << naive << std::endl;
}

}