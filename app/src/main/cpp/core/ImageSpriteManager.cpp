#include "ImageSpriteManager.h"
#include "ScriptLoader.h"


template<>
core::ImageSpriteManager* Singleton<core::ImageSpriteManager>::impl = nullptr;

namespace core
{

	Resource* ImageSpriteManager::createImpl(const std::string &_name, ResourceHandle _handle, const std::string &_group, ScriptNodePtr _scriptNode)
	{
		ImageSprite *sprite = new ImageSprite(_name, _handle, _group, this);

		return sprite;
	};


	void ImageSpriteManager::removeImpl(ResourcePtr _resource)
	{
		_resource->unload();
	};


	ImageSpritePtr ImageSpriteManager::getByName(const std::string& _name, const std::string& _group)
	{
		return std::static_pointer_cast<ImageSprite>(getResourceByName(_name, _group));
	};


	ImageSpritePtr ImageSpriteManager::getByHandle(ResourceHandle _handle)
	{
		return std::static_pointer_cast<ImageSprite>(getResourceByHandle(_handle));
	};


	void ImageSpriteManager::parseResourceScript(DataStreamPtr _script)
	{
		ScriptLoader &loader = ScriptLoader::getSingleton();
		ScriptNodeListPtr nodeList = loader.parse(_script);

		std::string name, group;

		for (auto it = nodeList->begin(); it != nodeList->end(); ++it)
		{
			name = loader.parseResourceName(*it);
			group = loader.parseResourceGroup(*it);

			ImageSpritePtr sprite = std::static_pointer_cast<ImageSprite>(createResource(name, group, *it));
		}
	};

}
