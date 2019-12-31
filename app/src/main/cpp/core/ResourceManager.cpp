#include "ResourceManager.h"


namespace core
{

	ResourcePtr ResourceManager::createResource(const std::string &_name, const std::string &_group, ScriptNodePtr _scriptNode)
	{
		ResourcePtr res = ResourcePtr(createImpl(_name, getFreeHandle(), _group.length() == 0 ? DEFAULT_RESOURCE_GROUP : _group, _scriptNode));

		// insert resource
		getResourceGroup(res->getGroup()).insert(ResourceNameMap::value_type(res->getName(), res));
		resourceHandleMap.insert(ResourceHandleMap::value_type(res->getHandle(), res));

		return res;
	};


	void ResourceManager::removeResource(ResourcePtr _resource)
	{
		removeImpl(_resource);
	};


	void ResourceManager::removeResource(const std::string &_name, const std::string &_group)
	{
		removeResource(getResourceByName(_name, _group));
	};


	void ResourceManager::removeResource(ResourceHandle _handle)
	{
		removeResource(getResourceByHandle(_handle));
	};


	void ResourceManager::removeImpl(ResourcePtr _resource)
	{
		assert(_resource && "Cannot remove empty resource pointer");

		resourceHandleMap.erase(_resource->getHandle());
		getResourceGroup(_resource->getGroup()).erase(_resource->getName());
	};



	ResourcePtr ResourceManager::getResourceByName(const std::string& _name, const std::string& _group)
	{
		if (isDefaultGroup(_group))
		{
			auto it = globalResourceMap.find(_name);
			if (it != globalResourceMap.end())
			{
				return it->second;
			}
		}
		else
		{
			auto resourceMapIt = groupedResourceMaps.find(_group);
			if (resourceMapIt != groupedResourceMaps.end())
			{
				auto it = resourceMapIt->second.find(_name);
				if (it != resourceMapIt->second.end())
				{
					return it->second;
				}
			}
		}

		//resource not found in specified group or group not found, look for it in all groups
		// check in global
		{
			auto it = globalResourceMap.find(_name);
			if (it != globalResourceMap.end())
			{
				return it->second;
			}
		}

		//check in all grouped lists
		for (auto resourceMapIt = groupedResourceMaps.begin(); resourceMapIt != groupedResourceMaps.end(); ++resourceMapIt)
		{
			auto it = resourceMapIt->second.find(_name);
			if (it != resourceMapIt->second.end())
			{
				return it->second;
			}
		}

		return ResourcePtr();
	};


	ResourcePtr ResourceManager::getResourceByHandle(ResourceHandle _handle)
	{
		auto it = resourceHandleMap.find(_handle);
		return it != resourceHandleMap.end() ? it->second : ResourcePtr();
	};


	void ResourceManager::loadAll(bool _inBackground)
	{
		if (!_inBackground)
		{
			// loop all through handle map
			for (ResourceHandleMap::iterator it = resourceHandleMap.begin(); it != resourceHandleMap.end(); ++it)
				(*it).second->load();
		}
		else
		{
			std::lock_guard<std::mutex> lockQueue(resourceQueueMutex);

			for (ResourceHandleMap::iterator it = resourceHandleMap.begin(); it != resourceHandleMap.end(); ++it)
			{
				if (!(*it).second->isLoaded())
					loadingResourceQueue.push_back((*it).second);
			}
		}
	};


	void ResourceManager::loadGroup(const std::string& _group, bool _inBackground)
	{
		if (!_inBackground)
		{
			if (isDefaultGroup(_group))
			{
				for (ResourceNameMap::iterator it = globalResourceMap.begin(); it != globalResourceMap.end(); ++it)
					(*it).second->load();
			}
			else
			{
				auto itgroupIt = groupedResourceMaps.find(_group);
				if (itgroupIt != groupedResourceMaps.end())
					for (ResourceNameMap::iterator it = (*itgroupIt).second.begin(); it != (*itgroupIt).second.end(); ++it)
						(*it).second->load();
			}
		}
		else
		{
			if (isDefaultGroup(_group))
			{
				std::lock_guard<std::mutex> lockQueue(resourceQueueMutex);

				for (ResourceNameMap::iterator it = globalResourceMap.begin(); it != globalResourceMap.end(); ++it)
					loadingResourceQueue.push_back((*it).second);
			}
			else
			{
				auto itgroupIt = groupedResourceMaps.find(_group);
				if (itgroupIt != groupedResourceMaps.end())
				{
					std::lock_guard<std::mutex> lockQueue(resourceQueueMutex);

					for (ResourceNameMap::iterator it = (*itgroupIt).second.begin(); it != (*itgroupIt).second.end(); ++it)
						loadingResourceQueue.push_back((*it).second);
				}
			}

		}
	};


	void ResourceManager::unloadAll()
	{
		{
			std::lock_guard<std::mutex> lockQueue(resourceQueueMutex);
			loadingResourceQueue = LoadingResourceQueue();
		}

		// loop all through handle map
		for (ResourceHandleMap::iterator it = resourceHandleMap.begin(); it != resourceHandleMap.end(); ++it)
			(*it).second->unload();
	};


	void ResourceManager::unloadGroup(const std::string& _group)
	{
		ResourceNameMap& resourceMap = getResourceGroup(_group);

		if (!resourceMap.size())
			return;

		//  clear the loading queue in case some resources are still waiting
		{
			std::lock_guard<std::mutex> lockQueue(resourceQueueMutex);

			for (LoadingResourceQueue::iterator it = loadingResourceQueue.begin(); it != loadingResourceQueue.end();)
			{
				if (!_group.compare((*it)->getGroup()))
					it = loadingResourceQueue.erase(it);
				else
					++it;
			}
		}


		if (isDefaultGroup(_group))
		{
			for (ResourceNameMap::iterator it = globalResourceMap.begin(); it != globalResourceMap.end(); ++it)
				(*it).second->unload();
		}
		else
		{
			auto itgroupIt = groupedResourceMaps.find(_group);
			if (itgroupIt != groupedResourceMaps.end())
				for (ResourceNameMap::iterator it = (*itgroupIt).second.begin(); it != (*itgroupIt).second.end(); ++it)
					(*it).second->unload();
		}
	};


	DataStreamPtr ResourceManager::openResource(const std::string &_resourceName)
	{
		return dataProvider->getDataStream(_resourceName);
	};


	ResourcePtr ResourceManager::loadQueuedResource()
	{
		ResourcePtr resource(nullptr);

		std::lock_guard<std::mutex> lockQueue(resourceQueueMutex);

		if (!loadingResourceQueue.size())
			return resource;

		resource = loadingResourceQueue.front();
		resource->load();

		loadingResourceQueue.pop_front();

		return resource;
	};


	void ResourceManager::loadAllQueuedResources()
	{
		while (loadQueuedResource());
	};


	void ResourceManager::registerDataProvider(DataProviderPtr _dataProvider)
	{
		dataProvider.swap(_dataProvider);
	};

	void ResourceManager::updateResourcePath(const std::string& _dir)
	{
		dataProvider->setDirectoryPath(_dir);
	};



	ResourceManager::ResourceNameMap& ResourceManager::getResourceGroup(const std::string& _group)
	{
		if (isDefaultGroup(_group))
			return globalResourceMap;

		auto findRes = groupedResourceMaps.find(_group);
		if (findRes != groupedResourceMaps.end())
			return (*findRes).second;

		auto emplaceRes = groupedResourceMaps.emplace(_group, ResourceNameMap());
		return (*emplaceRes.first).second;
	};


	void ResourceManager::addResource(ResourcePtr _resource)
	{
		getResourceGroup(_resource->getGroup()).insert(ResourceNameMap::value_type(_resource->getName(), _resource));
		resourceHandleMap.insert(ResourceHandleMap::value_type(_resource->getHandle(), _resource));
	};





}