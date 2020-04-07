#include "RigidObject.h"



namespace core
{
	namespace _2d
	{

		AxisAlignedBox RigidObject::_boundingBoxImpl() const
		{
			ShapesList &frame = frameTransformedShapes.front();
			AxisAlignedBox aabb;
			for (unsigned int i = 0, iEnd = frame.size(); i < iEnd; ++i)
			{
				aabb.merge(frame[i]->getAABB());
			}

			return aabb;
		};


		void RigidObject::updateShapes() const
		{
			frameTransformedShapes.emplace_front(shapes.size());
			ShapesList &frame = frameTransformedShapes.front();
			const Matrix3 &mx = this->getWorldTransform();
			for (unsigned int i = 0, iEnd = shapes.size(); i < iEnd; ++i)
			{
				frame[i] = shapes[i]->transform(mx);
			}

			while (frameTransformedShapes.size() > 2)
				frameTransformedShapes.pop_back();
		};


		void RigidObject::update(float _frameTime)
		{
			if (!staticObject && positionFrames.size() > 1)
			{
				for (AffectorsMap::iterator it = affectors.begin(), itEnd = affectors.end(); it != itEnd; ++it)
				{
					(*it).second->affect(this, _frameTime);
				}
			}

			progressTransformation(_frameTime);
			updateTransformation();

			while (positionFrames.size() > 2)
				positionFrames.pop_back();
		};


		bool RigidObject::isStatic()
		{
			return staticObject;
		};


		void RigidObject::setStatic(bool _static)
		{
			staticObject = _static;
		};


		float RigidObject::getBounceFactor()
		{
			return bounceFactor;
		};


		void RigidObject::setBounceFactor(float _bounce)
		{
			bounceFactor = _bounce;
		};


		const Vector2 &RigidObject::getDirectionVector()
		{
			return directionVector;
		};


		void RigidObject::setDirectionVector(const Vector2 &_direction)
		{
			directionVector = _direction;
		};


		const Vector2 &RigidObject::getCurrentPosition()
		{
			return positionFrames.front();
		};


		const Vector2 &RigidObject::getPreviousPosition()
		{
			if (positionFrames.size() > 1)
				return *(++positionFrames.begin());
			else
				return positionFrames.front();
		};


		void RigidObject::progressTransformation(float _time)
		{
			Vector2 worldDirection = transformVector(inverse(parent->getWorldTransform()), directionVector);
			//positionFrames.push_front(
			// getPosition() + (worldDirection * _time));
			positionFrames.push_front( (positionFrames.size() ? positionFrames.front() : getPosition()) + (worldDirection * _time));
		};


		void RigidObject::progressPartialTransformation(float _time)
		{
			Vector2 worldDirection = transformVector(inverse(parent->getWorldTransform()), directionVector);
			positionFrames.front() += (worldDirection * _time);
		};


		void RigidObject::rewindTransformation()
		{
			if (frameTransformedShapes.size())
				frameTransformedShapes.pop_front();
			if (positionFrames.size())
				positionFrames.pop_front();
		};


		void RigidObject::updateTransformation()
		{
			MovableObject::setPosition(positionFrames.front());
			updateShapes();
		};


		const std::vector<ShapePtr> &RigidObject::getCurrentFrameShapes() const
		{
			return frameTransformedShapes.front();
		};


		const std::vector<ShapePtr> &RigidObject::getPreviousFrameShapes() const
		{
			if (frameTransformedShapes.size() <= 2)
				return frameTransformedShapes.back();
			return *(++frameTransformedShapes.begin());
		};


		RigidObject::RigidObject(const std::string &_name, const std::string &_collidableObjectType, PhysicsSystem *_collisionSystem) :
				MovableObject(_name),
				collidableObjectType(_collidableObjectType),
				collisionSystem(nullptr),
				staticObject(false),
				bounceFactor(1.0f),
				directionVector(0.0f)
		{
			if (_collisionSystem)
				registerCollidableObject(_collisionSystem);
		};


		void RigidObject::addShape(ShapePtr &_shape)
		{
			shapes.push_back(std::move(_shape));
		};


		void RigidObject::addAffector(std::string _name)
		{
			std::pair<AffectorsMap::iterator,bool> emplaceResult = affectors.emplace(_name, nullptr);

			if (collisionSystem)
				(*emplaceResult.first).second = collisionSystem->getAffector(_name);
		};


		void RigidObject::setAffector(std::string _name, PhysicsAffectorPtr _affector)
		{
			auto findResult = affectors.find(_name);
			if (findResult != affectors.end())
				(*findResult).second = _affector;
		};


		void RigidObject::removeAffector(std::string _name)
		{
			affectors.erase(_name);
		};


		const std::string &RigidObject::getCollidableObjectType() const
		{
			return collidableObjectType;
		};


		void RigidObject::registerCollidableObject(PhysicsSystem *_collisionSystem)
		{
			if (collisionSystem == _collisionSystem)
				return;

			if (collisionSystem)
				collisionSystem->unregisterObject(this);

			collisionSystem = _collisionSystem;
			if (collisionSystem)
				collisionSystem->registerObject(this);


			for (auto it = affectors.begin(), itEnd = affectors.end(); it != itEnd; ++it)
				(*it).second = collisionSystem->getAffector((*it).first);
		};

	}
}
