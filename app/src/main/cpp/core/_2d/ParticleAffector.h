#pragma once

#include "Particle.h"

namespace core
{
	namespace _2d
	{
		class ParticleSystem;

		class ParticleAffector
		{
		protected:
			ParticleSystem *parent;

		public:
			ParticleAffector(ParticleSystem *_parent);
			virtual ~ParticleAffector();

			// this should never be here
			virtual ParticleAffector *clone(ParticleSystem *_parent) = 0;

			virtual void initParticle(Particle *_prt);
			virtual void affect(ParticleSystem &_system, float _timeElapsed);
		};

	}
}
