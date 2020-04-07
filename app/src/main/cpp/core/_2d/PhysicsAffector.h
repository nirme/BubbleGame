#pragma once

#include <memory>

namespace core
{
	namespace _2d
	{
		class RigidObject;

		class PhysicsAffector
		{
		public:

			virtual ~PhysicsAffector(){};
			virtual void affect(RigidObject *_affectedObject, float _frameTime) const = 0;
		};

		typedef std::shared_ptr<PhysicsAffector> PhysicsAffectorPtr;
	}
}
