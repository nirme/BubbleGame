#pragma once

#include <memory>

#include "SingletonTemplate.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Shader.h"
#include "ShadingProgram.h"


namespace core
{
	class RenderSystem;


	class ShadingProgramManager : public ResourceManager, public Singleton<ShadingProgramManager>
	{
		RenderSystem* renderer;

	public:
		ShadingProgramManager(RenderSystem* _renderer);

		virtual Resource* createImpl(const std::string &_name, ResourceHandle _handle, const std::string &_group, ScriptNodePtr _scriptNode);
		virtual void removeImpl(ResourcePtr _resource);

		ShadingProgramPtr getByName(const std::string& _name, const std::string& _group = DEFAULT_RESOURCE_GROUP);
		ShadingProgramPtr getByHandle(ResourceHandle _handle);

		void parseResourceScript(DataStreamPtr _script);
	};
}



