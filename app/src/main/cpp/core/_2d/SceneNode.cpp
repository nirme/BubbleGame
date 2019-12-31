#include "SceneNode.h"
#include "SceneManager.h"
#include "Camera.h"
#include "RenderQueue.h"


namespace core
{
    namespace _2d
    {

        void SceneNode::updateBoundingBox() const
        {
            assert(boundingBoxNeedUpdate && "Cashed bounding box don't require updates");

            AxisAlignedBox boundingBox;

            if (children.size())
            {
                for (ChildNodeConstIterator it = children.begin(); it != children.end(); ++it)
                    boundingBox.merge((*it)->getBoundingBox());
            }

			if (objects.size())
			{
				for (ObjectConstIterator it = objects.begin(); it != objects.end(); ++it)
					boundingBox.merge((*it)->getBoundingBox());
			}

            aaBox = boundingBox;

            boundingBoxNeedUpdate = false;
        };


		SceneNode::SceneNode(SceneManager *_owner, const std::string &_name) :
			owner(_owner),
			name(_name),
			parent(nullptr),
			scale(1.0f),
			rotation(0.0f),
			position(0.0f),
			cashedTransformNeedUpdate(true),
			boundingBoxNeedUpdate(true)
		{};



        SceneNode::~SceneNode()
        {
            for (auto it = children.begin(); it != children.end(); ++it)
            {
                delete (*it);
                (*it) = nullptr;
            }
        };


		SceneManager *SceneNode::getOwner() const
		{
			return owner;
		};


		void SceneNode::setParent(SceneNode *_parent)
		{
			parent = _parent;
			invalidateTransform();
		};

		SceneNode *SceneNode::getParent() const
		{
			return parent;
		};


		void SceneNode::setName(const std::string &_name)
		{
			name = _name;
		};

		const std::string &SceneNode::getName() const
		{
			return name;
		};


		void SceneNode::setScale(const Vector2 &_scale)
		{
			scale = _scale;
            invalidateTransform();
        };

		const Vector2 &SceneNode::getScale() const
		{
			return scale;
		};

		void SceneNode::setRotation(const Quaternion &_rotation)
		{
			rotation = _rotation;
            invalidateTransform();
		};

		const Quaternion &SceneNode::getRotation() const
		{
			return rotation;
		};

		void SceneNode::setPosition(const Vector2 &_position)
		{
			position = _position;
            invalidateTransform();
		};

		const Vector2 &SceneNode::getPosition() const
		{
			return position;
		};


		SceneNode *SceneNode::getNodeByName(const std::string &_name)
		{
			if (name.compare(_name) == 0)
				return this;

			SceneNode *node;
			for (unsigned int i = 0, iEnd = children.size(); i < iEnd; ++i)
			{
				if ((node = children[i]->getNodeByName(_name)))
					return node;
			}

			return nullptr;
		};


		SceneNode::ChildNodeIterator SceneNode::getChildIterator(bool _end)
        {
            return (!_end) ? children.begin() : children.end();
        };

        void SceneNode::appendChild(SceneNode* _child)
        {
            SceneNode *prevParent = _child->getParent();
            if (prevParent)
                prevParent->removeChild(_child);

            children.push_back(_child);
            _child->setParent(this);

            invalidateBoundingBox();
        };

        void SceneNode::removeChild(SceneNode* _child)
        {
			auto it = std::find(children.begin(), children.end(), _child);

			_child->setParent(nullptr);
			std::swap(*it, children.back());
			children.pop_back();

			invalidateBoundingBox();
        };

        const AxisAlignedBox& SceneNode::getBoundingBox() const
        {
            if (boundingBoxNeedUpdate)
                updateBoundingBox();

            return aaBox;
        };

        const Matrix3& SceneNode::getWorldTransform() const
        {
			if (cashedTransformNeedUpdate)
			{
				cashedWorldTransform = affine2DMatrix(scale, rotation, position);

				if (parent)
					cashedWorldTransform = parent->getWorldTransform() * cashedWorldTransform;

				cashedTransformNeedUpdate = false;
			}

            return cashedWorldTransform;
        };


        void SceneNode::findVisibleRenderables(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const
        {
            if (_bounds->isOverlapping(getBoundingBox()))
            {
				if (objects.size())
					for (ObjectConstIterator it = objects.begin(); it != objects.end(); ++it)
						(*it)->findVisibleRenderables(_camera, _queue, _bounds);

				if (children.size())
					for (ChildNodeConstIterator it = children.begin(); it != children.end(); ++it)
	                    (*it)->findVisibleRenderables(_camera, _queue, _bounds);
            }
        };

		
		void SceneNode::destroyChild(SceneNode *_child)
		{
			auto it = std::find(children.begin(), children.end(), _child);
			assert(it != children.end() && "cannot remove unattached child");

			_child->destroyAllChildren();
			_child->getOwner()->destroyNode(_child);

			invalidateBoundingBox();
		};

		void SceneNode::destroyAllChildren()
		{
			while (children.size())
			{
				SceneNode* child = children.back();
				child->destroyAllChildren();
				child->getOwner()->destroyNode(child);
			}
			children.clear();

            invalidateBoundingBox();
		};


		void SceneNode::appendObject(MovableObject* _object)
		{
			objects.push_back(_object);
			_object->setParent(this);

            invalidateBoundingBox();
		};


		void SceneNode::removeObject(MovableObject* _object)
		{
			auto it = std::find(objects.begin(), objects.end(), _object);
			assert(it != objects.end() && "cannot remove unattached object");

			(*it)->setParent(nullptr);

			std::swap(*it, objects.back());
			objects.pop_back();

			invalidateBoundingBox();
		};


		void SceneNode::invalidateTransform() const
		{
			cashedTransformNeedUpdate = true;

			for (ChildNodeConstIterator it = children.begin(), itEnd = children.end(); it != itEnd; ++it)
				(*it)->invalidateTransform();

			for (ObjectConstIterator it = objects.begin(), itEnd = objects.end(); it != itEnd; ++it)
				(*it)->invalidateWorldTransform();
		};

		void SceneNode::invalidateBoundingBox() const
		{
			boundingBoxNeedUpdate = true;

			if (parent)
			    parent->invalidateBoundingBox();
		};


    };

}
