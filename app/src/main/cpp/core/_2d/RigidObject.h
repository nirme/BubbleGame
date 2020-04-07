#pragma once

#include <string>
#include <vector>
#include "AxisAlignedBox.h"
#include "MovableObject.h"
#include "PhysicsSystem.h"
#include "shapes/Shape.h"
#include "PhysicsAffector.h"
#include "../Math2D.h"

namespace core
{
	namespace _2d
	{
		class PhysicsSystem;

		class RigidObject : virtual public MovableObject
		{
		public:
			typedef std::vector<ShapePtr> ShapesList;

		protected:
			typedef std::list<ShapesList> TransformedShapesFrameList;

			PhysicsSystem *collisionSystem;
			std::string collidableObjectType;

			ShapesList shapes;
			mutable TransformedShapesFrameList frameTransformedShapes;

			bool staticObject;
			float bounceFactor;
			Vector2 directionVector;
			std::list<Vector2> positionFrames;


			typedef std::unordered_map<std::string, PhysicsAffectorPtr> AffectorsMap;
			AffectorsMap affectors;


			virtual void _invalidateWorldTransformImpl() const
			{};
			virtual AxisAlignedBox _boundingBoxImpl() const;
			virtual void updateShapes() const;


		public:

			void update(float _frameTime);

			bool isStatic();
			void setStatic(bool _static);

			float getBounceFactor();
			void setBounceFactor(float _bounce);
			const Vector2 &getDirectionVector();
			void setDirectionVector(const Vector2 &_direction);

			const Vector2 &getCurrentPosition();
			const Vector2 &getPreviousPosition();

			virtual void progressTransformation(float _time);
			virtual void progressPartialTransformation(float _time);
			virtual void rewindTransformation();
			void updateTransformation();

			const std::vector<ShapePtr> &getCurrentFrameShapes() const;
			const std::vector<ShapePtr> &getPreviousFrameShapes() const;

			RigidObject(const std::string &_name, const std::string &_collidableObjectType, PhysicsSystem *_collisionSystem = nullptr);

			void addShape(ShapePtr &_shape);
			void addAffector(std::string _name);
			void setAffector(std::string _name, PhysicsAffectorPtr _affector);
			void removeAffector(std::string _name);

			const std::string &getCollidableObjectType() const;
			void registerCollidableObject(PhysicsSystem *_collisionSystem);

		};
	}
}
