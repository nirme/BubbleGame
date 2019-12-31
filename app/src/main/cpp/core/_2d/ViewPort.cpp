#include "ViewPort.h"


namespace core
{
    namespace _2d
    {

        void ViewPort::_updateCashe() const
        {
            cashedProjectionMatrix = Matrix3(((float)height / width) * scale * 0.5f, 0.0f, 0.0f, 0.0f, scale * 0.5f, 0.0f, 0.0f, 0.0f, 1.0f);

			Vector2 boundary(1.0f / cashedProjectionMatrix.m11, 1.0f / cashedProjectionMatrix.m22);

            cashedFrustrum = AxisAlignedBox(boundary * -1.0f, boundary);
			cashNeedUpdate = false;
        };


        ViewPort::ViewPort(unsigned int _width, unsigned int _height, float _scale) :
            width(_width),
            height(_height),
            scale(_scale),
			cashNeedUpdate(true),
            cashedProjectionMatrix(Matrix3::IDENTITY)
        {
            _updateCashe();
        };


        const Matrix3& ViewPort::getProjectionMatrix() const
        {
            if (cashNeedUpdate)
                _updateCashe();

            return cashedProjectionMatrix;
        };


		const AxisAlignedBox& ViewPort::getFrustrum() const
		{
			if (cashNeedUpdate)
				_updateCashe();

			return cashedFrustrum;
		};


		void ViewPort::setScreenSize(unsigned int _width, unsigned int _height)
        {
            width = _width;
            height = _height;
			cashNeedUpdate = true;
        };

        void ViewPort::setScale(float _scale)
        {
            scale = _scale;
            cashNeedUpdate = true;
        };



    }
}