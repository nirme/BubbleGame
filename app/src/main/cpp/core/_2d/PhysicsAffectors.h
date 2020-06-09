#pragma once

#include "../Vector2.h"
#include "PhysicsAffector.h"



namespace core
{
	namespace _2d
	{
		class PhysicsLinearAffector : public PhysicsAffector
		{
		protected:
			Vector2 direction;
		public:

			Vector2 getDirection();
			void setDirection(const Vector2 &_direction);

			PhysicsLinearAffector(const Vector2 &_direction = {0.0f});
			virtual ~PhysicsLinearAffector();
			void affect(RigidObject *_affectedObject, float _frameTime) const;
		};


		class PhysicsSpeedAffector : public PhysicsAffector
		{
		public:
			enum ModifierOperation
			{
				MO_SUM = 0x00,
				MO_AVG = 0x01,
				MO_MUL = 0x02,
			};

		protected:
			ModifierOperation operation;
			float factor;

		public:

			void setOperation(ModifierOperation _operation);
			void setFactor(float _factor);

			PhysicsSpeedAffector(ModifierOperation _operation, float _factor);
			virtual ~PhysicsSpeedAffector();
			void affect(RigidObject *_affectedObject, float _frameTime) const;
		};

	}
}
