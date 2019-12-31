#pragma once

#include <android/asset_manager.h>

#include "DataStream.h"


namespace core
{

	class AndroidDataStream : public DataStream
	{
	protected:
		AAsset* asset;


	public:

		AndroidDataStream(AAsset* _asset);
		virtual ~AndroidDataStream();

		virtual data_size getSize();

		virtual data_size seekPosition(data_size _pos);
		virtual data_size getPosition();

		virtual data_size readData(void * _buf, data_size _length = 0);

	};

}
