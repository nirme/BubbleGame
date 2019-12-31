#include "ShadingProgramParams.h"
#include "ShadingParamsPassthru.h"


namespace core
{

    Uniform::Uniform(UNIFORM_USAGE _usage, const std::string &_name, GLint _id, GLint _size, GLenum _type) :
            usage(_usage),
            name(_name),
            id(_id),
            size(_size),
            type(_type)
    {};



    void ShadingProgramParams::addUniformDefinition(const std::string &_name, GLint _id, GLint _size, GLenum _type)
    {
        programUniforms.emplace_back(getUsage(_name), _name, _id, _size, _type);
    };


    void ShadingProgramParams::applyUniformValues(const ShadingParamsPassthru* paramsps) const
    {
        try
        {

            for (auto it = programUniforms.begin(); it != programUniforms.end(); ++it)
            {
                switch ((*it).usage)
                {
                    case UU_2D_WORLD_MATRIX:
                    {
                        GL_ERROR_CHECK(glUniformMatrix3fv((*it).id, 1, GL_FALSE, paramsps->get2dWorldMatrix().m));
                        break;
                    }

                    case UU_2D_VIEW_MATRIX:
                    {
                        GL_ERROR_CHECK(glUniformMatrix3fv((*it).id, 1, GL_FALSE, paramsps->get2dViewMatrix().m));
                        break;
                    }

                    case UU_2D_PROJ_MATRIX:
                    {

                        GL_ERROR_CHECK(glUniformMatrix3fv((*it).id, 1, GL_FALSE, paramsps->get2dProjectionMatrix().m));
                        break;
                    }

                    case UU_2D_WORLDVIEW_MATRIX:
                    {
                        GL_ERROR_CHECK(glUniformMatrix3fv((*it).id, 1, GL_FALSE, paramsps->get2dWorldViewMatrix().m));
                        break;
                    }

                    case UU_2D_VIEWPROJ_MATRIX:
                    {
                        GL_ERROR_CHECK(glUniformMatrix3fv((*it).id, 1, GL_FALSE, paramsps->get2dViewProjectionMatrix().m));
                        break;
                    }

                    case UU_2D_WORLDVIEWPROJ_MATRIX:
                    {
                        GL_ERROR_CHECK(glUniformMatrix3fv((*it).id, 1, GL_FALSE, paramsps->get2dWorldViewProjectionMatrix().m));
                        break;
                    }

                    case UU_SAMPLER_0:
                    case UU_SAMPLER_1:
                    case UU_SAMPLER_2:
                    case UU_SAMPLER_3:
                    case UU_SAMPLER_4:
                    case UU_SAMPLER_5:
                    case UU_SAMPLER_6:
                    case UU_SAMPLER_7:
                    {
                        GLint unit = (*it).usage - UU_SAMPLER_0;
                        GL_ERROR_CHECK(glUniform1i( (*it).id, unit));  //set sampler index


                        // TEXTURE UNITS SHOULD BE BINDED BEFORE BY RENDERER STATE CASHE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                        //glActiveTexture(GL_TEXTURE0 + unit); //set texture for used sampler
                        //glBindTexture(GL_TEXTURE_2D, paramsps->getTextureId(unit));

                        break;
                    }


                    case UU_AMBIENT_LIGHT_COLOUR:
                    {
                        GL_ERROR_CHECK(glUniform3fv((*it).id, 1, paramsps->getAmbientLight().v));
                        break;
                    }

                    case UU_TIME:
                    {
                        GL_ERROR_CHECK(glUniform1f((*it).id, (GLfloat) paramsps->getTimeElapsed()));
                        break;
                    }

                    case UU_CUSTOM:
                    {
                        // add custom params
                        break;
                    }

                    default:
                        break;

                }
            }
        }
        catch (const std::exception &e)
        {
            Logger::getSingleton().write(e.what(), LL_ERROR);
        }
    };


	const ShadingProgramParams::TextureUnitList &ShadingProgramParams::getTextureUnitsUsed()
	{
		return usedTextureUnits;
	};


    UNIFORM_USAGE ShadingProgramParams::getUsage(const std::string &_name)
    {
        auto it = namedUniformList.find(_name);
        return it != namedUniformList.end() ? (*it).second : UNIFORM_USAGE::UU_CUSTOM;
    };


    const ShadingProgramParams::UniformNameMap ShadingProgramParams::namedUniformList = ShadingProgramParams::UniformNameMap(
    {
            { "worldMx3", UNIFORM_USAGE::UU_2D_WORLD_MATRIX },
            { "viewMx3", UNIFORM_USAGE::UU_2D_VIEW_MATRIX},
            { "projMx3", UNIFORM_USAGE::UU_2D_PROJ_MATRIX},
            { "worlViewdMx3", UNIFORM_USAGE::UU_2D_WORLDVIEW_MATRIX},
            { "viewProjMx3", UNIFORM_USAGE::UU_2D_VIEWPROJ_MATRIX},
            { "worldViewProjMx3", UNIFORM_USAGE::UU_2D_WORLDVIEWPROJ_MATRIX},
            { "texture_0", UNIFORM_USAGE::UU_SAMPLER_0},
            { "texture_1", UNIFORM_USAGE::UU_SAMPLER_1},
            { "texture_2", UNIFORM_USAGE::UU_SAMPLER_2},
            { "texture_3", UNIFORM_USAGE::UU_SAMPLER_3},
            { "texture_4", UNIFORM_USAGE::UU_SAMPLER_4},
            { "texture_5", UNIFORM_USAGE::UU_SAMPLER_5},
            { "texture_6", UNIFORM_USAGE::UU_SAMPLER_6},
            { "texture_7", UNIFORM_USAGE::UU_SAMPLER_7},
            { "ambientColor", UNIFORM_USAGE::UU_AMBIENT_LIGHT_COLOUR},
            { "time", UNIFORM_USAGE::UU_TIME}
    });

}
