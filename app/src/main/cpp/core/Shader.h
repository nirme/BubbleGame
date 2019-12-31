#pragma once

#include <memory>
#include <vector>
#include <GLES2/gl2.h>

#include "Exceptions.h"
#include "Logger.h"

#include "Resource.h"
#include "ResourceManager.h"


namespace core
{
	class RenderSystem;


	enum SHADER_TYPE : unsigned short
	{
		ST_VERTEX = 0x00,
		ST_FRAGMENT = 0x01,
		ST_UNKNOWN = 0xFF,
	};

	class Shader : public Resource
	{
	protected:

		RenderSystem *renderer;

		GLuint id;
		GLenum type;

	public:

		Shader(const std::string &_name, ResourceHandle _handle, const std::string &_group, RenderSystem *_renderer, ResourceManager *_manager = nullptr);

		~Shader();

		SHADER_TYPE getType();
		void setType(SHADER_TYPE _type);

		void loadImp();
		void unloadImp();

		unsigned int sizeCalcImpl();

		inline GLuint getId() const { return id; };
	};

	typedef std::shared_ptr<Shader> ShaderPtr;

}