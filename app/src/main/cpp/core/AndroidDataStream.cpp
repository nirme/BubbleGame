#include "AndroidDataStream.h"


namespace core
{

	AndroidDataStream::AndroidDataStream(AAsset* _asset) :
		DataStream(), 
		asset(_asset)
	{};

	AndroidDataStream::~AndroidDataStream()
	{
		AAsset_close(asset);
	};


	data_size AndroidDataStream::getSize()
	{
		return (data_size)AAsset_getLength(asset);

	};


	data_size AndroidDataStream::seekPosition(data_size _pos)
	{
		off_t result = AAsset_seek(asset, (off_t)_pos, SEEK_SET);
		return (result == (off_t)-1) ? (data_size)-1 : result;
	};


	data_size AndroidDataStream::getPosition()
	{
		return AAsset_seek(asset, 0, SEEK_CUR);
	};


	data_size AndroidDataStream::readData(void * _buf, data_size _length)
	{
		assert(_buf && "read buffer cannot be nullptr");
		return AAsset_read(asset, _buf, _length ? _length : getRemainingSize());
	};

}
