#include "TextureManager.h"
#include "RenderSystem.h"


template<>
core::TextureManager* Singleton<core::TextureManager>::impl = nullptr;

namespace core
{
	TextureManager::TextureManager(RenderSystem* _renderer) :
		ResourceManager(),
		renderer(_renderer)
	{};


	Resource* TextureManager::createImpl(const std::string &_name, ResourceHandle _handle, const std::string &_group, ScriptNodePtr _scriptNode)
	{
		Texture *texture = new Texture(_name, _handle, _group, renderer, this);

		if (_scriptNode)
		{
			texture->setType(ScriptLoader::getSingleton().parseTextureType(_scriptNode));
			texture->setFilter(ScriptLoader::getSingleton().parseTextureFilter(_scriptNode));
		}

		return texture;
	};

	void TextureManager::removeImpl(ResourcePtr _resource)
	{
		_resource->unload();
	};


	TexturePtr TextureManager::getByName(const std::string& _name, const std::string& _group)
	{
		return std::static_pointer_cast<Texture>(getResourceByName(_name, _group));
	};


	TexturePtr TextureManager::getByHandle(ResourceHandle _handle)
	{
		return std::static_pointer_cast<Texture>(getResourceByHandle(_handle));
	};


	void TextureManager::parseResourceScript(DataStreamPtr _script)
	{
		ScriptLoader &loader = ScriptLoader::getSingleton();
		ScriptNodeListPtr nodeList = loader.parse(_script);

		std::string name, group;

		for (auto it = nodeList->begin(); it != nodeList->end(); ++it)
		{
			name = loader.parseResourceName(*it);
			group = loader.parseResourceGroup(*it);

			loader.parseTextureType(*it);

			TexturePtr texture = std::static_pointer_cast<Texture>(createResource(name, group, *it));

			texture->setType(loader.parseTextureType(*it));
			texture->setFilter(loader.parseTextureFilter(*it));
		}
	};

}
