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


		PhysicsLinearAffector::PhysicsLinearAffector(const Vector2 &_direction) : direction(_direction)
		{};


		PhysicsLinearAffector::~PhysicsLinearAffector()
		{};


		void PhysicsLinearAffector::affect(RigidObject *_affectedObject, float _frameTime) const
		{
			_affectedObject->setDirectionVector(_affectedObject->getDirectionVector() + (direction * _frameTime));
		};



		void PhysicsSpeedAffector::setOperation(ModifierOperation _operation)
		{
			operation = _operation;
		};


		void PhysicsSpeedAffector::setFactor(float _factor)
		{
			factor = _factor;
		};


		PhysicsSpeedAffector::PhysicsSpeedAffector(ModifierOperation _operation, float _factor) :
				PhysicsAffector(),
				operation(_operation),
				factor(_factor)
		{};


		PhysicsSpeedAffector::~PhysicsSpeedAffector()
		{};


		void PhysicsSpeedAffector::affect(RigidObject *_affectedObject, float _frameTime) const
		{
			float modifier;

			switch (operation)
			{
				case MO_SUM:
					modifier = 1.0f + factor * _frameTime;
					break;

				case MO_AVG:
					modifier = (1.0f + factor * _frameTime) / (1.0f + _frameTime);
					break;

				case MO_MUL:
					modifier = (1.0f - _frameTime) + (factor * _frameTime);
			}

			_affectedObject->setDirectionVector(_affectedObject->getDirectionVector() * modifier);
		};

	}
}
