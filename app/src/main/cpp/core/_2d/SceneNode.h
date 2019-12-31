#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "../Vector2.h"
#include "Quaternion.h"
#include "../Matrix3.h"

#include "../Math2D.h"
#include "AxisAlignedBox.h"

//#include "SceneManager.h"
#include "MovableObject.h"


namespace core
{
	namespace _2d
	{

		class Camera;
		class RenderQueue;
		class SceneManager;


		class SceneNode
		{
		public:

			typedef std::vector<SceneNode*> ChildNodeList;
			typedef std::vector<SceneNode*>::iterator ChildNodeIterator;
			typedef std::vector<SceneNode*>::const_iterator ChildNodeConstIterator;

			typedef std::vector<MovableObject*> ObjectList;
			typedef std::vector<MovableObject*>::iterator ObjectIterator;
			typedef std::vector<MovableObject*>::const_iterator ObjectConstIterator;



		protected:

			SceneManager *owner;

			std::string name;

			SceneNode* parent;
			ChildNodeList children;

			Vector2 scale;
			Quaternion rotation;
			Vector2 position;


			ObjectList objects;


			mutable bool cashedTransformNeedUpdate;
			mutable Matrix3 cashedWorldTransform;

			mutable bool boundingBoxNeedUpdate;
			mutable AxisAlignedBox aaBox;


			void updateBoundingBox() const;

		public:

			SceneNode(SceneManager *_owner, const std::string &_name = "");
			virtual ~SceneNode();

			SceneManager *getOwner() const;

			virtual void setParent(SceneNode *_parent);
			virtual SceneNode *getParent() const;

			void setName(const std::string &_name);
			const std::string &getName() const;

			void setScale(const Vector2 &_scale);
			const Vector2 &getScale() const;

			void setRotation(const Quaternion &_rotation);
			const Quaternion &getRotation() const;

			void setPosition(const Vector2 &_position);
			const Vector2 &getPosition() const;

			SceneNode *getNodeByName(const std::string &_name);

			ChildNodeIterator getChildIterator(bool _end = false);
			void appendChild(SceneNode* _child);
			void removeChild(SceneNode* _child);
			const AxisAlignedBox& getBoundingBox() const;
			const Matrix3& getWorldTransform() const;
			void findVisibleRenderables(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const;

			void destroyChild(SceneNode *_child);
			void destroyAllChildren();

			void appendObject(MovableObject* _object);
			void removeObject(MovableObject* _object);

			void invalidateTransform() const;
			void invalidateBoundingBox() const;

		};


		typedef std::unique_ptr<SceneNode> SceneNodeUPtr;
		typedef std::shared_ptr<SceneNode> SceneNodePtr;
	}
}
