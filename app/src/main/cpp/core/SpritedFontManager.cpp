#include "SpritedFontManager.h"


template<>
core::SpritedFontManager* Singleton<core::SpritedFontManager>::impl = nullptr;

namespace core
{

	Resource* SpritedFontManager::createImpl(const std::string &_name, ResourceHandle _handle, const std::string &_group, ScriptNodePtr _scriptNode)
	{
		SpritedFont *font = new SpritedFont(_name, _handle, _group, this);
		return font;
	};

	void SpritedFontManager::removeImpl(ResourcePtr _resource)
	{
		_resource->unload();
	};


	SpritedFontPtr SpritedFontManager::getByName(const std::string& _name, const std::string& _group)
	{
		return std::static_pointer_cast<SpritedFont>(getResourceByName(_name, _group));
	};

	SpritedFontPtr SpritedFontManager::getByHandle(ResourceHandle _handle)
	{
		return std::static_pointer_cast<SpritedFont>(getResourceByHandle(_handle));
	};


	void SpritedFontManager::parseResourceScript(DataStreamPtr _script)
	{
		ScriptLoader &loader = ScriptLoader::getSingleton();
		ScriptNodeListPtr nodeList = loader.parse(_script);

		std::string name, group;

		for (auto it = nodeList->begin(); it != nodeList->end(); ++it)
		{
			name = loader.parseResourceName(*it);
			group = loader.parseResourceGroup(*it);

			SpritedFontPtr font = std::static_pointer_cast<SpritedFont>(createResource(name, group, *it));
		}
	};

}
