#pragma once

#include <memory>

#include "SingletonTemplate.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "ImageSprite.h"


namespace core
{

	class ImageSpriteManager : public ResourceManager, public Singleton<ImageSpriteManager>
	{

	public:

		virtual Resource* createImpl(const std::string &_name, ResourceHandle _handle, const std::string &_group, ScriptNodePtr _scriptNode);
		virtual void removeImpl(ResourcePtr _resource);

		ImageSpritePtr getByName(const std::string& _name, const std::string& _group = DEFAULT_RESOURCE_GROUP);
		ImageSpritePtr getByHandle(ResourceHandle _handle);

		void parseResourceScript(DataStreamPtr _script);
	};
}
