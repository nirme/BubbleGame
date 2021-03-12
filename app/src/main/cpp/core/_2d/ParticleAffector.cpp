#include "ParticleAffector.h"
#include "ParticleSystem.h"


namespace core
{
	namespace _2d
	{
		ParticleAffector::ParticleAffector(ParticleSystem *_parent) :
			parent(_parent)
		{
			parent->addAffector(this);
		};


		ParticleAffector::~ParticleAffector()
		{};


		void ParticleAffector::initParticle(Particle *_prt)
		{};


		void ParticleAffector::affect(ParticleSystem &_system, float _timeElapsed)
		{};
	}
}
