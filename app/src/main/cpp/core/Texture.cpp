#include "Texture.h"
#include "RenderSystem.h"


namespace core
{

	Texture::Texture(const std::string &_name, ResourceHandle _handle, const std::string &_group, RenderSystem *_renderer, ResourceManager *_manager) :
		Resource(_name, _handle, _group, _manager),
		renderer(_renderer), 
		id((GLuint)0),
		filter(GL_NEAREST)
	{};

	Texture::~Texture()
	{};


	void Texture::setType(TEXTURE_TYPE _type)
	{
		switch (_type)
		{
		case TT_TEXTURE_2D:
		default:
			type = GL_TEXTURE_2D;
		}
	};


	void Texture::setFilter(TEXTURE_FILTER _filter)
	{
		switch (_filter)
		{
		case TF_LINEAR:
			filter = GL_LINEAR;
			break;

		case TF_NEAREST:
		default:
			filter = GL_NEAREST;
		}
	};


	void Texture::loadImp()
	{
		assert(renderer && renderer->hasContext() && "No renderer/context to create graphic resource" );

		DataStreamPtr data = manager->openResource(getName());
		ImagePtr texImage(new Image());

		texImage->read(data);
		texImage->convert(IF_RAW, PF_UNDEFINED, PS_BYTE_PER_COMPONENT);

		originalWidth = texImage->getOriginalWidth();
		originalHeight = texImage->getOriginalHeight();
		width = texImage->getWidth();
		height = texImage->getHeight();
		bitDepth = texImage->getBitDepth();

		texelFormat = texImage->getHWTexelFormat();
		texelStruct = texImage->getHWTexelStructure();


		id = (GLuint)0;

		try
		{
			GL_ERROR_CHECK(glGenTextures(1, &id));

			renderer->getStateCashe()->immediateSetTexture(id);

			renderer->getStateCashe()->immediateSetTextureParami(GL_TEXTURE_MIN_FILTER, filter);
			renderer->getStateCashe()->immediateSetTextureParami(GL_TEXTURE_MAG_FILTER, filter);

			renderer->getStateCashe()->immediateSetTextureParami(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			renderer->getStateCashe()->immediateSetTextureParami(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


			GL_ERROR_CHECK(glTexImage2D(GL_TEXTURE_2D,
				0,
				texelFormat,
				width,
				height,
				0,
				texelFormat,
				texelStruct,
				texImage->getDataPtr()));
		}
		catch (const std::exception &e)
		{
			if (id != (GLuint)0)
			{
				glDeleteTextures(1, &id);
				id = (GLuint)0;
				glGetError();
			}

			Logger::getSingleton().write(e.what(), LL_ERROR);
			throw;
		}
	};


	void Texture::unloadImp()
	{
		glDeleteTextures(1, &id);
		id = (GLuint)0;
		glGetError();
	};

	unsigned int Texture::sizeCalcImpl()
	{
		unsigned int s = sizeof(id);
		s += sizeof(filter);

		s += sizeof(width);
		s += sizeof(height);
		s += sizeof(bitDepth);

		s += sizeof(texelFormat);
		s += sizeof(texelStruct);

		if (state == RS_LOADED)
			s += width * height * bitDepth / 8;

		return s;
	};


}