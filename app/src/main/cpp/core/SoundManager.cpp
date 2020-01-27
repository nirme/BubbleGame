#include "SoundManager.h"
#include "SoundSystem.h"


template<>
core::SoundManager* Singleton<core::SoundManager>::impl = nullptr;

namespace core
{
	SoundManager::SoundManager(SoundSystem* _soundSystem) :
		ResourceManager(),
		soundSystem(_soundSystem)
	{};


	Resource* SoundManager::createImpl(const std::string &_name, ResourceHandle _handle, const std::string &_group, ScriptNodePtr _scriptNode)
	{
		Sound *sound = new Sound(_name, _handle, _group, soundSystem, this);
		if (_scriptNode)
		{
		}

		return sound;
	};

	void SoundManager::removeImpl(ResourcePtr _resource)
	{
		_resource->unload();
	};


	SoundPtr SoundManager::getByName(const std::string& _name, const std::string& _group)
	{
		return std::static_pointer_cast<Sound>(getResourceByName(_name, _group));
	};


	SoundPtr SoundManager::getByHandle(ResourceHandle _handle)
	{
		return std::static_pointer_cast<Sound>(getResourceByHandle(_handle));
	};


	void SoundManager::parseResourceScript(DataStreamPtr _script)
	{
		ScriptLoader &loader = ScriptLoader::getSingleton();
		ScriptNodeListPtr nodeList = loader.parse(_script);

		std::string name, group;

		for (auto it = nodeList->begin(); it != nodeList->end(); ++it)
		{
			name = loader.parseResourceName(*it);
			group = loader.parseResourceGroup(*it);

			SoundPtr sound = std::static_pointer_cast<Sound>(createResource(name, group, *it));
		}
	};

}
