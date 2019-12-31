#pragma once

#include <memory>
#include "DataStream.h"

namespace core
{

	class DataProvider
	{
	public:

		virtual DataStreamPtr getDataStream(const std::string& _path) = 0;

		virtual void setDirectoryPath(const std::string& _dir) = 0;

		virtual DataProvider* clone() = 0;

		virtual ~DataProvider(){};
	};


	typedef std::shared_ptr<DataProvider> DataProviderPtr;
}
