#pragma once

#include <limits>
#include <cstdlib>
#include "core/_2d/shapes/Rectangle.h"
#include "core/_2d/Particle.h"
#include "core/_2d/ParticleSystem.h"


using namespace core;
using namespace core::_2d;


class InstanceParticleEmitters : public ParticleEmitter
{
protected:
	constexpr static unsigned short Mask = 0x3FF;
	constexpr static unsigned short BitsJumped = 2;
	constexpr static float Div = 1.0f / (Mask + 1);


	Rectangle emittingArea;

	// TODO: particle size inclusion and uniform distribution of particles instead of PxP
	//bool areaIncludeParticleSize;

	unsigned int particlesPerLength;
	Vector2 basePos;
	Vector2 posChange;
	Vector2 baseSize;
	Vector2 sizeChange;

	unsigned int currentParticleCounter;


public:

	InstanceParticleEmitters(ParticleSystem* _parent, unsigned int _particleCount, const Rectangle& _emittingArea);
	virtual ~InstanceParticleEmitters();

	void initParticle(Particle *_prt);
	// override emission rate getter to set as time independent (as expected from single off emitter)
	virtual unsigned int getEmissionCount(float _timeElapsed);
};

