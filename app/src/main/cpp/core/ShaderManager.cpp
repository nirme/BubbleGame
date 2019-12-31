#include "ShaderManager.h"
#include "RenderSystem.h"
#include "ScriptLoader.h"

template<>
core::ShaderManager* Singleton<core::ShaderManager>::impl = nullptr;

namespace core
{
	ShaderManager::ShaderManager(RenderSystem* _renderer) :
		ResourceManager(),
		renderer(_renderer)
	{};


	Resource* ShaderManager::createImpl(const std::string &_name, ResourceHandle _handle, const std::string &_group, ScriptNodePtr _scriptNode)
	{
		Shader *shader = new Shader(_name, _handle, _group, renderer, this);

		if (_scriptNode)
		{
			shader->setType(ScriptLoader::getSingleton().parseShaderType(_scriptNode));
		}

		return shader;
	};

	void ShaderManager::removeImpl(ResourcePtr _resource)
	{
		_resource->unload();
	};


	ShaderPtr ShaderManager::getByName(const std::string& _name, const std::string& _group)
	{
		return std::static_pointer_cast<Shader>(getResourceByName(_name, _group));
	};

	ShaderPtr ShaderManager::getByHandle(ResourceHandle _handle)
	{
		return std::static_pointer_cast<Shader>(getResourceByHandle(_handle));
	};


	void ShaderManager::parseResourceScript(DataStreamPtr _script)
	{
		ScriptLoader &loader = ScriptLoader::getSingleton();
		ScriptNodeListPtr nodeList = loader.parse(_script);

		std::string name, group;

		for (auto it = nodeList->begin(); it != nodeList->end(); ++it)
		{
			name = loader.parseResourceName(*it);
			group = loader.parseResourceGroup(*it);

			ShaderPtr shader = std::static_pointer_cast<Shader>(createResource(name, group, *it));

			shader->setType(loader.parseShaderType(*it));
		}
	};

}
