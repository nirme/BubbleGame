#include "ShadingProgram.h"
#include "RenderSystem.h"
#include "ScriptLoader.h"


namespace core
{

	ShadingProgram::ShadingProgram(const std::string &_name, ResourceHandle _handle, const std::string &_group, RenderSystem *_renderer, ResourceManager *_manager) :
		Resource(_name, _handle, _group, _manager), 
		renderer(_renderer),
		id(0)
	{};

	ShadingProgram::~ShadingProgram()
	{};

	void ShadingProgram::loadImp()
	{
		assert(renderer && renderer->hasContext() && "No renderer/context to create graphic resource");

		{
			DataStreamPtr data = manager->openResource(ShadingProgram::ProgramDefinitionFile);
			ScriptLoader &sloader = ScriptLoader::getSingleton();
			ScriptNodeListPtr programDataList = sloader.parse(data);

			ScriptNodePtr programData = sloader.findProgramNode(programDataList, getName());
			programDataList = programData->getChildList();

			for (auto it = programDataList->begin(); it != programDataList->end(); ++it)
			{
				std::string nodeType = (*it)->getName();
				if (nodeType.compare("shader") == 0)
				{
					std::string shaderName = sloader.parseShaderName(*it);
					ShaderPtr shader = ShaderManager::getSingleton().getByName(shaderName, getGroup());

					SHADER_TYPE type = shader->getType();
					if (type == ST_UNKNOWN)
						continue;

					shaders[type] = shader;
					shaders[type]->load();
				}
			}
		}

		paramsList = std::unique_ptr<ShadingProgramParams>(new ShadingProgramParams());

		GLint linkStatus;

		try
		{
			if (!shaders[ST_VERTEX] || !shaders[ST_VERTEX]->isLoaded())
				throw std::runtime_error("shader of type ST_VERTEX not declared per program");

			if (!shaders[ST_FRAGMENT] || !shaders[ST_FRAGMENT]->isLoaded())
				throw std::runtime_error("shader of type ST_FRAGMENT not declared per program");


			GLuint tempId = 0;
			GL_ERROR_CHECK(tempId = glCreateProgram());
			id = tempId;

			GL_ERROR_CHECK(glAttachShader(id, shaders[ST_VERTEX]->getId()));
			GL_ERROR_CHECK(glAttachShader(id, shaders[ST_FRAGMENT]->getId()));

			GL_ERROR_CHECK(glLinkProgram(id));

			GL_ERROR_CHECK(glGetProgramiv(id, GL_LINK_STATUS, &linkStatus));
			if (linkStatus != GL_TRUE)
				throw std::runtime_error("glLinkProgram function failed");


			unsigned int vertexSize(0);

			for (unsigned int index = 0; index < vertexAttribNames.size(); ++index)
			{
				GLint attribId(0);
				GL_ERROR_CHECK(attribId = glGetAttribLocation(id, vertexAttribNames[index]));

				if (attribId >= 0)
				{
					if (vertexAttribs.size() <= index)
						vertexAttribs.resize(index+1);

					vertexAttribs[index].id = attribId;
					GL_ERROR_CHECK(glGetActiveAttrib(id, index, 0, nullptr, &(vertexAttribs[index].size), &(vertexAttribs[index].type), nullptr));

					// offset
					vertexAttribs[index].offsetInBytes = vertexSize;

					unsigned int typeSize = GLTypeSize(vertexAttribs[index].type);
					vertexSize += vertexAttribs[index].size * typeSize;

					vertexAttribs[index].type = GL_FLOAT;
					vertexAttribs[index].size = typeSize/sizeof(GLfloat);
				}
			}
			vertexAttribs.shrink_to_fit();

			for (unsigned int index = 0; index < vertexAttribs.size(); ++index)
				vertexAttribs[index].stride = vertexSize;




			GLint uniformCount(0), uniformNameLen(0);
			GL_ERROR_CHECK(glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniformCount));

			GL_ERROR_CHECK(glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformNameLen));
			std::vector<char> uniformNameContainer(uniformNameLen+1);

			std::string uniformName;
			GLint uniformId;
			GLint uniformSize;
			GLenum uniformType;


			for (GLuint index = 0; index < uniformCount; ++index)
			{
				GL_ERROR_CHECK(glGetActiveUniform(id, index, uniformNameContainer.size(), nullptr, &uniformSize, &uniformType, uniformNameContainer.data()));
				uniformName = std::string(uniformNameContainer.data());
				GL_ERROR_CHECK(uniformId = glGetUniformLocation(id, uniformName.c_str()));

				paramsList->addUniformDefinition(uniformName, uniformId, uniformSize, uniformType);
			}

		}
		catch (const std::exception &e)
		{
			Logger::getSingleton().write(e.what(), LL_ERROR);

			if (linkStatus != GL_TRUE)
			{
				GLint logLen;
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLen);
				glGetError();

				std::string infoLog(logLen, '\0');
				glGetProgramInfoLog(id, logLen, nullptr, (&infoLog[0]));
				glGetError();

				Logger::getSingleton().write(infoLog, LL_DEBUG);
			}

			if (id)
			{
				glDeleteProgram(id);
				id = (GLuint)0;
				glGetError();
			}

			shaders[ST_VERTEX].reset();
			shaders[ST_FRAGMENT].reset();

			vertexAttribs.clear();
			vertexAttribs.shrink_to_fit();
			paramsList.reset();

			throw;
		}

	};


	void ShadingProgram::unloadImp()
	{
		if (id)
		{
			glDeleteProgram(id);
			id = (GLuint)0;
			glGetError();
		}

		shaders[ST_VERTEX].reset();
		shaders[ST_FRAGMENT].reset();

		vertexAttribs.clear();
		vertexAttribs.shrink_to_fit();
		paramsList.reset();
	};

	unsigned int ShadingProgram::sizeCalcImpl()
	{
		unsigned int s = sizeof(id);

		s += sizeof(shaders[ST_VERTEX]);
		s += sizeof(shaders[ST_FRAGMENT]);

		s += sizeof(id);

		s += sizeof(vertexAttribs);
		s += vertexAttribs.size() * sizeof(Attrib);

		//typedef std::map<std::string, Uniform> UniformNamedMap;
		s += sizeof(paramsList);
		s += sizeof(ShadingProgramParams);

		return s;
	};

}
