#pragma once

#include <memory>
#include <vector>
#include <array>
#include <cstring>
#include "ParticleSystem.h"
#include "../Texture.h"
#include "../ImageSprite.h"
#include "../Vector2.h"
#include "../math.h"


namespace core
{
	namespace _2d
	{
		class ParticleSystem;

		struct Particle
		{
			Vector2 position;
			float rotation;
			float size;
			float alpha;

			Vector2 direction;
			float linearSpeed;
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

			Particle(Vector2 _position = 0.0f,
					float _rotation = 0.0f,
					float _size = 1.0f,
					float _alpha = 1.0f,
					Vector2 _direction = 0.0f,
					float _linearSpeed = 0.0f,
					float _rotationalSpeed = 0.0f,
					float _sizeChangeSpeed = 0.0f,
					float _alphaChange = 0.0f,
					float _totalAge = 0.0f,
					unsigned short _spriteIndex = 0,
					bool _alive = false,
					PARTICLE_TYPE _type = PT_NORMAL) :
				position(_position),
				rotation(_rotation),
				size(_size),
				alpha(_alpha),
				direction(_direction),
				linearSpeed(_linearSpeed),
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
			bool doesEmitParticles;

			float emissionRate;

			Vector2 minPosition;
			Vector2 maxPosition;

			float minRotation;
			float maxRotation;

			float minSize;
			float maxSize;

			Vector2 minDirection;
			Vector2 maxDirection;

			float minLinearSpeed;
			float maxLinearSpeed;

			float minRotationalSpeed;
			float maxRotationalSpeed;

			float minSizeChangeSpeed;
			float maxSizeChangeSpeed;

			float minTimeAlive;
			float maxTimeAlive;

		public:

			void initParticle(Particle *_prt) {};

			bool emitParticles()
			{
				return doesEmitParticles;
			};


			unsigned int getEmissionCount(float _timeElapsed)
			{
				return _timeElapsed * emissionRate;
			};
		};


		class ParticleAffector
		{
		public:

			void initParticle(Particle *_prt) {};
			virtual void affect(ParticleSystem &_system, float _timeElapsed) = 0;
		};

		class ParticleSpeedAffector : public ParticleAffector
		{
		public:
			enum SPEED_MODIFIER_OPERATION
			{
				SMO_ADD = 0x00,
				SMO_AVERAGE = 0x01,
			};

		protected:
			Vector2 direction;
			float linearSpeed;


		public:

			virtual void affect(ParticleSystem &_system, float _timeElapsed)
			{
				_system.linearSpeed += linearSpeedModifier * _timeMod;
			};
		};

		class ParticleSpeedAffector
		{
		protected:
			Vector2 linearSpeedModifier;

		public:

			virtual void updateParticle(Particle& _p, float _timeElapsed)
			{
				_p.linearSpeed += linearSpeedModifier * _timeMod;
			};
		};

	}
}
