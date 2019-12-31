#include "ShadingProgramManager.h"
#include "RenderSystem.h"
#include "ScriptLoader.h"


template<>
core::ShadingProgramManager* Singleton<core::ShadingProgramManager>::impl = nullptr;

namespace core
{
	ShadingProgramManager::ShadingProgramManager(RenderSystem* _renderer) :
		ResourceManager(),
		renderer(_renderer)
	{};


	Resource* ShadingProgramManager::createImpl(const std::string &_name, ResourceHandle _handle, const std::string &_group, ScriptNodePtr _scriptNode)
	{
		ShadingProgram *program = new ShadingProgram(_name, _handle, _group, renderer, this);
		return program;
	};

	void ShadingProgramManager::removeImpl(ResourcePtr _resource)
	{
		_resource->unload();
	};


	ShadingProgramPtr ShadingProgramManager::getByName(const std::string& _name, const std::string& _group)
	{
		return std::static_pointer_cast<ShadingProgram>(getResourceByName(_name, _group));
	};

	ShadingProgramPtr ShadingProgramManager::getByHandle(ResourceHandle _handle)
	{
		return std::static_pointer_cast<ShadingProgram>(getResourceByHandle(_handle));
	};


	void ShadingProgramManager::parseResourceScript(DataStreamPtr _script)
	{
		ScriptLoader &loader = ScriptLoader::getSingleton();
		ScriptNodeListPtr nodeList = loader.parse(_script);

		std::string name, group;

		for (auto it = nodeList->begin(); it != nodeList->end(); ++it)
		{
			name = loader.parseResourceName(*it);
			group = loader.parseResourceGroup(*it);

			ShadingProgramPtr program = std::static_pointer_cast<ShadingProgram>(createResource(name, group, *it));
		}
	};

}
