#pragma once

#include <memory>

#include "SingletonTemplate.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "ScriptLoader.h"


namespace core
{

	class SpritedFontManager : public ResourceManager, public Singleton<SpritedFontManager>
	{

	public:

		virtual Resource* createImpl(const std::string &_name, ResourceHandle _handle, const std::string &_group, ScriptNodePtr _scriptNode);
		virtual void removeImpl(ResourcePtr _resource);

		SpritedFontPtr getByName(const std::string& _name, const std::string& _group = DEFAULT_RESOURCE_GROUP);
		SpritedFontPtr getByHandle(ResourceHandle _handle);

		void parseResourceScript(DataStreamPtr _script);
	};
}
