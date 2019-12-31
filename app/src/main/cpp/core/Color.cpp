#include "Color.h"


namespace core
{
	ColorInt8::ColorInt8(uchar _red, uchar _green, uchar _blue, uchar _alpha) :
		red(_red), 
		green(_green), 
		blue(_blue), 
		alpha(_alpha)
	{};

	ColorInt8::ColorInt8(float _red, float _green, float _blue, float _alpha) :
		red(_red * 0xFF), 
		green(_green * 0xFF), 
		blue(_blue * 0xFF), 
		alpha(_alpha * 0xFF)
	{};

	ColorF ColorInt8::getColorF()
	{
		return ColorF(red, green, blue, alpha);
	};


	ColorF::ColorF(float _red, float _green, float _blue, float _alpha) :
		red(_red), 
		green(_green), 
		blue(_blue), 
		alpha(_alpha)
	{};


	ColorF::ColorF(uchar _red, uchar _green, uchar _blue, uchar _alpha) :
		red(_red * (1.0f / 255.0f)),
		green(_green * (1.0f / 255.0f)),
		blue(_blue * (1.0f / 255.0f)),
		alpha(_alpha * (1.0f / 255.0f))
	{};

	ColorInt8 ColorF::getColorInt8()
	{
		return ColorInt8(red, green, blue, alpha);
	};

}
