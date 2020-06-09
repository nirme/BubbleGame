#pragma once

#include <memory>
#include <string>
#include "MovableObject.h"


namespace core
{
	namespace _2d
	{
		class EmptyObject : public MovableObject
		{
		protected:
			virtual AxisAlignedBox _boundingBoxImpl() const;
			virtual void _findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const;

		public:
			EmptyObject(const std::string &_name);
			virtual ~EmptyObject();
		};

		typedef std::unique_ptr<EmptyObject> EmptyObjectUPtr;
		typedef std::shared_ptr<EmptyObject> EmptyObjectPtr;
	}
}
