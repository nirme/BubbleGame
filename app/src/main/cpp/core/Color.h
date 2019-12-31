#pragma once


namespace core
{
	struct ColorInt8;
	struct ColorF;
	
	
	struct ColorInt8
	{
		typedef unsigned char uchar;
		union
		{
			uchar _c[4];
			struct
			{
				uchar red;
				uchar green;
				uchar blue;
				uchar alpha;
			};
		};

		ColorInt8(uchar _red = 0x00, uchar _green = 0x00, uchar _blue = 0x00, uchar _alpha = 0xFF);
		ColorInt8(float _red, float _green, float _blue, float _alpha);
		ColorF getColorF();
	};


	struct ColorF
	{
		typedef unsigned char uchar;
		union
		{
			float _c[4];
			struct
			{
				float red;
				float green;
				float blue;
				float alpha;
			};
		};

		ColorF(float _red = 0.0f, float _green = 0.0f, float _blue = 0.0f, float _alpha = 1.0f);
		ColorF(uchar _red, uchar _green, uchar _blue, uchar _alpha);
		ColorInt8 getColorInt8();

	};
}