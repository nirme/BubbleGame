#pragma once

#include <vector>

#include "SceneNode.h"
#include "Renderable.h"
#include "../Math2D.h"
#include "RenderQueue.h"



namespace core
{
	namespace _2d
	{

		class Entity : public SceneNode
		{
			typedef std::vector<Renderable*> RenderableList;
			typedef RenderableList::iterator RenderableIterator;
			typedef RenderableList::const_iterator RenderableConstIterator;

		protected:

			RenderableList renderables;


			virtual AxisAlignedBox _boundingBoxImpl() const
			{
				AxisAlignedBox bb;

				const Matrix3& mx = getWorldTransform();
				for (RenderableConstIterator it = renderables.begin(); it != renderables.end(); ++it)
					bb.merge((*it)->getBoundingBox().transform(mx));

				return bb;
			};


			virtual void _findVisibleRenderables(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const
			{
				for (RenderableConstIterator it = renderables.begin(); it != renderables.end(); ++it)
				{
					if ((*it)->isEnabled() && _bounds->isOverlapping((*it)->getBoundingBox()))
						_queue->addRenderable((*it));
				}
			};

		public:

			Entity(const char *_name = nullptr) :
				SceneNode(_name)
			{};

			~Entity()
			{
				while (renderables.size())
				{
					delete renderables.back();
					renderables.pop_back();
				}
			};

			unsigned int appendRenderable(Renderable *_renderable)
			{
				renderables.push_back(_renderable);
				return renderables.size() - 1;
			};


		};

	}
}
