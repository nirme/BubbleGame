#pragma once

#include <GLES2/gl2.h>

#include <vector>
#include <string>
#include <stdexcept>

#include "DataStream.h"
#include "Endianness.h"
#include "Pixel.h"
#include "Logger.h"

#define	PO2(val)	(((((((--val) | (val >> 16)) | (val >> 8)) | (val >> 4)) | (val >> 2)) | (val >> 1)) + 1);

#define SWAP_ENDIAN_2(v) ((0x00FF & (v >> 8)) | (0xFF00 & (v << 8)));
#define SWAP_ENDIAN_4(v) ((0x000000FF & (v >> 24)) | (0x0000FF00 & (v >> 8)) | (0x00FF0000 & (v << 8)) | (0xFF000000 & (v << 24)))



namespace core
{
	enum IMAGE_FORMAT
	{
		IF_UNDEFINED = 0x00,
		IF_RAW = 0x01,
		IF_BMP = 0x02,
		IF_JPG = 0x04,
		IF_PNG = 0x08,
	};

	enum PIXEL_FORMAT
	{
		PF_UNDEFINED = 0x0000,
		PF_RGB = 0x0001,
		PF_RGBA = 0x0002,
		//PF_ALPHA = 0x0004,
		//PF_LUMINANCE = 0x0008,
		//PF_LUMINANCE_ALPHA = 0x0010,
	};

	enum PIXEL_STRUCTURE
	{
		PS_UNDEFINED = 0x00,
		PS_BYTE_PER_COMPONENT = 0x01,
		PS_USHORT_4_4_4_4 = 0x02,
		PS_USHORT_5_5_5_1 = 0x04,
		PS_USHORT_5_6_5 = 0x08,
	};



	class Image
	{
	private:
	    // helper structs:

#pragma pack(push, 1)

        struct __attribute__ ((packed)) BMPHEADERS
        {
            // file header part
            unsigned short signature;
            unsigned int fileSize;
            unsigned short reserved1, reserved2;
            unsigned int offsetToPixelArray;

            // DIB header part
            unsigned int dibHeaderSize;
            unsigned int imageWidth, imageHeight;
            unsigned short planes, bitsPerPixel;
            unsigned int compression, imageSize;
            // ...
        };

#pragma pack(pop)





        template<typename IN, typename OUT>
        std::vector<unsigned char> convertBMPToRAW(
                unsigned int _widthOut,
                unsigned int _heightOut,
                const unsigned char *_dataIn,
                unsigned int _widthIn,
                unsigned int _heightIn,
                unsigned int _linePadding)
        {

            std::vector<unsigned char> outputData(sizeof(OUT) * _widthOut * _heightOut,0xFF);

            unsigned int inLineLength = sizeof(IN) * _widthIn + _linePadding;
            unsigned int outLineLength = sizeof(OUT) * _widthOut;

            const IN *bitmapPixPtr(nullptr);
            OUT *rawPixPtr(nullptr);

            unsigned int heightDiff = _heightOut - _heightIn;

            for (unsigned int row = 0; row < _heightIn; ++row)
            {
                bitmapPixPtr = reinterpret_cast<const IN *>(&(_dataIn[inLineLength * row]));
                rawPixPtr = reinterpret_cast<OUT *>(&(outputData[outLineLength * (row + heightDiff) ]));

                for (unsigned int col = 0; col < _widthIn; ++col)
                {
                    pixelConverter(bitmapPixPtr[col], rawPixPtr[col]);
                }
            }

            return outputData;
        };


    public:


	protected:

		unsigned int originalWidth;
		unsigned int originalHeight;
		unsigned int width;
		unsigned int height;

		IMAGE_FORMAT format;
		PIXEL_FORMAT pixelFormat;
		PIXEL_STRUCTURE pixelStructure;

		typedef std::vector<unsigned char> DataBlock;
		DataBlock data;



		static IMAGE_FORMAT getFormat(const std::vector<unsigned char> _header);

		void convertBMPToRAW(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure, bool _roundSizeToPow2 = true);
		void convertRAWToBMP(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure);

		void convertJPGToRAW(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure, bool _roundSizeToPow2 = true);
		void convertRAWToJPG(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure);

		void convertPNGToRAW(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure, bool _roundSizeToPow2 = true);
		void convertRAWToPNG(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure);


	public:

		Image();

		void clear()
		{
			originalWidth = 0;
			originalHeight = 0;

			width = 0;
			height = 0;

			format = IF_UNDEFINED;
			pixelFormat = PF_UNDEFINED;
			pixelStructure = PS_UNDEFINED;

			DataBlock empty(0);
			data.swap(empty);
		};

		inline PIXEL_FORMAT getSimplifiedPixelFormat() const
		{
			return pixelFormat;
		};

		inline unsigned int getOriginalWidth() const
		{
			return originalWidth;
		};

		inline unsigned int getOriginalHeight() const
		{
			return originalHeight;
		};


		inline unsigned int getWidth() const
		{
			return width;
		};

		inline unsigned int getHeight() const
		{
			return height;
		};

		virtual GLint getHWTexelFormat() const
		{
			switch(pixelFormat)
			{
			case PF_RGB:
				return GL_RGB;

			case PF_RGBA:
				return GL_RGBA;

			/*
			case PF_ALPHA:
				return GL_ALPHA;

			case PF_LUMINANCE:
				return GL_LUMINANCE;

			case PF_LUMINANCE_ALPHA:
				return GL_LUMINANCE_ALPHA;
			*/
			default:
				return 0;
			}

		};

		inline unsigned int getBitDepth() const
		{

			if ((pixelStructure & PS_USHORT_4_4_4_4) | (pixelStructure & PS_USHORT_5_5_5_1) | (pixelStructure & PS_USHORT_5_6_5))
				return 16;

			if ((pixelStructure & PS_BYTE_PER_COMPONENT) && (pixelFormat & PF_RGB))
				return 24;

			if ((pixelStructure & PS_BYTE_PER_COMPONENT) && (pixelFormat & PF_RGBA))
				return 32;

			return 0;
		};

		virtual GLint getHWTexelStructure() const
		{
			switch (pixelStructure)
			{
			case PS_BYTE_PER_COMPONENT:
				return GL_UNSIGNED_BYTE;

			case PS_USHORT_5_6_5:
				return GL_UNSIGNED_SHORT_5_6_5;

			case PS_USHORT_4_4_4_4:
				return GL_UNSIGNED_SHORT_4_4_4_4;

			case PS_USHORT_5_5_5_1:
				return GL_UNSIGNED_SHORT_5_5_5_1;

			default:
				return 0;
			}
		};


		inline const unsigned char* getDataPtr() const
		{
			return data.data();
		};



		void read(DataStreamPtr _stream);
		void write(DataStreamPtr _stream);

		void convert(IMAGE_FORMAT _newFormat = IF_RAW, PIXEL_FORMAT _newPixelFormat = PF_UNDEFINED, PIXEL_STRUCTURE _newPixelStructure = PS_UNDEFINED);


		unsigned int getSize()
		{
			unsigned int size = sizeof(width);
			size += sizeof(height);
			size += sizeof(format);
			size += sizeof(pixelFormat);
			size += sizeof(pixelStructure);
			size += sizeof(DataBlock) + data.size() * sizeof(DataBlock::value_type);

			return size;
		};
	};
}