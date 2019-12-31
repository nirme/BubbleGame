#pragma once

#include "../Matrix3.h"
#include "../Math2D.h"
#include "SceneNode.h"
#include "MovableObject.h"
#include "RenderQueue.h"
#include "ViewPort.h"

namespace core
{
	namespace _2d
	{

		class Camera : public MovableObject
		{
			ViewPort* viewPort;
		
			mutable bool cashedViewMatrixNeedUpdate;
			mutable Matrix3 cashedViewMatrix;

			
			virtual void _invalidateWorldTransformImpl() const;
			virtual AxisAlignedBox _boundingBoxImpl() const { return AxisAlignedBox(); };
			virtual void _findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const { return; };


		public:

			Camera(const char *_name = "camera", ViewPort* _viewPort = nullptr);
			virtual const Matrix3& getViewMatrix() const;
			virtual void setViewPort(ViewPort* _viewPort);
			virtual ViewPort* getViewPort() const;

		};

	}
}
