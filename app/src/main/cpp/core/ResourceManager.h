#pragma once

#include <unordered_map>
#include <map>
#include <deque>
#include <string>
#include <mutex>


#include "Resource.h"
#include "DataProvider.h"

#include "ScriptNode.h"


namespace core
{
	constexpr auto DEFAULT_RESOURCE_GROUP = "";

	class ResourceManager
	{
	protected:


		typedef std::unordered_map<std::string, ResourcePtr> ResourceNameMap;
		typedef std::unordered_map<std::string, ResourceNameMap> ResourceGroupMap;

		// change it to vector maybe?
		typedef std::map<ResourceHandle, ResourcePtr> ResourceHandleMap;

		typedef std::deque<ResourcePtr> LoadingResourceQueue;


		DataProviderPtr dataProvider;

		ResourceNameMap globalResourceMap;
		ResourceGroupMap groupedResourceMaps;

		ResourceHandleMap resourceHandleMap;

		std::mutex resourceQueueMutex;
		LoadingResourceQueue loadingResourceQueue;

		ResourceHandle freeHandle;



	public:

		ResourceManager(){};
		virtual ~ResourceManager(){};

		ResourcePtr createResource(const std::string &_name, const std::string &_group, ScriptNodePtr _scriptNode);

		void removeResource(ResourcePtr _resource);
		void removeResource(const std::string &_name, const std::string &_group = DEFAULT_RESOURCE_GROUP);
		void removeResource(ResourceHandle _handle);


		virtual Resource* createImpl(const std::string &_name, ResourceHandle _handle, const std::string &_group, ScriptNodePtr _scriptNode) = 0;
		virtual void removeImpl(ResourcePtr _resource);


		ResourcePtr getResourceByName(const std::string& _name, const std::string& _group = DEFAULT_RESOURCE_GROUP);
		ResourcePtr getResourceByHandle(ResourceHandle _handle);


		void loadAll(bool _inBackground = false);
		void loadGroup(const std::string& _group, bool _inBackground = false);

		void unloadAll();
		void unloadGroup(const std::string& _group);

		DataStreamPtr openResource(const std::string &_resourceName);

		ResourcePtr loadQueuedResource();
		void loadAllQueuedResources();

		void registerDataProvider(DataProviderPtr _dataProvider);
		void updateResourcePath(const std::string& _dir);

		virtual void parseResourceScript(DataStreamPtr _script) = 0;


	private:

		inline ResourceHandle getFreeHandle()
		{
			return freeHandle++;
		};

		inline bool isDefaultGroup(const std::string& _group)
		{
			return _group.length() == 0;
		};

		ResourceNameMap& getResourceGroup(const std::string& _group);
		void addResource(ResourcePtr _resource);


	};


}