#pragma once

#include <string>
#include <android/asset_manager.h>

#include "DataProvider.h"
#include "AndroidDataStream.h"
#include "Logger.h"

namespace core
{

	class AndroidDataProvider : public DataProvider
	{
	private:
		AAssetManager *assetManager;
		std::string directory;

	public:

		AndroidDataProvider(AAssetManager *_assetManager = nullptr, const std::string &_directory = "");

		virtual DataStreamPtr getDataStream(const std::string &_path);
		virtual void setDirectoryPath(const std::string &_dir);
		virtual DataProvider* clone();

	};
}