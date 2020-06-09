#pragma once

#include <string>
#include <vector>
#include "AxisAlignedBox.h"
#include "PhysicsSystem.h"
#include "shapes/Shape.h"
#include "MovableObject.h"
#include "PhysicsAffector.h"
#include "../Math2D.h"

namespace core
{
	namespace _2d
	{
		class PhysicsSystem;

		class RigidObject
		{
		public:
			typedef std::vector<ShapePtr> ShapesList;

			class Listener
			{
			public:
				virtual void onCollisionDetected(RigidObject *_object) = 0;
				virtual void onCollisionEnded(RigidObject *_object) = 0;
				virtual ~Listener(){};
			};


		protected:
			typedef std::list<ShapesList> TransformedShapesFrameList;

			PhysicsSystem *collisionSystem;
			std::string collidableObjectType;

			ShapesList shapes;
			mutable TransformedShapesFrameList frameTransformedShapes;

			mutable bool cashedAABBNeedUpdate;
			mutable AxisAlignedBox cashedAABB;

			MovableObject *entity;
			std::list<Vector2> positionFrames;

			bool enabled;
			bool staticObject;
			float bounceFactor;
			Vector2 directionVector;


			typedef std::unordered_map<std::string, PhysicsAffectorPtr> AffectorsMap;
			AffectorsMap affectors;


			Listener *listener;

			//virtual AxisAlignedBox _boundingBoxImpl() const;

			void updateShapes() const;

		public:

			void registerListener(Listener *_listener = nullptr);
			void collisionDetected(RigidObject *_object);
			void collisionEnded(RigidObject *_object);

			void progress(float _frameTime);

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

			RigidObject(const std::string &_collidableObjectType, PhysicsSystem *_collisionSystem = nullptr);
			~RigidObject();

			void setEntity(MovableObject *_entity);

			void setEnabled(bool _enabled);
			bool isEnabled();

			const AxisAlignedBox &getBoundingBox() const;

			void addShape(ShapePtr &_shape);
			void clearShapes();

			void addAffector(std::string _name);
			void setAffector(std::string _name, PhysicsAffectorPtr _affector);
			void removeAffector(std::string _name);

			const std::string &getCollidableObjectType() const;
			void registerCollidableObject(PhysicsSystem *_collisionSystem);

		};


		typedef std::unique_ptr<RigidObject> RigidObjectUPtr;
		typedef std::shared_ptr<RigidObject> RigidObjectPtr;

	}
}
