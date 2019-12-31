#include "Image.h"

namespace core
{

	Image::Image() :
		width(0), 
		height(0), 
		format(IF_UNDEFINED), 
		pixelFormat(PF_UNDEFINED), 
		pixelStructure(PS_UNDEFINED), 
		data(0)
	{};


	IMAGE_FORMAT Image::getFormat(const std::vector<unsigned char> _header)
	{
		// bmp files start with "BM" header
		if (_header.size() >= 2 && _header[0] == 0x42 && _header[1] == 0x4D)
			return IF_BMP;

		// jpg files start with 0xFF 0xD8 header
		if (_header.size() >= 2 && _header[0] == 0xFF && _header[1] == 0xD8)
			return IF_JPG;

		// png files start with 0x89 "PNG" header
		if (_header.size() >= 4 && _header[0] == 0x89 && _header[1] == 0x50 && _header[2] == 0x4E && _header[3] == 0x47)
			return IF_PNG;

		return IF_UNDEFINED;
	};


	void Image::convertBMPToRAW(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure, bool _roundSizeToPow2)
	{
		assert(format == IF_BMP && "format conversion require BMP input");

		// make sure short is 2b and int is 4b
		assert(sizeof(unsigned short) == 2 && "unsigned short size not equal to 2");
		assert(sizeof(unsigned int) == 4 && "unsigned int size not equal to 4");


		// get file header
		BMPHEADERS fileHeader;
		memcpy(&fileHeader, &data[0], sizeof(BMPHEADERS));

		if (ENDIANNESS::IsBigEndian())
		{
			//fileHeader.signature = ENDIANNESS::convert(fileHeader.signature); // don't care
			//fileHeader.fileSize = ENDIANNESS::convert(fileHeader.fileSize); // don't care
			//fileHeader.reserved1 = ENDIANNESS::convert(fileHeader.reserved1); // don't care
			//fileHeader.reserved2 = ENDIANNESS::convert(fileHeader.reserved2); // don't care
			fileHeader.offsetToPixelArray = ENDIANNESS::convert(fileHeader.offsetToPixelArray);
			fileHeader.dibHeaderSize = ENDIANNESS::convert(fileHeader.dibHeaderSize);
			fileHeader.imageWidth = ENDIANNESS::convert(fileHeader.imageWidth);
			fileHeader.imageHeight = ENDIANNESS::convert(fileHeader.imageHeight);
			//fileHeader.planes = ENDIANNESS::convert(fileHeader.planes); // don't care
			fileHeader.bitsPerPixel = ENDIANNESS::convert(fileHeader.bitsPerPixel);
			//fileHeader.compression = ENDIANNESS::convert(fileHeader.compression); // don't care
			//fileHeader.imageSize = ENDIANNESS::convert(fileHeader.imageSize); // don't care
		}

		originalWidth = fileHeader.imageWidth;
		originalHeight = fileHeader.imageHeight;


		// count new size
        unsigned int updatedWidth = fileHeader.imageWidth;
		unsigned int updatedHeight = fileHeader.imageHeight;

		if (_roundSizeToPow2)
		{
			updatedWidth = PO2(updatedWidth);
			updatedHeight = PO2(updatedHeight);
		}

		// find out new pix format
		if ((_newPixelStructure == PS_UNDEFINED ||_newPixelStructure == PS_BYTE_PER_COMPONENT) &&
			_newPixelFormat == PF_UNDEFINED)
		{
			if (fileHeader.bitsPerPixel == 32)
			{
				_newPixelStructure = PS_BYTE_PER_COMPONENT;
				_newPixelFormat = PF_RGBA;
			}
			else if (fileHeader.bitsPerPixel == 24)
			{
				_newPixelStructure = PS_BYTE_PER_COMPONENT;
				_newPixelFormat = PF_RGB;
			}
		}
		else if (_newPixelStructure == PS_USHORT_5_5_5_1 || _newPixelStructure == PS_USHORT_4_4_4_4)
		{
			_newPixelFormat = PF_RGBA;
		}
		else if (_newPixelStructure == PS_USHORT_5_6_5)
		{
			_newPixelFormat = PF_RGB;
		}


		assert(_newPixelFormat != PF_UNDEFINED && "unable to calculate new pixel format");
        assert(_newPixelStructure != PS_UNDEFINED && "unable to calculate new pixel structure");


		IMAGE_FORMAT updatedFormat = IF_RAW;
		PIXEL_FORMAT updatedPixelFormat = _newPixelFormat;
		PIXEL_STRUCTURE updatedPixelStructure = _newPixelStructure;

		// get line padding and data pointer
        unsigned short pixelArrayPadding = (4 - ((fileHeader.imageWidth * (unsigned short)(fileHeader.bitsPerPixel * 0.125)) % 4)) % 4;
		const unsigned char* pixelArray = &data[fileHeader.offsetToPixelArray];

		std::vector<unsigned char> newData(0);

		// if big endian system then pixels will be read in format RGBA instead of ABGR
		//if (ENDIANNESS::IsLittleEndian())
		if (ENDIANNESS::IsBigEndian())
		{
			//  from ARGB_8_8_8_8
			if (fileHeader.bitsPerPixel == 32)
			{
				//  to RGBA_8_8_8_8
				if (_newPixelStructure == PS_BYTE_PER_COMPONENT && _newPixelFormat == PF_RGBA)
					newData = convertBMPToRAW<BITMAP_PIXEL_ARGB_8BIT, PIXEL_RGBA_8BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

				//  to RGB_8_8_8
				else if (_newPixelStructure == PS_BYTE_PER_COMPONENT && _newPixelFormat == PF_RGB)
					newData = convertBMPToRAW<BITMAP_PIXEL_ARGB_8BIT, PIXEL_RGB_8BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

				//  to RGBA_4_4_4_4
				else if (_newPixelStructure == PS_USHORT_4_4_4_4)
					newData = convertBMPToRAW<BITMAP_PIXEL_ARGB_8BIT, PIXEL_RGBA_4BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

				//  to RGBA_5_5_5_1
				else if (_newPixelStructure == PS_USHORT_5_5_5_1)
					newData = convertBMPToRAW<BITMAP_PIXEL_ARGB_8BIT, PIXEL_RGBA_5551BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

				//  to RGB_5_6_5
				else if (_newPixelStructure == PS_USHORT_5_6_5)
					newData = convertBMPToRAW<BITMAP_PIXEL_ARGB_8BIT, PIXEL_RGB_565BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);
			}

			//  from RGB_8_8_8
			else if (fileHeader.bitsPerPixel == 24)
			{
				//  to RGBA_8_8_8_8
				if (_newPixelStructure == PS_BYTE_PER_COMPONENT && _newPixelFormat == PF_RGBA)
					newData = convertBMPToRAW<PIXEL_RGB_8BIT, PIXEL_RGBA_8BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

				//  to RGB_8_8_8
				else if (_newPixelStructure == PS_BYTE_PER_COMPONENT && _newPixelFormat == PF_RGB)
					newData = convertBMPToRAW<PIXEL_RGB_8BIT, PIXEL_RGB_8BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

				//  to RGBA_4_4_4_4
				else if (_newPixelStructure == PS_USHORT_4_4_4_4)
					newData = convertBMPToRAW<PIXEL_RGB_8BIT, PIXEL_RGBA_4BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

				//  to RGBA_5_5_5_1
				else if (_newPixelStructure == PS_USHORT_5_5_5_1)
					newData = convertBMPToRAW<PIXEL_RGB_8BIT, PIXEL_RGBA_5551BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

				//  to RGB_5_6_5
				else if (_newPixelStructure == PS_USHORT_5_6_5)
					newData = convertBMPToRAW<PIXEL_RGB_8BIT, PIXEL_RGB_565BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);
			}
		}

		else // if (ENDIANNESS::IsLittleEndian())
		{
			//  from BGRA_8_8_8_8
			if (fileHeader.bitsPerPixel == 32)
			{
				//  to RGBA_8_8_8_8
				if (_newPixelStructure == PS_BYTE_PER_COMPONENT && _newPixelFormat == PF_RGBA)
					newData = convertBMPToRAW<BITMAP_PIXEL_BGRA_8BIT, PIXEL_RGBA_8BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

					//  to RGB_8_8_8
				else if (_newPixelStructure == PS_BYTE_PER_COMPONENT && _newPixelFormat == PF_RGB)
					newData = convertBMPToRAW<BITMAP_PIXEL_BGRA_8BIT, PIXEL_RGB_8BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

					//  to RGBA_4_4_4_4
				else if (_newPixelStructure == PS_USHORT_4_4_4_4)
					newData = convertBMPToRAW<BITMAP_PIXEL_BGRA_8BIT, PIXEL_RGBA_4BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

					//  to RGBA_5_5_5_1
				else if (_newPixelStructure == PS_USHORT_5_5_5_1)
					newData = convertBMPToRAW<BITMAP_PIXEL_BGRA_8BIT, PIXEL_RGBA_5551BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

					//  to RGB_5_6_5
				else if (_newPixelStructure == PS_USHORT_5_6_5)
					newData = convertBMPToRAW<BITMAP_PIXEL_BGRA_8BIT, PIXEL_RGB_565BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);
			}

				//  from BGR_8_8_8
			else if (fileHeader.bitsPerPixel == 24)
			{
				//  to RGBA_8_8_8_8
				if (_newPixelStructure == PS_BYTE_PER_COMPONENT && _newPixelFormat == PF_RGBA)
					newData = convertBMPToRAW<PIXEL_BGR_8BIT, PIXEL_RGBA_8BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

					//  to RGB_8_8_8
				else if (_newPixelStructure == PS_BYTE_PER_COMPONENT && _newPixelFormat == PF_RGB)
					newData = convertBMPToRAW<PIXEL_BGR_8BIT, PIXEL_RGB_8BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

					//  to RGBA_4_4_4_4
				else if (_newPixelStructure == PS_USHORT_4_4_4_4)
					newData = convertBMPToRAW<PIXEL_BGR_8BIT, PIXEL_RGBA_4BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

					//  to RGBA_5_5_5_1
				else if (_newPixelStructure == PS_USHORT_5_5_5_1)
					newData = convertBMPToRAW<PIXEL_BGR_8BIT, PIXEL_RGBA_5551BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);

					//  to RGB_5_6_5
				else if (_newPixelStructure == PS_USHORT_5_6_5)
					newData = convertBMPToRAW<PIXEL_BGR_8BIT, PIXEL_RGB_565BIT>(updatedWidth, updatedHeight, pixelArray, fileHeader.imageWidth, fileHeader.imageHeight, pixelArrayPadding);
			}
		}

		// translation from color tables required!!!
		//  from RGB_8, BGR_8, RGB_4, BGR_4, monochrome


		// conversion completed, update image state

		width = updatedWidth;
		height = updatedHeight;

		format = updatedFormat;
		pixelFormat = updatedPixelFormat;
		pixelStructure = updatedPixelStructure;

		newData.swap(data);
	};

	void Image::convertRAWToBMP(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure)
	{
		throw std::logic_error("unimplemented");
	};


	void Image::convertJPGToRAW(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure, bool _roundSizeToPow2)
	{
		throw std::logic_error("unimplemented");
	};

	void Image::convertRAWToJPG(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure)
	{
		throw std::logic_error("unimplemented");
	};


	void Image::convertPNGToRAW(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure, bool _roundSizeToPow2)
	{
		throw std::logic_error("unimplemented");
	};

	void Image::convertRAWToPNG(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure)
	{
		throw std::logic_error("unimplemented");
	};



	void Image::read(DataStreamPtr _stream)
	{
		//make sure to reset position
		_stream->seekPosition(0);

		data_size size = _stream->getSize();
		data.resize(size);
		data.shrink_to_fit();

		_stream->readData(data.data());

		width = 0;
		height = 0;

		format = getFormat(data);
		pixelFormat = PF_UNDEFINED;
		pixelStructure = PS_UNDEFINED;
	};


	void Image::write(DataStreamPtr _stream)
	{
		throw std::logic_error("unimplemented");
	};



	void Image::convert(IMAGE_FORMAT _newFormat, PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure)
	{
		assert(format != IF_UNDEFINED && "cannot convert uninitialised image");

		// convert to raw from other formats
		if (_newFormat == IF_RAW)
		{
			switch (format)
			{
			case IF_BMP:
				convertBMPToRAW(_newPixelFormat, _newPixelStructure, true);
				break;

			case IF_JPG:
				convertJPGToRAW(_newPixelFormat, _newPixelStructure, true);
				break;

			case IF_PNG:
				convertPNGToRAW(_newPixelFormat, _newPixelStructure, true);
				break;
			}

			return;
		}


		// if converting from one format to other change first to raw without changing color depth
		if (format != IF_RAW)
		{
			switch (format)
			{
			case IF_BMP:
				convertBMPToRAW(PF_UNDEFINED, PS_UNDEFINED, false);
				break;

			case IF_JPG:
				convertJPGToRAW(PF_UNDEFINED, PS_UNDEFINED, false);
				break;

			case IF_PNG:
				convertPNGToRAW(PF_UNDEFINED, PS_UNDEFINED, false);
				break;
			}
		}


		// convert to requested format
		switch (_newFormat)
		{
		case IF_BMP:
			convertRAWToBMP(_newPixelFormat, _newPixelStructure);
			break;

		case IF_JPG:
			convertRAWToJPG(_newPixelFormat, _newPixelStructure);
			break;

		case IF_PNG:
			convertRAWToPNG(_newPixelFormat, _newPixelStructure);
			break;
		}

		return;
	};



}
