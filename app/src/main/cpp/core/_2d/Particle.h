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
		struct Particle
		{
			Vector2 pos;
			float rotation;
			float size;

			Vector2 linearSpeed;
			float rotationalSpeed;
			float sizeChangeSpeed;

			float timeAlive;
			float totalTimeAlive;
			unsigned short spriteIndex;
			bool alive;
		};


		class ParticleEmiter
		{
		protected:

			float emisionRate;

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


		};


		class ParticleAffector
		{
		public:

			// returns dead particles indexes
			virtual void affect(ParticleSystem &_system, float _timeElapsed) = 0;
		};

		class ParticleSpeedAffector
		{
		public:
			enum SPEED_MODIFIER_OPERATION
			{
				SMO_ADD = 0x00,
				SMO_AVERAGE = 0x01,
			};

		protected:
			Vector2 linearSpeedModifier;


		public:

			virtual void affect(ParticleSystem &_system, float _timeElapsed)
			{
				_p.linearSpeed += linearSpeedModifier * _timeMod;
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
