#include "PhysicsAffectors.h"
#include "RigidObject.h"



namespace core
{
	namespace _2d
	{

		Vector2 PhysicsLinearAffector::getDirection()
		{
			return direction;
		};


		void PhysicsLinearAffector::setDirection(const Vector2 &_direction)
		{
			direction = _direction;
		};


		PhysicsLinearAffector::PhysicsLinearAffector() : direction(0.0f)
		{};


		PhysicsLinearAffector::~PhysicsLinearAffector()
		{};


		void PhysicsLinearAffector::affect(RigidObject *_affectedObject, float _frameTime) const
		{
			_affectedObject->setDirectionVector(_affectedObject->getDirectionVector() + (direction * _frameTime));
		};

	}
}
