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


	Vector2 normalize(const Vector2& _v)
	{
		return _v / _v.length();
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
                _scale.x * rc,		_scale.x * rs,		_translation.x,
                _scale.y * -rs,		_scale.y * rc,		_translation.y,
                0.0f,				0.0f,				1.0f
        );
    };


    Matrix3 &translateMatrix(Matrix3 &_mx, const Vector2 &_translation)
    {
        _mx.m13 += _mx.m11 * _translation.x + _mx.m12 * _translation.y;
        _mx.m23 += _mx.m21 * _translation.x + _mx.m22 * _translation.y;
        _mx.m33 += _mx.m31 * _translation.x + _mx.m32 * _translation.y;

        return _mx;
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
        /*
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
        */
        // https://en.wikipedia.org/wiki/Invertible_matrix#Inversion_of_3_%C3%97_3_matrices

        float A = _mx.m22 * _mx.m33 - _mx.m23 * _mx.m32;
        float B = _mx.m23 * _mx.m31 - _mx.m21 * _mx.m33;
        float C = _mx.m21 * _mx.m32 - _mx.m22 * _mx.m31;
        float detDiv = 1.0f / ( _mx.m11 * A + _mx.m12 * B + _mx.m13 * C);

        return Matrix3(detDiv * A,
                       detDiv * (_mx.m13 * _mx.m32 - _mx.m12 * _mx.m33),
                       detDiv * (_mx.m12 * _mx.m23 - _mx.m13 * _mx.m22),
                       detDiv * B,
                       detDiv * (_mx.m11 * _mx.m33 - _mx.m13 * _mx.m31),
                       detDiv * (_mx.m13 * _mx.m21 - _mx.m11 * _mx.m23),
                       detDiv * C,
                       detDiv * (_mx.m12 * _mx.m31 - _mx.m11 * _mx.m32),
                       detDiv * (_mx.m11 * _mx.m22 - _mx.m12 * _mx.m21)
        );

    };


    float dotProduct(Vector2 _v0, Vector2 _v1)
    {
        return _v0.x * _v1.x + _v0.y * _v1.y;
    };


    float dotProduct(_2d::Quaternion _q0, _2d::Quaternion _q1)
    {
        return _q0.w * _q1.w + _q0.z * _q1.z;
    };


    Vector2 normalVectorCCW(const Vector2 & _v)
    {
        return {-_v.y, _v.x};
    };


    Vector2 normalVectorCW(const Vector2 & _v)
    {
        return {_v.y, -_v.x};
    };


    Vector2 rotateVector(const Vector2 &_v, float _r)
    {
        float rc = std::cos(_r);
        float rs = std::sin(_r);
        return Vector2(rc * _v.x + rs * _v.y,
                       -rs * _v.x + rc * _v.y
        );
    };


    Vector2 reflectAcrossNormal(const Vector2 &_v, const Vector2 &_normal)
    {
        return (_normal * (2.0f * (dotProduct(_normal, _v)))) - _v;
    };


    Vector2 reflectAcrossLine(const Vector2 &_v, const Vector2 &_line)
    {
        return (_line * (2.0f * (dotProduct(_line, _v) / dotProduct(_line, _line)))) - _v;
    };


    float degreeToRad(float _r)
    {
        return _r * (M_PI / 180.0f);
    };


    bool contains(const _2d::AxisAlignedBox &_box, const Vector2 &_point)
    {
        if (_box.isBox())
        {
            auto vMin = _box.getMinimum();
            auto vMax = _box.getMaximum();

            if (vMin.x <= _point.x && vMin.y <= _point.y &&
                vMax.x >= _point.x && vMax.y >= _point.y)
                return true;
        }
        else if (_box.isInfinite())
            return true;

        return false;
    };


    bool onLeftSide(const Vector2 &_segmentA, const Vector2 &_segmentB, const Vector2 &_point)
    {
        return dotProduct(normalVectorCCW(_segmentB - _segmentA), _point - _segmentA) > 0.0f;
    };


    bool onRightSide(const Vector2 &_segmentA, const Vector2 &_segmentB, const Vector2 &_point)
    {
        return dotProduct(normalVectorCCW(_segmentB - _segmentA), _point - _segmentA) < 0.0f;
    };


	bool segmentsCrossing(const Vector2 &_segment1A, const Vector2 &_segment1B, const Vector2 &_segment2A, const Vector2 &_segment2B)
	{
		Vector2 seg1Normal = normalVectorCCW(_segment1B - _segment1A);
		float seg1to2A = dotProduct(seg1Normal, _segment2A - _segment1A);
		float seg1to2B = dotProduct(seg1Normal, _segment2B - _segment1A);

		Vector2 seg2Normal = normalVectorCCW(_segment1B - _segment1A);
		float seg2to1A = dotProduct(seg2Normal, _segment1A - _segment2A);
		float seg2to1B = dotProduct(seg2Normal, _segment1B - _segment2A);


		if (seg1to2A * seg1to2B > 0.0f || seg2to1A * seg2to1B > 0.0f)
			return false;

		if (seg1to2A * seg1to2B < 0.0f && seg2to1A * seg2to1B < 0.0f ||
			std::abs(seg1to2A + seg1to2B) < EPSILON)
			return true;

		// segments on the same line
		// check if segments overlap
		return (_segment1A.x - _segment2A.x) * (_segment1A.x - _segment2B.x) <= 0.0f ||
			   (_segment1B.x - _segment2A.x) * (_segment1B.x - _segment2B.x) <= 0.0f;
	};


    float distance(const Vector2 &_segmentA, const Vector2 &_segmentB, const Vector2 &_point)
    {
        Vector2 segmentVect = _segmentB - _segmentA;
        float projDistSq = dotProduct(segmentVect, _point - _segmentA);

        // behind point A
        if (projDistSq <= 0.0f)
            return (_point - _segmentA).length();

        // behind point B
        if (projDistSq >= segmentVect.lengthSq())
            return (_point - _segmentB).length();

        // between A and B
        return std::abs(dotProduct(normalize(normalVectorCCW(segmentVect)), _point - _segmentA));
    };


}


