#include "EmptyObject.h"


namespace core
{
	namespace _2d
	{
		AxisAlignedBox EmptyObject::_boundingBoxImpl() const
		{
			return AxisAlignedBox();
		};

		void EmptyObject::_findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const
		{
			return;
		};

		EmptyObject::EmptyObject(const std::string &_name) : MovableObject(_name)
		{};

		EmptyObject::~EmptyObject()
		{};
	}
}
