#pragma once

#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <GLES2/gl2.h>

#include "Exceptions.h"
#include "Logger.h"
#include "Matrix3.h"
#include "Vector2.h"
#include "Vector3.h"

//#include "ShadingProgram.h"
//#include "ShadingParamsPassthru.h"


namespace core
{
	enum UNIFORM_USAGE
	{
		UU_UNKNOWN = 0x00,

		UU_2D_WORLD_MATRIX = 0x01,
		UU_2D_VIEW_MATRIX = 0x02,
		UU_2D_PROJ_MATRIX = 0x03,

		UU_2D_WORLDVIEW_MATRIX = 0x04,
		UU_2D_VIEWPROJ_MATRIX = 0x05,

		UU_2D_WORLDVIEWPROJ_MATRIX = 0x06,

		UU_SAMPLER_0 = 0x07,
		UU_SAMPLER_1 = 0x08,
		UU_SAMPLER_2 = 0x09,
		UU_SAMPLER_3 = 0x0A,
		UU_SAMPLER_4 = 0x0B,
		UU_SAMPLER_5 = 0x0C,
		UU_SAMPLER_6 = 0x0D,
		UU_SAMPLER_7 = 0x0E,

		UU_AMBIENT_LIGHT_COLOUR = 0x0F,

		UU_CUSTOM = 0x10,
		UU_TIME = 0x11,

		//		UU_LIGHT_POSITION = 0x12,
	};

	struct Uniform
	{
		UNIFORM_USAGE usage;
		std::string name;
		GLint id;
		GLint size;
		GLenum type;

		Uniform(UNIFORM_USAGE _usage, const std::string &_name, GLint _id, GLint _size, GLenum _type);
	};


	class ShadingProgram;
	class ShadingParamsPassthru;


	class ShadingProgramParams
	{
	public:
		typedef std::vector<unsigned int> TextureUnitList;

	protected:

		ShadingProgram *program;

		typedef std::vector<Uniform> UniformList;

		UniformList programUniforms;
		TextureUnitList usedTextureUnits;
		unsigned int vertexSize;


	public:

		void addUniformDefinition(const std::string &_name, GLint _id, GLint _size, GLenum _type);
		void applyUniformValues(const ShadingParamsPassthru* paramsps) const;
		const TextureUnitList &getTextureUnitsUsed();


	protected:
		typedef std::unordered_map<std::string, UNIFORM_USAGE> UniformNameMap;
		static const UniformNameMap namedUniformList;

	public:
		static UNIFORM_USAGE getUsage(const std::string &_name);


	};

}
