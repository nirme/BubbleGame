#pragma once

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <unordered_set>
#include <cmath>
#include <limits>
#include "Exceptions.h"
#include "Logger.h"

namespace core
{

	class RenderStateCashe
	{
	protected:
	public:
		enum RENDERER_STATE : unsigned int
		{
			RS_BLENDING,
			RS_BLENDING_FUNCTION,
			RS_BLENDING_EQUATION,

			RS_FACE_CULLING,
			RS_FACE_CULLING_MODE,

			RS_DEPTH_TEST,
			RS_DEPTH_TEST_FUNCTION,
			RS_DEPTH_TEST_RANGE,

			RS_DITHER,

			RS_SCISSOR_TEST,
			RS_SCISSOR_TEST_RECTANGLE,

			//RS_STENCIL_TEST,
			//RS_SAMPLE_ALPHA_TO_COVERAGE,
			//RS_SAMPLE_COVERAGE,

			RS_PROGRAM,

			RS_VERTEX_ATRIBS, 

			RS_TEXTURE_BINDING,

			RS_VERTEX_BUFFER_BINDING,
			RS_INDEX_BUFFER_BINDING,

			//RS_,
		};

		struct RendererState
		{
			bool blending; //GL_BLEND
			GLenum blendingSfactor; //GL_ONE_MINUS_DST_ALPHA
				//GL_ONE, GL_ZERO, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE
			GLenum blendingDfactor; //GL_DST_ALPHA
				//GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA.GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA
			GLenum blendingMode; //GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT

			bool faceCulling; //GL_CULL_FACE
			GLenum faceCullingMode; //GL_BACK, GL_FRONT, GL_FRONT_AND_BACK

			bool depthTest; //GL_DEPTH_TEST
			GLenum depthTestFunction; //GL_LESS, GL_NEVER, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS
			GLclampf depthTestNearVal; //0.0f
			GLclampf depthTestFarVal; //1.0f

			bool dither; //GL_DITHER

			bool scissorTest; //GL_SCISSOR_TEST
			GLint scissorTestX; //0
			GLint scissorTestY; //0
			GLsizei scissorTestWidth; //screenWidth
			GLsizei scissorTestHeight; //screenHeight

			// unimplemented
			/*
			//bool stencilTest; //GL_STENCIL_TEST
			//GLenum stencilTestFunc; //GL_ALWAYS, GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL
			//GLint stencilTestRef; //clamp value range[0, 2n-1]
			//GLuint stencilTestMask; //mask ended with values in stencil, default: 0xFFFFFFFF
			//GLenum stencilTestSfail; //GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT
			//GLenum stencilTestDpfail; //GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT
			//GLenum stencilTestDppass; //GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT

			//GLboolean sampleAlphaToCoverage; //GL_SAMPLE_ALPHA_TO_COVERAGE

			//GLboolean sampleCoverage; //GL_SAMPLE_COVERAGE
			//GLclampf value; //1.0
			//GLboolean invert; //GL_FALSE, GL_TRUE
			*/

			
			GLuint shadingProgram;
			unsigned int activeVertexAttribs;

			GLuint texture2D[8];

			GLuint vertexBuffer;
			GLuint indexBuffer;
		};


		typedef std::unordered_set<RENDERER_STATE> RenderStateChange;

		RendererState defaultRenderState;
		RendererState currentRenderState;
		RendererState newRenderState;

        RenderStateChange requiredStateChanges;


	public:

		RenderStateCashe() :
			defaultRenderState(),
			currentRenderState(),
			newRenderState(),
            requiredStateChanges()
		{};

        ~RenderStateCashe()
        {};

		void getCurrentState();

		void setDefaultBlending(bool _blending, GLenum _blendingSfactor = 0, GLenum _blendingDfactor = 0, GLenum _blendingMode = 0);
		void setDefaultFaceCulling(bool _faceCulling, GLenum _faceCullingMode = 0);
		void setDefaultDepthTest(bool _depthTest, GLenum _depthTestFunction = 0, GLclampf _depthTestNearVal = HUGE_VALF, GLclampf _depthTestFarVal = HUGE_VALF);
		void setDefaultDither(bool _dither);
		void setDefaultScissorTest(bool _scissorTest, GLint _scissorTestX = -1, GLint _scissorTestY = -1, GLsizei _scissorTestWidth = -1, GLsizei _scissorTestHeight = -1);
		void setDefaultTextures(unsigned int _textureId);

		void resetToDefault();

		void setBlending(bool _blending, GLenum _blendingSfactor = 0, GLenum _blendingDfactor = 0, GLenum _blendingMode = 0);
		void setFaceCulling(bool _faceCulling, GLenum _faceCullingMode = 0);
		void setDepthTest(bool _depthTest, GLenum _depthTestFunction = 0, GLclampf _depthTestNearVal = HUGE_VALF, GLclampf _depthTestFarVal = HUGE_VALF);
		void setDither(bool _dither);
		void setScissorTest(bool _scissorTest, GLint _scissorTestX = -1, GLint _scissorTestY = -1, GLsizei _scissorTestWidth = -1, GLsizei _scissorTestHeight = -1);
		void setShadingProgram(GLuint _shadingProgram);
		void setVertexAtrib(unsigned int _activeVertexAttribs);
		void setActiveTextures(unsigned int _textureCount, GLuint _textures[] = 0);
		void setActiveTexture(unsigned int _textureIndex, GLuint _textureId);


		void setVertexBuffer(GLuint _vertexBufferId)
		{
			assert(_vertexBufferId >= 0 && "incorrect vertex buffer id");

			newRenderState.vertexBuffer = _vertexBufferId;
			if (newRenderState.vertexBuffer != currentRenderState.vertexBuffer)
				requiredStateChanges.insert(RS_VERTEX_BUFFER_BINDING);
			else
				requiredStateChanges.erase(RS_VERTEX_BUFFER_BINDING);
		};

		void setIndexBuffer(GLuint _indexBufferId)
		{
			assert(_indexBufferId >= 0 && "incorrect vertex buffer id");

			newRenderState.indexBuffer = _indexBufferId;
			if (newRenderState.indexBuffer != currentRenderState.indexBuffer)
				requiredStateChanges.insert(RS_INDEX_BUFFER_BINDING);
			else
				requiredStateChanges.erase(RS_INDEX_BUFFER_BINDING);
		};


		void immediateSetTexture(GLuint _textureId, unsigned int _textureIndex = 0)
		{
			GL_ERROR_CHECK(glActiveTexture(GL_TEXTURE0 + _textureIndex));
			GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, _textureId));

			currentRenderState.texture2D[_textureIndex] = _textureId;
		};

		void immediateSetTextureParami(GLenum _paramName, GLint _param)
		{
			GL_ERROR_CHECK(glTexParameteri(GL_TEXTURE_2D, _paramName, _param));
		};


		void immediateSetBuffer(GLuint _bufferId, GLenum _bufferType)
		{

			if (_bufferType == GL_ARRAY_BUFFER && _bufferId != currentRenderState.vertexBuffer)
			{
				GL_ERROR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _bufferId));
				currentRenderState.vertexBuffer = _bufferId;
			}
			else if (_bufferType == GL_ELEMENT_ARRAY_BUFFER && _bufferId != currentRenderState.indexBuffer)
			{
				GL_ERROR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId));
				currentRenderState.indexBuffer = _bufferId;
			}
		};





		void applyState();
	};
}
