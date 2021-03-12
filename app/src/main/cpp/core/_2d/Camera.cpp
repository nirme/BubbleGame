#include "Camera.h"

namespace core
{
    namespace _2d
    {

		void Camera::_invalidateWorldTransformImpl() const
		{
			cashedViewMatrixNeedUpdate = true;
			cashedInverseViewMatrixNeedUpdate = true;
		};


		Camera::Camera(const char *_name, ViewPort* _viewPort) :
			MovableObject(_name),
			viewPort(_viewPort),
			cashedViewMatrixNeedUpdate(true)
		{};


		const Matrix3& Camera::getViewMatrix() const
		{
			if (cashedViewMatrixNeedUpdate)
			{
				Matrix3 mmm = getWorldTransform();
				cashedViewMatrix = inverse(mmm);
				//cashedViewMatrix = inverse(getWorldTransform());
				cashedViewMatrixNeedUpdate = false;
			}

			return cashedViewMatrix;
		};


		const Matrix3& Camera::getInvViewMatrix() const
		{
			if (cashedInverseViewMatrixNeedUpdate)
			{
				const Matrix3 &mx = getViewMatrix();
				cashedInverseViewMatrix = inverse(mx);
				cashedInverseViewMatrixNeedUpdate = false;
			}

			return cashedInverseViewMatrix;
		};


		void Camera::setViewPort(ViewPort* _viewPort)
		{
			viewPort = _viewPort;
		};


		ViewPort* Camera::getViewPort() const
		{
			return viewPort;
		};

    }
}