#pragma once

#include <vector>
#include "AxisAlignedBox.h"
//#include "SceneNode.h"
#include "RenderQueue.h"



namespace core
{
	namespace _2d
	{
		class SceneNode;
		class Camera;
		class RenderQueue;


		class MovableObject
		{

		protected:

			std::string name;
			SceneNode *parent;
			bool enabled;

			mutable bool boundingBoxNeedUpdate;
			mutable AxisAlignedBox cashedBoundingBox;

			mutable bool cashedTransformNeedUpdate;
			mutable Matrix3 cashedWorldTransform;


			Vector2 scale;
			Quaternion rotation;
			Vector2 position;


			virtual void _invalidateWorldTransformImpl() const {};
			virtual AxisAlignedBox _boundingBoxImpl() const = 0;
			virtual void _findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const = 0;

		public:

			MovableObject(const std::string &_name);
			virtual ~MovableObject();

			void setName(const std::string &_name);
			const std::string &getName() const;

			void setEnabled(bool _enabled);
			bool isEnabled() const;

			void setParent(SceneNode *_parent);
			SceneNode *getParent();

			void setScale(Vector2 _scale);
			Vector2 getScale() const;

			void setRotation(Quaternion _rotation);
			Quaternion getRotation() const;

			void setPosition(Vector2 _position);
			Vector2 getPosition() const;

			void invalidateWorldTransform();
			void invalidateBoundingBox();


			void findVisibleRenderables(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const;
			const AxisAlignedBox &getBoundingBox() const;
			const Matrix3 &getWorldTransform() const;
		};


		typedef std::unique_ptr<MovableObject> MovableObjectUPtr;
		typedef std::shared_ptr<MovableObject> MovableObjectPtr;
	}
}
