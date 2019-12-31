#include "AndroidDataProvider.h"


namespace core
{

	AndroidDataProvider::AndroidDataProvider(AAssetManager *_assetManager, const std::string &_directory) :
		assetManager(_assetManager),
		directory(_directory)
	{};


	DataStreamPtr AndroidDataProvider::getDataStream(const std::string &_path)
	{
		assert(assetManager && "no asset manager set");

		std::string fullPath = directory.length() ? (directory + _path) : _path;

		AAsset *asset = AAssetManager_open(assetManager, fullPath.c_str(), AASSET_MODE_BUFFER);

		if (!asset)
		{
			std::string message = "Cannot find asset file '" + fullPath + "'";
			Logger::getSingleton().write(message, LL_CRITICAL);
		}

		return DataStreamPtr(new AndroidDataStream(asset));
	};

	void AndroidDataProvider::setDirectoryPath(const std::string &_dir)
	{
	    std::string tmp = _dir;

		//remove first / if any
		std::size_t pos = tmp.find_first_not_of('/');
		if (pos && pos != std::string::npos)
            tmp = tmp.substr(pos);
		else if (pos == std::string::npos)
			tmp.clear();

        //remove any double //
		while ((pos = tmp.find("//")) != std::string::npos)
            tmp.replace(pos,2,"/");

		//add / at end if not already there
		if (tmp.length() > 1 && tmp.back() != '/')
		    tmp.push_back('/');

        directory = tmp;
	};

	DataProvider* AndroidDataProvider::clone()
	{
		return new AndroidDataProvider(assetManager, directory);
	};
}
