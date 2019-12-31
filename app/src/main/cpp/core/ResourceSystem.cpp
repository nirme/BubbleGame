#include "ResourceSystem.h"


template<>
core::ResourceSystem* Singleton<core::ResourceSystem>::impl = nullptr;


namespace core
{

	void ResourceSystem::registerResourceManager(const std::string& _resourceType, ResourceManager* _manager)
	{
		Logger::getSingleton().write("Registering resource manager of type " + _resourceType, LL_INFO);
		resourceManagers.emplace(_resourceType, _manager);
	};


	ResourceManager *ResourceSystem::releaseResourceManager(const std::string& _resourceType)
	{
		auto it = resourceManagers.find(_resourceType);
		if (it != resourceManagers.end())
		{
			ResourceManager *out = (*it).second.release();
			resourceManagers.erase(it);
			return out;
		}

		return nullptr;
	};


	void ResourceSystem::releaseAllResourceSystems()
	{
		unloadAllResources();
		resourceManagers.clear();
	};


	void ResourceSystem::registerDataProvider(DataProviderPtr _dataProvider)
	{
		dataProvider = _dataProvider;
	};


	void ResourceSystem::parseConfiguration(const std::string& _name)
	{
		assert(dataProvider && "data provider not set");

		DataStreamPtr dataStream = dataProvider->getDataStream(_name);
		assert(dataStream && "configuration file not found");

		ScriptNodeListPtr configList = ScriptLoader::getSingleton().parse(dataStream);

		std::string name, path, file;
		for (auto it = configList->begin(); it != configList->end(); ++it)
		{
			name = (*it)->getValue("name");
			path = (*it)->getValue("resource_path");
			file = (*it)->getValue("script_file");

			auto itMan = resourceManagers.find(name);
			if (itMan != resourceManagers.end())
			{
				ResourceManager *manager = (*itMan).second.get();

				DataProviderPtr newDataProvider(dataProvider->clone());
				newDataProvider->setDirectoryPath(path);
				manager->registerDataProvider(newDataProvider);

				//  add parsing the base res script and forward nodes to manager
				DataStreamPtr resourceScriptStream = dataProvider->getDataStream(file);
				manager->parseResourceScript(resourceScriptStream);
			}
			else
			{
				std::string message = "resource manager by name '" + name + "' not registered";
				Logger::getSingleton().write(message, LL_CRITICAL);
			}
		}
	};


	void ResourceSystem::loadAllResources()
	{
		for (auto it = resourceManagers.begin(), itEnd = resourceManagers.end(); it != itEnd; ++it)
			(*it).second->loadAll();
	};


	void ResourceSystem::loadGroupResources(const std::string& _group)
	{
		for (auto it = resourceManagers.begin(), itEnd = resourceManagers.end(); it != itEnd; ++it)
			(*it).second->loadGroup(_group);
	};


	void ResourceSystem::unloadAllResources()
	{
		for (auto it = resourceManagers.begin(), itEnd = resourceManagers.end(); it != itEnd; ++it)
			(*it).second->unloadAll();
	};


	void ResourceSystem::unloadGroupResources(const std::string& _group)
	{
		for (auto it = resourceManagers.begin(), itEnd = resourceManagers.end(); it != itEnd; ++it)
			(*it).second->unloadGroup(_group);
	};

}
