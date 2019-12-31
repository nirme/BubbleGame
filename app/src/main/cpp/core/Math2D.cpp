#include "Math2D.h"


namespace core
{

    //  by default V2 is treated like point in 2D
    Vector2 operator*(const Matrix3& _m, const Vector2& _v)
    {
        return Vector2(
                _m.m11 * _v.x + _m.m12 * _v.y + _m.m13,
                _m.m21 * _v.x + _m.m22 * _v.y + _m.m23
        );
    };


    //  transform 2D vector
    Vector2 transformVector(const Matrix3& _m, const Vector2& _v)
    {
        return Vector2(
                _m.m11 * _v.x + _m.m12 * _v.y,
                _m.m21 * _v.x + _m.m22 * _v.y
        );
    };


    //  default transformation goes scaling -> rotation -> translation
    Matrix3 affine2DMatrix(const Vector2 &_scale, const _2d::Quaternion &_rotation, const Vector2 &_translation)
    {
        //  mx from quaternion
        //  1.0f - (2 * z * z)		2 * z * w				0
        //  -2 * z * w				1.0f - (2 * z * z)		0
        //  0						0						1

        //  full matrix
        //	Sx * rc		Sy * rs		tx
        //	Sx * -rs	Sy * rc		ty
        //	0			0			1


        float rc = 1.0f - (2.0f * _rotation.z * _rotation.z);
        float rs = 2.0f * _rotation.z * _rotation.w;

        return Matrix3(
                _scale.x * rc,		_scale.y * rs,		_translation.x,
                _scale.x * -rs,		_scale.y * rc,		_translation.y,
                0.0f,				0.0f,				1.0f
        );

    };

    float determinant(const Matrix3 &_mx)
    {
        return (
                       _mx.m11*_mx.m22*_mx.m33 +
                       _mx.m12*_mx.m23*_mx.m31 +
                       _mx.m13*_mx.m21*_mx.m32
               ) - (
                       _mx.m11*_mx.m23*_mx.m32 +
                       _mx.m12*_mx.m21*_mx.m33 +
                       _mx.m13*_mx.m22*_mx.m31
               );
    };



    Matrix3 inverse(const Matrix3 &_mx)
    {
        float det = 1.0f / determinant(_mx);
        return Matrix3(	det * (_mx.m22 * _mx.m33 - _mx.m23 * _mx.m32),
                           det * (_mx.m13 * _mx.m32 - _mx.m12 * _mx.m33),
                           det * (_mx.m12 * _mx.m23 - _mx.m13 * _mx.m22),
                           det * (_mx.m23 * _mx.m31 - _mx.m21 * _mx.m33),
                           det * (_mx.m11 * _mx.m33 - _mx.m13 * _mx.m31),
                           det * (_mx.m13 * _mx.m21 - _mx.m11 * _mx.m23),
                           det * (_mx.m21 * _mx.m32 - _mx.m22 * _mx.m31),
                           det * (_mx.m12 * _mx.m31 - _mx.m11 * _mx.m32),
                           det * (_mx.m11 * _mx.m22 - _mx.m12 * _mx.m21)
        );
    };

}


