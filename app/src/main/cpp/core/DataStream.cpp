#include "DataStream.h"



namespace core
{
	
	data_size DataStream::skip(data_size _length)
	{
		return seekPosition(getPosition() + _length);
	};

	data_size DataStream::getRemainingSize()
	{
		return getSize() - getPosition();
	};

}