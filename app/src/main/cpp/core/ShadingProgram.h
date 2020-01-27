#pragma once

#include <memory>
#include <array>
#include <map>
#include <unordered_map>
#include <GLES2/gl2.h>

#include "Exceptions.h"
#include "Logger.h"

#include "Resource.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "ShadingProgramParams.h"
#include "GLutils.h"


namespace core
{
	class RenderSystem;


	struct Attrib
	{
		GLint id;
		GLint size;
		GLenum type;
		GLsizei stride;
		unsigned int offsetInBytes;


		Attrib(GLint _id = -1, GLint _size = 0, GLenum _type = 0, GLsizei _stride = 0, unsigned int _offsetInBytes = 0) :
			id(_id), size(_size), type(_type), 
			stride(_stride),
			offsetInBytes(_offsetInBytes)
		{};


	};


	enum VERTEX_ATTRIB
	{
		VA_POSITION = 0x00,
		VA_TEXTURE1 = 0x01,
		VA_TEXTURE2 = 0x02,
		VA_TEXTURE3 = 0x03,
		VA_TEXTURE4 = 0x04,
		VA_TEXTURE5 = 0x05,
		VA_TEXTURE6 = 0x06,
		VA_TEXTURE7 = 0x07,
		VA_TEXTURE8 = 0x08,

		VA_ALPHA = 0x09,
		VA_AGE = 0x0A,

		VA_ENUM_COUNT = 0X0B, // do not use
	};



	class ShadingProgram : public Resource
	{
	public:
		typedef std::vector<Attrib> VertexAttribList;

		// !!!need replacing from resource system loader!!!
		static constexpr char *ProgramDefinitionFile = "ShadingPrograms.xml";

	protected:

		static constexpr std::array<const char*, VA_ENUM_COUNT> vertexAttribNames{ { "position", "tex1", "tex2", "tex3", "tex4", "tex5", "tex6", "tex7", "tex8", "alpha", "age"} };

		RenderSystem *renderer;

		std::array<ShaderPtr, 2> shaders;

		GLuint id;


		//std::array<Attrib, VA_ENUM_COUNT> vertexAttribs;
		VertexAttribList vertexAttribs;

		unsigned int vertexSize;

		std::unique_ptr<ShadingProgramParams> paramsList;



	public:

		ShadingProgram(const std::string &_name, ResourceHandle _handle, const std::string &_group, RenderSystem *_renderer, ResourceManager *_manager = nullptr);
		~ShadingProgram();

		void loadImp();
		void unloadImp();

		unsigned int sizeCalcImpl();

		inline const Attrib& getAttrib(VERTEX_ATTRIB _attrib)
		{
			return vertexAttribs[_attrib];
		};

		inline GLint getId() const
		{
			return id;
		};

		const VertexAttribList& getAttribList() const
		{
			return vertexAttribs;
		};

		inline unsigned int getVertexSize() const
		{
			return vertexSize;
		};

		inline const ShadingProgramParams *getParams() const
		{
			return paramsList.get();
		};

	};


	typedef std::shared_ptr<ShadingProgram> ShadingProgramPtr;
}
