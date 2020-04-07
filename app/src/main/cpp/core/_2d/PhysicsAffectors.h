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

			PhysicsLinearAffector();
			virtual ~PhysicsLinearAffector();
			void affect(RigidObject *_affectedObject, float _frameTime) const;
		};

	}
}
