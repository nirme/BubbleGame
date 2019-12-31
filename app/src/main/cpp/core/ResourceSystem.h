#pragma once

#include <memory>
#include <unordered_map>

#include "SingletonTemplate.h"
#include "ResourceManager.h"
#include "Logger.h"

#include "ScriptLoader.h"



namespace core
{
	class ResourceSystem : public Singleton<ResourceSystem>
	{
	private:

		typedef std::unique_ptr<ResourceManager> ResourceManagerUPtr;

		typedef std::unordered_map<std::string, ResourceManagerUPtr> ResourceManagerMap;

		// after registering res system is responsible for res managers
		ResourceManagerMap resourceManagers;

		DataProviderPtr dataProvider;


	public:

		void registerResourceManager(const std::string& _resourceType, ResourceManager* _manager);
		ResourceManager *releaseResourceManager(const std::string& _resourceType);
		void releaseAllResourceSystems();
		void registerDataProvider(DataProviderPtr _dataProvider);

		void parseConfiguration(const std::string& _name);

		void loadAllResources();
		void loadGroupResources(const std::string& _group = DEFAULT_RESOURCE_GROUP);

		void unloadAllResources();
		void unloadGroupResources(const std::string& _group = DEFAULT_RESOURCE_GROUP);

	};

}
