#pragma once

#include <memory>
#include <vector>
#include <array>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES
//#include "ParticleSystem.h"
#include "../Texture.h"
#include "../ImageSprite.h"
#include "../Vector2.h"
#include "../Math2D.h"


namespace core
{
	namespace _2d
	{
		class ParticleSystem;

		class Particle
		{
		public:

			Vector2 position;
			float rotation;
			float size;
			float alpha;

			Vector2 direction;
			float rotationalSpeed;
			float sizeChangeSpeed;
			float alphaChange;

			float ageLeft;
			float totalAge;
			unsigned short spriteIndex;
			bool alive;

			enum PARTICLE_TYPE
			{
				PT_NORMAL,
				PT_EMITER,
			} type;

			Particle(PARTICLE_TYPE _type = PT_NORMAL,
					Vector2 _position = 0.0f,
					float _rotation = 0.0f,
					float _size = 1.0f,
					float _alpha = 1.0f,
					Vector2 _direction = 0.0f,
					float _rotationalSpeed = 0.0f,
					float _sizeChangeSpeed = 0.0f,
					float _alphaChange = 0.0f,
					float _totalAge = 0.0f,
					unsigned short _spriteIndex = 0,
					bool _alive = false) :
				position(_position),
				rotation(_rotation),
				size(_size),
				alpha(_alpha),
				direction(_direction),
				rotationalSpeed(_rotationalSpeed),
				sizeChangeSpeed(_sizeChangeSpeed),
				alphaChange(_alphaChange),
				ageLeft(0.0f),
				totalAge(_totalAge),
				spriteIndex(_spriteIndex),
				alive(_alive),
				type(_type)
			{};
		};


		class ParticleEmitter : public Particle
		{
		protected:
		    ParticleSystem *parent;

			bool doesEmitParticles;

			float emissionRate;
			float emissionTimeRemains;

			Vector2 minPosition;
			Vector2 maxPosition;

			float minRotation;
			float maxRotation;

			float minSize;
			float maxSize;

			float minDirection;
			float maxDirection;
			float minDirectionSpeed;
			float maxDirectionSpeed;

			float minRotationalSpeed;
			float maxRotationalSpeed;

			float minSizeChangeSpeed;
			float maxSizeChangeSpeed;

			float minTimeAlive;
			float maxTimeAlive;

		public:

			ParticleEmitter(ParticleSystem *_parent, bool _doesEmitParticles,
							float _emissionRate) :
				Particle(PT_EMITER),
				parent(_parent),
				doesEmitParticles(_doesEmitParticles),
				emissionRate(_emissionRate),
				emissionTimeRemains(0.0f),
				minPosition(0.0f),
				maxPosition(0.0f),
				minRotation(0.0f),
				maxRotation(0.0f),
				minSize(1.0f),
				maxSize(1.0f),
				minDirection(0.0f),
				maxDirection(2 * M_PI),
				minRotationalSpeed(0.0f),
				maxRotationalSpeed(0.0f),
				minSizeChangeSpeed(0.0f),
				maxSizeChangeSpeed(0.0f),
				minTimeAlive(1.0f),
				maxTimeAlive(1.0f)
			{};

			virtual ~ParticleEmitter(){};

			float getEmissionRate()
			{
				return emissionRate;
			};

			void setEmissionRate(float _emissionRate)
			{
				emissionRate = _emissionRate;
			};

			void setEmissionPosition(Vector2 _minPosition, Vector2 _maxPosition)
			{
				minPosition = _minPosition;
				maxPosition = _maxPosition;
			};

			void setEmissionRotation(float _minRotation, float _maxRotation)
			{
				minRotation = _minRotation;
				maxRotation = _maxRotation;
			};

			void setEmissionSize(float _minSize, float _maxSize)
			{
				minSize = _minSize;
				maxSize = _maxSize;
			}

			void setEmissionDirection(float _minDirection, float _maxDirection)
			{
				minDirection = _minDirection;
				maxDirection = _maxDirection;
			};

			void setEmissionLinearSpeed(float _minDirectionSpeed, float _maxDirectionSpeed)
			{
				minDirectionSpeed = _minDirectionSpeed;
				maxDirectionSpeed = _maxDirectionSpeed;
			};

			float getEmissionMinLinearSpeed() const
			{
				return minDirectionSpeed;
			};

			float getEmissionMaxLinearSpeed() const
			{
				return maxDirectionSpeed;
			};

			void setEmissionRotationalSpeed(float _minRotationalSpeed, float _maxRotationalSpeed)
			{
				minRotationalSpeed = _minRotationalSpeed;
				maxRotationalSpeed = _maxRotationalSpeed;
			};

			void setEmissionSizeChangeSpeed(float _minSizeChangeSpeed, float _maxSizeChangeSpeed)
			{
				minSizeChangeSpeed = _minSizeChangeSpeed;
				maxSizeChangeSpeed = _maxSizeChangeSpeed;
			};

			void setEmissionTimeAlive(float _minTimeAlive, float _maxTimeAlive)
			{
				minTimeAlive = _minTimeAlive;
				maxTimeAlive = _maxTimeAlive;
			};

			virtual void initParticle(Particle *_prt) {};

			bool emitParticles()
			{
				return doesEmitParticles;
			};


			virtual unsigned int getEmissionCount(float _timeElapsed)
			{
				return _timeElapsed * emissionRate + emissionTimeRemains;
			};

			void addTimeToRemains(float _time)
			{
				emissionTimeRemains = std::fmod(emissionTimeRemains + (_time * emissionRate), 1.0f);
			};
		};

	}
}
