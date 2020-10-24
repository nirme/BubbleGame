#include "InstanceParticleEmitters.h"


InstanceParticleEmitters::InstanceParticleEmitters(ParticleSystem* _parent, unsigned int _particleCount, const Rectangle& _emittingArea) :
		ParticleEmitter(_parent, true, 0.0f),
		emittingArea(_emittingArea),
		currentParticleCounter(0)
{
	// particle values
	Particle::alive = true;
	Particle::ageLeft = std::numeric_limits<float>::infinity();
	Particle::totalAge = std::numeric_limits<float>::infinity();

	// base values
	ParticleEmitter::minRotation = 0.0f;
	ParticleEmitter::maxRotation = 2.0f * M_PI;
	ParticleEmitter::minSize = 1.0f;
	ParticleEmitter::maxSize = 1.0f;
	ParticleEmitter::minDirection = 0.0f;
	ParticleEmitter::maxDirection = 2.0f * M_PI;
	ParticleEmitter::minDirectionSpeed = 1.0f;
	ParticleEmitter::maxDirectionSpeed = 1.0f;
	ParticleEmitter::minRotationalSpeed = 0.0f;
	ParticleEmitter::maxRotationalSpeed = 0.0f;
	ParticleEmitter::minSizeChangeSpeed = 1.0f;
	ParticleEmitter::maxSizeChangeSpeed = 1.0f;
	ParticleEmitter::minTimeAlive = 1.0f;
	ParticleEmitter::maxTimeAlive = 1.0f;


	particlesPerLength = std::floor(std::sqrt(_particleCount));
	emissionRate = particlesPerLength * particlesPerLength;

	basePos = emittingArea.getPoint(0);
	posChange = {emittingArea.getPoint(3).x - emittingArea.getPoint(0).x, emittingArea.getPoint(1).y - emittingArea.getPoint(0).y};

	baseSize = {emittingArea.getPoint(1).x - emittingArea.getPoint(0).x, emittingArea.getPoint(3).y - emittingArea.getPoint(0).y};

	sizeChange = {(emittingArea.getPoint(2).x - emittingArea.getPoint(3).x) - (emittingArea.getPoint(1).x - emittingArea.getPoint(0).x),
				  (emittingArea.getPoint(2).y - emittingArea.getPoint(1).y) - (emittingArea.getPoint(3).y - emittingArea.getPoint(0).y)};
};


InstanceParticleEmitters::~InstanceParticleEmitters()
{};


void InstanceParticleEmitters::initParticle(Particle *_prt)
{
	ageLeft = -1.0; // set as dead on first particle init to make sure it will be removed before emitting on next frame

	// change div to mul?
	float row = (float)(currentParticleCounter % particlesPerLength) / (particlesPerLength - 1);
	float col = (float)(currentParticleCounter / particlesPerLength) / (particlesPerLength - 1);

	// 0x7FFF guaranteed by rand
	unsigned int seed = std::rand();
	seed ^= seed << 15;


	_prt->position.x = basePos.x + (posChange.x * col) + ((baseSize.x + (sizeChange.x * col)) * row);
	_prt->position.y = basePos.y + (posChange.y * row) + ((baseSize.y + (sizeChange.y * row)) * col);

	_prt->rotation = minRotation + (maxRotation - minRotation) * (((seed >> (0 * BitsJumped)) & Mask) * Div);
	_prt->size = minSize+ (maxSize - minSize) * (((seed >> (1 * BitsJumped)) & Mask) * Div);

	_prt->direction = rotateVector({minDirectionSpeed + (maxDirectionSpeed - minDirectionSpeed) * (((seed >> (2 * BitsJumped)) & Mask) * Div), 0.0f},
								   minDirection + (maxDirection - minDirection) * (((seed >> (6 * BitsJumped)) & Mask) * Div)
	);

	_prt->rotationalSpeed = minRotationalSpeed + (maxRotationalSpeed - minRotationalSpeed) * (((seed >> (4 * BitsJumped)) & Mask ) * Div);
	_prt->sizeChangeSpeed = minSizeChangeSpeed + (maxSizeChangeSpeed - minSizeChangeSpeed) * (((seed >> (3 * BitsJumped)) & Mask ) * Div);

	_prt->totalAge = minTimeAlive + (maxTimeAlive - minTimeAlive) * (((seed >> (5 * BitsJumped)) & Mask) * Div);
	_prt->ageLeft = _prt->totalAge;

	_prt->alpha = 1.0f;
	_prt->alive = true;


	++currentParticleCounter;
};


unsigned int InstanceParticleEmitters::getEmissionCount(float _timeElapsed)
{
	return emissionRate;
};
