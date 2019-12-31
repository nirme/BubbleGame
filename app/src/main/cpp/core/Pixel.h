#pragma once


#include "Logger.h"


namespace core
{
#pragma pack(push, 1)

    struct __attribute__ ((packed)) PIXEL_RGBA_8BIT {
        union {
            unsigned int rgba;
            struct {
                unsigned char r;
                unsigned char g;
                unsigned char b;
                unsigned char a;
            };
        };
    };

    struct __attribute__ ((packed)) PIXEL_RGB_8BIT {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    struct __attribute__ ((packed)) PIXEL_ABGR_8BIT {
        union {
            unsigned int abgr;
            struct {
                unsigned char a;
                unsigned char b;
                unsigned char g;
                unsigned char r;
            };
        };
    };

    struct __attribute__ ((packed)) PIXEL_BGR_8BIT {
        unsigned char b;
        unsigned char g;
        unsigned char r;
    };


    struct __attribute__ ((packed)) PIXEL_RGBA_4BIT {
        union {
            unsigned short rgba;
            struct {
                unsigned char rg;
                unsigned char ba;
            };
        };
    };

    struct __attribute__ ((packed)) PIXEL_RGBA_5551BIT {
        unsigned short rgba;
    };

    struct __attribute__ ((packed)) PIXEL_RGB_565BIT {
        unsigned short rgb;
    };



    // BITMAP SPECIFIC

    struct __attribute__ ((packed)) BITMAP_PIXEL_ARGB_8BIT {
        union {
            unsigned int argb;
            struct {
                unsigned char a;
                unsigned char r;
                unsigned char g;
                unsigned char b;
            };
        };
    };


    struct __attribute__ ((packed)) BITMAP_PIXEL_BGRA_8BIT {
        union {
            unsigned int bgra;
            struct {
                unsigned char b;
                unsigned char g;
                unsigned char r;
                unsigned char a;
            };
        };
    };



#pragma pack(pop)


    // PIXEL_RGBA_8BIT

    inline void pixelConverter(const PIXEL_RGBA_8BIT &_in, PIXEL_RGBA_8BIT &_out )
    {
        _out.rgba = _in.rgba;
    };

    inline void pixelConverter(const PIXEL_RGBA_8BIT &_in, PIXEL_RGB_8BIT &_out )
    {
        _out.r = _in.r;
        _out.g = _in.g;
        _out.b = _in.b;
    };

    inline void pixelConverter(const PIXEL_RGBA_8BIT &_in, PIXEL_ABGR_8BIT &_out )
    {
        _out.abgr = ((_in.rgba & 0xFF000000) >> 24) |
                    ((_in.rgba & 0x00FF0000) >> 8 ) |
                    ((_in.rgba & 0x0000FF00) << 8 ) |
                    ((_in.rgba & 0x000000FF) << 24);
    };

    inline void pixelConverter(const PIXEL_RGBA_8BIT &_in, PIXEL_BGR_8BIT &_out )
    {
        _out.r = _in.r;
        _out.g = _in.g;
        _out.b = _in.b;
    };

    inline void pixelConverter(const PIXEL_RGBA_8BIT &_in, PIXEL_RGBA_4BIT &_out )
    {
        _out.rgba = (unsigned short)((_in.r & 0xF0) << 8 ) |
                                    ((_in.g & 0xF0) << 4 ) |
                                    ((_in.b & 0xF0)      ) |
                                    ((_in.a & 0xF0) >> 4 );
		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_RGBA_8BIT &_in, PIXEL_RGBA_5551BIT &_out )
    {
        _out.rgba = ((unsigned short)(_in.r & 0xF8) << 8 ) |
					((unsigned short)(_in.g & 0xF8) << 3 ) |
					((unsigned short)(_in.b & 0xF8) >> 2) |
					((unsigned short)(_in.a & 0x80) >> 7 );
		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_RGBA_8BIT &_in, PIXEL_RGB_565BIT &_out )
    {
        _out.rgb =  (unsigned short)((unsigned short)(_in.r & 0xF8) << 8) |
                                    ((unsigned short)(_in.g & 0xFC) << 3) |
                                    ((unsigned short)(_in.b & 0xF8) >> 3);
 		_out.rgb = (_out.rgb >> 8) | (_out.rgb << 8); //little endian req
   };

    inline void pixelConverter(const PIXEL_RGBA_8BIT &_in, BITMAP_PIXEL_ARGB_8BIT &_out )
    {
		_out.a = _in.a;
		_out.r = _in.r;
		_out.g = _in.g;
		_out.b = _in.b;
    };

    inline void pixelConverter(const PIXEL_RGBA_8BIT &_in, BITMAP_PIXEL_BGRA_8BIT &_out )
    {
		_out.a = _in.a;
		_out.r = _in.r;
		_out.g = _in.g;
		_out.b = _in.b;
    };


    // PIXEL_RGB_8BIT

    inline void pixelConverter(const PIXEL_RGB_8BIT &_in, PIXEL_RGBA_8BIT &_out )
    {
        _out.rgba = (((unsigned int)_in.r)      ) |
                    (((unsigned int)_in.g) << 8 ) |
                    (((unsigned int)_in.b) << 16) |
                    0xFF000000;
    };

    inline void pixelConverter(const PIXEL_RGB_8BIT &_in, PIXEL_RGB_8BIT &_out )
    {
        _out.r = _in.r;
        _out.g = _in.g;
        _out.b = _in.b;
    };

    inline void pixelConverter(const PIXEL_RGB_8BIT &_in, PIXEL_ABGR_8BIT &_out )
    {
        _out.abgr = 0x000000FF |
                    (((unsigned int)_in.b) << 8 ) |
                    (((unsigned int)_in.g) << 16) |
                    (((unsigned int)_in.r) << 24);
    };

    inline void pixelConverter(const PIXEL_RGB_8BIT &_in, PIXEL_BGR_8BIT &_out )
    {
        _out.r = _in.r;
        _out.g = _in.g;
        _out.b = _in.b;
    };

    inline void pixelConverter(const PIXEL_RGB_8BIT &_in, PIXEL_RGBA_4BIT &_out )
    {
        _out.rgba = (((unsigned short)(_in.r & 0xF0)) << 8 ) |
                    (((unsigned short)(_in.g & 0xF0)) << 4 ) |
                    (_in.b & 0xF0) |
                    0x0F;
		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_RGB_8BIT &_in, PIXEL_RGBA_5551BIT &_out )
    {
        _out.rgba = (((unsigned short)(_in.r & 0xF8)) << 8) |
                    (((unsigned short)(_in.g & 0xF8)) << 3) |
                    ((_in.b & 0xF8) >> 2) |
                    0x01;
		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_RGB_8BIT &_in, PIXEL_RGB_565BIT &_out )
    {
        _out.rgb =  (((unsigned short)(_in.r & 0xF8)) << 8) |
                    (((unsigned short)(_in.g & 0xFC)) << 3) |
                    ((_in.b & 0xF8) >> 3);
		_out.rgb = (_out.rgb >> 8) | (_out.rgb << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_RGB_8BIT &_in, BITMAP_PIXEL_ARGB_8BIT &_out )
    {
    };

    inline void pixelConverter(const PIXEL_RGB_8BIT &_in, BITMAP_PIXEL_BGRA_8BIT &_out )
    {
        _out.b = _in.b;
        _out.g = _in.g;
        _out.r = _in.r;
        _out.a = 0xFF;
    };


    // PIXEL_ABGR_8BIT

    inline void pixelConverter(const PIXEL_ABGR_8BIT &_in, PIXEL_RGBA_8BIT &_out )
    {
        _out.rgba = ((_in.abgr & 0xFF000000) >> 24) |
                    ((_in.abgr & 0x00FF0000) >> 8 ) |
                    ((_in.abgr & 0x0000FF00) << 8 ) |
                    ((_in.abgr & 0x000000FF) << 24);
    };

    inline void pixelConverter(const PIXEL_ABGR_8BIT &_in, PIXEL_RGB_8BIT &_out )
    {
        _out.r = _in.r;
        _out.g = _in.g;
        _out.b = _in.b;
    };

    inline void pixelConverter(const PIXEL_ABGR_8BIT &_in, PIXEL_ABGR_8BIT &_out )
    {
        _out.abgr = _in.abgr;
    };

    inline void pixelConverter(const PIXEL_ABGR_8BIT &_in, PIXEL_BGR_8BIT &_out )
    {
        _out.r = _in.r;
        _out.g = _in.g;
        _out.b = _in.b;
    };

    inline void pixelConverter(const PIXEL_ABGR_8BIT &_in, PIXEL_RGBA_4BIT &_out )
    {
        _out.rgba = ((unsigned short)(_in.r & 0xF0) << 8 ) |
					((unsigned short)(_in.g & 0xF0) << 4 ) |
                                    ((_in.b & 0xF0)      ) |
                                    ((_in.a & 0xF0) >> 4 );
		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_ABGR_8BIT &_in, PIXEL_RGBA_5551BIT &_out )
    {
        _out.rgba = ((unsigned short)(_in.r & 0xF8) << 8 ) |
					((unsigned short)(_in.g & 0xF8) << 3 ) |
                                    ((_in.b & 0xF8) >> 2) |
                                    ((_in.a & 0x80) >> 7);
		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_ABGR_8BIT &_in, PIXEL_RGB_565BIT &_out )
    {
        _out.rgb =  (unsigned short)((_in.r & 0xF8) << 8 ) |
                                    ((_in.g & 0xFC) >> 3 ) |
                                    ((_in.b & 0xF8) >> 3);
		_out.rgb = (_out.rgb >> 8) | (_out.rgb << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_ABGR_8BIT &_in, BITMAP_PIXEL_ARGB_8BIT &_out )
    {
        _out.a = _in.a;
        _out.r = _in.r;
        _out.g = _in.g;
        _out.b = _in.b;
    };

    inline void pixelConverter(const PIXEL_ABGR_8BIT &_in, BITMAP_PIXEL_BGRA_8BIT &_out )
    {
        _out.b = _in.b;
        _out.g = _in.g;
        _out.r = _in.r;
        _out.a = _in.a;
    };


    // PIXEL_BGR_8BIT

    inline void pixelConverter(const PIXEL_BGR_8BIT &_in, PIXEL_RGBA_8BIT &_out )
    {
        _out.rgba = (((unsigned int)_in.r)      ) |
                    (((unsigned int)_in.g) << 8 ) |
                    (((unsigned int)_in.b) << 16) |
					0xFF000000;
    };

    inline void pixelConverter(const PIXEL_BGR_8BIT &_in, PIXEL_RGB_8BIT &_out )
    {
        _out.r = _in.r;
        _out.g = _in.g;
        _out.b = _in.b;
    };

    inline void pixelConverter(const PIXEL_BGR_8BIT &_in, PIXEL_ABGR_8BIT &_out )
    {
        _out.abgr = 0x000000FF |
                    (((unsigned int)_in.b) << 8 ) |
                    (((unsigned int)_in.g) << 16) |
                    (((unsigned int)_in.r) << 24);
    };

    inline void pixelConverter(const PIXEL_BGR_8BIT &_in, PIXEL_BGR_8BIT &_out )
    {
        _out.r = _in.r;
        _out.g = _in.g;
        _out.b = _in.b;
    };

    inline void pixelConverter(const PIXEL_BGR_8BIT &_in, PIXEL_RGBA_4BIT &_out )
    {
        _out.rgba = (((unsigned short)(_in.r & 0xF0)) << 8 ) |
                    (((unsigned short)(_in.g & 0xF0)) << 4 ) |
                                      (_in.b & 0xF0) |
										0x0F;
		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_BGR_8BIT &_in, PIXEL_RGBA_5551BIT &_out )
    {
        _out.rgba = (((unsigned short)(_in.r & 0xF8)) << 8) |
                    (((unsigned short)(_in.g & 0xF8)) << 3) |
                                     ((_in.b & 0xF8) >> 2) |
										0x01;
		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_BGR_8BIT &_in, PIXEL_RGB_565BIT &_out )
    {
        _out.rgb = ((unsigned short)(_in.r & 0xF8) << 8) |
                   ((unsigned short)(_in.g & 0xFC) << 3) |
								   ((_in.b & 0xF8) >> 3);
		_out.rgb = (_out.rgb >> 8) | (_out.rgb << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_BGR_8BIT &_in, BITMAP_PIXEL_ARGB_8BIT &_out )
    {
        _out.a = 0xFF;
        _out.r = _in.r;
        _out.g = _in.g;
        _out.b = _in.b;
    };

    inline void pixelConverter(const PIXEL_BGR_8BIT &_in, BITMAP_PIXEL_BGRA_8BIT &_out )
    {
        _out.b = _in.b;
        _out.g = _in.g;
        _out.r = _in.r;
        _out.a = 0xFF;
    };


    // PIXEL_RGBA_4BIT

    inline void pixelConverter(const PIXEL_RGBA_4BIT &_in, PIXEL_RGBA_8BIT &_out )
    {
        _out.rgba = ((unsigned int)(_in.rgba & 0x00F0)      ) |
                    ((unsigned int)(_in.rgba & 0x000F) << 12) |
                    ((unsigned int)(_in.rgba & 0xF000) << 8 ) |
                    ((unsigned int)(_in.rgba & 0x0F00) << 20);
    };

    inline void pixelConverter(const PIXEL_RGBA_4BIT &_in, PIXEL_RGB_8BIT &_out )
    {
        _out.r = _in.rg & 0xF0;
        _out.g = _in.rg << 4;
        _out.b = _in.ba & 0xF0;
    };

    inline void pixelConverter(const PIXEL_RGBA_4BIT &_in, PIXEL_ABGR_8BIT &_out )
    {
        _out.abgr = (((unsigned int)(_in.rgba & 0x0F00)) >> 4 ) |
                    (((unsigned int)(_in.rgba & 0xF000))      ) |
                                   ((_in.rgba & 0x000F)  << 20) |
                                   ((_in.rgba & 0x00F0)  >> 24);
    };

    inline void pixelConverter(const PIXEL_RGBA_4BIT &_in, PIXEL_BGR_8BIT &_out )
    {
        _out.r = _in.rg & 0xF0;
        _out.g = _in.rg << 4;
        _out.b = _in.ba & 0xF0;
    };

    inline void pixelConverter(const PIXEL_RGBA_4BIT &_in, PIXEL_RGBA_4BIT &_out )
    {
        _out.rgba = _in.rgba;
    };

    inline void pixelConverter(const PIXEL_RGBA_4BIT &_in, PIXEL_RGBA_5551BIT &_out )
    {
        _out.rgba = ((_in.rgba & 0x00F0) << 8) |
                    ((_in.rgba & 0x000F) << 7) |
                    ((_in.rgba & 0xF000) >> 10) |
                    ((_in.rgba & 0x0F00) >> 11);
		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_RGBA_4BIT &_in, PIXEL_RGB_565BIT &_out )
    {
        _out.rgb =  ((_in.rgba & 0x00F0) << 8) |
                    ((_in.rgba & 0x000F) << 7) |
                    ((_in.rgba & 0xF000) >> 11);
		_out.rgb = (_out.rgb >> 8) | (_out.rgb << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_RGBA_4BIT &_in, BITMAP_PIXEL_ARGB_8BIT &_out )
    {
        _out.a = (unsigned char)((_in.rgba & 0x0F00) >> 4);
        _out.r = (unsigned char)((_in.rgba & 0x00F0));
        _out.g = (unsigned char)((_in.rgba & 0x000F) << 4);
        _out.b = (unsigned char)((_in.rgba & 0xF000) >> 8);
    };

    inline void pixelConverter(const PIXEL_RGBA_4BIT &_in, BITMAP_PIXEL_BGRA_8BIT &_out )
    {
        _out.b = (unsigned char)((_in.rgba & 0xF000) >> 8);
        _out.g = (unsigned char)((_in.rgba & 0x000F) << 4);
        _out.r = (unsigned char)((_in.rgba & 0x00F0));
        _out.a = (unsigned char)((_in.rgba & 0x0F00) >> 4);
    };


    // PIXEL_RGBA_5551BIT

    inline void pixelConverter(const PIXEL_RGBA_5551BIT &_in, PIXEL_RGBA_8BIT &_out )
    {
		unsigned short tmp = (_in.rgba >> 8) | (_in.rgba << 8); //little endian req

        _out.r = (unsigned char)((tmp & 0xF800) >> 8);
        _out.g = (unsigned char)((tmp & 0x07C0) >> 3);
        _out.b = (unsigned char)((tmp & 0x003E) << 2);

        _out.a = (tmp & 0x0001) | ((tmp & 0x0001) << 4);
        _out.a |= _out.a << 2;
        _out.a |= _out.a << 1;
    };

    inline void pixelConverter(const PIXEL_RGBA_5551BIT &_in, PIXEL_RGB_8BIT &_out )
    {
		unsigned short tmp = (_in.rgba >> 8) | (_in.rgba << 8); //little endian req

        _out.r = (unsigned char)((tmp & 0xF800) >> 8);
        _out.g = (unsigned char)((tmp & 0x07C0) >> 3);
        _out.b = (unsigned char)((tmp & 0x003E) << 2);
    };

    inline void pixelConverter(const PIXEL_RGBA_5551BIT &_in, PIXEL_ABGR_8BIT &_out )
    {
		unsigned short tmp = (_in.rgba >> 8) | (_in.rgba << 8); //little endian req

        _out.a = (tmp & 0x0001) | ((tmp & 0x0001) << 4);
        _out.a |= _out.a << 2;
        _out.a |= _out.a << 1;

        _out.b = (unsigned char)((tmp & 0x003E) << 2);
        _out.g = (unsigned char)((tmp & 0x07C0) >> 3);
        _out.r = (unsigned char)((tmp & 0xF800) >> 8);
    };

    inline void pixelConverter(const PIXEL_RGBA_5551BIT &_in, PIXEL_BGR_8BIT &_out )
    {
		unsigned short tmp = (_in.rgba >> 8) | (_in.rgba << 8); //little endian req

        _out.b = (unsigned char)((tmp & 0x003E) << 2);
        _out.g = (unsigned char)((tmp & 0x07C0) >> 3);
        _out.r = (unsigned char)((tmp & 0xF800) >> 8);
    };

    inline void pixelConverter(const PIXEL_RGBA_5551BIT &_in, PIXEL_RGBA_4BIT &_out )
    {
		unsigned short tmp = (_in.rgba >> 8) | (_in.rgba << 8); //little endian req

        _out.rgba = ((tmp & 0xF000)     ) |
                    ((tmp & 0x0780) << 1) |
                    ((tmp & 0x003C) << 2) |
                    ((tmp & 0x0001)     ) |
                    ((tmp & 0x0001) << 1) |
                    ((tmp & 0x0001) << 2) |
                    ((tmp & 0x0001) << 3);
					
		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_RGBA_5551BIT &_in, PIXEL_RGBA_5551BIT &_out )
    {
        _out.rgba = _in.rgba;
    };

    inline void pixelConverter(const PIXEL_RGBA_5551BIT &_in, PIXEL_RGB_565BIT &_out )
    {
		unsigned short tmp = (_in.rgba >> 8) | (_in.rgba << 8); //little endian req

        _out.rgb = ((tmp & 0xFFC0)     ) |
                   ((tmp & 0x003E) >> 1);

		_out.rgb = (_out.rgb >> 8) | (_out.rgb << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_RGBA_5551BIT &_in, BITMAP_PIXEL_ARGB_8BIT &_out )
    {
 		unsigned short tmp = (_in.rgba >> 8) | (_in.rgba << 8); //little endian req

		_out.a = (tmp & 0x0001) | ((tmp & 0x0001) << 4);
		_out.a |= _out.a << 2;
		_out.a |= _out.a << 1;
		
        _out.r = (unsigned char)((tmp & 0xF800) >> 8 );
        _out.g = (unsigned char)((tmp & 0x07C0) >> 3 );
        _out.b = (unsigned char)((tmp & 0x003E) << 2 );
    };

    inline void pixelConverter(const PIXEL_RGBA_5551BIT &_in, BITMAP_PIXEL_BGRA_8BIT &_out )
    {
 		unsigned short tmp = (_in.rgba >> 8) | (_in.rgba << 8); //little endian req

        _out.b = (unsigned char)((tmp & 0x003E) << 2 );
        _out.g = (unsigned char)((tmp & 0x07C0) >> 3 );
        _out.r = (unsigned char)((tmp & 0xF800) >> 8 );

		_out.a = (tmp & 0x0001) | ((tmp & 0x0001) << 4);
		_out.a |= _out.a << 2;
		_out.a |= _out.a << 1;
};


    // PIXEL_RGB_565BIT

    inline void pixelConverter(const PIXEL_RGB_565BIT &_in, PIXEL_RGBA_8BIT &_out )
    {
		unsigned short tmp = (_in.rgb >> 8) | (_in.rgb << 8); //little endian req
		
       _out.r = (unsigned char)((tmp & 0xF800) >> 8);
       _out.g = (unsigned char)((tmp & 0x07E0) >> 3);
       _out.b = (unsigned char)((tmp & 0x001F) << 3);
       _out.a = 0xFF;
    };

    inline void pixelConverter(const PIXEL_RGB_565BIT &_in, PIXEL_RGB_8BIT &_out )
    {
		unsigned short tmp = (_in.rgb >> 8) | (_in.rgb << 8); //little endian req
		
       _out.r = (unsigned char)((tmp & 0xF800) >> 8);
       _out.g = (unsigned char)((tmp & 0x07E0) >> 3);
       _out.b = (unsigned char)((tmp & 0x001F) << 3);
    };

    inline void pixelConverter(const PIXEL_RGB_565BIT &_in, PIXEL_ABGR_8BIT &_out )
    {
		unsigned short tmp = (_in.rgb >> 8) | (_in.rgb << 8); //little endian req
		
       _out.a = 0xFF;
       _out.b = (unsigned char)((tmp & 0x001F) << 3);
       _out.g = (unsigned char)((tmp & 0x07E0) >> 3);
       _out.r = (unsigned char)((tmp & 0xF800) >> 8);
    };

    inline void pixelConverter(const PIXEL_RGB_565BIT &_in, PIXEL_BGR_8BIT &_out )
    {
		unsigned short tmp = (_in.rgb >> 8) | (_in.rgb << 8); //little endian req
		
       _out.b = (unsigned char)((tmp & 0x001F) << 3);
       _out.g = (unsigned char)((tmp & 0x07E0) >> 3);
       _out.r = (unsigned char)((tmp & 0xF800) >> 8);
    };

    inline void pixelConverter(const PIXEL_RGB_565BIT &_in, PIXEL_RGBA_4BIT &_out )
    {
		unsigned short tmp = (_in.rgb >> 8) | (_in.rgb << 8); //little endian req
		
        _out.rgba = ((tmp & 0xF000)     ) |
                    ((tmp & 0x0780) << 1) |
                    ((tmp & 0x001E) << 3) |
                    0x000F;

		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_RGB_565BIT &_in, PIXEL_RGBA_5551BIT &_out )
    {
		unsigned short tmp = (_in.rgb >> 8) | (_in.rgb << 8); //little endian req

        _out.rgba = (tmp & 0xFFC0) |
                    (tmp & 0x001F) << 1 |
                    0x01;

		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const PIXEL_RGB_565BIT &_in, PIXEL_RGB_565BIT &_out )
    {
        _out.rgb = _in.rgb;
    };

    inline void pixelConverter(const PIXEL_RGB_565BIT &_in, BITMAP_PIXEL_ARGB_8BIT &_out )
    {
		unsigned short tmp = (_in.rgb >> 8) | (_in.rgb << 8); //little endian req

        _out.a = 0xFF;
        _out.r = (unsigned char)((tmp & 0xF800) >> 8 );
		_out.g = (unsigned char)((tmp & 0x07E0) >> 3 );
		_out.b = (unsigned char)((tmp & 0x001F) << 3 );
    };

    inline void pixelConverter(const PIXEL_RGB_565BIT &_in, BITMAP_PIXEL_BGRA_8BIT &_out )
    {
		unsigned short tmp = (_in.rgb >> 8) | (_in.rgb << 8); //little endian req

		_out.b = (unsigned char)((tmp & 0x001F) << 3 );
		_out.g = (unsigned char)((tmp & 0x07E0) >> 3 );
        _out.r = (unsigned char)((tmp & 0xF800) >> 8 );
        _out.a = 0xFF;
    };


    // BITMAP_PIXEL_ARGB_8BIT

    inline void pixelConverter(const BITMAP_PIXEL_ARGB_8BIT &_in, PIXEL_RGBA_8BIT &_out )
    {
		_out.r = _in.r;
		_out.g = _in.g;
		_out.b = _in.b;
		_out.a = _in.a;
    };

    inline void pixelConverter(const BITMAP_PIXEL_ARGB_8BIT &_in, PIXEL_RGB_8BIT &_out )
    {
		_out.r = _in.r;
		_out.g = _in.g;
		_out.b = _in.b;
    };

    inline void pixelConverter(const BITMAP_PIXEL_ARGB_8BIT &_in, PIXEL_ABGR_8BIT &_out )
    {
		_out.a = _in.a;
		_out.b = _in.b;
		_out.g = _in.g;
		_out.r = _in.r;
    };

    inline void pixelConverter(const BITMAP_PIXEL_ARGB_8BIT &_in, PIXEL_BGR_8BIT &_out )
    {
        _out.b = _in.b;
        _out.g = _in.g;
        _out.r = _in.r;
    };

    inline void pixelConverter(const BITMAP_PIXEL_ARGB_8BIT &_in, PIXEL_RGBA_4BIT &_out )
    {
        _out.rgba = (((unsigned short)(_in.r & 0xF0)) << 8) |
					(((unsigned short)(_in.g & 0xF0)) << 4) |
					(((unsigned short)(_in.b & 0xF0))     ) |
					(((unsigned short)(_in.a & 0xF0)) >> 4);

		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const BITMAP_PIXEL_ARGB_8BIT &_in, PIXEL_RGBA_5551BIT &_out )
    {
        _out.rgba = (((unsigned short)(_in.r & 0xF8)) << 8) |
					(((unsigned short)(_in.g & 0xF8)) << 3) |
					(((unsigned short)(_in.b & 0xF8)) >> 2) |
					(((unsigned short)(_in.a & 0x80)) >> 7);

		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const BITMAP_PIXEL_ARGB_8BIT &_in, PIXEL_RGB_565BIT &_out )
    {
        _out.rgb =  (((unsigned short)(_in.r & 0xF8)) << 8) |
					(((unsigned short)(_in.g & 0xFC)) << 3) |
					(((unsigned short)(_in.b & 0xF8)) >> 3);

		_out.rgb = (_out.rgb >> 8) | (_out.rgb << 8); //little endian req
    };

    inline void pixelConverter(const BITMAP_PIXEL_ARGB_8BIT &_in, BITMAP_PIXEL_ARGB_8BIT &_out )
    {
        _out.argb = _in.argb;
    };

    inline void pixelConverter(const BITMAP_PIXEL_ARGB_8BIT &_in, BITMAP_PIXEL_BGRA_8BIT &_out )
    {
        _out.bgra = ((_in.argb & 0x000000FF) << 24) |
                    ((_in.argb & 0x0000FF00) << 8 ) |
                    ((_in.argb & 0x00FF0000) >> 8 ) |
                    ((_in.argb & 0xFF000000) >> 24);
    };



    // BITMAP_PIXEL_BGRA_8BIT

    inline void pixelConverter(const BITMAP_PIXEL_BGRA_8BIT &_in, PIXEL_RGBA_8BIT &_out )
    {
		_out.r = _in.r;
		_out.g = _in.g;
		_out.b = _in.b;
		_out.a = _in.a;
    };

    inline void pixelConverter(const BITMAP_PIXEL_BGRA_8BIT &_in, PIXEL_RGB_8BIT &_out )
    {
		_out.r = _in.r;
		_out.g = _in.g;
		_out.b = _in.b;
    };

    inline void pixelConverter(const BITMAP_PIXEL_BGRA_8BIT &_in, PIXEL_ABGR_8BIT &_out )
    {
		_out.a = _in.a;
		_out.b = _in.b;
		_out.g = _in.g;
		_out.r = _in.r;
    };

    inline void pixelConverter(const BITMAP_PIXEL_BGRA_8BIT &_in, PIXEL_BGR_8BIT &_out )
    {
		_out.b = _in.b;
		_out.g = _in.g;
		_out.r = _in.r;
    };

    inline void pixelConverter(const BITMAP_PIXEL_BGRA_8BIT &_in, PIXEL_RGBA_4BIT &_out )
    {
        _out.rgba = (((unsigned short)(_in.r & 0xF0)) << 8) |
					(((unsigned short)(_in.g & 0xF0)) << 4) |
					(((unsigned short)(_in.b & 0xF0))     ) |
					(((unsigned short)(_in.a & 0xF0)) >> 4);

		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const BITMAP_PIXEL_BGRA_8BIT &_in, PIXEL_RGBA_5551BIT &_out )
    {
        _out.rgba = (((unsigned short)(_in.r & 0xF8)) << 8) |
					(((unsigned short)(_in.g & 0xF8)) << 3) |
					(((unsigned short)(_in.b & 0xF8)) >> 2) |
					(((unsigned short)(_in.a & 0x80)) >> 7);

		_out.rgba = (_out.rgba >> 8) | (_out.rgba << 8); //little endian req
    };

    inline void pixelConverter(const BITMAP_PIXEL_BGRA_8BIT &_in, PIXEL_RGB_565BIT &_out )
    {
        _out.rgb =  (((unsigned short)(_in.r & 0xF8)) << 8) |
					(((unsigned short)(_in.g & 0xFC)) << 3) |
					(((unsigned short)(_in.b & 0xF8)) >> 3);

		_out.rgb = (_out.rgb >> 8) | (_out.rgb << 8); //little endian req
    };

    inline void pixelConverter(const BITMAP_PIXEL_BGRA_8BIT &_in, BITMAP_PIXEL_ARGB_8BIT &_out )
    {
        _out.argb = ((_in.bgra & 0x000000FF) << 24) |
                    ((_in.bgra & 0x0000FF00) << 8 ) |
                    ((_in.bgra & 0x00FF0000) >> 8 ) |
                    ((_in.bgra & 0xFF000000) >> 24);
    };

    inline void pixelConverter(const BITMAP_PIXEL_BGRA_8BIT &_in, BITMAP_PIXEL_BGRA_8BIT &_out )
    {
        _out.bgra = _in.bgra;

    };



}

