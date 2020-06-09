#pragma once

#include <memory>

#include "SingletonTemplate.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Sound.h"


namespace core
{

	class SoundManager : public ResourceManager, public Singleton<SoundManager>
	{
		SoundSystem* soundSystem;

	public:
		SoundManager(SoundSystem* _soundSystem);

		virtual Resource* createImpl(const std::string &_name, ResourceHandle _handle, const std::string &_group, ScriptNodePtr _scriptNode);
		virtual void removeImpl(ResourcePtr _resource);

		SoundPtr getByName(const std::string& _name, const std::string& _group = DEFAULT_RESOURCE_GROUP);
		SoundPtr getByHandle(ResourceHandle _handle);

		void parseResourceScript(DataStreamPtr _script);

	};
}
